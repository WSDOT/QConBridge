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

#include <qconbrdg\suprtdo.h>
#include <qconbrdg\qcbapp.h>
#include <qconbrdg\bdomgr.h>
#include <qconbrdg\bview.h>
#include <owl\docmanag.h>

#define SUPPORT_SIZE 12

TSupportDisplayObject::TSupportDisplayObject(TBridgeDisplayObjectManager *doMgr,Support *aSupport) :
TDisplayObject(doMgr,aSupport),
TBridgeComponentDisplayObject()
{
   support = aSupport;
   numSegments=0;
   segments = NULL;
   SetColor(TColor::Gray);
   symbolColor = TColor::Gray;
}

TSupportDisplayObject::~TSupportDisplayObject()
{
   delete[] segments;
}

void TSupportDisplayObject::Setup(TMapper &mapr)
{
   const int tol = 15;
   TFPoint start,end;
   int seg;

   // Map support end points into device coordinates
   support->GetTopPoint(start.x,start.y);
   support->GetBottomPoint(end.x,end.y);
   mapr.WPtoDP(start,Start);
   mapr.WPtoDP(end,End);

   // If the support is a pier, there is more work to do.
   if (support->IsPier())
      {
      // Map segments into device coordinates
      numSegments = support->NumSegments();
      segments = new TPoint[numSegments];
      for (seg = 0; seg < numSegments; seg++)
         {
         double X,Y;
         X = (support->GetSegment(seg)->GetLocation() - support->Height() - end.y)*(start.x-end.x)/(start.y-end.y) + end.x;
         Y =  support->GetSegment(seg)->GetLocation() + end.y;
         TFPoint segmentPnt(X,Y);
         mapr.WPtoDP(segmentPnt,segments[seg]);
         }
      }
   else
      numSegments = 0;

   // Setup regions for drawing
   TRect rect(Start.x,Start.y,End.x,End.y);
   rect.Normalize();
   rect.Inflate(tol,0);
   rect.top -= tol;
   rect.bottom += tol;
   drawRegion = TRegion(rect);
}

int TSupportDisplayObject::ObjectType() const
{
   return otSupport;
}

void TSupportDisplayObject::Draw(TDC &dc)
{
   TColor color;
   int seg;
   int tickSize;
   int hingeSize;
   int supportSize;

   color = GetDrawingColor();
   dc.SelectObject(TPen(color,GetDrawingPenSize()));
   dc.SelectObject(TBrush(color));

   tickSize = dc.GetDeviceCaps(LOGPIXELSX)/8;
   hingeSize = dc.GetDeviceCaps(LOGPIXELSX)/16;
   supportSize = dc.GetDeviceCaps(LOGPIXELSX)/SUPPORT_SIZE;

   // If the support is a pier, then there is something to draw
   if (support->IsPier())
      {
      // Draw support
      dc.MoveTo(Start);
      dc.LineTo(End);

      // Draw support top fixity
      dc.SelectObject(TPen(color,1));
      dc.SelectObject(TBrush(TColor(::GetSysColor(COLOR_WINDOW))));
      if (support->TopContinuity() == ctHinged)
         dc.Ellipse(TRect(TPoint(Start.x-hingeSize/2,Start.y),TSize(hingeSize,hingeSize)));

      // Draw segment ticks
      if (DisplaySegmentTicks())
         {
         for (seg = 0; seg < numSegments; seg++)
            {
            dc.MoveTo(segments[seg].x+tickSize/2,segments[seg].y);
            dc.LineTo(segments[seg].x-tickSize/2,segments[seg].y);
            }
         }
      }

   color = GetSymbolDrawingColor();
   dc.SelectObject(TPen(color,1));
   dc.SelectObject(TBrush(color));

   // Draw the support symbol
   switch(support->SupportFixity())
      {
      case fxRoller:
           DrawRoller(dc);
           break;

      case fxPinned:
           DrawPinned(dc);
           break;

      case fxFixed:
           DrawFixed(dc,support->IsPier());
           break;
      }

   if (DisplayLabels())
      {
      char label[5];
		TColor oldColor;
		int bkMode;
		dc.SetTextAlign(TA_CENTER);
		dc.SelectObject(GetLabelDrawingFont());
		oldColor = dc.SetTextColor(GetLabelDrawingColor());
      bkMode = dc.SetBkMode(TRANSPARENT);
		ostrstream(label,sizeof(label)) << support->GetID() << ends;
		dc.TextOut(End.x,End.y+supportSize+1,label);
		dc.RestoreFont();
		dc.SetTextColor(oldColor);
      dc.SetBkMode(bkMode);
		}

   dc.RestorePen();
   dc.RestoreBrush();
}

