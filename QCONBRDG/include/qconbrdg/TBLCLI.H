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

#if !defined(__tblcli_h)              // Sentry, use file only if it's not already included.
#define __tblcli_h


#include <owl\window.h>
#include <qconbrdg\table.h>
#include <qconbrdg\hdrcli.h>


//{{TWindow = TTableClientWindow}}
class TTableClientWindow : public TWindow {
public:
    TTableClientWindow (TWindow* parent,TTable *Table,THeaderClientWindow *Header,const char far* title = 0, TModule* module = 0);
    virtual ~TTableClientWindow ();

    void SetTable(TTable *newTable);

//{{TTableClientWindowVIRTUAL_BEGIN}}
public:
    virtual void SetupWindow ();
    virtual void Paint (TDC& dc, bool erase, TRect& rect);
//{{TTableClientWindowVIRTUAL_END}}

protected:
    void UpdateScroller();

protected:
   int cxChar;
   int cyChar;
   TTable *table;
   THeaderClientWindow *header;

//{{TTableClientWindowRSP_TBL_BEGIN}}
protected:
    void EvHScroll (uint scrollCode,uint thumbPos,HWND hWndCtl);
    void EvRButtonDown (uint modKeys, TPoint& point);
//{{TTableClientWindowRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TTableClientWindow);
};    //{{TTableClientWindow}}


#endif                                      // __tblcli_h sentry.

