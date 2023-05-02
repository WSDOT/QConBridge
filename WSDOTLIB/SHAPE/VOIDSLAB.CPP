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

#include <shape\voidslab.h>
#include <shape\polygon.h>
#include <shape\ellipse.h>
#include <math.h>
#include <material\genmat.h>

TVoidedSlab::TVoidedSlab(double W,double H,double LET,double RET,double LO,double RO,
                         double crwnOffset,double crwnHeight,
                         double FV,double VS,double VCG,double VD,int n,
                         TVoidType type,Material* material) :
TSection()
{
   int i;
   double loc;

   Init(W,H,LET,RET,LO,RO,crwnOffset,crwnHeight);
   GenerateSection(material);

   for (i = 0; i < n; i++)
      {
      loc = FV + i*VS;
      AddVoid(loc,VCG,VD,type);
      }
}

TVoidedSlab::TVoidedSlab(double W,double H,double LET,double RET,double LO,double RO,
                         double crwnOffset,double crwnHeight,
                         Material* material) :
TSection()
{
   Init(W,H,LET,RET,LO,RO,crwnOffset,crwnHeight);
   GenerateSection(material);
}

void TVoidedSlab::Init(double W,double H,double LET,double RET,double LO,double RO,
                       double crwnOffset,double crwnHeight)
{
   voidStats = new TVoidStatContainer(10,0,10);

   width         = W;
   height        = H;
   leftExtThick  = LET;
   rightExtThick = RET;
   leftOverhang  = LO;
   rightOverhang = RO;
   crownOffset   = crwnOffset;
   crownHeight   = crwnHeight;
   numVoids = 0;
}

TVoidedSlab::~TVoidedSlab()
{
   delete voidStats;
}

void TVoidedSlab::GenerateSection(Material* material)
{
   // Main slab
   TPolyShape *slab;
   slab = new TPolyShape(material,TFPoint(0,0),0);
   slab->AddPoint(0,height);
   slab->AddPoint(width,height);
   slab->AddPoint(width,height-rightExtThick);
   slab->AddPoint(width-rightOverhang,0);
   slab->AddPoint(leftOverhang,0);
   slab->AddPoint(0,height-leftExtThick);
   AddShape(slab);

   // Crown
   TPolyShape *crown;
   crown = new TPolyShape(material->Clone(),TFPoint(0,0),0);
   crown->IsStructural(FALSE);
   crown->AddPoint(0,height);
   crown->AddPoint(crownOffset,height+crownHeight);
   crown->AddPoint(width,height);
   AddShape(crown);


   // first void shape index is 2. Main slab is 0, crown is 1.
   firstVoidShapeIdx = 2;
}

void TVoidedSlab::AddVoid(double loc,double vcg,double voidWidth,TVoidType type)
{
   TCircle    *circle;
   TPolyShape *octagon;
   TSquare    *square;
   TVoidStat  voidStat;

   switch(type)
      {
      case Circle:
           circle = new TCircle(voidWidth,new GeneralMaterial(baseShape->GetModE(),baseShape->GetUnitWeight()),TFPoint(loc,vcg),CenterCenter);
           circle->IsSolid(FALSE);
           AddShape(circle);
           break;

      case Octagon:
           double s;
           s = voidWidth*tan(M_PI/8);
           octagon = new TPolyShape(new GeneralMaterial(baseShape->GetModE(),baseShape->GetUnitWeight()),TFPoint(0,0),0);
           octagon->AddPoint(loc - s/2,vcg - voidWidth/2);
           octagon->AddPoint(loc - s/2 - s*cos(M_PI/4),vcg - voidWidth/2 + s*sin(M_PI/4));
           octagon->AddPoint(loc - s/2 - s*cos(M_PI/4),vcg + voidWidth/2 - s*sin(M_PI/4));
           octagon->AddPoint(loc - s/2,vcg + voidWidth/2);
           octagon->AddPoint(loc + s/2,vcg + voidWidth/2);
           octagon->AddPoint(loc + s/2 + s*cos(M_PI/4),vcg + voidWidth/2 - s*sin(M_PI/4));
           octagon->AddPoint(loc + s/2 + s*cos(M_PI/4),vcg - voidWidth/2 + s*sin(M_PI/4));
           octagon->AddPoint(loc + s/2,vcg - voidWidth/2);
           octagon->IsSolid(FALSE);
           AddShape(octagon);
           break;

      case Square:
           square = new TSquare(voidWidth,new GeneralMaterial(baseShape->GetModE(),baseShape->GetUnitWeight()),0.,TFPoint(loc,vcg),CenterCenter);
           square->IsSolid(FALSE);
           AddShape(square);
           break;
      }

   // store void type at the same index as the void is stored in
   // the shape container. This will make retreival consistant.
   voidStat.type = type;
   voidStat.location = loc;
   voidStat.cg = vcg;
   voidStat.diameter = voidWidth;
   voidStats->AddAt(voidStat,numVoids+firstVoidShapeIdx);
   numVoids++;
}

double TVoidedSlab::GetWidth()
{
   return width;
}

double TVoidedSlab::GetHeight()
{
   return height;
}

double TVoidedSlab::GetLeftExtThickness()
{
   return leftExtThick;
}

double TVoidedSlab::GetRightExtThickness()
{
   return rightExtThick;
}

double TVoidedSlab::GetLeftOverhang()
{
   return leftOverhang;
}

double TVoidedSlab::GetRightOverhang()
{
   return rightOverhang;
}

double TVoidedSlab::GetCrownOffset()
{
   return crownOffset;
}

double TVoidedSlab::GetCrownHeight()
{
   return crownHeight;
}

int TVoidedSlab::GetNumVoids()
{
   return numVoids;
}

double TVoidedSlab::GetVoidLocation(int v)
{
   int idx;
   TVoidStat voidStat;

   idx = v + firstVoidShapeIdx;
   voidStat = (*voidStats)[idx];
   return voidStat.location;
}

double TVoidedSlab::GetVoidCG(int v)
{
   int idx;
   TVoidStat voidStat;

   idx = v + firstVoidShapeIdx;
   voidStat = (*voidStats)[idx];
   return voidStat.cg;
}

double TVoidedSlab::GetVoidDiameter(int v)
{
   int idx;
   TVoidStat voidStat;

   idx = v + firstVoidShapeIdx;
   voidStat = (*voidStats)[idx];
   return voidStat.diameter;
}

TVoidType TVoidedSlab::GetVoidType(int v)
{
   int idx;
   TVoidStat voidStat;

   idx = v + firstVoidShapeIdx;
   voidStat = (*voidStats)[idx];
   return voidStat.type;
}

#if defined (_Windows)
void TVoidedSlab::Draw(TRealDC &dc)
{
   int i;
   for (i = firstVoidShapeIdx; i < numVoids+firstVoidShapeIdx; i++)
      {
      TShape *shape;
      shape = GetShape(i);
      shape->SetColor(TColor::SysWindow);
      }

   TSection::Draw(dc);
}
#endif

