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


DEFINE_RESPONSE_TABLE1(TSectPropWizard::Page1,TWizardStep)
   EV_BN_CLICKED(IDC_STEELBEAM,EvTypeChanged),
   EV_BN_CLICKED(IDC_CLOSEDBOX,EvTypeChanged),
   EV_BN_CLICKED(IDC_OPENBOX,EvTypeChanged),
   EV_BN_CLICKED(IDC_CIPBOX,EvTypeChanged),
   EV_BN_CLICKED(IDC_CIPTEE,EvTypeChanged),
   EV_BN_CLICKED(IDC_PRECASTSLAB_COMP,EvTypeChanged),
   EV_BN_CLICKED(IDC_PRECASTSLAB_INTG,EvTypeChanged),
   EV_BN_CLICKED(IDC_PRECASTCHANNEL,EvTypeChanged),
   EV_BN_CLICKED(IDC_PRECASTTEE,EvTypeChanged),
   EV_BN_CLICKED(IDC_PRECASTIBEAM,EvTypeChanged),
   EV_BN_CLICKED(IDC_WOOD,EvTypeChanged),
END_RESPONSE_TABLE;

TSectPropWizard::Page1::Page1(TSectPropWizard *wiz,TModule *module) :
TWizardStep(wiz,IDD_SECTPRPWIZARD_PAGE1,"Supporting Components",0,module),
wizard(wiz)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   beamType = new TRadioGroupBox(this,IDC_GIRDERGRP,IDC_STEELBEAM,IDC_WOOD);
}

TSectPropWizard::Page1::~Page1()
{
   delete beamType;
}

void TSectPropWizard::Page1::SetupWindow()
{
   TWizardStep::SetupWindow();

   // Disable supporting components that are not supported
   beamType->EnableRadioButton(RGB_BYCOMMAND,IDC_STEELBEAM,true);
   beamType->EnableRadioButton(RGB_BYCOMMAND,IDC_CLOSEDBOX,false);
   beamType->EnableRadioButton(RGB_BYCOMMAND,IDC_OPENBOX,false);
   beamType->EnableRadioButton(RGB_BYCOMMAND,IDC_CIPBOX,false);
   beamType->EnableRadioButton(RGB_BYCOMMAND,IDC_CIPTEE,false);
   beamType->EnableRadioButton(RGB_BYCOMMAND,IDC_PRECASTSLAB_COMP,false);
   beamType->EnableRadioButton(RGB_BYCOMMAND,IDC_PRECASTSLAB_INTG,false);
   beamType->EnableRadioButton(RGB_BYCOMMAND,IDC_PRECASTCHANNEL,false);
   beamType->EnableRadioButton(RGB_BYCOMMAND,IDC_PRECASTDOUBLETEE,false);
   beamType->EnableRadioButton(RGB_BYCOMMAND,IDC_PRECASTTEE,false);
   beamType->EnableRadioButton(RGB_BYCOMMAND,IDC_PRECASTIBEAM,true);
   beamType->EnableRadioButton(RGB_BYCOMMAND,IDC_WOOD,true);

   TWindow(GetDlgItem(IDC_PRECASTSLAB_INTG_TAG)).EnableWindow(false);
   TWindow(GetDlgItem(IDC_PRECASTDOUBLETEE_TAG)).EnableWindow(false);
   TWindow(GetDlgItem(IDC_PRECASTTEE_TAG)).EnableWindow(false);

   beamType->CheckRadioButton(RGB_BYPOSITION,0);

   EvTypeChanged();
}

void
TSectPropWizard::Page1::EvTypeChanged()
{
   // Disable all component description pages and then
   // enable the correct one based upon the selected beam type
   wizard->EnableStep(IDD_PAGE3A,false);
   wizard->EnableStep(IDD_PAGE3B,false);
   wizard->EnableStep(IDD_PAGE3C,false);

   switch(beamType->GetSelRadioButton())
      {
      case IDC_STEELBEAM:
           wizard->EnableStep(IDD_PAGE3B,true);
           break;

      case IDC_PRECASTIBEAM:
           wizard->EnableStep(IDD_PAGE3C,true);
           break;

      case IDC_WOOD:
           wizard->EnableStep(IDD_PAGE3A,true);
           break;

      case IDC_CLOSEDBOX:
      case IDC_OPENBOX:
      case IDC_CIPBOX:
      case IDC_CIPTEE:
      case IDC_PRECASTSLAB_COMP:
      case IDC_PRECASTSLAB_INTG:
      case IDC_PRECASTCHANNEL:
      case IDC_PRECASTDOUBLETEE:
      case IDC_PRECASTTEE:
           break; // do nothing, type not supported

      default:
           break; // Throw an exception, non-supported types
      }
}

void
TSectPropWizard::Page1::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Supporting_Components);
}

