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

#if !defined(__pgprint_h)              // Sentry, use file only if it's not already included.
#define __pgprint_h


#include <owl\printer.h>
#include <qconbrdg\rptview.h>
#include <qconbrdg\qcbapp.rh>            // Definition of all resources.


//{{TPrintout = TPagePrintout}}
class TPagePrintout : public TPrintout {
public:
    TPagePrintout (TPrinter *printer,TReportView *view,const char far* title);
    virtual ~TPagePrintout ();

protected:
    TReportView *rview;
    TPrinter *printer;
    int numPages;
    int linesPerPage;
    int currentLine;

//    void SetupPage(TPageLayout *layout);

//{{TPagePrintoutVIRTUAL_BEGIN}}
public:
    virtual void BeginPrinting ();
    virtual void PrintPage (int page, TRect& rect, unsigned flags);
    virtual void BeginDocument (int startPage, int endPage, unsigned flags);
    virtual bool HasPage (int pageNumber);
    virtual void GetDialogInfo (int& minPage, int& maxPage, int& selFromPage, int& selToPage);
    virtual void SetPrintParams (TPrintDC* dc, TSize pageSize);
//{{TPagePrintoutVIRTUAL_END}}
};    //{{TPagePrintout}}


#endif                                      // __pgprint_h sentry.

