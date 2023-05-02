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

#include <owl\dc.h>
#include <math.h>
#include <owltools\fonthelp.h>

int PointSizeToFontHeight(int pointSize,TDC &dc)
{
   int logPixelsY;
   int fontHeight;
   int sign;
   TPoint p(0,0);

   sign = (pointSize < 0) ? -1 : 1; // Retain the sign of the point size

   logPixelsY = dc.GetDeviceCaps(LOGPIXELSY);
   p.y = ::MulDiv(pointSize,logPixelsY,72); // (pointSize/72)*logPixelsY = # of pixels
   dc.DPtoLP(&p); // convert to logical units

   fontHeight = sign*abs(p.y);

   return fontHeight;
}

int FontHeightToPointSize(int fontHeight,TDC &dc)
{
   int logPixelsY;
   int pointSize;
   int sign;
   TPoint p(0,0);

   sign = (fontHeight < 0) ? -1 : 1; // Retain the sign of the font height

   logPixelsY = dc.GetDeviceCaps(LOGPIXELSY);
   p.y = ::MulDiv(fontHeight,72,logPixelsY); // (# pixels * 72)/logPixelsY = pointSize
   dc.LPtoDP(&p); // convert from logical units

   pointSize = sign*abs(p.y);

   return pointSize;
}


TFont *CreateSimpleFont(TDC &dc,const char *typeFace,int pointSize,
                        int weight,uint8 italic,uint8 underline)
{
   TFont *pFont;

   int height = PointSizeToFontHeight(pointSize,dc);
   int width = 0;
   int escapement = 0;
   int orientation = 0;
   uint8 pitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
   uint8 strikeout = false;
   uint8 charSet = ANSI_CHARSET;
   uint8 outputPrecision = OUT_DEFAULT_PRECIS;
   uint8 clipPrecision = CLIP_DEFAULT_PRECIS;
   uint8 quality = DEFAULT_QUALITY;

   pFont = new TFont(typeFace,height,width,escapement,orientation,weight,
                     pitchAndFamily,italic,underline,strikeout,charSet,
                     outputPrecision,clipPrecision,quality);

   return pFont;
}

