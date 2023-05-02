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

#include <qconbrdg\bdomgr.h>
#include <qconbrdg\bview.h>
#include <qconbrdg\qcbapp.h>
#include <owltools\fonthelp.h>
#include <owltools\arrow.h>
#include <owl\docmanag.h>
#include <strstrea.h>
#include <iomanip.h>

#include <checks.h>

#define SUPPORT_SIZE 12

TBridgeDisplayObjectManager::TBridgeDisplayObjectManager(TBridgeView *view) :
TDisplayObjectManager(view)
{
   bview = view;
   updateDisplay = true;
   enableUpdate = true;

   dimFontTypeFace = "Arial";
   dimFontPointSize = -8;
   dimFont = NULL;
   dimColor = TColor::Gray;
   penSize = 1;

   DisplayDimensions(bview->bridgeDoc.DisplayDimensions());
   DisplayLabels(bview->bridgeDoc.DisplayLabels());
   DisplaySegmentTicks(bview->bridgeDoc.DisplaySegmentTicks());

   spanDisplayObjects = new TISSpanDisplayObjectContainer(10,0,10);
   supportDisplayObjects = new TISupportDisplayObjectContainer(10,0,10);
   loadDisplayObjects  = new TILoadDisplayObjectContainer(10,0,10);;

   legend = NULL;
}

TBridgeDisplayObjectManager::~TBridgeDisplayObjectManager()
{
   delete dimFont;

   delete spanDisplayObjects;
   delete supportDisplayObjects;
   delete loadDisplayObjects;

   delete legend;
}


void TBridgeDisplayObjectManager::AddSpanDisplayObject(TSpanDisplayObject *sdo)
{
   spanDisplayObjects->Add(sdo);
}

void TBridgeDisplayObjectManager::AddSupportDisplayObject(TSupportDisplayObject *sdo)
{
   supportDisplayObjects->Add(sdo);
}

void TBridgeDisplayObjectManager::AddLoadDisplayObject(TLoadDisplayObject *ldo)
{
   loadDisplayObjects->Add(ldo);
}

void TBridgeDisplayObjectManager::SetDimensionFont(const char *typeFace,int pointSize)
{
   dimFontTypeFace = typeFace;
   dimFontPointSize = pointSize;
}

TFont &TBridgeDisplayObjectManager::GetDimensionFont(TDC &dc)
{
   delete dimFont;
   dimFont = CreateSimpleFont(dc,dimFontTypeFace.c_str(),dimFontPointSize);
   return *dimFont;
}

void TBridgeDisplayObjectManager::SetDimensionColor(TColor color)
{
   dimColor = color;
}

TColor TBridgeDisplayObjectManager::GetDimensionColor() const
{
   return dimColor;
}

void TBridgeDisplayObjectManager::SetDimensionPenSize(int size)
{
   penSize = size;
}

int TBridgeDisplayObjectManager::GetDimensionPenSize() const
{
   return penSize;
}

TFont &TBridgeDisplayObjectManager::GetDimensionDrawingFont(TDC &dc)
{
   return GetDimensionFont(dc);
}

TColor TBridgeDisplayObjectManager::GetDimensionDrawingColor() const
{
   TColor color;
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(bview->bridgeDoc.GetDocManager().GetApplication(),
                                              TQConBridgeApp);
   if(theApp->Printing)
      color = TColor::Black;
   else
      color = GetDimensionColor();

   return color;
}

int TBridgeDisplayObjectManager::GetDimensionDrawingPenSize() const
{
   return GetDimensionPenSize();
}

void TBridgeDisplayObjectManager::SetupDrawingRegions(TDC &dc,TRect &rect)
{
   SetupAxisRegion(dc,rect);
   SetupTitleRegion(dc,rect);
   SetupBridgeRegion(dc,rect);
}

