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
#include <owl\except.h>
#include <owltools\hlscolor.h>

#define EPS 10e-6
#define zero_val(x) (-EPS < (x) && (x) < EPS)

//
// THLSColor
//

THLSColor::THLSColor(double h,double l,double s) :
TColor()
{
   int red, green, blue;

   PRECONDITIONX(h < 360.,"Hue is out of range");
   PRECONDITIONX(0. <= l && l <= 1. ,"Lightness is out of range");
   PRECONDITIONX(0. <= s && s <= 1. ,"Saturation is out of range");

   THLSColor::HLStoRGB(h,l,s,red,green,blue);

   SetValue(RGB(red,green,blue));
}

// HLStoRGB
//
// Converts hls to rgb
// h in [0,360) (or less than zero = UNDEFINED)
// l and s in [0,1]
// r,g,b each in [0,255]
void THLSColor::HLStoRGB(double h,double l,double s,int &r,int &g,int &b)
{
   double red,green,blue;
   double m1,m2;

   PRECONDITIONX(h < 360.,"Hue is out of range");
   PRECONDITIONX(0. <= l && l <= 1. ,"Lightness is out of range");
   PRECONDITIONX(0. <= s && s <= 1. ,"Saturation is out of range");

   if (l <= 0.5)
      m2 = l*(1 + s);
   else
      m2 = l + s - l*s;

   m1 = 2*l - m2;

   if (zero_val(s))
      {
      // Achromatic : there is no hue
      if (h < 0) // UNDEFINED
         {
         red   = l;
         green = l;
         blue  = l;
         }
      else
         {
         // Major error... Throw an exception
         // if hue is undefined, saturation must be zero...
         THROW(TXOwl("THLSColor::HLStoRGB() ~ Conversion error"));
         }
      }
   else
      {
      // Chromatic case, so there is a hue
      red   = ComputeValue(m1,m2,h + 120.);
      green = ComputeValue(m1,m2,h);
      blue  = ComputeValue(m1,m2,h - 120.);
      }

   // Convert to r,g,b values for Windows
   // The 0.001 provides just a little "boost" for those values that
   // are just about the next whole number so that they get truncated
   // to int's properly (e.g. 1.9995 normally becomes 1, but the correct
   // answer in this implementation is 2, so 1.9995 + 0.001 = 2.0005, or 2
   // when represented as an int). If a number is something like 5.0032 adding
   // 0.001 wont have any effect.
   r = red*255. + 0.001;
   g = green*255. + 0.001;
   b = blue*255. + 0.001;
}

// RGBtoHLS
//
// Converts rgb to hls.
// r,g,b each in [0,255]
// h in [0,360)
// l and s in [0,1], except if s = 0, then h = (less than zero - UNDEFINED)
void THLSColor::RGBtoHLS(int r,int g,int b,double &h,double &l,double &s)
{
   double maxColor, minColor;
   double delta;
   double red, green, blue;

   PRECONDITIONX(0 <= r && r <= 255,"Red is out of range");
   PRECONDITIONX(0 <= b && b <= 255,"Blue is out of range");
   PRECONDITIONX(0 <= g && g <= 255,"Green is out of range");

   // convert r,g,b to [0,1]
   red   = r/255.;
   green = g/255.;
   blue  = b/255.;

   maxColor = max(max(red,green),blue);
   minColor = min(min(red,green),blue);

   // Compute lightness
   l = (maxColor + minColor)/2;

   // Compute saturation
   if (zero_val(maxColor - minColor))
      {
      // Achromatic case, because r=b=g
      s = 0.0;
      h = -1.0; // UNDEFINED
      }
   else
      {
      // Chromatic case
      // First calculate saturation
      if (l <= 0.5)
         s = (maxColor - minColor)/(maxColor + minColor);
      else
         s = (maxColor - minColor)/(2 - maxColor - minColor);

      // Now, calculate hue
      delta = maxColor - minColor;
      if (zero_val(red - maxColor))
         h = (green - blue)/delta; // resulting color is between yellow and magenta
      else if (zero_val(green - maxColor))
         h = 2 + (blue - red)/delta; // resulting color is between cyan and yellow
      else if (zero_val(blue - maxColor))
         h = 4 + (red - green)/delta; // resulting color is between magenta and cyan

      // convert to degrees
      h *= 60.;

      // Make degrees be nonnegative
      if (h < 0)
         h += 360.;
      }

}

double THLSColor::GetHue()
{
   double h,l,s;
   THLSColor::RGBtoHLS(Red(),Green(),Blue(),h,l,s);
   return h;
}

double THLSColor::GetLightness()
{
   double h,l,s;
   THLSColor::RGBtoHLS(Red(),Green(),Blue(),h,l,s);
   return l;
}

double THLSColor::GetSaturation()
{
   double h,l,s;
   THLSColor::RGBtoHLS(Red(),Green(),Blue(),h,l,s);
   return s;
}

double THLSColor::ComputeValue(double n1,double n2,double hue)
{
   double value;

   if (hue > 360.)
      hue -= 360.;
   else if (hue < 0.)
      hue += 360.;

   if (hue < 60.)
      value = n1 + (n2 - n1)*hue/60.;
   else if (hue < 180.)
      value = n2;
   else if (hue < 240.)
      value = n1 + (n2 - n1)*(240. - hue)/60.;
   else
      value = n1;

   return value;
}

#if defined (GEN_DEBUG_CODE)

bool THLSColor::Test()
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

            RGBtoHLS(rIn,gIn,bIn,h,l,s);
            HLStoRGB(h,l,s,rOut,gOut,bOut);

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

