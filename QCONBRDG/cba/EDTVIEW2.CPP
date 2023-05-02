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

#include <qconbrdg\editview.h>
#include <qconbrdg\viewev.h>
#include <qconbrdg\clipbord.h>
#include <qconbrdg\loadfact.h>
#include <qconbrdg\spandlg.h>
#include <qconbrdg\editsupp.h>
#include <qconbrdg\pastesp.h>
#include <qconbrdg\imdlg.h>
#include <qconbrdg\peddlg.h>
#include <qconbrdg\llgendlg.h>
#include <qconbrdg\stddload.h>
#include <qconbrdg\concload.h>
#include <qconbrdg\unifload.h>
#include <qconbrdg\qcbapp.rh>

#include <owl\checkbox.h>  // for BF_xxxx constants
#include <owl\menu.h>

void TBridgeEditorView::CmMakeCantilever ()
{
   bridgeDoc.Cantilever(true,IsFirstSpanSelected());
}


void TBridgeEditorView::CeMakeCantilever (TCommandEnabler &tce)
{
    // INSERT>> Your code here.
    bool enable;
    Span *firstSpan;
    Span *lastSpan;

    firstSpan = bridgeDoc.GetBridge().GetFirstSpan();
    lastSpan  = bridgeDoc.GetBridge().GetLastSpan();

    if (!editorDispObjMgr->IsSpanSelected())
       enable = false;
    else if (IsFirstSpanSelected() && !firstSpan->IsCantilevered())
       enable = true;
    else if (IsLastSpanSelected() && !lastSpan->IsCantilevered())
       enable = true;
    else
       enable = false;

    tce.Enable(enable);
}


void TBridgeEditorView::CmSupportCantilever ()
{
   bridgeDoc.Cantilever(false,IsFirstSpanSelected());
}


void TBridgeEditorView::CeSupportCantilever (TCommandEnabler &tce)
{
    // INSERT>> Your code here.
    bool enable;
    Span *firstSpan;
    Span *lastSpan;

    firstSpan = bridgeDoc.GetBridge().GetFirstSpan();
    lastSpan  = bridgeDoc.GetBridge().GetLastSpan();

    if (!editorDispObjMgr->IsSpanSelected())
       enable = false;
    else if (IsFirstSpanSelected() && firstSpan->IsCantilevered())
       enable = true;
    else if (IsLastSpanSelected()  && lastSpan->IsCantilevered())
       enable = true;
    else
       enable = false;

    tce.Enable(enable);

}

void TBridgeEditorView::CmAddSpan ()
{
    TAddSpanDlgXferBuffer xfer(editorDispObjMgr->IsSpanSelected(),bridgeDoc.GetUnits());
    TAddSpanDlg dlg(this,bridgeDoc.GetUnits());

    dlg.SetupTransferBuffer(&xfer);
    dlg.SetCaption("Add Span");

    if (dlg.Execute() == IDOK)
       {
       LRFDSpan *span = xfer.GetSpan();
       Support *support = xfer.GetSupport();

       switch(xfer.GetLocation())
         {
         case aslAtStart:
              bridgeDoc.AddSpanSupport(span,support,false);
              break;

         case aslAtEnd:
              bridgeDoc.AddSpanSupport(span,support,true);
              break;

         case aslAtCurrent:
              bridgeDoc.InsertSpanSupport(editorDispObjMgr->GetSelSpan(),
                                         (GetInsertionMode() == imAfter),
                                          span,
                                          support,
                                         (editorDispObjMgr->GetSelectionMode() == TEditorDisplayObjectManager::smRightSupport));
              break;

         default:
              #pragma warn -ccc
              CHECKX(false,"TBridgeEditorView::CmAddSpan() - Bad support location");
              #pragma warn .ccc
         }
       }
}


