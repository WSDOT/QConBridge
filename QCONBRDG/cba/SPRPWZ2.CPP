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

DEFINE_RESPONSE_TABLE1(TSectPropWizard::Page2,TWizardStep)
   EV_BN_CLICKED(IDC_CONCRETE,EvTypeChanged),
   EV_BN_CLICKED(IDC_STEEL_GRID,EvTypeChanged),
   EV_BN_CLICKED(IDC_FILLED_GRID,EvTypeChanged),
   EV_BN_CLICKED(IDC_WOOD_PLANK,EvTypeChanged),
   EV_BN_CLICKED(IDC_STRESS_LAMINATED,EvTypeChanged),
   EV_BN_CLICKED(IDC_SPIKE_LAMINATED,EvTypeChanged),
   EV_BN_CLICKED(IDC_GLUE_LAMINATED,EvTypeChanged),
END_RESPONSE_TABLE;

TSectPropWizard::Page2::Page2(TSectPropWizard *wiz,TModule *module) :
TWizardStep(wiz,IDD_SECTPRPWIZARD_PAGE2,"Deck Types",0,module),
wizard(wiz)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   deckType = new TRadioGroupBox(this,IDC_DECKTYPES,IDC_CONCRETE,IDC_GLUE_LAMINATED);
}

TSectPropWizard::Page2::~Page2()
{
   delete deckType;
}

void TSectPropWizard::Page2::SetupWindow()
{
   TWizardStep::SetupWindow();

   switch (wizard->page1->GetBeamType())
      {
      case IDC_STEELBEAM:
           deckType->CheckRadioButton(RGB_BYCOMMAND,IDC_CONCRETE);
           break;

      case IDC_PRECASTIBEAM:
           deckType->CheckRadioButton(RGB_BYCOMMAND,IDC_CONCRETE);
           break;

      case IDC_WOOD:
           deckType->CheckRadioButton(RGB_BYCOMMAND,IDC_WOOD_PLANK);
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
      default:
           break; // Throw an exception, non-supported types
      }

   EvTypeChanged();
}

void
TSectPropWizard::Page2::GetDeckType(char* type,int max)
{
   int id = deckType->GetSelRadioButton();
   GetDlgItemText(id,type,max);
}

int
TSectPropWizard::Page2::SetActive(TNotify far& nmHdr)
{
   int retval = TWizardStep::SetActive(nmHdr);

   int defaultSelection;

   // Disable all deck types, then re-enable those that apply
   // to the selected beam type
   deckType->EnableRadioButton(RGB_BYCOMMAND,IDC_CONCRETE,false);
   deckType->EnableRadioButton(RGB_BYCOMMAND,IDC_STEEL_GRID,false);
   deckType->EnableRadioButton(RGB_BYCOMMAND,IDC_FILLED_GRID,false);
   deckType->EnableRadioButton(RGB_BYCOMMAND,IDC_WOOD_PLANK,false);
   deckType->EnableRadioButton(RGB_BYCOMMAND,IDC_STRESS_LAMINATED,false);
   deckType->EnableRadioButton(RGB_BYCOMMAND,IDC_SPIKE_LAMINATED,false);
   deckType->EnableRadioButton(RGB_BYCOMMAND,IDC_GLUE_LAMINATED,false);

   switch(wizard->page1->GetBeamType())
      {
      case IDC_STEELBEAM:
           deckType->EnableRadioButton(RGB_BYCOMMAND,IDC_CONCRETE,true);
           deckType->EnableRadioButton(RGB_BYCOMMAND,IDC_STEEL_GRID,true);
           deckType->EnableRadioButton(RGB_BYCOMMAND,IDC_STRESS_LAMINATED,true);
           deckType->EnableRadioButton(RGB_BYCOMMAND,IDC_SPIKE_LAMINATED,true);
           deckType->EnableRadioButton(RGB_BYCOMMAND,IDC_GLUE_LAMINATED,true);
           defaultSelection = IDC_CONCRETE;
           break;

      case IDC_PRECASTIBEAM:
           deckType->EnableRadioButton(RGB_BYCOMMAND,IDC_CONCRETE,true);
           defaultSelection = IDC_CONCRETE;
           break;

      case IDC_WOOD:
           deckType->EnableRadioButton(RGB_BYCOMMAND,IDC_CONCRETE,true);
           deckType->EnableRadioButton(RGB_BYCOMMAND,IDC_WOOD_PLANK,true);
           deckType->EnableRadioButton(RGB_BYCOMMAND,IDC_STRESS_LAMINATED,true);
           deckType->EnableRadioButton(RGB_BYCOMMAND,IDC_SPIKE_LAMINATED,true);
           deckType->EnableRadioButton(RGB_BYCOMMAND,IDC_GLUE_LAMINATED,true);
           defaultSelection = IDC_CONCRETE;
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
      default:
           break; // Throw an exception, non-supported types
      }

   if (!deckType->GetRadioButton(RGB_BYCOMMAND,deckType->GetSelRadioButton())->IsWindowEnabled())
      {
      deckType->GetRadioButton(RGB_BYCOMMAND,deckType->GetSelRadioButton())->SetCheck(BF_UNCHECKED);
      deckType->CheckRadioButton(RGB_BYCOMMAND,defaultSelection);
      }

   EvTypeChanged();

   return retval;
}

void
TSectPropWizard::Page2::EvTypeChanged()
{
   // Disable all deck description pages and then
   // enable the correct one based upon the selected beam type
   wizard->EnableStep(IDD_PAGE4A,false);
   wizard->EnableStep(IDD_PAGE4B,false);
   wizard->EnableStep(IDD_PAGE4C,false);
   wizard->EnableStep(IDD_PAGE5,false);
   wizard->EnableStep(IDD_PAGE6A,false);
   wizard->EnableStep(IDD_PAGE6B,false);

   int beamType = wizard->page1->GetBeamType();

   switch(deckType->GetSelRadioButton())
      {
      case IDC_CONCRETE:
           wizard->EnableStep((beamType == IDC_WOOD) ? IDD_PAGE4A : IDD_PAGE4C,true); // concrete slab
           wizard->EnableStep((beamType == IDC_WOOD) ? IDD_PAGE6A : IDD_PAGE6B,true);

           // Skew angle is only available if steel or precast I-beam
           // with concrete deck
           if (beamType == IDC_STEELBEAM || beamType == IDC_PRECASTIBEAM)
              wizard->EnableStep(IDD_PAGE5,true);

           break;

      case IDC_STEEL_GRID:
           wizard->EnableStep(IDD_PAGE4B,true); // Steel grid deck
           wizard->EnableStep(IDD_PAGE6A,true); // Non-composite girder results
           break;

      case IDC_WOOD_PLANK:
      case IDC_STRESS_LAMINATED:
      case IDC_SPIKE_LAMINATED:
      case IDC_GLUE_LAMINATED:
           wizard->EnableStep(IDD_PAGE4A,true); // Wood deck
           wizard->EnableStep(IDD_PAGE6A,true); // Non-composite girder results
           break;

      case IDC_FILLED_GRID:
           break; // do nothing, type not supported

      default:
           break; // Throw an exception, non-supported types
      }
}


void
TSectPropWizard::Page2::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Deck_Types);
}