void TBridgeDisplayObjectManager::SetupTitleRegion(TDC &dc,TRect &rect)
{
   TEXTMETRIC tm;
   int cyChar;

   dc.GetTextMetrics(tm);
   cyChar = tm.tmHeight + tm.tmExternalLeading;

   titleRect  = TRect(axisRect.TopRight(),TPoint(rect.Width(),rect.top+5*cyChar/2));
}

void TBridgeDisplayObjectManager::SetupBridgeRegion(TDC &/*dc*/,TRect &rect)
{
   bridgeRect = TRect(titleRect.BottomLeft(),rect.BottomRight());

   // Use only 98% of bridgeRect
   int dw = 0.01*(double)bridgeRect.Width();
   int dh = 0.01*(double)bridgeRect.Height();
   bridgeRect.Inflate(-dw,-dh);
}

void TBridgeDisplayObjectManager::SetupAxisRegion(TDC &/*dc*/,TRect &rect)
{
   axisRect = TRect(rect.TopLeft(),rect.BottomLeft());
}

void TBridgeDisplayObjectManager::SetupLegend()
{
}

void TBridgeDisplayObjectManager::SetupDisplay(TDC &dc,TRect &rect,bool bestFit)
{
   SetupDrawingRegions(dc,rect);
   SetupMapper(dc,bestFit);
   SetupDisplayObjects();
   SetupLegend();
}

void TBridgeDisplayObjectManager::Draw(TDC &dc,TRect &rect,bool bestFit)
{
   if (updateDisplay)
      {
      SetupDisplay(dc,rect,bestFit);
      updateDisplay = false;
      }

   DrawTitle(dc);
   DrawAxis(dc);
   DrawLegend(dc);
   DrawSpans(dc);
   DrawSupports(dc);
   DrawLoads(dc);

   if (DisplayDimensions())
      DrawDimensions(dc);
}

void TBridgeDisplayObjectManager::DrawTitle(TDC &dc)
{
   TFont *titleFont;
   TFont *scaleFont;
   TEXTMETRIC tm;
   string title;
   string scale;
   int cyChar;
   int x,y;
   int bkMode;
   int textAlign;

   titleFont = CreateSimpleFont(dc,"Arial",-16,FW_BOLD);
   scaleFont = CreateSimpleFont(dc,"Courier New",-8);

   dc.SelectObject(*titleFont);

   bkMode = dc.SetBkMode(TRANSPARENT);
   textAlign = dc.SetTextAlign(TA_TOP | TA_CENTER);

   dc.GetTextMetrics(tm);
   cyChar = tm.tmHeight + tm.tmExternalLeading;

   title = bview->GetViewName();
   x = titleRect.left + titleRect.Width()/2;
   y = titleRect.top;
   dc.TextOut(x,y,title.c_str());

   dc.RestoreFont();

   dc.SelectObject(*scaleFont);

   GetScaleString(scale);
   y += cyChar;
   dc.TextOut(x,y,scale.c_str());

   dc.RestoreFont();
   dc.SetBkMode(bkMode);
   dc.SetTextAlign(textAlign);

   delete titleFont;
   delete scaleFont;
}

void TBridgeDisplayObjectManager::GetScaleString(string &scale)
{
   ostrstream os;
   double scaleX;
   double scaleY;

   scaleX = bridgeMapper.GetScaleX();
   scaleY = bridgeMapper.GetScaleY();

   if (bview->bridgeDoc.GetUnits() == unitsSI)
      {
      os << setiosflags(ios::showpoint) << setprecision(2)
         << "HScale: " << scaleX << " m/mm - "
         << "VScale: " << scaleY << " m/mm" << ends;
      }
   else
      {
      // internal units in m/mm, convert to feet/inch
      scaleX *= 1000./12.;
      scaleY *= 1000./12.;
      os << setiosflags(ios::showpoint) << setprecision(2)
         << "HScale: " << scaleX << " ft/inch - "
         << "VScale: " << scaleY << " ft/inch" << ends;
      }

   scale = os.str();
   delete[] os.str();
}

