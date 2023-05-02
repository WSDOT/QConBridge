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

FFPartUniformLoad::FFPartUniformLoad(double La,double Lb,double w,double l,double e,double i) :
RoarkBeam(l,e,i)
{
   a = La;
   b = Lb;
   c = b - a;
   d = l - 0.5*a - 0.5*b;

   W = w*c;
}


void FFPartUniformLoad::GetReactions(double &Ra,double &Rb)
{
   Ra = W*(12*d*d - 8*d*d*d/L + 2*b*c*c/L - c*c*c/L - c*c)/(4*L*L);
   Rb = W - Ra;
}

void FFPartUniformLoad::GetMoments(double &Ma,double &Mb)
{
   Ma = -W*(24*d*d*d/L - 6*b*c*c/L + 3*c*c*c/L + 4*c*c - 24*d*d)/(24*L);
   Mb =  W*(24*d*d*d/L - 6*b*c*c/L + 3*c*c*c/L + 2*c*c - 48*d*d + 24*d*L)/(24*L);
}

void FFPartUniformLoad::GetRotations(double &ra,double &rb)
{
   ra = 0;
   rb = 0;
}

void FFPartUniformLoad::GetDeflections(double &Ya,double &Yb)
{
   Ya = 0;
   Yb = 0;
}

double FFPartUniformLoad::ComputeShear(double x)
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

double FFPartUniformLoad::ComputeMoment(double x)
{
   double M;
   double Ra, Rb;
   double Ma, Mb;

   GetReactions(Ra,Rb);
   GetMoments(Ma,Mb);

   if (x < a)
      M = -Ma + Ra*x;
   else if (x < b)
      M = -Ma + Ra*x - W*(x-a)*(x-a)/(2*c);
   else
      M = -Ma + Ra*x - W*(x - L + d);

   return M;
}

double FFPartUniformLoad::ComputeDeflection(double x)
{
   double y;

   double Ra, Rb;
   double Ma, Mb;

   GetReactions(Ra,Rb);
   GetMoments(Ma,Mb);

   if (x < a)
      y = (Ra*x*x*x - 3*Ma*x*x)/(6*EI);
   else if (x < b)
      y = (Ra*x*x*x - 3*Ma*x*x - W*pow((x-a),2)/(4*c))/(6*EI);
   else
      y = (Rb*pow((L-x),3) - 3*Mb*pow((L-x),2))/(6*EI);

   return y;
}
