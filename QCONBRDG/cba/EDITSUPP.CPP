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

#include <qconbrdg\editsupp.h>
#include <qconbrdg\qcbapp.h>
#include <qconbrdg\common.rh>
#include <qconbrdg\gt0valid.h>
#include <Context.hh>

#include <shape\genshape.h>
#include <material\genmat.h>

EditSupportXferBuffer::EditSupportXferBuffer(Support &support)
{
   Segment *seg;
   TShape* sect;

   if (support.IsPier())
      {
      simple = BF_UNCHECKED;
      height = support.Height();
      offset = support.BaseOffset();

      seg = support.GetSegment(0);
      sect = seg->GetSection();

      ax = sect->GetArea();
      iz = sect->GetCentroidalInertia().Ixx;
      modE = sect->GetModE();
      unitWeight = sect->GetUnitWeight();

      hinge = (support.TopContinuity() == ctHinged) ? BF_CHECKED : BF_UNCHECKED;
      }
   else
      {
      simple = BF_CHECKED;
      height = 1.0;
      offset = 0.0;
      ax = 1.0;
      iz = 1.0;
      modE = 1.0;
      unitWeight = 1.0;
      hinge = BF_UNCHECKED;
      }

   switch(support.SupportFixity())
      {
      case fxRoller:
           fixity = IDC_ROLLER;
           break;

      case fxPinned:
           fixity = IDC_PINNED;
           break;

      case fxFixed:
           fixity = IDC_FIXED;
           break;
      }
}

Support *EditSupportXferBuffer::GetSupport() const
{
   Support *support;
   if (simple == BF_CHECKED)
      {
      support = new Support;
      }
   else
      {
      GeneralShape* newShape = new GeneralShape(ax,0,0,iz,0,0,0,0,new GeneralMaterial(modE,unitWeight));
      newShape->ComputeProperties();
      support = new Support(height,offset,newShape);
      support->TopContinuity( (hinge == BF_CHECKED) ? ctHinged : ctContinuous );
      }

   switch(fixity)
      {
      case IDC_PINNED:
           support->SupportFixity(fxPinned);
           break;

      case IDC_ROLLER:
           support->SupportFixity(fxRoller);
           break;

      case IDC_FIXED:
           support->SupportFixity(fxFixed);
           break;
      }

   return support;
}

//
//-------------------------------------------
//

class TEditSupportPage : public TMyPropertyPage {
public:
    TEditSupportPage (TPropertySheet* parent,SystemOfUnits units, TModule* module = 0);
    virtual ~TEditSupportPage();

protected:
   TCheckBox            *simpleChk;
   TLengthEdit          *height;
   TLengthEdit          *offset;
   TLength2Edit         *ax;
   TLength4Edit         *iz;
   TPressureEdit        *modE;
   TUnitWeightEdit      *unitWeight;
   TMaterialDensityEdit *density;
   TRadioGroupBox       *fixity;
   TCheckBox            *hinge;

   SystemOfUnits units;

public:
    virtual void SetupWindow ();

protected:
    void Help(TNotify far&);
    void BNClicked_Simple ();
    void EnableWindows();

DECLARE_RESPONSE_TABLE(TEditSupportPage);
};

DEFINE_RESPONSE_TABLE1(TEditSupportPage, TMyPropertyPage)
    EV_BN_CLICKED(IDC_SIMPLE, BNClicked_Simple),
END_RESPONSE_TABLE;


