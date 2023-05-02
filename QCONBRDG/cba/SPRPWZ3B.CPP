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



#include <qconbrdg\sprpwiz.h>
#include <qconbrdg\flprint.h>
#include <qconbrdg\qcbapp.h>
#include <Context.hh>

#include <shape\genshape.h>
#include <shape\plategdr.h>
#include <material\genmat.h>
#include <code\lrfdcode.h>
#include <esl\strutil.h>
#include <code\lldist.h>
#include <esl\engnotat.h>
#include <owl\printer.h>
#include <winsys\registry.h>
#include <dir.h> // For MAXPATH


DEFINE_RESPONSE_TABLE1(TSectPropWizard::Page3b,TWizardStep)
   EV_BN_CLICKED(IDC_BEAMCHECK,BNClicked_BeamCheck),
   EV_BN_CLICKED(IDC_INTGIRDER,BNClicked_GirderLocation),
   EV_BN_CLICKED(IDC_EXTGIRDER,BNClicked_GirderLocation),
END_RESPONSE_TABLE;

TSectPropWizard::Page3b::Page3b(TSectPropWizard *wiz,TModule *module) :
TWizardStep(wiz,IDD_SECTPRPWIZARD_PAGE3B,"Beam/Girder Description",0,module),
wizard(wiz)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   if(wizard->data.dispUnits == unitsSI)
      {
      useList = new TCheckBox(this,IDC_BEAMCHECK);
      list = new TComboBox(this,IDC_BEAMCOMBOBOX);
      tfThick = new TLengthEdit(this,IDC_TF_THICK,SIConstants::millimeter,11,3,IDC_TF_THICK_TAG,"mm");
      tfWidth = new TLengthEdit(this,IDC_TF_WIDTH,SIConstants::millimeter,11,3,IDC_TF_WIDTH_TAG,"mm");
      webThick = new TLengthEdit(this,IDC_WEB_THICK,SIConstants::millimeter,11,3,IDC_WEB_THICK_TAG,"mm");
      webHeight = new TLengthEdit(this,IDC_WEB_HEIGHT,SIConstants::millimeter,11,3,IDC_WEB_HEIGHT_TAG,"mm");
      bfThick = new TLengthEdit(this,IDC_BF_THICK,SIConstants::millimeter,11,3,IDC_BF_THICK_TAG,"mm");
      bfWidth = new TLengthEdit(this,IDC_BF_WIDTH,SIConstants::millimeter,11,3,IDC_BF_WIDTH_TAG,"mm");
      unitWgt = 0;
      density = new TMaterialDensityEdit(this,IDC_UNITWGT,SIConstants::kilogram_per_meter3,11,3,IDC_UNITWGT_TAG,"kg/m^3");
      modE = new TPressureEdit(this,IDC_MODE,SIConstants::MPa,11,3,IDC_MODE_TAG,"MPa");;
      location = new TRadioGroupBox(this,IDC_LOCATIONGRP,IDC_INTGIRDER,IDC_EXTGIRDER);
      xFrames = new TCheckBox(this,IDC_XFRAMES);
      spanLength = new TLengthEdit(this,IDC_LENGTH,SIConstants::meter,11,3,IDC_LENGTH_TAG,"m");
      spacing = new TLengthEdit(this,IDC_SPACING,SIConstants::millimeter,11,3,IDC_SPACING_TAG,"mm");
      numBeams = new TIntEdit(this,IDC_NUMBEAMS);
      numLanes = new TIntEdit(this,IDC_NUMLANES);
      }
   else
      {
      useList = new TCheckBox(this,IDC_BEAMCHECK);
      list = new TComboBox(this,IDC_BEAMCOMBOBOX);
      tfThick = new TLengthEdit(this,IDC_TF_THICK,USConstants::inch,11,3,IDC_TF_THICK_TAG,"inch");
      tfWidth = new TLengthEdit(this,IDC_TF_WIDTH,USConstants::inch,11,3,IDC_TF_WIDTH_TAG,"inch");
      webThick = new TLengthEdit(this,IDC_WEB_THICK,USConstants::inch,11,3,IDC_WEB_THICK_TAG,"inch");
      webHeight = new TLengthEdit(this,IDC_WEB_HEIGHT,USConstants::inch,11,3,IDC_WEB_HEIGHT_TAG,"inch");
      bfThick = new TLengthEdit(this,IDC_BF_THICK,USConstants::inch,11,3,IDC_BF_THICK_TAG,"inch");
      bfWidth = new TLengthEdit(this,IDC_BF_WIDTH,USConstants::inch,11,3,IDC_BF_WIDTH_TAG,"inch");
      unitWgt = new TUnitWeightEdit(this,IDC_UNITWGT,USConstants::pound_per_feet3,11,3,IDC_UNITWGT_TAG,"pcf");
      density = 0;
      modE = new TPressureEdit(this,IDC_MODE,USConstants::pound_per_inch2,11,3,IDC_MODE_TAG,"psi");;
      location = new TRadioGroupBox(this,IDC_LOCATIONGRP,IDC_INTGIRDER,IDC_EXTGIRDER);
      xFrames = new TCheckBox(this,IDC_XFRAMES);
      spanLength = new TLengthEdit(this,IDC_LENGTH,USConstants::feet,11,3,IDC_LENGTH_TAG,"feet");
      spacing = new TLengthEdit(this,IDC_SPACING,USConstants::feet,11,3,IDC_SPACING_TAG,"feet");
      numBeams = new TIntEdit(this,IDC_NUMBEAMS);
      numLanes = new TIntEdit(this,IDC_NUMLANES);
      }
}

