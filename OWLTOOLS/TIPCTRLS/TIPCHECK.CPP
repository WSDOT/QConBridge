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

#include <owltools\tipcheck.h>


DEFINE_RESPONSE_TABLE2(TTipCheckBox,TCheckBox,TControlTipMixin)
END_RESPONSE_TABLE;

TTipCheckBox::TTipCheckBox(TWindow* parent, int id, const char far* title, int x, int y, int w, int h, const char* tip, TGroupBox* group, TModule* module) :
   TCheckBox(parent, id, title, x, y, w, h, group, module),
   TControlTipMixin(tip)
{
}

TTipCheckBox::TTipCheckBox(TWindow* parent, int id, const char far* title, int x, int y, int w, int h,int msgResourceId, TGroupBox* group, TModule* module) :
   TCheckBox(parent, id, title, x, y, w, h, group, module),
   TControlTipMixin(msgResourceId)
{
}

TTipCheckBox::TTipCheckBox(TWindow* parent, int resourceId,const char* tip,TGroupBox* group,TModule* module) :
   TCheckBox(parent,resourceId,group,module),
   TControlTipMixin(tip)
{
}

TTipCheckBox::TTipCheckBox(TWindow* parent, int resourceId,int msgResourceId,TGroupBox* group,TModule* module) :
   TCheckBox(parent,resourceId,group,module),
   TControlTipMixin(msgResourceId < 0 ? resourceId : msgResourceId)
{
}


TTipCheckBox::~TTipCheckBox()
{
}


void TTipCheckBox::SetupWindow()
{
  TCheckBox::SetupWindow();
  SetupTips();
}

bool TTipCheckBox::PreProcessMsg(MSG& msg)
{
  return TControlTipMixin::PreProcessMsg(msg) || TCheckBox::PreProcessMsg(msg);
}

