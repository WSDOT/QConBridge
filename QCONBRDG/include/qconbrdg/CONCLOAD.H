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

#if !defined(__concload_h)              // Sentry, use file only if it's not already included.
#define __concload_h


#include <owl\dialog.h>
#include <owl\checkbox.h>
#include <owltools\unitedit.h>
#include <owltools\radiogrp.h>

#include <qconbrdg\mytypes.h>

#include <qconbrdg\qcbapp.rh>            // Definition of all resources.

class ConcLoadXferBuffer
{
public:
   ConcLoadXferBuffer(double start,bool fract,double mag,bool all,bool dc);
   double GetStartLoc();
   bool IsFract();
   double GetLoad();
   bool AllSpans();
   bool DCLoading();
   bool DWLoading();

protected:
   double startLoc;
   int isFract;
   double load;
   int allSpans;
   int loading;
};

//{{TDialog = TConcLoadDlg}}
class TConcLoadDlg : public TDialog {
public:
    TConcLoadDlg (TWindow* parent,SystemOfUnits units,double maxLoc,bool disableAllSpans, TResId resId = IDD_CONCLOAD, TModule* module = 0);
    virtual ~TConcLoadDlg ();

protected:
   bool isSI;
   TLengthEdit *start;
   TNumberEdit *startFract;
   TCheckBox *fract;
   TForceEdit *mag;
   TCheckBox *allSpans;
   TRadioGroupBox *loading;
   bool DisableAllSpans;

//{{TConcLoadDlgVIRTUAL_BEGIN}}
public:
    virtual void SetupWindow ();
//{{TConcLoadDlgVIRTUAL_END}}

//{{TConcLoadDlgRSP_TBL_BEGIN}}
protected:
    void BNClicked_Help ();
    void BNClicked_Fractional ();
//{{TConcLoadDlgRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TConcLoadDlg);
};    //{{TConcLoadDlg}}


#endif                                      // __concload_h sentry.

