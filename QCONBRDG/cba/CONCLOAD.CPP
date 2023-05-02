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

#include <qconbrdg\concload.h>
#include <qconbrdg\qcbapp.h>
#include <qconbrdg\gt0valid.h>
#include <owltools\numvalid.h>

#include <Context.hh>

ConcLoadXferBuffer::ConcLoadXferBuffer(double start,bool fract,double mag,bool all,bool dc)
{
   startLoc = start;
   isFract = (fract) ? BF_CHECKED : BF_UNCHECKED;
   load = mag;
   allSpans = (all) ? BF_CHECKED : BF_UNCHECKED;
   loading = (dc) ? IDC_DC : IDC_DW;
}

double ConcLoadXferBuffer::GetStartLoc()
{
   return startLoc;
}

bool ConcLoadXferBuffer::IsFract()
{
   return (isFract == BF_CHECKED);
}

double ConcLoadXferBuffer::GetLoad()
{
   return load;
}

bool ConcLoadXferBuffer::AllSpans()
{
   return (allSpans == BF_CHECKED);
}

bool ConcLoadXferBuffer::DCLoading()
{
   return (loading == IDC_DC);
}

bool ConcLoadXferBuffer::DWLoading()
{
   return (loading == IDC_DW);
}


//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(TConcLoadDlg, TDialog)
//{{TConcLoadDlgRSP_TBL_BEGIN}}
    EV_BN_CLICKED(IDHELP, BNClicked_Help),
    EV_BN_CLICKED(IDC_FRACTIONAL, BNClicked_Fractional),
//{{TConcLoadDlgRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TConcLoadDlg Implementation}}


TConcLoadDlg::TConcLoadDlg (TWindow* parent,SystemOfUnits units,double maxLoc,bool disableAllSpans, TResId resId, TModule* module):
    TDialog(parent, resId, module)
{
    // INSERT>> Your constructor code here.
    DisableAllSpans = disableAllSpans;
    isSI = (units == unitsSI);

    if (isSI)
       {
       start = new TLengthEdit(this,IDC_STARTLOC,SIConstants::meter,11,3,IDC_STARTLOC_TAG,"m");
       start->SetValidator(new TTagIncRangeValidator("Loc","m",0.0,DerivedUnits::Length(maxLoc).ConvertTo(SIConstants::meter).value()));
       startFract = new TNumberEdit(this,IDC_STARTLOC_FRACT,11,3);
       startFract->SetValidator(new TIncRangeValidator(0.0,1.0));
       startFract->DisableTransfer();
       fract = new TCheckBox(this,IDC_FRACTIONAL);
       mag = new TForceEdit(this,IDC_LOAD,SIConstants::newton,11,3,IDC_LOAD_TAG,"N");
       allSpans = new TCheckBox(this,IDC_ALLSPANS);
       loading = new TRadioGroupBox(this,IDC_LOADINGGRP,IDC_DC,IDC_DW);
       }
    else
       {
       start = new TLengthEdit(this,IDC_STARTLOC,USConstants::feet,11,3,IDC_STARTLOC_TAG,"feet");
       start->SetValidator(new TTagIncRangeValidator("Loc","feet",0.0,DerivedUnits::Length(maxLoc).ConvertTo(USConstants::feet).value()));
       startFract = new TNumberEdit(this,IDC_STARTLOC_FRACT,11,3);
       startFract->SetValidator(new TIncRangeValidator(0.0,1.0));
       startFract->DisableTransfer();
       fract = new TCheckBox(this,IDC_FRACTIONAL);
       mag = new TForceEdit(this,IDC_LOAD,USConstants::kip,11,3,IDC_LOAD_TAG,"kip");
       allSpans = new TCheckBox(this,IDC_ALLSPANS);
       loading = new TRadioGroupBox(this,IDC_LOADINGGRP,IDC_DC,IDC_DW);
       }
}


TConcLoadDlg::~TConcLoadDlg ()
{
    Destroy();

   delete start;
   delete startFract;
   delete fract;
   delete mag;
   delete allSpans;
   delete loading;
}


void TConcLoadDlg::BNClicked_Help ()
{
    TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
    theApp->ActivateHelp( this, IDH_Concentrated_Load );
}


void TConcLoadDlg::SetupWindow ()
{
    TDialog::SetupWindow();

    if (DisableAllSpans)
       {
       allSpans->EnableWindow(false);
       allSpans->Show(SW_HIDE);
       }

    BNClicked_Fractional();

    // Now that the location edit controls (start and startFract) have been
    // setup by BNClicked_Fractional() we must forcefully extract the
    // data from the transfer buffer again. TWindow::SetupWindow()
    // has already initialized the controls with the data in the transfer buffer
    // but at that time start as enabled. If the location data was fractional
    // a units conversion was performed. Now that the correct edit controls
    // has been Enabled/Disabled we must re-transfer the data.
    TransferData(tdSetData);
}


void TConcLoadDlg::BNClicked_Fractional ()
{
   if (fract->GetCheck() == BF_CHECKED)
      {
      startFract->SetValue(start->GetValueEx());
      startFract->Show(SW_SHOW);
      startFract->EnableTransfer();

      start->DisableTransfer();
      start->Show(SW_HIDE);
      start->ShowTag(SW_HIDE);
      }
   else
      {
      start->SetValueEx(startFract->GetValue());
      start->Show(SW_SHOW);
      start->ShowTag(SW_SHOW);
      start->EnableTransfer();

      startFract->DisableTransfer();
      startFract->Show(SW_HIDE);
      }
}

