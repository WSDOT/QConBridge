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

RoarkBeam::RoarkBeam(double l,double e,double i)
{
   L = l; // Length of beam
   EI = e*i;
}

/*
C_DLLEXP_VOID Roark22(Float64 W, Float64 L, Float64 a, Float64 EI, FE_RESULTS *results)
{
   Float64 Ra, Rb;
   Float64 Ma, Mb;
   Float64 Ya, Yb;
   Float64 ra, rb;

   Rb = 0.5*W*(3*a*a*L - a*a*a)/(L*L*L);
   Ra = W - Rb;
   Mb = 0;
   Ma = 0.5*W*(a*a*a + 2*a*L*L - 3*a*a*L)/(L*L);
   Yb = 0;
   Ya = 0;
   rb = W*(a*a*a/L - a*a)/(4*EI);
   ra = 0;

   results->Ra = -Ra;
   results->Rb = -Rb;
   results->Ma = -Ma;
   results->Mb = -Mb;
   results->Ya = -Ya;
   results->Yb = -Yb;
   results->ra =  ra;
   results->rb =  rb;
}
*/
/*
C_DLLEXP_VOID Roark24(Float64 w, Float64 L, Float64 a, Float64 b, Float64 EI, FE_RESULTS *results)
{
   Float64 Ra, Rb;
   Float64 Ma, Mb;
   Float64 Ya, Yb;
   Float64 ra, rb;
   Float64 c, W;

   c = b - a;
   W = w*c;

   Rb = W*(4*L*(a*a + a*b + b*b) - a*a*a - a*b*b - a*a*b - b*b*b)/(8*L*L*L);
   Ra = W - Rb;
   Mb = 0;
   Ma = -Rb*L + 0.5*W*(a + b);
   Yb = 0;
   Ya = 0;
   rb = -(0.5*Rb*L*L - W*(0.5*a*a + 0.5*a*c + c*c/6))/(EI);
   ra = 0;

   results->Ra = -Ra;
   results->Rb = -Rb;
   results->Ma = -Ma;
   results->Mb = -Mb;
   results->Ya = -Ya;
   results->Yb = -Yb;
   results->ra =  ra;
   results->rb =  rb;
}
*/
/*
C_DLLEXP_VOID Roark26(Float64 w, Float64 L, Float64 a, Float64 b, Float64 EI, FE_RESULTS *results)
{
   Float64 Ra, Rb;
   Float64 Ma, Mb;
   Float64 Ya, Yb;
   Float64 ra, rb;
   Float64 c, W;

   c = b - a;
   W = 0.5*w*c;

   Rb = W*((10*a*b + 15*a*a + 5*b*b)*L -4*a*a*a - 2*a*b*b - 3*a*a*b - b*b*b)/(20*L*L*L);
   Ra = W - Rb;
   Mb = 0;
   Ma = -Rb*L + W*(2*a + b)/3;
   Yb = 0;
   Ya = 0;
   rb = -(0.5*Rb*L*L - W*(c*c/12 + a*c/3 + a*a/2))/(EI);
   ra = 0;

   results->Ra = -Ra;
   results->Rb = -Rb;
   results->Ma = -Ma;
   results->Mb = -Mb;
   results->Ya = -Ya;
   results->Yb = -Yb;
   results->ra =  ra;
   results->rb =  rb;
}
*/
/*
C_DLLEXP_VOID Roark28(Float64 w, Float64 L, Float64 a, Float64 b, Float64 EI, FE_RESULTS *results)
{
   Float64 Ra, Rb;
   Float64 Ma, Mb;
   Float64 Ya, Yb;
   Float64 ra, rb;
   Float64 c, W;

   c = b - a;
   W = 0.5*w*c;

   Rb = W*((10*a*b + 5*a*a + 15*b*b)*L - a*a*a - 2*a*b*b - 3*a*a*b - 4*b*b*b)/(20*L*L*L);
   Ra = W - Rb;
   Mb = 0;
   Ma = -Rb*L + W*(a + 2*b)/3;
   Yb = 0;
   Ya = 0;
   rb = -(0.5*Rb*L*L - W*(c*c/4 + 2*a*c/3 + a*a/2))/(EI);
   ra = 0;

   results->Ra = -Ra;
   results->Rb = -Rb;
   results->Ma = -Ma;
   results->Mb = -Mb;
   results->Ya = -Ya;
   results->Yb = -Yb;
   results->ra =  ra;
   results->rb =  rb;
}
*/
/*
C_DLLEXP_VOID Roark32(Float64 W, Float64 L, Float64 a, Float64 EI, FE_RESULTS *results)
{
   Float64 Ra, Rb;
   Float64 Ma, Mb;
   Float64 Ya, Yb;
   Float64 ra, rb;
   Float64 b;

   b = L - a;

   Ra = W*b*b*(3*a + b)/(L*L*L);
   Rb = W*a*a*(3*b + a)/(L*L*L);
   Ma = W*a*b*b/(L*L);
   Mb = W*a*a*b/(L*L);
   Ya = 0;
   Yb = 0;
   ra = 0;
   rb = 0;

   results->Ra = -Ra;
   results->Rb = -Rb;
   results->Ma = -Ma;
   results->Mb = -Mb;
   results->Ya = -Ya;
   results->Yb = -Yb;
   results->ra = -ra;
   results->rb = -rb;

   EI *= Ma + Mb;
}
*/
/*
C_DLLEXP_VOID Roark34(Float64 w, Float64 L, Float64 a, Float64 b, Float64 EI, FE_RESULTS *results)
{
   Float64 Ra, Rb;
   Float64 Ma, Mb;
   Float64 Ya, Yb;
   Float64 ra, rb;
   Float64 c, d, W;

   c = b - a;
   d = L - a/2 - b/2;
   W = w*c;

   Ra = W*(12*d*d - 8*d*d*d/L + 2*b*c*c/L - c*c*c/L - c*c)/(4*L*L);
   Rb = W - Ra;
   Ma = -W*(24*d*d*d/L - 6*b*c*c/L + 3*c*c*c/L + 4*c*c - 24*d*d)/(24*L);
   Mb =  W*(24*d*d*d/L - 6*b*c*c/L + 3*c*c*c/L + 2*c*c - 48*d*d + 24*d*L)/(24*L);
   Ya = 0;
   Yb = 0;
   ra = 0;
   rb = 0;

   results->Ra = -Ra;
   results->Rb = -Rb;
   results->Ma = -Ma;
   results->Mb = -Mb;
   results->Ya = -Ya;
   results->Yb = -Yb;
   results->ra = -ra;
   results->rb = -rb;

   EI *= Ma + Mb;
}
*/
/*
C_DLLEXP_VOID Roark36(Float64 w, Float64 L, Float64 a, Float64 b, Float64 EI, FE_RESULTS *results)
{
   Float64 Ra, Rb;
   Float64 Ma, Mb;
   Float64 Ya, Yb;
   Float64 ra, rb;
   Float64 c, d, W;

   c = b - a;
   d = L - a/3 - 2*b/3;
   W = 0.5*w*c;

   Ra = W*(3*d*d - c*c/6 + b*c*c/(3*L) - 17*c*c*c/(135*L) - 2*d*d*d/L)/(L*L);
   Rb = W - Ra;
   Ma = -(W/L)*(d*d*d/L + c*c/9 + 51*c*c*c/(810*L) - c*c*b/(6*L) - d*d);
   Mb =  (W/L)*(d*d*d/L + c*c/18 + 51*c*c*c/(810*L) - c*c*b/(6*L) - 2*d*d + d*L);
   Ya = 0;
   Yb = 0;
   ra = 0;
   rb = 0;

   results->Ra = -Ra;
   results->Rb = -Rb;
   results->Ma = -Ma;
   results->Mb = -Mb;
   results->Ya = -Ya;
   results->Yb = -Yb;
   results->ra = -ra;
   results->rb = -rb;

   EI *= Ma + Mb;
}
*/