TSectPropWizard::Page3b::~Page3b()
{
   // Must remove ItemData from list
   for (int i = 0; i < list->GetCount(); i++)
      {
      BeamDimensions *girder = (BeamDimensions *)list->GetItemData(i);
      delete girder;
      list->SetItemData(i,0);
      }

   delete useList;
   delete list;
   delete tfThick;
   delete tfWidth;
   delete webThick;
   delete webHeight;
   delete bfThick;
   delete bfWidth;
   delete unitWgt;
   delete density;
   delete modE;
   delete location;
   delete xFrames;
   delete spanLength;
   delete spacing;
   delete numBeams;
   delete numLanes;
}

void TSectPropWizard::Page3b::SetupWindow()
{
   TWizardStep::SetupWindow();

   if(wizard->data.dispUnits == unitsSI)
      SetDlgItemText(IDC_UNITWGT_LABEL,"Density");
   else
      SetDlgItemText(IDC_UNITWGT_LABEL,"Unit Wgt");

   LoadBeamTable();
   list->SetSelIndex(0); // select the first item

   useList->SetCheck(BF_CHECKED);
   BNClicked_BeamCheck();

   location->CheckRadioButton(RGB_BYCOMMAND,IDC_INTGIRDER);
   BNClicked_GirderLocation();

   spanLength->SetValue(wizard->data.spanLength);

   if(wizard->data.dispUnits == unitsSI)
      {
      density->SetValueEx(7850.); // ~490pcf
      modE->SetValueEx(200e3); // 200000 MPa
      spacing->SetValueEx(3660.); // ~12ft
      }
   else
      {
      unitWgt->SetValueEx(490.);
      modE->SetValueEx(29e6);
      spacing->SetValueEx(12.);
      }
   numBeams->SetValue(4);
   numLanes->SetValue(3);
}

int
TSectPropWizard::Page3b::SetActive(TNotify far& nmHdr)
{
   int retval = TWizardStep::SetActive(nmHdr);

   switch(wizard->page2->GetDeckType())
      {
      case IDC_WOOD_PLANK:
      case IDC_STRESS_LAMINATED:
      case IDC_SPIKE_LAMINATED:
      case IDC_GLUE_LAMINATED:
           spanLength->EnableWindow(false);
           TWindow(GetDlgItem(IDC_LENGTH_LABEL)).EnableWindow(false);
           numBeams->EnableWindow(false);
           TWindow(GetDlgItem(IDC_NUMBEAMS_LABEL)).EnableWindow(false);
           numLanes->EnableWindow(false);
           TWindow(GetDlgItem(IDC_NUMLANES_LABEL)).EnableWindow(false);
           xFrames->EnableWindow(false);
           break;
      }

   return retval;
}

void
TSectPropWizard::Page3b::BNClicked_BeamCheck()
{
   bool enableList;
   bool enablePlates;
   if (useList->GetCheck() == BF_CHECKED)
      {
      enableList = true;
      enablePlates = false;
      }
   else
      {
      enableList = false;
      enablePlates = true;
      }

   list->EnableWindow(enableList);
   tfThick->EnableWindow(enablePlates);
   TWindow(GetDlgItem(IDC_TF_GROUP)).EnableWindow(enablePlates);
   TWindow(GetDlgItem(IDC_TF_THICK_LABEL)).EnableWindow(enablePlates);
   tfWidth->EnableWindow(enablePlates);
   TWindow(GetDlgItem(IDC_TF_WIDTH_LABEL)).EnableWindow(enablePlates);
   webThick->EnableWindow(enablePlates);
   TWindow(GetDlgItem(IDC_WEB_GROUP)).EnableWindow(enablePlates);
   TWindow(GetDlgItem(IDC_WEB_THICK_LABEL)).EnableWindow(enablePlates);
   webHeight->EnableWindow(enablePlates);
   TWindow(GetDlgItem(IDC_WEB_HEIGHT_LABEL)).EnableWindow(enablePlates);
   bfThick->EnableWindow(enablePlates);
   TWindow(GetDlgItem(IDC_BF_GROUP)).EnableWindow(enablePlates);
   TWindow(GetDlgItem(IDC_BF_THICK_LABEL)).EnableWindow(enablePlates);
   bfWidth->EnableWindow(enablePlates);
   TWindow(GetDlgItem(IDC_BF_WIDTH_LABEL)).EnableWindow(enablePlates);
}

