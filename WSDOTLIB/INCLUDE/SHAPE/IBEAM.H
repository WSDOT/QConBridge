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

#if !defined SHAPE_IBEAM_H_
#define SHAPE_IBEAM_H_

#include <shape\section.h>
#include <shape\compslab.h>
#include <shape\posrect.h>

class SHAPECLASS TIBeam : public TSection
{
// Data Members
protected:
   TPosRect *posRect;
   TPolyShape *ibeam;
   double A,B,C,D,E,F,G,H,J,V,W,X,Y,Z;

// Constructors/Destructor
public:
   TIBeam(double a,double b,double c,double d,double e,double f,double g,
          double h,double j,double v,double w,double x,double y,double z,
          Material* material,
          TFPoint& rp = TFPoint(0,0),
          TLocationPoint lp = BottomCenter);
   ~TIBeam();

// Member Functions
public:
   double GetA();
   double GetB();
   double GetC();
   double GetD();
   double GetE();
   double GetF();
   double GetG();
   double GetH();
   double GetJ();
   double GetV();
   double GetW();
   double GetX();
   double GetY();
   double GetZ();

   double GetHeight() {return GetH();}
   double GetTopFlangeWidth() {return GetA();}
   double GetWebWidth() {return 0.5*(GetV() + GetW());}

protected:
   void GenerateSection(Material* material);

   friend class SHAPECLASS TCompIBeam;
};

class SHAPECLASS TCompIBeam : public TSection
{
// Data Members
protected:
   double Qslab;
   TCompSlab* slab;
   TIBeam* ibeam;

// Constructors/Destructor
public:
   TCompIBeam(double a,double b,double c,double d,double e,double f,double g,
              double h,double j,double v,double w,double x,double y,double z,
              Material* matGirder,
              double ts,double ws,double so,double tw,
              Material* matSlab,
              TFPoint& rp = TFPoint(0,0),TLocationPoint lp = CenterCenter);

// Member Functions
public:
   TIBeam& GetIBeam() const {return *ibeam;}
   TCompSlab& GetSlab() const {return *slab;}

   double GetEslab();
   double GetSlabUnitWeight();
   double GetTslab();
   double GetWslab();
   double GetSlabOffset();
   double GetTwearing();
   double GetQslab();

#if defined (_Windows)
   void SetColor(TColor beamColor,TColor slabColor);
#endif

protected:
   void ComputeLocalProperties();
};

#endif // SHAPE_IBEAM_H_
