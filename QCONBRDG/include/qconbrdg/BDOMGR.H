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

#if !defined BDOMGR_H
#define BDOMGR_H

#include <qconbrdg\domgr.h>
#include <qconbrdg\spando.h>
#include <qconbrdg\suprtdo.h>
#include <qconbrdg\loaddo.h>
#include <qconbrdg\legncell.h>

class TBridgeView;

class TBridgeDisplayObjectManager : public TDisplayObjectManager
{
public:
   TBridgeDisplayObjectManager(TBridgeView *view);
   virtual ~TBridgeDisplayObjectManager();

   void AddSpanDisplayObject(TSpanDisplayObject *sdo);
   void AddSupportDisplayObject(TSupportDisplayObject *sdo);
   void AddLoadDisplayObject(TLoadDisplayObject *ldo);

   void SetDimensionFont(const char *typeFace,int pointSize);
   TFont &GetDimensionFont(TDC &dc);
   void SetDimensionColor(TColor color);
   TColor GetDimensionColor() const;
   void SetDimensionPenSize(int size);
   int GetDimensionPenSize() const;

   void DisplayDimensions(bool display);
   bool DisplayDimensions() const;

   void DisplayLabels(bool display);
   bool DisplayLabels() const;

   void DisplaySegmentTicks(bool display);
   bool DisplaySegmentTicks() const;

   virtual TFont &GetDimensionDrawingFont(TDC &dc);
   virtual TColor GetDimensionDrawingColor() const;
   virtual int GetDimensionDrawingPenSize() const;

   void Draw(TDC &dc,TRect &rect,bool bestFit);

   void Clear();

   virtual void RemoveSpan(Span *span);
   virtual void RemoveSupport(Support *support);
   virtual void RemoveLoad(BridgeLoad *load);
   void ReplaceSpan(Span *oldSpan,Span *newSpan);
   void ReplaceSupport(Support *oldSupport,Support *newSupport);
   void UpdateDisplay();
   void EnableUpdate(bool enable);
   virtual void UpdateLegend();


   TBridgeView *bview;

protected:
   virtual void SetupDisplay(TDC &dc,TRect &rect,bool bestFit);
   virtual void SetupLegend();
   void SetupDisplayObjects();

   void SetupDrawingRegions(TDC &dc,TRect &rect);
   virtual void SetupAxisRegion(TDC &dc,TRect &rect);
   virtual void SetupBridgeRegion(TDC &dc,TRect &rect);
   virtual void SetupTitleRegion(TDC &dc,TRect &rect);
   void SetupMapper(TDC &dc,bool bestFit);
   virtual void SetupScales(TMapper &mapr);

protected:
   TMapper bridgeMapper;

   bool updateDisplay;
   bool enableUpdate;
   bool drawDimensions;
   string dimFontTypeFace;
   int dimFontPointSize;
   TFont *dimFont;
   TColor dimColor;
   int penSize;
   bool displayLabels;
   bool displaySegmentTicks;

   TISSpanDisplayObjectContainer   *spanDisplayObjects;
   TISupportDisplayObjectContainer *supportDisplayObjects;
   TILoadDisplayObjectContainer    *loadDisplayObjects;

   void DrawTitle(TDC &dc);
   virtual void DrawAxis(TDC &dc);
   virtual void DrawLegend(TDC &dc);
   void DrawSpans(TDC &dc);
   void DrawSupports(TDC &dc);
   void DrawLoads(TDC &dc);
   void DrawDimensions(TDC &dc);
   virtual void GetScaleString(string &scale);

   TSpanDisplayObject *GetSpanDisplayObject(Span *span);
   TSupportDisplayObject *GetSupportDisplayObject(Support *support);
   TLoadDisplayObject *GetLoadDisplayObject(BridgeLoad *load);

   TRect titleRect;
   TRect axisRect;
   TRect bridgeRect;

   TLegend *legend;
};

#endif