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


#include <qconbrdg\stddload.h>
#include <qconbrdg\qcbapp.h>
#include <Context.hh>

class TDCLoadPage : public TMyPropertyPage
{
public:
   TDCLoadPage(TPropertySheet* parent,SystemOfUnits units,TModule* module = 0);
   ~TDCLoadPage();
   void SetupWindow();

protected:
   TCheckBox      *selfWeightChk;
   TCheckBox      *tBarrierChk;
   TLinearForceEdit *tBarrierLd;

protected:
   void BNClicked_TBarrier();
   void Help(TNotify far&);

DECLARE_RESPONSE_TABLE(TDCLoadPage);
};

DEFINE_RESPONSE_TABLE1(TDCLoadPage, TMyPropertyPage)
    EV_BN_CLICKED(IDC_TBARRIER_CHK, BNClicked_TBarrier),
END_RESPONSE_TABLE;

TDCLoadPage::TDCLoadPage(TPropertySheet* parent,SystemOfUnits units,TModule* module) :
TMyPropertyPage(parent,IDD_STDDEADLOAD_DC,"DC Loads",0,module)
{
   PageInfo.dwFlags |= PSP_HASHELP;


   LinearForceCtrlData barrier[]  = {LinearForceCtrlData(IDC_TBARRIER,SIConstants::newton_per_meter,11,3,IDC_TBARRIER_TAG,"N/m"),
                                     LinearForceCtrlData(IDC_TBARRIER,USConstants::pound_per_feet,11,3,IDC_TBARRIER_TAG,"lbs/ft")};
   selfWeightChk = new TCheckBox(this,IDC_GENWEIGHT);
   tBarrierChk   = new TCheckBox(this,IDC_TBARRIER_CHK);
   tBarrierLd    = new TLinearForceEdit(this,DLGCTRLDATA(barrier,units));
}

TDCLoadPage::~TDCLoadPage()
{
   Destroy();

   delete selfWeightChk;
   delete tBarrierChk;
   delete tBarrierLd;
}

void TDCLoadPage::SetupWindow ()
{
    TPropertyPage::SetupWindow();

    // Pretend the buttons were checked so that the edit controls may
    // enabled or disables, as appropreate.
    BNClicked_TBarrier ();
}

void TDCLoadPage::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_DC_Loads);
}

void TDCLoadPage::BNClicked_TBarrier ()
{
    // INSERT>> Your code here.
    HWND edit;
    HWND tag;

    edit = GetDlgItem(IDC_TBARRIER);
    tag  = GetDlgItem(IDC_TBARRIER_TAG);

    TWindow editWnd(edit);
    TWindow tagWnd(tag);

    if (IsDlgButtonChecked(IDC_TBARRIER_CHK))
       {
       editWnd.EnableWindow(true);
       tagWnd.EnableWindow(true);
       }
    else
       {
       editWnd.EnableWindow(false);
       tagWnd.EnableWindow(false);
       }
}

//
// ----------------------------------------------------------------------
//

class TDWLoadPage : public TMyPropertyPage
{
public:
   TDWLoadPage(TPropertySheet* parent,SystemOfUnits units,TModule *module = 0);
   ~TDWLoadPage();
    void SetupWindow();

protected:
   TCheckBox        *utilityChk;
   TLinearForceEdit *utilityLd;
   TCheckBox        *wearSurfChk;
   TLinearForceEdit *wearSurfLd;

protected:
   void BNClicked_Utility ();
   void BNClicked_WearingSurface ();
   void Help(TNotify far&);

DECLARE_RESPONSE_TABLE(TDWLoadPage);
};

DEFINE_RESPONSE_TABLE1(TDWLoadPage, TMyPropertyPage)
    EV_BN_CLICKED(IDC_UTILITY_CHK, BNClicked_Utility),
    EV_BN_CLICKED(IDC_WEARSURF_CHK, BNClicked_WearingSurface),
END_RESPONSE_TABLE;

