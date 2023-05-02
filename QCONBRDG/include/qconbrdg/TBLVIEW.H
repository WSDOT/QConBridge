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

#if !defined(__tblview_h)              // Sentry, use file only if it's not already included.
#define __tblview_h


#include <owl\docview.h>
#include <owl\window.h>
#include <owl\printer.h>
#include <qconbrdg\qcbview.h>
#include <qconbrdg\tblwin.h>
#include <qconbrdg\qcbapp.rh>            // Definition of all resources.


//{{TWindowView = TTableView}}
class TTableView : public TQConBridgeView, public TTableWindow
{
public:
    TTableView (TBridgeDocument& doc, TWindow* parent = 0);
    virtual ~TTableView ();

    bool VnIsWindow(HWND hWnd);

    virtual bool CanPrint();

    static const char far* StaticName() { return "Tabular Results"; }
    TResId GetIconResourceId() const;
    TPrintout *GetPrintout();

protected:
    TTable *selectedTable;
    DiagramCatagory dc;
    bool tableSelected;

//{{TTableViewVIRTUAL_BEGIN}}
protected:
    virtual const char far* GetViewName ();
    virtual bool SetDocTitle (const char far* docname, int index);
    virtual void SetupWindow ();
    virtual TWindow* GetWindow ();
//{{TTableViewVIRTUAL_END}}

//{{TTableViewRSP_TBL_BEGIN}}
protected:
    bool VnAnalyzed(bool analyzed);
    bool VnUnitsChanged(void);
    void EvRButtonDown (uint modKeys, TPoint& point);
    void CmSelectTable ();
    void CmExportTable ();
    void CeExportTable(TCommandEnabler &tce);
//{{TTableViewRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TTableView);
};    //{{TTableView}}


#endif                                      // __tblview_h sentry.

