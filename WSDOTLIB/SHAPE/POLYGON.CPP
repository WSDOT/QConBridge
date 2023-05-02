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

#include <shape\polygon.h>
#include <math.h>

//
// TPolyShape
//

TPolyShape::TPolyShape(Material* material,TFPoint Origin,double Angle) :
TShape(material)
{
   int HIGH = 10;
   int LOW = 0;
   int INCREMENT = 10;

   pntContainer = new TIFPointContainer(HIGH,LOW,INCREMENT);
   origin = Origin;
   angle  = Angle;
}

TPolyShape::~TPolyShape()
{
   TRACE("TPolyShape::~TPolyShape()");
   delete pntContainer;
}

void TPolyShape::AddPoint(double x,double y)
{
   TFPoint *newPoint;

   // rotate point from global x-y to the current local coordinate system
   newPoint = new TFPoint(x*cos(angle) - y*sin(angle) + origin.x,
                          y*cos(angle) + x*sin(angle) + origin.y);

   pntContainer->Add(newPoint);

   UpdateExtents(*newPoint);
}

void TPolyShape::AddPoint(TFPoint &sp)
{
   AddPoint(sp.x,sp.y);
}

void TPolyShape::SetOrigin(double x,double y)
{
   SetOrigin(TFPoint(x,y));
}

void TPolyShape::SetOrigin(TFPoint Origin)
{
   TFPoint oldOrigin = origin;
   origin = Origin;

   // Adjust coordinates of all points in the container for the new origin
   TIFPointIterator i(*pntContainer);
   while (i)
      {
      TFPoint *pnt = i++;
      *pnt -= oldOrigin;
      *pnt += origin;
      }
}

void TPolyShape::SetRotationAngle(double Angle)
{
   double oldAngle = angle;
   angle = Angle;

   // Adjust coordinates of all points in the container for the new
   // local coordinate system
   TIFPointIterator i(*pntContainer);
   while (i)
      {
      TFPoint *pnt = i++;
      TFPoint tempPnt;
      *pnt -= origin;
      tempPnt.x = pnt->x * cos(-oldAngle) - pnt->y * sin(-oldAngle);
      tempPnt.y = pnt->y * cos(-oldAngle) + pnt->x * sin(-oldAngle);
      pnt->x = tempPnt.x*cos(angle) - tempPnt.y*sin(angle);
      pnt->y = tempPnt.y*cos(angle) + tempPnt.x*sin(angle);
      *pnt += origin;
      }
}

void TPolyShape::ComputeLocalProperties()
{
   int numPoints;
   int pnt;
   double dist;

   numPoints = pntContainer->GetItemsInContainer();

   // repeat first point as last point if they are different
   dist = (*pntContainer)[0]->Distance(*(*pntContainer)[numPoints-1]);
   if (!zero_val(dist))
      {
      // don't use AddPoint... AddPoint will perform coordinate
      // transformations to a point that is already in the correct
      // coordinate system.
      pntContainer->Add( new TFPoint(*(*pntContainer)[0]) );
      numPoints = pntContainer->GetItemsInContainer();
      }

   for (pnt = 0; pnt < numPoints-1; pnt++)
      {
      double dy, dx;
      double Ar, At;
      TFPoint *pnt1, *pnt2;

      pnt1 = (*pntContainer)[pnt];
      pnt2 = (*pntContainer)[pnt+1];

      dx = pnt2->x - pnt1->x;
      dy = pnt2->y - pnt1->y;

      Ar = dx * pnt1->y;
      At = 0.5*dy*dx;

      // Gross Area
      area += (Ar + At);

      // Centriod
      cg.Xbar += Ar*(pnt2->x + pnt1->x)/2 + At*(2*dx/3 + pnt1->x);
      cg.Ybar += Ar*(pnt1->y/2) + At*(dy/3 + pnt1->y);

      // Inertia about global axes
      Iglobal.Ixx += (pnt1->y)*(pnt1->y)*(pnt1->y)*dx/12 + Ar*(pnt1->y/2)*(pnt1->y/2) +
                     dy*dy*dy*dx/36 + At*(dy/3 + pnt1->y)*(dy/3 + pnt1->y);

      Iglobal.Iyy += (pnt1->y)*dx*dx*dx/12 + Ar*(pnt1->x + dx/2)*(pnt1->x + dx/2) +
                     dy*dx*dx*dx/36 + At*(2*dx/3 + pnt1->x)*(2*dx/3 + pnt1->x);

      Iglobal.Ixy += Ar*(pnt1->y/2)*(pnt1->x + dx/2) +
                     At*(dy/3 + pnt1->y)*(2*dx/3 + pnt1->x) +
                     dy*dy*dx*dx/72;
      }

   if (IsNullShape())
      return;

   // Finish centriod
   cg.Xbar /= area;
   cg.Ybar /= area;

   // Inertia about local axes
   Ilocal.Ixx = Iglobal.Ixx - area*cg.Ybar*cg.Ybar;
   Ilocal.Iyy = Iglobal.Iyy - area*cg.Xbar*cg.Xbar;
   Ilocal.Ixy = Iglobal.Ixy - area*cg.Ybar*cg.Xbar;

   // If the polygon was defined counter-clockwise, then the product of
   // inertia will be off by a factor of -1. You can tell if the polygon
   // was described cw or ccw by the sign of the area. If the area is less
   // than zero, the polygon was described ccw.
   if (area < 0)
      {
      Iglobal.Ixy *= -1.;
      Ilocal.Ixy  *= -1.;
      }
}

