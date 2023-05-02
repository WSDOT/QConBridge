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
 Cantilever, intermediate couple
***********************************************/
C_DLLEXP_VOID Roark10(Float64 Mo,Float64 L,Float64 a,Float64 EI,FE_RESULTS *results)
{
   results->Ra = 0;
   results->Rb = 0;
   results->Ma = 0;
   results->Mb = Mo;
   results->Ya = Mo*a*(L - 0.5*a)/(2*EI);
   results->Yb = 0;
   results->ra = -Mo*a/EI;
   results->rb = 0;
}

C_DLLEXP_FLOAT64 Roark10Shear(Float64 x,Float64 Mo,Float64 L,Float64 a)
{
   x += a + Mo + L; /* just to keep the compiler happy */

   return 0;
}

C_DLLEXP_FLOAT64 Roark10Moment(Float64 x,Float64 Mo,Float64 L,Float64 a)
{
   Float64 M;

   if (x < (L-a))
      M = 0;
   else if ((L-a) < x)
      M = Mo;

   return M;
}

C_DLLEXP_FLOAT64 Roark10Disp(Float64 x,Float64 Mo,Float64 L,Float64 a,Float64 EI)
{
   Float64 y;

   if (x < (L-a))
      y = Mo*a*(L - 0.5*a - x)/(2*EI);
   else if ((L-a) < x)
      y = Mo*(pow((x-L+a),2) - 2*a*(x-L+a) + a*a)/(2*EI);

   return y;
}