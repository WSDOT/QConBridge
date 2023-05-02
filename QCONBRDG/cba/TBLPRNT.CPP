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


#include <qconbrdg\tblprnt.h>
#include <qconbrdg\qcblyout.h>


//{{TTablePrintout Implementation}}


TTablePrintout::TTablePrintout (TPrinter *Printer,TTableView *tView,TTable *Table,const char far* title) :
    TPrintout(title)
{
    // INSERT>> Your constructor code here.
    printer = Printer;
    table = Table;
    tview = tView;

    totalPages = 0;
    moreRecords = true;
}


TTablePrintout::~TTablePrintout ()
{
    // INSERT>> Your destructor code here.

}


void TTablePrintout::GetDialogInfo (int& minPage, int& maxPage, int& selFromPage, int& selToPage)
{
    TPrintout::GetDialogInfo(minPage, maxPage, selFromPage, selToPage);

    // INSERT>> Your code here.
    minPage = 1;
    maxPage = 999;
    selFromPage = 0;
    selToPage = 0;

  // Force default page orientation to portrait...
  // Portrait will be selected in the Printer Setup dialog
  DEVMODE* dm = const_cast<DEVMODE*> (printer->GetSetup().GetDevMode());
  dm->dmFields |= DM_ORIENTATION;
  dm->dmOrientation = DMORIENT_PORTRAIT;
}


void TTablePrintout::SetPrintParams (TPrintDC* dc, TSize pageSize)
{
    TPrintout::SetPrintParams(dc, pageSize);

    // INSERT>> Your code here.

    // If you have no idea how many pages there are going to be, use a huge #
    totalPages = 32000;
}


void TTablePrintout::BeginDocument (int startPage, int endPage, unsigned flags)
{
    TPrintout::BeginDocument(startPage, endPage, flags);

    // INSERT>> Your code here.
    numRecords = table->GetNumRecords();
    currentRecord = 0;
}


bool TTablePrintout::HasPage (int /*pageNumber*/)
{
    return moreRecords;
}


void TTablePrintout::PrintPage (int page, TRect& rect, unsigned flags)
{
    TPrintout::PrintPage(page, rect, flags);

    // INSERT>> Your code here.
    char buffer[256];
    char **headings;
    int *fieldWidths;
    int numFields;
    int field;
    int rowsPerPage;

    DC->SetMapMode(MM_HIENGLISH);
    TQConBridgeTableLayout *layout = new TQConBridgeTableLayout(*DC,tview->GetDocument().GetDocPath(),*printer->GetApplication());
    layout->SetCurrentPage(page);
    layout->DrawHeader();

    //
    // Setup table
    //
    table->GetTitle(buffer,sizeof(buffer));
    layout->SetTitle(buffer);
    layout->SetTitleFont("Arial",18,FW_BOLD);
    layout->SetHeadingFont("Arial",12);
    layout->SetCellFont("Courier New",10);

    numFields = table->GetNumFields();

    // Setup proportionally spaced columns
    headings = new char*[numFields];
    fieldWidths = new int[numFields];
    for (field = 0; field < numFields; field++)
       {
       table->GetFieldName(field,buffer,sizeof(buffer));
       headings[field] = new char[strlen(buffer)+1];
       strcpy(headings[field],buffer);
       fieldWidths[field] = table->GetFieldWidth(field);
       }

    layout->DefineColumn(numFields,headings,fieldWidths);

    for (field = 0; field < numFields; field++)
       {
       delete[] headings[field];
       }
    delete[] headings;
    delete[] fieldWidths;


    // Draw the table format and the "live" data
    rowsPerPage = layout->GetRowsPerPage();

    layout->DrawTable();
    int row = 0;
    for (; currentRecord < page*rowsPerPage && currentRecord < numRecords; currentRecord++)
       {
       for (field = 0; field < numFields; field++)
          {
          table->GetEntry(currentRecord,field,buffer,sizeof(buffer));
          layout->TableTextOut(row,field,buffer);
          }

       if (((currentRecord+1) % table->GetRecordGroupCount()) == 0)
          layout->HorizGridLine(row);

       row++;
       }

    if (currentRecord >= numRecords)
       moreRecords = false;
}


void TTablePrintout::EndDocument ()
{
    TPrintout::EndDocument();

    // INSERT>> Your code here.
}

