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

#if !defined OWLTOOLS_TIPBTN_H_
#define OWLTOOLS_TIPBTN_H_

#include <owltools\owltldll.h>
#include <owl/button.h>
#include <owltools/ctrltip.h>

class _OWLTOOLSCLASS TTipButton : public TButton, public TControlTipMixin
{
public:
    TTipButton(TWindow* parent, int id, const char far* text, int X, int Y, int W, int H, const char* tip, bool isDefault = false, TModule* module = 0);
    TTipButton(TWindow* parent, int id, const char far* text, int X, int Y, int W, int H, int msgResourceId, bool isDefault = false, TModule* module = 0);
    TTipButton(TWindow* parent, int resourceId, const char* tip, TModule* module = 0);
    TTipButton(TWindow* parent, int resourceId, int msgResourceId = -1, TModule* module = 0);
    virtual ~TTipButton();

   virtual bool PreProcessMsg(MSG& msg);
   virtual void SetupWindow();

DECLARE_RESPONSE_TABLE(TTipButton);
};

#endif

