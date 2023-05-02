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


#include <qconbrdg\tblview.h>
#include <qconbrdg\viewev.h>
#include <qconbrdg\seltable.h>
#include <qconbrdg\tblprnt.h>
#include <qconbrdg\qcbapp.h>
#include <owl\opensave.h>
#include <fstream.h>

//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE2(TTableView,TQConBridgeView,TTableWindow)
//{{TTableViewRSP_TBL_BEGIN}}
    EV_VN_ISWINDOW,
    EV_VN_ANALYZED,
    EV_VN_UNITSCHANGED,
    EV_WM_RBUTTONDOWN,
    EV_COMMAND(CM_SELECTTABLE, CmSelectTable),
    EV_COMMAND(CM_EXPORT, CmExportTable),
    EV_COMMAND_ENABLE(CM_EXPORT, CeExportTable),
//{{TTableViewRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TTableView Implementation}}

TTableView::TTableView (TBridgeDocument& doc, TWindow* parent):
    TQConBridgeView(doc),
    TTableWindow(parent)
{
    // INSERT>> Your constructor code here.
    selectedTable = NULL;
    tableSelected = false;
    dc = dcDC;

    SetAnalyzed(bridgeDoc.IsAnalyzed());

    SetTable(selectedTable);
}


TTableView::~TTableView ()
{
    // INSERT>> Your destructor code here.
    delete selectedTable;
}

bool TTableView::CanPrint()
{
   bool retval = (selectedTable != NULL);
   return retval;
}

TPrintout *TTableView::GetPrintout()
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   TPrintout *printout = new TTablePrintout(theApp->GetPrinter(),this,table,GetViewName());

   return printout;
}

const char far* TTableView::GetViewName ()
{
    return StaticName();
}

TResId TTableView::GetIconResourceId() const
{
   return IDI_TABLEVIEW;
}


bool TTableView::SetDocTitle (const char far* /*docname*/, int /*index*/)
{
   return TLayoutWindow::SetDocTitle(GetViewName(),0);
}


void TTableView::SetupWindow ()
{
    TTableWindow::SetupWindow();
}


TWindow* TTableView::GetWindow ()
{
   return (TWindow *)this;
}

bool TTableView::VnAnalyzed(bool analyzed)
{
   SetAnalyzed(analyzed); // Notify TTableWindow

   if (analyzed)
      {
      if (tableSelected)
         {
         delete selectedTable;
         selectedTable = bridgeDoc.CreateTable(dc);
         }
      }
   else
      {
      delete selectedTable;
      selectedTable = NULL;
      }

   SetTable(selectedTable);

   return true;
}

void TTableView::EvRButtonDown (uint /*modKeys*/, TPoint& point)
{
    TPopupMenu menu;

    ClientToScreen(point);

    menu.AppendMenu(MF_BYCOMMAND,CM_SELECTTABLE,"Select Table...");
    menu.AppendMenu(MF_SEPARATOR);
    menu.AppendMenu(MF_BYCOMMAND,CM_EXPORT,"Export...");

    menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point,0,HWindow);
}


void TTableView::CmSelectTable ()
{
    TSelTableDlg dlg(this);
    SelTableXferBuffer xfer(GetApplication());

    xfer.DrawDiagram(dc,true); // change this for actual selection

    dlg.SetTransferBuffer(&xfer);

    if (dlg.Execute() == IDOK)
       {
       dc = xfer.GetSelection();
       tableSelected = true;

       if (bridgeDoc.IsAnalyzed())
          {
          // Can only get new table if the bridge is analyzed
          delete selectedTable;
          selectedTable = bridgeDoc.CreateTable(dc);
          }
       SetTable(selectedTable);
       }
}


bool TTableView::VnUnitsChanged(void)
{
   selectedTable->SetUnits(bridgeDoc.GetUnits());
   Invalidate();
   UpdateWindow();

   return true;
}

bool TTableView::VnIsWindow(HWND hWnd)
{
  return hWnd == HWindow || IsChild(hWnd);
}

void TTableView::CmExportTable()
{
   if (!selectedTable)
      return;

   // Prompt for file name
   char buffer[256];
   selectedTable->GetTitle(buffer,256);
   ostrstream title;
   title << "Export " << buffer << ends;

   string temp = GetApplication()->LoadString(IDS_TABLEEXPORTFILTER);
   char* filter = new char[temp.length() + 1];
   strcpy(filter,temp.c_str());
   TOpenSaveDialog::TData data(OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
                               filter,
                               0,
                               "",
                               "EXP");
   TFileSaveDialog dlg(this,data);
   dlg.SetCaption(title.str());
   delete[] title.str();

   if (dlg.Execute() == IDOK)
      {
      // open file
      ofstream ofile(data.FileName);

      // test for successful opening
      if (!ofile)
         {
         // if not successful, notify errors, return
         ostrstream os;
         os << "Error opening " << data.FileName << ends;
         MessageBox(os.str(),"Error",MB_OK | MB_ICONEXCLAMATION);
         delete[] os.str();
         }

      // stream table to file
      ofile << (*selectedTable) << endl;

      // close file
      ofile.close();
      }

   delete[] filter;
}

void TTableView::CeExportTable(TCommandEnabler &tce)
{
    tce.Enable(selectedTable != NULL);
}
