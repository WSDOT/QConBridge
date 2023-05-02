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

#if !defined DIAGRAM_H_
#define DIAGRAM_H_

#include <bridge\brdgdll.h>
#include <bridge\analres.h>
#include <fem2d\femall.h> // for MbrFace
#include <classlib\arrays.h>

class _BRIDGECLASS Diagram
{
public:
   class Point
      {
      public:
         Point(MbrFace face,double x,double y);
         Point(Diagram::Point &p);
         bool operator == (const Diagram::Point &p) const;
         bool operator  < (const Diagram::Point &p) const;
         bool operator <= (const Diagram::Point &p) const;

         MbrFace face;
         double x,y;
      };
   typedef TISArrayAsVector<Diagram::Point> PointContainer;
   typedef TISArrayAsVectorIterator<Diagram::Point> PointIterator;

   int id; // unique identifier
   string description; // description of loading that caused these results

   PointContainer *points;

   double minX,minY,maxX,maxY;

public:
   Diagram(int id,const char *description = 0);
   Diagram(int id,Diagram &d,const char *description = 0);
   Diagram(int id,AnalysisResults &ar);
   virtual ~Diagram();
   int GetID() const { return id; }
   char *GetDescription(char *desc,int maxChar);
   string GetDescription();
   virtual void AddPoint(Diagram::Point *p);
   virtual void AddDiagram(Diagram &d);
   int NumPoints() const;
   void GetPoint(int idx,double &x,double &y);
   Diagram::Point *GetPoint(int idx);
   virtual double Evaluate(double x,MbrFace face = mbrLeftFace);
   void GetRange(double &minx,double &maxx,double &miny,double &maxy);

   bool operator == (const Diagram &d) const {return id == d.GetID();}

#if defined GEN_DEBUG_CODE
   virtual void Write(ostream &os);
   friend ostream &operator << (ostream &os,Diagram &diagram);
#endif

protected:
   double Interpolate(Diagram::Point *p1,Diagram::Point *p2,double x,MbrFace face);
};

typedef TIArrayAsVector<Diagram>         DiagramContainer;
typedef TIArrayAsVectorIterator<Diagram> DiagramIterator;

#if defined GEN_DEBUG_CODE
   ostream &operator << (ostream &os,Diagram &diagram);
#endif

#endif // DIAGRAM_H_