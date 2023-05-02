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

#include <owltools\spinner.h>

TIntSpinner::TIntSpinner(TWindow* parent,int editId,int updownId,TModule* module) :
TUpDown(parent,updownId,0,module)
{
//   Attr.Style |= UDS_SETBUDDYINT | UDS_ALIGNRIGHT;
   Attr.Style |= UDS_AUTOBUDDY | UDS_ALIGNRIGHT;

//   intEdit = new TIntEdit(parent,editId,false,module);
//   intEdit = new TEdit(parent,editId,256,module);
//   intEdit->DisableTransfer();
}

TIntSpinner::~TIntSpinner()
{
//   delete intEdit;
}

void TIntSpinner::SetupWindow()
{
   TUpDown::SetupWindow();

//   SetBuddy(*intEdit);
   SetRange(5,20);
   SetPos(10);
}


