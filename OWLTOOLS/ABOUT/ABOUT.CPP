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

//
//  About box object for C++ OWL
//
//  Copyright 1993 by Danny Thorpe
//

#include <owltools\about.h>


DEFINE_RESPONSE_TABLE1(TAboutBox,TDialog)
   EV_BN_CLICKED(IDHOTKEY,HotKey_BNClicked),
END_RESPONSE_TABLE;

TAboutBox::TAboutBox(TWindow *parent,
                     TResId resId,
                     char *title,
                     TResId bitmapResId,
                     LPSTR AStringList[],
                     TModule *module) :
     TDialog(parent,resId,module)
{
  Title = _fstrdup(title ? title : "");
  bmpId = bitmapResId;
  StringList = AStringList;
};

TAboutBox::~TAboutBox()
{
  if ( HIWORD(Title) )
    farfree((void*)Title);
};

void TAboutBox::SetupWindow()
{
  TRect RDialog,R,RBitWnd,RShade,RBump,ROk;
  int X8, Y8;
  HDC DC;

  InitCreditWindow(bmpId, StringList);
  TDialog::SetupWindow();
  SetWindowText(Title);
  DC = GetDC(HWindow);
  X8 = GetDeviceCaps(DC,LOGPIXELSX) / 8;   // 1/8 inch
  Y8 = GetDeviceCaps(DC,LOGPIXELSY) / 8;
  ReleaseDC(HWindow, DC);
  TWindow(GetDlgItem(IDSHADE)).GetClientRect(RShade);
  TWindow(GetDlgItem(IDBUMP)).GetClientRect(RBump);
  TWindow(GetDlgItem(IDOK)).GetClientRect(ROk);
  CreditWindow->GetClientRect(RBitWnd);
  RShade.top = Y8;
  RShade.left = X8;
  if (RShade.right < RBitWnd.right + 2*X8)
    RShade.right = RBitWnd.right + 2*X8;
  if (RShade.bottom < RBitWnd.bottom + 2*Y8)
    RShade.bottom = RBitWnd.bottom + 2*Y8;

  GetWindowRect(RDialog);
  GetClientRect(R);
  RDialog.right = RDialog.right - RDialog.left - R.right;
  RDialog.bottom = RDialog.bottom - RDialog.top - R.bottom;
  RDialog.right += X8 + RShade.right + X8;   // 1/8 inch margins
  RDialog.bottom +=  Y8 + RShade.bottom
                   + Y8 + RBump.bottom
                   + Y8 + ROk.bottom + Y8;
  if (Parent)
  {
    Parent->GetWindowRect(R);
//         Center dialog in parent's window
    RDialog.left = R.left + (R.right - R.left) / 2
                          - RDialog.right / 2;
    RDialog.top = R.top + (R.bottom - R.top) / 2
                        - RDialog.bottom / 2;
  };
  SetWindowPos(0, RDialog.left, RDialog.top,
                        RDialog.right, RDialog.bottom,
                        SWP_NOACTIVATE | SWP_NOZORDER);

  TWindow(GetDlgItem(IDSHADE)).SetWindowPos(0, RShade.left,
               RShade.top, RShade.right, RShade.bottom,
               SWP_NOACTIVATE | SWP_NOZORDER);
  CreditWindow->SetWindowPos(0,  RShade.left + X8,
               RShade.top + Y8, 0, 0,
               SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);

  RBump.left = -1;
  RBump.right = RDialog.right + 2;
  RBump.top = RShade.top + RShade.bottom + Y8;
  TWindow(GetDlgItem(IDBUMP)).SetWindowPos(0, RBump.left,
               RBump.top, RBump.right, RBump.bottom,
               SWP_NOACTIVATE | SWP_NOZORDER);

  GetClientRect(R);
  TWindow(GetDlgItem(IDOK)).SetWindowPos(0,
               R.right / 2 - ROk.right / 2,
               RBump.top + RBump.bottom + Y8, 0, 0,
               SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
};

void TAboutBox::InitCreditWindow(TResId bitmapResId, LPSTR AStringList[])
{
  CreditWindow = new TCreditWindow(this, bitmapResId, AStringList);
};

void TAboutBox::HotKey_BNClicked()
{
  CreditWindow->ShowCredits();
};