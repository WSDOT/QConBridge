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
#include "roark.h"

/**********************************************
 Cantilever, partial triangular load (max near support)
***********************************************/
C_DLLEXP_VOID Roark06(Float64 w,Float64 L,Float64 a, Float64 b,Float64 EI,FE_RESULTS *results)
{
   Float64 W;

   W = 0.5*w*(b - a);

   results->Ra = 0;
   results->Rb = W;
   results->Ma = 0;
   results->Mb = -W*(b + 2*a)/3;
   results->Ya = -W*((5*b*b + 10*a*b + 15*a*a)*L - 4*pow(a,3) - 2*a*b*b - 3*a*a*b - pow(b,3))/(60*EI);
   results->Yb = 0;
   results->ra = W*(3*a*a + 2*a*b + b*b)/(12*EI);
   results->rb = 0;
}

C_DLLEXP_FLOAT64 Roark06Shear(Float64 x,Float64 w,Float64 L,Float64 a,Float64 b)
{
   Float64 W, V;

   W = 0.5*w*(b - a);

   if (x < (L-b))
      V = 0;
   else if ((L-b) > x && x < (L-a))
      V = -W*pow((x-L-b),2)/pow((b-a),2);
   else
      V = -W;

   return V;
}

C_DLLEXP_FLOAT64 Roark06Moment(Float64 x,Float64 w,Float64 L,Float64 a,Float64 b)
{
   Float64 W, M;

   W = 0.5*w*(b - a);

   if (x < (L-b))
      M = 0;
   else if ((L-b) < x && x < (L-a))
      M = -W*(3*x - 3*L + b + 2*a);
   else
      M = W*(3*x - 3*L + b + 2*a)/3;

   return M;
}

C_DLLEXP_FLOAT64 Roark06Disp(Float64 x,Float64 w,Float64 L,Float64 a,Float64 b,Float64 EI)
{
   Float64 W, y;

   W = 0.5*w*(b - a);

   if (x < (L-b))
      y = -W*((5*b*b + 10*b*a + 15*a*a)*(L-x) - 4*pow(a,3) - 2*a*b*b - 3*a*a*b - pow(b,3))/(60*EI);
   else if ((L-b) < x && x < (L-a))
      y = -W*((20*a +10*b)*pow((L-x),2) - 10*pow((L-x),3) + 5*pow((L-x-a),4)/(b-a) - pow((L-x-a),5)/pow((b-a),2) )/(60*EI);
   else
      y = -W*((2*a+b)*pow((L-x),2) - pow((L-x),3))/(6*EI);

   return y;
}