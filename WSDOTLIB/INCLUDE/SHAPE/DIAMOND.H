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


#if !defined SHAPE_DIAMOND_H_
#define SHAPE_DIAMOND_H_

#include <shape\polygon.h>
#include <shape\posrect.h>

class SHAPECLASS TDiamond : public TPolyShape
{
// Data Members
private:
   TPosRect *posRect;
   double h,b;

// Constructors/Destructor
public:
   TDiamond(double H,double B,Material* material,double Angle = 0.,
            TFPoint& rp = TFPoint(0,0),TLocationPoint lp = CenterCenter);
   ~TDiamond();

// Member Functions
protected:
   void GenerateShape();
};

#endif // SHAPE_DIAMOND_H_