void TBridgeDisplayObjectManager::DrawAxis(TDC &/*dc*/)
{
}

void TBridgeDisplayObjectManager::DrawLegend(TDC &dc)
{
   if (legend)
      legend->Draw(dc,bridgeRect);
}

void TBridgeDisplayObjectManager::SetupDisplayObjects()
{
   //
   // Setup spans
   //
   TISSpanDisplayObjectIterator i(*spanDisplayObjects);
   while (i)
      {
      TSpanDisplayObject *dispObj = i++;
      dispObj->Setup(bridgeMapper);
      }

   //
   // Setup supports
   //
   TISupportDisplayObjectIterator j(*supportDisplayObjects);
   while (j)
      {
      TSupportDisplayObject *dispObj = j++;
      dispObj->Setup(bridgeMapper);
      }

   //
   // Setup loads
   //
   TILoadDisplayObjectIterator k(*loadDisplayObjects);
   while (k)
      {
      TLoadDisplayObject *dispObj = k++;
      dispObj->Setup(bridgeMapper);
      }
}

void TBridgeDisplayObjectManager::DrawSpans(TDC &dc)
{
   TISSpanDisplayObjectIterator i(*spanDisplayObjects);

   while (i)
      {
      TSpanDisplayObject *dispObj = i++;

      dispObj->DisplayLabels(DisplayLabels());

      // The font choosen here is just a dummy font... A label font
      // must be added to this display object manager
      if (DisplayLabels())
         dispObj->SetLabelFont(GetDimensionDrawingFont(dc));

      dispObj->DisplaySegmentTicks(DisplaySegmentTicks());

      dispObj->Draw(dc);
      }
}

void TBridgeDisplayObjectManager::DrawSupports(TDC &dc)
{
   TISupportDisplayObjectIterator i(*supportDisplayObjects);

   while (i)
      {
      TSupportDisplayObject *dispObj = i++;

      dispObj->DisplayLabels(DisplayLabels());

      // The font choosen here is just a dummy font... A label font
      // must be added to this display object manager
      if (DisplayLabels())
         dispObj->SetLabelFont(GetDimensionDrawingFont(dc));

      dispObj->DisplaySegmentTicks(DisplaySegmentTicks());

      dispObj->Draw(dc);
      }
}

void TBridgeDisplayObjectManager::DrawLoads(TDC &dc)
{
   TILoadDisplayObjectIterator i(*loadDisplayObjects);

   while (i)
      {
      TLoadDisplayObject *dispObj = i++;
      dispObj->Draw(dc);
      }
}

