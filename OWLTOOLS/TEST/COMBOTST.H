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

#if !defined(__combotest_h)              // Sentry, use file only if it's not already included.
#define __combotest_h

#include "testapp.rh"            // Definition of all resources.

#include <owltools\symbol.h>
#include <owl\dialog.h>
#include <owl\color.h>
#include <owl\updown.h>

class _OWLTOOLSCLASS TColorList;
class _OWLTOOLSCLASS TPenStyleList;
class _OWLTOOLSCLASS TSymbolList;
class _OWLTOOLSCLASS TIntSpinner;
class _OWLTOOLSCLASS TIntEdit;
class _OWLCLASS TUpDown;


struct ComboTesterXferBuffer
{
   TColor color;
   int penStyle;
   TSymbolType type;
   int spinValue;

   ComboTesterXferBuffer():color(RGB(255,0,255)),penStyle(PS_DOT),type(stCircle),spinValue(5){}
};

//{{TDialog = TComboTesterDlg}}
class TComboTesterDlg : public TDialog {
public:
    TComboTesterDlg (TWindow* parent, TResId resId = IDD_COMBOTESTER, TModule* module = 0);
    ~TComboTesterDlg();

    void SetupWindow();
    bool UDNDeltaPos(TNmUpDown& udm);

protected:
   TColorList *colorList;
   TPenStyleList *lineList;
   TSymbolList *symbolList;
//   TIntSpinner *spinner;
   TIntEdit* intEdit;
   TUpDown* upDown;

   DECLARE_RESPONSE_TABLE(TComboTesterDlg);
};    //{{RadioGrpTestDlg}}


#endif                                      // __rgbdlg_h sentry.
