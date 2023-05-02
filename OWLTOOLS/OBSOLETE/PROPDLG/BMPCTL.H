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

// =========================================================
// BMPCTL.H
// header file for implementation of TBitmapControl
// =========================================================
#ifndef __BITMAP_CONTROL_H
#define __BITMAP_CONTROL_H

#if !defined (OWL_CONTROL_H)
#include <owl\control.h>
#endif

#if !defined (OWL_GDIOBJEC_H)
#include <owl\gdiobjec.h>
#endif


#if defined (BUILDPROPDLGDLL)
  #define PROPDLGEXPORT	__export
#elif defined (PROPDLGDLL)
  #define PROPDLGEXPORT	__import
#else
  #define PROPDLGEXPORT
#endif
/*
// expect to export if propdlg being exported..
#if !defined (PROPDLGEXPORT)
  #define PROPDLGEXPORT
#endif
*/
class PROPDLGEXPORT TBitmapControl : public TControl
{
	TDib*			dib;
	TPalette*		palette;
	TBitmap*		bitmap;

public:
	enum Style { NoBorder, Plain, Recessed, Raised } style;

	TBitmapControl (TWindow* parent, int resourceID, Style style = Recessed, TModule* module = NULL);
	~TBitmapControl ();

	void			SetupWindow ();
	void			Paint (TDC& dc, BOOL bErase, TRect& rc);

    void			UpdatePalette ();
};

#endif