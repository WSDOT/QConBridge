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
 End supports, center load
***********************************************/
C_DLLEXP_VOID Roark11(Float64 W, Float64 L,Float64 EI,FE_RESULTS *results)
{
   results->Ra = W/2;
   results->Rb = W/2;
   results->Ma = 0;
   results->Mb = 0;
   results->Ya = 0;
   results->Yb = 0;
   results->ra = -W*pow(L,2)/(16*EI);
   results->rb = W*pow(L,2)/(16*EI);
}

C_DLLEXP_FLOAT64 Roark11Shear(Float64 x,Float64 W,Float64 L)
{
   Float64 V;

   if (x < L/2)
      V = W/2;
   else
      V = -W/2;

   return V;
}

C_DLLEXP_FLOAT64 Roark11Moment(Float64 x,Float64 W,Float64 L)
{
   Float64 M;

   if (x < L/2)
      M = W*x/2;
   else
      M = W*(L-x)/2;

   return M;
}

C_DLLEXP_FLOAT64 Roark11Disp(Float64 x,Float64 W,Float64 L,Float64 EI)
{
   Float64 y;

   if (x < L/2)
      y = -W*(3*L*L*x - 4*pow(x,3))/(48*EI);
   else
      y = -W*(L-x)*(3*L*L - pow((L-x),2))/(48*EI);

   return y;
}