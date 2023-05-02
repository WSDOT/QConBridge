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


#include <qconbrdg\llgendlg.h>
#include <qconbrdg\qcbapp.h>
#include <Context.hh>

//
//
//

class TPropertyPage1 : public TMyPropertyPage
{
public:
	TPropertyPage1 (TPropertySheet* parent,const char* title,TModule *module = 0);
   ~TPropertyPage1();

protected:
   TCheckBox *disable;

   void Help(TNotify far&);
};

TPropertyPage1::TPropertyPage1(TPropertySheet* parent,const char* title, TModule *module) :
TMyPropertyPage(parent,IDD_LLGENERATION_PAGE1,title,0,module)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   disable = new TCheckBox(this,IDC_DISABLE);
}

TPropertyPage1::~TPropertyPage1()
{
   delete disable;
}

void TPropertyPage1::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   long help_topic;
   TMyPropertySheet* pPropSheet = GetParentSheet();
   switch(pPropSheet->IndexOfPage(this))
   {
   case 0:   help_topic = IDH_Design_Tandem; break;
   case 4:   help_topic = IDH_Fatigue_Truck; break;
   }
   theApp->ActivateHelp(this,help_topic);
}

///----------------------------------------------------------------------------

class TPropertyPage2 : public TMyPropertyPage
{
public:
   TPropertyPage2 (TPropertySheet* parent,const char* title,TModule *module = 0);
   ~TPropertyPage2();

protected:
   void BNClicked_Disable();
   void Help(TNotify far&);

   TCheckBox *disable;
   TIntEdit *numAxleInc;
   DECLARE_RESPONSE_TABLE(TPropertyPage2);
};

DEFINE_RESPONSE_TABLE1(TPropertyPage2, TMyPropertyPage)
    EV_BN_CLICKED(IDC_DISABLE, BNClicked_Disable),
END_RESPONSE_TABLE;

TPropertyPage2::TPropertyPage2(TPropertySheet* parent,const char* title,TModule *module) :
TMyPropertyPage(parent,IDD_LLGENERATION_PAGE2,title,0,module)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   disable = new TCheckBox(this,IDC_DISABLE);
   numAxleInc = new TIntEdit(this,IDC_AXLE_INCREMENTS,true);
}

TPropertyPage2::~TPropertyPage2()
{
   delete disable;
   delete numAxleInc;
}

void TPropertyPage2::BNClicked_Disable()
{
   numAxleInc->EnableWindow(disable->GetCheck() == BF_UNCHECKED);
}

void TPropertyPage2::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Design_Truck);
}

///------------------------------------------------------------------------

class TPropertyPage3 : public TMyPropertyPage
{
public:
   TPropertyPage3 (TPropertySheet* parent,SystemOfUnits units,const char* title,TModule *module = 0);
   ~TPropertyPage3();

protected:
   void SetupWindow();

   void BNClicked_Disable();
   void BNClicked_EnableRange2();
   void BNClicked_EnableRange3();

   void Help(TNotify far&);

   TCheckBox  *disabled;
   TLengthEdit *r1From;
   TLengthEdit *r1To;
   TIntEdit   *r1Inc;
   TCheckBox  *range2;
   TLengthEdit *r2From;
   TLengthEdit *r2To;
   TIntEdit   *r2Inc;
   TCheckBox  *range3;
   TLengthEdit *r3From;
   TLengthEdit *r3To;
   TIntEdit   *r3Inc;

   DECLARE_RESPONSE_TABLE(TPropertyPage3);
};

DEFINE_RESPONSE_TABLE1(TPropertyPage3, TMyPropertyPage)
    EV_BN_CLICKED(IDC_DISABLE, BNClicked_Disable),
    EV_BN_CLICKED(IDC_ENABLE_RANGE2, BNClicked_EnableRange2),
    EV_BN_CLICKED(IDC_ENABLE_RANGE3, BNClicked_EnableRange3),
END_RESPONSE_TABLE;

