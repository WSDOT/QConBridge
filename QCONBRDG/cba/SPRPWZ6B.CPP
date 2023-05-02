///////////////////////////////////////////////////////////////////////
// QConBridge - HL93 Live Load Analysis
// Copyright (C) 1999  Washington State Department of Transportation
//                     Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation, 
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but 
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source 
// License along with this program; if not, write to the Washington 
// State Department of Transportation, Bridge and Structures Office, 
// 4500 3rd AVE SE - P.O. Box  47340, Olympia, WA 98503, USA or e-mail 
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Revision Log
// ============
// May 1997 - Created, Richard Brice, PE
// Oct 1999 - Released as Open Source



#include <qconbrdg\sprpwiz.h>
#include <qconbrdg\flprint.h>
#include <Context.hh>

#include <shape\genshape.h>
#include <shape\plategdr.h>
#include <material\genmat.h>
#include <code\lrfdcode.h>
#include <esl\strutil.h>
#include <code\lldist.h>
#include <esl\engnotat.h>
#include <owl\printer.h>
#include <owl\profile.h>
#include <dir.h> // For MAXPATH


DEFINE_RESPONSE_TABLE1(TSectPropWizard::Page6b,TWizardStep)
   EV_BN_CLICKED(IDC_PRINT,BNClicked_Print),
END_RESPONSE_TABLE;

TSectPropWizard::Page6b::Page6b(TSectPropWizard *wiz,TModule *module) :
TWizardStep(wiz,IDD_SECTPRPWIZARD_PAGE6B,"Properties and Distribution Factors",0,module),
wizard(wiz)
{
   PageInfo.dwFlags |= PSP_HASHELP;

    lldf = 0;
}

TSectPropWizard::Page6b::~Page6b()
{
   delete lldf;
}

void TSectPropWizard::Page6b::SetupWindow()
{
   TWizardStep::SetupWindow();
}

int
TSectPropWizard::Page6b::SetActive(TNotify far& nmHdr)
{
   int retval = TWizardStep::SetActive(nmHdr);

   switch(wizard->page1->GetBeamType())
      {
      case IDC_STEELBEAM:
           SteelBeam_ConcreteDeck();
           break;

      case IDC_PRECASTIBEAM:
           PrecastIBeam_ConcreteDeck();
           break;

      default:
           break;
      }

   return retval;
}

