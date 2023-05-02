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


#include "bmpctl.h"

// =========================================================
// TBitmapControl
//
// bitmap control - assumes a placeholder exists in the
// dialog template (eg. a blackrect static control)
// =========================================================
TBitmapControl::TBitmapControl (TWindow* parent, int resourceID, Style _style, TModule* module)
	: TControl (parent, resourceID, module)
{
	style   = _style;
	dib     = new TDib (GetModule()->GetInstance(), resourceID);

	if (dib->GetInfoHeader ()->biBitCount > 8)
	{
		palette = new TPalette (HPALETTE (GetStockObject (DEFAULT_PALETTE)));
	}
	else
	{
		palette = new TPalette (*dib);
	}

	bitmap  = new TBitmap (*dib, palette);

	UpdatePalette();
}

TBitmapControl::~TBitmapControl ()
{
	delete dib;
	delete palette;
	delete bitmap;
}

void TBitmapControl::SetupWindow ()
{
    TControl::SetupWindow ();

	ClearFlag (wfPredefinedClass);
}

void TBitmapControl::Paint (TDC& dc, BOOL, TRect&)
{
	BITMAP          bm;
	TMemoryDC memoryDC(dc);

	bitmap->GetObject(bm);

	TRect    client (0, 0, bm.bmWidth, bm.bmHeight);
	int      nMargin;

	switch (style)
	{
		case Plain :
		{
			nMargin = 1;
			break;
		}

		case Recessed :
		case Raised :
		{
			nMargin = 2;
			break;
		}

		default :
		{
			nMargin = 0;
			break;
		}
	}

	if (nMargin)
	{
		client.right    += (nMargin << 1);
		client.bottom   += (nMargin << 1);
	}

	switch (style)
	{
		case Plain :
		{
			dc.FrameRect (client, TBrush (GetSysColor (COLOR_WINDOWFRAME)));
			client = client.InflatedBy (-1, -1);
			break;
		}

		case Recessed :
		case Raised :
		{
			TPen    penShadow (GetSysColor (style == Recessed ? COLOR_BTNSHADOW : COLOR_BTNHIGHLIGHT));
			TPen    penHilite (GetSysColor (style == Recessed ? COLOR_BTNHIGHLIGHT : COLOR_WINDOWFRAME));
			TPen    penBlack  (GetSysColor (style == Recessed ? COLOR_WINDOWFRAME : COLOR_BTNFACE));
			TPen    penGrey   (GetSysColor (style == Recessed ? COLOR_BTNFACE : COLOR_BTNSHADOW));

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
			break;
		}
	}

	//
	// draw the bitmap
	//
	TMemoryDC       dcMem(dc);

	dcMem.SelectObject(*bitmap);
	dc.SelectObject(*palette, FALSE);
	dc.RealizePalette();
	dcMem.SelectObject(*palette, FALSE);
	dc.BitBlt(client, dcMem, TPoint (0, 0));
}

void TBitmapControl::UpdatePalette()
{
	if (palette && HWindow)
	{
		TClientDC clientDC(*this);
		palette->UnrealizeObject();
		clientDC.SelectObject(*palette, FALSE);

		if (clientDC.RealizePalette() > 0)
		{
			Invalidate(FALSE);
		}
	}
}

