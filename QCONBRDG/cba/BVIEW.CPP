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


#include <qconbrdg\bview.h>
#include <qconbrdg\viewev.h>
#include <qconbrdg\dispopt.h>
#include <qconbrdg\qcbapp.h>
#include <qconbrdg\vwprtout.h>
#include <bridge\lrfdbrdg.h> // for DiagramCatagory
#include <owl\docmanag.h>
#include <owl\opensave.h>
#include <dir.h> // for MAXPATH

//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE2(TBridgeView,TQConBridgeView,TWindow)
//{{TBridgeViewRSP_TBL_BEGIN}}
    EV_WM_SIZE,
    EV_WM_DISPLAYCHANGE,
    EV_VN_ISWINDOW,
    EV_VN_DISPLAYDIMENSIONS,
    EV_VN_DISPLAYLABELS,
    EV_VN_DISPLAYSEGMENTTICKS,
    EV_VN_ANALYZED,
    EV_VN_REBUILDDISPLAY,
    EV_VN_CLEARALL,
    EV_VN_REMOVESPAN,
    EV_VN_REPLACESPAN,
    EV_VN_REMOVESUPPORT,
    EV_VN_REPLACESUPPORT,
    EV_VN_REMOVELOAD,
    EV_VN_ADDLOAD,
    EV_VN_ADDSPANSUPPORT,
    EV_VN_ADDSUPPORT,
    EV_VN_ADDSPAN,
    EV_VN_REMOVESUPPORT,
    EV_VN_UNITSCHANGED,
    EV_COMMAND(CM_DISPLAYOPTIONS, CmDisplayOptions),
//    EV_COMMAND(CM_EXPORT,CmExport), // Export not supported
//{{TBridgeViewRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TBridgeView Implementation}}


TBridgeView::TBridgeView (TBridgeDocument& doc,TWindow *parent):
    TQConBridgeView(doc),
    TWindow(parent)
{
    // INSERT>> Your constructor code here.
   bridgeDispObjMgr = NULL;
}


TBridgeView::~TBridgeView ()
{
    // INSERT>> Your destructor code here.

}

TBridgeDisplayObjectManager *TBridgeView::GetDisplayObjectManager()
{
   return bridgeDispObjMgr;
}

TPrintout *TBridgeView::GetPrintout()
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   TPrintout *printout;

   printout = new TViewPrintout (theApp->GetPrinter(),this,GetViewName());

   return printout;
}

void TBridgeView::SetupWindow()
{
   TWindow::SetupWindow();
   bridgeDispObjMgr->DisplayDimensions(bridgeDoc.DisplayDimensions());
   bridgeDispObjMgr->DisplayLabels(bridgeDoc.DisplayLabels());
   bridgeDispObjMgr->DisplaySegmentTicks(bridgeDoc.DisplaySegmentTicks());
}

TWindow* TBridgeView::GetWindow ()
{
   return (TWindow *)this;
}


bool TBridgeView::SetDocTitle (const char far* /*docname*/, int /*index*/)
{
   return TWindow::SetDocTitle(GetViewName(),0);
}

void TBridgeView::CreateDisplayObjects()
{
   Span *span;
   Support *support;

   LRFDBridge &bridge = bridgeDoc.GetBridge();

   for (span = bridge.GetFirstSpan(); span; span = span->NextSpan())
      {
      bridgeDispObjMgr->AddSpanDisplayObject(CreateSpanDisplayObject(span));
      }

   for (support = bridge.GetFirstSupport(); support; support = support->NextSupport())
      {
      bridgeDispObjMgr->AddSupportDisplayObject(CreateSupportDisplayObject(support));
      }

   //
   // Loads
   //
   BridgeLoading *loading;

   // DC Load
   loading = bridgeDoc.GetBridge().GetLoading(DC_LOAD);
   BridgeLoadIterator *i = loading->GetIterator();
   while(*i)
      {
      BridgeLoad *load = (*i)++;
      bridgeDispObjMgr->AddLoadDisplayObject(CreateLoadDisplayObject(load));
      }
   delete i;

   // DW Load
   loading = bridgeDoc.GetBridge().GetLoading(DW_LOAD);
   BridgeLoadIterator *j = loading->GetIterator();
   while(*j)
      {
      BridgeLoad *load = (*j)++;
      bridgeDispObjMgr->AddLoadDisplayObject(CreateLoadDisplayObject(load));
      }
   delete j;
}

TSpanDisplayObject *TBridgeView::CreateSpanDisplayObject(Span *span)
{
   return new TSpanDisplayObject(bridgeDispObjMgr,span);
}

TSupportDisplayObject *TBridgeView::CreateSupportDisplayObject(Support *support)
{
   return new TSupportDisplayObject(bridgeDispObjMgr,support);
}

TLoadDisplayObject *TBridgeView::CreateLoadDisplayObject(BridgeLoad *load)
{
   return new TLoadDisplayObject(bridgeDispObjMgr,load);
}

