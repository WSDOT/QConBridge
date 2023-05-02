///////////////////////////////////////////////////////////////////////
// QConBridge - HL93 Live Load Analysis
// Copyright (C) 1999  Washington State Department of Transportation
//                     Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation, 
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but 
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source 
// License along with this program; if not, write to the Washington 
// State Department of Transportation, Bridge and Structures Office, 
// 4500 3rd AVE SE - P.O. Box  47340, Olympia, WA 98503, USA or e-mail 
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Revision Log
// ============
// May 1997 - Created, Richard Brice, PE
// Oct 1999 - Released as Open Source

#if !defined (DIAGDOBJ_H_)
#define DIAGDOBJ_H_

#include <qconbrdg\dispobj.h>
#include <bridge\diagram.h>
#include <bridge\lrfdbrdg.h> // for DiagramCatagory
#include <owltools\symbol.h>
#include <classlib\arrays.h>
#include <owl\dc.h>
#include <owl\gdiobjec.h>
#include <winsys\geometry.h>

#define otDiagram 4

class TDiagramDisplayObject : public TDisplayObject
{
public:
   TDiagramDisplayObject(TBridgeDisplayObjectManager *doMgr,DiagramCatagory cat);
   ~TDiagramDisplayObject();

   DiagramCatagory GetDiagramCatagory();
   int  ObjectType() const;
   void Draw(TDC &dc);
   void Setup(TMapper &map);
   void SetColor(const TColor newColor);
   void SetPenStyle(const int newPenStyle);
   void SetSymbol(TSymbol *newSymbol);
   void AddDiagram(Diagram *diagram);
   void GetRange(double &minx,double &maxx,double &miny,double &maxy);
   bool operator == (const TDiagramDisplayObject &ddo) const {return true;}

protected:
   DiagramCatagory dc;
   DiagramContainer *diagrams;
   typedef TIArrayAsVector<TPoint> PointContainer;
   typedef TIArrayAsVectorIterator<TPoint> PointIterator;

   struct TPointContainer
      {
      PointContainer *points;
      TPointContainer() {points = new PointContainer(10,0,10);}
      ~TPointContainer() { delete points; }
      bool operator == (const TPointContainer &pc) const { return true; }
      };

   typedef TIArrayAsVector<TPointContainer> PointContainerContainer;
   typedef TIArrayAsVectorIterator<TPointContainer> PointContainerIterator;
   PointContainerContainer *pointContainers;
   TColor color;
   int penStyle;
   TSymbol *symbol;
};

typedef TIArrayAsVector<TDiagramDisplayObject> TIDiagramDisplayObjectContainer;
typedef TIArrayAsVectorIterator<TDiagramDisplayObject> TIDiagramDisplayObjectIterator;

#endif // DIAGDOBJ_H_
