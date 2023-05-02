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

#if !defined GENSHAPE_H_
#define GENSHAPE_H_

#include <shape\shape.h>
#include <shape\posrect.h>

class SHAPECLASS GeneralShape : public TShape
{
// Constructors/Destructor
public:
   GeneralShape(double area,double xBar,double yBar,
                double ixx,double iyy,double ixy,
                double h,double w,
                Material* material,
                TFPoint& refPoint = TFPoint(0,0),TLocationPoint lp = BottomCenter);
   ~GeneralShape();

public: // Graphical Properties
   void ComputeExtents();
   double GetHeight() {return h;}
   double GetWidth()  {return w;}
   TShape* Clone() const;

#if defined(_Windows)
   void Draw(TRealDC &dc);
#endif

protected:
   TPosRect* posRect;
   double Area;
   double xBar;
   double yBar;
   double ixx;
   double iyy;
   double ixy;
   double h;
   double w;
   void ComputeLocalProperties();
};


#endif // GENSHAPE_H_
