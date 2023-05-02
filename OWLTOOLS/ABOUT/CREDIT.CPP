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

#include <owltools\credit.h>

DEFINE_RESPONSE_TABLE1(TCreditWindow,TWindow)
   EV_WM_TIMER,
END_RESPONSE_TABLE;

TCreditWindow::TCreditWindow(TWindow *parent,
                  TResId bmpResId, LPSTR AStringList[],TModule *module)
        : TWindow(parent,"",module)
{
  TEXTMETRIC TM;

  Attr.Style = WS_CHILD | WS_VISIBLE;
  Bitmap = new TBitmap(GetApplication()->LoadBitmap(bmpResId));
  if (!Bitmap)
  {
    Status = 1; // EM_INVALIDWINDOW
    return;
  };
  Bitmap->GetObject(BitSize);
  ScrollPos = 0;
  TDC DC(::GetDC(0));
  ScrollUnit = 2;
  ScrollRate = 80;
  DC.SelectStockObject(ANSI_VAR_FONT);
  DC.GetTextMetrics(TM);
  FontHeight = TM.tmHeight + TM.tmExternalLeading + 5;
  DC.RestoreFont();
  ReleaseDC(0,DC);
  StringList = AStringList;

  // Count strings in stringlist.
  for (StringCount = 0; StringList[StringCount]; StringCount++);
};

TCreditWindow::~TCreditWindow()
{
   delete Bitmap;
};

void TCreditWindow::GetWindowClass(WNDCLASS& WC)
{
  TWindow::GetWindowClass(WC);
  WC.style = CS_BYTEALIGNWINDOW;   // for BitBlt speed
  WC.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);
};

void TCreditWindow::SetupWindow()
{
  TWindow::SetupWindow();
  SetWindowPos(0, 0, 0, BitSize.bmWidth, BitSize.bmHeight,
               SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREDRAW);
};

void TCreditWindow::Destroy(int retVal)
{
  TWindow::Destroy(retVal);

  if (ScrollPos) // We're scrolling and need to kill the timer
  {
    KillTimer(1);
    ScrollPos = 0;
  };
};

void TCreditWindow::DrawBitmap(TDC &DC,int X, int Y, int W, int H)
{
  TMemoryDC MemDC(DC);
  MemDC.SelectObject(*Bitmap);
  DC.BitBlt(X, Y, W, H, MemDC, 0, 0, SRCCOPY);
  MemDC.RestoreObjects();
};

void TCreditWindow::Paint(TDC &dc,bool erase,TRect &rect)
{
  RECT R;
  int FirstLine, LastLine, Y;

  TWindow::Paint(dc,erase,rect);
  dc.SaveDC();
  dc.SetViewportOrg(TPoint(0, -ScrollPos));
  OffsetRect(&rect, 0, ScrollPos);
  R.left = 0;
  R.top = 0;
  R.right = Attr.W;
  R.bottom = Attr.H;
  if (IntersectRect(&R, &rect, &R))
  {
    DrawBitmap(dc,0, 0, Attr.W, Attr.H);
    if ((R.top < rect.top) && (R.bottom > rect.top))
      rect.top = R.bottom;
    if ((R.top < rect.bottom) && (R.bottom > rect.bottom))
      rect.bottom = R.top;
    if (rect.top > rect.bottom)
      rect.top = rect.bottom;
  };
  if (ScrollPos > 0)    // we're scrolling
  {
    FirstLine = (rect.top - Attr.H) / FontHeight;
    if (FirstLine < 0)
      FirstLine = 0;
    if (FirstLine < StringCount)
    {                             // we have text to draw
      dc.SetTextAlign(TA_CENTER);
      dc.SetBkColor(0);
      dc.SetTextColor(RGB(0xff,0xff,0xff));
      LastLine = (rect.bottom - Attr.H) / FontHeight;
      for (Y = FirstLine; Y <= LastLine; Y++)
      {
        if (Y < StringCount)
          dc.TextOut(Attr.W / 2, Y*FontHeight + Attr.H,
                     StringList[Y], _fstrlen(StringList[Y]));
      };
    };
                            // Paint second image of bitmap at bottom
    if (rect.bottom > (Attr.H + FontHeight * StringCount))
      DrawBitmap(dc,0, Attr.H + FontHeight * StringCount,
                    Attr.W, Attr.H);
  };
  dc.RestoreDC();
};

void TCreditWindow::ShowCredits()
{
  SetTimer(1, ScrollRate, NULL);
};

void TCreditWindow::EvTimer(uint timerId)
{
  ScrollPos += ScrollUnit;
  // Check to see if it's time to stop scrolling
  if (ScrollPos > Attr.H + FontHeight * StringCount)
  {
    ScrollPos = 0;
    KillTimer(timerId);
    Invalidate(FALSE);
  }
  else
    ScrollWindow(0, -ScrollUnit, NULL, NULL);
  UpdateWindow();
};

