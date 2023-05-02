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

FFIntermediateCouple::FFIntermediateCouple(double M,double La,double l,double e,double i) :
RoarkBeam(l,e,i)
{
   a = La;
   Mo = M;
}


void FFIntermediateCouple::GetReactions(double &Ra,double &Rb)
{
   Ra = -6*Mo*(a*L - a*a)/(L*L*L);
   Rb =  6*Mo*(a*L - a*a)/(L*L*L);
}

void FFIntermediateCouple::GetMoments(double &Ma,double &Mb)
{
   Ma = -Mo*(4*L*a - 3*a*a - L*L)/(L*L);
   Mb =  Mo*(2*L*a - 3*a*a)/(L*L);
}

void FFIntermediateCouple::GetRotations(double &ra,double &rb)
{
   ra = 0;
   rb = 0;
}

void FFIntermediateCouple::GetDeflections(double &Ya,double &Yb)
{
   Ya = 0;
   Yb = 0;
}

double FFIntermediateCouple::ComputeShear(double)
{
   double Ra, Rb;

   GetReactions(Ra,Rb);

   return Ra;
}

double FFIntermediateCouple::ComputeMoment(double x)
{
   double M;
   double Ra, Rb;
   double Ma, Mb;

   GetReactions(Ra,Rb);
   GetMoments(Ma,Mb);

   if (x < a)
      M = -Ma + Ra*x;
   else
      M = -Ma + Ra*x + Mo;

   return M;
}

double FFIntermediateCouple::ComputeDeflection(double x)
{
   double y;
   double Ra, Rb;
   double Ma, Mb;

   GetReactions(Ra,Rb);
   GetMoments(Ma,Mb);

   if (x < a)
      y = -(3*Ma*x*x - Ra*x*x*x)/(6*EI);
   else
      y = ((Mo-Ma)*(3*x*x - 6*L*x + 3*L*L) - Ra*(3*L*L*x - x*x*x - 2*L*L*L))/(6*EI);

   return y;
}

