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
#include "shape.hpp"

TRightTriangle::TRightTriangle(double B,double H,int Quad,TShapePoint &sp,double e,double d) :
TPolyShape(e,d)
{
   b = B;
   h = H;
   quad = Quad;
   startPnt = sp;

   GenerateShape();
}

void TRightTriangle::GenerateShape(void)
{
   TShapePoint cornerPnt;
   double bsign, hsign;

   switch(quad)
      {
      case QUAD_1: bsign =  1.; hsign =  1.; break;
      case QUAD_2: bsign = -1.; hsign =  1.; break;
      case QUAD_3: bsign = -1.; hsign = -1.; break;
      case QUAD_4: bsign =  1.; hsign = -1.; break;
      }

   AddShapePoint(startPnt);

   cornerPnt.y = hsign*h;
   cornerPnt.z = 0;
   AddShapePoint(startPnt + cornerPnt);

   cornerPnt.y = 0;
   cornerPnt.z = bsign*b;
   AddShapePoint(startPnt + cornerPnt);
}

