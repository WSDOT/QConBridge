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

#if !defined(__selload_h)              // Sentry, use file only if it's not already included.
#define __selload_h


#include <owl\dialog.h>
#include <owl\listbox.h>
#include <bridge\lrfdbrdg.h> // For DiagramCatagory
#include <qconbrdg\qcbapp.rh>            // Definition of all resources.

struct SelLoadingXferBuffer
{
   SelLoadingXferBuffer(TModule *module);
   TListBoxData lbData;

   void DrawDiagram(DiagramCatagory dc,bool draw);
   bool DrawDiagram(DiagramCatagory dc);
};


//{{TDialog = TSelLoadingDlg}}
class TSelLoadingDlg : public TDialog {
public:
    TSelLoadingDlg (TWindow* parent, TResId resId = IDD_SELLOADING, TModule* module = 0);
    virtual ~TSelLoadingDlg ();

protected:
    TListBox *lbox;

//{{TSelLoadingDlgRSP_TBL_BEGIN}}
protected:
    void BNClicked_MarkAll ();
    void BNClicked_ClearAll ();
    void BNClicked_Help ();
//{{TSelLoadingDlgRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSelLoadingDlg);
};    //{{TSelLoadingDlg}}


#endif                                      // __selload_h sentry.