void
TSectPropWizard::Page3b::BNClicked_GirderLocation()
{
   switch(wizard->page2->GetDeckType())
      {
      case IDC_WOOD_PLANK:
      case IDC_STRESS_LAMINATED:
      case IDC_SPIKE_LAMINATED:
      case IDC_GLUE_LAMINATED:
           return;
      }

   bool enable;
   if (location->GetSelRadioButton() == IDC_INTGIRDER)
      enable = false;
   else
      enable = true;

   xFrames->EnableWindow(enable);
   numLanes->EnableWindow(enable);
   TWindow(GetDlgItem(IDC_NUMLANES_LABEL)).EnableWindow(enable);
}

double
TSectPropWizard::Page3b::GetWebThickness() const
{
   double thick;

   if(UseList())
      {
      BeamDimensions* dim;

      dim = (BeamDimensions *)list->GetItemData(list->GetSelIndex());
      thick = dim->tw;
      }
   else
      {
      thick = webThick->GetValue();
      }

   return thick;
}

double
TSectPropWizard::Page3b::GetTopFlangeWidth() const
{
   double tfw;

   if(UseList())
      {
      BeamDimensions* dim;

      dim = (BeamDimensions *)list->GetItemData(list->GetSelIndex());
      tfw = dim->bf;
      }
   else
      {
      tfw = tfWidth->GetValue();
      }

   return tfw;
}

double
TSectPropWizard::Page3b::GetHeight() const
{
   double height;

   if(UseList())
      {
      BeamDimensions* dim;

      dim = (BeamDimensions *)list->GetItemData(list->GetSelIndex());
      height = dim->d;
      }
   else
      {
      height = tfThick->GetValue()     +
               webHeight->GetValue() +
               bfThick->GetValue();
      }

   return height;
}

TShape*
TSectPropWizard::Page3b::GetBeam() const
{
   TShape* beam;
   GeneralMaterial* material;

   material = new GeneralMaterial(GetModE(),GetUnitWgt());

   if(UseList())
      {
      BeamDimensions* dim;

      dim = (BeamDimensions *)list->GetItemData(list->GetSelIndex());
      beam = new GeneralShape(dim->ax,0.00,dim->d/2,dim->iz,0.00,0.00,dim->d,0.00,material);
      }
   else
      {
      beam = new TPlateGirder(tfThick->GetValue(),
                              tfWidth->GetValue(),
                              bfThick->GetValue(),
                              bfWidth->GetValue(),
                              webThick->GetValue(),
                              webHeight->GetValue(),
                              material);
      }

   beam->ComputeProperties();
   return beam;
}

void
TSectPropWizard::Page3b::LoadBeamTable()
{
   BeamDimensions *girder;
   char buffer[256];
   char fname[MAXPATH];
   char name[21];
   double ax, iz, d, tw, bf;
   char regKey[256];

   TQConBridgeApp* app = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   app->GetRegistryKey(0,regKey,sizeof(regKey));

   uint32 size = sizeof(fname);
   if (TRegKey(TRegKey::CurrentUser(),regKey).QueryValue("Wide Flange Table",0,(uint8*)&fname,&size) != ERROR_SUCCESS)
      return; // Table not found

   ifstream ifile(fname);

   if (!ifile)
      return;

   bool readingHeader = false;
   while(ifile)
      {
      initstg(buffer,' ',sizeof(buffer));
      ifile.getline(buffer,sizeof(buffer));

      if (strncmp(buffer,"# =",3) == 0)
         {
         readingHeader = true;
         continue;
         }

      if (readingHeader)
         {
         if (strncmp(buffer,"===",3) == 0)
            readingHeader = false;
         }
      else
         {
         getstg(buffer,1,7,name);
         ax = getf(buffer,16,7)*SIConstants::millimeter2.value();
         iz = getf(buffer,25,12)*SIConstants::millimeter4.value();
         d  = getf(buffer,39,6)*SIConstants::millimeter.value();
         tw = getf(buffer,47,6)*SIConstants::millimeter.value();
         bf = getf(buffer,55,6)*SIConstants::millimeter.value();

         girder = new BeamDimensions(ax,iz,d,tw,bf);
         list->SetItemData(list->AddString(name),(int32)girder);
         }
      }
}

void
TSectPropWizard::Page3b::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Beam_Girder_Description_Steel_Beam);
}

