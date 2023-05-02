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


#if !defined SHAPE_ELLIPSE_H_
#define SHAPE_ELLIPSE_H_

#include <shape\shape.h>
#include <shape\posrect.h>

class SHAPECLASS TEllipse : public TShape
{
// Data Members
protected:
   double h,b;
   TPosRect *posRect;

// Constructors/Destructor
public:
   TEllipse(double H,double B,Material* material,
            TFPoint& rp = TFPoint(0,0),TLocationPoint lp=CenterCenter);
   ~TEllipse();

   TShape* Clone() const;

#if defined(_Windows)
   void Draw(TRealDC &dc);
#endif

// Member Functions
protected:
   void ComputeLocalProperties();

private:
   virtual void ComputeExtents();
};

class SHAPECLASS TCircle : public TEllipse
{
// Data Members

// Constructors/Destructor
public:
   TCircle(double D,Material* material,
           TFPoint& rp = TFPoint(0,0),TLocationPoint lp = CenterCenter);

// Member Functions
};

/*************************************************
class TQuarterEllipse : public TEllipse
{
// Data Members

// Constructors/Destructor
public:
   TQuarterEllipse(double B,double H,double Angle = 0.,
                   TFPoint& rp = TFPoint(0,0),TLocationPoint lp = CenterCenter,
                   int color = 1,double e = 1.,double d = 1.);

// Member Functions
public:
   virtual void ComputeProperties();
   virtual void Draw(TRealDC &dc);
};

class TQuarterCircle : public TQuarterEllipse
{
// Data Members

// Constructors/Destructor
public:
   TQuarterCircle(double R,double Angle = 0.,
                  TFPoint& rp = TFPoint(0,0),TLocationPoint lp = CenterCenter,
                  int color = 1,double e = 1.,double d = 1.);

// Member Functions
};

class TEllipticComplement : public TQuarterEllipse
{
// Data Members

// Constructors/Destructor
public:
   TEllipticComplement(double B,double H,double Angle = 0.,
                       TFPoint& rp = TFPoint(0,0),TLocationPoint lp = CenterCenter,
                       int color = 1,double e = 1.,double d = 1.);

// Member Functions
public:
   virtual void ComputeProperties();
};

class TCircleComplement : public TEllipticComplement
{
// Data Members

// Constructors/Destructor
public:
   TCircleComplement(double R,double Angle = 0.,
                     TFPoint& rp = TFPoint(0,0),TLocationPoint lp = CenterCenter,
                     int color = 1,double e = 1.,double d = 1.);

// Member Functions
};

***************************************************/

#endif // SHAPE_ELLIPSE_H_
