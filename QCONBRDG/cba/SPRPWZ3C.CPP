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



DEFINE_RESPONSE_TABLE1(TSectPropWizard::Page3c,TWizardStep)
   EV_BN_CLICKED(IDC_BEAMCHECK,BNClicked_BeamCheck),
   EV_BN_CLICKED(IDC_INTGIRDER,BNClicked_GirderLocation),
   EV_BN_CLICKED(IDC_EXTGIRDER,BNClicked_GirderLocation),
END_RESPONSE_TABLE;

TSectPropWizard::Page3c::Page3c(TSectPropWizard *wiz,TModule *module) :
TWizardStep(wiz,IDD_SECTPRPWIZARD_PAGE3C,"I-Beam/Bulb-Tee Description",0,module),
wizard(wiz)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   if(wizard->data.dispUnits == unitsSI)
      {
      useList = new TCheckBox(this,IDC_BEAMCHECK);
      list = new TComboBox(this,IDC_BEAMCOMBOBOX);
      ax = new TLength2Edit(this,IDC_AX,SIConstants::millimeter2,11,3,IDC_AX_TAG,"mm^2");
      iz = new TLength4Edit(this,IDC_IZ,SIConstants::millimeter4,11,3,IDC_IZ_TAG,"mm^4");
      yb = new TLengthEdit(this,IDC_YB,SIConstants::millimeter,11,3,IDC_YB_TAG,"mm");
      h  = new TLengthEdit(this,IDC_HEIGHT,SIConstants::millimeter,11,3,IDC_HEIGHT_TAG,"mm");
      tfWidth = new TLengthEdit(this,IDC_TOPFLGWIDTH,SIConstants::millimeter,11,3,IDC_TOPFLGWIDTH_TAG,"mm");
      wWeb = new TLengthEdit(this,IDC_WEBWIDTH,SIConstants::millimeter,11,3,IDC_WEBWIDTH_TAG,"mm");
      unitWgt = 0;
      density = new TMaterialDensityEdit(this,IDC_UNITWGT,SIConstants::kilogram_per_meter3,11,3,IDC_UNITWGT_TAG,"kg/m^3");
      fc = new TPressureEdit(this,IDC_FC,SIConstants::MPa,11,3,IDC_FC_TAG,"MPa");
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
      ax = new TLength2Edit(this,IDC_AX,USConstants::inch2,11,3,IDC_AX_TAG,"inch^2");
      iz = new TLength4Edit(this,IDC_IZ,USConstants::inch4,11,3,IDC_IZ_TAG,"inch^4");
      yb = new TLengthEdit(this,IDC_YB,USConstants::inch,11,3,IDC_YB_TAG,"inch");
      h  = new TLengthEdit(this,IDC_HEIGHT,USConstants::inch,11,3,IDC_HEIGHT_TAG,"inch");
      tfWidth = new TLengthEdit(this,IDC_TOPFLGWIDTH,USConstants::inch,11,3,IDC_TOPFLGWIDTH_TAG,"inch");
      wWeb = new TLengthEdit(this,IDC_WEBWIDTH,USConstants::inch,11,3,IDC_WEBWIDTH_TAG,"inch");
      unitWgt = new TUnitWeightEdit(this,IDC_UNITWGT,USConstants::pound_per_feet3,11,3,IDC_UNITWGT_TAG,"pcf");
      density = 0;
      fc = new TPressureEdit(this,IDC_FC,USConstants::pound_per_inch2,11,3,IDC_FC_TAG,"psi");
      location = new TRadioGroupBox(this,IDC_LOCATIONGRP,IDC_INTGIRDER,IDC_EXTGIRDER);
      xFrames = new TCheckBox(this,IDC_XFRAMES);
      spanLength = new TLengthEdit(this,IDC_LENGTH,USConstants::feet,11,3,IDC_LENGTH_TAG,"feet");
      spacing = new TLengthEdit(this,IDC_SPACING,USConstants::feet,11,3,IDC_SPACING_TAG,"feet");
      numBeams = new TIntEdit(this,IDC_NUMBEAMS);
      numLanes = new TIntEdit(this,IDC_NUMLANES);
      }
}

