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

#include <owltools\intedit.h>
#include <owl/validate.h>

//
//
//
TIntEdit::TIntEdit (TWindow* parent, int resourceID,bool restrictToPositive, TModule* module):
TEdit(parent,resourceID,256,module)
{
   restrict = restrictToPositive;
}

TIntEdit::~TIntEdit ()
{
   Destroy();
}

void TIntEdit::SetupWindow ()
{
    TEdit::SetupWindow();

    if (restrict)
       SetValidator(new TPXPictureValidator("[* ][*#]"));
    else
       SetValidator(new TPXPictureValidator("[* ][-][*#]"));
}

uint TIntEdit::Transfer (void* buffer, TTransferDirection direction)
{
   // Note that this method bypasses the Validators ability to transfer data.

   if (direction == tdSetData)
      {
      SetValue(*(int *)buffer);
      }
   else if (direction == tdGetData)
      {
      *(int *)buffer = GetValue();
      }

   return sizeof(int);
}

int TIntEdit::GetValue()
{
   char *text;
   int val;

   text = new char[GetTextLen()+1];
   GetText(text,GetTextLen()+1);
   if (text[0] == 0) // treat empty controls as zero
      val = 0;
   else
      istrstream(text) >> val;
   delete[] text;

   return val;
}

void TIntEdit::SetValue(int val)
{
   ostrstream os;
   os << val << ends;
   SetText(os.str());
   delete[] os.str();
}

