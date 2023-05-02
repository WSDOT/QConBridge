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

#ifndef TUBE_HPP_
#define TUBE_HPP_

#include "section.hpp"

class TRectTube : public TSection
{
// Data Members
protected:
   double h, b, t1, t2, ri, ro;

// Constructors/Destructor
public:
   TRectTube(double H,double B,double T1,double T2,double Ri,double Ro,int color = 1,double e =1.,double d = 1.);

// Member Functions
protected:
   void GenerateSection(void);
};

class TEllipticalTube : public TSection
{
// Data Members
protected:
   double b, h, t;

// Constructors/Destructor
public:
   TEllipticalTube(double B,double H,double T,int color = 1,double e =1.,double d = 1.);

// Member Functions
protected:
   void GenerateSection(void);
};

class TCircularTube : public TEllipticalTube
{
// Data Members

// Constructors/Destructor
public:
   TCircularTube(double D,double T,int color = 1,double e =1.,double d = 1.);

// Member Functions
};

#endif // TUBE_HPP_