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


DEFINE_RESPONSE_TABLE1(TSectPropWizard::Page6a,TWizardStep)
   EV_BN_CLICKED(IDC_PRINT,BNClicked_Print),
END_RESPONSE_TABLE;

TSectPropWizard::Page6a::Page6a(TSectPropWizard *wiz,TModule *module) :
TWizardStep(wiz,IDD_SECTPRPWIZARD_PAGE6A,"Properties and Distribution Factors",0,module),
wizard(wiz)
{
   PageInfo.dwFlags |= PSP_HASHELP;

    lldf = 0;
}

TSectPropWizard::Page6a::~Page6a()
{
   delete lldf;
}

void TSectPropWizard::Page6a::SetupWindow()
{
   TWizardStep::SetupWindow();
}

int
TSectPropWizard::Page6a::SetActive(TNotify far& nmHdr)
{
   int retval = TWizardStep::SetActive(nmHdr);

   switch(wizard->page1->GetBeamType())
      {
      case IDC_STEELBEAM:
           switch(wizard->page2->GetDeckType())
              {
              case IDC_STEEL_GRID:
                   SteelBeam_GridDeck();
                   break;

              case IDC_STRESS_LAMINATED:
              case IDC_SPIKE_LAMINATED:
              case IDC_GLUE_LAMINATED:
                   SteelBeam_WoodDeck();
                   break;
              }
           break;

      case IDC_WOOD:
           switch(wizard->page2->GetDeckType())
              {
              case IDC_CONCRETE:
//                   WoodBeam_ConcDeck();
                   WoodBeam_Deck();
                   break;

              case IDC_WOOD_PLANK:
              case IDC_STRESS_LAMINATED:
              case IDC_SPIKE_LAMINATED:
              case IDC_GLUE_LAMINATED:
//                   WoodBeam_WoodDeck();
                   WoodBeam_Deck();
                   break;
              }
           break;

      default:
           break;
      }

   return retval;
}
/*
void
TSectPropWizard::Page6a::WoodBeam_ConcDeck()
{
   TSectPropWizard::Page3a* page3a = TYPESAFE_DOWNCAST(&wizard->GetPageRef(IDD_PAGE3A),TSectPropWizard::Page3a);
   TSectPropWizard::Page4c* page4c = TYPESAFE_DOWNCAST(&wizard->GetPageRef(IDD_PAGE4C),TSectPropWizard::Page4c);

   double effUnitWgt;
   double de;
   double S;
   double wLane;

   TShape* beam = page3a->GetBeam();
   results.girderAx = beam->GetArea();
   results.girderIz = beam->GetCentroidalInertia().Ixx;
   results.girderCG = beam->GetYbar();

   TCompSlab* slab = page4c->GetSlab();

   effUnitWgt = (slab->GetArea()*slab->GetUnitWeight() + beam->GetArea()*beam->GetUnitWeight())/beam->GetArea();

   results.effUnitWgt = effUnitWgt;
   results.modE = beam->GetModE();

   de = page4c->Getde();

   de = DerivedUnits::Length(de).ConvertTo(SIConstants::millimeter).value();

   S = DerivedUnits::Length(page3a->GetSpacing()).ConvertTo(SIConstants::millimeter).value();
   wLane = DerivedUnits::Length(page4c->GetDesignLaneWidth()).ConvertTo(SIConstants::millimeter).value();

   delete lldf;
   lldf = new LLDF_TypeL(S,de,wLane);

   LLDF::Location location = (page3a->GetGirderLocation() == IDC_INTGIRDER) ? LLDF::IntGirder : LLDF::ExtGirder;

   results.s_1_gMoment = lldf->GetMomentDF(location,LLDF::OneLoadedLane,LLDF::Strength);
   results.s_2_gMoment = lldf->GetMomentDF(location,LLDF::TwoOrMoreLoadedLanes,LLDF::Strength);
   results.s_1_gShear  = lldf->GetShearDF(location,LLDF::OneLoadedLane,LLDF::Strength);
   results.s_2_gShear  = lldf->GetShearDF(location,LLDF::TwoOrMoreLoadedLanes,LLDF::Strength);
   results.f_gMoment   = lldf->GetMomentDF(location,LLDF::OneLoadedLane,LLDF::Fatigue);
   results.f_gShear    = lldf->GetShearDF(location,LLDF::OneLoadedLane,LLDF::Fatigue);

   wizard->data.ax = results.girderAx;
   wizard->data.iz = results.girderIz;
   wizard->data.modE = results.modE;
   wizard->data.unitWeight = results.effUnitWgt;
   wizard->data.s_gMoment = max(results.s_1_gMoment,results.s_2_gMoment);
   wizard->data.s_gShear = max(results.s_1_gShear,results.s_2_gShear);
   wizard->data.f_gMoment = results.f_gMoment;
   wizard->data.f_gShear = results.f_gShear;

   ResultsToDlg();
}
*/
void
//TSectPropWizard::Page6a::WoodBeam_WoodDeck()
TSectPropWizard::Page6a::WoodBeam_Deck()
{
   double tribWidth;
   double depth;
   double unitWgt;
   double effUnitWgt;
   double de;
   double S;
   double wLane;

   TShape* beam = wizard->page3a->GetBeam();
   results.girderAx = beam->GetArea();
   results.girderIz = beam->GetCentroidalInertia().Ixx;
   results.girderCG = beam->GetYbar();

   tribWidth = wizard->page4a->GetTribWidth();
   depth = wizard->page4a->GetDepth();
   unitWgt = wizard->page4a->GetUnitWgt();
   de = wizard->page4a->Getde();
   double deckArea = tribWidth*depth;

   effUnitWgt = (deckArea*unitWgt + beam->GetArea()*beam->GetUnitWeight())/beam->GetArea();

   results.effUnitWgt = effUnitWgt;
   results.modE = beam->GetModE();

   de = DerivedUnits::Length(de).ConvertTo(SIConstants::millimeter).value();

   S = DerivedUnits::Length(wizard->page3a->GetSpacing()).ConvertTo(SIConstants::millimeter).value();
   wLane = DerivedUnits::Length(wizard->page4a->GetDesignLaneWidth()).ConvertTo(SIConstants::millimeter).value();

   LLDF::DeckType deckType;

   switch(wizard->page2->GetDeckType())
      {
      case IDC_WOOD_PLANK:       deckType = LLDF::WoodPlank;       break;
      case IDC_STRESS_LAMINATED: deckType = LLDF::StressLaminated; break;
      case IDC_SPIKE_LAMINATED:  deckType = LLDF::SpikeLaminated;  break;
      case IDC_GLUE_LAMINATED:   deckType = LLDF::GlueLaminated;   break;
      }

   delete lldf;
   lldf = new LLDF_TypeAL_Wood(S,de,wLane,deckType);

   LLDF::Location location = (wizard->page3a->GetGirderLocation() == IDC_INTGIRDER) ? LLDF::IntGirder : LLDF::ExtGirder;

   results.s_1_gMoment = lldf->GetMomentDF(location,LLDF::OneLoadedLane,LLDF::Strength);
   results.s_2_gMoment = lldf->GetMomentDF(location,LLDF::TwoOrMoreLoadedLanes,LLDF::Strength);
   results.s_1_gShear  = lldf->GetShearDF(location,LLDF::OneLoadedLane,LLDF::Strength);
   results.s_2_gShear  = lldf->GetShearDF(location,LLDF::TwoOrMoreLoadedLanes,LLDF::Strength);
   results.f_gMoment   = lldf->GetMomentDF(location,LLDF::OneLoadedLane,LLDF::Fatigue);
   results.f_gShear    = lldf->GetShearDF(location,LLDF::OneLoadedLane,LLDF::Fatigue);

   wizard->data.ax = results.girderAx;
   wizard->data.iz = results.girderIz;
   wizard->data.modE = results.modE;
   wizard->data.unitWeight = results.effUnitWgt;
   wizard->data.s_gMoment = max(results.s_1_gMoment,results.s_2_gMoment);
   wizard->data.s_gShear = max(results.s_1_gShear,results.s_2_gShear);
   wizard->data.f_gMoment = results.f_gMoment;
   wizard->data.f_gShear = results.f_gShear;

   ResultsToDlg();

   delete beam;
}

