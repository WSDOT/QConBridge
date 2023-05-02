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

/************************************************************
 BMPCTL.CPP

 Bitmap Control for OWL 2
 Used to place bitmaps in a dialog box

 Author:  Richard Brice
*************************************************************/

#include <checks.h>

#include <owltools\bmpctl.h>

DIAG_DEFINE_GROUP(BmpView,0,0);

// =========================================================
// TBitmapControl
//
// bitmap control - assumes a placeholder exists in the
// dialog template (eg. a blackrect static control)
// =========================================================
TBitmapControl::TBitmapControl (TWindow* parent,
                                int      resourceID,
                                int      bmpID,
                                Style    _style,
                                TModule* module)
	: TControl (parent, resourceID, module)
{
	style 	 = _style;
   adjustFrame = false;

   SetupInstance(bmpID);
}

TBitmapControl::TBitmapControl (TWindow* parent,
                                int      resourceID,
                                Style    _style,
                                TModule* module)
	: TControl (parent, resourceID, module)
{
	style 	 = _style;
   adjustFrame = false;

   SetupInstance(resourceID);
}

TBitmapControl::TBitmapControl (TWindow*  parent,
                                int       resourceID,
                                int       bmpID,
                                HoldPoint _holdPoint,
                                Style     _style,
                                Align     _align,
                                TModule*  module)
	: TControl (parent, resourceID, module)
{
	style 	 = _style;
   align     = _align;
   holdPoint = _holdPoint;

   adjustFrame = true;

   SetupInstance(bmpID);
}

TBitmapControl::~TBitmapControl ()
{
   delete Bitmap;
   delete Palette;
   delete Dib;
   delete BkgndBrush;
}

void TBitmapControl::SetupInstance(int bmpID)
{
   DisableTransfer();

   Bitmap     = 0;
   Palette    = 0;
   Dib        = 0;
   BkgndBrush = new TBrush(::GetSysColor(COLOR_WINDOW));

   TScreenDC screenDC;
   if (screenDC.GetDeviceCaps(NUMCOLORS) < 3 )
     Rop = NOTSRCCOPY;
   else
     Rop = SRCCOPY;

   upleftBmpCorner = TPoint(0,0);

   LoadBitmapResource(bmpID);
}

void TBitmapControl::SetupWindow ()
{
	TControl::SetupWindow ();
	ClearFlag (wfPredefinedClass);

   if (adjustFrame)
      AdjustFrame();
}

void TBitmapControl::AdjustFrame()
{
// Place frame around bitmap
	BITMAP bm;
	Bitmap->GetObject(bm);

   // Get initial control geometry
   TRect ctlRect = GetWindowRect(); // in screen coords.

   // Bitmap Size
   TSize bmpSize(bm.bmWidth,bm.bmHeight);

   // Compute coordinates of Top Left corner of control
   // keeping the coordinates of holdPoint fixed
   TPoint ctlPoint;
   int dx, dy;

   switch(holdPoint)
      {
      case TopLeft:
           ctlPoint = TPoint(ctlRect.TopLeft());
           break;

      case TopCenter:
           dx = ctlRect.left + ctlRect.Size().cx/2 - bm.bmWidth/2;
           ctlPoint = TPoint(dx,ctlRect.top);
           break;

      case TopRight:
           dx = ctlRect.left + ctlRect.Size().cx - bm.bmWidth;
           ctlPoint = TPoint(dx,ctlRect.top);
           break;

      case LeftCenter:
           dy = ctlRect.top + ctlRect.Size().cy/2 - bm.bmHeight/2;
           ctlPoint = TPoint(ctlRect.left,dy);
           break;

      case CenterCenter:
           dx = ctlRect.left + ctlRect.Size().cx/2 - bm.bmWidth/2;
           dy = ctlRect.top + ctlRect.Size().cy/2 - bm.bmHeight/2;
           ctlPoint = TPoint(dx,dy);
           break;

      case RightCenter:
           dx = ctlRect.left + ctlRect.Size().cx - bm.bmWidth;
           dy = ctlRect.top + ctlRect.Size().cy/2 - bm.bmHeight/2;
           ctlPoint = TPoint(dx,dy);
           break;

      case BottomLeft:
           dy = ctlRect.top + ctlRect.Size().cy - bm.bmHeight;
           ctlPoint = TPoint(ctlRect.left,dy);
           break;

      case BottomCenter:
           dx = ctlRect.left + ctlRect.Size().cx/2 - bm.bmWidth/2;
           dy = ctlRect.top + ctlRect.Size().cy - bm.bmHeight;
           ctlPoint = TPoint(dx,dy);
           break;

      case BottomRight:
           dx = ctlRect.left + ctlRect.Size().cx - bm.bmWidth;
           dy = ctlRect.top + ctlRect.Size().cy - bm.bmHeight;
           ctlPoint = TPoint(dx,dy);
           break;
      }

   // modify upper-left point of control to center control in parent
   TRect parentRect = Parent->GetClientRect(); // in client coordinates
   TPoint parentPnt = parentRect.TopLeft();
   Parent->ClientToScreen(parentPnt); // get upper left in screen coordinates
   switch(align)
      {
      case NoCenter:
           // Do nothing
           break;

      case HorizCenter:
           ctlPoint.x = parentPnt.x +
                       (parentRect.Size().cx-bmpSize.cx)/2;
           break;

      case VertCenter:
           ctlPoint.y = parentPnt.y +
                       (parentRect.Size().cy-bmpSize.cy)/2;
           break;

      case BothCenter:
           ctlPoint.x = parentPnt.x +
                       (parentRect.Size().cx-bmpSize.cx)/2;
           ctlPoint.y = parentPnt.y +
                       (parentRect.Size().cy-bmpSize.cy)/2;
           break;
      }

   // Convert control point to client coordinates
   Parent->ScreenToClient(ctlPoint);

   // Create a new rectangle for the control
   TRect newCtlRect(ctlPoint,bmpSize);

   // Move/Resize the control
   MoveWindow(newCtlRect);
}

