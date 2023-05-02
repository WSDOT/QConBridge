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


#include <qconbrdg\tblcli.h>
#include <qconbrdg\qcbapp.rh>
#include <owl\dc.h>
#include <owl\scroller.h>

//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(TTableClientWindow, TWindow)
//{{TTableClientWindowRSP_TBL_BEGIN}}
    EV_WM_HSCROLL,
    EV_WM_RBUTTONDOWN,
//{{TTableClientWindowRSP_TBL_END}}
END_RESPONSE_TABLE;


TTableClientWindow::TTableClientWindow (TWindow* parent,TTable *Table,THeaderClientWindow *Header, const char far* title, TModule* module):
    TWindow(parent, title, module)
{
    // INSERT>> Your constructor code here.
   table = Table;
   header = Header;

   Attr.Style |= WS_VSCROLL | WS_HSCROLL | WS_CHILD;
   Scroller = new TScroller(this,1,1,1,1);
}


TTableClientWindow::~TTableClientWindow ()
{
    Destroy();

    // INSERT>> Your destructor code here.

}

void TTableClientWindow::SetTable(TTable *newTable)
{
   table = newTable;
   UpdateScroller();
   if (HWindow)
      {
      Invalidate();
      UpdateWindow();
      }
}

void TTableClientWindow::SetupWindow ()
{
    TWindow::SetupWindow();

    // INSERT>> Your code here.
   TEXTMETRIC tm;
   TClientDC dc(HWindow);

   dc.GetTextMetrics(tm);
   cxChar = tm.tmAveCharWidth;
   cyChar = tm.tmHeight + tm.tmExternalLeading;

   UpdateScroller();
}

void TTableClientWindow::UpdateScroller()
{
    if (table == NULL)
       return;

   int totalRecordWidth = 0;
   for (int i = 0; i < table->GetNumFields(); i++)
      totalRecordWidth += table->GetFieldWidth(i);
   totalRecordWidth += table->GetNumFields();

   Scroller->SetUnits(cxChar,cyChar);
   Scroller->SetRange(totalRecordWidth,table->GetNumRecords());
}


void TTableClientWindow::Paint (TDC& dc, bool erase, TRect& rect)
{
    TWindow::Paint(dc, erase, rect);

    // INSERT>> Your code here.
    if (table == NULL)
       return;

    char buffer[256];
    int textAlign;
    int numFields;
    int field;
    int numRecords;
    int record;
    int fieldStart;
    int fieldWidth;
    TRect clientRect = GetClientRect();
    TColor color;
    TColor oldColor;
    int bkMode;
    int colorNum;

    textAlign = dc.SetTextAlign(TA_TOP | TA_RIGHT);
    numRecords = table->GetNumRecords();
    numFields = table->GetNumFields();

    oldColor = dc.GetTextColor();
    bkMode = dc.SetBkMode(TRANSPARENT);

    int x,y;
    int start,end;
    /*
    int linesPerPage = Scroller->YPage/Scroller->YLine;
    start = Scroller->YPos;
    end = start + linesPerPage + 1;
    end = min(end,numRecords);
    */
    start = 0;
    end = numRecords;
    color = TColor::Black;
    colorNum = 0;
    for (record = start; record < end; record++)
       {
       if ((record % table->GetRecordGroupCount()) == 0)
          {
          color = ((colorNum++ % 2) == 0) ? TColor::Black : TColor::LtBlue;
          }
       dc.SetTextColor(color);

       fieldStart = 0;
       y = record*cyChar;
       for (field = 0; field < numFields; field++)
          {
          fieldWidth = table->GetFieldWidth(field)*cxChar;

          table->GetEntry(record,field,buffer,sizeof(buffer));
          x = fieldStart + fieldWidth;
          dc.TextOut(x,y,buffer);

          fieldStart += (fieldWidth + cxChar);
          }
       }

    dc.SetTextAlign(textAlign);
    dc.SetTextColor(oldColor);
    dc.SetBkMode(bkMode);
}

void TTableClientWindow::EvHScroll(uint scrollCode,uint thumbPos,HWND hWndCtl)
{
   TWindow::EvHScroll(scrollCode,thumbPos,hWndCtl);
   header->SetOffset(Scroller->XPos);
}


void TTableClientWindow::EvRButtonDown (uint modKeys, TPoint& point)
{
    MapWindowPoints(Parent->HWindow,&point,1);
    LPARAM lParam = MAKELONG(point.x,point.y);
    Parent->HandleMessage(WM_RBUTTONDOWN,modKeys,lParam);
}

