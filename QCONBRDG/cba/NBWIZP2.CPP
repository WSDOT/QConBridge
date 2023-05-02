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
// Page 2
//
DEFINE_RESPONSE_TABLE1(TNewBridgeWizard::TWizardPage2, TWizardStep)
    EV_BN_CLICKED(IDC_LS_PINNED,EvLeftSupportChanged),
    EV_BN_CLICKED(IDC_LS_ROLLER,EvLeftSupportChanged),
    EV_BN_CLICKED(IDC_IS_PINNED,EvInteriorSupportChanged),
    EV_BN_CLICKED(IDC_IS_ROLLER,EvInteriorSupportChanged),
    EV_BN_CLICKED(IDC_IS_COLUMN,EvInteriorSupportChanged),
    EV_BN_CLICKED(IDC_RS_PINNED,EvRightSupportChanged),
    EV_BN_CLICKED(IDC_RS_ROLLER,EvRightSupportChanged),
END_RESPONSE_TABLE;


TNewBridgeWizard::TWizardPage2::TWizardPage2(TNewBridgeWizard* wizard,SystemOfUnits units, TModule *module) :
TWizardStep(wizard,IDD_NEWBRIDGE_PAGE2,"Bridge Contractor - Bridge Geometry",0,module),
Wizard(wizard)
{
   PageInfo.dwFlags |= PSP_HASHELP;

    if (units == unitsSI)
      {
      l1 = new TLengthEdit(this,IDC_LENGTH1,SIConstants::meter,11,3,IDC_LENGTH1_TAG,"m");
//      l1->SetValidator(new TGTZeroValidator("L1","m"));

      l2 = new TLengthEdit(this,IDC_LENGTH2,SIConstants::meter,11,3,IDC_LENGTH2_TAG,"m");
//      l2->SetValidator(new TGTZeroValidator("L2","m"));
      }
   else
      {
      l1 = new TLengthEdit(this,IDC_LENGTH1,USConstants::feet,11,3,IDC_LENGTH1_TAG,"feet");
//      l1->SetValidator(new TGTZeroValidator("L1","feet"));

      l2 = new TLengthEdit(this,IDC_LENGTH2,USConstants::feet,11,3,IDC_LENGTH2_TAG,"feet");
//      l2->SetValidator(new TGTZeroValidator("L2","feet"));
      }

    leftSupport = new TRadioGroupBox(this,IDC_LEFTSUPPORT,IDC_LS_PINNED,IDC_LS_ROLLER);
    interiorSupport  = new TRadioGroupBox(this,IDC_INTERIORSUPPORT,IDC_IS_PINNED,IDC_IS_COLUMN);
    rightSupport = new TRadioGroupBox(this,IDC_RIGHTSUPPORT,IDC_RS_PINNED,IDC_RS_ROLLER);
}

TNewBridgeWizard::TWizardPage2::~TWizardPage2()
{
   delete l1;
   delete l2;
   delete leftSupport;
   delete interiorSupport;
   delete rightSupport;
}

void TNewBridgeWizard::TWizardPage2::SetupWindow()
{
   TWizardStep::SetupWindow();

   if (GetNumSpans() > 1)
      {
      leftSupport->CheckRadioButton(RGB_BYCOMMAND,IDC_LS_ROLLER);
      EvLeftSupportChanged();

      interiorSupport->CheckRadioButton(RGB_BYCOMMAND,IDC_IS_PINNED);
      EvInteriorSupportChanged();

      rightSupport->CheckRadioButton(RGB_BYCOMMAND,IDC_RS_ROLLER);
      EvRightSupportChanged();
      }
   else
      {
      if (leftSupport->GetSelRadioButton() == IDC_LS_ROLLER)
         {
         rightSupport->CheckRadioButton(RGB_BYCOMMAND,IDC_RS_PINNED);
         EvLeftSupportChanged();
         }
      }
}

