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


#include <qconbrdg\pgprint.h>
#include <qconbrdg\qcblyout.h>


//{{TPagePrintout Implementation}}


TPagePrintout::TPagePrintout (TPrinter *printer,TReportView *view,const char far* title) :
    TPrintout(title),
    printer(printer),
    rview(view)
{
    // INSERT>> Your constructor code here.

}


TPagePrintout::~TPagePrintout ()
{
    // INSERT>> Your destructor code here.

}

void TPagePrintout::BeginPrinting ()
{
    TPrintout::BeginPrinting();

    // INSERT>> Your code here.
}



void TPagePrintout::BeginDocument (int startPage, int endPage, unsigned flags)
{
    TPrintout::BeginDocument(startPage, endPage, flags);

    // INSERT>> Your code here.
    currentLine = 0;
}

void TPagePrintout::PrintPage (int page, TRect& rect, unsigned flags)
{
   TRACE("Entering TPagePrintout::PrintPage()");

    TPrintout::PrintPage(page, rect, flags);

    // INSERT>> Your code here.
    DC->SetMapMode(MM_HIENGLISH);
    TQConBridgePageLayout *layout = new TQConBridgePageLayout(*DC,rview->GetDocument().GetDocPath(),*printer->GetApplication());

    layout->SetCurrentPage(page);
    layout->SetTotalPages(numPages);
    layout->DrawHeader();

    // Frame the page
    TRect rPage = layout->GetPageRect();
    layout->Rectangle(rPage);

    int lineOffset = 0;
    while (currentLine < linesPerPage*page &&
           currentLine < rview->NumLines())
       {
       char buffer[100];

       rview->GetLine(currentLine,buffer,sizeof(buffer));

       layout->LineTextOut(lineOffset,2,buffer);

       lineOffset++;
       currentLine++;
       }

    delete layout;

   TRACE("Leaving TPagePrintout::PrintPage()");
}


bool TPagePrintout::HasPage (int pageNumber)
{
   return (pageNumber <= numPages);
}


void TPagePrintout::GetDialogInfo (int& minPage, int& maxPage, int& selFromPage, int& selToPage)
{
   TRACE("Entering TPagePrintout::GetDialogInfo()");

   TPrintout::GetDialogInfo(minPage, maxPage, selFromPage, selToPage);

   // Force default page orientation to portrait...
   // Portrait will be selected in the Printer Setup dialog
   DEVMODE* dm = const_cast<DEVMODE*> (printer->GetSetup().GetDevMode());
   dm->dmFields |= DM_ORIENTATION;
   dm->dmOrientation = DMORIENT_PORTRAIT;

   TRACE("Leaving TPagePrintout::GetDialogInfo()");
}


void TPagePrintout::SetPrintParams (TPrintDC* dc, TSize pageSize)
{
    TPrintout::SetPrintParams(dc, pageSize);

    // INSERT>> Your code here.


    // Setup the page layout so that we can compute the number of lines per
    // page.

    DC->SetMapMode(MM_HIENGLISH);
    TQConBridgePageLayout *layout = new TQConBridgePageLayout(*DC,rview->GetDocument().GetDocPath(),*printer->GetApplication());

    linesPerPage = layout->GetLinesPerPage();

    int numLines = rview->NumLines();

    numPages = numLines/linesPerPage + 1;

    delete layout;
}

