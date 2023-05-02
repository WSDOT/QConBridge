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

PFIntermediateCouple::PFIntermediateCouple(double M,double La,double l,double e,double i) :
RoarkBeam(l,e,i)
{
   a = La;
   Mo = M;
}


void PFIntermediateCouple::GetReactions(double &Ra,double &Rb)
{
   Ra = -3*Mo*(L*L - a*a)/(2*L*L*L);
   Rb =  3*Mo*(L*L - a*a)/(2*L*L*L);
}

void PFIntermediateCouple::GetMoments(double &Ma,double &Mb)
{
   Ma = 0;
   Mb = 0.5*Mo*(1 - 3*a*a/(L*L));
}

void PFIntermediateCouple::GetRotations(double &ra,double &rb)
{
   ra = Mo*(a - 0.24*L - 0.75*a*a/L)/EI;
   rb = 0;
}

void PFIntermediateCouple::GetDeflections(double &Ya,double &Yb)
{
   Ya = 0;
   Yb = 0;
}

double PFIntermediateCouple::ComputeShear(double)
{
   double Ra, Rb;

   GetReactions(Ra,Rb);

   return Ra;
}

double PFIntermediateCouple::ComputeMoment(double x)
{
   double M;
   double Ra, Rb;

   GetReactions(Ra,Rb);
   if (x < a)
      M = Ra*x;
   else
      M = Ra*x + Mo;

   return M;
}

double PFIntermediateCouple::ComputeDeflection(double x)
{
   double y;

   if (x < a)
      y = Mo*((L*L - a*a)*(3*L*L*x - x*x*x)/(4*L*L*L) - (L-a)*x)/(EI);
   else
      y = Mo*((L*L - a*a)*(3*L*L*x - x*x*x)/(4*L*L*L) - L*x + 0.5*(x*x + a*a))/(EI);

   return y;
}

//
//
//
FPIntermediateCouple::FPIntermediateCouple(double M,double La,double l,double e,double i) :
PFIntermediateCouple(M,l-La,l,e,i)
{
}

void FPIntermediateCouple::GetReactions(double &Ra,double &Rb)
{
   double Temp;
   PFIntermediateCouple::GetReactions(Ra,Rb);
   Temp = Ra;
   Ra = Rb;
   Rb = Temp;
}

void FPIntermediateCouple::GetMoments(double &Ma,double &Mb)
{
   double Temp;
   PFIntermediateCouple::GetMoments(Ma,Mb);
   Temp = Ma;
   Ma = Mb;
   Mb = Temp;
}

void FPIntermediateCouple::GetRotations(double &ra,double &rb)
{
   double Temp;
   PFIntermediateCouple::GetRotations(ra,rb);
   Temp = ra;
   ra = rb;
   rb = Temp;
}

void FPIntermediateCouple::GetDeflections(double &Ya,double &Yb)
{
   double Temp;
   PFIntermediateCouple::GetDeflections(Ya,Yb);
   Temp = Ya;
   Ya = Yb;
   Yb = Temp;
}

double FPIntermediateCouple::ComputeShear(double x)
{
   return PFIntermediateCouple::ComputeShear(L-x);
}

double FPIntermediateCouple::ComputeMoment(double x)
{
   return PFIntermediateCouple::ComputeMoment(L-x);
}

double FPIntermediateCouple::ComputeDeflection(double x)
{
   return PFIntermediateCouple::ComputeDeflection(L-x);
}

