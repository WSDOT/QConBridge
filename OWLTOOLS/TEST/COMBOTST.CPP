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

#include "combotst.h"

#include <owltools\colorbox.h>
#include <owltools\penstyle.h>
#include <owltools\symlist.h>
#include <owltools\spinner.h>
#include <owltools\arrowbtn.h>
#include <owltools\intedit.h>
#include <owl\updown.h>

DEFINE_RESPONSE_TABLE1(TComboTesterDlg,TDialog)
    EV_UDN_DELTAPOS(IDC_UPDOWN,UDNDeltaPos),
END_RESPONSE_TABLE;

TComboTesterDlg::TComboTesterDlg (TWindow* parent, TResId resId, TModule* module) :
TDialog(parent,resId,module)
{
   colorList = new TColorList(this,IDC_COLOR);
   lineList = new TPenStyleList(this,IDC_LINE);
   symbolList = new TSymbolList(this,IDC_SYMBOL);
//   spinner = new TIntSpinner(this,IDC_SPINNER,IDC_UPDOWN);
//   spinner->SetRange(1,100);
//   spinner->SetPos(10);
   intEdit = new TIntEdit(this,IDC_SPINNER,false);
   upDown = new TUpDown(this,IDC_UPDOWN,intEdit);
   upDown->Attr.Style |= UDS_SETBUDDYINT | UDS_ALIGNRIGHT;
}

TComboTesterDlg::~TComboTesterDlg()
{
   delete colorList;
   delete lineList;
   delete symbolList;
//   delete spinner;
   delete intEdit;
   delete upDown;
}

void TComboTesterDlg::SetupWindow()
{
   TDialog::SetupWindow();

   upDown->SetBuddy(intEdit->HWindow);
   upDown->SetPos(intEdit->GetValue());
}

bool TComboTesterDlg::UDNDeltaPos(TNmUpDown& udn)
{
   int pos = udn.iPos + udn.iDelta;
   int lower,upper;
   upDown->GetRange(upper,lower);
   pos = (pos < lower) ? lower : pos;
   pos = (pos > upper) ? upper : pos;
   intEdit->SetValue(pos);

   return false;
}

