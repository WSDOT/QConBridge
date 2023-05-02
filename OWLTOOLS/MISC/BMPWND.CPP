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


#include <owltools\bmpwnd.h>

DIAG_DECLARE_GROUP(BmpView);

//{{TBitmapWindow Implementation}}


TBitmapWindow::TBitmapWindow (TWindow* parent, int resId, TModule* module):
    TWindow(parent, "", module)
{
    // INSERT>> Your constructor code here.
   Bitmap     = 0;
   Palette    = 0;
   Dib        = 0;
   BkgndBrush = new TBrush(::GetSysColor(COLOR_WINDOW));

   TScreenDC screenDC;
   if (screenDC.GetDeviceCaps(NUMCOLORS) < 3 )
     Rop = NOTSRCCOPY;
   else
     Rop = SRCCOPY;

    LoadBitmapResource(resId);

	BITMAP bm;
	Bitmap->GetObject(bm);

   Attr.Style = WS_CHILD | WS_VISIBLE;
   Attr.W = bm.bmWidth;
   Attr.H = bm.bmHeight;
}


TBitmapWindow::~TBitmapWindow ()
{
    Destroy();

    // INSERT>> Your destructor code here.
   delete Bitmap;
   delete Palette;
   delete Dib;
   delete BkgndBrush;

}


void TBitmapWindow::SetupWindow ()
{
    TWindow::SetupWindow();

    // INSERT>> Your code here.
	ClearFlag (wfPredefinedClass);
   Invalidate();
}

BOOL TBitmapWindow::LoadBitmapResource(int resId)
{
  TDib* newDib;
  newDib = new TDib(*GetModule(), TResId(resId));
//  try
//     {
//     newDib = new TDib(*GetModule(), TResId(resId));
//     }
//
//  catch (TGdiBase::TXGdi)
//     {
//     MessageBox("Cannot access bitmap resource", GetApplication()->GetName(),MB_OK);
//     return FALSE;
//     }
//
  SetupFromDib(newDib);

  return TRUE;
}

void TBitmapWindow::SetupFromDib(TDib* dib)
{
  TRACEX(BmpView, 1, "Enter SetupFromDib()");
  TRACEX(BmpView, 1, "SetupFromDib::Dib: " << hex << (UINT)(HANDLE)*dib);

  if (Dib)
     {
     delete Dib;
     }
  Dib = dib;

  if (Palette)
     {
     delete Palette;
     }
  Palette = 0;

  try
     {
     Palette = new TPalette(*dib);
     }

  catch (...)
     {
     MessageBox("Dib has no color table, using default palette",
                GetApplication()->GetName(), MB_OK);
     Palette = new TPalette((HPALETTE)::GetStockObject(DEFAULT_PALETTE));
     }

  TRACEX(BmpView, 1, "SetupFromDib::Palette: " << hex << (UINT)(HPALETTE)*Palette);

  if (Bitmap)
     {
     delete Bitmap;
     }
  Bitmap = new TBitmap(*Dib, Palette);

  TRACEX(BmpView, 1, "SetupFromDib::Bitmap: " << hex << (UINT)(HBITMAP)*Bitmap);

  PixelWidth  = Dib->Width();
  PixelHeight = Dib->Height();

  UpdatePalette(TRUE);

  TRACEX(BmpView, 1, "Leave SetupFromDib()");
}

BOOL TBitmapWindow::UpdatePalette(BOOL alwaysRepaint)
{
  TRACEX(BmpView, 2, "Enter UpdatePalette()");
  if (Palette) {
    TClientDC clientDC(*this);
    #if !defined(__WIN32__)
      Palette->UnrealizeObject();
    #endif
    TRACEX(BmpView, 2, "UpdatePalette::Palette: " << hex << (UINT)(HPALETTE)*Palette);
    clientDC.SelectObject(*Palette, FALSE);
    if (clientDC.RealizePalette() > 0)
      if (alwaysRepaint)
        Invalidate(FALSE);
      else
        clientDC.UpdateColors();
    TRACEX(BmpView, 2, "Leave UpdatePalette(TRUE)");
    return TRUE;
  }
  TRACEX(BmpView, 2, "Leave UpdatePalette(FALSE)");
  return FALSE;
}


void TBitmapWindow::Paint (TDC& dc, bool erase, TRect& rect)
{
    TWindow::Paint(dc, erase, rect);

    // INSERT>> Your code here.
   TRect imageRect(0,0, PixelWidth, PixelHeight);
   dc.SetDIBitsToDevice(imageRect,TPoint(0,0),*Dib);

}

