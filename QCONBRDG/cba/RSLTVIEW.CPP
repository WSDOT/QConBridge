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


#include <qconbrdg\rsltview.h>
#include <qconbrdg\selload.h>
#include <owltools\fonthelp.h>

//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(TBridgeResultsView, TBridgeView)
//{{TBridgeResultsViewRSP_TBL_BEGIN}}
    EV_VN_ANALYZED,
    EV_VN_PLOTSTYLECHANGED,
    EV_VN_DISPLAYGRID,
    EV_COMMAND(CM_SELECTLOADING, CmSelectLoading),
    EV_WM_RBUTTONDOWN,
//{{TBridgeResultsViewRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TBridgeResultsView Implementation}}


TBridgeResultsView::TBridgeResultsView (TBridgeDocument& doc,TWindow *parent):
    TBridgeView(doc,parent)
{
   resultsDispObjMgr = new TResultsDisplayObjectManager(this);
   bridgeDispObjMgr = resultsDispObjMgr;
   resultsDispObjMgr->Analyzed(bridgeDoc.IsAnalyzed());
}


TBridgeResultsView::~TBridgeResultsView ()
{
    delete resultsDispObjMgr;
}

void TBridgeResultsView::SetupWindow()
{
   TWindow::SetupWindow();
   resultsDispObjMgr->DisplayGrid(bridgeDoc.DisplayGrid());
}

bool TBridgeResultsView::VnAnalyzed (bool analyzed)
{
   resultsDispObjMgr->Clear();

   CreateDisplayObjects(); // rebuilds all display objects

   resultsDispObjMgr->Analyzed(analyzed);
   return TBridgeView::VnAnalyzed(analyzed);
}

bool TBridgeResultsView::VnPlotStyleChanged (DiagramCatagory dc)
{
   if (resultsDispObjMgr->DrawDiagram(dc))
   {
      resultsDispObjMgr->UpdateDisplay();
      Invalidate();
   }

   return true;
}

bool TBridgeResultsView::VnDisplayGrid(bool display)
{
   if (resultsDispObjMgr->DisplayGrid() != display)
      {
      resultsDispObjMgr->DisplayGrid(display);
      Invalidate();
      }

   return true;
}

void TBridgeResultsView::CmSelectLoading ()
{
   TSelLoadingDlg dlg(this);
   SelLoadingXferBuffer xfer(GetApplication());
   DiagramCatagory dc;

   for (dc = dcDC; dc <= dcFatigue; dc++)
      {
      xfer.DrawDiagram(dc,resultsDispObjMgr->DrawDiagram(dc));
      }

   dlg.SetTransferBuffer(&xfer);

   if (dlg.Execute() == IDOK)
      {
      for (dc = dcDC; dc <= dcFatigue; dc++)
         {
         resultsDispObjMgr->DrawDiagram(dc,xfer.DrawDiagram(dc));
         resultsDispObjMgr->UpdateDisplay();
         Invalidate();
         }
      }
}


void TBridgeResultsView::EvRButtonDown (uint /*modKeys*/, TPoint& point)
{
    TPopupMenu menu;

    ClientToScreen(point);

    menu.AppendMenu(MF_BYCOMMAND,CM_SELECTLOADING, "Select Loading...");
    menu.AppendMenu(MF_BYCOMMAND,CM_DISPLAYOPTIONS,"Display Options...");
    /* Export not supported
    menu.AppendMenu(MF_SEPARATOR);
    menu.AppendMenu(MF_BYCOMMAND,CM_EXPORT,        "Export...");
    */

    menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point,0,HWindow);
}


void TBridgeResultsView::Draw(TDC &dc,TRect &rect,bool bestFit)
{
    TBridgeView::Draw(dc,rect,bestFit);

    // INSERT>> Your code here.
    bool addNote = false;
    for (DiagramCatagory dcat = dcAll; dcat <= dcFatigueTruck; dcat++)
       {
       if (resultsDispObjMgr->DrawDiagram(dcat))
          {
          addNote = true;
          break;
          }
       }

    if (!addNote)
       return;

    TFont *font;
    TSize textExtent;
    TPoint p;
    string msg = "Live Loads are per Lane";

    font = CreateSimpleFont(dc,"Arial",10,FW_BOLD,false,false);
    dc.SelectObject(*font);

    textExtent = dc.GetTextExtent(msg.c_str(),msg.length());
    p.x = rect.right - textExtent.cx;
    p.y = rect.bottom - textExtent.cy;

    int oldMode = dc.SetBkMode(TRANSPARENT);

    dc.TextOut(p,msg.c_str(),msg.length());

    dc.RestoreFont();
    dc.SetBkMode(oldMode);

    delete font;
}

