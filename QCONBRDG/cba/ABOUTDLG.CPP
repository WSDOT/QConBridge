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

#include <qconbrdg\aboutdlg.h>
#include <owl\static.h>
//{{TAboutDlg Implementation}}


//////////////////////////////////////////////////////////
// TAboutDlg
// ==========
// Construction/Destruction handling.
TAboutDlg::TAboutDlg (TWindow *parent, TResId resId, TModule *module)
    : TCenteredDialog(parent, resId, module)
{
    // INSERT>> Your constructor code here.
}


TAboutDlg::~TAboutDlg ()
{
    Destroy();

    // INSERT>> Your destructor code here.
}

void TAboutDlg::SetupWindow()
{
   TCenteredDialog::SetupWindow();

   TModuleVersionInfo verInfo(*GetApplication());

   const char far* buffer;
   string temp;

   verInfo.GetProductName(buffer);
   SetDlgItemText(IDC_PRODUCT,buffer);

   verInfo.GetProductVersion(buffer);
   temp = "Version " + (string)buffer;
   SetDlgItemText(IDC_VERSION,temp.c_str());

   verInfo.GetInfoString("ReleaseDate",buffer);
   SetDlgItemText(IDC_RELEASEDATE,buffer);

   verInfo.GetInfoString("CompanyName",buffer);
   SetDlgItemText(IDC_COMPANY,buffer);

   verInfo.GetInfoString("Division",buffer);
   SetDlgItemText(IDC_DIVISION,buffer);

   verInfo.GetLegalCopyright(buffer);
   SetDlgItemText(IDC_COPYRIGHT,buffer);

   verInfo.GetInfoString("Author",buffer);
   SetDlgItemText(IDC_AUTHOR,buffer);

   verInfo.GetInfoString("Documentation",buffer);
   SetDlgItemText(IDC_WRITER,buffer);
}
