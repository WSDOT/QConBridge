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

CFIntermediateLoad::CFIntermediateLoad(double w,double Lb,double l,double e,double i) :
RoarkBeam(l,e,i)
{
   b = Lb;
   a = L - b;
   W = w;
}

void CFIntermediateLoad::GetReactions(double &Ra,double &Rb)
{
   Ra = 0;
   Rb = W;
}

void CFIntermediateLoad::GetMoments(double &Ma,double &Mb)
{
   Ma = 0;
   Mb = -W*a;
}

void CFIntermediateLoad::GetRotations(double &ra,double &rb)
{
   ra = W*pow(a,2)/(2*EI);
   rb = 0;
}

void CFIntermediateLoad::GetDeflections(double &Ya,double &Yb)
{
   Ya = -(W/(6*EI))*(3*pow(a,2)*L - pow(a,3));
   Yb = 0;
}

double CFIntermediateLoad::ComputeShear(double x)
{
   double V;

   if (x < b)
      V = 0;
   else
      V = -W;

   return V;
}

double CFIntermediateLoad::ComputeMoment(double x)
{
   double M;

   if (x < b)
      M = 0;
   else
      M = -W*(x - b);

   return M;
}

double CFIntermediateLoad::ComputeDeflection(double x)
{
   double y;

   y = -W/(6*EI);
   if (x < b)
      y *= (-pow(a,3) + 3*pow(a,2)*L - 3*pow(a,2)*x);
   else
      y *= (pow((x-b),3) - 3*pow(a,2)*(x-b) + 2*pow(a,3));

   return y;
}