void
TSectPropWizard::Page6a::SteelBeam_WoodDeck()
{
   double tribWidth;
   double depth;
   double unitWgt;
   double effUnitWgt;
   double de;
   double S;
   double wLane;

   TShape* beam = wizard->page3b->GetBeam();
   results.girderAx = beam->GetArea();
   results.girderIz = beam->GetCentroidalInertia().Ixx;
   results.girderCG = beam->GetYbar();

   tribWidth = wizard->page4a->GetTribWidth();
   depth = wizard->page4a->GetDepth();
   unitWgt = wizard->page4a->GetUnitWgt();
   de = wizard->page4a->Getde();
   double deckArea = tribWidth*depth;

   effUnitWgt = (deckArea*unitWgt + beam->GetArea()*beam->GetUnitWeight())/beam->GetArea();

   results.effUnitWgt = effUnitWgt;
   results.modE = beam->GetModE();

   de = DerivedUnits::Length(de).ConvertTo(SIConstants::millimeter).value();

   S = DerivedUnits::Length(wizard->page3b->GetSpacing()).ConvertTo(SIConstants::millimeter).value();
   wLane = DerivedUnits::Length(wizard->page4a->GetDesignLaneWidth()).ConvertTo(SIConstants::millimeter).value();

   LLDF::DeckType deckType;

   switch(wizard->page2->GetDeckType())
      {
      case IDC_STRESS_LAMINATED: deckType = LLDF::StressLaminated; break;
      case IDC_SPIKE_LAMINATED:  deckType = LLDF::SpikeLaminated;  break;
      case IDC_GLUE_LAMINATED:   deckType = LLDF::GlueLaminated;   break;
      }

   delete lldf;
   lldf = new LLDF_TypeAL_Steel(S,de,wLane,deckType);

   LLDF::Location location = (wizard->page3b->GetGirderLocation() == IDC_INTGIRDER) ? LLDF::IntGirder : LLDF::ExtGirder;

   results.s_1_gMoment = lldf->GetMomentDF(location,LLDF::OneLoadedLane,LLDF::Strength);
   results.s_2_gMoment = lldf->GetMomentDF(location,LLDF::TwoOrMoreLoadedLanes,LLDF::Strength);
   results.s_1_gShear  = lldf->GetShearDF(location,LLDF::OneLoadedLane,LLDF::Strength);
   results.s_2_gShear  = lldf->GetShearDF(location,LLDF::TwoOrMoreLoadedLanes,LLDF::Strength);
   results.f_gMoment   = lldf->GetMomentDF(location,LLDF::OneLoadedLane,LLDF::Fatigue);
   results.f_gShear    = lldf->GetShearDF(location,LLDF::OneLoadedLane,LLDF::Fatigue);

   wizard->data.spanLength = wizard->page3b->GetSpanLength();
   wizard->data.ax = results.girderAx;
   wizard->data.iz = results.girderIz;
   wizard->data.modE = results.modE;
   wizard->data.unitWeight = results.effUnitWgt;
   wizard->data.s_gMoment = max(results.s_1_gMoment,results.s_2_gMoment);
   wizard->data.s_gShear = max(results.s_1_gShear,results.s_2_gShear);
   wizard->data.f_gMoment = results.f_gMoment;
   wizard->data.f_gShear = results.f_gShear;

   ResultsToDlg();

   delete beam;
}