void TBridgeDisplayObjectManager::DrawDimensions(TDC &dc)
{
   // If there aren't any spans, then there are no dimensions to draw
   if(spanDisplayObjects->GetItemsInContainer() == 0)
      return;

// If dimension lines and leader lines are treated as display object,
// they could be setup in advance... TRect &rect would not be needed
   Support *support;
   Span *span;
   int YdimLine;
   int Yleader;
   TEXTMETRIC tm;
   TArrowHead arrowHead;
   TColor oldColor;
   int bkMode;
   int logPixelsX;
   int logPixelsY;
   int supportSize;

   // use 1/8" arrowheads
   logPixelsX = dc.GetDeviceCaps(LOGPIXELSX); // dpi
   logPixelsY = dc.GetDeviceCaps(LOGPIXELSY); // dpi
   arrowHead.Resize(logPixelsX/8,logPixelsY/8);

   supportSize = dc.GetDeviceCaps(LOGPIXELSX)/SUPPORT_SIZE;

   dc.SelectObject(GetDimensionDrawingFont(dc));
   oldColor = dc.SetTextColor(GetDimensionDrawingColor());
   bkMode = dc.SetBkMode(TRANSPARENT);
   dc.SetTextAlign(TA_CENTER);
   dc.SelectObject(TPen(GetDimensionDrawingColor(),GetDimensionDrawingPenSize()));
   dc.GetTextMetrics(tm);

   // Determine height of the tallest support in device coordinates
   TFPoint pierHeight(0,-bview->bridgeDoc.GetBridge().MaxSupportHeight());
   TPoint height;
   bridgeMapper.WPtoDP(pierHeight,height);

   YdimLine = height.y + supportSize + 2*tm.tmHeight + 2;
   Yleader = YdimLine + logPixelsY/8;

   //
   // Draw Leader lines
   //

   // Special adjustment if first span is cantilevered.
   // Since the first support is at the second span, we need a leader
   // from the start of the first span
   if (bview->bridgeDoc.GetBridge().GetFirstSpan()->IsCantilevered())
      {
      TSpanDisplayObject *spanDispObj = GetSpanDisplayObject(bview->bridgeDoc.GetBridge().GetFirstSpan());
      dc.MoveTo(spanDispObj->Start+TPoint(0,supportSize + tm.tmHeight + 1));
      dc.LineTo(spanDispObj->Start.x,Yleader);
      }

   for (support = bview->bridgeDoc.GetBridge().GetFirstSupport(); support; support = support->NextSupport())
      {
      TSupportDisplayObject *supportDispObj = GetSupportDisplayObject(support);

      // Draw leader line
      if (supportDispObj->Start.x == supportDispObj->End.x)
         {
         // vertical pier
         dc.MoveTo(supportDispObj->End+TPoint(0,supportSize + tm.tmHeight + 1));
         dc.LineTo(supportDispObj->End.x,Yleader);
         }
      else
         {
         // sloped pier
         dc.MoveTo(supportDispObj->Start+TPoint(0,supportSize + tm.tmHeight + 1));
         dc.LineTo(supportDispObj->Start.x,Yleader);
         }
      }

   if (bview->bridgeDoc.GetBridge().GetLastSpan()->IsCantilevered())
      {
      TSpanDisplayObject *spanDispObj = GetSpanDisplayObject(bview->bridgeDoc.GetBridge().GetLastSpan());
      dc.MoveTo(spanDispObj->End+TPoint(0,supportSize + tm.tmHeight + 1));
      dc.LineTo(spanDispObj->End.x,Yleader);
      }

   // Draw dimension lines
   for (span = bview->bridgeDoc.GetBridge().GetFirstSpan(); span; span = span->NextSpan())
      {
      TSpanDisplayObject *spanDispObj = GetSpanDisplayObject(span);
      dc.MoveTo(spanDispObj->Start.x,YdimLine);
      dc.LineTo(spanDispObj->End.x,YdimLine);

      dc.SelectObject(TBrush(GetDimensionDrawingColor()));
      arrowHead.Draw(TPoint(spanDispObj->Start.x,YdimLine),TArrowHead::arrowLeft,dc);
      arrowHead.Draw(TPoint(spanDispObj->End.x,YdimLine),TArrowHead::arrowRight,dc);
      dc.RestoreBrush();

      char dim[30];
      FundamentalUnits::Length length(span->Length());
      if (bview->bridgeDoc.GetUnits() == unitsSI)
         ostrstream(dim,sizeof(dim)) << setiosflags(ios::fixed | ios::showpoint)
                                     << setprecision(3)
                                     << length.ConvertTo(SIConstants::meter).value() << " m" << ends;
      else
         ostrstream(dim,sizeof(dim)) << setiosflags(ios::fixed | ios::showpoint)
                                     << setprecision(3)
                                     << length.ConvertTo(USConstants::feet).value() << " ft" << ends;

      dc.TextOut((spanDispObj->Start.x + spanDispObj->End.x)/2,YdimLine - tm.tmHeight - 1,dim);
      }

   dc.SetTextColor(oldColor);
   dc.SetBkMode(bkMode);
   dc.RestorePen();
   dc.RestoreFont();
}

void TBridgeDisplayObjectManager::Clear()
{
   spanDisplayObjects->Flush();
   supportDisplayObjects->Flush();
   loadDisplayObjects->Flush();
}