void TBridgeView::Paint (TDC& dc, bool erase, TRect& rect)
{
    TWindow::Paint(dc, erase, rect);

    // INSERT>> Your code here.

    // Don't refresh the screen if we are printing or if there aren't any spans
    TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(bridgeDoc.GetDocManager().GetApplication(),TQConBridgeApp);
    if (!theApp->Printing && bridgeDoc.GetNumSpans() > 0)
       Draw(dc,GetClientRect());
}

void TBridgeView::Draw(TDC &dc,TRect &rect,bool bestFit)
{
    bridgeDispObjMgr->Draw(dc,rect,bestFit);
}


void TBridgeView::EvSize (uint sizeType, TSize& size)
{
    TWindow::EvSize(sizeType, size);

    // INSERT>> Your code here.
    bridgeDispObjMgr->UpdateDisplay();
    Invalidate();
}

void TBridgeView::EvDisplayChange(bool, int, int)
{
    bridgeDispObjMgr->UpdateDisplay();
    Invalidate();
}

void TBridgeView::CmDisplayOptions ()
{
    // INSERT>> Your code here.
   TDisplayOptionsDlg  dlg(this);
   DisplayOptionsXferBuffer xfer;
   DiagramCatagory dc;

   xfer.decorations.DrawLabels(bridgeDoc.DisplayLabels());
   xfer.decorations.DrawTicks(bridgeDoc.DisplaySegmentTicks());
   xfer.decorations.DrawDimensions(bridgeDoc.DisplayDimensions());
   xfer.decorations.DrawGrid(bridgeDoc.DisplayGrid());
   xfer.decorations.Invert(bridgeDoc.InvertMomentDiagrams());

   for (dc = dcDC; dc <= dcFatigue; dc++)
      {
      xfer.screenPlotStyle[dc].SetColor(bridgeDoc.GetPlotColor(plScreen,dc));
      xfer.screenPlotStyle[dc].SetPenStyle(bridgeDoc.GetPenStyle(plScreen,dc));
      xfer.screenPlotStyle[dc].SetSymbol(bridgeDoc.GetPlotSymbolType(plScreen,dc));

      xfer.printerPlotStyle[dc].SetColor(bridgeDoc.GetPlotColor(plPrinter,dc));
      xfer.printerPlotStyle[dc].SetPenStyle(bridgeDoc.GetPenStyle(plPrinter,dc));
      xfer.printerPlotStyle[dc].SetSymbol(bridgeDoc.GetPlotSymbolType(plPrinter,dc));
      }

   dlg.SetupTransferBuffer(&xfer);

   if (dlg.Execute() == IDOK)
      {
      bridgeDoc.DisplayLabels(xfer.decorations.DrawLabels());
      bridgeDoc.DisplaySegmentTicks(xfer.decorations.DrawTicks());
      bridgeDoc.DisplayDimensions(xfer.decorations.DrawDimensions());
      bridgeDoc.DisplayGrid(xfer.decorations.DrawGrid());
      bridgeDoc.InvertMomentDiagrams(xfer.decorations.Invert());

      for (dc = dcDC; dc <= dcFatigue; dc++)
         {
         bridgeDoc.SetPlotColor(plScreen,dc,xfer.screenPlotStyle[dc].GetColor());
         bridgeDoc.SetPenStyle(plScreen,dc,xfer.screenPlotStyle[dc].GetPenStyle());
         bridgeDoc.SetPlotSymbolType(plScreen,dc,xfer.screenPlotStyle[dc].GetSymbol());

         bridgeDoc.SetPlotColor(plPrinter,dc,xfer.printerPlotStyle[dc].GetColor());
         bridgeDoc.SetPenStyle(plPrinter,dc,xfer.printerPlotStyle[dc].GetPenStyle());
         bridgeDoc.SetPlotSymbolType(plPrinter,dc,xfer.printerPlotStyle[dc].GetSymbol());
         }
      }
}

bool TBridgeView::VnDisplayDimensions(bool display)
{
   bridgeDispObjMgr->DisplayDimensions(display);
   Invalidate();
   return true;
}

bool TBridgeView::VnDisplayLabels(bool display)
{
   bridgeDispObjMgr->DisplayLabels(display);
   Invalidate();
   return true;
}

bool TBridgeView::VnDisplaySegmentTicks(bool display)
{
   bridgeDispObjMgr->DisplaySegmentTicks(display);
   Invalidate();
   return true;
}

bool TBridgeView::VnAnalyzed(bool /*analyzed*/)
{
   bridgeDispObjMgr->UpdateDisplay();
   Invalidate();
   return true;
}

bool TBridgeView::VnRebuildDisplay(void)
{
   Clear();
   CreateDisplayObjects();
   bridgeDispObjMgr->UpdateDisplay();
   return true;
}