TPropertyPage3::TPropertyPage3(TPropertySheet* parent,SystemOfUnits units,const char* title,TModule *module) :
TMyPropertyPage(parent,IDD_LLGENERATION_PAGE3,title,0,module)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   if (units == unitsSI)
      {
      disabled = new TCheckBox(this,IDC_DISABLE);
      r1From  = new TLengthEdit(this,IDC_R1_FROM,SIConstants::meter,11,3,IDC_R1_FROM_TAG,"m");
      r1To    = new TLengthEdit(this,IDC_R1_TO  ,SIConstants::meter,11,3,IDC_R1_TO_TAG  ,"m");
      r1Inc   = new TIntEdit(this,IDC_R1_INC,true);
      range2  = new TCheckBox(this,IDC_ENABLE_RANGE2);
      r2From  = new TLengthEdit(this,IDC_R2_FROM,SIConstants::meter,11,3,IDC_R2_FROM_TAG,"m");
      r2To    = new TLengthEdit(this,IDC_R2_TO  ,SIConstants::meter,11,3,IDC_R2_TO_TAG  ,"m");
      r2Inc   = new TIntEdit(this,IDC_R2_INC,true);
      range3  = new TCheckBox(this,IDC_ENABLE_RANGE3);
      r3From  = new TLengthEdit(this,IDC_R3_FROM,SIConstants::meter,11,3,IDC_R3_FROM_TAG,"m");
      r3To    = new TLengthEdit(this,IDC_R3_TO  ,SIConstants::meter,11,3,IDC_R3_TO_TAG  ,"m");
      r3Inc   = new TIntEdit(this,IDC_R3_INC,true);
      }
   else
      {
      disabled = new TCheckBox(this,IDC_DISABLE);
      r1From  = new TLengthEdit(this,IDC_R1_FROM,USConstants::feet ,11,3,IDC_R1_FROM_TAG,"feet");
      r1To    = new TLengthEdit(this,IDC_R1_TO  ,USConstants::feet ,11,3,IDC_R1_TO_TAG  ,"feet");
      r1Inc   = new TIntEdit(this,IDC_R1_INC,true);
      range2  = new TCheckBox(this,IDC_ENABLE_RANGE2);
      r2From  = new TLengthEdit(this,IDC_R2_FROM,USConstants::feet ,11,3,IDC_R2_FROM_TAG,"feet");
      r2To    = new TLengthEdit(this,IDC_R2_TO  ,USConstants::feet ,11,3,IDC_R2_TO_TAG  ,"feet");
      r2Inc   = new TIntEdit(this,IDC_R2_INC,true);
      range3  = new TCheckBox(this,IDC_ENABLE_RANGE3);
      r3From  = new TLengthEdit(this,IDC_R3_FROM,USConstants::feet ,11,3,IDC_R3_FROM_TAG,"feet");
      r3To    = new TLengthEdit(this,IDC_R3_TO  ,USConstants::feet ,11,3,IDC_R3_TO_TAG  ,"feet");
      r3Inc   = new TIntEdit(this,IDC_R3_INC,true);
      }
}

TPropertyPage3::~TPropertyPage3()
{
   delete disabled;
   delete r1From;
   delete r1To;
   delete r1Inc;
   delete range2;
   delete r2From;
   delete r2To;
   delete r2Inc;
   delete range3;
   delete r3From;
   delete r3To;
   delete r3Inc;
}

void TPropertyPage3::SetupWindow()
{
   TMyPropertyPage::SetupWindow();

   BNClicked_Disable();
   BNClicked_EnableRange2();
   BNClicked_EnableRange3();
}