void
TSectPropWizard::Page6a::SteelBeam_GridDeck()
{
   double deckWgt;
   double effWeight;

   TShape* beam = wizard->page3b->GetBeam();
   results.girderAx = beam->GetArea();
   results.girderIz = beam->GetCentroidalInertia().Ixx;
   results.girderCG = beam->GetYbar();

   deckWgt = wizard->page4b->GetWeight();

   effWeight = beam->GetUnitWeight()*beam->GetArea() + deckWgt;
   effWeight /= beam->GetArea();

   results.effUnitWgt = effWeight;
   results.modE = beam->GetModE();

   double S;
   double de;
   double wLane;
   double tg;

   S = DerivedUnits::Length(wizard->page3b->GetSpacing()).ConvertTo(SIConstants::millimeter).value();
   de = DerivedUnits::Length(wizard->page4b->Getde()).ConvertTo(SIConstants::millimeter).value();
   wLane = DerivedUnits::Length(wizard->page4b->GetDesignLaneWidth()).ConvertTo(SIConstants::millimeter).value();
   tg = DerivedUnits::Length(wizard->page4b->Gettg()).ConvertTo(SIConstants::millimeter).value();

   delete lldf;
   lldf = new LLDF_TypeA(S,de,wLane,tg);

   LLDF::Location location = (wizard->page3b->GetGirderLocation() == IDC_INTGIRDER) ? LLDF::IntGirder : LLDF::ExtGirder;

   results.s_1_gMoment = lldf->GetMomentDF(location,LLDF::OneLoadedLane,LLDF::Strength);
   results.s_2_gMoment = lldf->GetMomentDF(location,LLDF::TwoOrMoreLoadedLanes,LLDF::Strength);
   results.s_1_gShear  = lldf->GetShearDF(location,LLDF::OneLoadedLane,LLDF::Strength);
   results.s_2_gShear  = lldf->GetShearDF(location,LLDF::TwoOrMoreLoadedLanes,LLDF::Strength);
   results.f_gMoment   = lldf->GetMomentDF(location,LLDF::OneLoadedLane,LLDF::Fatigue);
   results.f_gShear    = lldf->GetShearDF(location,LLDF::OneLoadedLane,LLDF::Fatigue);

   wizard->data.spanLength = wizard->page3b->GetSpanLength();
   wizard->data.ax = results.girderAx;
   wizard->data.iz = results.girderIz;
   wizard->data.modE = results.modE;
   wizard->data.unitWeight = results.effUnitWgt;
   wizard->data.s_gMoment = max(results.s_1_gMoment,results.s_2_gMoment);
   wizard->data.s_gShear = max(results.s_1_gShear,results.s_2_gShear);
   wizard->data.f_gMoment = results.f_gMoment;
   wizard->data.f_gShear = results.f_gShear;

   ResultsToDlg();

   delete beam;
}

