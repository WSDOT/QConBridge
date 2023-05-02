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


#include "unitdlg.h"

#include <owltools\numvalid.h>

#pragma hrdstop
#include <esl\siconst.h>


//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(UnitDlg, TDialog)
//{{UnitDlgRSP_TBL_BEGIN}}
    EV_BN_CLICKED(IDC_ENGLISH, BNClicked_English),
    EV_BN_CLICKED(IDC_SI, BNClicked_SI),
//{{UnitDlgRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{UnitDlg Implementation}}


UnitDlg::UnitDlg (TWindow* parent, TResId resId, TModule* module):
    TDialog(parent, resId, module)
{
    // INSERT>> Your constructor code here.
   length = new TLengthEdit(this,IDC_EDIT1,USConstants::feet,11,3,IDC_STATIC1,"feet");
   length->SetValidator(new TIncRangeValidator(0.00,1.0E+99)); // in feet

   units = new TRadioGroupBox(this,IDC_UNITSGRP,IDC_SI,IDC_ENGLISH);
}


UnitDlg::~UnitDlg ()
{
    Destroy();

    // INSERT>> Your destructor code here.
    delete length;
    delete units;
}


void UnitDlg::BNClicked_English ()
{
    // INSERT>> Your code here.
    length->SetUnits(USConstants::feet,"feet");
}


void UnitDlg::BNClicked_SI ()
{
    // INSERT>> Your code here.
    length->SetUnits(SIConstants::meter,"m");
}