void
TSectPropWizard::Page6b::SteelBeam_ConcreteDeck()
{
   //
   // Get beam shape and write basic properties
   //
   TShape* beam = wizard->page3b->GetBeam();
   results.girderAx = beam->GetArea();
   results.girderIz = beam->GetCentroidalInertia().Ixx;
   results.girderCG = beam->GetYbar();

   // Get slab shape
   TCompSlab* slab = wizard->page4c->GetSlab();
   results.effSlabWidth = wizard->page4c->GetEffSlabWidth();
   results.slabModE = wizard->page4c->GetModE();

   // Build composite shape
   TSection section;
   section.AddShape(beam);
   section.AddShape(slab);
   section.ComputeProperties();
   results.cmpAx = section.GetArea();
   results.cmpIz = section.GetCentroidalInertia().Ixx;
   results.cmpCG = section.GetYbar();
   results.effUnitWgt = section.GetUnitWeight();
   results.cmpModE = section.GetModE();
   results.n = beam->GetModE()/slab->GetModE();

   double eg = Compute_eg(beam,slab);
   double Kg = ComputeKg(beam,slab,eg);
   results.eg = eg;
   results.Kg = Kg;

   // build dist factor object
   double spacing = DerivedUnits::Length(wizard->page3b->GetSpacing()).ConvertTo(SIConstants::millimeter).value();
   double de = DerivedUnits::Length(wizard->page4c->Getde()).ConvertTo(SIConstants::millimeter).value();
   double wLane = DerivedUnits::Length(wizard->page4c->GetDesignLaneWidth()).ConvertTo(SIConstants::millimeter).value();
   double spanLength = DerivedUnits::Length(wizard->page3b->GetSpanLength()).ConvertTo(SIConstants::millimeter).value();
   double tSlab = wizard->page4c->GetSlabDepth() - wizard->page4c->GetSacraficalDepth();
   tSlab = DerivedUnits::Length(tSlab).ConvertTo(SIConstants::millimeter).value();
   int    nBeams = wizard->page3b->GetNumBeams();
   int    nLanes = wizard->page3b->GetNumLanes();
   bool   xFrames = wizard->page3b->XFrames();
   double skewAngle = wizard->page5->GetSkewAngle();
   bool   correctMoment = wizard->page5->CorrectMoment();
   bool   correctShear = wizard->page5->CorrectShear();

   Kg = DerivedUnits::Length4(Kg).ConvertTo(SIConstants::millimeter4).value();

   delete lldf;
   lldf = new LLDF_TypeAEK(spacing,de,wLane,spanLength,tSlab,Kg,nBeams,nLanes,xFrames,skewAngle,correctMoment,correctShear);
   LLDF::Location location = (wizard->page3b->GetGirderLocation() == IDC_INTGIRDER) ? LLDF::IntGirder : LLDF::ExtGirder;

   results.s_1_gMoment = lldf->GetMomentDF(location,LLDF::OneLoadedLane,LLDF::Strength);
   results.s_2_gMoment = lldf->GetMomentDF(location,LLDF::TwoOrMoreLoadedLanes,LLDF::Strength);
   results.s_1_gShear  = lldf->GetShearDF(location,LLDF::OneLoadedLane,LLDF::Strength);
   results.s_2_gShear  = lldf->GetShearDF(location,LLDF::TwoOrMoreLoadedLanes,LLDF::Strength);
   results.f_gMoment   = lldf->GetMomentDF(location,LLDF::OneLoadedLane,LLDF::Fatigue);
   results.f_gShear    = lldf->GetShearDF(location,LLDF::OneLoadedLane,LLDF::Fatigue);

   wizard->data.spanLength = wizard->page3b->GetSpanLength();
   wizard->data.ax = results.cmpAx;
   wizard->data.iz = results.cmpIz;
   wizard->data.modE = results.cmpModE;
   wizard->data.unitWeight = results.effUnitWgt;
   wizard->data.s_gMoment = max(results.s_1_gMoment,results.s_2_gMoment);
   wizard->data.s_gShear = max(results.s_1_gShear,results.s_2_gShear);
   wizard->data.f_gMoment = results.f_gMoment;
   wizard->data.f_gShear = results.f_gShear;

   ResultsToDlg();
}

void
TSectPropWizard::Page6b::PrecastIBeam_ConcreteDeck()
{
   //
   // Get beam shape and write basic properties
   //
   TShape* beam = wizard->page3c->GetBeam();
   results.girderAx = beam->GetArea();
   results.girderIz = beam->GetCentroidalInertia().Ixx;
   results.girderCG = beam->GetYbar();

   // Get slab shape
   TCompSlab* slab = wizard->page4c->GetSlab();
   results.effSlabWidth = wizard->page4c->GetEffSlabWidth();
   results.slabModE = wizard->page4c->GetModE();

   // Build composite shape
   TSection section;
   section.AddShape(beam);
   section.AddShape(slab);
   section.ComputeProperties();
   results.cmpAx = section.GetArea();
   results.cmpIz = section.GetCentroidalInertia().Ixx;
   results.cmpCG = section.GetYbar();
   results.effUnitWgt = section.GetUnitWeight();
   results.cmpModE = section.GetModE();
   results.n = beam->GetModE()/slab->GetModE();

   double eg = Compute_eg(beam,slab);
   double Kg = ComputeKg(beam,slab,eg);
   results.eg = eg;
   results.Kg = Kg;

   // build dist factor object
   double spacing = DerivedUnits::Length(wizard->page3c->GetSpacing()).ConvertTo(SIConstants::millimeter).value();
   double de = DerivedUnits::Length(wizard->page4c->Getde()).ConvertTo(SIConstants::millimeter).value();
   double wLane = DerivedUnits::Length(wizard->page4c->GetDesignLaneWidth()).ConvertTo(SIConstants::millimeter).value();
   double spanLength = DerivedUnits::Length(wizard->page3c->GetSpanLength()).ConvertTo(SIConstants::millimeter).value();
   double tSlab = wizard->page4c->GetSlabDepth() - wizard->page4c->GetSacraficalDepth();
   tSlab = DerivedUnits::Length(tSlab).ConvertTo(SIConstants::millimeter).value();
   int    nBeams = wizard->page3c->GetNumBeams();
   int    nLanes = wizard->page3c->GetNumLanes();
   bool   xFrames = wizard->page3c->XFrames();
   double skewAngle = wizard->page5->GetSkewAngle();
   bool   correctMoment = wizard->page5->CorrectMoment();
   bool   correctShear = wizard->page5->CorrectShear();

   Kg = DerivedUnits::Length4(results.Kg).ConvertTo(SIConstants::millimeter4).value();

   delete lldf;
   lldf = new LLDF_TypeAEK(spacing,de,wLane,spanLength,tSlab,Kg,nBeams,nLanes,xFrames,skewAngle,correctMoment,correctShear);
   LLDF::Location location = (wizard->page3c->GetGirderLocation() == IDC_INTGIRDER) ? LLDF::IntGirder : LLDF::ExtGirder;

   results.s_1_gMoment = lldf->GetMomentDF(location,LLDF::OneLoadedLane,LLDF::Strength);
   results.s_2_gMoment = lldf->GetMomentDF(location,LLDF::TwoOrMoreLoadedLanes,LLDF::Strength);
   results.s_1_gShear  = lldf->GetShearDF(location,LLDF::OneLoadedLane,LLDF::Strength);
   results.s_2_gShear  = lldf->GetShearDF(location,LLDF::TwoOrMoreLoadedLanes,LLDF::Strength);
   results.f_gMoment   = lldf->GetMomentDF(location,LLDF::OneLoadedLane,LLDF::Fatigue);
   results.f_gShear    = lldf->GetShearDF(location,LLDF::OneLoadedLane,LLDF::Fatigue);

   wizard->data.spanLength = wizard->page3c->GetSpanLength();
   wizard->data.ax = results.cmpAx;
   wizard->data.iz = results.cmpIz;
   wizard->data.modE = results.cmpModE;
   wizard->data.unitWeight = results.effUnitWgt;
   wizard->data.s_gMoment = max(results.s_1_gMoment,results.s_2_gMoment);
   wizard->data.s_gShear = max(results.s_1_gShear,results.s_2_gShear);
   wizard->data.f_gMoment = results.f_gMoment;
   wizard->data.f_gShear = results.f_gShear;

   ResultsToDlg();
}

