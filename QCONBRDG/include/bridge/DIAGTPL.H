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

#if !defined DIAGTPL_H
#define DIAGTPL_H

#if defined (BUILDBRIDGE) && defined(_BRIDGEDLL)
#define _BRIDGECLASS _export
#elif defined(_BRIDGEDLL)
#define _BRIDGECLASS _import
#else
#define _BRIDGECLASS
#endif

#if !defined( CLASSLIB_ARRAYS_H )
#include <classlib\arrays.h>
#endif

#include <values.h>

#define EPS  1.0e-10
#undef   zero_val
#define  zero_val( x )     (((-EPS)<=(x)) && ((x)<=(EPS)))

template <class Pnt>
class _BRIDGECLASS DiagramTemplate
{
public:
   typedef TISArrayAsVector<Pnt> PointContainer;
   typedef TISArrayAsVectorIterator<Pnt> PointIterator;

   int id; // unique identifier
   string description; // description of loading that caused these results

   PointContainer *points;

   double minX,minY,maxX,maxY;

public:
   DiagramTemplate(int id,const char *description = 0) :
   id(id),
   description(description)
   {
      points = new PointContainer(10,0,10);
      minX =  MAXDOUBLE;
      minY =  MAXDOUBLE;
      maxX = -MAXDOUBLE;
      maxY = -MAXDOUBLE;
   }


   DiagramTemplate(int id,DiagramTemplate<Pnt> &d,const char *description = 0) :
   id(id),
   description(description)
   {
      int numPoints;
      Pnt *point;
      Pnt *newPoint;

      points = new PointContainer(10,0,10);
      minX =  MAXDOUBLE;
      minY =  MAXDOUBLE;
      maxX = -MAXDOUBLE;
      maxY = -MAXDOUBLE;

      numPoints = d.NumPoints();
      for (int i = 0; i < numPoints; i++)
         {
         point = d.GetPoint(i);
         newPoint = new Pnt(*point);
         AddPoint(newPoint);
         }
   }

   virtual ~DiagramTemplate() { delete points; }

   int GetID() const { return id; }

   char *GetDescription(char *desc,int maxChar)
      {
      memset(desc,0,maxChar);
      strncat(desc,description.c_str(),maxChar);
      return desc;
      }

   string GetDescription() {return description;}

   virtual void AddPoint(Pnt *point)
      {
      point->y = zero_val(point->y) ? 0. : point->y;

      minX = min(minX,point->x);
      maxX = max(maxX,point->x);
      minY = min(minY,point->y);
      maxY = max(maxY,point->y);

      points->Add(point);
      }

   virtual void AddDiagram(DiagramTemplate<Pnt> &d)
      {
      PointIterator i(*d.points);

      while(i)
         {
         Pnt *point = i++;
         Pnt *newPoint;

         newPoint = new Pnt(*point);
         AddPoint(newPoint);
         }
      }

   int NumPoints() const
      {return points->GetItemsInContainer();}

   void GetPoint(int idx,double &x,double &y)
      {
      Pnt *p = GetPoint(idx);
      x = p->x;
      y = p->y;
      }

   Pnt *GetPoint(int idx)
      {
      PRECONDITIONX(idx < points->GetItemsInContainer(),"DiagramTemplate::GetPoint() - Invalid index");
      return (*points)[idx];
      }

   virtual double Evaluate(double x)
   {
      Pnt *p1 = NULL;
      Pnt *p2 = NULL;
      PointIterator i(*points);
      double y;

      PRECONDITIONX(minX <= x && x <= maxX,"DiagramTemplate::Evaluate() - x is out of range");

      p1 = i++;
      while (i)
         {
         p2 = i++;
         if (p1->x <= x && x <= p2->x)
             break;
         p1 = p2;
         }

      if (p1 == NULL || p2 == NULL)
         y = 0;
      else
         y = Interpolate(p1,p2,x);

      y = (zero_val(y)) ? 0 : y;

      return y;
   }

   void GetRange(double &minx,double &maxx,double &miny,double &maxy)
   {
      minx = minX;
      maxx = maxX;
      miny = minY;
      maxy = maxY;
   }

   bool operator == (const DiagramTemplate<Pnt> &d) const {return id == d.GetID();}

protected:
   double Interpolate(Pnt *p1,Pnt *p2,double x)
   {
      double dx,dy;
      double y;

      PRECONDITIONX(p1 != NULL,"DiagramTemplate::Interpolate() - p1 cannot be NULL");
      PRECONDITIONX(p2 != NULL,"DiagramTemplate::Interpolate() - p2 cannot be NULL");

      dx = p2->x - p1->x;
      dy = p2->y - p1->y;

      if (zero_val(dx))
        {
        y = p1->y;
        }
      else
         y = p1->y + (x - p1->x)*dy/dx;

      return y;
   }

#if defined GEN_DEBUG_CODE
   friend ostream &operator << (ostream &os,DiagramTemplate<Pnt> &diagram);
#endif
};

#if defined GEN_DEBUG_CODE

#include <iomanip.h>
template <class Pnt>
ostream &operator << (ostream &os,DiagramTemplate<Pnt> &diagram)
{
   DiagramTemplate<Pnt>::PointIterator i(*diagram.points);

   os << "DiagramTemplate Points for "
      << diagram.GetDescription()
      << " ID (" << diagram.GetID() << ")" << endl;

   while(i)
      {
      Pnt *point = i++;
      os << setiosflags(ios::right | ios::scientific | ios::showpoint)
         << setw(12) << setprecision(4)
         << point->x << " "
         << setw(12) << setprecision(4)
         << point->y << endl;
      }

   return os;
}

#endif // GEN_DEBUG_CODE

#endif // DIAGTPL_H_