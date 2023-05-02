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


#include <qconbrdg\rptview.h>
#include <qconbrdg\pgprint.h>
#include <qconbrdg\viewev.h>
#include <qconbrdg\qcbapp.h>
#include <qconbrdg\qcbapp.rh>
#include <owl\opensave.h>
#include <fstream.h>


//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE2(TReportView,TQConBridgeView,TReportWindow)
//{{TReportViewRSP_TBL_BEGIN}}
    EV_WM_RBUTTONDOWN,
    EV_VN_ISWINDOW,
    EV_VN_ANALYZED,
    EV_VN_UNITSCHANGED,
    EV_COMMAND(CM_EXPORT, CmExportReport),
    EV_COMMAND_ENABLE(CM_EXPORT, CeExportReport),
//{{TReportViewRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TReportView Implementation}}

TReportView::TReportView (TBridgeDocument& doc, TWindow* parent):
    TQConBridgeView(doc),
    TReportWindow(parent,doc.GetReportName())
{
    // INSERT>> Your constructor code here.


}


TReportView::~TReportView ()
{
    // INSERT>> Your destructor code here.

}

const char far* TReportView::GetViewName ()
{
    return StaticName();
}

TResId TReportView::GetIconResourceId() const
{
   return IDI_REPORTVIEW;
}


TWindow* TReportView::GetWindow ()
{
   return (TWindow *)this;
}


bool TReportView::VnIsWindow(HWND hWnd)
{
  return hWnd == HWindow || IsChild(hWnd);
}

bool TReportView::SetDocTitle (const char far* /*docname*/, int /*index*/)
{
   return TWindow::SetDocTitle(GetViewName(),0);
}

TPrintout *TReportView::GetPrintout()
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   TPrintout *printout;

   printout = new TPagePrintout (theApp->GetPrinter(),this,GetViewName());

   return printout;
}


bool TReportView::VnAnalyzed(bool analyzed)
{
   if (analyzed)
      {
      OpenReportFile(bridgeDoc.GetReportName());
      Invalidate();
      UpdateWindow();
      }
   else
      {
      CloseReportFile();
      Invalidate();
      UpdateWindow();
      }

   return true;
}

bool TReportView::VnUnitsChanged(void)
{
   if (bridgeDoc.IsAnalyzed())
      {
      OpenReportFile(bridgeDoc.GetReportName());
      Invalidate();
      UpdateWindow();
      }

   return true;
}

void TReportView::EvRButtonDown (uint /*modKeys*/, TPoint& point)
{
    TPopupMenu menu;

    ClientToScreen(point);
    menu.AppendMenu(MF_BYCOMMAND,CM_EXPORT,"Export...");
    menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point,0,HWindow);
}

void TReportView::CmExportReport()
{
   // Prompt for file name
   ostrstream title;
   title << "Export Report" << ends;

   string temp = GetApplication()->LoadString(IDS_REPORTEXPORTFILTER);
   char* filter = new char[temp.length()+1];
   strcpy(filter,temp.c_str());
   TOpenSaveDialog::TData data(OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
                               filter,
                               0,
                               "",
                               "RPT");
   TFileSaveDialog dlg(this,data);
   dlg.SetCaption(title.str());
   delete[] title.str();

   if (dlg.Execute() == IDOK)
      {
      if (!WriteToFile(data.FileName))
         {
         // if not successful, notify errors, return
         ostrstream os;
         os << "Error opening " << data.FileName << ends;
         MessageBox(os.str(),"Error",MB_OK | MB_ICONEXCLAMATION);
         delete[] os.str();
         }
      }

   delete[] filter;
}

void TReportView::CeExportReport(TCommandEnabler &tce)
{
    tce.Enable(bridgeDoc.IsAnalyzed());
}