void
TSectPropWizard::Page6a::ResultsToDlg()
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

   value = (wizard->data.dispUnits == unitsSI) ? DerivedUnits::Density(results.effUnitWgt/SIConstants::g.value()).ConvertTo(SIConstants::kilogram_per_meter3).value() :
                                                 DerivedUnits::UnitWeight(results.effUnitWgt).ConvertTo(USConstants::pound_per_feet3).value();
   if (wizard->data.dispUnits == unitsSI)
      ostrstream(buffer,sizeof(buffer)) << "Eff. Density = " << EngNotation(value,3) << " " << "kg/m^3" << ends;
   else
      ostrstream(buffer,sizeof(buffer)) << "Unit Wgt = " << EngNotation(value,3) << " " << "lbs/ft^3" << ends;
   SetDlgItemText(IDC_UNITWEIGHT_RESULT,buffer);

   value = (wizard->data.dispUnits == unitsSI) ? DerivedUnits::Pressure(results.modE).ConvertTo(SIConstants::Pascal).value() :
                                                 DerivedUnits::Pressure(results.modE).ConvertTo(USConstants::pound_per_inch2).value();
   ostrstream(buffer,sizeof(buffer)) << "Mod. E = " << EngNotation(value,3) << " " << ((wizard->data.dispUnits == unitsSI) ? "Pa" : "psi") << ends;
   SetDlgItemText(IDC_MODE_RESULT,buffer);

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
TSectPropWizard::Page6a::BNClicked_Print()
{
   TPrinter printer;

   WriteReport();
   TFilePrintout printout(&printer,filename,"Report");
   printer.Print(Parent,printout,true);

   OFSTRUCT of;
   OpenFile(filename,&of,OF_DELETE);
}

void
TSectPropWizard::Page6a::WriteReport()
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

      case IDC_WOOD:
           ofile << (*wizard->page3a) << endl;
           break;

      default:
           break;
      }

   ofile << endl;
   ofile << "Deck Description" << endl;

   switch(wizard->page2->GetDeckType())
      {
      case IDC_STEEL_GRID:
           ofile << (*wizard->page4b) << endl;
           break;

      case IDC_CONCRETE:
      case IDC_WOOD_PLANK:
      case IDC_STRESS_LAMINATED:
      case IDC_SPIKE_LAMINATED:
      case IDC_GLUE_LAMINATED:
           ofile << (*wizard->page4a) << endl;;
           break;
      }

   ofile << endl;
   ofile << "Skew Corrections" << endl;
   ofile << (*wizard->page5) << endl;

   ofile << endl;
   ofile << (*this) << endl;
}

void
TSectPropWizard::Page6a::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Properties_and_Distribution_Factors);
}

