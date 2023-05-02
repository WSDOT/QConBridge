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

#if !defined(__tblwin_h)              // Sentry, use file only if it's not already included.
#define __tblwin_h


#include <owl\window.h>
#include <owl\layoutwi.h>
#include <qconbrdg\hdrcli.h>
#include <qconbrdg\tblcli.h>
#include <qconbrdg\table.h>


//{{TLayoutWindow = TTableWindow}}
class TTableWindow : public TLayoutWindow {
public:
    TTableWindow (TWindow* parent,TTable *Table = NULL, const char far* title = 0, TModule* module = 0);
    virtual ~TTableWindow ();
    void SetTable(TTable *newTable);
    void SetAnalyzed(bool analyzed);

protected:
    THeaderClientWindow *headerWnd;
    TTableClientWindow *tableWnd;

    TLayoutMetrics headerMetrics;
    TLayoutMetrics tableMetrics;

    TTable *table;

//{{TTableWindowVIRTUAL_BEGIN}}
public:
    virtual void SetupWindow ();
//{{TTableWindowVIRTUAL_END}}

//{{TTableWindowRSP_TBL_BEGIN}}
protected:
    void EvKeyDown (uint key, uint repeatCount, uint flags);
//{{TTableWindowRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TTableWindow);
};    //{{TTableWindow}}


#endif                                      // __tblwin_h sentry.

