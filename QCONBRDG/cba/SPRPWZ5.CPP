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


DEFINE_RESPONSE_TABLE1(TSectPropWizard::Page5,TWizardStep)
   EV_BN_CLICKED(IDC_CORRECTMOMENT,BNClicked_CorrectForSkew),
   EV_BN_CLICKED(IDC_CORRECTSHEAR,BNClicked_CorrectForSkew),
END_RESPONSE_TABLE;

TSectPropWizard::Page5::Page5(TSectPropWizard *wiz,TModule *module) :
TWizardStep(wiz,IDD_SECTPRPWIZARD_PAGE5,"Skew Corrections",0,module),
wizard(wiz)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   correctMoment = new TCheckBox(this,IDC_CORRECTMOMENT);
   correctShear  = new TCheckBox(this,IDC_CORRECTSHEAR);
   angle = new TNumberEdit(this,IDC_SKEWANGLE,11,3);
}

TSectPropWizard::Page5::~Page5()
{
   delete correctMoment;
   delete correctShear;
   delete angle;
}

void TSectPropWizard::Page5::SetupWindow()
{
   TWizardStep::SetupWindow();

   BNClicked_CorrectForSkew();
}

void TSectPropWizard::Page5::BNClicked_CorrectForSkew()
{
   bool enable;

   if (correctMoment->GetCheck() == BF_CHECKED ||
       correctShear->GetCheck()  == BF_CHECKED)
      enable = true;
   else
      enable = false;

   TWindow(GetDlgItem(IDC_SKEWANGLE_LABEL)).EnableWindow(enable);
   angle->EnableWindow(enable);
   TWindow(GetDlgItem(IDC_SKEWANGLE_TAG)).EnableWindow(enable);
}

void
TSectPropWizard::Page5::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Skew_Corrections);
}

