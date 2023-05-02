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

#if !defined(__peddlg_h)              // Sentry, use file only if it's not already included.
#define __peddlg_h


#include <owl\dialog.h>
#include <owltools\unitedit.h>
#include <qconbrdg\mytypes.h>
#include <qconbrdg\qcbapp.rh>            // Definition of all resources.

//{{TDialog = TPedestrianDlg}}
class TPedestrianDlg : public TDialog {
public:
    TPedestrianDlg (TWindow* parent,SystemOfUnits units, TResId resId = IDD_PEDESTRIAN, TModule* module = 0);
    virtual ~TPedestrianDlg ();

protected:
    TLinearForceEdit *load;

//{{TPedestrianDlgRSP_TBL_BEGIN}}
protected:
    void BNClicked_Help ();
//{{TPedestrianDlgRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TPedestrianDlg);
};    //{{TPedestrianDlg}}


#endif                                      // __peddlg_h sentry.

