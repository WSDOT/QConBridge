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

#include <qconbrdg\diagdobj.h>
#include <qconbrdg\bdomgr.h>
#include <qconbrdg\bdoc.h>
#include <qconbrdg\domgr.h>
#include <float.h>

TDiagramDisplayObject::TDiagramDisplayObject(TBridgeDisplayObjectManager *doMgr,DiagramCatagory cat) :
TDisplayObject(doMgr,NULL)
{
   pointContainers = new PointContainerContainer(10,0,10);
   diagrams = new DiagramContainer(10,0,10);
   color = TColor::Black;
   penStyle = PS_SOLID;
   symbol = NULL;
   dc = cat;
}

TDiagramDisplayObject::~TDiagramDisplayObject()
{
   diagrams->Flush(TShouldDelete::NoDelete);
   delete diagrams;
   delete pointContainers;
}

DiagramCatagory TDiagramDisplayObject::GetDiagramCatagory()
{
   return dc;
}

int TDiagramDisplayObject::ObjectType() const
{
   return otDiagram;
}

void TDiagramDisplayObject::AddDiagram(Diagram *diagram)
{
   diagrams->Add(diagram);
}

void TDiagramDisplayObject::SetColor(const TColor newColor)
{
   color = newColor;
}

void TDiagramDisplayObject::SetPenStyle(const int newPenStyle)
{
   penStyle = newPenStyle;
}

void TDiagramDisplayObject::SetSymbol(TSymbol *newSymbol)
{
   symbol = newSymbol;
}

void TDiagramDisplayObject::Draw(TDC &dc)
{
   TRACE("TDiagramDisplayObject::Draw()");

   int symbolSize = dc.GetDeviceCaps(LOGPIXELSX) / 16; // 1/16" symbols

   TPen diagramPen(color,1,penStyle);
   TPen symbolPen(color,1,PS_SOLID);
   TBrush symbolBrush(color);

   PointContainerIterator i(*pointContainers);
   while (i)
      {
      TPointContainer *points = i++;
      PointIterator j(*points->points);
      TPoint *p;

      p = j++;
      dc.MoveTo(*p);

      dc.SelectObject(symbolPen);
      dc.SelectObject(symbolBrush);
      if (symbol)
         symbol->Draw(dc,symbolSize,*p);

      while(j)
          {
          p = j++;
          dc.SelectObject(diagramPen);
          dc.LineTo(*p);

          dc.SelectObject(symbolPen);
          dc.SelectObject(symbolBrush);
          if (symbol)
             symbol->Draw(dc,symbolSize,*p);
          }
       }

   dc.RestorePen();
}

void TDiagramDisplayObject::Setup(TMapper &mapr)
{
   TRACE("TDiagramDisplayObject::Setup()");
   double minx,maxx;
   double miny,maxy;
   int i,n;
   double x,y; // World Point

#if defined (__FLAT__)
   long X,Y;    // Device Point
#else
   int X,Y;    // Device Point
#endif

   pointContainers->Flush(TShouldDelete::Delete);

   DiagramIterator j(*diagrams);
   while (j)
      {
      Diagram *diagram = j++;
      TPointContainer *points = new TPointContainer();
      pointContainers->Add(points);

      diagram->GetRange(minx,maxx,miny,maxy);
      n = diagram->NumPoints();
      for (i = 0; i < n; i++)
         {
         diagram->GetPoint(i,x,y);
         mapr.WPtoDP(x,y,X,Y);
         points->points->Add(new TPoint(X,Y));
         }
      }
}

void TDiagramDisplayObject::GetRange(double &minx,double &maxx,double &miny,double &maxy)
{
   double MinX,MinY,MaxX,MaxY;

   minx = DBL_MAX;
   miny = DBL_MAX;
   maxx = -DBL_MAX;
   maxy = -DBL_MAX;

   DiagramIterator j(*diagrams);
   while (j)
      {
      Diagram *diagram = j++;
      TPointContainer *points = new TPointContainer();
      pointContainers->Add(points);

      diagram->GetRange(MinX,MaxX,MinY,MaxY);
      minx = min(minx,MinX);
      maxx = max(maxx,MaxX);
      miny = min(miny,MinY);
      maxy = max(maxy,MaxY);
      }
}