void TPolyShape::ComputeExtents()
{
   TIFPointIterator i(*pntContainer);

   extents.minX = extents.maxX = extents.minY = extents.maxY = 0;
   while(i)
      {
      TFPoint *pnt = i++;

      extents.minX = min(extents.minX,pnt->x);
      extents.maxX = max(extents.maxX,pnt->x);

      extents.minY = min(extents.minY,pnt->y);
      extents.maxY = max(extents.maxY,pnt->y);
      }
}

void TPolyShape::UpdateExtents(TFPoint &pnt)
{
   extents.minX = min(extents.minX,pnt.x);
   extents.maxX = max(extents.maxX,pnt.x);

   extents.maxY = max(extents.maxY,pnt.y);
   extents.minY = min(extents.minY,pnt.y);
}

TShape*
TPolyShape::Clone() const
{
   TPolyShape* clone = new TPolyShape(material->Clone(),origin,angle);

   TIFPointIterator iter(*pntContainer);
   while(iter)
      {
      TFPoint* point = iter++;
      clone->AddPoint(point->x,point->y);
      }

   return clone;
}

#if defined(_Windows)
void TPolyShape::Draw(TRealDC &dc)
{
   dc.SelectObject(TBrush(color));
   dc.SelectObject(TPen(color));

   TIFPointIterator iter(*pntContainer);
   TFPoint* points = new TFPoint[pntContainer->GetItemsInContainer()];
   int count = 0;
   while(iter)
      {
      TFPoint* point = iter++;
      points[count].x = point->x;
      points[count].y = point->y;
      count++;
      }

   dc.Polygon(points,count);
   delete[] points;
}
#endif

//
// TTrapezoid
//
TTrapezoid::TTrapezoid(double H,double B1,double B2,double C,Material* material,double Angle,
            TFPoint &rp,TLocationPoint lp) :
TPolyShape(material,TFPoint(0,0),Angle)
{
   h  = H;
   b1 = B1;
   b2 = B2;
   c  = C;

   posTrap = new TPosTrapezoid(H,B1,B2,C,Angle,rp,lp);
   GenerateShape();
}

TTrapezoid::~TTrapezoid()
{
   if (posTrap)
      delete posTrap;
}

void TTrapezoid::GenerateShape()
{
   TFPoint origin;

   origin = posTrap->GetBottomLeft();
   SetOrigin(origin);
   AddPoint(0,0);
   AddPoint(c,h);
   AddPoint(c+b1,h);
   AddPoint(b2,0);
}

//
// TParallelogram
//
TParallelogram::TParallelogram(double H,double B,double C,Material* material,double Angle,
                               TFPoint &rp,TLocationPoint lp) :
TTrapezoid(H,B,B,C,material,Angle,rp,lp)
{
}

TParallelogram::~TParallelogram()
{
}

//
// TRectangle
//
TRectangle::TRectangle(double H,double B,Material* material,double Angle,
                       TFPoint &rp,TLocationPoint lp) :
TParallelogram(H,B,0,material,Angle,rp,lp)
{
}

TRectangle::~TRectangle()
{
}

//
// TSquare
//
TSquare::TSquare(double S,Material* material,double Angle,
                 TFPoint &rp,TLocationPoint lp) :
TRectangle(S,S,material,Angle,rp,lp)
{
}

TSquare::~TSquare()
{
}

//
// TRightTriangle
//
TRightTriangle::TRightTriangle(double B,double H,Material* material,double Angle,
                               TFPoint &rp,TLocationPoint lp) :
TTrapezoid(H,B,0.,0.,material,Angle,rp,lp)
{
}

TRightTriangle::~TRightTriangle()
{
}