double
TSectPropWizard::Page6b::ComputeKg(TShape* beam,TCompSlab* slab,double eg)
{
   double Kg;
   Kg = LLDF_TypeAEKIJ::ComputeKg(beam->GetModE()/slab->GetModE(),
                                  beam->GetCentroidalInertia().Ixx,
                                  beam->GetArea(),
                                  eg);
   return Kg;
}

double
TSectPropWizard::Page6b::Compute_eg(TShape* beam,TCompSlab* slab)
{
   double h;
   double yBar;
   double tSlab;
   double sacDepth;
   double slabPad;
   double eg;
   TIBeam* ibeam;
   TPlateGirder* pltGirder;
   GeneralShape* genShape;

   ibeam = TYPESAFE_DOWNCAST(beam,TIBeam);
   genShape = TYPESAFE_DOWNCAST(beam,GeneralShape);
   pltGirder = TYPESAFE_DOWNCAST(beam,TPlateGirder);

   if (ibeam)
      h = ibeam->GetHeight();
   else if (genShape)
      h = genShape->GetHeight();
   else if (pltGirder)
      h = pltGirder->GetHeight();

   yBar = beam->GetYbar();

   tSlab = slab->GetTslab();
   sacDepth = slab->GetTwearing();
   slabPad = slab->GetSlabOffset() - tSlab;

   eg = h - yBar + slabPad + (tSlab - sacDepth)/2;

   return eg;
}

