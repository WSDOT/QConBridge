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


#include <qconbrdg\unifload.h>
#include <qconbrdg\qcbapp.h>
#include <qconbrdg\gt0valid.h>
#include <owltools\numvalid.h>
#include <Context.hh>

UnifLoadXferBuffer::UnifLoadXferBuffer(double start,double end,bool fract,double mag,bool all,bool dc)
{
   startLoc = start;
   endLoc = end;
   isFract = (fract) ? BF_CHECKED : BF_UNCHECKED;
   load = mag;
   allSpans = (all) ? BF_CHECKED : BF_UNCHECKED;
   loading = (dc) ? IDC_DC : IDC_DW;
}

double UnifLoadXferBuffer::GetStartLoc()
{
   return startLoc;
}

double UnifLoadXferBuffer::GetEndLoc()
{
   return endLoc;
}

bool UnifLoadXferBuffer::IsFract()
{
   return (isFract == BF_CHECKED);
}

double UnifLoadXferBuffer::GetLoad()
{
   return load;
}

bool UnifLoadXferBuffer::AllSpans()
{
   return (allSpans == BF_CHECKED);
}

bool UnifLoadXferBuffer::DCLoading()
{
   return (loading == IDC_DC);
}

bool UnifLoadXferBuffer::DWLoading()
{
   return (loading == IDC_DW);
}


//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(TUnifLoadDlg, TDialog)
//{{TUnifLoadDlgRSP_TBL_BEGIN}}
    EV_BN_CLICKED(IDHELP, BNClicked_Help),
    EV_BN_CLICKED(IDC_FRACTIONAL, BNClicked_Fractional),
//{{TUnifLoadDlgRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TUnifLoadDlg Implementation}}


TUnifLoadDlg::TUnifLoadDlg (TWindow* parent,SystemOfUnits units,double maxLoc,bool disableAllSpans, TResId resId, TModule* module):
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

       end   = new TLengthEdit(this,IDC_ENDLOC,SIConstants::meter,11,3,IDC_ENDLOC_TAG,"m");
       end->SetValidator(new TTagIncRangeValidator("Loc","m",0.0,DerivedUnits::Length(maxLoc).ConvertTo(SIConstants::meter).value()));

       endFract = new TNumberEdit(this,IDC_ENDLOC_FRACT,11,3);
       endFract->SetValidator(new TIncRangeValidator(0.0,1.0));
       endFract->DisableTransfer();

       fract = new TCheckBox(this,IDC_FRACTIONAL);
       mag = new TLinearForceEdit(this,IDC_LOAD,SIConstants::newton_per_meter,11,3,IDC_LOAD_TAG,"N/m");
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

       end   = new TLengthEdit(this,IDC_ENDLOC,USConstants::feet,11,3,IDC_ENDLOC_TAG,"feet");
       end->SetValidator(new TTagIncRangeValidator("Loc","feet",0.0,DerivedUnits::Length(maxLoc).ConvertTo(USConstants::feet).value()));

       endFract = new TNumberEdit(this,IDC_ENDLOC_FRACT,11,3);
       endFract->SetValidator(new TIncRangeValidator(0.0,1.0));
       endFract->DisableTransfer();

       fract = new TCheckBox(this,IDC_FRACTIONAL);
       mag = new TLinearForceEdit(this,IDC_LOAD,USConstants::pound_per_feet,11,3,IDC_LOAD_TAG,"lbs/ft");
       allSpans = new TCheckBox(this,IDC_ALLSPANS);
       loading = new TRadioGroupBox(this,IDC_LOADINGGRP,IDC_DC,IDC_DW);
       }

}


TUnifLoadDlg::~TUnifLoadDlg ()
{
    Destroy();

   delete start;
   delete startFract;
   delete end;
   delete endFract;
   delete fract;
   delete mag;
   delete allSpans;
   delete loading;
}


void TUnifLoadDlg::BNClicked_Help ()
{
    TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
    theApp->ActivateHelp( this, IDH_Uniform_Load );
}


void TUnifLoadDlg::SetupWindow ()
{
    TDialog::SetupWindow();

    // INSERT>> Your code here.
    if (DisableAllSpans)
       {
       allSpans->EnableWindow(false);
       allSpans->Show(SW_HIDE);
       }

    BNClicked_Fractional();

    // Now that the location edit controls (start/end and start/endFract) have been
    // setup by BNClicked_Fractional() we must forcefully extract the
    // data from the transfer buffer again. TWindow::SetupWindow()
    // has already initialized the controls with the data in the transfer buffer
    // but at that time start as enabled. If the location data was fractional
    // a units conversion was performed. Now that the correct edit controls
    // has been Enabled/Disabled we must re-transfer the data.
    TransferData(tdSetData);
}


void TUnifLoadDlg::BNClicked_Fractional ()
{
    if (fract->GetCheck() == BF_CHECKED)
       {
       start->DisableTransfer();
       start->Show(SW_HIDE);
       start->ShowTag(SW_HIDE);

       end->DisableTransfer();
       end->Show(SW_HIDE);
       end->ShowTag(SW_HIDE);

       startFract->SetValue(start->GetValueEx());
       startFract->EnableTransfer();
       startFract->Show(SW_SHOW);

       endFract->SetValue(end->GetValueEx());
       endFract->EnableTransfer();
       endFract->Show(SW_SHOW);
       }
    else
       {
       startFract->DisableTransfer();
       startFract->Show(SW_HIDE);

       endFract->DisableTransfer();
       endFract->Show(SW_HIDE);

       start->SetValueEx(startFract->GetValue());
       start->EnableTransfer();
       start->Show(SW_SHOW);
       start->ShowTag(SW_SHOW);

       end->SetValueEx(endFract->GetValue());
       end->EnableTransfer();
       end->Show(SW_SHOW);
       end->ShowTag(SW_SHOW);
       }
}

