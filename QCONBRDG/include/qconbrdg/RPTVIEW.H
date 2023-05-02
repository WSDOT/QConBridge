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

#if !defined(__rptview_h)              // Sentry, use file only if it's not already included.
#define __rptview_h


#include <qconbrdg\rptwin.h>
#include <qconbrdg\qcbview.h>


//{{TWindowView = TReportView}}
class TReportView : public TQConBridgeView, public TReportWindow
{
public:
    TReportView (TBridgeDocument& doc, TWindow* parent = 0);
    virtual ~TReportView ();

    static const char far* StaticName() { return "Report"; }
    const char far* GetViewName ();

    TResId GetIconResourceId() const;

    TWindow* GetWindow ();
    bool VnIsWindow(HWND hWnd);
    bool SetDocTitle (const char far* /*docname*/, int /*index*/);

    TPrintout *GetPrintout();

    void EvRButtonDown (uint /*modKeys*/, TPoint& point);
    bool VnAnalyzed(bool analyzed);
    bool VnUnitsChanged(void);
    void CmExportReport();
    void CeExportReport(TCommandEnabler &tce);

DECLARE_RESPONSE_TABLE(TReportView);
};    //{{TReportView}}


#endif                                      // __rptview_h sentry.

