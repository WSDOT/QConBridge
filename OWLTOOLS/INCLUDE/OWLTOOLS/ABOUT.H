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

//
//  About box object for C++ OWL
//  From:
//  "Creating Hidden Credit Screens In Windows Programs"
//  PC Magazine June 15, 1993
//
//  Copyright 1993 by Danny Thorpe
//

#include <owltools\owltldll.h>
#include <owltools\credit.h>
#include <owl\dialog.h>
#include <string.h>
#include <alloc.h>

#define  IDSHADE   100
#define  IDBUMP    101
#define  IDHOTKEY  103

//
//
//

class _OWLTOOLSCLASS TAboutBox : public TDialog
{
// Data Members
protected:
    LPCSTR Title;
    TResId bmpId;
    LPSTR* StringList;
    TCreditWindow* CreditWindow;

// Constructor/Destructor
public:
    TAboutBox(TWindow *parent,
              TResId resId,
              char *title,
              TResId bitmapResId,
              LPSTR AStringList[],
              TModule *module = 0);
    virtual ~TAboutBox();

// Methods
public:
    virtual void SetupWindow();
    virtual void InitCreditWindow(TResId bitmapResId, LPSTR AStringList[]);
    virtual void HotKey_BNClicked();

    DECLARE_RESPONSE_TABLE(TAboutBox);
};