void TPropertyPage3::BNClicked_Disable()
{
   bool enable;
   bool enable2;
   bool enable3;

   enable = (disabled->GetCheck() == BF_UNCHECKED);

   TWindow(GetDlgItem(IDC_LABEL)).EnableWindow(enable);
   TWindow(GetDlgItem(IDC_ENABLE_RANGE1)).EnableWindow(enable);
   r1From->EnableWindow(enable);
   r1To->EnableWindow(enable);
   r1Inc->EnableWindow(enable);
   TWindow(GetDlgItem(IDC_MISC_TEXT1)).EnableWindow(enable);
   TWindow(GetDlgItem(IDC_MISC_TEXT2)).EnableWindow(enable);
   TWindow(GetDlgItem(IDC_MISC_TEXT3)).EnableWindow(enable);

   enable2 = (range2->GetCheck() == BF_CHECKED);
   range2->EnableWindow(enable);
   r2From->EnableWindow(enable && enable2);
   r2To->EnableWindow(enable && enable2);
   r2Inc->EnableWindow(enable && enable2);
   TWindow(GetDlgItem(IDC_MISC_TEXT4)).EnableWindow(enable && enable2);
   TWindow(GetDlgItem(IDC_MISC_TEXT5)).EnableWindow(enable && enable2);
   TWindow(GetDlgItem(IDC_MISC_TEXT6)).EnableWindow(enable && enable2);

   enable3 = (range3->GetCheck() == BF_CHECKED);
   range3->EnableWindow(enable);
   r3From->EnableWindow(enable && enable3);
   r3To->EnableWindow(enable && enable3);
   r3Inc->EnableWindow(enable && enable3);
   TWindow(GetDlgItem(IDC_MISC_TEXT7)).EnableWindow(enable && enable3);
   TWindow(GetDlgItem(IDC_MISC_TEXT8)).EnableWindow(enable && enable3);
   TWindow(GetDlgItem(IDC_MISC_TEXT9)).EnableWindow(enable && enable3);
}

void TPropertyPage3::BNClicked_EnableRange2()
{
   bool enable;

   enable = (range2->GetCheck() == BF_CHECKED);

   r2From->EnableWindow(enable);
   r2To->EnableWindow(enable);
   r2Inc->EnableWindow(enable);
   TWindow(GetDlgItem(IDC_MISC_TEXT4)).EnableWindow(enable);
   TWindow(GetDlgItem(IDC_MISC_TEXT5)).EnableWindow(enable);
   TWindow(GetDlgItem(IDC_MISC_TEXT6)).EnableWindow(enable);
}

void TPropertyPage3::BNClicked_EnableRange3()
{
   bool enable;

   enable = (range3->GetCheck() == BF_CHECKED);

   r3From->EnableWindow(enable);
   r3To->EnableWindow(enable);
   r3Inc->EnableWindow(enable);
   TWindow(GetDlgItem(IDC_MISC_TEXT7)).EnableWindow(enable);
   TWindow(GetDlgItem(IDC_MISC_TEXT8)).EnableWindow(enable);
   TWindow(GetDlgItem(IDC_MISC_TEXT9)).EnableWindow(enable);
}

void TPropertyPage3::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   long help_topic;
   TMyPropertySheet* pPropSheet = GetParentSheet();
   switch(pPropSheet->IndexOfPage(this))
   {
   case 2:   help_topic = IDH_Dual_Truck_Train; break;
   case 3:   help_topic = IDH_Dual_Tandem_Train; break;
   }
   theApp->ActivateHelp(this,help_topic);
}

///--------------------------------------------------------------------------

TLiveLoadGenDlg::TLiveLoadGenDlg (TWindow* parent,SystemOfUnits units,TModule* module) :
TMyPropertySheet(parent,"Live Load Generation Parameters",0,false,
                 PSH_DEFAULT | PSH_HASHELP | PSH_NOAPPLYNOW,
                 module)
{
   page1 = new TPropertyPage1(this,"Design Tandem",module);
   page2 = new TPropertyPage2(this,"Design Truck",module);
   page3 = new TPropertyPage3(this,units,"Dual Truck Train",module);
   page4 = new TPropertyPage3(this,units,"Dual Tandem Train",module);
   page5 = new TPropertyPage1(this,"Fatigue Truck",module);

   AddPage(*page1);
   AddPage(*page2);
   AddPage(*page3);
   AddPage(*page4);
   AddPage(*page5);
}


TLiveLoadGenDlg::~TLiveLoadGenDlg ()
{
    Destroy();

    delete page1;
    delete page2;
    delete page3;
    delete page4;
    delete page5;
}

void TLiveLoadGenDlg::SetupTransferBuffer(LLGenXferBuffer *buffer)
{
   page1->SetTransferBuffer(&buffer->designTandem);
   page2->SetTransferBuffer(&buffer->varAxleTruck);
   page3->SetTransferBuffer(&buffer->dualTruckTrain);
   page4->SetTransferBuffer(&buffer->dualTandemTrain);
   page5->SetTransferBuffer(&buffer->fatigueTruck);
}

