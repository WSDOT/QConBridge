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


#include <qconbrdg\imdlg.h>
#include <qconbrdg\qcbapp.h>
#include <Context.hh>


//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(TImpactDlg, TDialog)
//{{TImpactDlgRSP_TBL_BEGIN}}
    EV_BN_CLICKED(IDHELP, BNClicked_Help),
//{{TImpactDlgRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TImpactDlg Implementation}}


TImpactDlg::TImpactDlg (TWindow* parent, TResId resId, TModule* module):
    TDialog(parent, resId, module)
{
    // INSERT>> Your constructor code here.
   truck   = new TNumberEdit(this,IDC_IMTRUCK,11,3);
   lane    = new TNumberEdit(this,IDC_IMLANE,11,3);
   fatigue = new TNumberEdit(this,IDC_IMFATIGUE,11,3);
}


TImpactDlg::~TImpactDlg ()
{
    Destroy();

    // INSERT>> Your destructor code here.
    delete truck;
    delete lane;
    delete fatigue;
}


void TImpactDlg::BNClicked_Help ()
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Dynamic_Load_Allowance);
}

