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

#if !defined(__vwprtout_h)              // Sentry, use file only if it's not already included.
#define __vwprtout_h


#include <qconbrdg\bview.h>

//{{TPrintout = TViewPrintout}}
class TViewPrintout : public TPrintout {
public:
    TViewPrintout (TPrinter *printer,TBridgeView *bView,const char far* title);
    virtual ~TViewPrintout ();

protected:
   TBridgeView *bview;
   TPrinter *Printer;

//{{TViewPrintoutVIRTUAL_BEGIN}}
public:
    virtual void GetDialogInfo (int& minPage, int& maxPage, int& selFromPage, int& selToPage);
    virtual void PrintPage (int page, TRect& rect, unsigned flags);
    virtual void SetPrintParams (TPrintDC* dc, TSize pageSize);
//{{TViewPrintoutVIRTUAL_END}}
};    //{{TViewPrintout}}


#endif                                      // __vwprtout_h sentry.

