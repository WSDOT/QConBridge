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

//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(TBridgeEditorView, TBridgeView)
//{{TBridgeEditorViewRSP_TBL_BEGIN}}
    EV_WM_LBUTTONDOWN,
    EV_WM_LBUTTONDBLCLK,
    EV_WM_RBUTTONDOWN,
    EV_WM_KEYDOWN,
    EV_COMMAND(CM_SELECTLEFT,               CmSelectLeft),
    EV_COMMAND_ENABLE(CM_SELECTLEFT,        CeSelectLeft),
    EV_COMMAND(CM_SELECTRIGHT,              CmSelectRight),
    EV_COMMAND_ENABLE(CM_SELECTRIGHT,       CeSelectRight),
    EV_COMMAND(CM_INSERTBEFORE,             CmInsertBefore),
    EV_COMMAND_ENABLE(CM_INSERTBEFORE,      CeInsertBefore),
    EV_COMMAND(CM_INSERTAFTER,              CmInsertAfter),
    EV_COMMAND_ENABLE(CM_INSERTAFTER,       CeInsertAfter),
    EV_COMMAND(CM_RESET_TO_DEFAULTS,        CmResetToDefaults),
    EV_COMMAND(CM_LOADFACTORS,              CmLoadFactors),
    EV_COMMAND(CM_EDITSPAN,                 CmEditSpan),
    EV_COMMAND_ENABLE(CM_EDITSPAN,          CeEditSpan),
    EV_COMMAND(CM_EDITSUPPORT,              CmEditSupport),
    EV_COMMAND_ENABLE(CM_EDITSUPPORT,       CeEditSupport),
    EV_COMMAND(CM_MAKECANTILEVER,           CmMakeCantilever),
    EV_COMMAND_ENABLE(CM_MAKECANTILEVER,    CeMakeCantilever),
    EV_COMMAND(CM_SUPPORTCANTILEVER,        CmSupportCantilever),
    EV_COMMAND_ENABLE(CM_SUPPORTCANTILEVER, CeSupportCantilever),
    EV_COMMAND(CM_ADDSPAN,                  CmAddSpan),
    EV_COMMAND_ENABLE(CM_ADDSPAN,           CeAddSpan),
    EV_COMMAND(CM_EDITDELETE,               CmDelete),
    EV_COMMAND_ENABLE(CM_EDITDELETE,        CeDelete),
    EV_COMMAND(CM_EDITCLEAR,                CmClearAll),
    EV_COMMAND(CM_EDITCOPY,                 CmCopy),
    EV_COMMAND_ENABLE(CM_EDITCOPY,          CeCopy),
    EV_COMMAND(CM_EDITCUT,                  CmCut),
    EV_COMMAND_ENABLE(CM_EDITCUT,           CeCut),
    EV_COMMAND(CM_PASTEINSERT,              CmPasteInsert),
    EV_COMMAND_ENABLE(CM_PASTEINSERT,       CePasteInsert),
    EV_COMMAND(CM_PASTEREPLACE,             CmPasteReplace),
    EV_COMMAND_ENABLE(CM_PASTEREPLACE,      CePasteReplace),
    EV_COMMAND(CM_PASTESPECIAL,             CmPasteSpecial),
    EV_COMMAND_ENABLE(CM_PASTESPECIAL,      CePasteSpecial),
    EV_COMMAND(CM_IMPACT,                   CmImpact),
    EV_COMMAND(CM_PEDESTRIAN,               CmPedestrian),
    EV_COMMAND(CM_LLGENERATION,             CmLiveLoadGeneration),
    EV_COMMAND(CM_STANDARD_DEADLOAD,        CmStdDeadLoad),
    EV_COMMAND(CM_EDITLOAD,                 CmEditLoad),
    EV_COMMAND_ENABLE(CM_EDITLOAD,          CeEditLoad),
    EV_COMMAND(CM_CONCENTRATED_LOAD,        CmAddConcLoad),
    EV_COMMAND_ENABLE(CM_CONCENTRATED_LOAD, CeAddConcLoad),
    EV_COMMAND(CM_UNIFORM_LOAD,             CmAddUnifLoad),
    EV_COMMAND_ENABLE(CM_UNIFORM_LOAD,      CeAddUnifLoad),
    EV_VN_PLOTSTYLECHANGED,
    EV_VN_STDDEADLOADSCHANGED,
    EV_VN_PEDESTRIANLOADCHANGED,
//{{TBridgeEditorViewRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TBridgeEditorView Implementation}}

