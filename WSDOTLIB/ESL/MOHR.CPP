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

#include <math.h>
#include <esl\mohr.h>

#if defined _MSC_VER
#define M_PI 3.1415926539
#endif

MohrCircle2d::MohrCircle2d(double sii,double sjj,double sij)
{
   Init(sii,sjj,sij);
}

MohrCircle2d::MohrCircle2d(const MohrCircle2d &mc)
{
   double sii;
   double sjj;
   double sij;

   mc.GetState(0.00,sii,sjj,sij);
   Init(sii,sjj,sij);
}

void
MohrCircle2d::Init(double sii,double sjj,double sij)
{
   // principle values
   radius = pow((sii - sjj),2)/4 + pow(sij,2);
   radius = sqrt(radius);
   sMin = (sii + sjj)/2 + radius;
   sMax = (sii + sjj)/2 - radius;

   if (sMax < sMin)
      {
      double temp = sMin;
      sMin = sMax;
      sMax = temp;
      }

   // center of the circle
   center = (sMin + sMax)/2;

   // maximum coordinate of circle
   tMax = radius;

   // angle to principle plane
   if (radius == 0)
      theta = 0.0;
   else
      theta = atan2(2*sij,(sii - sjj))/2;
}

void
MohrCircle2d::GetState(double angle,double &sii,double &sjj,double &sij) const
{
   sii = center + radius*cos(2*angle) - tMax*sin(2*angle);
   sjj = center - radius*cos(2*angle) - tMax*sin(2*angle);
   sij = radius*sin(2*angle) + tMax*cos(2*angle);
}

//
//
//
MohrCircle3d::MohrCircle3d(double sii,double sjj,double skk,double sij,double sjk,double sik)
{
   double I1, I2, I3;        // invarients
   double t[3], temp;        // principle values, and a temp value
   double Q, R, S, T, alpha; // intermediate values
   double a, b, c, k;
   int i, j; // general purpose counters

   // invarients
   I1 = sii + sjj + skk;
   I2 = sii*sjj + sii*skk + sjj*skk - pow(sij,2) - pow(sik,2) - pow(sjk,2);
   I3 = sii*sjj*skk + 2*sij*sik*sjk - skk*pow(sij,2) - sjj*pow(sik,2) - sii*pow(sjk,2);

   // intermediate values
   R = pow(I1,2)/3 - I2;
   S = sqrt(R/3);
   T = sqrt(pow(R,3)/27);
   Q = I1*I2/3 - I3 - 2*pow(I1,3)/27;
   alpha = acos(-Q/(2*T));
   alpha /= 3;

   // principle values
   t[0] = 2*S*cos(alpha)            + I1/3;
   t[1] = 2*S*cos(alpha + 2*M_PI/3) + I1/3;
   t[2] = 2*S*cos(alpha + 4*M_PI/3) + I1/3;

   // sort principle values
   for (i = 0; i < 3; i++)
      {
      for (j = i; j < 3; j++)
         {
         if (t[i] < t[j])
            {
            temp = t[i];
            t[i] = t[j];
            t[j] = temp;
            }
         }
      }
   s0 = t[0];
   s1 = t[1];
   s2 = t[2];

   // Compute direction cosines
   for (i = 0; i < 3; i++)
      {
      a = (sjj - t[i])*(skk - t[i]) - pow(sjk,2);
      b = -( sij*(skk - t[i]) - sjk*sik );
      c = sij*sjk - sik*(sjj - t[i]);
      k = 1/sqrt(pow(a,2) + pow(b,2) + pow(c,2));
      v[i][0] = a*k;
      v[i][1] = b*k;
      v[i][2] = c*k;
      }

   // Principle stress/strains
   t0  = (s1 - s2)/2;
   sn0 = (s1 + s2)/2;
   t1  = (s0 - s2)/2;
   sn1 = (s0 + s2)/2;
   t2  = (s0 - s1)/2;
   sn2 = (s0 + s1)/2;
}

double MohrCircle3d::GetSoct() const
{
   return (s0 + s1 + s2)/3;
}

double MohrCircle3d::GetToct() const
{
   return sqrt(pow(s0-s1,2) + pow(s1-s2,2) + pow(s2-s0,2))/3;
}

