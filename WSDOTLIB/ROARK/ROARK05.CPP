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
 Cantilever, triangular load (max at support)
***********************************************/
C_DLLEXP_VOID Roark05(Float64 w,Float64 L,Float64 EI,FE_RESULTS *results)
{
   Float64 W;

   W = 0.5*w*L;

   results->Ra = 0;
   results->Rb = W;
   results->Ma = 0;
   results->Mb = -W*L/3;
   results->Ya = -W*pow(L,3)/(15*EI);
   results->Yb = 0;
   results->ra = W*L*L/(12*EI);
   results->rb = 0;
}

C_DLLEXP_FLOAT64 Roark05Shear(Float64 x,Float64 w,Float64 L)
{
   Float64 W, V;

   W = 0.5*w*L;

   V = -W*x*x/(L*L);

   return V;
}

C_DLLEXP_FLOAT64 Roark05Moment(Float64 x,Float64 w,Float64 L)
{
   Float64 W, M;

   W = 0.5*w*L;

   M = -W*pow(x,3)/(3*L*L);
   return M;
}

C_DLLEXP_FLOAT64 Roark05Disp(Float64 x,Float64 w,Float64 L,Float64 EI)
{
   Float64 W, y;

   W = 0.5*w*L;

   y = -W*(pow(x,5) - 5*pow(L,4)*x + 4*pow(L,5))/(60*EI*L*L);
   return y;
}