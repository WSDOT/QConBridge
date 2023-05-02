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

#include <qconbrdg\loadfact.h>
#include <qconbrdg\qcbapp.h>
#include <Context.hh>


class TLoadFactorDlgPage1 : public TMyPropertyPage
{
public:
   TLoadFactorDlgPage1(TPropertySheet* parent,const char far* title = "Strength I",TModule *module = 0);
   ~TLoadFactorDlgPage1();

   void Help(TNotify far&);

private:
   TNumberEdit *dcMin,*dcMax;
   TNumberEdit *dwMin,*dwMax;
   TNumberEdit *ll;
};

TLoadFactorDlgPage1::TLoadFactorDlgPage1(TPropertySheet* parent,const char far* title,TModule *module) :
TMyPropertyPage(parent,TResId(IDD_LOADFACTORS_PAGE1),title,0,module)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   dcMin = new TNumberEdit(this,IDC_DC_MIN,11,3);
   dcMax = new TNumberEdit(this,IDC_DC_MAX,11,3);
   dwMin = new TNumberEdit(this,IDC_DW_MIN,11,3);
   dwMax = new TNumberEdit(this,IDC_DW_MAX,11,3);
   ll    = new TNumberEdit(this,IDC_LL    ,11,3);
}

TLoadFactorDlgPage1::~TLoadFactorDlgPage1()
{
   delete dcMin;
   delete dcMax;
   delete dwMin;
   delete dwMax;
   delete ll;
}

void TLoadFactorDlgPage1::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Load_Factors);
}

//
//
//

class TLoadFactorDlgPage2 : public TMyPropertyPage
{
public:
   TLoadFactorDlgPage2(TPropertySheet* parent,const char far* title,TModule *module = 0);
   ~TLoadFactorDlgPage2();

   void Help(TNotify far&);

private:
   TNumberEdit *dc;
   TNumberEdit *dw;
   TNumberEdit *ll;
};

TLoadFactorDlgPage2::TLoadFactorDlgPage2(TPropertySheet* parent,const char far* title,TModule *module) :
TMyPropertyPage(parent,TResId(IDD_LOADFACTORS_PAGE2),title,0,module)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   dc = new TNumberEdit(this,IDC_DC_MIN,11,3);
   dw = new TNumberEdit(this,IDC_DW_MIN,11,3);
   ll = new TNumberEdit(this,IDC_LL    ,11,3);
}

TLoadFactorDlgPage2::~TLoadFactorDlgPage2()
{
   delete dc;
   delete dw;
   delete ll;
}

void TLoadFactorDlgPage2::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Load_Factors);
}



//{{TLoadFactorDlg Implementation}}

TLoadFactorDlg::TLoadFactorDlg(TWindow* parent,TModule* module) :
TMyPropertySheet(parent,"Load Factors",0,false,
                 PSH_DEFAULT | PSH_HASHELP | PSH_NOAPPLYNOW,
                 module)
{
   page1 = new TLoadFactorDlgPage1(this,"Strength I",module);
   page2 = new TLoadFactorDlgPage2(this,"Service I",module);
   page3 = new TLoadFactorDlgPage2(this,"Service II",module);
   page4 = new TLoadFactorDlgPage2(this,"Service III",module);
   page5 = new TLoadFactorDlgPage2(this,"Fatigue",module);

   AddPage(*page1);
   AddPage(*page2);
   AddPage(*page3);
   AddPage(*page4);
   AddPage(*page5);
}

TLoadFactorDlg::~TLoadFactorDlg ()
{
    Destroy();

    // INSERT>> Your destructor code here.
    delete page1;
    delete page2;
    delete page3;
    delete page4;
    delete page5;
}

void TLoadFactorDlg::SetupTransferBuffer(LoadFactorXferBuffer *buffer)
{
   page1->SetTransferBuffer(&buffer->StrengthI);
   page2->SetTransferBuffer(&buffer->ServiceI);
   page3->SetTransferBuffer(&buffer->ServiceII);
   page4->SetTransferBuffer(&buffer->ServiceIII);
   page5->SetTransferBuffer(&buffer->Fatigue);
}

