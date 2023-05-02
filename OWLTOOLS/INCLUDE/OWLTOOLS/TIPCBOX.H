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

#if !defined OWLTOOLS_TIPCBOX_H_
#define OWLTOOLS_TIPCBOX_H_

#include <owltools\owltldll.h>
#include <owl/combobox.h>
#include <owltools/ctrltip.h>

class _OWLTOOLSCLASS TTipComboBox : public TComboBox, public TControlTipMixin
{
public:
    TTipComboBox(TWindow* parent, int id, int x, int y, int w, int h, uint32 style, uint textLimit, const char* tip, TModule* module = 0);
    TTipComboBox(TWindow* parent, int id, int x, int y, int w, int h, uint32 style, uint textLimit, int msgResourceId, TModule* module = 0);
    TTipComboBox(TWindow* parent, int resourceId, const char* tip, uint textLen = 0, TModule* module = 0);
    TTipComboBox(TWindow* parent, int resourceId, int msgResourceId = -1, uint textLen = 0, TModule* module = 0);
    virtual ~TTipComboBox();

   virtual bool PreProcessMsg(MSG& msg);
   virtual void SetupWindow();

DECLARE_RESPONSE_TABLE(TTipComboBox);
};

#endif
