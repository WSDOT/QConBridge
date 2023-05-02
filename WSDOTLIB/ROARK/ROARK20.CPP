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

PPIntermediateCouple::PPIntermediateCouple(double M,double La,double l,double e,double i) :
RoarkBeam(l,e,i)
{
   a = La;
   Mo = M;
}


void PPIntermediateCouple::GetReactions(double &Ra,double &Rb)
{
   Ra = -Mo/L;
   Rb =  Mo/L;
}

void PPIntermediateCouple::GetMoments(double &Ma,double &Mb)
{
   Ma = 0;
   Mb = 0;
}

void PPIntermediateCouple::GetRotations(double &ra,double &rb)
{
   ra = -Mo*(2*L - 6*a + 3*a*a/L)/(6*EI);
   rb =  Mo*(L - 3*a*a/L)/(6*EI);
}

void PPIntermediateCouple::GetDeflections(double &Ya,double &Yb)
{
   Ya = 0;
   Yb = 0;
}

double PPIntermediateCouple::ComputeShear(double)
{
   double V;

   V = -Mo/L;

   return V;
}

double PPIntermediateCouple::ComputeMoment(double x)
{
   double M;
   
   if (x < a)
      M = -Mo*x/L;
   else
      M = Mo*(1 - x/L);

   return M;
}

double PPIntermediateCouple::ComputeDeflection(double x)
{
   double y;

   if (x < a)
      y = Mo*(x*(6*a - 3*a*a/L - 2*L) - pow(x,3)/L)/(6*EI);
   else
      y = Mo*(3*a*a + 3*x*x - pow(x,3)/L - (2*L + 3*a*a/L)*x)/(6*EI);

   return y;
}