void TNewBridgeWizard::TWizardPage2::EvLeftSupportChanged()
{
   if (GetNumSpans() == 1)
      {
      // If left support is roller, right support cannot be
      if (leftSupport->GetSelRadioButton() == IDC_LS_ROLLER)
         rightSupport->GetRadioButton(RGB_BYCOMMAND,IDC_RS_ROLLER)->EnableWindow(false);
      else
         rightSupport->GetRadioButton(RGB_BYCOMMAND,IDC_RS_ROLLER)->EnableWindow(true);
      }
   else
      {
      if ( (leftSupport->GetSelRadioButton() == IDC_LS_ROLLER) )
         {
         if (interiorSupport->GetSelRadioButton() == IDC_IS_ROLLER)
            rightSupport->GetRadioButton(RGB_BYCOMMAND,IDC_RS_ROLLER)->EnableWindow(false);
         else if (rightSupport->GetSelRadioButton() == IDC_RS_ROLLER)
            interiorSupport->GetRadioButton(RGB_BYCOMMAND,IDC_IS_ROLLER)->EnableWindow(false);
         else
            {
            rightSupport->GetRadioButton(RGB_BYCOMMAND,IDC_RS_ROLLER)->EnableWindow(true);
            interiorSupport->GetRadioButton(RGB_BYCOMMAND,IDC_IS_ROLLER)->EnableWindow(true);
            }
         }
      else
         {
         rightSupport->GetRadioButton(RGB_BYCOMMAND,IDC_RS_ROLLER)->EnableWindow(true);
         interiorSupport->GetRadioButton(RGB_BYCOMMAND,IDC_IS_ROLLER)->EnableWindow(true);
         }
      }

   TWizardPage4* step5 = Wizard->GetStep5();
   step5->BoundryConditions(leftSupport->GetSelRadioButton(),rightSupport->GetSelRadioButton());
}

void TNewBridgeWizard::TWizardPage2::EvInteriorSupportChanged()
{
   Wizard->EnableStep(IDPAGE_STEP5,interiorSupport->GetSelRadioButton() == IDC_IS_COLUMN);

   if (interiorSupport->GetSelRadioButton() == IDC_IS_ROLLER)
      {
      if ( (rightSupport->GetSelRadioButton() == IDC_RS_ROLLER) )
         leftSupport->GetRadioButton(RGB_BYCOMMAND,IDC_LS_ROLLER)->EnableWindow(false);
      else if (leftSupport->GetSelRadioButton() == IDC_LS_ROLLER)
         rightSupport->GetRadioButton(RGB_BYCOMMAND,IDC_RS_ROLLER)->EnableWindow(false);
      else
         {
         leftSupport->GetRadioButton(RGB_BYCOMMAND,IDC_LS_ROLLER)->EnableWindow(true);
         rightSupport->GetRadioButton(RGB_BYCOMMAND,IDC_RS_ROLLER)->EnableWindow(true);
         }
      }
   else
      {
      leftSupport->GetRadioButton(RGB_BYCOMMAND,IDC_LS_ROLLER)->EnableWindow(true);
      rightSupport->GetRadioButton(RGB_BYCOMMAND,IDC_RS_ROLLER)->EnableWindow(true);
      }
}

