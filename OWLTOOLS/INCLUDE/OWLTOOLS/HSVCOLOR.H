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

#if !defined (HSVCOLOR_H_)
#define HSVCOLOR_H_

#include <owltools\owltldll.h>
#include <owl\color.h>

// Hue-Saturation-Value(Brightness) Color model implementation of
// TColor.
//
// Reference : "Computer Graphics - Principles and Practice"
// Foley, van Dam, Feiner, Hughes, 1990 Addison-Wesly,
// ISBN 0-201-12110-7 pg 590 -593
//
// Note:
// Due to floating point to integer round off, a call to HSVtoRGB followed by
// a call to RGBtoHSV will not yield the original HSV values.
//
// A call to RGBtoHSV followed by a call to HSVtoRGB will yield the original
// RGB values.

class _OWLTOOLSCLASS THSVColor : public TColor
{
public:
   THSVColor(double h,double s,double v);
   static void HSVtoRGB(double h,double s,double v,int &r,int &g,int &b);
   static void RGBtoHSV(int r,int g,int b,double &h,double &s,double &v);

   double GetHue();
   double GetSaturation();
   double GetBrightness();

#if defined (GEN_DEBUG_CODE)
   static bool Test();
#endif
};

#endif // HSVCOLOR_H_