TDWLoadPage::TDWLoadPage(TPropertySheet* parent,SystemOfUnits units,TModule *module) :
TMyPropertyPage(parent,IDD_STDDEADLOAD_DW,"DW Loads",0,module)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   // Define lookups
   LinearForceCtrlData utility[2]  = {LinearForceCtrlData(IDC_UTILITY,SIConstants::newton_per_meter,11,3,IDC_UTILITY_TAG,"N/m"),
                                      LinearForceCtrlData(IDC_UTILITY,USConstants::pound_per_feet,11,3,IDC_UTILITY_TAG,"lbs/ft")};
   LinearForceCtrlData wearSurf[2] = {LinearForceCtrlData(IDC_WEARSURF,SIConstants::newton_per_meter,11,3,IDC_WEARSURF_TAG,"N/m"),
                                      LinearForceCtrlData(IDC_WEARSURF,USConstants::pound_per_feet,11,3,IDC_WEARSURF_TAG,"lbs/ft")};

   // Build controls
   utilityChk    = new TCheckBox(this,IDC_UTILITY_CHK);
   utilityLd     = new TLinearForceEdit(this,DLGCTRLDATA(utility,units));
   wearSurfChk   = new TCheckBox(this,IDC_WEARSURF_CHK);
   wearSurfLd    = new TLinearForceEdit(this,DLGCTRLDATA(wearSurf,units));
}

TDWLoadPage::~TDWLoadPage()
{
   Destroy();

   delete utilityChk;
   delete utilityLd;
   delete wearSurfChk;
   delete wearSurfLd;
}

void TDWLoadPage::SetupWindow ()
{
    TPropertyPage::SetupWindow();

    // Pretend the buttons were checked so that the edit controls may
    // enabled or disables, as appropreate.
    BNClicked_Utility ();
    BNClicked_WearingSurface ();
}

void TDWLoadPage::BNClicked_Utility ()
{
    // INSERT>> Your code here.
    HWND edit;
    HWND tag;

    edit = GetDlgItem(IDC_UTILITY);
    tag  = GetDlgItem(IDC_UTILITY_TAG);

    TWindow editWnd(edit);
    TWindow tagWnd(tag);

    if (IsDlgButtonChecked(IDC_UTILITY_CHK))
       {
       editWnd.EnableWindow(true);
       tagWnd.EnableWindow(true);
       }
    else
       {
       editWnd.EnableWindow(false);
       tagWnd.EnableWindow(false);
       }

}

void TDWLoadPage::BNClicked_WearingSurface ()
{
    // INSERT>> Your code here.
    HWND edit;
    HWND tag;

    edit = GetDlgItem(IDC_WEARSURF);
    tag  = GetDlgItem(IDC_WEARSURF_TAG);

    TWindow editWnd(edit);
    TWindow tagWnd(tag);

    if (IsDlgButtonChecked(IDC_WEARSURF_CHK))
       {
       editWnd.EnableWindow(true);
       tagWnd.EnableWindow(true);
       }
    else
       {
       editWnd.EnableWindow(false);
       tagWnd.EnableWindow(false);
       }

}

void TDWLoadPage::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_DW_Loads);
}

//
// ----------------------------------------------------------------------
//

TStdDeadLoadDlg::TStdDeadLoadDlg(TWindow* parent,
                                 SystemOfUnits units,
                                 TModule* module ) :
TMyPropertySheet(parent,"Standard Dead Loads",0,false,
                 PSH_DEFAULT | PSH_HASHELP | PSH_NOAPPLYNOW,
                 module)
{
   page1 = new TDCLoadPage(this,units,module);
   page2 = new TDWLoadPage(this,units,module);

   AddPage(*page1);
   AddPage(*page2);
}

TStdDeadLoadDlg::~TStdDeadLoadDlg()
{
   delete page1;
   delete page2;
}

void TStdDeadLoadDlg::SetupTransferBuffer(StdDeadLoadXferBuffer *buffer)
{
   page1->SetTransferBuffer(&buffer->page1);
   page2->SetTransferBuffer(&buffer->page2);
}

