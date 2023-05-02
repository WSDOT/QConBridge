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
 Cantilever, end couple
***********************************************/
C_DLLEXP_VOID Roark09(Float64 Mo,Float64 L,Float64 EI,FE_RESULTS *results)
{
   results->Ra = 0;
   results->Rb = 0;
   results->Ma = 0;
   results->Mb = Mo;
   results->Ya = Mo*L*L/(2*EI);
   results->Yb = 0;
   results->ra = -Mo*L/EI;
   results->rb = 0;
}

C_DLLEXP_FLOAT64 Roark09Shear(Float64 x,Float64 Mo,Float64 L)
{
   x += Mo + L; /* just to keep the compiler happy */

   return 0;
}

C_DLLEXP_FLOAT64 Roark09Moment(Float64 x,Float64 Mo,Float64 L)
{
   x += L; /* just to keep the compiler happy */

   return Mo;
}

C_DLLEXP_FLOAT64 Roark09Disp(Float64 x,Float64 Mo,Float64 L,Float64 EI)
{
   Float64 y;

   y = Mo*(L*L - 2*L*x + x*x)/(2*EI);

   return y;
}