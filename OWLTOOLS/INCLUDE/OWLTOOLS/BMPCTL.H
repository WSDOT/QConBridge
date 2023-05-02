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

#if !defined(BMPCTL_H_)
#define BMPCTL_H_

#include <owltools\owltldll.h>
#include <owl\control.h>
#include <owl\gdiobjec.h>

class _OWLTOOLSCLASS TBitmapControl : public TControl
{
// Data Members
protected:
    TDib*     Dib;
    TBitmap*  Bitmap;
    TPalette* Palette;
    TBrush*   BkgndBrush;
    long      Rop;
    int       PixelWidth;
    int       PixelHeight;
    WORD      Colors;
    bool      adjustFrame; // false if bitmap is streched to fill frame
    TPoint    upleftBmpCorner; // upper left corner of bitmap goes here

public:
	enum Style { NoBorder, Plain, Recessed, Raised } style;
   enum Align { NoCenter,    // Don't center in parent
                HorizCenter, // Center horizontally in parent
                VertCenter,  // Center vertically in parent
                BothCenter   // Center both horizontally and vertically in parent
              } align;

   // Used when frame is altered to fit bitmap.
   // Holds a specific frame point. Align has
   // precidence over HoldPoint.
   enum HoldPoint { TopLeft,    TopCenter,    TopRight,
                    LeftCenter, CenterCenter, RightCenter,
                    BottomLeft, BottomCenter, BottomRight
						} holdPoint;

// Constructors/Destructors
public:

   // Bitmap is streched to fill frame
   // Frame is not altered
	TBitmapControl (TWindow* parent,
                   int      resourceID,
                   int      bmpID,
                   Style    style  = NoBorder,
                   TModule* module = NULL);

   // Bitmap is streched to fill frame
   // Frame is not altered
   // Control and bitmap have same id
	TBitmapControl (TWindow* parent,
                   int      resourceID,
                   Style    style  = NoBorder,
                   TModule* module = NULL);

   // Frame is altered to fit bitmap.
   // A specific point of the frame is held constant
   // Frame may be centered in parent (alters hold point)
	TBitmapControl (TWindow*  parent,
                   int       resourceID,
                   int       bmpID,
                   HoldPoint holdPoint = TopLeft,
                   Style     style     = NoBorder,
                   Align     align     = NoCenter,
                   TModule*  module    = NULL);

	~TBitmapControl ();

// Member Functions
public:
	void	SetupWindow ();
	void	Paint (TDC& dc, bool bErase, TRect& rc);

protected:
   void  PaintBitmap(TDC& dc,TRect& clientRect);
   void  PaintBorder(TDC& dc,TRect& client);

private:
	void  SetupInstance(int bmpID);
   void  AdjustFrame();
   bool  LoadBitmapResource(int resId);
   void  SetupFromDib(TDib* dib);
   bool  UpdatePalette(bool alwaysRepaint);
   void  DrawPlainBorder(TDC& dc,TRect& client);
   void  DrawRecessedBorder(TDC& dc,TRect& client);
   void  DrawRaisedBorder(TDC& dc,TRect& client);
   void  DrawFancyBorder(TDC &dc,TRect &client,
                         TPen &penShadow,TPen &penHilite,
                         TPen &penGrey,TPen &penBlack);
};

#endif // BMPCTL_H_