void TBitmapControl::Paint (TDC& dc, bool, TRect&)
{
   TRect client = GetClientRect();

   PaintBitmap(dc,client);
   PaintBorder(dc,client);
}

void TBitmapControl::PaintBitmap(TDC& dc,TRect& clientRect)
{
/*
  TRACEX(BmpView, 2, "Enter Paint()");
  TRACEX(BmpView, 2, "Paint::BkBrush: " << hex << (UINT)(HBRUSH)*BkgndBrush);

  dc.SelectObject(*BkgndBrush);

  if (Bitmap)
    {
    TMemoryDC memoryDC(dc);
    TRACEX(BmpView, 2, "Paint::Bitmap: " << hex << (UINT)(HBITMAP)*Bitmap);
    memoryDC.SelectObject(*Bitmap);
    dc.SetStretchBltMode(COLORONCOLOR);
    if (Palette)
       {
        TRACEX(BmpView, 2, "Paint::Palette: " << hex << (UINT)(HPALETTE)*Palette);
        dc.SelectObject(*Palette, false);
        dc.RealizePalette();
        memoryDC.SelectObject(*Palette, false);
        }
    if (Rop == SRCCOPY)
      {
      if (Colors == 2)
         {
         if (Palette)
           {
           PALETTEENTRY pe;
           Palette->GetPaletteEntry(0, pe);
           dc.SetTextColor(TColor(pe));
           Palette->GetPaletteEntry(1, pe);
           dc.SetBkColor(TColor(pe));
           }
         else
           {
           dc.SetBkColor(TColor(0,0,0));
           dc.SetTextColor(TColor(255,255,255));
           }
         }
      }

    TRect imageRect(0,0, PixelWidth, PixelHeight);
    if (Fit)
       {
       dc.StretchBlt(clientRect, memoryDC, imageRect, Rop);
       }
    else
       {
       dc.BitBlt(imageRect, memoryDC, TPoint(0,0), Rop);

       // Clear borders here for no flicker
       //
       dc.PatBlt(TRect(TPoint(PixelWidth,0), clientRect.BottomRight()));
       dc.PatBlt(TRect(TPoint(0,PixelHeight), clientRect.BottomRight()));
       }
    }
 else
    dc.PatBlt(clientRect, PATCOPY);

  TRACEX(BmpView, 2, "Leave Paint()" << endl);
*/
   TRect imageRect(0,0, PixelWidth, PixelHeight);

   if (adjustFrame)
      dc.SetDIBitsToDevice(imageRect,upleftBmpCorner,*Dib);
   else
      dc.StretchDIBits(clientRect,imageRect,*Dib,Rop);
}

