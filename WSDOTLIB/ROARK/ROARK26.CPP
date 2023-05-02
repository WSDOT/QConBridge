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

PFPartRightTriangleLoad::PFPartRightTriangleLoad(double w,double La,double Lb,double l,double e,double i) :
RoarkBeam(l,e,i)
{
   a = L - Lb;
   b = L - La;
   c = b - a;
   d = l - b;

   W = 0.5*w*c;
}


void PFPartRightTriangleLoad::GetReactions(double &Ra,double &Rb)
{
   Ra = W*(L*(10*a*b + 15*a*a + 5*b*b) - 4*a*a*a - 2*a*b*b - 3*a*a*b - b*b*b)/(20*L*L*L);
   Rb = W - Ra;
}

void PFPartRightTriangleLoad::GetMoments(double &Ma,double &Mb)
{
   double Ra, Rb;
   GetReactions(Ra,Rb);

   Ma = 0;
   Mb = -Ra*L + W*(2*a + b)/3;
}

void PFPartRightTriangleLoad::GetRotations(double &ra,double &rb)
{
   double Ra, Rb;
   GetReactions(Ra,Rb);

   ra = -(Ra*L*L/2 - W*(c*c/12 + a*c/3 + a*a/2))/EI;
   rb = 0;
}

void PFPartRightTriangleLoad::GetDeflections(double &Ya,double &Yb)
{
   Ya = 0;
   Yb = 0;
}

double PFPartRightTriangleLoad::ComputeShear(double x)
{
   double V;
   double Ra, Rb;
   GetReactions(Ra,Rb);

   if (x < d)
      V = Ra;
   else if (x < d+c)
      V = Ra - W*pow((x - d)/c,2);
   else
      V = Ra - W;

   return V;
}

double PFPartRightTriangleLoad::ComputeMoment(double x)
{
   double M;
   double Ra, Rb;
   GetReactions(Ra,Rb);

   if (x < d)
      M = Ra*x;
   else if (x < d+c)
      M = Ra*x - W*pow((x-d),3)/(3*c*c);
   else
      M = Ra*x - W*(x - d - 2*c/3);

   return M;
}

double PFPartRightTriangleLoad::ComputeDeflection(double x)
{
   double Y;
   double Ra, Rb;
   GetReactions(Ra,Rb);

   if (x < d)
      {
      Y = Ra*(x*x*x/6 - L*L*x/2) + W*x*(a*a/2 + a*c/3 + c*c/12);
      Y /= EI;
      }
   else if (x < d+c)
      {
      Y = Ra*(x*x*x/6 - L*L*x/2) + W*x*(a*a/2 + a*c/3 + c*c/12) - W*pow((x-d),5)/(60*c*c);
      Y /= EI;
      }
   else
      {
      Y = Ra*(x*x*x/6 - L*L*x/2 + L*L*L/3);
      Y += W*(0.5*pow((a + c/3),2)*x - pow((x - d- 2*c/3),3)/6 + pow((a + c/3),3)/6 - pow((a + c/3),2)*(L/2));
      Y /= EI;
      }

   return Y;
}

//
//
//
FPPartLeftTriangleLoad::FPPartLeftTriangleLoad(double w,double La,double Lb,double l,double e,double i) :
PFPartRightTriangleLoad(w,l-La,l-Lb,l,e,i)
{
}


void FPPartLeftTriangleLoad::GetReactions(double &Ra,double &Rb)
{
   double Temp;

   PFPartRightTriangleLoad::GetReactions(Ra,Rb);
   Temp = Ra;
   Ra = Rb;
   Rb = Temp;
}

void FPPartLeftTriangleLoad::GetMoments(double &Ma,double &Mb)
{
   double Temp;

   PFPartRightTriangleLoad::GetMoments(Ma,Mb);
   Temp = Ma;
   Ma = Mb;
   Mb = Temp;
}

void FPPartLeftTriangleLoad::GetRotations(double &ra,double &rb)
{
   double Temp;

   PFPartRightTriangleLoad::GetRotations(ra,rb);
   Temp = ra;
   ra = rb;
   rb = Temp;
}

void FPPartLeftTriangleLoad::GetDeflections(double &Ya,double &Yb)
{
   double Temp;

   PFPartRightTriangleLoad::GetDeflections(Ya,Yb);
   Temp = Ya;
   Ya = Yb;
   Yb = Temp;
}

double FPPartLeftTriangleLoad::ComputeShear(double x)
{
   return PFPartRightTriangleLoad::ComputeShear(L-x);
}

double FPPartLeftTriangleLoad::ComputeMoment(double x)
{
   return PFPartRightTriangleLoad::ComputeMoment(L-x);
}

double FPPartLeftTriangleLoad::ComputeDeflection(double x)
{
   return PFPartRightTriangleLoad::ComputeDeflection(L-x);
}