int TBridgeEditorView::count = 0;

TBridgeEditorView::TBridgeEditorView (TBridgeDocument& doc,TWindow *parent):
    TBridgeView(doc,parent)
{
    // INSERT>> Your constructor code here.
   count++;
   editorDispObjMgr = new TEditorDisplayObjectManager(this);
   bridgeDispObjMgr = editorDispObjMgr;

   insertionMode = imAfter;

   CreateDisplayObjects();
}


TBridgeEditorView::~TBridgeEditorView ()
{
    // INSERT>> Your destructor code here.
    count--;
    delete editorDispObjMgr;
}


const char far* TBridgeEditorView::GetViewName ()
{
    return StaticName();
}

TResId TBridgeEditorView::GetIconResourceId() const
{
   return IDI_EDITORVIEW;
}


TSpanDisplayObject *TBridgeEditorView::CreateSpanDisplayObject(Span *span)
{
   return new TSelSpanDisplayObject(editorDispObjMgr,span);
}

TSupportDisplayObject *TBridgeEditorView::CreateSupportDisplayObject(Support *support)
{
   return new TSelSupportDisplayObject(editorDispObjMgr,support);
}

TLoadDisplayObject *TBridgeEditorView::CreateLoadDisplayObject(BridgeLoad *load)
{
   return new TSelLoadDisplayObject(editorDispObjMgr,load);
}

void TBridgeEditorView::EvLButtonDown (uint /*modKeys*/, TPoint& point)
{
    editorDispObjMgr->Select(point);
}


void TBridgeEditorView::EvLButtonDblClk (uint /*modKeys*/, TPoint& point)
{
   TDisplayObject        *dispObj;
   TSpanDisplayObject    *spanDispObj;
   TSupportDisplayObject *supportDispObj;
   TLoadDisplayObject    *loadDispObj;

   dispObj = editorDispObjMgr->Find(point);

   // Nothing was found, get the heck outta here
   if (dispObj == NULL)
      return;

   switch(dispObj->ObjectType())
      {
      case otSpan:
           spanDispObj = TYPESAFE_DOWNCAST(dispObj,TSpanDisplayObject);
           EditSpan(TYPESAFE_DOWNCAST(spanDispObj->GetSpan(),LRFDSpan));
           break;

      case otSupport:
           supportDispObj = TYPESAFE_DOWNCAST(dispObj,TSupportDisplayObject);
           EditSupport(supportDispObj->GetSupport());
           break;

      case otLoad:
           loadDispObj = TYPESAFE_DOWNCAST(dispObj,TLoadDisplayObject);
           EditLoad(loadDispObj->GetLoad());
           break;
      }
}


void TBridgeEditorView::EvRButtonDown (uint modKeys, TPoint& point)
{
    TWindow::EvRButtonDown(modKeys, point);

    // INSERT>> Your code here.

    TPopupMenu menu;
    TMenu pasteMenu;

    ClientToScreen(point);

    pasteMenu.AppendMenu(MF_BYCOMMAND,CM_PASTEINSERT,"Paste Insert");
    pasteMenu.AppendMenu(MF_BYCOMMAND,CM_PASTEREPLACE,"Paste Replace");
    pasteMenu.AppendMenu(MF_BYCOMMAND,CM_PASTESPECIAL,"Paste Special...");

    menu.AppendMenu(MF_BYCOMMAND,CM_EDITCUT,"Cut");
    menu.AppendMenu(MF_BYCOMMAND,CM_EDITCOPY,"Copy");
    menu.AppendMenu(MF_POPUP,pasteMenu,"Paste");
    menu.AppendMenu(MF_BYCOMMAND,CM_EDITDELETE,"Delete");
    menu.AppendMenu(MF_SEPARATOR);
    menu.AppendMenu(MF_BYCOMMAND,CM_ADDSPAN,"Add Span/Support...");
    menu.AppendMenu(MF_BYCOMMAND,CM_EDITSPAN,"Edit Span...");
    menu.AppendMenu(MF_BYCOMMAND,CM_EDITSUPPORT,"Edit Support...");
    menu.AppendMenu(MF_SEPARATOR);
    menu.AppendMenu(MF_BYCOMMAND,CM_STANDARD_DEADLOAD,"Standard Dead Loads...");
    menu.AppendMenu(MF_BYCOMMAND,CM_CONCENTRATED_LOAD,"Add Concentrated Load...");
    menu.AppendMenu(MF_BYCOMMAND,CM_UNIFORM_LOAD,"Add Uniform Load...");
    menu.AppendMenu(MF_BYCOMMAND,CM_EDITLOAD,"Edit Load...");
    /* Export not supported
    menu.AppendMenu(MF_SEPARATOR);
    menu.AppendMenu(MF_BYCOMMAND,CM_EXPORT,"Export...");
    */
    
    menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point,0,HWindow);
}

