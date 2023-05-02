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

PPIntermediateLoad::PPIntermediateLoad(double w,double La,double l,double e,double i) :
RoarkBeam(l,e,i)
{
   a = La;
   b = L - a;
   W = w;
}


void PPIntermediateLoad::GetReactions(double &Ra,double &Rb)
{
   Ra = W*b/L;
   Rb = W*a/L;
}

void PPIntermediateLoad::GetMoments(double &Ma,double &Mb)
{
   Ma = 0;
   Mb = 0;
}

void PPIntermediateLoad::GetRotations(double &ra,double &rb)
{
   ra = -W*(b*L - pow(b,3)/L)/(6*EI);
   rb =  W*(2*b*L + pow(b,3)/L - 3*b*b)/(6*EI);
}

void PPIntermediateLoad::GetDeflections(double &Ya,double &Yb)
{
   Ya = 0;
   Yb = 0;
}

double PPIntermediateLoad::ComputeShear(double x)
{
   double V;

   if (x < a)
      V = W*b/L;
   else
      V = -W*a/L;

   return V;
}

double PPIntermediateLoad::ComputeMoment(double x)
{
   double M;

   if (x < a)
      M = W*b*x/L;
   else
      M = W*a*(L-x)/L;

   return M;
}

double PPIntermediateLoad::ComputeDeflection(double x)
{
   double y;

   if (x < a)
      y = -W*b*x*(2*L*(L-x) - b*b - pow((L-x),2))/(6*EI*L);
   else
      y = -W*a*(L-x)*(2*L*b - b*b - pow((L-x),2))/(6*EI*L);

   return y;
}

