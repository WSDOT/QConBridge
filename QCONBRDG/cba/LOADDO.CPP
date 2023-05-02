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

#include <qconbrdg\loaddo.h>
#include <qconbrdg\qcbapp.h>
#include <qconbrdg\bdomgr.h>
#include <qconbrdg\bview.h>
#include <owl\docmanag.h>
#include <owltools\arrow.h>
#include <bridge\lrfdbrdg.h> // for DW_LOAD

TLoadDisplayObject::TLoadDisplayObject(TBridgeDisplayObjectManager *doMgr,BridgeLoad *Load) :
TDisplayObject(doMgr,Load)
{
   load = Load;
   SetColor(TColor::Gray);
}

TLoadDisplayObject::~TLoadDisplayObject()
{
    load = NULL;
}

void TLoadDisplayObject::Draw(TDC &dc)
{
   if (load->Type() == blConc)
      ConcLoadDraw(dc);
   else
      UnifLoadDraw(dc);
}

void TLoadDisplayObject::Setup(TMapper &mapr)
{
   if (load->Type() == blConc)
      ConcLoadSetup(mapr);
   else
      UnifLoadSetup(mapr);
}

int TLoadDisplayObject::ObjectType() const
{
   return otLoad;
}

BridgeLoad *TLoadDisplayObject::GetLoad() const
{
   return load;
}

void TLoadDisplayObject::SetLoad(BridgeLoad *newLoad)
{
   load = newLoad;
}


void TLoadDisplayObject::ConcLoadDraw(TDC &dc)
{
   ConcBridgeLoad *cload = TYPESAFE_DOWNCAST(load,ConcBridgeLoad);
   TArrowHead arrow(6,6);
   TColor color;

   color = GetDrawingColor();
   dc.SelectObject(TPen(color,GetDrawingPenSize()));
   dc.SelectObject(TBrush(color));

   dc.MoveTo(Top);
   dc.LineTo(Bottom);

   if (cload->Magnitude() > 0)
      arrow.Draw(Bottom,TArrowHead::arrowDown,dc);
   else
      arrow.Draw(Top,TArrowHead::arrowUp,dc);

   dc.RestorePen();
   dc.RestoreBrush();
}

void TLoadDisplayObject::ConcLoadSetup(TMapper &mapr)
{
   TFPoint p;
   Span *span;
   double loc;
   ConcBridgeLoad *cload = TYPESAFE_DOWNCAST(load,ConcBridgeLoad);

   span = cload->GetSpan();
   span->GetStartPoint(p.x,p.y);

   loc = cload->StartLocation();
   if (cload->IsFract())
      loc *= span->Length();

   p.x += loc;
   mapr.WPtoDP(p,Bottom);
   Bottom.y -= GetDrawingPenSize();
   Top.x = Bottom.x;
   Top.y = Bottom.y - 60;

   TRect rect(Top,Bottom);
   rect.Inflate(4,1);

   drawRegion = TRegion(rect);
}


void TLoadDisplayObject::UnifLoadDraw(TDC &dc)
{
   UnifBridgeLoad *uload = TYPESAFE_DOWNCAST(load,UnifBridgeLoad);
   TArrowHead arrow(6,6);
   TColor color;

   color = GetDrawingColor();
   dc.SelectObject(TPen(color,GetDrawingPenSize()));
   dc.SelectObject(TBrush(color));

   dc.MoveTo(UnifLoadRect.TopLeft());
   dc.LineTo(UnifLoadRect.TopRight());

   double width, space, dist;
   width = UnifLoadRect.Width();
   space = width/10;
   if (space < 12)
      {
      // no closer than 2 arrowheads
      int num = int(width/12.) + 1;
      space = width/num;
      }
   dist = 0;

   while (dist <= width)
      {
      TPoint point(UnifLoadRect.left + dist,UnifLoadRect.top);
      dc.MoveTo(point);
      dc.LineTo(point + TPoint(0,UnifLoadRect.Height()));

      if (uload->Magnitude() > 0)
         arrow.Draw(point+TPoint(0,UnifLoadRect.Height()),TArrowHead::arrowDown,dc);
      else
         arrow.Draw(point,TArrowHead::arrowUp,dc);

      dist += space;
      }

   dc.RestorePen();
   dc.RestoreBrush();
}

void TLoadDisplayObject::UnifLoadSetup(TMapper &mapr)
{
   UnifBridgeLoad *uload = TYPESAFE_DOWNCAST(load,UnifBridgeLoad);
   TFPoint p;
   TFPoint startPnt,endPnt;
   Span *span;
   double start,end;
   TPoint topLeft,botRight;
   TRect Rect;

   span = uload->GetSpan();
   span->GetStartPoint(p.x,p.y);

   start = uload->StartLocation();
   if (uload->IsFract())
      start *= span->Length();

   end = uload->EndLocation();
   if (uload->IsFract())
      end *= span->Length();

   startPnt = p;
   startPnt.x += start;
   mapr.WPtoDP(startPnt,topLeft);

   endPnt = p;
   endPnt.x += end;
   mapr.WPtoDP(endPnt,botRight);

   botRight.y -= GetDrawingPenSize();
   topLeft.y   = botRight.y - 30;
   UnifLoadRect = TRect(topLeft,botRight);

   Rect = UnifLoadRect;
   Rect.Inflate(4,GetDrawingPenSize());

   drawRegion = TRegion(Rect);
}

bool TLoadDisplayObject::operator == (const TLoadDisplayObject &ldo) const
{
   return (*load == *ldo.GetLoad());
}

//
//
//

TSelLoadDisplayObject::TSelLoadDisplayObject(TBridgeDisplayObjectManager *doMgr,BridgeLoad *Load,bool selected) :
TLoadDisplayObject(doMgr,Load),
TSelectableObject(selected)
{
}

TSelLoadDisplayObject::~TSelLoadDisplayObject()
{
}

TColor TSelLoadDisplayObject::GetDrawingColor() const
{
   DiagramCatagory dc = (load->GetOwner()->GetID() == DW_LOAD) ? dcDW : dcDC;
   TColor color;
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(doMgr->bview->bridgeDoc.GetDocManager().GetApplication(),
                                              TQConBridgeApp);

   if (IsSelected() && !theApp->Printing)
      color = GetSelColor();
   else
      color = doMgr->bview->bridgeDoc.GetPlotColor(dc);

   return color;
}

void TSelLoadDisplayObject::ConcLoadSetup(TMapper &mapr)
{
   TLoadDisplayObject::ConcLoadSetup(mapr);
   pickRegion = drawRegion;
}

void TSelLoadDisplayObject::UnifLoadSetup(TMapper &mapr)
{
   TLoadDisplayObject::UnifLoadSetup(mapr);
   pickRegion = drawRegion;
}

