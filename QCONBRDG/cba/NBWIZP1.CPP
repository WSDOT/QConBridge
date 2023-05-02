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
//
//
DEFINE_RESPONSE_TABLE1(TNewBridgeWizard::TWizardPage1, TWizardStep)
    EV_EN_CHANGE(IDC_NUMSPANS,EvValueChanged),
    EV_BN_CLICKED(IDC_CONTINUOUSSPANS,EvContinuityChanged),
    EV_BN_CLICKED(IDC_SIMPLESPANS,EvContinuityChanged),
END_RESPONSE_TABLE;


TNewBridgeWizard::TWizardPage1::TWizardPage1(TNewBridgeWizard* wizard,TModule *module) :
   TWizardStep(wizard,IDD_NEWBRIDGE_PAGE1,"Bridge Contractor - General Properties",0,module),
   Wizard(wizard)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   numSpans = new TRangeSpinEdit(this,IDC_NUMSPANS);
   numSpans->Spinner.SetRange(1,10);

   continuity = new TRadioGroupBox(this,IDC_CONTINUITY,
                                        IDC_CONTINUOUSSPANS,
                                        IDC_SIMPLESPANS);
   units = new TRadioGroupBox(this,IDC_UNITSGRP,IDC_SI,IDC_ENGLISH);
}

TNewBridgeWizard::TWizardPage1::~TWizardPage1()
{
   delete numSpans;
   delete continuity;
   delete units;
}

int TNewBridgeWizard::TWizardPage1::GetNumSpans() const
{
   return numSpans->GetCurrentValue();
}

SystemOfUnits TNewBridgeWizard::TWizardPage1::GetUnits() const
{
   return (units->GetSelRadioButton() == IDC_SI) ? unitsSI : unitsEnglish;
}

void TNewBridgeWizard::TWizardPage1::SetupWindow()
{
   TWizardStep::SetupWindow();
   EvValueChanged();
}

void TNewBridgeWizard::TWizardPage1::EvValueChanged()
{
   if (numSpans->GetCurrentValue() == 1)
      continuity->EnableWindow(false);
   else
      continuity->EnableWindow(true);

   if (numSpans->GetCurrentValue() == 1) // 1 span
      {
      Wizard->EnableStep(IDPAGE_STEP4,false);
      Wizard->EnableStep(IDPAGE_STEP5,false);
      }
   else // 2 or more spans
      {
      Wizard->EnableStep(IDPAGE_STEP4,true);
      Wizard->EnableStep(IDPAGE_STEP5,false);
      }
}

void TNewBridgeWizard::TWizardPage1::EvContinuityChanged()
{
   TWizardPage4* step5 = Wizard->GetStep5();
   step5->EnableHinged(continuity->GetSelRadioButton() == IDC_CONTINUOUSSPANS);
}

void
TNewBridgeWizard::TWizardPage1::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_General_Properties_Bridge_Contractor);
}