void
TSectPropWizard::Page6b::ResultsToDlg()
{
   double value;
   char buffer[50];

   // Noncomposite beam properties
   value = (wizard->data.dispUnits == unitsSI) ? DerivedUnits::Length2(results.girderAx).ConvertTo(SIConstants::millimeter2).value() :
                                                 DerivedUnits::Length2(results.girderAx).ConvertTo(USConstants::inch2).value();
   ostrstream(buffer,sizeof(buffer)) << "Ax = " << EngNotation(value,3) << " " << ((wizard->data.dispUnits == unitsSI) ? "mm^2" : "inch^2") << ends;
   SetDlgItemText(IDC_GIRDER_AX,buffer);

   value = (wizard->data.dispUnits == unitsSI) ? DerivedUnits::Length4(results.girderIz).ConvertTo(SIConstants::millimeter4).value() :
                                                 DerivedUnits::Length4(results.girderIz).ConvertTo(USConstants::inch4).value();
   ostrstream(buffer,sizeof(buffer)) << "Iz = " << EngNotation(value,3) << " " << ((wizard->data.dispUnits == unitsSI) ? "mm^4" : "inch^4") << ends;
   SetDlgItemText(IDC_GIRDER_IZ,buffer);

   value = (wizard->data.dispUnits == unitsSI) ? DerivedUnits::Length(results.girderCG).ConvertTo(SIConstants::millimeter).value() :
                                                 DerivedUnits::Length(results.girderCG).ConvertTo(USConstants::inch).value();
   ostrstream(buffer,sizeof(buffer)) << "CG = " << EngNotation(value,3) << " " << ((wizard->data.dispUnits == unitsSI) ? "mm" : "inch") << ends;
   SetDlgItemText(IDC_GIRDER_CG,buffer);

   // Slab properties
   value = (wizard->data.dispUnits == unitsSI) ? DerivedUnits::Length(results.effSlabWidth).ConvertTo(SIConstants::millimeter).value() :
                                                 DerivedUnits::Length(results.effSlabWidth).ConvertTo(USConstants::inch).value();
   ostrstream(buffer,sizeof(buffer)) << "Eff. Flange Width = " << EngNotation(value,3) << " " << ((wizard->data.dispUnits == unitsSI) ? "mm" : "inch") << ends;
   SetDlgItemText(IDC_EFFWIDTH,buffer);

   value = (wizard->data.dispUnits == unitsSI) ? DerivedUnits::Pressure(results.slabModE).ConvertTo(SIConstants::Pascal).value() :
                                                 DerivedUnits::Pressure(results.slabModE).ConvertTo(USConstants::pound_per_inch2).value();
   ostrstream(buffer,sizeof(buffer)) << "Mod. E = " << EngNotation(value,3) << " " << ((wizard->data.dispUnits == unitsSI) ? "Pa" : "psi") << ends;
   SetDlgItemText(IDC_ESLAB,buffer);

   // Composite Beam Properties
   value = (wizard->data.dispUnits == unitsSI) ? DerivedUnits::Length2(results.cmpAx).ConvertTo(SIConstants::millimeter2).value() :
                                                 DerivedUnits::Length2(results.cmpAx).ConvertTo(USConstants::inch2).value();
   ostrstream(buffer,sizeof(buffer)) << "Ax = " << EngNotation(value,3) << " " << ((wizard->data.dispUnits == unitsSI) ? "mm^2" : "inch^2") << ends;
   SetDlgItemText(IDC_AX_RESULT,buffer);

   value = (wizard->data.dispUnits == unitsSI) ? DerivedUnits::Length4(results.cmpIz).ConvertTo(SIConstants::millimeter4).value() :
                                                 DerivedUnits::Length4(results.cmpIz).ConvertTo(USConstants::inch4).value();
   ostrstream(buffer,sizeof(buffer)) << "Iz = " << EngNotation(value,3) << " " << ((wizard->data.dispUnits == unitsSI) ? "mm^4" : "inch^4") << ends;
   SetDlgItemText(IDC_IZ_RESULT,buffer);

   value = (wizard->data.dispUnits == unitsSI) ? DerivedUnits::Length(results.cmpCG).ConvertTo(SIConstants::millimeter).value() :
                                                 DerivedUnits::Length(results.cmpCG).ConvertTo(USConstants::inch).value();
   ostrstream(buffer,sizeof(buffer)) << "CG = " << EngNotation(value,3) << " " << ((wizard->data.dispUnits == unitsSI) ? "mm" : "inch") << ends;
   SetDlgItemText(IDC_CG_RESULT,buffer);

   value = (wizard->data.dispUnits == unitsSI) ? DerivedUnits::Density(results.effUnitWgt/SIConstants::g.value()).ConvertTo(SIConstants::kilogram_per_meter3).value() :
                                                 DerivedUnits::UnitWeight(results.effUnitWgt).ConvertTo(USConstants::pound_per_feet3).value();
   if (wizard->data.dispUnits == unitsSI)
      ostrstream(buffer,sizeof(buffer)) << "Eff. Density = " << EngNotation(value,3) << " " << "kg/m^3" << ends;
   else
      ostrstream(buffer,sizeof(buffer)) << "Unit Wgt = " << EngNotation(value,3) << " " << "lbs/ft^3" << ends;
   SetDlgItemText(IDC_UNITWEIGHT_RESULT,buffer);

   value = (wizard->data.dispUnits == unitsSI) ? DerivedUnits::Pressure(results.cmpModE).ConvertTo(SIConstants::Pascal).value() :
                                                 DerivedUnits::Pressure(results.cmpModE).ConvertTo(USConstants::pound_per_inch2).value();
   ostrstream(buffer,sizeof(buffer)) << "Mod. E = " << EngNotation(value,3) << " " << ((wizard->data.dispUnits == unitsSI) ? "Pa" : "psi") << ends;
   SetDlgItemText(IDC_MODE_RESULT,buffer);

   ostrstream(buffer,sizeof(buffer)) << "n = " << results.n << ends;
   SetDlgItemText(IDC_N,buffer);

   // eg
   value = (wizard->data.dispUnits == unitsSI) ? DerivedUnits::Length(results.eg).ConvertTo(SIConstants::millimeter).value() :
                                                 DerivedUnits::Length(results.eg).ConvertTo(USConstants::inch).value();
   ostrstream(buffer,sizeof(buffer)) << "eg = " << EngNotation(value,3) << " " << ((wizard->data.dispUnits == unitsSI) ? "mm" : "inch") << ends;
   SetDlgItemText(IDC_EG,buffer);

   // Kg
   value = (wizard->data.dispUnits == unitsSI) ? DerivedUnits::Length4(results.Kg).ConvertTo(SIConstants::millimeter4).value() :
                                                 DerivedUnits::Length4(results.Kg).ConvertTo(USConstants::inch4).value();
   ostrstream(buffer,sizeof(buffer)) << "Kg = " << EngNotation(value,3) << " " << ((wizard->data.dispUnits == unitsSI) ? "mm^4" : "inch^4") << ends;
   SetDlgItemText(IDC_KG,buffer);

   // Dist factors
   ostrstream(buffer,sizeof(buffer)) << "1 Loaded Lane = " << results.s_1_gMoment << ends;
   SetDlgItemText(IDC_S_GMOMENT_1_RESULT,buffer);

   ostrstream(buffer,sizeof(buffer)) << "2+ Loaded Lanes = " << results.s_2_gMoment << ends;
   SetDlgItemText(IDC_S_GMOMENT_2_RESULT,buffer);

   ostrstream(buffer,sizeof(buffer)) << "1 Loaded Lane = " << results.s_1_gShear << ends;
   SetDlgItemText(IDC_S_GSHEAR_1_RESULT,buffer);

   ostrstream(buffer,sizeof(buffer)) << "2+ Loaded Lanes = " << results.s_2_gShear << ends;
   SetDlgItemText(IDC_S_GSHEAR_2_RESULT,buffer);

   ostrstream(buffer,sizeof(buffer)) << "gMoment = " << results.f_gMoment << ends;
   SetDlgItemText(IDC_F_GMOMENT_RESULT,buffer);

   ostrstream(buffer,sizeof(buffer)) << "gShear = " << results.f_gShear << ends;
   SetDlgItemText(IDC_F_GSHEAR_RESULT,buffer);
}