TEditSupportPage::TEditSupportPage (TPropertySheet* parent,SystemOfUnits units,TModule* module):
TMyPropertyPage(parent,IDD_EDITSUPPORT,"Support",0,module),
units(units)
{
    // INSERT>> Your constructor code here.
   PageInfo.dwFlags |= PSP_HASHELP;

   simpleChk = new TCheckBox(this,IDC_SIMPLE);

   if (units == unitsSI)
      {
      height = new TLengthEdit(this,IDC_HEIGHT,SIConstants::meter,11,3,IDC_HEIGHT_TAG,"m");
      height->SetValidator(new TGTZeroValidator("Height","m"));

      offset = new TLengthEdit(this,IDC_OFFSET,SIConstants::meter,11,3,IDC_OFFSET_TAG,"m");

      ax = new TLength2Edit(this,IDC_AX,SIConstants::millimeter2,11,3,IDC_AX_TAG,"mm^2");
      ax->SetValidator(new TGTZeroValidator("Ax","mm^2"));

      iz = new TLength4Edit(this,IDC_IZ,SIConstants::millimeter4,11,3,IDC_IZ_TAG,"mm^4");
      iz->SetValidator(new TGTZeroValidator("Iz","mm^4"));

      modE = new TPressureEdit(this,IDC_MODE,SIConstants::Pascal,11,3,IDC_MODE_TAG,"Pa");
      modE->SetValidator(new TGTZeroValidator("Mod. E","Pa"));

      unitWeight = NULL;
      density = new TMaterialDensityEdit(this,IDC_DENSITY,SIConstants::kilogram_per_meter3,11,3,IDC_DENSITY_TAG,"kg/m^3");
      density->SetValidator(new TGTZeroValidator("Density","kg/m^3"));
      }
   else
      {
      height = new TLengthEdit(this,IDC_HEIGHT,USConstants::feet,11,3,IDC_HEIGHT_TAG,"feet");
      height->SetValidator(new TGTZeroValidator("Height","feet"));

      offset = new TLengthEdit(this,IDC_OFFSET,USConstants::feet,11,3,IDC_OFFSET_TAG,"feet");

      ax = new TLength2Edit(this,IDC_AX,USConstants::inch2,11,3,IDC_AX_TAG,"in^2");
      ax->SetValidator(new TGTZeroValidator("Ax","in^2"));

      iz = new TLength4Edit(this,IDC_IZ,USConstants::inch4,11,3,IDC_IZ_TAG,"in^4");
      iz->SetValidator(new TGTZeroValidator("Iz","in^4"));

      modE = new TPressureEdit(this,IDC_MODE,USConstants::kip_per_inch2,11,3,IDC_MODE_TAG,"ksi");
      modE->SetValidator(new TGTZeroValidator("Mod. E","ksi"));

      density = NULL;
      unitWeight = new TUnitWeightEdit(this,IDC_DENSITY,USConstants::pound_per_feet3,11,3,IDC_DENSITY_TAG,"pcf");
      unitWeight->SetValidator(new TGTZeroValidator("Unit Wgt","pcf"));
      }

   fixity = new TRadioGroupBox(this,IDC_BCGROUP,IDC_PINNED,IDC_FIXED);
   hinge = new TCheckBox(this,IDC_HINGED);

}


TEditSupportPage::~TEditSupportPage ()
{
    Destroy();

    // INSERT>> Your destructor code here.
   delete simpleChk;
   delete height;
   delete offset;
   delete ax;
   delete iz;
   delete modE;
   delete unitWeight;
   delete density;
   delete fixity;
   delete hinge;
}

void TEditSupportPage::SetupWindow ()
{
    TMyPropertyPage::SetupWindow();

    // INSERT>> Your code here.
    SetDlgItemText(IDC_DENSITY_LABEL,(units == unitsSI) ? "Density" : "Unit Wgt" );
    EnableWindows();
}

void TEditSupportPage::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Edit_Support);
}

void TEditSupportPage::BNClicked_Simple ()
{
    // INSERT>> Your code here.
    EnableWindows();
    if (simpleChk->GetCheck() == BF_CHECKED &&
        fixity->GetSelRadioButton() == IDC_FIXED )
       {
       // The fixed radio button is checked, and it is not a valid option
       // for the current state.
       // Check Pinned option so that something valid will be checked.
       fixity->CheckRadioButton(RGB_BYCOMMAND,IDC_PINNED);
       }
}

void TEditSupportPage::EnableWindows()
{
    if (simpleChk->GetCheck() == BF_CHECKED)
       {
       // Disable the fixed option. Make sure it is unchecked.
       fixity->GetRadioButton(RGB_BYCOMMAND,IDC_FIXED)->EnableWindow(false);

       // Disable others
       height->EnableWindow(false);
       offset->EnableWindow(false);
       ax->EnableWindow(false);
       iz->EnableWindow(false);
       modE->EnableWindow(false);
       if (density)
          density->EnableWindow(false);
       if (unitWeight)
          unitWeight->EnableWindow(false);
       hinge->EnableWindow(false);
       }
    else
       {
       // Enable the fixed option.
       fixity->GetRadioButton(RGB_BYCOMMAND,IDC_FIXED)->EnableWindow(true);

       // Enable others
       height->EnableWindow(true);
       offset->EnableWindow(true);
       ax->EnableWindow(true);
       iz->EnableWindow(true);
       modE->EnableWindow(true);
       if (density)
          density->EnableWindow(true);
       if (unitWeight)
          unitWeight->EnableWindow(true);
       hinge->EnableWindow(true);
       }
}

//
//
//

TEditSupportDlg::TEditSupportDlg(TWindow* parent,SystemOfUnits units,TModule* module) :
TMyPropertySheet(parent,"Edit Support",0,false,PSH_DEFAULT | PSH_HASHELP | PSH_NOAPPLYNOW,module)
{
   page1 = new TEditSupportPage(this,units,module);
   AddPage(*page1);
}

TEditSupportDlg::~TEditSupportDlg()
{
   Destroy();
   delete page1;
}

void TEditSupportDlg::SetupTransferBuffer(EditSupportXferBuffer *buffer)
{
   page1->SetTransferBuffer(buffer);
}

