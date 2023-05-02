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

#if !defined SHAPE_BOXGDR_H_
#define SHAPE_BOXGDR_H_

#include <shape\section.h>
#include <shape\polygon.h>
#include <shape\posrect.h>

typedef struct
   {
   double thick;
   BOOL   horizontal;
   double offset;
   double taper;
   double bTopFillet;
   double hTopFillet;
   double bBotFillet;
   double hBotFillet;
   } EXTWEB;

typedef struct
   {
   double thick;
   double lTaper;
   double rTaper;
   double bTopLeftFillet;
   double hTopLeftFillet;
   double bTopRightFillet;
   double hTopRightFillet;
   double bBotLeftFillet;
   double hBotLeftFillet;
   double bBotRightFillet;
   double hBotRightFillet;
   } INTWEB;

typedef struct
   {
   double leftOverhang;
   double rightOverhang;
   double thick;
   double leftExtThick;
   double leftIntThick;
   double rightExtThick;
   double rightIntThick;
   double tWearing;
   double modE;
   double density;
   } TOPSLAB;

class SHAPECLASS TBoxGirder : public TSection
{
// Data Members
protected:
   double  botSlabWidth;
   double  botSlabThick;
   double  boxHeight;
   EXTWEB  lextWeb;
   EXTWEB  rextWeb;
   TOPSLAB topSlab;

   TPolyShape *botSlabShape;
   TPolyShape *topSlabShape;
   TPolyShape *wearingSurface;

   double Qtop; // top slab
   double Qbot; // bottom slab

// Constructors/Destructor
public:
   TBoxGirder(double bsw,double bst,double bh,EXTWEB &lWeb,EXTWEB &rWeb,TOPSLAB &ts,
              Material* material);

// Member Functions
public:
   void AddInteriorWeb(double loc,INTWEB &web,Material* material);
   void AddInteriorWeb(double loc,double space,int nwebs,INTWEB &web,Material* material);
   double GetQts();
   double GetQbs();

protected:
   void GenerateSection(Material* material);
   void ComputeLocalProperties();
};

#endif // SHAPE_BOXGDR_H_
