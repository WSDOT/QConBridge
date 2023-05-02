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


#include <qconbrdg\flprint.h>
#include <qconbrdg\qcblyout.h>


//{{TFilePrintout Implementation}}
TFilePrintout::TFilePrintout (TPrinter *printer,char* filename,const char far* title) :
    TPrintout(title),
    printer(printer),
    filename(filename)
{
    // INSERT>> Your constructor code here.

}


TFilePrintout::~TFilePrintout ()
{
    // INSERT>> Your destructor code here.

}

void TFilePrintout::BeginPrinting ()
{
    TPrintout::BeginPrinting();

    // INSERT>> Your code here.
}



void TFilePrintout::BeginDocument (int startPage, int endPage, unsigned flags)
{
    TPrintout::BeginDocument(startPage, endPage, flags);

    // INSERT>> Your code here.
    currentLine = 0;
    hasLines = true;
    ifile.open(filename.c_str());
}

void TFilePrintout::PrintPage (int page, TRect& rect, unsigned flags)
{
    TPrintout::PrintPage(page, rect, flags);

    // INSERT>> Your code here.
    DC->SetMapMode(MM_HIENGLISH);
    TQConBridgePageLayout *layout = new TQConBridgePageLayout(*DC,"",*printer->GetApplication());

    layout->SetCurrentPage(page);
    layout->DrawHeader();

    // Frame the page
    TRect rPage = layout->GetPageRect();
    layout->Rectangle(rPage);

    int lineOffset = 0;
    while (lineOffset < linesPerPage &&
           ifile)
       {
       char buffer[100];
       ifile.getline(buffer,sizeof(buffer));
       layout->LineTextOut(lineOffset,2,buffer);
       lineOffset++;
       currentLine++;
       }

    if (!ifile)
       hasLines = false;

    delete layout;
}


bool TFilePrintout::HasPage (int)
{
   return hasLines;
}


void TFilePrintout::GetDialogInfo (int& minPage, int& maxPage, int& selFromPage, int& selToPage)
{
   TPrintout::GetDialogInfo(minPage, maxPage, selFromPage, selToPage);
}


void TFilePrintout::SetPrintParams (TPrintDC* dc, TSize pageSize)
{
    TPrintout::SetPrintParams(dc, pageSize);

    // INSERT>> Your code here.


    // Setup the page layout so that we can compute the number of lines per
    // page.

    DC->SetMapMode(MM_HIENGLISH);
    TQConBridgePageLayout *layout = new TQConBridgePageLayout(*DC,"",*printer->GetApplication());

    linesPerPage = layout->GetLinesPerPage();

    delete layout;
}

void TFilePrintout::EndDocument ()
{
    TPrintout::EndDocument();

    // INSERT>> Your code here.
    ifile.close();
}