bool TBridgeView::VnUnitsChanged(void)
{
   bridgeDispObjMgr->UpdateLegend();
   Invalidate();
   return true;
}

bool TBridgeView::VnClearAll(void)
{
   Clear();
   Invalidate();

   return true;
}

bool TBridgeView::VnRemoveSpan(long lParam)
{
   SSNOTIFY *ssNotify = (SSNOTIFY *)lParam;

   bridgeDispObjMgr->RemoveSpan(ssNotify->oldSpan);
   bridgeDispObjMgr->UpdateDisplay();
   Invalidate();

   return true;
}
bool TBridgeView::VnReplaceSpan(long lParam)
{
   SSNOTIFY *ssNotify = (SSNOTIFY *)lParam;

   bridgeDispObjMgr->ReplaceSpan(ssNotify->oldSpan,ssNotify->newSpan);
   bridgeDispObjMgr->UpdateDisplay();
   Invalidate();

   return true;
}

bool TBridgeView::VnReplaceSupport(long lParam)
{
   SSNOTIFY *ssNotify = (SSNOTIFY *)lParam;

   bridgeDispObjMgr->ReplaceSupport(ssNotify->oldSupport,ssNotify->newSupport);
   bridgeDispObjMgr->UpdateDisplay();
   Invalidate();

   return true;
}

bool TBridgeView::VnRemoveSupport(long lParam)
{
   SSNOTIFY *ssNotify = (SSNOTIFY *)lParam;

   bridgeDispObjMgr->RemoveSupport(ssNotify->oldSupport);
   bridgeDispObjMgr->UpdateDisplay();
   Invalidate();

   return true;
}

bool TBridgeView::VnRemoveLoad(long lParam)
{
   TRACE("Entering TBridgeView::VnRemoveLoad()");

   LDNOTIFY *ldNotify = (LDNOTIFY *)lParam;

   bridgeDispObjMgr->RemoveLoad(ldNotify->load);
   bridgeDispObjMgr->UpdateDisplay();
   Invalidate();

   TRACE("Leaving TBridgeView::VnRemoveLoad()");
   return true;
}

bool TBridgeView::VnAddLoad(long lParam)
{
   LDNOTIFY *ldNotify = (LDNOTIFY *)lParam;

   bridgeDispObjMgr->AddLoadDisplayObject(CreateLoadDisplayObject(ldNotify->load));
   bridgeDispObjMgr->UpdateDisplay();
   Invalidate();

   return true;
}

bool TBridgeView::VnAddSpanSupport(long lParam)
{
   VnAddSpan(lParam);
   VnAddSupport(lParam);

   return true;
}

bool TBridgeView::VnAddSpan(long lParam)
{
   SSNOTIFY *ssNotify = (SSNOTIFY *)lParam;

   bridgeDispObjMgr->AddSpanDisplayObject(CreateSpanDisplayObject(ssNotify->newSpan));
   bridgeDispObjMgr->UpdateDisplay();
   Invalidate();

   return true;
}

bool TBridgeView::VnAddSupport(long lParam)
{
   SSNOTIFY *ssNotify = (SSNOTIFY *)lParam;

   bridgeDispObjMgr->AddSupportDisplayObject(CreateSupportDisplayObject(ssNotify->newSupport));
   bridgeDispObjMgr->UpdateDisplay();
   Invalidate();

   return true;
}

void TBridgeView::Clear()
{
   bridgeDispObjMgr->Clear();
}


bool TBridgeView::VnIsWindow(HWND hWnd)
{
  return hWnd == HWindow || IsChild(hWnd);
}

void TBridgeView::CmExport ()
{
// Exporting graphics to a metafile is a really good idea, except that
// device context Getxxxx functions (i.e. GetDeviceCap(), GetTextMetrics(), etc)
// are not supported. This is a big deal because bridgeDispObjMgr->Draw()
// calls some of these functions (TLegend::Draw() calls them first and you get
// a divide by zero error).
//
// The changes required to support the metafile requirements are too extensive
// to make at this time.
//
// Win95 and WinNT apps support an enhanced metafile format which supports the
// Getxxxx functions. Exporting the bridge view to a metafile will be supported
// when we move to a 32bit OS. Enhanced metafiles are not supported by Win32s

//   MessageBox("This feature is not currently supported","Notice",MB_OK | MB_ICONEXCLAMATION);

/*
   string filter = GetApplication()->LoadString(IDS_METAFILEFILTER);
   TOpenSaveDialog::TData data(OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
                               filter.c_str(),
                               0,
                               "",
                               "WMF");

   TFileSaveDialog dlg(GetApplication()->GetMainWindow(),data);

   if (dlg.Execute() == IDOK)
      {
      TRACE("Drawing Metafile");
      TMetaFileDC dc(data.FileName);
      bridgeDispObjMgr->Draw(dc,GetClientRect(),false);
      }
*/
}
