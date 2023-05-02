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

#if !defined(__tblprnt_h)              // Sentry, use file only if it's not already included.
#define __tblprnt_h


#include <owl\printer.h>
#include <qconbrdg\tblview.h>
#include <qconbrdg\table.h>


//{{TPrintout = TTablePrintout}}
class TTablePrintout : public TPrintout {
public:
    TTablePrintout (TPrinter *Printer,TTableView *tView,TTable *Table,const char far* title);
    virtual ~TTablePrintout ();

protected:
   TTable *table;
   TTableView *tview;
   TPrinter *printer;
   int totalPages;
   bool moreRecords;
   int numRecords;
   int currentRecord;
   int cxChar, cyChar;

//{{TTablePrintoutVIRTUAL_BEGIN}}
public:
    virtual void GetDialogInfo (int& minPage, int& maxPage, int& selFromPage, int& selToPage);
    virtual void SetPrintParams (TPrintDC* dc, TSize pageSize);
    virtual void BeginDocument (int startPage, int endPage, unsigned flags);
    virtual bool HasPage (int pageNumber);
    virtual void PrintPage (int page, TRect& rect, unsigned flags);
    virtual void EndDocument ();
//{{TTablePrintoutVIRTUAL_END}}
};    //{{TTablePrintout}}


#endif                                      // __tblprnt_h sentry.

