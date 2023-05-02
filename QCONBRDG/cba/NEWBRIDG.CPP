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


#include <checks.h>
#include <qconbrdg\newbridg.h>
#include <qconbrdg\sprpwiz.h>
#include <qconbrdg\gt0valid.h>

/* -------------------------------------------------------------------------
NOTICE of some "GOTCHA" code...

On Page3 and Page4, there is one control each for density and unit weight.
Two controls must be used in this case because the posibility of the system
of units being changed on the file exists. As such, great care was taken to
disable/enable the transfer mechanism of the edit control not being used. In
addition, great care was taken to hide/show the edit control not being used.
-----------------------------------------------------------------------------*/


//{{TNewBridgeWizard Implementation}}


TNewBridgeWizard::TNewBridgeWizard (TWindow* parent,SystemOfUnits units, TModule* module) :
TWizard(parent,"Bridge Contractor",0,PSH_DEFAULT | PSH_HASHELP, module)
{
    // INSERT>> Your constructor code here.
   step1 = new TWizardPage1(this);
   step2 = new TWizardPage2(this,units);
   step3 = new TWizardPage3(this,units,true);
   step4 = new TWizardPage3(this,units,false);
   step5 = new TWizardPage4(this,units);
   step6 = new TWizardPage5(this);

   AddPage(*step1);
   AddPage(*step2);
   AddPage(*step3);
   AddPage(*step4);
   AddPage(*step5);
   AddPage(*step6);
}


TNewBridgeWizard::~TNewBridgeWizard ()
{
    Destroy();

    delete step1;
    delete step2;
    delete step3;
    delete step4;
    delete step5;
    delete step6;
}

void TNewBridgeWizard::SetupTransferBuffer(TNewBridgeXferBuffer *buffer)
{
   step1->SetTransferBuffer(&buffer->step1);
   step2->SetTransferBuffer(&buffer->step2);
   step3->SetTransferBuffer(&buffer->step3);
   step4->SetTransferBuffer(&buffer->step4);
   step5->SetTransferBuffer(&buffer->step5);
}

TNewBridgeWizard::TWizardPage1* TNewBridgeWizard::GetStep1()
{
   return TYPESAFE_DOWNCAST(PageAtIndex(0),TWizardPage1);
}

TNewBridgeWizard::TWizardPage2* TNewBridgeWizard::GetStep2()
{
   return TYPESAFE_DOWNCAST(PageAtIndex(1),TWizardPage2);
}

TNewBridgeWizard::TWizardPage3* TNewBridgeWizard::GetStep3()
{
   return TYPESAFE_DOWNCAST(PageAtIndex(2),TWizardPage3);
}

TNewBridgeWizard::TWizardPage3* TNewBridgeWizard::GetStep4()
{
   return TYPESAFE_DOWNCAST(PageAtIndex(3),TWizardPage3);
}

TNewBridgeWizard::TWizardPage4* TNewBridgeWizard::GetStep5()
{
   return TYPESAFE_DOWNCAST(PageAtIndex(4),TWizardPage4);
}

TNewBridgeWizard::TWizardPage5* TNewBridgeWizard::GetStep6()
{
   return TYPESAFE_DOWNCAST(PageAtIndex(5),TWizardPage5);
}