void TNewBridgeWizard::TWizardPage2::EvRightSupportChanged()
{
   if (GetNumSpans() == 1)
      {
      // If right support is roller, left support cannot be
      if (rightSupport->GetSelRadioButton() == IDC_RS_ROLLER)
         leftSupport->GetRadioButton(RGB_BYCOMMAND,IDC_LS_ROLLER)->EnableWindow(false);
      else
         leftSupport->GetRadioButton(RGB_BYCOMMAND,IDC_LS_ROLLER)->EnableWindow(true);
      }
   else
      {
      if ( (rightSupport->GetSelRadioButton() == IDC_RS_ROLLER) )
         {
         if (interiorSupport->GetSelRadioButton() == IDC_IS_ROLLER)
            leftSupport->GetRadioButton(RGB_BYCOMMAND,IDC_LS_ROLLER)->EnableWindow(false);
         else if (leftSupport->GetSelRadioButton() == IDC_LS_ROLLER)
            interiorSupport->GetRadioButton(RGB_BYCOMMAND,IDC_IS_ROLLER)->EnableWindow(false);
         else
            {
            leftSupport->GetRadioButton(RGB_BYCOMMAND,IDC_LS_ROLLER)->EnableWindow(true);
            interiorSupport->GetRadioButton(RGB_BYCOMMAND,IDC_IS_ROLLER)->EnableWindow(true);
            }
         }
      else
         {
         leftSupport->GetRadioButton(RGB_BYCOMMAND,IDC_LS_ROLLER)->EnableWindow(true);
         interiorSupport->GetRadioButton(RGB_BYCOMMAND,IDC_IS_ROLLER)->EnableWindow(true);
         }
      }

   TWizardPage4* step5 = Wizard->GetStep5();
   step5->BoundryConditions(leftSupport->GetSelRadioButton(),rightSupport->GetSelRadioButton());
}

int TNewBridgeWizard::TWizardPage2::GetNumSpans() const
{
   return Wizard->GetStep1()->GetNumSpans();
}

int TNewBridgeWizard::TWizardPage2::SetActive(TNotify far& notifyInfo)
{
   int retval = TWizardStep::SetActive(notifyInfo);

   // Enable/Disable interior supports
   if (GetNumSpans() > 1)
      {
      interiorSupport->EnableWindow(true);
      interiorSupport->ShowWindow(SW_SHOW);
      }
   else
      {
      interiorSupport->EnableWindow(false);
      interiorSupport->ShowWindow(SW_HIDE);
      }

   // Set span length labels
   if (GetNumSpans() == 1)
      {
      SetDlgItemText(IDC_LENGTH1_LABEL,"Span Length");
      TWindow(GetDlgItem(IDC_LENGTH2_LABEL)).ShowWindow(SW_HIDE);
      l2->ShowWindow(SW_HIDE);
      TWindow(GetDlgItem(IDC_LENGTH2_TAG)).ShowWindow(SW_HIDE);
      }
   else if (GetNumSpans() == 2)
      {
      SetDlgItemText(IDC_LENGTH1_LABEL,"Left Span Length");
      SetDlgItemText(IDC_LENGTH2_LABEL,"Right Span Length");
      TWindow(GetDlgItem(IDC_LENGTH2_LABEL)).ShowWindow(SW_SHOW);
      l2->ShowWindow(SW_SHOW);
      TWindow(GetDlgItem(IDC_LENGTH2_TAG)).ShowWindow(SW_SHOW);
      }
   else // 2+ spans
      {
      SetDlgItemText(IDC_LENGTH1_LABEL,"Exterior Span Length");
      SetDlgItemText(IDC_LENGTH2_LABEL,"Interior Span Length");
      TWindow(GetDlgItem(IDC_LENGTH2_LABEL)).ShowWindow(SW_SHOW);
      l2->ShowWindow(SW_SHOW);
      TWindow(GetDlgItem(IDC_LENGTH2_TAG)).ShowWindow(SW_SHOW);
      }

   UpdateUnitsTags();

   return retval;
}

void TNewBridgeWizard::TWizardPage2::UpdateUnitsTags()
{
   SystemOfUnits units = Wizard->GetStep1()->GetUnits();

   l1->SetUnits((units == unitsSI) ? SIConstants::meter : USConstants::feet,
               (units == unitsSI) ? "m" : "ft");

   l2->SetUnits((units == unitsSI) ? SIConstants::meter : USConstants::feet,
               (units == unitsSI) ? "m" : "ft");
}


void
TNewBridgeWizard::TWizardPage2::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Bridge_Geometry_Bridge_Contractor);
}

