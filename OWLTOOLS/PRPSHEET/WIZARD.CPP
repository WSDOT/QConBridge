///////////////////////////////////////////////////////////////////////
// OWLTools - Extension Library for the Borland OWL Framework
// Copyright (C) 1999  Washington State Department of Transportation
//                     Bridge and Structures Office
//
// This library was developed as part of the Alternate Route Project
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the Alternate Route Library Open Source License as published by 
// the Washington State Department of Transportation, Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but is distributed 
// AS IS, WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
// or FITNESS FOR A PARTICULAR PURPOSE. See the Alternate Route Library Open Source 
// License for more details.
//
// You should have received a copy of the Alternate Route Library Open Source License 
// along with this program; if not, write to the Washington State Department of 
// Transportation, Bridge and Structures Office, 4500 3rd Ave SE - P.O. Box  47340, 
// Olympia, WA 98503, USA or e-mail Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Revision Log
// ============
// May 1997 - Created, Richard Brice
// Oct 1999 - Released as open source

#include <owltools\wizard.h>

TWizardStep::TWizardStep(TWizard* wizard, TResId resid, const char far*title,
               TResId iconRes, TModule* module) :
   TMyPropertyPage(wizard,resid,title,iconRes,module)
{
   enabled = true;
}

TWizardStep::~TWizardStep()
{
}

bool TWizardStep::WizFinish(TNotify far& notifyInfo)
{
   bool result = TMyPropertyPage::WizFinish(notifyInfo);

   if (!result) // Parent says it ok to finish, but lets apply first
      result = !GetSheet()->Apply(); // Apply causes the data to be transfered

   return result;
}

void TWizardStep::EnableStep(bool enable)
{
   enabled = enable ? true : false;
}

bool TWizardStep::IsStepEnabled() const
{
   return enabled;
}

int TWizardStep::SetActive(TNotify far& notifyInfo)
{
   int result = TMyPropertyPage::SetActive(notifyInfo);

   if (result != 0)
      return result;

   PROPSHEETHEADER& psh = GetSheet()->GetPropSheetHeader();
   if (psh.dwFlags & PSH_WIZARD)
      UpdateButtons();

   int activeIndex;
   int index = GetParentSheet()->IndexOfPage(this);
   int firstPage = 0;
   int lastPage = GetParentSheet()->GetPageCount() - 1;

   if (index == firstPage && !IsStepEnabled())
      activeIndex = 1;
   else if (index == lastPage && !IsStepEnabled())
      activeIndex = lastPage-1;
   else if (!IsStepEnabled())
      activeIndex = -1;
   else
      activeIndex = 0;

   return activeIndex;
}

void TWizardStep::UpdateButtons()
{
   uint32 flags;

   flags  = GetNextStep() ? PSWIZB_NEXT : PSWIZB_FINISH;
   flags |= GetPrevStep() ? PSWIZB_BACK : 0;

   TWizard* wizard = TYPESAFE_DOWNCAST(GetSheet(),TWizard);
   wizard->SetWizButtons(flags);
}


TWizardStep* TWizardStep::GetNextStep() const
{
   TWizard* wizard = TYPESAFE_DOWNCAST(GetSheet(),TWizard);
   TWizardStep* nextStep;

   int index = wizard->IndexOfPage(this);
   int lastPage = wizard->GetPageCount() - 1;

   if (index != lastPage)
      {
      nextStep = TYPESAFE_DOWNCAST(wizard->PageAtIndex(index+1),TWizardStep);
      if (nextStep && !nextStep->IsStepEnabled())
         nextStep = nextStep->GetNextStep();
      }
   else
      nextStep = 0;

   return nextStep;
}

TWizardStep* TWizardStep::GetPrevStep() const
{
   TWizard* wizard = TYPESAFE_DOWNCAST(GetSheet(),TWizard);
   TWizardStep* prevStep;

   int index = wizard->IndexOfPage(this);
   int firstPage = 0;

   if (index != firstPage)
      {
      prevStep = TYPESAFE_DOWNCAST(wizard->PageAtIndex(index-1),TWizardStep);
      if (prevStep && !prevStep->IsStepEnabled())
         prevStep = prevStep->GetPrevStep();
      }
   else
      prevStep = 0;

   return prevStep;
}

//
//-----------------------------------------------------------------------
//


TWizard::TWizard(TWindow* parent,const char far* title,uint startPage,uint32 flags ,TModule* module) :
   TMyPropertySheet(parent,title,startPage,true,flags,module)
{
}

TWizard::~TWizard()
{
}

void TWizard::EnableStep(int index,bool enable)
{
   TWizardStep* step = TYPESAFE_DOWNCAST(PageAtIndex(index),TWizardStep);
   int current = IndexOfPage(GetActivePage());

   if (current != index)
      step->EnableStep(enable);
   else
      {
      // current == index
      if (enable)
         step->EnableStep(enable);
      else
         {
         // Attempting to disable current step
         TWizardStep* nextStep = step->GetNextStep();
         TWizardStep* prevStep = step->GetPrevStep();
         if (nextStep)
            {
            SetActivePage(IndexOfPage(nextStep));
            step->EnableStep(enable);
            }
         else if (prevStep)
            {
            SetActivePage(IndexOfPage(prevStep));
            step->EnableStep(enable);
            }
         }
      }
}

bool TWizard::IsStepEnabled(int index)
{
   TWizardStep* step = TYPESAFE_DOWNCAST(PageAtIndex(index),TWizardStep);
   return step->IsStepEnabled();
}

