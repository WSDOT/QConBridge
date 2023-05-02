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
#include <Context.hh>

#include <shape\genshape.h>
#include <shape\plategdr.h>
#include <material\genmat.h>
#include <code\lrfdcode.h>
#include <esl\strutil.h>
#include <code\lldist.h>
#include <esl\engnotat.h>
#include <owl\printer.h>
#include <owl\profile.h>
#include <dir.h> // For MAXPATH



TSectPropWizard::Page3a::Page3a(TSectPropWizard *wiz,TModule *module) :
TWizardStep(wiz,IDD_SECTPRPWIZARD_PAGE3A,"Wood Beams",0,module),
wizard(wiz)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   if(wizard->data.dispUnits == unitsSI)
      {
      depth = new TLengthEdit(this,IDC_DEPTH,SIConstants::millimeter,11,3,IDC_DEPTH_TAG,"mm");
      width = new TLengthEdit(this,IDC_WIDTH,SIConstants::millimeter,11,3,IDC_WIDTH_TAG,"mm");
      unitWgt = 0;
      density = new TMaterialDensityEdit(this,IDC_UNITWGT,SIConstants::kilogram_per_meter3,11,3,IDC_UNITWGT_TAG,"kg/m^3");
      modE = new TPressureEdit(this,IDC_MODE,SIConstants::MPa,11,3,IDC_MODE_TAG,"MPa");
      location = new TRadioGroupBox(this,IDC_LOCATIONGRP,IDC_INTGIRDER,IDC_EXTGIRDER);
      spacing = new TLengthEdit(this,IDC_SPACING,SIConstants::millimeter,11,3,IDC_SPACING_TAG,"mm");
      }
   else
      {
      depth = new TLengthEdit(this,IDC_DEPTH,USConstants::inch,11,3,IDC_DEPTH_TAG,"inch");
      width = new TLengthEdit(this,IDC_WIDTH,USConstants::inch,11,3,IDC_WIDTH_TAG,"inch");
      unitWgt = new TUnitWeightEdit(this,IDC_UNITWGT,USConstants::pound_per_feet3,11,3,IDC_UNITWGT_TAG,"pcf");
      density = 0;
      modE = new TPressureEdit(this,IDC_MODE,USConstants::pound_per_inch2,11,3,IDC_MODE_TAG,"psi");
      location = new TRadioGroupBox(this,IDC_LOCATIONGRP,IDC_INTGIRDER,IDC_EXTGIRDER);
      spacing = new TLengthEdit(this,IDC_SPACING,USConstants::feet,11,3,IDC_SPACING_TAG,"feet");
      }
}

TSectPropWizard::Page3a::~Page3a()
{
   delete depth;
   delete width;
   delete unitWgt;
   delete density;
   delete modE;
   delete location;
   delete spacing;
}

void TSectPropWizard::Page3a::SetupWindow()
{
   TWizardStep::SetupWindow();

   if(wizard->data.dispUnits == unitsSI)
      {
      SetDlgItemText(IDC_UNITWGT_LABEL,"Density");
      density->SetValueEx(960.);
      modE->SetValueEx(11000.); // MPa
      spacing->SetValueEx(3660.); // ~12ft
      }
   else
      {
      SetDlgItemText(IDC_UNITWGT_LABEL,"Unit Wgt");
      unitWgt->SetValueEx(60.);
      modE->SetValueEx(1000000.); // psi
      spacing->SetValueEx(12.);
      }

   location->CheckRadioButton(RGB_BYCOMMAND,IDC_INTGIRDER);
}

TShape*
TSectPropWizard::Page3a::GetBeam()
{
   GeneralMaterial* material = new GeneralMaterial(GetModE(),GetUnitWgt());
   TRectangle* shape = new TRectangle(GetDepth(),GetWidth(),material,0.,TFPoint(0,0),BottomCenter);
   shape->ComputeProperties();
   return shape;
}

void
TSectPropWizard::Page3a::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Wood_Beam_Girder_Description);
}

