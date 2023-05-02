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

CFEndLoad::CFEndLoad(double w,double l,double e,double i) :
RoarkBeam(l,e,i)
{
   W = w;
}

void CFEndLoad::GetReactions(double &Ra,double &Rb)
{
   Ra = 0;
   Rb = W;
}

void CFEndLoad::GetMoments(double &Ma,double &Mb)
{
   Ma = 0;
   Mb = -W*L;
}

void CFEndLoad::GetRotations(double &ra,double &rb)
{
   ra = W*pow(L,2)/(2*EI);
   rb = 0;
}

void CFEndLoad::GetDeflections(double &Ya,double &Yb)
{
   Ya = -W*pow(L,3)/(3*EI);
   Yb = 0;
}

double CFEndLoad::ComputeShear(double )
{
   return -W;
}

double CFEndLoad::ComputeMoment(double x)
{
   return -W*x;
}

double CFEndLoad::ComputeDeflection(double x)
{
   double y;

   y = -W/(6*EI);
   y *= (pow(x,3) - 3*pow(L,2)*x + 2*pow(L,3));

   return y;
}

