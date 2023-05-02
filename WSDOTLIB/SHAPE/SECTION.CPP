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

#include <shape\section.h>
#include <material\genmat.h>

TSection::TSection(bool roundN) :
TShape(0),
roundN(roundN)
{
   int HIGH = 10;
   int LOW = 0;
   int INCREMENT = 10;

   sContainer = new TShapeContainer(HIGH,LOW,INCREMENT);

   InitProperties();
}

TSection::~TSection()
{
   TRACE("TSection::~TSection()");
   delete sContainer;
}

bool TSection::RoundToNearestN(bool r)
{
   bool temp;
   temp = roundN;
   roundN = r;
   return temp;
}

bool TSection::RoundToNearestN()
{
   return roundN;
}

bool TSection::IsSolid(bool s)
{
   bool temp;
   TShapeContainerIterator i(*sContainer);

   temp = isSolid;
   isSolid = s;

   while (i)
      {
      TShape *shape = i++;
      shape->IsSolid(s);
      }

   return temp;
}

bool TSection::IsStructural(bool s)
{
   bool temp;
   TShapeContainerIterator i(*sContainer);

   temp = isSolid;
   isSolid = s;

   while (i)
      {
      TShape *shape = i++;
      shape->IsSolid(s);
      }

   return temp;
}

void TSection::AddShape(TShape* s)
{
   if (sContainer->GetItemsInContainer() == 0)
      baseShape = s;

   sContainer->Add(s);

   UpdateExtents(s);
}

TShape *TSection::GetShape(int i)
{
   return (*sContainer)[i];
}

void TSection::ComputeLocalProperties()
{
   double EquivUnitWeight = 0;

   TShapeContainerIterator i(*sContainer);
   grossArea = 0;

   while (i)
      {
      TShape *shape = i++;
      shape->ComputeProperties();

      grossArea += shape->GetArea();
      EquivUnitWeight += shape->GetArea() * shape->GetUnitWeight();

      // if shape is non-structural, there a no other properties to compute.
      // continue with next shape in this section.
      if (!shape->IsStructural())
         continue;

      double n;
      n = shape->GetModE()/baseShape->GetModE();
      if (RoundToNearestN())
         RoundN(n);

      area += n*shape->GetArea();

      TInertia IgShape = shape->GetGlobalInertia();
      Iglobal.Ixx += n*IgShape.Ixx;
      Iglobal.Iyy += n*IgShape.Iyy;
      Iglobal.Ixy += n*IgShape.Ixy;

      TCentroid centroid = shape->GetCentroid();
      cg.Xbar += n*shape->GetArea()*centroid.Xbar;
      cg.Ybar += n*shape->GetArea()*centroid.Ybar;
      }

   if (IsNullShape())
      return;

   EquivUnitWeight /= area;

   delete material;
   material = new GeneralMaterial(baseShape->GetModE(),EquivUnitWeight);

   cg.Xbar /= area;
   cg.Ybar /= area;

   Ilocal.Ixx = Iglobal.Ixx - area*cg.Ybar*cg.Ybar;
   Ilocal.Iyy = Iglobal.Iyy - area*cg.Xbar*cg.Xbar;
   Ilocal.Ixy = Iglobal.Ixy - area*cg.Ybar*cg.Xbar;
}

bool TSection::IsNullShape()
{
   bool retval = false;

   if (TShape::IsNullShape())
      {
      InitProperties();
      retval = true;
      }

   return retval;
}

double TSection::GetGrossArea()
{
   return grossArea;
}

void TSection::UpdateExtents(TShape *s)
{
   TFRect r;

   s->GetExtents(r);

   extents.minX = min(r.minX,extents.minX);
   extents.maxX = max(r.maxX,extents.maxX);

   extents.minY = min(r.minY,extents.minY);
   extents.maxY = max(r.maxY,extents.maxY);
}

void TSection::ComputeExtents()
{
   TShapeContainerIterator i(*sContainer);

   extents.minX = extents.maxX = extents.minY = extents.maxY = 0;

   while (i)
      {
      TShape *shape = i++;
      shape->ComputeExtents();
      UpdateExtents(shape);
      }
}

void TSection::InitProperties()
{
   TShape::InitProperties();
   grossArea = 0.;

   TShapeContainerIterator i(*sContainer);

   while (i)
      {
      TShape *shape = i++;
      shape->InitProperties();
      }
}

void TSection::RoundN(double &n)
{
   double N = floor(n);

   // 0.0 to 0.5 round down, else round up
   if ((n - N) > 0.5)
      N += 1.0;

   n = N;
}

TShape*
TSection::Clone() const
{
   TSection* clone = new TSection(roundN);

   TShapeContainerIterator iter(*sContainer);
   while (iter)
      {
      TShape *shape = iter++;
      TShape* shapeClone = shape->Clone();
      clone->AddShape(shapeClone);
      }

   return clone;
}

#if defined(_Windows)
void TSection::Draw(TRealDC &dc)
{
   TShapeContainerIterator i(*sContainer);

   while (i)
      {
      TShape *shape = i++;
      shape->Draw(dc);
      }
}

void TSection::SetColor(TColor Color)
{
   TShapeContainerIterator i(*sContainer);

   while (i)
      {
      TShape *shape = i++;
      shape->SetColor(Color);
      }
}

#endif