void TBridgeEditorView::CmSelectLeft ()
{
   editorDispObjMgr->SetSelectionMode(TEditorDisplayObjectManager::smLeftSupport);
}


void TBridgeEditorView::CeSelectLeft (TCommandEnabler &tce)
{
    tce.SetCheck( (editorDispObjMgr->GetSelectionMode() == TEditorDisplayObjectManager::smLeftSupport) ? BF_CHECKED : BF_UNCHECKED );
}


void TBridgeEditorView::CmSelectRight ()
{
   editorDispObjMgr->SetSelectionMode(TEditorDisplayObjectManager::smRightSupport);
}


void TBridgeEditorView::CeSelectRight (TCommandEnabler &tce)
{
    tce.SetCheck( (editorDispObjMgr->GetSelectionMode() == TEditorDisplayObjectManager::smRightSupport) ? BF_CHECKED : BF_UNCHECKED );
}

void TBridgeEditorView::SetInsertionMode(InsertionMode mode)
{
   insertionMode = mode;
}

TBridgeEditorView::InsertionMode TBridgeEditorView::GetInsertionMode() const
{
   return insertionMode;
}


void TBridgeEditorView::CmInsertBefore ()
{
   SetInsertionMode(imBefore);
}


void TBridgeEditorView::CeInsertBefore (TCommandEnabler &tce)
{
    tce.SetCheck( (GetInsertionMode() == imBefore) ? BF_CHECKED : BF_UNCHECKED );
}


void TBridgeEditorView::CmInsertAfter ()
{
   SetInsertionMode(imAfter);
}


void TBridgeEditorView::CeInsertAfter (TCommandEnabler &tce)
{
    tce.SetCheck( (GetInsertionMode() == imAfter) ? BF_CHECKED : BF_UNCHECKED );
}

void TBridgeEditorView::EvKeyDown (uint key, uint repeatCount, uint flags)
{
    TWindow::EvKeyDown(key, repeatCount, flags);

    bool ctrl  = (GetKeyState(VK_CONTROL) < 0) ? true : false;
    bool shift = (GetKeyState(VK_SHIFT)   < 0) ? true : false;

    switch(key)
       {
       case VK_RETURN: VkEnter(ctrl,shift,repeatCount);   break;
       case VK_LEFT:   VkLeft (ctrl,shift,repeatCount);   break;
       case VK_RIGHT:  VkRight(ctrl,shift,repeatCount);   break;
       }
}

void TBridgeEditorView::VkEnter(bool ctrl,bool /*shift*/, uint /*repeatCount*/)
{
   if (!editorDispObjMgr->IsSomethingSelected())
      return;

   if (editorDispObjMgr->IsLoadSelected())
      {
      EditLoad(editorDispObjMgr->GetSelLoad());
      return;
      }

   if (ctrl)
      {
      if (!editorDispObjMgr->IsSupportSelected())
         return;

      Support *support;
      support = editorDispObjMgr->GetSelSupport();
      EditSupport(support);
      }
   else
      {
      if (!editorDispObjMgr->IsSpanSelected())
         return;

      LRFDSpan *span;
      span = TYPESAFE_DOWNCAST(editorDispObjMgr->GetSelSpan(),LRFDSpan);
      EditSpan(span);
      }
}

void TBridgeEditorView::VkLeft(bool /*ctrl*/,bool /*shift*/,uint repeatCount)
{
   for (int i = 0; i < repeatCount; i++)
      editorDispObjMgr->SelectPrev();
}

void TBridgeEditorView::VkRight(bool /*ctrl*/,bool /*shift*/,uint repeatCount)
{
   for (int i = 0; i < repeatCount; i++)
      editorDispObjMgr->SelectNext();
}


void TBridgeEditorView::CmResetToDefaults ()
{
    bridgeDoc.ResetToDefaults();
    bridgeDoc.SetDirty(true);
    bridgeDoc.SetAnalyzed(false);
}


void TBridgeEditorView::CmLoadFactors ()
{
	TLoadFactorDlg dlg(this);
   LoadFactorXferBuffer xfer;

   bridgeDoc.GetLoadFactors(xfer);
   dlg.SetupTransferBuffer(&xfer);

   if (dlg.Execute() == IDOK)
      {
      bridgeDoc.SetLoadFactors(xfer);
      }
}


