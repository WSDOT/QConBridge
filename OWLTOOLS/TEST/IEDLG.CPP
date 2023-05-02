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

#include "iedlg.h"

#ifndef __IOSTREAM_H
#include <iostream.h>
#endif

#ifndef __STRSTREAM_H
#include <strstrea.h>
#endif


//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(TIncExcDialog, TDialog)
//{{TIncExcDialogRSP_TBL_BEGIN}}
//{{TIncExcDialogRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TIncExcDialog Implementation}}


TIncExcDialog::TIncExcDialog (TWindow* parent, TResId resId, TModule* module):
    TDialog(parent, resId, module)
{
    // INSERT>> Your constructor code here.
    preCheck = new TCheckBox(this,IDC_PRECHECK);
    lbox1 = new TLinkedLBox(this,IDC_LISTBOX1);
    btn1  = new TLinkedLBoxButton(this,IDC_EXTRACTBTN1);
    btn2  = new TLinkedLBoxButton(this,IDC_EXTRACTBTN2);
    lbox2 = new TLinkedLBox(this,IDC_LISTBOX2);
    postCheck = new TCheckBox(this,IDC_POSTCHECK);

    lbox1->Attach(btn1,lbox2);
    btn1->Attach(lbox1);

    lbox2->Attach(btn2,lbox1);
    btn2->Attach(lbox2);
}


TIncExcDialog::~TIncExcDialog ()
{
    Destroy();

    // INSERT>> Your destructor code here.
    delete preCheck;
    delete lbox1;
    delete btn1;
    delete btn2;
    delete lbox2;
    delete postCheck;
}