void TBridgeEditorView::CeAddSpan (TCommandEnabler &tce)
{
    bool isValidOperation;

    // This operation is invalid if
    // 1. There are no selected spans and both the first and last spans
    //    are cantilevered.
    // 2. The selected span is the first span, the first span is cantilevered,
    //    and the insertion mode is before.
    // 3. The selected span is the last span, the last span is cantilevered,
    //    an the insertion mode is after
    Span *firstSpan;
    Span *lastSpan;

    firstSpan = bridgeDoc.GetBridge().GetFirstSpan();
    lastSpan  = bridgeDoc.GetBridge().GetLastSpan();

    if (bridgeDoc.GetNumSpans() == 0)
       isValidOperation = true;
    else if (!editorDispObjMgr->IsSpanSelected() && firstSpan->IsCantilevered() && lastSpan->IsCantilevered())
       isValidOperation = false;
    else if (editorDispObjMgr->IsSpanSelected() && IsFirstSpanSelected() && firstSpan->IsCantilevered() && (GetInsertionMode() == imBefore))
       isValidOperation = false;
    else if (editorDispObjMgr->IsSpanSelected() && IsLastSpanSelected()  && lastSpan->IsCantilevered()  && (GetInsertionMode() == imAfter))
       isValidOperation = false;
    else
       isValidOperation = true;

    tce.Enable(isValidOperation);
}


void TBridgeEditorView::CmDelete ()
{
   DeleteCurrentSelection();
}

void TBridgeEditorView::CeDelete (TCommandEnabler &tce)
{
   if (editorDispObjMgr->IsSpanSelected() && bridgeDoc.GetNumSpans() == 1)
      tce.Enable(false);
   else
      tce.Enable(editorDispObjMgr->IsSomethingSelected());
}

void TBridgeEditorView::DeleteCurrentSelection()
{
   if (editorDispObjMgr->IsSpanSelected() || editorDispObjMgr->IsSupportSelected())
      DeleteSpanSupport();
   else if (editorDispObjMgr->IsLoadSelected())
      DeleteLoad();
   else
      return;
}

void TBridgeEditorView::DeleteSpanSupport()
{
   TRACE("Entering TBridgeEditorView::DeleteSpanSupport()");
   //
   // Remove the span from the bridge model
   //
   Span *span; // span to get selected when curSpan is deleted
   Span *curSpan; // currently selected span

   curSpan = editorDispObjMgr->GetSelSpan();
   span = curSpan->NextSpan() ? curSpan->NextSpan() : curSpan->PrevSpan();

   bool rightSupport = (editorDispObjMgr->GetSelectionMode() == TEditorDisplayObjectManager::smRightSupport);
   bridgeDoc.RemoveSpan(curSpan,rightSupport);

   editorDispObjMgr->SelectSpan(span);
   TRACE("Leaving TBridgeEditorView::DeleteSpanSupport()");
}

void TBridgeEditorView::DeleteLoad()
{
   bridgeDoc.RemoveLoad(editorDispObjMgr->GetSelLoad());
}


