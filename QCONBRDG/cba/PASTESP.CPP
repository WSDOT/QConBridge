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


#include <qconbrdg\pastesp.h>
#include <qconbrdg\qcbapp.h>
#include <Context.hh>

//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(TPasteSpecialDlg, TDialog)
//{{TPasteSpecialDlgRSP_TBL_BEGIN}}
    EV_BN_CLICKED(IDHELP, BNClicked_Help),
    EV_BN_CLICKED(IDC_REPLACESPAN, BNClicked_ReplaceSpan),
    EV_BN_CLICKED(IDC_REPLACESUPPORT, BNClicked_ReplaceSupport),
//{{TPasteSpecialDlgRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TPasteSpecialDlg Implementation}}


TPasteSpecialDlg::TPasteSpecialDlg (TWindow* parent,bool enableReplaceSupport, TResId resId, TModule* module):
    TDialog(parent, resId, module)
{
    // INSERT>> Your constructor code here.
    EnableReplaceSupport = enableReplaceSupport;
    radioGrpBox = new TRadioGroupBox(this,IDC_PASTEOPTIONS,IDC_REPLACESPAN,IDC_REPLACESUPPORT);
    mirrorChk = new TCheckBox(this,IDC_MIRRORSPAN);
}


TPasteSpecialDlg::~TPasteSpecialDlg ()
{
    Destroy();

    // INSERT>> Your destructor code here.
    delete radioGrpBox;
    delete mirrorChk;
}


void TPasteSpecialDlg::BNClicked_Help ()
{
    TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Paste_Special_dialog_box);
}


void TPasteSpecialDlg::BNClicked_ReplaceSpan ()
{
    // INSERT>> Your code here.
    mirrorChk->EnableWindow(true);
}


void TPasteSpecialDlg::BNClicked_ReplaceSupport ()
{
    // INSERT>> Your code here.
    mirrorChk->EnableWindow(false);

}


void TPasteSpecialDlg::SetupWindow ()
{
    TDialog::SetupWindow();

    // INSERT>> Your code here.
    if (!EnableReplaceSupport)
       {
       TWindow(GetDlgItem(IDC_REPLACESUPPORT)).EnableWindow(false);
       }
}

