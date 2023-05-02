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


PPPartRightTriangleLoad::PPPartRightTriangleLoad(double La,double Lb,double w,double l,double e,double i) :
RoarkBeam(l,e,i)
{
   a = La;
   b = Lb;
   c = b - a;
   d = l - a/3. - 2.*b/3.;

   W = 0.5*w*c;
}


void PPPartRightTriangleLoad::GetReactions(double &Ra,double &Rb)
{
   Ra = W*d/L;
   Rb = W*(L - d)/L;
}

void PPPartRightTriangleLoad::GetMoments(double &Ma,double &Mb)
{
   Ma = 0.;
   Mb = 0.;
}

void PPPartRightTriangleLoad::GetRotations(double &ra,double &rb)
{
   double Ra, Rb;

   GetReactions(Ra,Rb);
   ra = (-Ra*L*L + W*(d*d*d/L + c*c/6 + 17*c*c*c/(270*L) - c*c*b/(6*L)))/(6*EI);
   rb = (2*Ra*L*L + W*(d*d*d/L + 17*c*c*c/(270*L) - c*c*b/(6*L) - 3*d*d))/(6*EI);
}

void PPPartRightTriangleLoad::GetDeflections(double &Ya,double &Yb)
{
   Ya = 0;
   Yb = 0;
}

double PPPartRightTriangleLoad::ComputeShear(double x)
{
   double V;
   double Ra,Rb;

   GetReactions(Ra,Rb);

   if (x < a)
      V = Ra;
   else if (x < b)
      V = Ra - W*pow((x - a)/c,2);
   else
      V = Ra - W;

   return V;
}

double PPPartRightTriangleLoad::ComputeMoment(double x)
{
   double M;
   double Ra, Rb;

   GetReactions(Ra,Rb);

   if (x < a)
      M = Ra*x;
   else if (x < b)
      M = Ra*x - W*pow((x-a),3)/(3*c*c);
   else
      M = Ra*x - W*(3*x - a - 2*b)/3;

   return M;
}

double PPPartRightTriangleLoad::ComputeDeflection(double x)
{
   double y;

   double Ra, Rb;

   GetReactions(Ra,Rb);

   if (x < a)
      {
      y = (Ra*(pow(x,3) - L*L*x) + W*x*(pow(d,3)/L + (c*c/6)*(1-b/L) + 17*pow(c,3)/(270*L)))/(6*EI);
      }
   else if (x < b)
      {
      y = (Ra*(pow(x,3) - L*L*x) - W*pow((x-a),5)/(10*c*c) + W*x*(pow(d,3)/L + (c*c/6)*(1-b/L) + 17*pow(c,3)/(270*L)))/(6*EI);
      }
   else
      {
      y = (Ra*(pow(x,3) - L*L*x) - W*(pow((x-(a/3)-(2*b/3)),3) - pow(d,3)*x/L - (b*c*c/6)*(1-x/L) + 17*pow(c,3)*(1-x/L)/270 ))/(6*EI);
      }

   return y;
}

//
//
//
PPPartLeftTriangleLoad::PPPartLeftTriangleLoad(double La,double Lb,double w,double l,double e,double i) :
PPPartRightTriangleLoad(l - La,l - Lb,w,l,e,i)
{
}


void PPPartLeftTriangleLoad::GetReactions(double &Ra,double &Rb)
{
   double Temp;
   PPPartRightTriangleLoad::GetReactions(Ra,Rb);
   Temp = Ra;
   Ra = Rb;
   Rb = Temp;
}

void PPPartLeftTriangleLoad::GetMoments(double &Ma,double &Mb)
{
   double Temp;
   PPPartRightTriangleLoad::GetMoments(Ma,Mb);
   Temp = Ma;
   Ma = Mb;
   Mb = Temp;
}

void PPPartLeftTriangleLoad::GetRotations(double &ra,double &rb)
{
   double Temp;
   PPPartRightTriangleLoad::GetRotations(ra,rb);
   Temp = ra;
   ra = rb;
   rb = Temp;
}

void PPPartLeftTriangleLoad::GetDeflections(double &Ya,double &Yb)
{
   double Temp;
   PPPartRightTriangleLoad::GetDeflections(Ya,Yb);
   Temp = Ya;
   Ya = Yb;
   Yb = Temp;
}

double PPPartLeftTriangleLoad::ComputeShear(double x)
{
   return PPPartRightTriangleLoad::ComputeShear(L - x);
}

double PPPartLeftTriangleLoad::ComputeMoment(double x)
{
   return PPPartRightTriangleLoad::ComputeMoment(L - x);
}

double PPPartLeftTriangleLoad::ComputeDeflection(double x)
{
   return PPPartRightTriangleLoad::ComputeDeflection(L - x);
}

