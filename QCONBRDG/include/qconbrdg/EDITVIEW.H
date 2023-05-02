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

#if !defined(__editview_h)              // Sentry, use file only if it's not already included.
#define __editview_h


#include <qconbrdg\bview.h>
#include <qconbrdg\edtdomgr.h>


//{{TWindowView = TBridgeEditorView}}
class TBridgeEditorView : public TBridgeView
{
public:
    TBridgeEditorView (TBridgeDocument& doc,TWindow *parent = 0);
    virtual ~TBridgeEditorView ();

    static const char far* StaticName() { return "Editor"; }

    TResId GetIconResourceId() const;

    TEditorDisplayObjectManager &GetEditorDisplayObjectManager() {return *editorDispObjMgr;}

    static int count;

//{{TBridgeEditorViewVIRTUAL_BEGIN}}
protected:
    virtual const char far* GetViewName ();
    virtual bool CanClose ();
//{{TBridgeEditorViewVIRTUAL_END}}

protected:
   TEditorDisplayObjectManager *editorDispObjMgr;

   TSpanDisplayObject *CreateSpanDisplayObject(Span *span);
   TSupportDisplayObject *CreateSupportDisplayObject(Support *support);
   TLoadDisplayObject *CreateLoadDisplayObject(BridgeLoad *load);

   void VkEnter(bool ctrl,bool shift, uint repeatCount);
   void VkLeft (bool ctrl,bool shift, uint repeatCount);
   void VkRight(bool ctrl,bool shift, uint repeatCount);

   enum InsertionMode {imBefore,imAfter};
   InsertionMode insertionMode;

   void SetInsertionMode(InsertionMode mode);
   InsertionMode GetInsertionMode() const;

   void EditSpan(LRFDSpan *span);
   void EditSupport(Support *support);
   void EditLoad(BridgeLoad *load);
   void EditConcLoad(ConcBridgeLoad *cload);
   void EditUnifLoad(UnifBridgeLoad *uload);

   bool IsFirstSpanSelected();
   bool IsLastSpanSelected();

   void DeleteCurrentSelection();
   void DeleteSpanSupport();
   void DeleteLoad();

   void CopyCurrentSelection();

   void ReplaceSpan();
   void ReplaceLoad();

   bool TestLoadPlacement(BridgeLoad *load,Span *destSpan);

//{{TBridgeEditorViewRSP_TBL_BEGIN}}
protected:
    void EvLButtonDown   (uint modKeys, TPoint& point);
    void EvLButtonDblClk (uint modKeys, TPoint& point);
    void EvRButtonDown   (uint modKeys, TPoint& point);
    void CmSelectLeft ();
    void CeSelectLeft (TCommandEnabler &tce);
    void CmSelectRight ();
    void CeSelectRight (TCommandEnabler &tce);
    void CmInsertBefore ();
    void CeInsertBefore (TCommandEnabler &tce);
    void CmInsertAfter ();
    void CeInsertAfter (TCommandEnabler &tce);
    void EvKeyDown (uint key, uint repeatCount, uint flags);
    void CmResetToDefaults ();
    void CmLoadFactors ();
    void CmEditSpan ();
    void CeEditSpan (TCommandEnabler &tce);
    void CmEditSupport ();
    void CeEditSupport (TCommandEnabler &tce);
    void CmMakeCantilever ();
    void CeMakeCantilever (TCommandEnabler &tce);
    void CmSupportCantilever ();
    void CeSupportCantilever (TCommandEnabler &tce);
    void CmAddSpan ();
    void CeAddSpan (TCommandEnabler &tce);
    void CmDelete ();
    void CeDelete (TCommandEnabler &tce);
    void CmClearAll ();
    void CmCopy ();
    void CeCopy (TCommandEnabler &tce);
    void CmCut ();
    void CeCut (TCommandEnabler &tce);
    void CmPasteInsert ();
    void CePasteInsert (TCommandEnabler &tce);
    void CmPasteReplace ();
    void CePasteReplace (TCommandEnabler &tce);
    void CmPasteSpecial ();
    void CePasteSpecial (TCommandEnabler &tce);
    void CmImpact ();
    void CmPedestrian ();
    void CmLiveLoadGeneration ();
    void CmStdDeadLoad ();
    void CmEditLoad ();
    void CeEditLoad (TCommandEnabler &tce);
    void CmAddConcLoad ();
    void CeAddConcLoad (TCommandEnabler &tce);
    void CmAddUnifLoad ();
    void CeAddUnifLoad (TCommandEnabler &tce);
    bool VnPlotStyleChanged (DiagramCatagory dc);
    bool VnStdDeadLoadsChanged ();
    bool VnPedestrianLoadChanged ();
//{{TBridgeEditorViewRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TBridgeEditorView);
};    //{{TBridgeEditorView}}


#endif                                      // __editview_h sentry.

