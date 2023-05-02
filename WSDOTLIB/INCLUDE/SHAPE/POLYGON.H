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

//////////////////////////////////////////////////////////////////
//    OVERVIEW
//    ========
//    Class definition for TPolyShape
//
//    NOTES
//    =====
//    The interpertation of TLocationPoint for TTrapezoid and its derived classes
//    is as follows (except TRightTriangle):
//    TopLeft      = Top left corner of shape (not the bounding rectangle)
//    TopCenter    = Midpoint of top side
//    TopRight     = Top right corner of shape
//    LeftCenter   = Midpoint of left side
//    CenterCenter = Intersection point for a lines drawn between the opposite
//                   corners of the shape
//    RightCenter  = Midpoint of right side
//    BottomLeft   = Bottom left corner of the shape
//    BottomCenter = Midpoint of bottom side
//    BottomRight  = Bottom right corner of the shape
//
//    The interpertation of TLocationPoint for TRightTriangle and its derived classes
//    is as follows:
//    TopLeft      = Top left corner of shape (not the bounding rectangle)
//    TopCenter    = Same as TopLeft
//    TopRight     = Same as TopLeft
//    LeftCenter   = Midpoint of left side
//    CenterCenter = Midpoint of the hypotanose
//    RightCenter  = Same as CenterCenter
//    BottomLeft   = Bottom left corner of the shape
//    BottomCenter = Midpoint of bottom side
//    BottomRight  = Bottom right corner of the shape
//
//    The default position of a TRightTriangle object is (Angle = 0.) in the first
//    quadrant of the local coordinate system
//

#if !defined SHAPE_POLYGON_H_
#define SHAPE_POLYGON_H_

#include <shape\shape.h>
#include <shape\posrect.h>
#include <checks.h>

#include <classlib\arrays.h>
typedef TIArrayAsVector<TFPoint> TIFPointContainer;
typedef TIArrayAsVectorIterator<TFPoint> TIFPointIterator;

class SHAPECLASS TPolyShape : public TShape
{
// Data Members
private:
   TIFPointContainer *pntContainer;
   TFPoint           origin;  // location of local coordinate system origin
   double            angle;   // rotation of local coordinate system

// Constructors/Destructor
public:
   TPolyShape(Material* material,TFPoint Origin = TFPoint(0,0),double Angle = 0);
   ~TPolyShape();

// Member Functions
public:
   void SetOrigin(double x,double y);
   void SetOrigin(TFPoint rp);
   void SetRotationAngle(double Angle);
   void AddPoint(double x,double y);     // points are added in local coordinates
   void AddPoint(TFPoint &sp);

   TShape* Clone() const;

#if defined(_Windows)
   void Draw(TRealDC &dc);
#endif


protected:
   void ComputeExtents();
   void UpdateExtents(TFPoint &pnt);
   void ComputeLocalProperties();
};

class SHAPECLASS TTrapezoid : public TPolyShape
{
// Data Members
protected:
   TPosTrapezoid *posTrap;
   double h,b1,b2,c;

// Constructors/Destructor
public:
   TTrapezoid(double H,double B1,double B2,double C,Material *material,double Angle = 0.,
              TFPoint& rp = TFPoint(0,0),TLocationPoint lp=CenterCenter);
   ~TTrapezoid();

// Member Functions
private:
   void GenerateShape();
};

class SHAPECLASS TParallelogram : public TTrapezoid
{
// Data Members

// Constructors/Destructors
public:
   TParallelogram(double H,double B,double C,Material* material,double Angle = 0.,
                  TFPoint& rp = TFPoint(0,0),TLocationPoint lp=CenterCenter);
   ~TParallelogram();

// Member Functions
};

class SHAPECLASS TRectangle : public TParallelogram
{
// Data Members

// Constructors/Destructors
public:
   TRectangle(double H,double B,Material* material,double Angle = 0.,
              TFPoint& rp = TFPoint(0,0),TLocationPoint lp=CenterCenter);
   ~TRectangle();

// Member Functions
};

class SHAPECLASS TSquare : public TRectangle
{
// Data Members

// Constructors/Destructors
public:
   TSquare(double S,Material* material,double Angle = 0.,
           TFPoint& rp = TFPoint(0,0),TLocationPoint lp=CenterCenter);
   ~TSquare();

// Member Functions
};

class SHAPECLASS TRightTriangle : public TTrapezoid
{
// Data Members

// Constructors/Destructor
public:
   TRightTriangle(double B,double H,Material* material,double Angle = 0.,
                  TFPoint& rp = TFPoint(0,0),TLocationPoint lp=CenterCenter);
   ~TRightTriangle();

// Member Functions
};

#endif // SHAPE_POLYGON_H_
