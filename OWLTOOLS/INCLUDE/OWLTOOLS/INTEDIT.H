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

#if !defined(__ingedit_h)              // Sentry, use file only if it's not already included.
#define __ingedit_h

#include <owltools\owltldll.h>
#include <owl\edit.h>

class _OWLTOOLSCLASS TIntEdit : public TEdit
{
public:
    TIntEdit (TWindow* parent, int resourceID,bool restrictToPositive = false, TModule* module = 0);
    virtual ~TIntEdit ();

public:
    virtual void SetupWindow ();
    virtual uint Transfer (void* buffer, TTransferDirection direction);
    int GetValue();
    void SetValue(int val);

protected:
   bool restrict;
};

#endif                                      // __intedit_h sentry.