TSectPropWizard::Page3c::~Page3c()
{
   // Must remove ItemData from list
   for (int i = 0; i < list->GetCount(); i++)
      {
      GirderDimensions *girder = (GirderDimensions *)list->GetItemData(i);
      delete girder;
      list->SetItemData(i,0);
      }

   delete useList;
   delete list;
   delete ax;
   delete iz;
   delete yb;
   delete h;
   delete tfWidth;
   delete wWeb;
   delete unitWgt;
   delete density;
   delete fc;
   delete location;
   delete xFrames;
   delete spanLength;
   delete spacing;
   delete numBeams;
   delete numLanes;
}

void TSectPropWizard::Page3c::SetupWindow()
{
   TWizardStep::SetupWindow();

   if(wizard->data.dispUnits == unitsSI)
      SetDlgItemText(IDC_UNITWGT_LABEL,"Density");
   else
      SetDlgItemText(IDC_UNITWGT_LABEL,"Unit Wgt");

   GirderDimensions *girder;

   girder = new GirderDimensions(1.092,0.073,0.067,0.051,1.448,0.076,0.152,1.867,0.635,0.152,0.152,0.051,0.419,0.241);
   list->SetItemData(list->AddString("W74G"),(int32)girder);

   girder = new GirderDimensions(0.635,0.127,0.051,0.000,1.067,0.076,0.152,1.473,0.635,0.152,0.152,0.000,0.241,0.241);
   list->SetItemData(list->AddString("W58G"),(int32)girder);

   girder = new GirderDimensions(0.508,0.127,0.051,0.000,0.864,0.076,0.152,1.270,0.635,0.152,0.152,0.000,0.241,0.241);
   list->SetItemData(list->AddString("W50G"),(int32)girder);

   girder = new GirderDimensions(0.381,0.089,0.038,0.000,0.762,0.051,0.152,1.067,0.508,0.152,0.152,0.000,0.178,0.178);
   list->SetItemData(list->AddString("W42G"),(int32)girder);

   LoadBeamTable();

   list->SetSelIndex(0); // select the first item

   useList->SetCheck(BF_CHECKED);
   BNClicked_BeamCheck();

   location->CheckRadioButton(RGB_BYCOMMAND,IDC_INTGIRDER);
   BNClicked_GirderLocation();

   spanLength->SetValue(wizard->data.spanLength);

   if(wizard->data.dispUnits == unitsSI)
      {
      density->SetValueEx(2560); // 2560 kg/m^3 (~160pcf)
      fc->SetValueEx(48); // 48MPa (~7000psi)
      spacing->SetValueEx(3660.); // ~12ft
      }
   else
      {
      unitWgt->SetValueEx(160); // 160pcf
      fc->SetValueEx(7000); /// 7000psi
      spacing->SetValueEx(12.);
      }
   numBeams->SetValue(4);
   numLanes->SetValue(3);
}

void
TSectPropWizard::Page3c::BNClicked_BeamCheck()
{
   bool enableList;
   bool enableProperties;
   if (useList->GetCheck() == BF_CHECKED)
      {
      enableList = true;
      enableProperties = false;
      }
   else
      {
      enableList = false;
      enableProperties = true;
      }

   list->EnableWindow(enableList);
   TWindow(GetDlgItem(IDC_PROPERTIES_GROUP)).EnableWindow(enableProperties);
   TWindow(GetDlgItem(IDC_AX_LABEL)).EnableWindow(enableProperties);
   ax->EnableWindow(enableProperties);
   TWindow(GetDlgItem(IDC_IZ_LABEL)).EnableWindow(enableProperties);
   iz->EnableWindow(enableProperties);
   TWindow(GetDlgItem(IDC_YB_LABEL)).EnableWindow(enableProperties);
   yb->EnableWindow(enableProperties);
   TWindow(GetDlgItem(IDC_HEIGHT_LABEL)).EnableWindow(enableProperties);
   h->EnableWindow(enableProperties);
   tfWidth->EnableWindow(enableProperties);
   TWindow(GetDlgItem(IDC_TOPFLGWIDTH_LABEL)).EnableWindow(enableProperties);
   wWeb->EnableWindow(enableProperties);
   TWindow(GetDlgItem(IDC_WEBWIDTH_LABEL)).EnableWindow(enableProperties);
}

