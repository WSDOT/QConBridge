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

#if !defined(__legalwiz_h)              // Sentry, use file only if it's not already included.
#define __legalwiz_h

#include <qconbrdg\qcbapp.rh>

#include <owltools\wizard.h>
#include <owl\edit.h>
#include <owl\checkbox.h>

class TLegalWizard : public TWizard
{
public:
    TLegalWizard (TWindow* parent,TModule* module = 0);
    virtual ~TLegalWizard ();

    void ShowAgain(bool bShowAgain);
    bool ShowAgain();
    long Show(bool bGiveChoice);

protected:
    bool m_bGiveChoice;
    void SetupWindow();

   // placeholder definitions
   class TWizardPage1;
   class TWizardPage2;

   class TWizardPage1 : public TWizardStep
      {
      public:
         TWizardPage1(TWizard* pWizard,TModule *module = 0);
         ~TWizardPage1();
         void SetupWindow();

      protected:
         TEdit* pEdit;
      };

   struct XferBuffer
   {
      uint16 checked;
   };

   class TWizardPage2 : public TWizardStep
      {
      public:
         TWizardPage2(TWizard* pWizard,TModule *module = 0);
         ~TWizardPage2();

         void ShowAgain(bool bShowAgain);
         bool ShowAgain();

      protected:
         void SetupWindow();
         int  SetActive(TNotify far& notifyInfo);

         TEdit* pEdit;
         TCheckBox* pCheck;

         TLegalWizard::XferBuffer m_XferBuffer;
      };

   private:
      TWizardPage1* step1;
      TWizardPage2* step2;
};


#endif