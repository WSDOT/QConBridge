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

#include <qconbrdg\hdrcli.h>
#include <owl\dc.h>

//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(THeaderClientWindow, TWindow)
//{{THeaderClientWindowRSP_TBL_BEGIN}}
    EV_WM_SIZE,
    EV_WM_RBUTTONDOWN,
//{{THeaderClientWindowRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{THeaderClientWindow Implementation}}


THeaderClientWindow::THeaderClientWindow (TWindow* parent,TTable *Table, const char far* title, TModule* module):
    TWindow(parent, title, module)
{
    // INSERT>> Your constructor code here.
   table = Table;
   isAnalyzed = false;

   TColor windowColor = ::GetSysColor(COLOR_WINDOW);
   TColor bkColor = (windowColor == TColor::LtGray) ? TColor::Gray : TColor::LtGray;
   SetBkgndColor(bkColor);
   Attr.Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;

   // Setup some dummy values... The layout constraints will adjust them
   Attr.X = 0;
   Attr.Y = 0;
   Attr.W = 0;
   Attr.H = 0;

   offset = 0;
}


THeaderClientWindow::~THeaderClientWindow ()
{
    Destroy();

    // INSERT>> Your destructor code here.

}

void THeaderClientWindow::SetAnalyzed(bool analyzed)
{
   isAnalyzed = analyzed;
}

void THeaderClientWindow::SetTable(TTable *newTable)
{
   table = newTable;
   if (HWindow)
      {
      Invalidate();
      UpdateWindow();
      }
}

void THeaderClientWindow::Paint (TDC& dc, bool erase, TRect& rect)
{
    TWindow::Paint(dc, erase, rect);

    // INSERT>> Your code here.

    TRect clientRect = GetClientRect();
    char buffer[256];
    int textAlign;
    int bkMode;
    TColor textColor;
    int numFields;
    int field;
    int fieldStart;
    int fieldWidth;

    if (!isAnalyzed || (table == NULL))
       {
       string msg = (!isAnalyzed) ? "Results Not Available - Use Bridge | Analyze" : "Use View menu to select a table";
       bkMode = dc.SetBkMode(TRANSPARENT);
       textAlign = dc.SetTextAlign(TA_TOP | TA_CENTER);
       textColor = dc.SetTextColor(TColor::LtRed);

       dc.TextOut(clientRect.left + clientRect.Width()/2,0,msg.c_str());

       dc.SetTextColor(textColor);
       dc.SetTextAlign(textAlign);
       dc.SetBkMode(bkMode);

       return;
       }

    table->GetTitle(buffer,sizeof(buffer));

    textAlign = dc.SetTextAlign(TA_TOP | TA_CENTER);

    dc.TextOut(clientRect.left + clientRect.Width()/2,0,buffer);

    numFields = table->GetNumFields();
    fieldStart = offset*cxChar;
    for (field = 0; field < numFields; field++)
       {
       fieldWidth = table->GetFieldWidth(field)*cxChar;
       table->GetFieldName(field,buffer,sizeof(buffer));
       dc.TextOut(fieldStart + fieldWidth/2,
                  clientRect.top+clientRect.Height()/2,
                  buffer);
       fieldStart += (fieldWidth + cxChar);
       }

    dc.SetTextAlign(textAlign);
}


void THeaderClientWindow::SetupWindow ()
{
    TWindow::SetupWindow();

    // INSERT>> Your code here.
   TEXTMETRIC tm;
   TClientDC dc(HWindow);

   dc.GetTextMetrics(tm);
   cxChar = tm.tmAveCharWidth;
   cyChar = tm.tmHeight + tm.tmExternalLeading;

   SetWindowPos(0,0,0,0,cyChar,SWP_NOZORDER | SWP_NOMOVE);
}


void THeaderClientWindow::EvSize (uint sizeType, TSize& size)
{
    TWindow::EvSize(sizeType, size);

    // INSERT>> Your code here.
    Invalidate();
    UpdateWindow();
}

int THeaderClientWindow::GetHeight()
{
   return 2*cyChar;
}

void THeaderClientWindow::SetOffset(int off)
{
   offset = -off;
   Invalidate();
   UpdateWindow();
}

void THeaderClientWindow::EvRButtonDown (uint modKeys, TPoint& point)
{
    MapWindowPoints(Parent->HWindow,&point,1);
    LPARAM lParam = MAKELONG(point.x,point.y);
    Parent->HandleMessage(WM_RBUTTONDOWN,modKeys,lParam);
}

