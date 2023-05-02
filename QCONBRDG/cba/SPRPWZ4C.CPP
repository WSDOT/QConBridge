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



TSectPropWizard::Page4c::Page4c(TSectPropWizard *wiz,TModule *module) :
TWizardStep(wiz,IDD_SECTPRPWIZARD_PAGE4C,"Concrete Slab Description",0,module),
wizard(wiz)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   if(wizard->data.dispUnits == unitsSI)
      {
      depth = new TLengthEdit(this,IDC_DEPTH,SIConstants::millimeter,11,3,IDC_DEPTH_TAG,"mm");
      padDepth = new TLengthEdit(this,IDC_PAD,SIConstants::millimeter,11,3,IDC_PAD_TAG,"mm");
      sacDepth = new TLengthEdit(this,IDC_SACDEPTH,SIConstants::millimeter,11,3,IDC_SACDEPTH_TAG,"mm");
      overhang = new TLengthEdit(this,IDC_OVERHANG,SIConstants::millimeter,11,3,IDC_OVERHANG_TAG,"mm");
      de = new TLengthEdit(this,IDC_DE,SIConstants::millimeter,11,3,IDC_DE_TAG,"mm");
      unitWgt = 0;
      density = new TMaterialDensityEdit(this,IDC_UNITWGT,SIConstants::kilogram_per_meter3,11,3,IDC_UNITWGT_TAG,"kg/m^3");
      fc = new TPressureEdit(this,IDC_FC,SIConstants::MPa,11,3,IDC_FC_TAG,"MPa");
      effSpanLength = new TLengthEdit(this,IDC_LENGTH,SIConstants::meter,11,3,IDC_LENGTH_TAG,"m");
      designLane = new TLengthEdit(this,IDC_DESIGNLANE,SIConstants::millimeter,11,3,IDC_DESIGNLANE_TAG,"mm");
      }
   else
      {
      depth = new TLengthEdit(this,IDC_DEPTH,USConstants::inch,11,3,IDC_DEPTH_TAG,"inch");
      padDepth = new TLengthEdit(this,IDC_PAD,USConstants::inch,11,3,IDC_PAD_TAG,"inch");
      sacDepth = new TLengthEdit(this,IDC_SACDEPTH,USConstants::inch,11,3,IDC_SACDEPTH_TAG,"inch");
      overhang = new TLengthEdit(this,IDC_OVERHANG,USConstants::inch,11,3,IDC_OVERHANG_TAG,"inch");
      de = new TLengthEdit(this,IDC_DE,USConstants::feet,11,3,IDC_DE_TAG,"feet");
      unitWgt = new TUnitWeightEdit(this,IDC_UNITWGT,USConstants::pound_per_feet3,11,3,IDC_UNITWGT_TAG,"lbs/ft^3");
      density = 0;
      fc = new TPressureEdit(this,IDC_FC,USConstants::pound_per_inch2,11,3,IDC_FC_TAG,"psi");
      effSpanLength = new TLengthEdit(this,IDC_LENGTH,USConstants::feet,11,3,IDC_LENGTH_TAG,"feet");
      designLane = new TLengthEdit(this,IDC_DESIGNLANE,USConstants::feet,11,3,IDC_DESIGNLANE_TAG,"feet");
      }
}

TSectPropWizard::Page4c::~Page4c()
{
   delete depth;
   delete padDepth;
   delete sacDepth;
   delete overhang;
   delete de;
   delete unitWgt;
   delete density;
   delete fc;
   delete effSpanLength;
   delete designLane;
}

void TSectPropWizard::Page4c::SetupWindow()
{
   TWizardStep::SetupWindow();

   effSpanLength->SetValue(wizard->data.spanLength);

   if(wizard->data.dispUnits == unitsSI)
      {
      SetDlgItemText(IDC_UNITWGT_LABEL,"Density");
      depth->SetValueEx(190.); // ~7.5"
      padDepth->SetValueEx(0.);
      sacDepth->SetValueEx(15.); // ~0.5"
      overhang->SetValueEx(910.);
      density->SetValueEx(2560); // 2560 kg/m^3 (~160pcf)
      fc->SetValueEx(27.5); // 27.5MPa (~4000psi)
      designLane->SetValueEx(3600.);
      de->SetValueEx(910.);
      }
   else
      {
      SetDlgItemText(IDC_UNITWGT_LABEL,"Unit Wgt");
      depth->SetValueEx(7.5);
      padDepth->SetValueEx(0.);
      sacDepth->SetValueEx(0.5);
      overhang->SetValueEx(3.);
      unitWgt->SetValueEx(160); // 160pcf
      fc->SetValueEx(4000); /// 4000psi
      designLane->SetValueEx(12.);
      de->SetValueEx(3.);
      }
}