void TBridgeDisplayObjectManager::DisplayDimensions(bool display)
{
   drawDimensions = display;
}

bool TBridgeDisplayObjectManager::DisplayDimensions() const
{
   return drawDimensions;
}

void TBridgeDisplayObjectManager::DisplayLabels(bool display)
{
   displayLabels = display;
}

bool TBridgeDisplayObjectManager::DisplayLabels() const
{
   return displayLabels;
}

void TBridgeDisplayObjectManager::DisplaySegmentTicks(bool display)
{
   displaySegmentTicks = display;
}

bool TBridgeDisplayObjectManager::DisplaySegmentTicks() const
{
   return displaySegmentTicks;
}


TSpanDisplayObject *TBridgeDisplayObjectManager::GetSpanDisplayObject(Span *span)
{
   TISSpanDisplayObjectIterator i(*spanDisplayObjects);

   while(i)
      {
      TSpanDisplayObject *dispObj = TYPESAFE_DOWNCAST(i++,TSpanDisplayObject);
      if (dispObj->GetSpan()->GetID() == span->GetID())
         return dispObj;
      }

   return NULL;
}

TSupportDisplayObject *TBridgeDisplayObjectManager::GetSupportDisplayObject(Support *support)
{
   TISupportDisplayObjectIterator i(*supportDisplayObjects);

   while(i)
      {
      TSupportDisplayObject *dispObj = TYPESAFE_DOWNCAST(i++,TSupportDisplayObject);
      if (dispObj->GetSupport()->GetID() == support->GetID())
         return dispObj;
      }

   return NULL;
}


TLoadDisplayObject *TBridgeDisplayObjectManager::GetLoadDisplayObject(BridgeLoad *load)
{
   TILoadDisplayObjectIterator i(*loadDisplayObjects);

   while(i)
      {
      TLoadDisplayObject *dispObj = TYPESAFE_DOWNCAST(i++,TLoadDisplayObject);
      if (dispObj->GetLoad() == load)
         return dispObj;
      }

   return NULL;
}

void TBridgeDisplayObjectManager::ReplaceSpan(Span *oldSpan,Span *newSpan)
{
   TSpanDisplayObject *dispObj;
   dispObj = GetSpanDisplayObject(oldSpan);

   if (dispObj == NULL)
      return;

   dispObj->SetSpan(newSpan);
}

void TBridgeDisplayObjectManager::ReplaceSupport(Support *oldSupport,Support *newSupport)
{
   TSupportDisplayObject *dispObj;
   dispObj = GetSupportDisplayObject(oldSupport);

   if (dispObj == NULL)
      return;

   dispObj->SetSupport(newSupport);
}

void TBridgeDisplayObjectManager::RemoveSpan(Span *span)
{
   int idx;
   TSpanDisplayObject *dispObj;

   // Get the display object
   dispObj = GetSpanDisplayObject(span);

   // If not found, get the heck outta here
   if (dispObj == NULL)
      return;

   // Get the display object's location in the container
   idx = spanDisplayObjects->Find(dispObj);

   // If not in the container, get the heck outta here
   if (idx == INT_MAX)
      return;

   // Remove the entry from the container (packs the container)
   spanDisplayObjects->RemoveEntry(idx);

   // Delete the object
   delete dispObj;
}

void TBridgeDisplayObjectManager::RemoveSupport(Support *support)
{
   int idx;
   TSupportDisplayObject *dispObj;

   // Get the display object
   dispObj = GetSupportDisplayObject(support);

   // If not found, get the heck outta here
   if (dispObj == NULL)
      return;

   // Get the display object's location in the container
   idx = supportDisplayObjects->Find(dispObj);

   // If not in the container, get the heck outta here
   if (idx == INT_MAX)
      return;

   // Remove the entry from the container (packs the container)
   supportDisplayObjects->RemoveEntry(idx);

   // Delete the object
   delete dispObj;
}

