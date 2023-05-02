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

PFPartUniformLoad::PFPartUniformLoad(double w,double La,double Lb,double l,double e,double i) :
RoarkBeam(l,e,i)
{
   a = L - Lb;
   b = L - La;
   c = b - a;
   d = l - b;

   W = w*c;
}


void PFPartUniformLoad::GetReactions(double &Ra,double &Rb)
{
   Ra = W*(4*L*(a*a + a*b + b*b) - a*a*a - a*b*b - a*a*b - b*b*b)/(8*L*L*L);
   Rb = W - Ra;
}

void PFPartUniformLoad::GetMoments(double &Ma,double &Mb)
{
   double Ra, Rb;
   GetReactions(Ra,Rb);

   Ma = 0;
   Mb = -Ra*L + 0.5*W*(a + b);
}

void PFPartUniformLoad::GetRotations(double &ra,double &rb)
{
   double Ra, Rb;
   GetReactions(Ra,Rb);

   ra = -(0.5*Ra*L*L - W*(0.5*a*a + 0.5*a*c + c*c/6))/(EI);
   rb = 0;
}

void PFPartUniformLoad::GetDeflections(double &Ya,double &Yb)
{
   Ya = 0;
   Yb = 0;
}

double PFPartUniformLoad::ComputeShear(double x)
{
   double V;
   double Ra, Rb;
   GetReactions(Ra,Rb);

   if (x < d)
      V = Ra;
   else if (x < d+c)
      V = Ra - W*(x - d)/c;
   else
      V = Ra - W;

   return V;
}

double PFPartUniformLoad::ComputeMoment(double x)
{
   double M;
   double Ra, Rb;
   GetReactions(Ra,Rb);

   if (x < d)
      M = Ra*x;
   else if (x < d+c)
      M = Ra*x - W*pow((x-d),2)/(2*c);
   else
      M = Ra*x - W*(x - d - 0.5*c);

   return M;
}

double PFPartUniformLoad::ComputeDeflection(double x)
{
   double Y;
   double Ra, Rb;
   GetReactions(Ra,Rb);

   if (x < d)
      {
      Y = Ra*(x*x*x/6 - 0.5*L*L*x) + W*x*(0.5*a*a + 0.5*a*c +c*c/6);
      Y /= EI;
      }
   else if (x < d+c)
      {
      Y = Ra*(x*x*x/6 - 0.5*L*L*x) + W*x*(0.5*a*a + 0.5*a*c +c*c/6) - W*pow((x-d),4)/(24*c);
      Y /= EI;
      }
   else
      {
      Y = Ra*(x*x*x/6 - L*L*x/2 + L*L*L/3);
      Y += W*(pow((a + 0.5*c),3)/6 - 0.5*pow((a + 0.5*c),2)*L - pow((x - d - 0.5*c),3)/6 + 0.5*pow((a + 0.5*c),2)*x);
      Y /= EI;
      }

   return Y;
}

//
//
//
FPPartUniformLoad::FPPartUniformLoad(double w,double La,double Lb,double l,double e,double i) :
PFPartUniformLoad(w,l-La,l-Lb,l,e,i)
{
}


void FPPartUniformLoad::GetReactions(double &Ra,double &Rb)
{
   double Temp;

   PFPartUniformLoad::GetReactions(Ra,Rb);
   Temp = Ra;
   Ra = Rb;
   Rb = Temp;
}

void FPPartUniformLoad::GetMoments(double &Ma,double &Mb)
{
   double Temp;

   PFPartUniformLoad::GetMoments(Ma,Mb);
   Temp = Ma;
   Ma = Mb;
   Mb = Temp;
}

void FPPartUniformLoad::GetRotations(double &ra,double &rb)
{
   double Temp;

   PFPartUniformLoad::GetRotations(ra,rb);
   Temp = ra;
   ra = rb;
   rb = Temp;
}

void FPPartUniformLoad::GetDeflections(double &Ya,double &Yb)
{
   double Temp;

   PFPartUniformLoad::GetDeflections(Ya,Yb);
   Temp = Ya;
   Ya = Yb;
   Yb = Temp;
}

double FPPartUniformLoad::ComputeShear(double x)
{
   return PFPartUniformLoad::ComputeShear(L-x);
}

double FPPartUniformLoad::ComputeMoment(double x)
{
   return PFPartUniformLoad::ComputeMoment(L-x);
}

double FPPartUniformLoad::ComputeDeflection(double x)
{
   return PFPartUniformLoad::ComputeDeflection(L-x);
}
