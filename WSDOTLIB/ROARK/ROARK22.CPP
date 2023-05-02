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


PFIntermediateLoad::PFIntermediateLoad(double w,double Lb,double l,double e,double i) :
RoarkBeam(l,e,i)
{
   a = L - Lb;
   W = w;
}


void PFIntermediateLoad::GetReactions(double &Ra,double &Rb)
{
   Ra = 0.5*W*(3*a*a*L - a*a*a)/(L*L*L);
   Rb = W - Ra;
}

void PFIntermediateLoad::GetMoments(double &Ma,double &Mb)
{
   Ma = 0.5*W*(a*a*a + 2*a*L*L - 3*a*a*L)/(L*L);
   Mb = 0;
}

void PFIntermediateLoad::GetRotations(double &ra,double &rb)
{
   ra = W*(a*a*a/L - a*a)/(4*EI);
   rb = 0;
}

void PFIntermediateLoad::GetDeflections(double &Ya,double &Yb)
{
   Ya = 0;
   Yb = 0;
}

double PFIntermediateLoad::ComputeShear(double x)
{
   double V;
   double Ra,Rb;

   GetReactions(Ra,Rb);

   if (x < L-a)
      V = Ra;
   else
      V = Ra - W;

   return V;
}

double PFIntermediateLoad::ComputeMoment(double x)
{
   double M;
   double Ra, Rb;

   GetReactions(Ra,Rb);

   if (x < L-a)
      M = Ra*x;
   else
      M = Ra*x - W*(x - L + a);

   return M;
}

double PFIntermediateLoad::ComputeDeflection(double x)
{
   double y;

   double Ra, Rb;

   GetReactions(Ra,Rb);

   if (x < L-a)
      y = (Ra*(x*x*x - 3*L*L*x) + 3*W*a*a*x)/(6*EI);
   else
      y = (Ra*(x*x*x - 3*L*L*x) + W*(3*a*a*x - pow((x - (L-a)),3)))/(6*EI);

   return y;
}

//
//
//
FPIntermediateLoad::FPIntermediateLoad(double w,double Lb,double l,double e,double i) :
PFIntermediateLoad(w,l-Lb,l,e,i)
{
}


void FPIntermediateLoad::GetReactions(double &Ra,double &Rb)
{
   double Temp;

   PFIntermediateLoad::GetReactions(Ra,Rb);
   Temp = Ra;
   Ra = Rb;
   Rb = Temp;
}

void FPIntermediateLoad::GetMoments(double &Ma,double &Mb)
{
   double Temp;

   PFIntermediateLoad::GetMoments(Ma,Mb);
   Temp = Ma;
   Ma = Mb;
   Mb = Temp;
}

void FPIntermediateLoad::GetRotations(double &ra,double &rb)
{
   double Temp;

   PFIntermediateLoad::GetRotations(ra,rb);
   Temp = ra;
   ra = rb;
   rb = Temp;
}

void FPIntermediateLoad::GetDeflections(double &Ya,double &Yb)
{
   double Temp;

   PFIntermediateLoad::GetDeflections(Ya,Yb);
   Temp = Ya;
   Ya = Yb;
   Yb = Temp;
}

double FPIntermediateLoad::ComputeShear(double x)
{
   return PFIntermediateLoad::ComputeShear(L - x);
}

double FPIntermediateLoad::ComputeMoment(double x)
{
   return PFIntermediateLoad::ComputeMoment(L - x);
}

double FPIntermediateLoad::ComputeDeflection(double x)
{
   return PFIntermediateLoad::ComputeDeflection(L - x);
}
