///////////////////////////////////////////////////////////////////////
// Bridge32 - Bridge Modeling Framework
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

#include <bridge\diagram.h>
#include <values.h>

#include <bridge\debug.h>

Diagram::Point::Point(MbrFace face,double x,double y) :
face(face),x(x),y(y)
{
}

Diagram::Point::Point(Diagram::Point &p)
{
   face = p.face;
   x = p.x;
   y = p.y;
}

bool Diagram::Point::operator == (const Diagram::Point &p) const
{
   return (p.face == face && zero_val(p.x - x));
}

bool Diagram::Point::operator < (const Diagram::Point &p) const
{
   bool retval;

   if (zero_val(x - p.x))
      retval = (face < p.face);
   else
      retval = (x < p.x);

   return retval;
}

bool Diagram::Point::operator <= (const Diagram::Point &p) const
{
   bool isLessThan;
   bool isEqualTo;

   isLessThan = (*this < p);
   isEqualTo = (*this == p);

   return (isLessThan || isEqualTo);
}

//
//
//

Diagram::Diagram(int id,const char *description) :
id(id),
description(description)
{
   points = new PointContainer(10,0,10);
   minX =  MAXDOUBLE;
   minY =  MAXDOUBLE;
   maxX = -MAXDOUBLE;
   maxY = -MAXDOUBLE;
}

Diagram::Diagram(int id,Diagram &d,const char *description) :
id(id),
description(description)
{
   int numPoints;
   Diagram::Point *point;
   Diagram::Point *newPoint;

   points = new PointContainer(10,0,10);
   minX =  MAXDOUBLE;
   minY =  MAXDOUBLE;
   maxX = -MAXDOUBLE;
   maxY = -MAXDOUBLE;

   numPoints = d.NumPoints();
   for (int i = 0; i < numPoints; i++)
      {
      point = d.GetPoint(i);
      newPoint = new Diagram::Point(*point);
      AddPoint(newPoint);
      }
}

Diagram::Diagram(int id,AnalysisResults &ar) :
id(id)
{
   int i, numResults;
   double offset, lfValue, rfValue;
   Diagram::Point *newPoint;

   points = new PointContainer(10,0,10);
   minX =  MAXDOUBLE;
   minY =  MAXDOUBLE;
   maxX = -MAXDOUBLE;
   maxY = -MAXDOUBLE;

   description = ar.GetDescription();
   numResults = ar.GetNumResults();

   for (i = 0; i < numResults; i++)
      {
      ar.GetResultByIdx(i,offset,lfValue,rfValue);

      newPoint = new Diagram::Point(mbrLeftFace,offset,lfValue);
      AddPoint(newPoint);

      newPoint = new Diagram::Point(mbrRightFace,offset,rfValue);
      AddPoint(newPoint);
      }
}

Diagram::~Diagram()
{
   delete points;
}

char *Diagram::GetDescription(char *desc,int maxChar)
{
   memset(desc,0,maxChar);
   strncat(desc,description.c_str(),maxChar);
   return desc;
}

string Diagram::GetDescription()
{
   return description;
}

void Diagram::AddPoint(Diagram::Point *point)
{
   point->y = zero_val(point->y) ? 0. : point->y;

   minX = min(minX,point->x);
   maxX = max(maxX,point->x);
   minY = min(minY,point->y);
   maxY = max(maxY,point->y);

   points->Add(point);
}

void Diagram::AddDiagram(Diagram &d)
{
   PointIterator i(*d.points);

   while(i)
      {
      Diagram::Point *point = i++;
      Diagram::Point *newPoint;

      newPoint = new Diagram::Point(*point);
      AddPoint(newPoint);
      }
}

int Diagram::NumPoints() const
{
   return points->GetItemsInContainer();
}

void Diagram::GetPoint(int idx,double &x,double &y)
{
   Diagram::Point *p = GetPoint(idx);
   x = p->x;
   y = p->y;
}

Diagram::Point *Diagram::GetPoint(int idx)
{
   PRECONDITIONX(idx < points->GetItemsInContainer(),"Diagram::GetPoint() - Invalid index");
   return (*points)[idx];
}

double Diagram::Evaluate(double x,MbrFace face)
{
   Diagram::Point *p1 = NULL;
   Diagram::Point *p2 = NULL;
   Diagram::Point testPoint(face,x,0);
   PointIterator i(*points);
   double y;

   PRECONDITIONX(minX <= x && x <= maxX,"Diagram::Evaluate() - x is out of range");

   p1 = i++;
   while (i)
      {
      p2 = i++;
      if ( (*p1) <= testPoint && testPoint <= (*p2) )
          break;
      p1 = p2;
      }

   if (p1 == NULL || p2 == NULL)
      y = 0;
   else
      y = Interpolate(p1,p2,x,face);

   y = (zero_val(y)) ? 0 : y;

   return y;
}

void Diagram::GetRange(double &minx,double &maxx,double &miny,double &maxy)
{
   minx = minX;
   maxx = maxX;
   miny = minY;
   maxy = maxY;
}

double Diagram::Interpolate(Diagram::Point *p1,Diagram::Point *p2,double x,MbrFace face)
{
   double dx,dy;
   double y;

   PRECONDITIONX(p1 != NULL,"Diagram::Interpolate() - p1 cannot be NULL");
   PRECONDITIONX(p2 != NULL,"Diagram::Interpolate() - p2 cannot be NULL");

   dx = p2->x - p1->x;
   dy = p2->y - p1->y;

   if (zero_val(dx))
      {
      if (face == mbrLeftFace)
         y = p1->y;
      else
         y = p2->y;
      }
   else
      y = p1->y + (x - p1->x)*dy/dx;

   return y;
}

#if defined GEN_DEBUG_CODE

#include <iomanip.h>

void Diagram::Write(ostream &os)
{
   Diagram::PointIterator i(*points);

   os << "Diagram Points for "
      << GetDescription()
      << " ID (" << GetID() << ")" << endl;

   while(i)
      {
      Diagram::Point *point = i++;
      os << setiosflags(ios::right | ios::scientific | ios::showpoint)
         << setw(12) << setprecision(4)
         << point->x << " "
         << setw(12) << setprecision(4)
         << point->y << endl;
      }
}

ostream &operator << (ostream &os,Diagram &diagram)
{
   diagram.Write(os);

   return os;
}

#endif

