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
#include <qconbrdg\legalwiz.h>


TLegalWizard::TLegalWizard (TWindow* parent,TModule* module) :
TWizard(parent,"Legal Wizard",0,PSH_DEFAULT, module)
{
   step1 = new TWizardPage1(this,module);
   step2 = new TWizardPage2(this,module);

   AddPage(*step1);
   AddPage(*step2);
}


TLegalWizard::~TLegalWizard ()
{
    Destroy();

    delete step1;
    delete step2;
}

void TLegalWizard::ShowAgain(bool bShowAgain)
{
   step2->ShowAgain(bShowAgain);
}

bool TLegalWizard::ShowAgain()
{
   return step2->ShowAgain();
}

long TLegalWizard::Show(bool bGiveChoice)
{
   m_bGiveChoice = bGiveChoice;

   if ( Execute() != IDOK )
      return 2; // Agreement was rejected

   return 1; // Agreement was accepted
}

#if !defined ID_WIZFINISH
#define ID_WIZFINISH 0x3025
#endif

void TLegalWizard::SetupWindow()
{
   TWizard::SetupWindow();

   if ( m_bGiveChoice )
   {
      THandle hWnd = GetDlgItem( ID_WIZFINISH );
      ::SetWindowText( hWnd, "I Accept" );
   }
   else
   {
      THandle hWnd = GetDlgItem(IDCANCEL);
      ::ShowWindow( hWnd, SW_HIDE );
   }
}

//
// Page 1
//

TLegalWizard::TWizardPage1::TWizardPage1(TWizard* pWizard,TModule *module) :
   TWizardStep(pWizard,TResId(IDD_LICENSEWIZ_1),"License Agreement",0,module)
{
   pEdit = new TEdit(this,IDC_LIC_TEXT);
}

TLegalWizard::TWizardPage1::~TWizardPage1()
{
   delete pEdit;
}

void TLegalWizard::TWizardPage1::SetupWindow()
{
   TWizardStep::SetupWindow();

   TModule* pModule = GetModule();

   HRSRC hResInfo = pModule->FindResource( TResId(IDR_LEGAL),"TEXT");
   if ( hResInfo == 0 )
      DWORD dwError = GetLastError();

   DWORD dwSize = pModule->SizeofResource( hResInfo );
   HGLOBAL hResData = pModule->LoadResource( hResInfo );
   LPVOID pVoid = LockResource( hResData );

   char* pText = new char [dwSize];
   memcpy((void*)pText,pVoid,dwSize);
   pText[dwSize-1] = 0;

   THandle hWnd = GetDlgItem(IDC_LIC_TEXT);
   ::SetWindowText(hWnd,pText);

   delete[] pText;

}

//
// Page 2
//
TLegalWizard::TWizardPage2::TWizardPage2(TWizard* pWizard,TModule *module) :
TWizardStep(pWizard,TResId(IDD_LICENSEWIZ_2),"Disclaimer",0,module)
{
   pEdit = new TEdit(this,IDC_DISCLAIMER);
   pCheck = new TCheckBox(this,IDC_CHECK);

   SetTransferBuffer( &m_XferBuffer );
   pCheck->EnableTransfer();
   pEdit->DisableTransfer();
}

TLegalWizard::TWizardPage2::~TWizardPage2()
{
   delete pEdit;
   delete pCheck;
}

void TLegalWizard::TWizardPage2::ShowAgain(bool bShowAgain)
{
   m_XferBuffer.checked = (bShowAgain == true ? BF_UNCHECKED : BF_CHECKED);
}

bool TLegalWizard::TWizardPage2::ShowAgain()
{
   return (m_XferBuffer.checked == BF_UNCHECKED ? true : false);
}

void TLegalWizard::TWizardPage2::SetupWindow()
{
   TWizardStep::SetupWindow();

   TModule* pModule = GetModule();

   HRSRC hResInfo = pModule->FindResource( TResId(IDR_DISCLAIMER),"TEXT");
   if ( hResInfo == 0 )
      DWORD dwError = GetLastError();

   DWORD dwSize = pModule->SizeofResource( hResInfo );
   HGLOBAL hResData = pModule->LoadResource( hResInfo );
   LPVOID pVoid = LockResource( hResData );

   char* pText = new char [dwSize];
   memcpy((void*)pText,pVoid,dwSize);
   pText[dwSize-1] = 0;

   THandle hWnd = GetDlgItem(IDC_DISCLAIMER);
   ::SetWindowText(hWnd,pText);

   delete[] pText;

}

int  TLegalWizard::TWizardPage2::SetActive(TNotify far& notifyInfo)
{
   int retval = TWizardStep::SetActive(notifyInfo);
   return retval;
}

