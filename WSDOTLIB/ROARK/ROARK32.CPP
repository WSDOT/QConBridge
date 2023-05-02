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

FFIntermediateLoad::FFIntermediateLoad(double w,double La,double l,double e,double i) :
RoarkBeam(l,e,i)
{
   a = La;
   b = L - a;
   W = w;
}


void FFIntermediateLoad::GetReactions(double &Ra,double &Rb)
{
   Ra = W*b*b*(3*a + b)/pow(L,3);
   Rb = W*a*a*(3*b + a)/pow(L,3);
}

void FFIntermediateLoad::GetMoments(double &Ma,double &Mb)
{
   Ma = W*a*b*b/pow(L,2);
   Mb = W*b*a*a/pow(L,2);
}

void FFIntermediateLoad::GetRotations(double &ra,double &rb)
{
   ra = 0;
   rb = 0;
}

void FFIntermediateLoad::GetDeflections(double &Ya,double &Yb)
{
   Ya = 0;
   Yb = 0;
}

double FFIntermediateLoad::ComputeShear(double x)
{
   double V;
   double Ra,Rb;

   GetReactions(Ra,Rb);

   if (x < a)
      V = Ra;
   else
      V = Ra - W;

   return V;
}

double FFIntermediateLoad::ComputeMoment(double x)
{
   double M;
   double Ra, Rb;
   double Ma, Mb;

   GetReactions(Ra,Rb);
   GetMoments(Ma,Mb);

   if (x < a)
      M = -Ma + Ra*x;
   else
      M = -Ma + Ra*x - W*(x - a);

   return M;
}

double FFIntermediateLoad::ComputeDeflection(double x)
{
   double y;

   double Ra, Rb;

   GetReactions(Ra,Rb);

   if (x < a)
      y = (W*b*b*x*x*(3*a*x + b*x - 3*a*L))/(6*EI*L*L*L);
   else
      y = (W*a*a*pow((L-x),2)*((3*b + a)*(L-x) - 3*b*L))/(6*EI*L*L*L);

   return y;
}