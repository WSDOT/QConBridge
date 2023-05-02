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

#if !defined(__rsltview_h)              // Sentry, use file only if it's not already included.
#define __rsltview_h


#include <qconbrdg\bview.h>
#include <qconbrdg\resdomgr.h>
#include <qconbrdg\viewev.h>

//{{TWindowView = TBridgeResultsView}}
class TBridgeResultsView : public TBridgeView
{
public:
   TBridgeResultsView (TBridgeDocument& doc,TWindow *parent);
   virtual ~TBridgeResultsView ();

   virtual void ConvertToDisplayUnits(double &value) = 0;
   virtual void Draw(TDC &dc,TRect &rect,bool bestFit = false);

protected:
   TResultsDisplayObjectManager *resultsDispObjMgr;

//{{TBridgeResultsViewVIRTUAL_BEGIN}}
public:
    virtual void SetupWindow();
//{{TBridgeResultsViewVIRTUAL_END}}

//{{TBridgeResultsViewRSP_TBL_BEGIN}}
protected:
    bool VnAnalyzed(bool analyzed);
    bool VnPlotStyleChanged (DiagramCatagory dc);
    bool VnDisplayGrid(bool display);
    void CmSelectLoading ();
    void EvRButtonDown (uint modKeys, TPoint& point);
//{{TBridgeResultsViewRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TBridgeResultsView);
};    //{{TBridgeResultsView}}


#endif                                      // __rsltview_h sentry.

