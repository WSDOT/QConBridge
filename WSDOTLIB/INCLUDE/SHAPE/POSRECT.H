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

#if !defined SHAPE_POSRECT_H_
#define SHAPE_POSRECT_H_

#include <shape\shapedll.h>
#include <shape\points.h>

typedef enum TLocationPoint {
             TopLeft,
             TopCenter,
             TopRight,
             BottomLeft,
             BottomCenter,
             BottomRight,
             LeftCenter,
             RightCenter,
             CenterCenter} TLocationPoint;

class SHAPECLASS TPosTrapezoid
{
// Data Members
private:
   TFPoint centerPnt;

protected:
   double h;
   double b;
   double bTop;
   double c;
   TFPoint refPoint;
   TLocationPoint locPoint;
   double rotAngle;

// Constructors/Destructor
public:
   TPosTrapezoid(double H,double B1,double B2,double C,double Angle = 0,TFPoint& rp = TFPoint(0,0),TLocationPoint lp = CenterCenter);
   virtual ~TPosTrapezoid();

// Member Functions
public:
   TFPoint GetTopLeft();
   TFPoint GetTopCenter();
   TFPoint GetTopRight();
   TFPoint GetLeftCenter();
   TFPoint GetCenterCenter();
   TFPoint GetRightCenter();
   TFPoint GetBottomLeft();
   TFPoint GetBottomCenter();
   TFPoint GetBottomRight();
   TFPoint GetPosPoint(TLocationPoint lp);
   double  GetRotationAngle();

private:
   TFPoint ToGlobalCoordinates(TFPoint &lsp);
   void ComputeCenterCenter();
};

class SHAPECLASS TPosRect : public TPosTrapezoid
{
// Constructors/Destructor
public:
   TPosRect(double H,double B,double Angle = 0.,TFPoint& rp = TFPoint(0,0),TLocationPoint lp = CenterCenter);
   ~TPosRect();
};

#endif // SHAPE_POSRECT_H_
