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


#include <checks.h>
#include <qconbrdg\newbridg.h>
#include <qconbrdg\sprpwiz.h>
#include <qconbrdg\gt0valid.h>
#include <Context.hh>

//
// Page 4
//

TNewBridgeWizard::TWizardPage4::TWizardPage4(TNewBridgeWizard* wizard,SystemOfUnits units, TModule *module) :
TWizardStep(wizard,IDD_NEWBRIDGE_PAGE4,"Bridge Contractor - Interior Support Description",0,module),
Wizard(wizard)
{
   PageInfo.dwFlags |= PSP_HASHELP;

    if (units == unitsSI)
      {
      h = new TLengthEdit(this,IDC_HEIGHT,SIConstants::meter,11,3,IDC_HEIGHT_TAG,"m");
//      h->SetValidator(new TGTZeroValidator("H","m"));

      ax = new TLength2Edit(this,IDC_AX,SIConstants::millimeter2,11,3,IDC_AX_TAG,"mm^2");
//      ax->SetValidator(new TGTZeroValidator("Ax","mm^2"));

      iz = new TLength4Edit(this,IDC_IZ,SIConstants::millimeter4,11,3,IDC_IZ_TAG,"mm^4");
//      iz->SetValidator(new TGTZeroValidator("Iz","mm^4"));

      modE = new TPressureEdit(this,IDC_MODE,SIConstants::Pascal,11,3,IDC_MODE_TAG,"Pa");
//      modE->SetValidator(new TGTZeroValidator("Mod. E","Pa"));

      unitWeight = new TUnitWeightEdit(this,IDC_UNITWEIGHT,USConstants::pound_per_feet3,11,3,IDC_UNITWEIGHT_TAG,"pcf");
      unitWeight->DisableTransfer();
      density = new TMaterialDensityEdit(this,IDC_DENSITY,SIConstants::kilogram_per_meter3,11,3,IDC_DENSITY_TAG,"kg/m^3");
//      density->SetValidator(new TGTZeroValidator("Density","kg/m^3"));
      }
   else
      {
      h = new TLengthEdit(this,IDC_HEIGHT,USConstants::feet,11,3,IDC_HEIGHT_TAG,"feet");
//      h->SetValidator(new TGTZeroValidator("H","feet"));

      ax = new TLength2Edit(this,IDC_AX,USConstants::inch2,11,3,IDC_AX_TAG,"in^2");
//      ax->SetValidator(new TGTZeroValidator("Ax","in^2"));

      iz = new TLength4Edit(this,IDC_IZ,USConstants::inch4,11,3,IDC_IZ_TAG,"in^4");
//      iz->SetValidator(new TGTZeroValidator("Iz","in^4"));

      modE = new TPressureEdit(this,IDC_MODE,USConstants::kip_per_inch2,11,3,IDC_MODE_TAG,"ksi");
//      modE->SetValidator(new TGTZeroValidator("Mod. E","ksi"));

      density = new TMaterialDensityEdit(this,IDC_DENSITY,SIConstants::kilogram_per_meter3,11,3,IDC_DENSITY_TAG,"kg/m^3");
      density->DisableTransfer();
      unitWeight = new TUnitWeightEdit(this,IDC_UNITWEIGHT,USConstants::pound_per_feet3,11,3,IDC_UNITWEIGHT_TAG,"pcf");
//      unitWeight->SetValidator(new TGTZeroValidator("Density","pcf"));
      }

   support = new TRadioGroupBox(this,IDC_INTERIORSUPPORT,IDC_IS_PINNED,IDC_IS_COLUMN);
   hingeChk = new TCheckBox(this,IDC_HINGED,support);
}

TNewBridgeWizard::TWizardPage4::~TWizardPage4()
{
   delete h;
   delete ax;
   delete iz;
   delete modE;
   delete density;
   delete unitWeight;
   delete support;
   delete hingeChk;
}

void TNewBridgeWizard::TWizardPage4::SetupWindow()
{
   TWizardStep::SetupWindow();
}

void TNewBridgeWizard::TWizardPage4::BoundryConditions(int firstSupport,int lastSupport)
{
   if ( (firstSupport == IDC_LS_ROLLER) &&
        (lastSupport  == IDC_RS_ROLLER) )
      support->GetRadioButton(RGB_BYCOMMAND,IDC_IS_ROLLER)->EnableWindow(false);
   else
      support->GetRadioButton(RGB_BYCOMMAND,IDC_IS_ROLLER)->EnableWindow(true);
}

void TNewBridgeWizard::TWizardPage4::EnableHinged(bool enable)
{
   hingeChk->EnableWindow(enable);
}

void TNewBridgeWizard::TWizardPage4::UpdateUnitsTags()
{
   SystemOfUnits units = Wizard->GetStep1()->GetUnits();

   h->SetUnits((units == unitsSI) ? SIConstants::meter : USConstants::feet,
               (units == unitsSI) ? "m" : "ft");
   h->SetValueEx(1.0);

   ax->SetUnits((units == unitsSI) ? SIConstants::millimeter2 : USConstants::inch2,
               (units == unitsSI) ? "mm^2" : "in^2");
   ax->SetValueEx(1.0);

   iz->SetUnits((units == unitsSI) ? SIConstants::millimeter4 : USConstants::inch4,
               (units == unitsSI) ? "mm^4" : "in^4");
   iz->SetValueEx(1.0);

   modE->SetUnits((units == unitsSI) ? SIConstants::Pascal : USConstants::kip_per_inch2,
               (units == unitsSI) ? "Pa" : "ksi");
   modE->SetValueEx(1.0);

   if (units == unitsSI)
      {
      density->EnableTransfer();
      density->Show(SW_SHOW);
      density->ShowTag(SW_SHOW);
      density->SetUnits(SIConstants::kilogram_per_meter3,"kg/m^3");
      density->SetValueEx(1.0);
      SetDlgItemText(IDC_UNITWEIGHT_LABEL,"Density");
      unitWeight->DisableTransfer();
      unitWeight->Show(SW_HIDE);
      unitWeight->ShowTag(SW_HIDE);
      }
   else
      {
      unitWeight->EnableTransfer();
      unitWeight->Show(SW_SHOW);
      unitWeight->ShowTag(SW_SHOW);
      unitWeight->SetUnits(USConstants::pound_per_feet3,"pcf");
      unitWeight->SetValueEx(1.0);
      SetDlgItemText(IDC_UNITWEIGHT_LABEL,"Unit Wgt");
      density->DisableTransfer();
      density->Show(SW_HIDE);
      density->ShowTag(SW_HIDE);
      }
}

int TNewBridgeWizard::TWizardPage4::SetActive(TNotify far& notifyInfo)
{
   int retval = TWizardStep::SetActive(notifyInfo);
   SystemOfUnits units = Wizard->GetStep1()->GetUnits();

   if (units == unitsSI)
      {
      unitWeight->Show(SW_HIDE);
      unitWeight->ShowTag(SW_HIDE);

      SetDlgItemText(IDC_UNITWEIGHT_LABEL,"Density");
      }
   else
      {
      density->Show(SW_HIDE);
      density->ShowTag(SW_HIDE);

      SetDlgItemText(IDC_UNITWEIGHT_LABEL,"Unit Wgt");
      }

   UpdateUnitsTags();

   return retval;
}

void
TNewBridgeWizard::TWizardPage4::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Interior_Support_Description_Bridge_Contractor);
}

