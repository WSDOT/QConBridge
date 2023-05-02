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

#if !defined SHAPE_VOIDSLAB_H_
#define SHAPE_VOIDSLAB_H_

#include <shape\section.h>

typedef enum TVoidType {Circle,Octagon,Square} TVoidType;
struct TVoidStat
   {
   TVoidType type;
   double location;
   double cg;
   double diameter;
   bool operator == (const TVoidStat &s) {return this->type == s.type;};
   };

typedef TArrayAsVector<TVoidStat> TVoidStatContainer;

class SHAPECLASS TVoidedSlab : public TSection
{
// Data Members
private:
   int firstVoidShapeIdx; // index into shape array for the first void.

protected:
   TVoidStatContainer *voidStats;
   int numVoids;
   double width;
   double height;
   double leftExtThick, rightExtThick;
   double leftOverhang, rightOverhang;
   double crownOffset,crownHeight;

// Constructors/Destructor
public:
   TVoidedSlab(double W,double H,double LET,double RET,double LO,double RO,
               double crwnOffset,double crwnHeight,double FV,
               double VS,double VCG,double VD,int n,TVoidType type,Material* material);
   TVoidedSlab(double W,double H,double LET,double RET,double LO,double RO,
               double crwnOffset,double crwnHeight,
               Material* material);
   ~TVoidedSlab();

// Member Functions
public:
   void      AddVoid(double loc,double h,double voidWidth,TVoidType type);
   double    GetWidth();
   double    GetHeight();
   double    GetLeftExtThickness();
   double    GetRightExtThickness();
   double    GetLeftOverhang();
   double    GetRightOverhang();
   double    GetCrownOffset();
   double    GetCrownHeight();
   int       GetNumVoids();
   double    GetVoidLocation(int v);
   double    GetVoidCG(int v);
   double    GetVoidDiameter(int v);
   TVoidType GetVoidType(int v);

#if defined (_Windows)
   void Draw(TRealDC &dc);
#endif

protected:
   void Init(double W,double H,double LET,double RET,double LO,double RO,double crwnOffset,double crwnHeight);
   void GenerateSection(Material* material);
};

#endif // SHAPE_VOIDSLAB_H_