void TSupportDisplayObject::DrawRoller(TDC &dc)
{
   TPoint c(End);
   int supportSize;

   supportSize = dc.GetDeviceCaps(LOGPIXELSX)/SUPPORT_SIZE;

   c.y += supportSize/2;

   TRect rect(c,c);
   rect.Inflate(supportSize/2,supportSize/2);
   dc.Ellipse(rect);
}

void TSupportDisplayObject::DrawPinned(TDC &dc)
{
   TPoint pnt[3];
   int supportSize;

   supportSize = dc.GetDeviceCaps(LOGPIXELSX)/SUPPORT_SIZE;

   pnt[0] = End;
   pnt[1].x = End.x + supportSize;
   pnt[1].y = End.y + supportSize;
   pnt[2].x = End.x - supportSize;
   pnt[2].y = End.y + supportSize;

   dc.Polygon(pnt,3);
}

void TSupportDisplayObject::DrawFixed(TDC &dc,bool isPier)
{
   TRect rect;
   int supportSize;

   supportSize = dc.GetDeviceCaps(LOGPIXELSX)/SUPPORT_SIZE;

   rect.top = End.y;
   rect.left = End.x - supportSize/2;
   rect.bottom = End.y+supportSize/2;
   rect.right = rect.left + supportSize;

   if (!isPier)
      {
      // if the support is not a pier, draw the fixed symbol vertically
      TPoint center(rect.top + rect.Height()/2,rect.left + rect.Width()/2);
      int height = rect.Width();
      int width = rect.Height();
      TPoint topLeft(center.x - width/2,center.y - height/2);
      rect = TRect(topLeft,TSize(width,height));
      }

   dc.Rectangle(rect);
}

void TSupportDisplayObject::SetSupport(Support *newSupport)
{
   support = newSupport;
}

Support *TSupportDisplayObject::GetSupport() const
{
   return support;
}

void TSupportDisplayObject::SetSymbolColor(TColor newColor)
{
   symbolColor = newColor;
}

TColor TSupportDisplayObject::GetSymbolColor() const
{
   return symbolColor;
}

TColor TSupportDisplayObject::GetSymbolDrawingColor() const
{
   TColor color;
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(doMgr->bview->bridgeDoc.GetDocManager().GetApplication(),
                                              TQConBridgeApp);
   if(theApp->Printing)
      color = TColor::Black;
   else
      color = GetSymbolColor();

   return color;
}

TColor TSupportDisplayObject::GetLabelDrawingColor() const
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

bool TSupportDisplayObject::operator == (const TSupportDisplayObject &sdo) const
{
   return (support->GetID() == sdo.GetSupport()->GetID());
}

bool TSupportDisplayObject::operator <  (const TSupportDisplayObject &sdo) const
{
   return (support->GetID() < sdo.GetSupport()->GetID());
}

//
//
//

TSelSupportDisplayObject::TSelSupportDisplayObject(TBridgeDisplayObjectManager *doMgr,Support *support,bool selected) :
TSupportDisplayObject(doMgr,support),
TSelectableObject(selected)
{
   SetColor(TColor::Black);
   SetSymbolColor(TColor::LtBlue);
   SetLabelColor(TColor::Black);
}

TSelSupportDisplayObject::~TSelSupportDisplayObject()
{
}

TColor TSelSupportDisplayObject::GetDrawingColor() const
{
   TColor color;

   if (IsSelected())
      color = GetSelColor();
   else
      color = GetColor();

   return color;
}

TColor TSelSupportDisplayObject::GetSymbolDrawingColor() const
{
   if (IsSelected())
      return GetSelColor();
   else
      return GetSymbolColor();
}

void TSelSupportDisplayObject::Setup(TMapper &mapr)
{
   TSupportDisplayObject::Setup(mapr);
   pickRegion = TRegion(drawRegion);
}

#if defined GEN_DEBUG_CODE
void TSelSupportDisplayObject::Draw(TDC &dc)
{
   TSupportDisplayObject::Draw(dc);

   TRect rect = pickRegion.GetRgnBox();

   dc.SelectStockObject(NULL_BRUSH);
   dc.SelectObject(TPen(TColor::LtGray));

   dc.Rectangle(rect);

   dc.RestorePen();
   dc.RestoreBrush();
}
#endif

