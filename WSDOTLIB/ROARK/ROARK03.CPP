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
 Cantilever, uniform load
***********************************************/
C_DLLEXP_VOID Roark03(Float64 w, Float64 L,Float64 EI,FE_RESULTS *results)
{
   Float64 W;

   W = w*L;

   results->Ra = 0;
   results->Rb = W;
   results->Ma = 0;
   results->Mb = -W*L/2;
   results->Ya = -W*pow(L,3)/(8*EI);
   results->Yb = 0;
   results->ra = W*pow(L,2)/(6*EI);
   results->rb = 0;
}

C_DLLEXP_FLOAT64 Roark03Shear(Float64 x,Float64 w,Float64 L)
{
   Float64 V, W;

   W = w*L;

   V = -W*x/L;

   return V;
}

C_DLLEXP_FLOAT64 Roark03Moment(Float64 x,Float64 w,Float64 L)
{
   Float64 M, W;

   W = w*L;

   M = -0.5*W*pow(x,2)/L;

   return M;
}

C_DLLEXP_FLOAT64 Roark03Disp(Float64 x,Float64 w,Float64 L,Float64 EI)
{
   Float64 y, W;

   W = w*L;

   y = -W*(pow(x,4) - 4*pow(L,3)*x + 3*pow(L,4))/(24*EI*L);

   return y;
}
