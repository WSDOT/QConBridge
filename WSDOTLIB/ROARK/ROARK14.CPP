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

PPPartUniformLoad::PPPartUniformLoad(double La,double Lb,double w,double l,double e,double i) :
RoarkBeam(l,e,i)
{
   a = La;
   b = Lb;
   c = b - a;
   d = l - 0.5*a - 0.5*b;

   W = w*c;
}


void PPPartUniformLoad::GetReactions(double &Ra,double &Rb)
{
   Ra = W*d/L;
   Rb = W*(a + 0.5*c)/L;
}

void PPPartUniformLoad::GetMoments(double &Ma,double &Mb)
{
   Ma = 0.;
   Mb = 0.;
}

void PPPartUniformLoad::GetRotations(double &ra,double &rb)
{
   double Ra, Rb;

   GetReactions(Ra,Rb);
   ra = (-8*Ra*L*L + W*(8*d*d*d/L - 2*b*c*c/L +c*c*c/L + 2*c*c))/(48*EI);
   rb = (16*Ra*L*L - W*(24*d*d - 8*d*d*d/L + 2*b*c*c/L - c*c*c/L))/(48*EI);
}

void PPPartUniformLoad::GetDeflections(double &Ya,double &Yb)
{
   Ya = 0;
   Yb = 0;
}

double PPPartUniformLoad::ComputeShear(double x)
{
   double V;
   double Ra,Rb;

   GetReactions(Ra,Rb);

   if (x < a)
      V = Ra;
   else if (x < b)
      V = Ra - W*(x - a)/c;
   else
      V = Ra - W;

   return V;
}

double PPPartUniformLoad::ComputeMoment(double x)
{
   double M;
   double Ra, Rb;

   GetReactions(Ra,Rb);

   if (x < a)
      M = Ra*x;
   else if (x < b)
      M = Ra*x - W*(x-a)*(x-a)/(2*c);
   else
      M = Ra*x - W*(x - 0.5*a - 0.5*b);

   return M;
}

double PPPartUniformLoad::ComputeDeflection(double x)
{
   double y;

   double Ra, Rb;

   GetReactions(Ra,Rb);

   if (x < a)
      {
      y = 8*Ra*(x*x*x - L*L*x) + W*x*(8*d*d*d/L - 2*b*c*c/L + c*c*c/L + 2*c*c);
      }
   else if (x < b)
      {
      y = 8*Ra*(x*x*x - L*L*x) + W*x*(8*d*d*d/L - 2*b*c*c/L + c*c*c/L + 2*c*c);
      y -= 2*W*pow((x-a),4)/c;
      }
   else
      {
      y = 8*Ra*(x*x*x - L*L*x) + W*x*(8*d*d*d/L - 2*b*c*c/L + c*c*c/L);
      y -= 8*W*pow((x - 0.5*a - 0.5*b),3) + W*(2*b*c*c - c*c*c);
      }

   y /= 48*EI;

   return y;
}

