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

#if !defined(__bview_h)              // Sentry, use file only if it's not already included.
#define __bview_h


#include <qconbrdg\qcbview.h>
#include <qconbrdg\bdomgr.h>
#include <owl/printer.h>

//{{TWindowView = TBridgeView}}
class TBridgeView : public TQConBridgeView, public TWindow
{
public:
    TBridgeView (TBridgeDocument& doc,TWindow *parent = 0);
    virtual ~TBridgeView ();
    TBridgeDisplayObjectManager *GetDisplayObjectManager();

    bool VnIsWindow(HWND hWnd);

    TPrintout *GetPrintout();

protected:
    TBridgeDisplayObjectManager *bridgeDispObjMgr;

//{{TBridgeViewVIRTUAL_BEGIN}}
public:
    virtual void SetupWindow();
    virtual TWindow* GetWindow ();
    virtual bool SetDocTitle (const char far* docname, int index);
    virtual void Paint (TDC& dc, bool erase, TRect& rect);
//{{TBridgeViewVIRTUAL_END}}

    virtual void Draw(TDC &dc,TRect &rect,bool bestFit = false);
    void Clear();

protected:
   virtual void CreateDisplayObjects();
   virtual TSpanDisplayObject *CreateSpanDisplayObject(Span *span);
   virtual TSupportDisplayObject *CreateSupportDisplayObject(Support *support);
   virtual TLoadDisplayObject *CreateLoadDisplayObject(BridgeLoad *load);

//{{TBridgeViewRSP_TBL_BEGIN}}
protected:
    bool VnDisplayDimensions(bool display);
    bool VnDisplayLabels(bool display);
    bool VnDisplaySegmentTicks(bool display);
    bool VnAnalyzed(bool analyzed);
    bool VnRebuildDisplay(void);
    bool VnClearAll(void);
    bool VnRemoveSpan(long lParam);
    bool VnReplaceSpan(long lParam);
    bool VnRemoveSupport(long lParam);
    bool VnReplaceSupport(long lParam);
    bool VnRemoveLoad(long lParam);
    bool VnAddLoad(long lParam);
    bool VnAddSpan(long lParam);
    bool VnAddSupport(long lParam);
    bool VnAddSpanSupport(long lParam);
    bool VnUnitsChanged(void);
    void EvSize (uint sizeType, TSize& size);
    void EvDisplayChange(bool, int, int);
    void CmDisplayOptions ();
    void CmExport ();
//{{TBridgeViewRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TBridgeView);
};    //{{TBridgeView}}


#endif                                      // __bview_h sentry.

