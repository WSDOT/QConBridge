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

#if !defined(__pastesp_h)              // Sentry, use file only if it's not already included.
#define __pastesp_h


#include <owl\dialog.h>
#include <owl\checkbox.h>
#include <owltools\radiogrp.h>
#include <qconbrdg\qcbapp.rh>            // Definition of all resources.

struct PasteSpecialXferBuffer
{
   int ctrlID;
   int mirror;

   PasteSpecialXferBuffer(bool pasteSpan = true,bool mirrorSpan = false)
      {
      ctrlID = pasteSpan ? IDC_REPLACESPAN : IDC_REPLACESUPPORT;
      mirror = BF_UNCHECKED;
      };

   bool PasteSpan() const
      {
      return (ctrlID == IDC_REPLACESPAN);
      }

   bool PasteSupport() const
      {
      return (ctrlID == IDC_REPLACESUPPORT);
      }

   bool MirrorSpan() const
      {
      return (PasteSpan() && mirror == BF_CHECKED);
      }
};


//{{TDialog = TPasteSpecialDlg}}
class TPasteSpecialDlg : public TDialog {
public:
    TPasteSpecialDlg (TWindow* parent,bool enableReplaceSupport, TResId resId = IDD_PASTESPECIAL, TModule* module = 0);
    virtual ~TPasteSpecialDlg ();

protected:
   bool EnableReplaceSupport;
   TRadioGroupBox *radioGrpBox;
   TCheckBox *mirrorChk;

//{{TPasteSpecialDlgVIRTUAL_BEGIN}}
public:
    virtual void SetupWindow ();
//{{TPasteSpecialDlgVIRTUAL_END}}

//{{TPasteSpecialDlgRSP_TBL_BEGIN}}
protected:
    void BNClicked_Help ();
    void BNClicked_ReplaceSpan ();
    void BNClicked_ReplaceSupport ();
//{{TPasteSpecialDlgRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TPasteSpecialDlg);
};    //{{TPasteSpecialDlg}}


#endif                                      // __pastesp_h sentry.

