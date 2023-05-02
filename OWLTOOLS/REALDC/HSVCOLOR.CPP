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

#include <checks.h>
#include <math.h>
#include <owl\except.h>
#include <owltools\hsvcolor.h>


#define EPS 10e-6
#define zero_val(x) (-EPS < (x) && (x) < EPS)

//
// THSVColor
//

THSVColor::THSVColor(double h,double s,double v) :
TColor()
{
   int red, green, blue;

   PRECONDITIONX(h < 360.,"Hue is out of range");
   PRECONDITIONX(0. <= s && s <= 1.,"Saturation is out of range");
   PRECONDITIONX(s  <= v && v <= 1.,"Value(brightness) is out of range");

   THSVColor::HSVtoRGB(h,s,v,red,green,blue);

   SetValue(RGB(red,green,blue));
}


// HSVtoRGB
//
// Converts hsv to rgb
// h in [0,360) (or less than zero = UNDEFINED)
// s and v in [0,1]
// r,g,b each in [0,255]
void THSVColor::HSVtoRGB(double h,double s,double v,int &r,int &g,int &b)
{
   double red,green,blue;
   double f,p,q,t;
   int i;

   PRECONDITIONX(h < 360.,"Hue is out of range");
   PRECONDITIONX(0. <= s && s <= 1.,"Saturation is out of range");
   PRECONDITIONX(s  <= v && v <= 1.,"Value(brightness) is out of range");

   if (zero_val(s))
      {
      // color is on the black-and-white centerline
      if (h < 0) // UNDEFINED
         {
         // Achromatic color : There is no hue
         red   = v;
         green = v;
         blue  = v;
         }
      else
         {
         // Major error... Throw an exception
         // if hue is undefined, saturation must be zero...
         THROW(TXOwl("THSVColor::HSVtoRGB() ~ Conversion error"));
         }
      }
   else
      {
      // Chromatic case
      h /= 60.;
      i = floor(h);
      f = h - i;
      p = v*(1 - s);
      q = v*(1 - s*f);
      t = v*(1 - s*(1 - f));
      switch (i)
         {
         case 0:
              red   = v;
              green = t;
              blue  = p;
              break;

         case 1:
              red   = q;
              green = v;
              blue  = p;
              break;

         case 2:
              red   = p;
              green = v;
              blue  = t;
              break;

         case 3:
              red   = p;
              green = q;
              blue  = v;
              break;

         case 4:
              red   = t;
              green = p;
              blue  = v;
              break;

         case 5:
              red   = v;
              green = p;
              blue  = q;
              break;
         }
      }

   // Convert to r,g,b values for Windows
   // The 0.001 provides just a little "boost" for those values that
   // are just about the next whole number so that they get truncated
   // to int's properly (e.g. 1.9995 normally becomes 1, but the correct
   // answer in this implementation is 2, so 1.9995 + 0.001 = 2.0005, or 2
   // when represented as an int). If a number is something like 5.0032 adding
   // 0.001 wont have any effect.
   r = red*255.   + 0.001;
   g = green*255. + 0.001;
   b = blue*255.  + 0.001;
}

// RGBtoHSV
//
// Converts rgb to hsv.
// r,g,b each in [0,255]
// h in [0,360)
// s and v in [0,1], except if s = 0, then h = (less than zero - UNDEFINED)
void THSVColor::RGBtoHSV(int r,int g,int b,double &h,double &s,double &v)
{
   double red,green,blue;
   double maxColor, minColor;
   double delta;

   PRECONDITIONX(0 <= r && r <= 255,"Red is out of range");
   PRECONDITIONX(0 <= b && b <= 255,"Blue is out of range");
   PRECONDITIONX(0 <= g && g <= 255,"Green is out of range");

   // Convert from Windows color numbers to normalized values [0,1]
   red   = r/255.;
   green = g/255.;
   blue   = b/255.;

   maxColor = max(max(red,green),blue);
   minColor = min(min(red,green),blue);

   v = maxColor; // This is the value (Brightness)

   // calculate saturation
   if (!zero_val(maxColor))
      s = (maxColor - minColor)/maxColor;
   else
      s = 0.;

   if (zero_val(s))
      {
      // Achromatic case
      h = -1; // UNDEFINED
      }
   else
      {
      // Chromatic case
      delta = maxColor - minColor;
      if (zero_val(red - maxColor))
         h = (green - blue)/delta; // Resulting color is between yellow and magenta
      else if(zero_val(green - maxColor))
         h = 2 + (blue - red)/delta; // Resulting color is between cyan and yellow
      else if(zero_val(blue - maxColor))
         h = 4 + (red - green)/delta; // Resulting color is between magenta and cyan

      h *= 60.;

      // make sure hue is nonnegative
      if (h < 0)
         h += 360;
      }


}

double THSVColor::GetHue()
{
   double h,s,v;
   THSVColor::RGBtoHSV(Red(),Green(),Blue(),h,s,v);
   return h;
}

double THSVColor::GetSaturation()
{
   double h,s,v;
   THSVColor::RGBtoHSV(Red(),Green(),Blue(),h,s,v);
   return s;
}

double THSVColor::GetBrightness()
{
   double h,s,v;
   THSVColor::RGBtoHSV(Red(),Green(),Blue(),h,s,v);
   return v;
}

#if defined (GEN_DEBUG_CODE)

bool THSVColor::Test()
{
   bool success = true; // assume successful
   int i,j,k;
   int rIn,gIn,bIn;
   int rOut,gOut,bOut;
   double h,l,s;

   for (i = 0; i < 255; i++)
      {
      for (j = 0; j < 255; j++)
         {
         for (k = 0; k < 255; k++)
            {
            rIn = i;
            gIn = j;
            bIn = k;

            RGBtoHSV(rIn,gIn,bIn,h,l,s);
            HSVtoRGB(h,l,s,rOut,gOut,bOut);

            if (rIn != rOut || gIn != gOut || bIn != bOut)
               {
               success = false;
               break;
               }
            }
         if (!success)
            break;
         }
      if (!success)
         break;
      }

   return success;
}

#endif
