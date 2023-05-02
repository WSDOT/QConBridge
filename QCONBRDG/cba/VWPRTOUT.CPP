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


#include <qconbrdg\vwprtout.h>
#include <qconbrdg\qcblyout.h>
#include <classlib/time.h>
#include <classlib/date.h>

//{{TViewPrintout Implementation}}


TViewPrintout::TViewPrintout (TPrinter *printer,TBridgeView *bView,const char far* title) :
   TPrintout(title),
   Printer(printer),
   bview(bView)
{
}


TViewPrintout::~TViewPrintout ()
{
}


void TViewPrintout::GetDialogInfo (int& minPage, int& maxPage, int& selFromPage, int& selToPage)
{
   // Don't enable page range in print dialog since only one page
   // is available to print
   minPage = maxPage = 0;
   selFromPage = selToPage = 0;

  // Force default page orientation to landscape...
  // Landscape will be selected in the Printer Setup dialog
  DEVMODE* dm = const_cast<DEVMODE*> (Printer->GetSetup().GetDevMode());
  dm->dmFields |= DM_ORIENTATION;
  dm->dmOrientation = DMORIENT_LANDSCAPE;
}


void TViewPrintout::PrintPage (int /*page*/, TRect& /*rect*/, unsigned /*flags*/)
{
    DC->SetMapMode(MM_HIENGLISH);
    TQConBridgePageLayout *layout = new TQConBridgePageLayout(*DC,bview->GetDocument().GetDocPath(),*Printer->GetApplication());
    layout->SetCurrentPage(1);
    layout->SetTotalPages(1);
    layout->DrawHeader();

    // Frame the page
    TRect rPage = layout->GetPageRect();
    layout->Rectangle(rPage);

    // Convert page rect to pixel units
    // For use below
    DC->LPtoDP((TPoint *)&rPage,2);

    delete layout;
 
    // layout is no longer valid
    DC->SetMapMode(MM_TEXT);

    // Draw the contents of the view
    bview->GetDisplayObjectManager()->UpdateDisplay();
    bview->Draw(*DC,rPage,true);
    bview->GetDisplayObjectManager()->UpdateDisplay();
    bview->UpdateWindow();
}


void TViewPrintout::SetPrintParams (TPrintDC* dc, TSize pageSize)
{
    TPrintout::SetPrintParams(dc, pageSize);
}

