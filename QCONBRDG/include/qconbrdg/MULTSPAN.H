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

#if !defined(__multspan_h)              // Sentry, use file only if it's not already included.
#define __multspan_h


#if !defined(OWL_DIALOG_H)
#include <owl\dialog.h>
#endif

#if !defined(OWL_CHECKBOX_H)
#include <owl\checkbox.h>
#endif

#if !defined (__unitedit_h)
#include "unitedit.h"
#endif

#include "qcbapp.rh"            // Definition of all resources.

struct TMultiSpanXferBuffer
{
   double l1,l2;
   double axSpan,izSpan;
   double h;
   int hinge;
   double axSupport, izSupport;
   double modE,density;

   TMultiSpanXferBuffer(bool isSI);

   bool IsHinged()
      {
      return (hinge == BF_CHECKED);
      }
};


//{{TDialog = TMultiSpanDlg}}
class TMultiSpanDlg : public TDialog {
public:
    TMultiSpanDlg (TWindow* parent,bool _isSI,int prototype, TResId resId = IDD_MULTISPANS, TModule* module = 0);
    virtual ~TMultiSpanDlg ();

protected:
   TLengthEdit *l1;
   TLengthEdit *l2;
   TAreaEdit *axSpan;
   TMomentInertiaEdit *izSpan;
   TLengthEdit *h;
   TCheckBox *hingeChk;
   TAreaEdit *axSupport;
   TMomentInertiaEdit *izSupport;
   TPressureEdit *modE;
   TDensityEdit *density;

   int Prototype;

//{{TMultiSpanDlgVIRTUAL_BEGIN}}
public:
    virtual void SetupWindow ();
//{{TMultiSpanDlgVIRTUAL_END}}
};    //{{TMultiSpanDlg}}


#endif                                      // __multspan_h sentry.

