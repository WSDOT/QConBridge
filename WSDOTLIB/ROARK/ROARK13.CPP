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
 End supports, uniform load
***********************************************/
C_DLLEXP_VOID Roark13(Float64 w, Float64 L,Float64 EI,FE_RESULTS *results)
{
   Float64 W;

   W = w*L;

   results->Ra = W/2;
   results->Rb = W/2;
   results->Ma = 0;
   results->Mb = 0;
   results->Ya = 0;
   results->Yb = 0;
   results->ra = -W*L*L/(24*EI);
   results->rb = W*L*L/(24*EI);
}

C_DLLEXP_FLOAT64 Roark13Shear(Float64 x,Float64 w,Float64 L)
{
   Float64 V, W;

   W = w*L;

   V = 0.5*W*(1 - 2*x/L);

   return V;
}

C_DLLEXP_FLOAT64 Roark13Moment(Float64 x,Float64 w,Float64 L)
{
   Float64 M, W;

   W = w*L;

   M = 0.5*W*(x - x*x/L);

   return M;
}

C_DLLEXP_FLOAT64 Roark13Disp(Float64 x,Float64 w,Float64 L,Float64 EI)
{
   Float64 y, W;

   W = w*L;

   y = -W*(pow(L,3) - 2*L*x*x + pow(x,3))/(24*EI*L);

   return y;
}