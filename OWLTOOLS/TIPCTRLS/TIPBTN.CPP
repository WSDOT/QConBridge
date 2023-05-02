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

#include <owltools\tipbtn.h>

DEFINE_RESPONSE_TABLE2(TTipButton,TButton,TControlTipMixin)
END_RESPONSE_TABLE;

TTipButton::TTipButton(TWindow* parent, int id, const char far* text, int X, int Y, int W, int H, const char* tip, bool isDefault, TModule* module) :
   TButton(parent, id, text, X, Y, W, H, isDefault, module),
   TControlTipMixin(tip)
{
}

TTipButton::TTipButton(TWindow* parent, int id, const char far* text, int X, int Y, int W, int H, int msgResourceId, bool isDefault, TModule* module) :
   TButton(parent, id, text, X, Y, W, H, isDefault, module),
   TControlTipMixin(msgResourceId)
{
}

TTipButton::TTipButton(TWindow* parent, int resourceId, const char* tip, TModule* module) :
   TButton(parent,resourceId,module),
   TControlTipMixin(tip)
{
}

TTipButton::TTipButton(TWindow* parent, int resourceId, int msgResourceId, TModule* module) :
   TButton(parent,resourceId,module),
   TControlTipMixin(msgResourceId < 0 ? resourceId : msgResourceId)
{
}


TTipButton::~TTipButton()
{
}


void TTipButton::SetupWindow()
{
  TButton::SetupWindow();
  SetupTips();
}

bool TTipButton::PreProcessMsg(MSG& msg)
{
  return TControlTipMixin::PreProcessMsg(msg) || TButton::PreProcessMsg(msg);
}

