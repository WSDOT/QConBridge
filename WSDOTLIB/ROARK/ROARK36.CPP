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

FFPartRightTriangleLoad::FFPartRightTriangleLoad(double La,double Lb,double w,double l,double e,double i) :
RoarkBeam(l,e,i)
{
   a = La;
   b = Lb;
   c = b - a;
   d = L - a/3 - 2*b/3;
   W = 0.5*w*c;
}


void FFPartRightTriangleLoad::GetReactions(double &Ra,double &Rb)
{
   Ra = W*(3*d*d - c*c/6 + b*c*c/(3*L) - 17*c*c*c/(135*L) - 2*d*d*d/L)/(L*L);
   Rb = W - Ra;
}

void FFPartRightTriangleLoad::GetMoments(double &Ma,double &Mb)
{
   Ma = -W*(d*d*d/L + c*c/9 + (51/810)*(c*c*c/L) - c*c*b/(6*L) - d*d)/L;
   Mb =  W*(d*d*d/L + c*c/18 + (51/810)*(c*c*c/L) - c*c*b/(6*L) - 2*d*d + d*L)/L;
}

void FFPartRightTriangleLoad::GetRotations(double &ra,double &rb)
{
   ra = 0;
   rb = 0;
}

void FFPartRightTriangleLoad::GetDeflections(double &Ya,double &Yb)
{
   Ya = 0;
   Yb = 0;
}

double FFPartRightTriangleLoad::ComputeShear(double x)
{
   double V;
   double Ra, Rb;

   GetReactions(Ra,Rb);

   if (x < a)
      V = Ra;
   else if (x < b)
      V = Ra - W*pow((x-a),2)/(c*c);
   else
      V = Ra - W;

   return V;
}

double FFPartRightTriangleLoad::ComputeMoment(double x)
{
   double M;
   double Ra, Rb;
   double Ma, Mb;

   GetReactions(Ra,Rb);
   GetMoments(Ma,Mb);

   if (x < a)
      M = -Ma + Ra*x;
   else if (x < b)
      M = -Ma + Ra*x - W*pow((x-a),3)/(3*c*c);
   else
      M = -Ma + Ra*x - W*(x - L + d);

   return M;
}

double FFPartRightTriangleLoad::ComputeDeflection(double x)
{
   double y;
   double Ra, Rb;
   double Ma, Mb;

   GetReactions(Ra,Rb);
   GetMoments(Ma,Mb);

   if (x < a)
      y = (Ra*x*x*x - 3*Ma*x*x)/(6*EI);
   else if (x < b)
      y = (Ra*x*x*x/6 - Ma*x*x/2 - W*pow((x-a),5)/(60*c*c))/EI;
   else
      y = (Ra*(x*x*x - 3*L*L*x + 2*L*L*L) - 3*Ma*(L-x)*(L-x)
          + W*(3*d*d*x + d*d*d - 3*d*d*L - pow((x - L + d),3)))/(6*EI);

   return y;
}

//
//
//
FFPartLeftTriangleLoad::FFPartLeftTriangleLoad(double La,double Lb,double w,double l,double e,double i) :
FFPartRightTriangleLoad(l-Lb,l-La,w,l,e,i)
{
}


void FFPartLeftTriangleLoad::GetReactions(double &Ra,double &Rb)
{
   double Temp;

   FFPartRightTriangleLoad::GetReactions(Ra,Rb);
   Temp = Ra;
   Ra = Rb;
   Rb = Temp;
}

void FFPartLeftTriangleLoad::GetMoments(double &Ma,double &Mb)
{
   double Temp;

   FFPartRightTriangleLoad::GetMoments(Ma,Mb);
   Temp = Ma;
   Ma = Mb;
   Mb = Temp;
}

void FFPartLeftTriangleLoad::GetRotations(double &ra,double &rb)
{
   double Temp;

   FFPartRightTriangleLoad::GetRotations(ra,rb);
   Temp = ra;
   ra = rb;
   rb = Temp;
}

void FFPartLeftTriangleLoad::GetDeflections(double &Ya,double &Yb)
{
   double Temp;

   FFPartRightTriangleLoad::GetDeflections(Ya,Yb);
   Temp = Ya;
   Ya = Yb;
   Yb = Temp;
}

double FFPartLeftTriangleLoad::ComputeShear(double x)
{
   return FFPartRightTriangleLoad::ComputeShear(L-x);
}

double FFPartLeftTriangleLoad::ComputeMoment(double x)
{
   return FFPartRightTriangleLoad::ComputeMoment(L-x);
}

double FFPartLeftTriangleLoad::ComputeDeflection(double x)
{
   return FFPartRightTriangleLoad::ComputeDeflection(L-x);
}

