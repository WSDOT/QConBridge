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



TSectPropWizard::Page4a::Page4a(TSectPropWizard *wiz,TModule *module) :
TWizardStep(wiz,IDD_SECTPRPWIZARD_PAGE4A,"Wood/Concrete Deck Description",0,module),
wizard(wiz)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   if(wizard->data.dispUnits == unitsSI)
      {
      width = new TLengthEdit(this,IDC_WIDTH,SIConstants::millimeter,11,3,IDC_WIDTH_TAG,"mm");
      depth = new TLengthEdit(this,IDC_DEPTH,SIConstants::millimeter,11,3,IDC_DEPTH_TAG,"mm");
      de    = new TLengthEdit(this,IDC_DE,SIConstants::millimeter,11,3,IDC_DE_TAG,"mm");
      unitWgt = 0;
      density = new TMaterialDensityEdit(this,IDC_UNITWGT,SIConstants::kilogram_per_meter3,11,3,IDC_UNITWGT_TAG,"kg/m^3");
      designLane = new TLengthEdit(this,IDC_DESIGNLANE,SIConstants::millimeter,11,3,IDC_DESIGNLANE_TAG,"mm");
      }
   else
      {
      width = new TLengthEdit(this,IDC_WIDTH,USConstants::inch,11,3,IDC_WIDTH_TAG,"inch");
      depth = new TLengthEdit(this,IDC_DEPTH,USConstants::inch,11,3,IDC_DEPTH_TAG,"inch");
      de    = new TLengthEdit(this,IDC_DE,USConstants::feet,11,3,IDC_DE_TAG,"feet");
      unitWgt = new TUnitWeightEdit(this,IDC_UNITWGT,USConstants::pound_per_feet3,11,3,IDC_UNITWGT_TAG,"pcf");
      density = 0;
      designLane = new TLengthEdit(this,IDC_DESIGNLANE,USConstants::feet,11,3,IDC_DESIGNLANE_TAG,"feet");
      }
}

TSectPropWizard::Page4a::~Page4a()
{
   delete width;
   delete depth;
   delete de;
   delete unitWgt;
   delete density;
   delete designLane;
}

void TSectPropWizard::Page4a::SetupWindow()
{
   TWizardStep::SetupWindow();

   if(wizard->data.dispUnits == unitsSI)
      {
      SetDlgItemText(IDC_UNITWGT_LABEL,"Density");
      density->SetValueEx(960.);
      designLane->SetValueEx(3600.);
      de->SetValueEx(910.);
      }
   else
      {
      SetDlgItemText(IDC_UNITWGT_LABEL,"Unit Wgt");
      unitWgt->SetValueEx(60.);
      designLane->SetValueEx(12.);
      de->SetValueEx(3.);
      }
}

bool
TSectPropWizard::Page4a::IsInteriorGirder()
{
   bool isInterior;

   switch(wizard->page1->GetBeamType())
      {
      case IDC_STEELBEAM:
           isInterior = (wizard->page3b->GetGirderLocation() == IDC_INTGIRDER);
           break;

      case IDC_WOOD:
           isInterior = (wizard->page3a->GetGirderLocation() == IDC_INTGIRDER);
           break;

      default:
           break;
      }

   return isInterior;
}

int
TSectPropWizard::Page4a::SetActive(TNotify far& nmHdr)
{
   char type[25];
   int retval = TWizardStep::SetActive(nmHdr);

   wizard->page2->GetDeckType(type,sizeof(type));
   SetDlgItemText(IDC_DECKTYPE,type);

   bool enable = !IsInteriorGirder();

   TWindow(GetDlgItem(IDC_DE_LABEL)).EnableWindow(enable);
   de->EnableWindow(enable);

   return retval;
}

void
TSectPropWizard::Page4a::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Wood_Deck_Description);
}

