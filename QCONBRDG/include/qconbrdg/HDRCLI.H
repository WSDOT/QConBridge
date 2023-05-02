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

#if !defined(__hdrcli_h)              // Sentry, use file only if it's not already included.
#define __hdrcli_h


#include <owl\window.h>
#include <owl\color.h>
#include <qconbrdg\table.h>


//{{TWindow = THeaderClientWindow}}
class THeaderClientWindow : public TWindow {
public:
    THeaderClientWindow (TWindow* parent,TTable *Table,const char far* title = 0, TModule* module = 0);
    virtual ~THeaderClientWindow ();

    int GetHeight();
    void SetOffset(int off);
    void SetTable(TTable *newTable);
    void SetAnalyzed(bool analyzed);

protected:
   TTable *table;
   bool isAnalyzed;
   int cxChar;
   int cyChar;
   int offset;

//{{THeaderClientWindowVIRTUAL_BEGIN}}
public:
    virtual void Paint (TDC& dc, bool erase, TRect& rect);
    virtual void SetupWindow ();
//{{THeaderClientWindowVIRTUAL_END}}

//{{THeaderClientWindowRSP_TBL_BEGIN}}
protected:
    void EvSize (uint sizeType, TSize& size);
    void EvRButtonDown (uint modKeys, TPoint& point);
//{{THeaderClientWindowRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(THeaderClientWindow);
};    //{{THeaderClientWindow}}


#endif                                      // __hdrcli_h sentry.

