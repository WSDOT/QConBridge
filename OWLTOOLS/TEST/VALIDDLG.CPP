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

#include "validdlg.h"

#include <owltools\numvalid.h>


//{{ValidatorDlg Implementation}}


ValidatorDlg::ValidatorDlg (TWindow* parent, TResId resId, TModule* module):
    TDialog(parent, resId, module)
{
    // INSERT>> Your constructor code here.
    number1 = new TEdit(this,IDC_NUMBERFIELD1,6);
    TIncRangeValidator *validator = new TIncRangeValidator(5.00,10.00);
    validator->SetOption(voTransfer);
    number1->SetValidator(validator);

    number2 = new TNumberEdit(this,IDC_NUMBERFIELD2,6,3);
    number2->SetValidator(new TExcRangeValidator(5.00,10.00));

    number3 = new TNumberEdit(this,IDC_NUMBERFIELD3,6,3);
    number3->SetValidator(new TExtremeValueValidator(10.00,TExtremeValueValidator::ltLT));

    number4 = new TNumberEdit(this,IDC_NUMBERFIELD4,6,3);
    number4->SetValidator(new TExtremeValueValidator(10.00,TExtremeValueValidator::ltLE));

    number5 = new TNumberEdit(this,IDC_NUMBERFIELD5,6,3);
    number5->SetValidator(new TExtremeValueValidator(10.00,TExtremeValueValidator::ltGT));

    number6 = new TNumberEdit(this,IDC_NUMBERFIELD6,6,3);
    number6->SetValidator(new TExtremeValueValidator(10.00,TExtremeValueValidator::ltGE));

    number7 = new TNumberEdit(this,IDC_NUMBERFIELD7,6,3);
    number7->SetValidator(new TIncRangeValidator(-5.00,10.00));

    number8 = new TNumberEdit(this,IDC_NUMBERFIELD8,6,3);
    number8->SetValidator(new TExtremeValueValidator(-10.00,TExtremeValueValidator::ltGT));

    number9 = new TNumberEdit(this,IDC_NUMBERFIELD9,6,3);
    number9->SetValidator(new TExtremeValueValidator(-10.00,TExtremeValueValidator::ltGE));
}


ValidatorDlg::~ValidatorDlg ()
{
    Destroy();

    // INSERT>> Your destructor code here.
    delete number1;
    delete number2;
    delete number3;
    delete number4;
    delete number5;
    delete number6;
    delete number7;
    delete number8;
    delete number9;
}

