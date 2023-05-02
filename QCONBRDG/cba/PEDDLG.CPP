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


#include <qconbrdg\peddlg.h>
#include <qconbrdg\qcbapp.h>
#include <Context.hh>

//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(TPedestrianDlg, TDialog)
//{{TPedestrianDlgRSP_TBL_BEGIN}}
    EV_BN_CLICKED(IDHELP, BNClicked_Help),
//{{TPedestrianDlgRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TPedestrianDlg Implementation}}


TPedestrianDlg::TPedestrianDlg (TWindow* parent,SystemOfUnits units, TResId resId, TModule* module):
    TDialog(parent, resId, module)
{
    // Define Lookup
    LinearForceCtrlData ped[2] = {LinearForceCtrlData(IDC_PEDESTRIAN,SIConstants::newton_per_meter,11,3,IDC_PEDESTRIAN_TAG,"N/m"),
                                  LinearForceCtrlData(IDC_PEDESTRIAN,USConstants::pound_per_feet,11,3,IDC_PEDESTRIAN_TAG,"lbs/ft")};

    // Build controls
    load = new TLinearForceEdit(this,DLGCTRLDATA(ped,units));
}


TPedestrianDlg::~TPedestrianDlg ()
{
    Destroy();

    // INSERT>> Your destructor code here.
    delete load;
}


void TPedestrianDlg::BNClicked_Help ()
{
    TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Pedestrian_Live_Load);
}

