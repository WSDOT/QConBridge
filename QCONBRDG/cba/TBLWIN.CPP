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


#include <qconbrdg\tblwin.h>
#include <owl\dc.h>


//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(TTableWindow, TLayoutWindow)
//{{TTableWindowRSP_TBL_BEGIN}}
    EV_WM_KEYDOWN,
//{{TTableWindowRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TTableWindow Implementation}}


TTableWindow::TTableWindow (TWindow* parent,TTable *Table, const char far* title, TModule* module):
    TLayoutWindow(parent, title, module)
{
    // INSERT>> Your constructor code here.
    table = Table;

    headerWnd = new THeaderClientWindow(this,table);
    tableWnd = new TTableClientWindow(this,table,headerWnd);
}

TTableWindow::~TTableWindow ()
{
    Destroy();

    // INSERT>> Your destructor code here.
    delete headerWnd;
    delete tableWnd;
}


void TTableWindow::SetTable(TTable *newTable)
{
   headerWnd->SetTable(newTable);
   tableWnd->SetTable(newTable);

   table = newTable;
}

void TTableWindow::SetupWindow ()
{
    TLayoutWindow::SetupWindow();

    // INSERT>> Your code here.
    headerMetrics.X.Set(lmLeft,lmSameAs,lmParent,lmLeft);
    headerMetrics.Y.Set(lmTop ,lmSameAs,lmParent,lmTop );
    headerMetrics.Width.SameAs(lmParent,lmRight);
    headerMetrics.Height.Absolute(headerWnd->Attr.H);

    SetChildLayoutMetrics(*headerWnd,headerMetrics);

    tableMetrics.X.Set(lmLeft,lmSameAs,lmParent,lmLeft);
    tableMetrics.Y.Set(lmTop,lmSameAs,headerWnd,lmBottom);
    tableMetrics.Width.SameAs(lmParent,lmRight);
    tableMetrics.Height.SameAs(lmParent,lmBottom);

    SetChildLayoutMetrics(*tableWnd,tableMetrics);

    Layout();
}


void TTableWindow::EvKeyDown (uint key, uint repeatCount, uint flags)
{
    TLayoutWindow::EvKeyDown(key, repeatCount, flags);

    switch(key)
       {
       case VK_HOME:  ::SendMessage(tableWnd->HWindow,WM_VSCROLL,SB_TOP,     0L); break;
       case VK_END:   ::SendMessage(tableWnd->HWindow,WM_VSCROLL,SB_BOTTOM,  0L); break;
       case VK_PRIOR: ::SendMessage(tableWnd->HWindow,WM_VSCROLL,SB_PAGEUP,  0L); break;
       case VK_NEXT:  ::SendMessage(tableWnd->HWindow,WM_VSCROLL,SB_PAGEDOWN,0L); break;
       case VK_UP:    ::SendMessage(tableWnd->HWindow,WM_VSCROLL,SB_LINEUP,  0L); break;
       case VK_DOWN:  ::SendMessage(tableWnd->HWindow,WM_VSCROLL,SB_LINEDOWN,0L); break;
       case VK_LEFT:  ::SendMessage(tableWnd->HWindow,WM_HSCROLL,SB_LINEUP,  0L); break;
       case VK_RIGHT: ::SendMessage(tableWnd->HWindow,WM_HSCROLL,SB_LINEDOWN,0L); break;
       }
}

void TTableWindow::SetAnalyzed(bool analyzed)
{
   // Pass notification along to table header
   headerWnd->SetAnalyzed(analyzed);
}