void TBridgeEditorView::CmEditSpan ()
{
   LRFDSpan *span;
   span = TYPESAFE_DOWNCAST(editorDispObjMgr->GetSelSpan(),LRFDSpan);
   EditSpan(span);
}


void TBridgeEditorView::CeEditSpan (TCommandEnabler &tce)
{
   tce.Enable(editorDispObjMgr->IsSpanSelected());
}


void TBridgeEditorView::CmEditSupport ()
{
   Support *support = editorDispObjMgr->GetSelSupport();
   EditSupport(support);
}


void TBridgeEditorView::CeEditSupport (TCommandEnabler &tce)
{
   tce.Enable(editorDispObjMgr->IsSupportSelected());
}

void TBridgeEditorView::EditSpan(LRFDSpan *span)
{
   PRECONDITIONX(span != NULL || span->GetID() > 0,"TBridgeEditorView::EditSpan() - Invalid span");

   TEditSpanDlgXferBuffer xfer(span);
   TEditSpanDlg dlg(this,bridgeDoc.GetUnits());

   ostrstream os;
   os << "Edit Span - Span " << span->GetID() << ends;
   dlg.SetCaption(os.str());
   delete[] os.str();

   dlg.SetupTransferBuffer(&xfer);
   if( dlg.Execute() == IDOK )
      {
      LRFDSpan *newSpan = xfer.GetSpan();
      bridgeDoc.ReplaceSpan(span,newSpan);
      }
}

void TBridgeEditorView::EditSupport(Support *support)
{
    EditSupportXferBuffer xfer(*support);

    TEditSupportDlg dlg(this,bridgeDoc.GetUnits());

    ostrstream os;
    os << "Edit Support - Pier " << support->GetID() << ends;
    dlg.SetCaption(os.str());
    delete[] os.str();

    dlg.SetupTransferBuffer(&xfer);
    if (dlg.Execute() == IDOK)
       {
       Support *newSupport = xfer.GetSupport();
       bridgeDoc.ReplaceSupport(support,newSupport);
       }
}

void TBridgeEditorView::EditLoad(BridgeLoad *load)
{
   if (load->Type() == blConc)
      EditConcLoad(TYPESAFE_DOWNCAST(load,ConcBridgeLoad));
   else
      EditUnifLoad(TYPESAFE_DOWNCAST(load,UnifBridgeLoad));
}

void TBridgeEditorView::EditConcLoad(ConcBridgeLoad *cload)
{
   BridgeLoading *loading = cload->GetOwner();
   Span *span = cload->GetSpan();

   ConcLoadXferBuffer xfer(cload->StartLocation(),cload->IsFract(),cload->Magnitude(),false,loading->GetID() == DC_LOAD);
   TConcLoadDlg dlg(this,bridgeDoc.GetUnits(),span->Length(),true);
   dlg.SetTransferBuffer(&xfer);

   if (dlg.Execute() == IDOK)
      {
      bridgeDoc.ReplaceLoad(cload,&xfer);
      }
}

void TBridgeEditorView::EditUnifLoad(UnifBridgeLoad *uload)
{
   BridgeLoading *loading = uload->GetOwner();
   Span *span = uload->GetSpan();

   UnifLoadXferBuffer xfer(uload->StartLocation(),uload->EndLocation(),uload->IsFract(),uload->Magnitude(),false,loading->GetID() == DC_LOAD);
   TUnifLoadDlg dlg(this,bridgeDoc.GetUnits(),span->Length(),true);
   dlg.SetTransferBuffer(&xfer);

   if (dlg.Execute() == IDOK)
      {
      bridgeDoc.ReplaceLoad(uload,&xfer);
      }
}

bool TBridgeEditorView::IsFirstSpanSelected()
{
    Span *firstSpan;

    if (!editorDispObjMgr->IsSpanSelected())
       return false;

    firstSpan = bridgeDoc.GetBridge().GetFirstSpan();
    return (firstSpan == editorDispObjMgr->GetSelSpan());
}

bool TBridgeEditorView::IsLastSpanSelected()
{
    Span *lastSpan;

    if (!editorDispObjMgr->IsSpanSelected())
       return false;

    lastSpan = bridgeDoc.GetBridge().GetLastSpan();
    return (lastSpan == editorDispObjMgr->GetSelSpan());
}

