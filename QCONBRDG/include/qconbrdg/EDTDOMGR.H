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

#if !defined EDTDOMGR_H
#define EDTDOMGR_H

#include <qconbrdg\bdomgr.h>

class TBridgeEditorView;

class TEditorDisplayObjectManager : public TBridgeDisplayObjectManager
{
public:
   TEditorDisplayObjectManager(TBridgeEditorView *view);
   ~TEditorDisplayObjectManager();

   void SelectNext();
   void SelectPrev();
   bool IsSomethingSelected() const;
   bool IsSpanSelected() const;
   bool IsSupportSelected() const;
   bool IsLoadSelected() const;

   enum SelectionMode {smLeftSupport,smRightSupport};

   void SetSelectionMode(SelectionMode mode);
   SelectionMode GetSelectionMode() const;

   bool Select(TPoint &point);
   TDisplayObject *Find(TPoint &point);

   bool SelectSpan(Span *span);
   bool SelectSupport(Support *support);
   bool SelectLoad(BridgeLoad *load);

   Span *GetSelSpan() const;
   Support *GetSelSupport() const;
   BridgeLoad *GetSelLoad() const;

   TSelSpanDisplayObject *GetSelSpanDisplayObject() const;
   TSelSupportDisplayObject *GetSelSupportDisplayObject() const;
   TSelLoadDisplayObject *GetSelLoadDisplayObject() const;

   void RemoveSpan(Span *span);
   void RemoveSupport(Support *support);
   void RemoveLoad(BridgeLoad *load);

   void UpdateLegend();

protected:
   TSelSpanDisplayObject *selSpanDisplayObject;
   TSelSupportDisplayObject *selSupportDisplayObject;
   TSelLoadDisplayObject *selLoadDisplayObject;

   SelectionMode selectionMode;

   void SelectSpan(TSelSpanDisplayObject *spanDispObj);
   void SelectSupport(TSelSupportDisplayObject *supportDispObj);
   void SelectLoad(TSelLoadDisplayObject *loadDispObj);
   void ClearSelection();

   void Clear();

   void SetupLegend();
};

#endif