void TBridgeEditorView::CmClearAll ()
{
    if (MessageBox("The entire bridge model is about to be deleted.\nProceed?","Warning",MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
       {
       bridgeDoc.Clear();
       }
}


void TBridgeEditorView::CmCopy ()
{
    CopyCurrentSelection();
}

void TBridgeEditorView::CeCopy (TCommandEnabler &tce)
{
    tce.Enable(editorDispObjMgr->IsSomethingSelected());
}

void TBridgeEditorView::CopyCurrentSelection()
{
    TQConBridgeClipboard clipboard(HWindow);

    if (editorDispObjMgr->IsLoadSelected())
       {
       clipboard.Put(*(editorDispObjMgr->GetSelLoad()));
       }
    else
       {
       clipboard.Put(*(TYPESAFE_DOWNCAST(editorDispObjMgr->GetSelSpan(),LRFDSpan)),
                     *(editorDispObjMgr->GetSelSupport()),
                      (editorDispObjMgr->GetSelectionMode() == TEditorDisplayObjectManager::smRightSupport));
       }
}


void TBridgeEditorView::CmCut ()
{
   CopyCurrentSelection();
   DeleteCurrentSelection();
}


void TBridgeEditorView::CeCut (TCommandEnabler &tce)
{
   if (editorDispObjMgr->IsSpanSelected() && bridgeDoc.GetNumSpans() == 1)
      tce.Enable(false);
   else
      tce.Enable(editorDispObjMgr->IsSomethingSelected());
}


void TBridgeEditorView::CmPasteInsert ()
{
   LRFDSpan *newSpan;
   Support *newSupport;
   bool isRightSupport;
   BridgeLoad *load;
   int lcase;

   TQConBridgeClipboard clipboard(HWindow);

   if (clipboard.IsQCBSpanFormatAvailable())
      {
      clipboard.Get(&newSpan,&newSupport,isRightSupport);
      bridgeDoc.InsertSpanSupport(editorDispObjMgr->GetSelSpan(),
                                  (GetInsertionMode() == imAfter),
                                   newSpan,
                                   newSupport,
                                  (editorDispObjMgr->GetSelectionMode() == TEditorDisplayObjectManager::smRightSupport));
      }
   else if (clipboard.IsQCBLoadFormatAvailable())
      {
      clipboard.Get(&load,lcase);

      if (TestLoadPlacement(load,editorDispObjMgr->GetSelSpan()))
         {
         load->SetSpan(editorDispObjMgr->GetSelSpan());
         bridgeDoc.InsertLoad(load,lcase);
         }
      }
}


void TBridgeEditorView::CePasteInsert (TCommandEnabler &tce)
{
   TRACE("Entering TBridgeEditorView::CePasteInsert()");

    bool isValidOperation;
    TQConBridgeClipboard clipboard(HWindow);

    if (!editorDispObjMgr->IsSomethingSelected())
        isValidOperation = false;
    else if (clipboard.IsQCBSpanFormatAvailable())
       {
       // There is a span/support on the clipboard...

       // This is not a valid operation if...
       // 1. There is not a support on the clipboard
       // 2. The first span is selected and cantilevered, and
       //    the span on the clipboard is left supported, and the
       //    insertion mode is set to imBefore.
       // 3. The last span is selected and cantilevered, and
       //    the span on the clipboard is right supported, and
       //    the insertion mode is set to imAfter.
       // 4. A load is selected
       Span *firstSpan = bridgeDoc.GetBridge().GetFirstSpan();
       Span *lastSpan  = bridgeDoc.GetBridge().GetLastSpan();
       LRFDSpan *newSpan;
       Support *newSupport;
       bool isRightSupport;

       TQConBridgeClipboard clipboard(HWindow);
       clipboard.Get(&newSpan,&newSupport,isRightSupport);

       if (!newSupport)
          isValidOperation = false;
       else if (IsFirstSpanSelected()       &&
                firstSpan->IsCantilevered() &&
                !isRightSupport             &&
                GetInsertionMode() == TBridgeEditorView::imBefore)
          {
          isValidOperation = false;
          }
       else if (IsLastSpanSelected()       &&
                lastSpan->IsCantilevered() &&
                isRightSupport             &&
                GetInsertionMode() == TBridgeEditorView::imAfter)
          {
          isValidOperation = false;
          }
       else if (editorDispObjMgr->IsLoadSelected())
          {
          isValidOperation = false;
          }
       else
          {
          isValidOperation = true;
          }

       delete newSpan;
       delete newSupport;
       }
    else if (clipboard.IsQCBLoadFormatAvailable())
       {
       // There is a load on the clipboard... Insertion is valid if there
       // is a span selected

       isValidOperation =  editorDispObjMgr->IsSpanSelected() ? true : false;
       }
    else
       isValidOperation = false;

    tce.Enable(isValidOperation);

   TRACE("Leaving TBridgeEditorView::CePasteInsert()");
}


void TBridgeEditorView::CmPasteReplace ()
{
   if (TQConBridgeClipboard(HWindow).IsQCBSpanFormatAvailable())
      ReplaceSpan();
   else if (TQConBridgeClipboard(HWindow).IsQCBLoadFormatAvailable())
      ReplaceLoad();
}


void TBridgeEditorView::ReplaceSpan()
{
   LRFDSpan *newSpan;
   Support *newSupport;
   bool isRightSupport;
   SSNOTIFY ssNotify;

   TQConBridgeClipboard clipboard(HWindow);
   clipboard.Get(&newSpan,&newSupport,isRightSupport);
   bridgeDoc.ReplaceSpan(editorDispObjMgr->GetSelSpan(),newSpan);

   if (newSupport)
      {
      bridgeDoc.ReplaceSupport(editorDispObjMgr->GetSelSupport(),newSupport);
      }
}

void TBridgeEditorView::ReplaceLoad()
{
   BridgeLoad *newLoad;
   BridgeLoad *selLoad;
   Span *destSpan;
   int lcase;

   TQConBridgeClipboard(HWindow).Get(&newLoad,lcase);

   selLoad = editorDispObjMgr->GetSelLoad();
   destSpan = selLoad->GetSpan();

   if (TestLoadPlacement(newLoad,destSpan))
      bridgeDoc.ReplaceLoad(editorDispObjMgr->GetSelLoad(),newLoad,lcase);
}


void TBridgeEditorView::CePasteReplace (TCommandEnabler &tce)
{
    // Replacement is only available if the item on the clipboard is the
    // same type as the currently selected item.

    bool isValid;
    LRFDSpan *newSpan;
    Support *newSupport;
    bool isRightSupport;

    if (TQConBridgeClipboard(HWindow).IsQCBSpanFormatAvailable() &&
        (editorDispObjMgr->IsSpanSelected() || editorDispObjMgr->IsSupportSelected()))
       {
       TQConBridgeClipboard(HWindow).Get(&newSpan,&newSupport,isRightSupport);
       if (newSupport)
          isValid = true;
       else
          isValid = false;

       delete newSpan;
       delete newSupport;
       }
    else if (TQConBridgeClipboard(HWindow).IsQCBLoadFormatAvailable() &&
             editorDispObjMgr->IsLoadSelected())
       isValid = true;
    else
       isValid = false;

    tce.Enable(bridgeDoc.GetNumSpans() > 0 && isValid);
}


void TBridgeEditorView::CmPasteSpecial ()
{
    PasteSpecialXferBuffer xfer;
    LRFDSpan *newSpan;
    Support *newSupport;
    bool isRightSupport;
    TQConBridgeClipboard(HWindow).Get(&newSpan,&newSupport,isRightSupport);

    TPasteSpecialDlg dlg(this,!newSupport ? false : true);
    dlg.SetTransferBuffer(&xfer);
    if (dlg.Execute() == IDOK)
       {
       if (xfer.PasteSpan())
          {
          if (xfer.MirrorSpan())
             newSpan->Mirror();

          bridgeDoc.ReplaceSpan(editorDispObjMgr->GetSelSpan(),newSpan);
          }
       else
          {
          bridgeDoc.ReplaceSupport(editorDispObjMgr->GetSelSupport(),newSupport);
          }
       }
}


void TBridgeEditorView::CePasteSpecial (TCommandEnabler &tce)
{
    bool enable;

    if (TQConBridgeClipboard(HWindow).IsQCBSpanFormatAvailable() &&
        bridgeDoc.GetNumSpans() > 0 &&
        (editorDispObjMgr->IsSpanSelected() || editorDispObjMgr->IsSupportSelected()) )
       enable = true;
    else if (TQConBridgeClipboard(HWindow).IsQCBLoadFormatAvailable())
       enable = false;
    else
       enable = false;

    tce.Enable(enable);
}

bool TBridgeEditorView::TestLoadPlacement(BridgeLoad *load,Span *destSpan)
{
   ConcBridgeLoad *cload;
   UnifBridgeLoad *uload;
   bool error = false;
   switch(load->Type())
      {
      case blConc:
           cload = TYPESAFE_DOWNCAST(load,ConcBridgeLoad);
           if (!cload->IsFract() &&
              (cload->StartLocation() > destSpan->Length()) )
              error = true;
           break;

      case blUnif:
           uload = TYPESAFE_DOWNCAST(load,UnifBridgeLoad);
           if (!uload->IsFract() &&
               (uload->StartLocation() > destSpan->Length()) ||
               (uload->EndLocation()   > destSpan->Length()) )
              error = true;
           break;
      }

   if (error)
      {
      ostrstream os;
      os << "Error inserting load." << endl;
      os << "Load beyond end of span." << ends;

      MessageBox(os.str(),"Error",MB_OK | MB_ICONEXCLAMATION);

      delete[] os.str();
      }

   return !error;
}

void TBridgeEditorView::CmImpact ()
{
    TImpactDlg dlg(this);
    ImpactXferBuffer xfer;

    bridgeDoc.GetDynamicLoadAllowance(xfer.IMtruck,xfer.IMlane,xfer.IMfatigue);

    dlg.SetTransferBuffer(&xfer);
    if (dlg.Execute() == IDOK)
       {
       bridgeDoc.SetDynamicLoadAllowance(xfer.IMtruck,xfer.IMlane,xfer.IMfatigue);
       }
}


void TBridgeEditorView::CmPedestrian ()
{
   double xfer;
   xfer = bridgeDoc.GetPedestrianLoad();
   TPedestrianDlg dlg(this,bridgeDoc.GetUnits());

   dlg.SetTransferBuffer(&xfer);
   if (dlg.Execute() == IDOK)
      {
      bridgeDoc.SetPedestrianLoad(xfer);
      }
}


void TBridgeEditorView::CmLiveLoadGeneration ()
{
   TLiveLoadGenDlg dlg(this,bridgeDoc.GetUnits());
   LLGenXferBuffer xfer;

   bridgeDoc.GetLiveLoadGenParameters(xfer);
   dlg.SetupTransferBuffer(&xfer);

   if (dlg.Execute() == IDOK)
      {
      bridgeDoc.SetLiveLoadGenParameters(xfer);
      }
}


void TBridgeEditorView::CmStdDeadLoad ()
{
    StdDeadLoadXferBuffer xfer;
    TStdDeadLoadDlg dlg(this,bridgeDoc.GetUnits());

    bridgeDoc.GetStdDeadLoads(xfer);
    dlg.SetupTransferBuffer(&xfer);

    if (dlg.Execute() == IDOK)
       {
       bridgeDoc.SetStdDeadLoads(xfer);
       }
}


void TBridgeEditorView::CmEditLoad ()
{
   EditLoad(editorDispObjMgr->GetSelLoad());
}


void TBridgeEditorView::CeEditLoad (TCommandEnabler &tce)
{
   tce.Enable(editorDispObjMgr->IsLoadSelected());
}


void TBridgeEditorView::CmAddConcLoad ()
{
    ConcLoadXferBuffer xfer(0.0,false,0.0,false,true);
    Span *span = editorDispObjMgr->GetSelSpan();
    TConcLoadDlg dlg(this,bridgeDoc.GetUnits(),span->Length(),false);
    dlg.SetTransferBuffer(&xfer);

    if (dlg.Execute() == IDOK)
       {
       bridgeDoc.AddLoad(editorDispObjMgr->GetSelSpan(),&xfer);
       }
}


void TBridgeEditorView::CeAddConcLoad (TCommandEnabler &tce)
{
   tce.Enable(editorDispObjMgr->IsSpanSelected());
}


void TBridgeEditorView::CmAddUnifLoad ()
{
    UnifLoadXferBuffer xfer(0.0,0.0,false,0.0,false,true);
    Span *span = editorDispObjMgr->GetSelSpan();
    TUnifLoadDlg dlg(this,bridgeDoc.GetUnits(),span->Length(),false);
    dlg.SetTransferBuffer(&xfer);

    if (dlg.Execute() == IDOK)
       {
       bridgeDoc.AddLoad(editorDispObjMgr->GetSelSpan(),&xfer);
       }
}


void TBridgeEditorView::CeAddUnifLoad (TCommandEnabler &tce)
{
   tce.Enable(editorDispObjMgr->IsSpanSelected());
}



bool TBridgeEditorView::CanClose ()
{
   return (TWindow::CanClose() && bridgeDoc.CanClose());
}


bool TBridgeEditorView::VnPlotStyleChanged (DiagramCatagory dc)
{
   if (dc == dcDW || dc == dcDC)
      {
      Invalidate();
      UpdateWindow();
      }

   return true;
}

bool TBridgeEditorView::VnStdDeadLoadsChanged(void)
{
   editorDispObjMgr->UpdateLegend();
   Invalidate();
   UpdateWindow();

   return true;
}

bool TBridgeEditorView::VnPedestrianLoadChanged(void)
{
   editorDispObjMgr->UpdateLegend();
   Invalidate();
   UpdateWindow();

   return true;
}