void
TSectPropWizard::Page3c::BNClicked_GirderLocation()
{
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
TSectPropWizard::Page3c::GetModE() const
{
   double fc = DerivedUnits::Pressure(GetFc()*SIConstants::Pascal).ConvertTo(SIConstants::MPa).value();
   double E;
   E = EConcrete(GetDensity(),fc);
   return DerivedUnits::Pressure(E*SIConstants::MPa).ConvertTo(SIConstants::Pascal).value();
}

TShape*
TSectPropWizard::Page3c::GetBeam() const
{
   TShape* shape;
   GeneralMaterial* material;

   material = new GeneralMaterial(GetModE(),GetUnitWgt());

   if(UseList())
      {
      GirderDimensions* dim;

      dim = (GirderDimensions *)list->GetItemData(list->GetSelIndex());
      shape = new TIBeam(dim->A,dim->B,dim->C,dim->D,dim->E,dim->F,dim->G,
                         dim->H,dim->J,dim->V,dim->W,dim->X,dim->Y,dim->Z,
                         material);
      }
   else
      {
      shape = new GeneralShape(GetAx(),0.00,GetYbar(),GetIz(),0.00,0.00,GetHeight(),0.00,material);
      }

   shape->ComputeProperties();

   return shape;
}

double
TSectPropWizard::Page3c::GetWebThickness() const
{
   double webThickness;

   if(UseList())
      {
      GirderDimensions* dim;

      dim = (GirderDimensions *)list->GetItemData(list->GetSelIndex());
      webThickness = dim->W;
      }
   else
      {
      webThickness = wWeb->GetValue();
      }

   return webThickness;
}

double
TSectPropWizard::Page3c::GetTopFlangeWidth() const
{
   double topFlangWidth;

   if(UseList())
      {
      GirderDimensions* dim;

      dim = (GirderDimensions *)list->GetItemData(list->GetSelIndex());
      topFlangWidth = dim->A;
      }
   else
      {
      topFlangWidth = tfWidth->GetValue();
      }

   return topFlangWidth;
}

double
TSectPropWizard::Page3c::GetHeight() const
{
   double height;

   if(UseList())
      {
      GirderDimensions* dim;

      dim = (GirderDimensions *)list->GetItemData(list->GetSelIndex());
      height = dim->H;
      }
   else
      {
      height = h->GetValue();
      }

   return height;
}

void
TSectPropWizard::Page3c::LoadBeamTable()
{
   GirderDimensions *girder;
   char buffer[256];
   char fname[MAXPATH];
   char name[21];
   double A,B,C,D,E,F,G,H,J,V,W,X,Y,Z;
   char regKey[256];

   TQConBridgeApp* app = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   app->GetRegistryKey(0,regKey,sizeof(regKey));

   uint32 size = sizeof(fname);
   if (TRegKey(TRegKey::CurrentUser(),regKey).QueryValue("Girder Table",0,(uint8*)&fname,&size) != ERROR_SUCCESS)
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
         getstg(buffer,1,20,name);
         A = getf(buffer,22,5);
         B = getf(buffer,29,5);
         C = getf(buffer,36,5);
         D = getf(buffer,43,5);
         E = getf(buffer,50,5);
         F = getf(buffer,57,5);
         G = getf(buffer,64,5);
         H = getf(buffer,71,5);
         J = getf(buffer,78,5);
         W = getf(buffer,85,5);
         V = W;
         X = getf(buffer,92,5);
         Y = getf(buffer,99,5);
         Z = getf(buffer,106,5);

         girder = new GirderDimensions(A,B,C,D,E,F,G,H,J,V,W,X,Y,Z);
         list->SetItemData(list->AddString(name),(int32)girder);
         }
      }
}

void
TSectPropWizard::Page3c::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Beam_Girder_Description_Concrete_Beam);
}

