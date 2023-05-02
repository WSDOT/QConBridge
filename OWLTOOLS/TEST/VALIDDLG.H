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

#if !defined(__validdlg_h)              // Sentry, use file only if it's not already included.
#define __validdlg_h

#include <owl\dialog.h>
#include <owltools\nbredit.h>

#include "testapp.rh"            // Definition of all resources.

struct ValidatorXferBuffer
{
   double value1;
   double value2;
   double value3;
   double value4;
   double value5;
   double value6;
   double value7;
   double value8;
   double value9;
};

//{{TDialog = ValidatorDlg}}
class ValidatorDlg : public TDialog {
public:
    ValidatorDlg (TWindow* parent, TResId resId = IDD_VALIDATORDLG, TModule* module = 0);
    virtual ~ValidatorDlg ();

protected:
   TEdit *number1;
   TNumberEdit *number2;
   TNumberEdit *number3;
   TNumberEdit *number4;
   TNumberEdit *number5;
   TNumberEdit *number6;
   TNumberEdit *number7;
   TNumberEdit *number8;
   TNumberEdit *number9;
};    //{{ValidatorDlg}}


#endif                                      // __validdlg_h sentry.

