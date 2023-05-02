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
 Cantilever, partial uniform load
***********************************************/
C_DLLEXP_VOID Roark04(Float64 W,Float64 L,Float64 a,Float64 b,Float64 EI,FE_RESULTS *results)
{
   results->Ra = 0;
   results->Rb = W;
   results->Ma = 0;
   results->Mb = -W*(a+b)/2;
   results->Ya = -W*(4*(a*a + a*b + b*b)*L - pow(a,3) - a*b*b - a*a*b - pow(b,3))/(24*EI);
   results->Yb = 0;
   results->ra = W*(a*a + a*b + b*b)/(6*EI);
   results->rb = 0;
}

C_DLLEXP_FLOAT64 Roark04Shear(Float64 x,Float64 W,Float64 L,Float64 a,Float64 b)
{
   Float64 V;

   if (x < a)
      V = 0;
   else if (a < x && x < b)
      V = W*(x - L + b)/(b-a);
   else
      V = -W;

   return V;
}

C_DLLEXP_FLOAT64 Roark04Moment(Float64 x,Float64 W,Float64 L,Float64 a,Float64 b)
{
   Float64 M;

   if (x < a)
      M = 0;
   else if (a < x && x < b)
      M = -0.5*W*pow((x - L + b),2)/(b-a);
   else
      M = -0.5*W*(2*x - 2*L + a + b);

   return M;
}

C_DLLEXP_FLOAT64 Roark04Disp(Float64 x,Float64 W,Float64 L,Float64 a,Float64 b,Float64 EI)
{
   Float64 y;

   if (x < a)
      y = -W*(4*(a*a + a*b + b*b)*(L-x) - pow(a,3) -a*b*b - a*a*b - pow(b,3))/(24*EI);
   else if (a < x && x < b)
      y = -W*(6*(a+b)*pow((L-x),2) - 4*pow((L-x),3) + pow((L-x-a),4)/(b-a))/(24*EI);
   else
      y = -W*(3*(a+b)*pow((L-x),2) - 2*pow((L-x),3))/(12*EI);

   return y;
}