void
TSectPropWizard::Page6b::BNClicked_Print()
{
   TPrinter printer;

   WriteReport();
   TFilePrintout printout(&printer,filename,"Report");
   printer.Print(Parent,printout,true);

   OFSTRUCT of;
   OpenFile(filename,&of,OF_DELETE);
}

void
TSectPropWizard::Page6b::WriteReport()
{
#if defined (__FLAT__)
   char buffer[MAXPATH];
   ::GetTempPath(sizeof(buffer),buffer);
   ::GetTempFileName(buffer,"spc",0,filename);
#else
   ::GetTempFileName(0,"spc",0,filename);
#endif

   ofstream ofile(filename);

   ofile << (*wizard->page1) << endl;
   ofile << (*wizard->page2) << endl;

   ofile << endl;
   ofile << "Supporting Component Description" << endl;
   switch(wizard->page1->GetBeamType())
      {
      case IDC_STEELBEAM:
           ofile << (*wizard->page3b) << endl;
           break;

      case IDC_PRECASTIBEAM:
           ofile << (*wizard->page3c) << endl;
           break;

      default:
           break;
      }


   ofile << endl;
   ofile << "Deck Description" << endl;
   ofile << (*wizard->page4c) << endl;

   ofile << endl;
   ofile << "Skew Corrections" << endl;
   ofile << (*wizard->page5) << endl;

   ofile << endl;
   ofile << (*this) << endl;
}

void
TSectPropWizard::Page6b::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Properties_and_Distribution_Factors);
}

