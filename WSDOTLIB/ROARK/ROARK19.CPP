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
 End supports, end couple
***********************************************/
C_DLLEXP_VOID Roark19(Float64 Mo,Float64 L,Float64 EI,FE_RESULTS *results)
{
   results->Ra = -Mo/L;
   results->Rb = Mo/L;
   results->Ma = 0;
   results->Mb = 0;
   results->Ya = 0;
   results->Yb = 0;
   results->ra = -Mo*L/(3*EI);
   results->rb = Mo*L/(6*EI);
}

C_DLLEXP_FLOAT64 Roark19Shear(Float64 x,Float64 Mo,Float64 L)
{
   Float64 V;

   V = -Mo/L;

   x += Mo + L; /* just to keep the compiler happy */

   return V;
}

C_DLLEXP_FLOAT64 Roark19Moment(Float64 x,Float64 Mo,Float64 L)
{
   Float64 M;

   M = Mo*(1 - x/L);

   return M;
}

C_DLLEXP_FLOAT64 Roark19Disp(Float64 x,Float64 Mo,Float64 L,Float64 EI)
{
   Float64 y;

   y = Mo*(3*x*x - pow(x,3)/L - 2*L*x)/(6*EI);

   return y;
}