void TBitmapControl::PaintBorder(TDC &dc,TRect &client)
{
	int nMargin;

	switch (style)
	   {
		case Plain :
           nMargin = 1;
           break;

		case Recessed :
		case Raised :
           nMargin = 2;
           break;

		default :
           nMargin = 0;
           break;
	}

	if (nMargin)
	   {
      int sign;
      sign = (adjustFrame) ? 0 : 1;
		client.right 	+= sign*(nMargin << 1);
		client.bottom 	+= sign*(nMargin << 1);
	   }

	switch (style)
	   {
		case Plain :
           DrawPlainBorder(dc,client);
           break;

		case Recessed :
           DrawRecessedBorder(dc,client);
		 	  break;

		case Raised :
           DrawRaisedBorder(dc,client);
		 	  break;
	   }
}

void TBitmapControl::DrawPlainBorder(TDC &dc,TRect &client)
{
	dc.FrameRect (client, TBrush (GetSysColor (COLOR_WINDOWFRAME)));
	client = client.InflatedBy (-1, -1);
}

void TBitmapControl::DrawRecessedBorder(TDC &dc,TRect &client)
{
	TPen	penShadow (GetSysColor (COLOR_BTNSHADOW));
	TPen	penHilite (GetSysColor (COLOR_BTNHIGHLIGHT));
	TPen	penGrey (GetSysColor (COLOR_BTNFACE));
	TPen	penBlack (GetSysColor (COLOR_WINDOWFRAME));

   DrawFancyBorder(dc,client,penShadow,penHilite,penGrey,penBlack);
}

void TBitmapControl::DrawRaisedBorder(TDC &dc,TRect &client)
{
	TPen	penShadow (GetSysColor (COLOR_BTNSHADOW));
	TPen	penHilite (GetSysColor (COLOR_BTNHIGHLIGHT));
	TPen	penGrey (GetSysColor (COLOR_BTNFACE));
	TPen	penBlack (GetSysColor (COLOR_WINDOWFRAME));

   DrawFancyBorder(dc,client,penHilite,penShadow,penBlack,penGrey);
}

void TBitmapControl::DrawFancyBorder(TDC &dc,TRect &client,
                                     TPen &penShadow,TPen &penHilite,
                                     TPen &penGrey,TPen &penBlack)
{
	dc.SelectObject (penShadow);
	dc.MoveTo (client.right - 1, client.top);
	dc.LineTo (client.left, client.top);
	dc.LineTo (client.left, client.bottom - 1);

	dc.SelectObject (penHilite);
	dc.LineTo (client.right - 1, client.bottom - 1);
	dc.LineTo (client.right - 1, client.top - 1);

	client = client.InflatedBy (-1, -1);

	dc.SelectObject (penBlack);
	dc.MoveTo (client.right - 1, client.top);
	dc.LineTo (client.left, client.top);
	dc.LineTo (client.left, client.bottom - 1);

	dc.SelectObject (penGrey);
	dc.LineTo (client.right - 1, client.bottom - 1);
	dc.LineTo (client.right - 1, client.top - 1);

	client = client.InflatedBy (-1, -1);

	//
	// select stock objects back into the DC
	//
	dc.SelectStockObject (BLACK_PEN);
}

//
// Private Methods
//

bool TBitmapControl::LoadBitmapResource(int resId)
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
//     return false;
//     }
//
  SetupFromDib(newDib);

  return true;
}

void TBitmapControl::SetupFromDib(TDib* dib)
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

  UpdatePalette(true);

  TRACEX(BmpView, 1, "Leave SetupFromDib()");
}

bool TBitmapControl::UpdatePalette(bool alwaysRepaint)
{
  TRACEX(BmpView, 2, "Enter UpdatePalette()");
  if (Palette) {
    TClientDC clientDC(*this);
    #if !defined(__WIN32__)
      Palette->UnrealizeObject();
    #endif
    TRACEX(BmpView, 2, "UpdatePalette::Palette: " << hex << (UINT)(HPALETTE)*Palette);
    clientDC.SelectObject(*Palette, false);
    if (clientDC.RealizePalette() > 0)
      if (alwaysRepaint)
        Invalidate(false);
      else
        clientDC.UpdateColors();
    TRACEX(BmpView, 2, "Leave UpdatePalette(true)");
    return true;
  }
  TRACEX(BmpView, 2, "Leave UpdatePalette(false)");
  return false;
}

