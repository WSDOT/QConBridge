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


#include <qconbrdg\rptwin.h>
#include <owl\scroller.h>
#include <owl\gdiobjec.h>
#include <iostream.h>
#include <fstream.h>

//{{TReportWindow Implementation}}


TReportWindow::TReportWindow (TWindow* parent,const char *filename, const char far* title, TModule* module):
    TWindow(parent, title, module),
    Filename(filename)
{
    // INSERT>> Your constructor code here.
	 Attr.Style |= WS_VSCROLL | WS_HSCROLL;
	 Scroller = new TScroller(this,1,1,1,1);

	 lines = new TIStringArray(10,0,10);

	 ReadFile();
}


TReportWindow::~TReportWindow ()
{
	 Destroy();

	 // INSERT>> Your destructor code here.
	 delete lines;
}

void TReportWindow::SetupWindow()
{
   TWindow::SetupWindow();

   TEXTMETRIC tm;
   TClientDC dc(HWindow);

   dc.GetTextMetrics(tm);

   cxChar = tm.tmAveCharWidth;
   cyChar = tm.tmHeight + tm.tmExternalLeading;

   UpdateScroller();
}

void TReportWindow::UpdateScroller()
{
    if (NumLines() == 0)
      {
      Scroller->SetUnits(1,1);
      Scroller->SetRange(1,1);
      }
    else
      {
      Scroller->SetUnits(cxChar,cyChar);
      Scroller->SetRange(80,NumLines());
      }
}

void TReportWindow::OpenReportFile(const char* filename)
{
   CloseReportFile();
   Filename = filename;

   ReadFile();

   UpdateScroller();
}

void TReportWindow::CloseReportFile()
{
   Filename = "";
   lines->Flush();
   UpdateScroller();
}

void TReportWindow::ReadFile()
{
	ifstream ifile(Filename.c_str());

	if (!ifile)
		return;

	while(ifile)
		{
		char buffer[100];
		ifile.getline(buffer,sizeof(buffer));

		string *newLine = new string(buffer);
		lines->Add(newLine);
		}

	ifile.close();
}

void TReportWindow::Paint (TDC& dc, bool erase, TRect& rect)
{
	 TRACE("Entering TReportWindow::Paint()");

	 TWindow::Paint(dc, erase, rect);

	 // INSERT>> Your code here.
    int cxChar,cyChar;
    TEXTMETRIC tm;
    dc.GetTextMetrics(tm);
    cxChar = tm.tmAveCharWidth;
    cyChar = tm.tmHeight;

    // If there are not lines to write, then there are not results available
    if (lines->GetItemsInContainer() == 0)
       {
       dc.TextOut(0,0,"Results not available.");
       dc.TextOut(0,2*cyChar,"Select Bridge | Analyze to analyze the bridge");
       return;
       }

    // Since this wont change, move it to SetupWindow

    // Setup a fixed point font
    dc.SelectStockObject(OEM_FIXED_FONT);

    int i;
    for (i = 0; i < lines->GetItemsInContainer(); i++)
       {
       string *line = (*lines)[i];
       dc.TextOut(cxChar,i*cyChar,line->c_str());
       }

    TRACE("Leaving TReportWindow::Paint()");
}

int TReportWindow::NumLines()
{
   return lines->GetItemsInContainer();
}

void TReportWindow::GetLine(int i,char *buffer,int maxChar)
{
   PRECONDITIONX(i < lines->GetItemsInContainer(),"TReportWindow::GetLine() - Invalid line number");
   string *line = (*lines)[i];
   buffer[0] = 0;
   strncat(buffer,line->c_str(),maxChar);
}

bool TReportWindow::WriteToFile(const char *filename)
{
   ofstream ofile(filename);

   if (!ofile)
      return false;

   ofile << "Washington State Department of Transporation" << endl;
   ofile << "Bridge and Structures Office" << endl;
   ofile << "QConBridge Version 1.0" << endl;
   ofile << endl;

    int i;
    for (i = 0; i < lines->GetItemsInContainer(); i++)
       {
       string *line = (*lines)[i];
       ofile << (*line) << endl;
       }

   return true;
}

