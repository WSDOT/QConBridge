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


TSectPropWizard::Page4b::Page4b(TSectPropWizard *wiz,TModule *module) :
TWizardStep(wiz,IDD_SECTPRPWIZARD_PAGE4B,"Steel Grid Deck Description",0,module),
wizard(wiz)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   if(wizard->data.dispUnits == unitsSI)
      {
      tg = new TLengthEdit(this,IDC_TG,SIConstants::millimeter,11,3,IDC_TG_TAG,"mm");
      de = new TLengthEdit(this,IDC_DE,SIConstants::millimeter,11,3,IDC_DE_TAG,"mm");
      weight = new TLinearForceEdit(this,IDC_WEIGHT,SIConstants::newton_per_meter,11,3,IDC_WEIGHT_TAG,"N/m");
      designLane = new TLengthEdit(this,IDC_DESIGNLANE,SIConstants::millimeter,11,3,IDC_DESIGNLANE_TAG,"mm");
      }
   else
      {
      tg = new TLengthEdit(this,IDC_TG,USConstants::inch,11,3,IDC_TG_TAG,"inch");
      de = new TLengthEdit(this,IDC_DE,USConstants::feet,11,3,IDC_DE_TAG,"feet");
      weight = new TLinearForceEdit(this,IDC_WEIGHT,USConstants::pound_per_feet,11,3,IDC_WEIGHT_TAG,"lbs/ft");
      designLane = new TLengthEdit(this,IDC_DESIGNLANE,USConstants::feet,11,3,IDC_DESIGNLANE_TAG,"feet");
      }
}

TSectPropWizard::Page4b::~Page4b()
{
   delete tg;
   delete de;
   delete weight;
   delete designLane;
}

void TSectPropWizard::Page4b::SetupWindow()
{
   TWizardStep::SetupWindow();

   if(wizard->data.dispUnits == unitsSI)
      {
      designLane->SetValueEx(3600.);
      de->SetValueEx(910.);
      }
   else
      {
      designLane->SetValueEx(12.);
      de->SetValueEx(3.);
      }
}

bool
TSectPropWizard::Page4b::IsInteriorGirder()
{
   return (wizard->page3b->GetGirderLocation() == IDC_INTGIRDER);
}

int
TSectPropWizard::Page4b::SetActive(TNotify far& nmHdr)
{
   int retval = TWizardStep::SetActive(nmHdr);
   bool enable = !IsInteriorGirder();

   TWindow(GetDlgItem(IDC_DE_LABEL)).EnableWindow(enable);
   de->EnableWindow(enable);

   return retval;
}

void
TSectPropWizard::Page4b::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Steel_Grid_Deck_Description);
}

