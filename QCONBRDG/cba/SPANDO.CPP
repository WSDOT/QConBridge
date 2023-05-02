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

#include <qconbrdg\spando.h>
#include <qconbrdg\qcbapp.h>
#include <qconbrdg\bdomgr.h>
#include <qconbrdg\bview.h>
#include <owl\docmanag.h>

TSpanDisplayObject::TSpanDisplayObject(TBridgeDisplayObjectManager *doMgr,Span *aSpan) :
TDisplayObject(doMgr,aSpan),
TBridgeComponentDisplayObject()
{
   span = aSpan;
   numHinges = 0;
   numSegments = 0;
   segments = NULL;
   SetColor(TColor::Gray);
}

TSpanDisplayObject::~TSpanDisplayObject()
{
   delete[] segments;
}

void TSpanDisplayObject::Setup(TMapper &mapr)
{
   const int tol = 20;
   TFPoint start,end;
   int hinge;
   int seg;

   // Map the span end points into device coordinates
   span->GetStartPoint(start.x,start.y);
   span->GetEndPoint(end.x,end.y);
   mapr.WPtoDP(start,Start);
   mapr.WPtoDP(end,End);

   // Map hinge locations into device coordinates
   numHinges = span->NumHinges();
   for (hinge = 0; hinge < numHinges; hinge++)
      {
      TFPoint HingePnt(span->GetHinge(hinge)->GetLocation()+start.x,0);
      mapr.WPtoDP(HingePnt,hingePnt[hinge]);
      }

   // Map segment locations into device coordinates
   numSegments = span->NumSegments();
   segments = new TPoint[numSegments];
   for (seg = 0; seg < numSegments; seg++)
      {
      TFPoint segmentPnt(span->GetSegment(seg)->GetLocation()+start.x,0);
      mapr.WPtoDP(segmentPnt,segments[seg]);
      }

   // Setup regions for drawing
   drawRegion = TRegion(TRect(Start.x,Start.y-tol,End.x,End.y+tol));
}

int TSpanDisplayObject::ObjectType() const
{
   return otSpan;
}

void TSpanDisplayObject::Draw(TDC &dc)
{
   TColor color;
   int hinge;
   int seg;
   int tickSize;
   int hingeSize;

   color = GetDrawingColor();
   dc.SelectObject(TPen(color,GetDrawingPenSize()));
   dc.SelectObject(TBrush(color));

   tickSize = dc.GetDeviceCaps(LOGPIXELSY)/8; // 1/8" ticks
   hingeSize = dc.GetDeviceCaps(LOGPIXELSY)/16; // 1/16" symbols

   // Draw the span
   dc.MoveTo(Start);
   dc.LineTo(End);

   // Draw span end boundary conditions
   dc.SelectObject(TPen(color,1));
   dc.SelectObject(TBrush(TColor(::GetSysColor(COLOR_WINDOW))));
   if (span->StartContinuity() == ctHinged)
      dc.Ellipse(TRect(TPoint(Start.x,Start.y-hingeSize/2),TSize(hingeSize,hingeSize)));

   if (span->EndContinuity() == ctHinged)
      dc.Ellipse(TRect(TPoint(End.x,End.y-hingeSize/2),TSize(-hingeSize,hingeSize)));

   // Draw inspan hinges
   for (hinge = 0; hinge < numHinges; hinge++)
      {
      dc.Ellipse(TRect(hingePnt[hinge],TSize(0,0)).Inflate(hingeSize/2,hingeSize/2));
      }

   // Draw segment ticks
   if (DisplaySegmentTicks())
      {
      for (seg = 0; seg < numSegments; seg++)
         {
         dc.MoveTo(segments[seg].x,segments[seg].y-tickSize/2);
         dc.LineTo(segments[seg].x,segments[seg].y+tickSize/2);
         }
      }

   // Draw labels
   if (DisplayLabels())
      {
      char label[5];
		TColor oldColor;
		int bkMode;

		dc.SetTextAlign(TA_CENTER);
		dc.SelectObject(GetLabelDrawingFont());
		oldColor = dc.SetTextColor(GetLabelDrawingColor());
		bkMode = dc.SetBkMode(TRANSPARENT);
		ostrstream(label,sizeof(label)) << span->GetID() << ends;
		TPoint textPoint;
		textPoint.x = (Start.x + End.x)/2;
		textPoint.y = End.y + min(hingeSize,tickSize)/2 + 2;
		dc.TextOut(textPoint,label);
		dc.RestoreFont();
		dc.SetTextColor(oldColor);
		dc.SetBkMode(bkMode);
		}

   dc.RestorePen();
   dc.RestoreBrush();
}

void TSpanDisplayObject::SetSpan(Span *newSpan)
{
   span = newSpan;
}

Span *TSpanDisplayObject::GetSpan() const
{
   return span;
}

TColor TSpanDisplayObject::GetLabelDrawingColor() const
{
   TColor color;
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(doMgr->bview->bridgeDoc.GetDocManager().GetApplication(),
                                              TQConBridgeApp);
   if(theApp->Printing)
      color = TColor::Black;
   else
      color = TBridgeComponentDisplayObject::GetLabelDrawingColor();

   return color;
}

bool TSpanDisplayObject::operator == (const TSpanDisplayObject &sdo) const
{
   return (span->GetID() == sdo.GetSpan()->GetID());
}

bool TSpanDisplayObject::operator <  (const TSpanDisplayObject &sdo) const
{
   return (span->GetID() < sdo.GetSpan()->GetID());
}

//
//
//

TSelSpanDisplayObject::TSelSpanDisplayObject(TBridgeDisplayObjectManager *doMgr,Span *span,bool selected) :
TSpanDisplayObject(doMgr,span),
TSelectableObject(selected)
{
   SetColor(TColor::Black);
   SetLabelColor(TColor::Black);
}

TSelSpanDisplayObject::~TSelSpanDisplayObject()
{
}

TColor TSelSpanDisplayObject::GetDrawingColor() const
{
   TColor color;

   if (IsSelected())
      color = GetSelColor();
   else
      color = GetColor();

   return color;
}

void TSelSpanDisplayObject::Setup(TMapper &mapr)
{
   const int tol = 15;

   TSpanDisplayObject::Setup(mapr);

   TRect pickRect(Start.x,Start.y-tol,End.x,End.y+tol);
   if (span->LeftSupport())
      pickRect.left += tol;
   if (span->RightSupport())
      pickRect.right -= tol;

   pickRegion = TRegion(pickRect);
}

#if defined GEN_DEBUG_CODE
void TSelSpanDisplayObject::Draw(TDC &dc)
{
   TSpanDisplayObject::Draw(dc);

   TRect rect = pickRegion.GetRgnBox();

   dc.SelectStockObject(NULL_BRUSH);
   dc.SelectObject(TPen(TColor::LtGray));

   dc.Rectangle(rect);

   dc.RestorePen();
   dc.RestoreBrush();
}
#endif

