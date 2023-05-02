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

#ifndef SPANDREL_HPP_
#define SPANDREL_HPP_

#include "shape.hpp"

class TGeneralSpandrel : public TShape
{
// Data Members
protected:
   TShapePoint refPnt;
   double a, h;
   int n;
   int quad;

// Constructors/Destructor
public:
   TGeneralSpandrel(double A,double H,int N,double Angle,
                    TShapePoint &rp,TLocationPoint lp,
                    int color = 1,double e = 1.,double d = 1.);

// Member Functions
public:
   virtual void Draw(TRealDC &dc);
};


// Zero Slope General Spandrel
class TZSSpandrel : public TGeneralSpandrel
{
// Data Members

// Constructors/Destructor
public:
   TZSSpandrel(double A,double H,int N,double Angle,
               TShapePoint &rp,TLocationPoint lp,
               int color = 1,double e = 1.,double d = 1.);

// Member Functions
public:
   virtual void ComputeProperties(void);
};

// Infinite Slope General Spandrel
class TISSpandrel : public TGeneralSpandrel
{
// Data Members

// Constructors/Destructor
public:
   TISSpandrel(double A,double H,int N,double Angle,
               TShapePoint &rp,TLocationPoint lp,
               int color = 1,double e = 1.,double d = 1.);

// Member Functions
public:
   virtual void ComputeProperties(void);
};

#endif // SPANDREL_HPP_