void TBridgeDisplayObjectManager::RemoveLoad(BridgeLoad *load)
{
   int idx;
   TLoadDisplayObject *dispObj;

   // Get the display object
   dispObj = GetLoadDisplayObject(load);

   // If not found, get the heck outta here
   if (dispObj == NULL)
      {
      #pragma warn -ccc
      WARN(1,"TBridgeDisplayObjectManager::RemoveLoad() - Display Object Not Found");
      #pragma warn .ccc
      return;
      }

   // Get the display object's location in the container
   idx = loadDisplayObjects->Find(dispObj);

   // If not in the container, get the heck outta here
   if (idx == INT_MAX)
      {
      #pragma warn -ccc
      WARN(1,"TBridgeDisplayObjectManager::RemoveLoad() - Display Object Not Found (INT_MAX)");
      #pragma warn .ccc
      return;
      }

   // Remove the entry from the container (packs the container)
   loadDisplayObjects->RemoveEntry(idx);

   // Delete the object
   delete dispObj;
}


void TBridgeDisplayObjectManager::SetupMapper(TDC &dc,bool bestFit)
{
   TBridgeDocument &bridgeDoc( *TYPESAFE_DOWNCAST((&(view->GetDocument())),TBridgeDocument) );
   LRFDBridge &bridge(bridgeDoc.GetBridge());

   SetupScales(bridgeMapper);

   bridgeMapper.SetLogPixelsX(dc.GetDeviceCaps(LOGPIXELSX));
   bridgeMapper.SetLogPixelsY(dc.GetDeviceCaps(LOGPIXELSY));

   bridgeMapper.SetDeviceExt(bridgeRect.Size());
   bridgeMapper.SetDeviceOrg(bridgeRect.left+bridgeRect.Width()/2,bridgeRect.top + bridgeRect.Height()/2);

   bridgeMapper.SetWorldExt(bridge.Length(),2*bridge.MaxSupportHeight());
   bridgeMapper.SetWorldOrg(bridge.Length()/2,0.0);

   if (bestFit)
      bridgeMapper.SetMapMode(mmIsotropic | mmBestFitX);
   else
      bridgeMapper.SetMapMode(mmIsotropic);
}

void TBridgeDisplayObjectManager::UpdateDisplay()
{
   if (enableUpdate)
      {
      updateDisplay = true;
      }
}

void TBridgeDisplayObjectManager::EnableUpdate(bool enable)
{
   enableUpdate = enable;
}

void TBridgeDisplayObjectManager::SetupScales(TMapper &mapr)
{
   mapr.ClearScaleX();
   mapr.ClearScaleY();
   //
   // Setup Best fit scales for bridge geometry
   //
   if (bview->bridgeDoc.GetUnits() == unitsSI)
      {
      mapr.AddScaleX(0.100);
      mapr.AddScaleX(0.125);
      mapr.AddScaleX(0.150);
      mapr.AddScaleX(0.200);
      mapr.AddScaleX(0.250);
      mapr.AddScaleX(0.500);

      mapr.AddScaleY(0.100);
      mapr.AddScaleY(0.125);
      mapr.AddScaleY(0.150);
      mapr.AddScaleY(0.200);
      mapr.AddScaleY(0.250);
      mapr.AddScaleY(0.500);
      }
   else
      {
      mapr.AddScaleX(0.1*0.012);
      mapr.AddScaleX(0.2*0.012);
      mapr.AddScaleX(0.3*0.012);
      mapr.AddScaleX(0.4*0.012);
      mapr.AddScaleX(0.5*0.012);
      mapr.AddScaleX(0.6*0.012);

      mapr.AddScaleY(0.1*0.012);
      mapr.AddScaleY(0.2*0.012);
      mapr.AddScaleY(0.3*0.012);
      mapr.AddScaleY(0.4*0.012);
      mapr.AddScaleY(0.5*0.012);
      mapr.AddScaleY(0.6*0.012);
      }
}

void TBridgeDisplayObjectManager::UpdateLegend()
{
}

