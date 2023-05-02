///////////////////////////////////////////////////////////////////////
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
// May 1997 - Created, Richard Brice, PE
// Oct 1999 - Released as Open Source

#include "roark.h"

PFPartLeftTriangleLoad::PFPartLeftTriangleLoad(double w,double La,double Lb,double l,double e,double i) :
RoarkBeam(l,e,i)
{
   a = L - Lb;
   b = L - La;
   c = b - a;
   d = l - b;

   W = 0.5*w*c;
}


void PFPartLeftTriangleLoad::GetReactions(double &Ra,double &Rb)
{
   Ra = W*(L*(10*a*b + 5*a*a + 15*b*b) - a*a*a - 2*a*b - 3*a*b*b - 4*b*b*b)/(20*L*L*L);
   Rb = W - Ra;
}

void PFPartLeftTriangleLoad::GetMoments(double &Ma,double &Mb)
{
   double Ra, Rb;
   GetReactions(Ra,Rb);

   Ma = 0;
   Mb = -Ra*L + W*(a + 2*b)/3;
}

void PFPartLeftTriangleLoad::GetRotations(double &ra,double &rb)
{
   double Ra, Rb;
   GetReactions(Ra,Rb);

   ra = -(Ra*L*L/2 - W*(0.5*a*a + 2*a*c/3 + 0.25*c*c))/EI;
   rb = 0;
}

void PFPartLeftTriangleLoad::GetDeflections(double &Ya,double &Yb)
{
   Ya = 0;
   Yb = 0;
}

double PFPartLeftTriangleLoad::ComputeShear(double x)
{
   double V;
   double Ra, Rb;
   GetReactions(Ra,Rb);

   if (x < d)
      V = Ra;
   else if (x < d+c)
      V = Ra - W*2*(x-d)/c + W*pow((x-d),2)/(c*c);
   else
      V = Ra - W;

   return V;
}

double PFPartLeftTriangleLoad::ComputeMoment(double x)
{
   double M;
   double Ra, Rb;
   GetReactions(Ra,Rb);

   if (x < d)
      M = Ra*x;
   else if (x < d+c)
      M = Ra*x - W*pow((x-d),2)/c + W*pow((x-d),3)/(3*c*c);
   else
      M = Ra*x - W*(x - d - c/3);

   return M;
}

double PFPartLeftTriangleLoad::ComputeDeflection(double x)
{
   double Y;
   double Ra, Rb;
   GetReactions(Ra,Rb);

   if (x < d)
      {
      Y = Ra*(x*x*x/6 - L*L*x/2) + W*x*(a*a/2 + 2*a*c/3 + c*c/4);
      Y /= EI;
      }
   else if (x < d+c)
      {
      Y = Ra*(x*x*x/6 - L*L*x/2) + W*(pow((x-d),5)/(60*c*c) - pow((x-d),4)/(12*c) + x*(a*a/c + 2*a*c/3 + c*c/4));
      Y /= EI;
      }
   else
      {
      Y = Ra*(x*x*x/6 - L*L*x/2 + L*L*L/3);
      Y -= 0.5*W*(L-x)*(L-x)*(a + 2*c/3 - (L-x)/3);
      Y /= EI;
      }

   return Y;
}

//
//
//
FPPartRightTriangleLoad::FPPartRightTriangleLoad(double w,double La,double Lb,double l,double e,double i) :
PFPartLeftTriangleLoad(w,l-La,l-Lb,l,e,i)
{
}


void FPPartRightTriangleLoad::GetReactions(double &Ra,double &Rb)
{
   double Temp;

   PFPartLeftTriangleLoad::GetReactions(Ra,Rb);
   Temp = Ra;
   Ra = Rb;
   Rb = Temp;
}

void FPPartRightTriangleLoad::GetMoments(double &Ma,double &Mb)
{
   double Temp;

   PFPartLeftTriangleLoad::GetMoments(Ma,Mb);
   Temp = Ma;
   Ma = Mb;
   Mb = Temp;
}

void FPPartRightTriangleLoad::GetRotations(double &ra,double &rb)
{
   double Temp;

   PFPartLeftTriangleLoad::GetRotations(ra,rb);
   Temp = ra;
   ra = rb;
   rb = Temp;
}

void FPPartRightTriangleLoad::GetDeflections(double &Ya,double &Yb)
{
   double Temp;

   PFPartLeftTriangleLoad::GetDeflections(Ya,Yb);
   Temp = Ya;
   Ya = Yb;
   Yb = Temp;
}

double FPPartRightTriangleLoad::ComputeShear(double x)
{
   return PFPartLeftTriangleLoad::ComputeShear(L-x);
}

double FPPartRightTriangleLoad::ComputeMoment(double x)
{
   return PFPartLeftTriangleLoad::ComputeMoment(L-x);
}

double FPPartRightTriangleLoad::ComputeDeflection(double x)
{
   return PFPartLeftTriangleLoad::ComputeDeflection(L-x);
}