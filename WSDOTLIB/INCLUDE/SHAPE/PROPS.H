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

#if !defined SHAPE_PROPS_H_
#define SHAPE_PROPS_H_

#include <shape\shapedll.h>

class SHAPECLASS TCentroid
{
// Data Members
public:
   double Xbar, Ybar;

// Constructors
public:
   TCentroid() {Xbar=Ybar=0;};
   TCentroid(double x,double y) {Xbar=x;Ybar=y;};
   TCentroid(const TCentroid& cg) {Xbar = cg.Xbar; Ybar = cg.Ybar;}
   void operator = (const TCentroid& cg) {Xbar = cg.Xbar; Ybar = cg.Ybar;}
};

class SHAPECLASS TPrincipleInertia
{
// Data Members
public:
   double I11,I22,I12;
   double pDir; // Principle direction (radians)

// Constructors
public:
   TPrincipleInertia() { I11 = I22 = I12 = pDir = 0;};
   TPrincipleInertia(double I11,double I22,double I12,double pDir);
};

class SHAPECLASS TInertia
{
// Data Members
public:
   double Ixx, Iyy, Ixy;

// Constructors
public:
   TInertia() {Ixx = 0; Iyy = 0; Ixy = 0;};
   TInertia(double ixx,double iyy,double ixy) {Ixx = ixx; Iyy = iyy; Ixy = ixy;};

// Member Functions
public:
   void TransformInertia(double angle);
   TPrincipleInertia ComputePrincipleValues();
};

#endif // SHAPE_PROPS_H_
