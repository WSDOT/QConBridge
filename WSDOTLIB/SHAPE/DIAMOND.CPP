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

#include <shape\diamond.h>

TDiamond::TDiamond(double H,double B,Material* material,double Angle,
                   TFPoint& rp,TLocationPoint lp) :
TPolyShape(material,TFPoint(0,0),Angle)
{
   h = H;
   b = B;
   posRect = new TPosRect(H,B,Angle,rp,lp);
   GenerateShape();
}

TDiamond::~TDiamond()
{
   if (posRect)
      delete posRect;
}

void TDiamond::GenerateShape()
{
   TFPoint p;

   p = posRect->GetBottomCenter();
   SetOrigin(p);

   AddPoint(0,0);
   AddPoint(-b/2,h/2);
   AddPoint(0,h);
   AddPoint(b/2,h/2);
}

