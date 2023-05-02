///////////////////////////////////////////////////////////////////////
// OWLTools - Extension Library for the Borland OWL Framework
// Copyright (C) 1999  Washington State Department of Transportation
//                     Bridge and Structures Office
//
// This library was developed as part of the Alternate Route Project
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the Alternate Route Library Open Source License as published by 
// the Washington State Department of Transportation, Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but is distributed 
// AS IS, WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
// or FITNESS FOR A PARTICULAR PURPOSE. See the Alternate Route Library Open Source 
// License for more details.
//
// You should have received a copy of the Alternate Route Library Open Source License 
// along with this program; if not, write to the Washington State Department of 
// Transportation, Bridge and Structures Office, 4500 3rd Ave SE - P.O. Box  47340, 
// Olympia, WA 98503, USA or e-mail Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Revision Log
// ============
// May 1997 - Created, Richard Brice
// Oct 1999 - Released as open source

#include <owltools\progdlg.h>
#include <owltools\progress.rh>


DEFINE_RESPONSE_TABLE1(TProgressDlg, TDialog)
    EV_BN_CLICKED(IDC_PROGRESSBTN, BNClicked_ProgressBtn),
    EV_BN_CLICKED(IDCANCEL, CmCancel), // Must do this because CmCancel is not virtual
END_RESPONSE_TABLE;

TProgressDlg::TProgressDlg(TWindow *parent,int resId) :
TDialog(parent,resId)
{
   progressMsg = new TStatic(this,IDC_PROGRESSMSG);
   progressGauge = new TGauge(this,"",1,0,0,0,0);
   shouldContinue = true;
   okPressed = false;
}

TProgressDlg::~TProgressDlg()
{
   Destroy();
   delete progressGauge;
   delete progressMsg;
}

void TProgressDlg::SetupWindow()
{
   TDialog::SetupWindow();

   // Setup button
   MakeCancelButton();

   // Setup the gauge
   TWindow(GetDlgItem(IDC_PROGRESSGAUGE)).EnableWindow(false);
   TWindow(GetDlgItem(IDC_PROGRESSGAUGE)).Show(SW_HIDE);
   TRect rect = TWindow(GetDlgItem(IDC_PROGRESSGAUGE)).GetWindowRect(); // Get rect of placeholder element (screen coordinates)
   ScreenToClient(rect.TopLeft());                            // Map top left to client coordinates
   ScreenToClient(rect.BottomRight());                        // Map bottom right to client coordinates
   progressGauge->SetWindowPos(0,rect,SWP_SHOWWINDOW);        // Move and Size guage to match the placeholder element

   // Center the dialog
   TPoint newOrigin(GetSystemMetrics(SM_CXSCREEN)/2,
                    GetSystemMetrics(SM_CYSCREEN)/2);
   TRect tempRect = GetWindowRect();
   newOrigin.x -= tempRect.Width()/2;
   newOrigin.y -= tempRect.Height()/2;

   MoveWindow(TRect(newOrigin,tempRect.Size()));

   progressMsg->SetText("Analysis In Progress");
}

void TProgressDlg::MakeOkButton()
{
   SetDlgItemText(IDC_PROGRESSBTN,"&OK");
   shouldContinue = false;
}

void TProgressDlg::MakeCancelButton()
{
   SetDlgItemText(IDC_PROGRESSBTN,"&Cancel");
   shouldContinue = true;
}

void TProgressDlg::CmCancel()
{
   // NOTE:
   //
   // This bypasses TDialog::CmCancel(). CmCancel() does provide the
   // behavior we want so we just forget about it.

   BNClicked_ProgressBtn();
}

void TProgressDlg::BNClicked_ProgressBtn()
{
   if (shouldContinue)
      {
      shouldContinue = false;
      }
   else
      {
      okPressed = true;
      }
}

bool TProgressDlg::WasOkPressed()
{
   return okPressed;
}

void TProgressDlg::SetProgressMsg(const char *msg)
{
   GetApplication()->PumpWaitingMessages();
   progressMsg->SetText(msg);
}

bool TProgressDlg::ShouldContinue()
{
   GetApplication()->PumpWaitingMessages();
   return shouldContinue;
}

void TProgressDlg::SetGaugeValue(int value)
{
   progressGauge->SetValue(value);
}

int TProgressDlg::GetGaugeValue() const
{
   return progressGauge->GetValue();
}

void TProgressDlg::SetGaugeColor(TColor color)
{
   progressGauge->SetColor(color);
}

void TProgressDlg::IncrementGauge()
{
   int min, max;
   int currentValue =  progressGauge->GetValue();
   progressGauge->GetRange(min,max);
   if (currentValue >= max)
      currentValue = min;
   else
      currentValue++;

   progressGauge->SetValue(currentValue);
}

void TProgressDlg::DecrementGauge()
{
   int min, max;
   int currentValue =  progressGauge->GetValue();
   progressGauge->GetRange(min,max);
   if (currentValue <= min)
      currentValue = max;
   else
      currentValue--;

   progressGauge->SetValue(currentValue);
}

void TProgressDlg::SetLedSpacing(int spacing)
{
   progressGauge->SetLed(spacing);
}

void TProgressDlg::SetLedRange(int min,int max)
{
   progressGauge->SetRange(min,max);
}