bool
TSectPropWizard::Page4c::IsInteriorGirder()
{
   bool isInterior;

   switch(wizard->page1->GetBeamType())
      {
      case IDC_STEELBEAM:
           isInterior = (wizard->page3b->GetGirderLocation() == IDC_INTGIRDER);
           break;

      case IDC_PRECASTIBEAM:
           isInterior = (wizard->page3c->GetGirderLocation() == IDC_INTGIRDER);
           break;

      default:
           break;
      }

   return isInterior;
}

int
TSectPropWizard::Page4c::SetActive(TNotify far& nmHdr)
{
   int retval = TWizardStep::SetActive(nmHdr);
   bool enable = !IsInteriorGirder();

   TWindow(GetDlgItem(IDC_DE_LABEL)).EnableWindow(enable);
   de->EnableWindow(enable);
   TWindow(GetDlgItem(IDC_OVERHANG_LABEL)).EnableWindow(enable);
   overhang->EnableWindow(enable);

   return retval;
}

double
TSectPropWizard::Page4c::GetModE() const
{
   double fc = DerivedUnits::Pressure(GetFc()*SIConstants::Pascal).ConvertTo(SIConstants::MPa).value();
   double E;
   E = EConcrete(GetDensity(),fc);
   return DerivedUnits::Pressure(E*SIConstants::MPa).ConvertTo(SIConstants::Pascal).value();
}

double
TSectPropWizard::Page4c::GetEffSlabWidth() const
{
   double spacing;
   double tWeb;
   double wTopFlange;
   bool isInterior;

   if (wizard->IsStepEnabled(IDD_PAGE3B)) // Steel beam
      {
      spacing = wizard->page3b->GetSpacing();
      tWeb = wizard->page3b->GetWebThickness();
      wTopFlange = wizard->page3b->GetTopFlangeWidth();
      isInterior = (wizard->page3b->GetGirderLocation() == IDC_INTGIRDER);
      }
   else if (wizard->IsStepEnabled(IDD_PAGE3C)) // Conc beam
      {
      spacing = wizard->page3c->GetSpacing();
      tWeb = wizard->page3c->GetWebThickness();
      wTopFlange = wizard->page3c->GetTopFlangeWidth();
      isInterior = (wizard->page3c->GetGirderLocation() == IDC_INTGIRDER);
      }

   return ::IBeam_EffectiveSlabWidth(GetEffSpanLength(),
                                     spacing,
                                     GetSlabDepth() - GetSacraficalDepth(),
                                     tWeb,
                                     wTopFlange,
                                     GetOverhang(),
                                     isInterior);
}

TCompSlab*
TSectPropWizard::Page4c::GetSlab() const
{
   double wTopFlange;
   double effSlabWidth;
   double height;

   if (wizard->IsStepEnabled(IDD_PAGE3B)) // Steel beam
      {
      wTopFlange = wizard->page3b->GetTopFlangeWidth();
      height = wizard->page3b->GetHeight();
      }
   else if (wizard->IsStepEnabled(IDD_PAGE3C)) // Conc beam
      {
      wTopFlange = wizard->page3c->GetTopFlangeWidth();
      height = wizard->page3c->GetHeight();
      }
   else if (wizard->IsStepEnabled(IDD_PAGE3A)) // Wood beam
      {
      wTopFlange = wizard->page3a->GetWidth();
      height = wizard->page3a->GetDepth();
      }

   effSlabWidth = GetEffSlabWidth();
   TCompSlab* slab = new TCompSlab(GetSlabDepth(),
                         effSlabWidth,
                         GetSlabDepth() + GetPadDepth(),
                         wTopFlange,
                         GetSacraficalDepth(),
                         TFPoint(0,height),
                         new GeneralMaterial(GetModE(),GetUnitWgt()));

   slab->ComputeProperties();

   return slab;
}

void
TSectPropWizard::Page4c::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Concrete_Slab_Deck_Type);
}

