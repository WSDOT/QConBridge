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

#include <stdio.h>
#include <strstrea.h>
#include <iomanip.h>
#include <owltools\nbredit.h>
#include <owl/validate.h>

/******************************************************************************
TNumberEdit::TNumberEdit

Description:
   Constructor
   Edit control object for double precision numbers

Arguements:
   TWindow* parent - pointer to the parent window
   int resourceID  - edit control resource id
   uint Width      - width of display
   uint Precision  - display precision
   TModule* module - pointer to a module

Return:
   None
*******************************************************************************/
TNumberEdit::TNumberEdit (TWindow* parent, int resourceID,uint Width,uint Precision, TModule* module):
TEdit(parent,resourceID,Width < 2 ? 2 : Width,module)
{
   width = Width;
   precision = Precision;
   SetValidator(new TPXPictureValidator("[* ][-][*#][.][*#][{{E,e}[{-,+}]*#}]"));
}


/******************************************************************************
TNumberEdit::~TNumberEdit

Description:
   Destructor

Arguements:
   None

Return:
   None
*******************************************************************************/
TNumberEdit::~TNumberEdit ()
{
   Destroy();
}


/******************************************************************************
TNumberEdit::Transfer

Description:
   Transfer doubles to and from the edit control

Arguements:
   See TWindow::Transfer

Return:
   size of double
*******************************************************************************/
uint TNumberEdit::Transfer (void* buffer, TTransferDirection direction)
{
   uint result;

   // Note that this method bypasses the Validators ability to transfer data.

   if (direction == tdSetData)
      {
      SetValue(*(double *)buffer);
      }
   else if (direction == tdGetData)
      {
      *(double *)buffer = GetValue();
      }

   result = sizeof(double);

   return result;
}

/******************************************************************************
TNumberEdit::SetPrecision

Description:
   Sets the precision of the display

Arguements:
   uint Precision - new display precision

Return:
   Returns the previous display precision
*******************************************************************************/
uint TNumberEdit::SetPrecision(uint Precision)
{
   uint temp;
   temp = precision;
   precision = Precision;
   return temp;
}


/******************************************************************************
TNumberEdit::GetPrecision

Description:
   Returns the current display precision

Arguements:
   none

Return:
   Returns the current display precision
*******************************************************************************/
uint TNumberEdit::GetPrecision() const
{
   return precision;
}


/******************************************************************************
TNumberEdit::SetWidth

Description:
   Sets the width of the display

Arguements:
   uint Width - new display width

Return:
   Returns the previous display width
*******************************************************************************/
uint TNumberEdit::SetWidth(uint Width)
{
   uint temp;
   temp = width;
   width = Width;
   TextLen = Width;
   return temp;
}


/******************************************************************************
TNumberEdit::GetWidth

Description:
   Returns the current display width

Arguements:
   none

Return:
   Returns the current display width
*******************************************************************************/
uint TNumberEdit::GetWidth() const
{
   return width;
}

/******************************************************************************
TNumberEdit::SetValue

Description:
   Sets the value of the edit control

Arguements:
   double value - the value to be placed in the edit control

Return:
   None
*******************************************************************************/
void TNumberEdit::SetValue(double value)
{
   ostrstream os;
   ostrstream os1;
   string text;

   os << setiosflags(ios::left)
      << setiosflags(ios::fixed)
      << setiosflags(ios::showpoint)
      << setw(width)
      << setprecision(precision)
      << value << ends;
   text = string( string(os.str()).strip(string::Both,' ') );
   delete[] os.str();

   // If string is too long, try scientific notation
   if (text.length() > width)
      {
      os1 << setiosflags(ios::left | ios::scientific | ios::showpoint | ios::uppercase)
          << setw(9+precision)
          << setprecision(precision)
          << value << ends;
      text = string( string(os1.str()).strip(string::Both,' ') );
      }
   SetText(text.c_str());
}


/******************************************************************************
TNumberEdit::GetValue

Description:
   Gets a value from a edit control

Arguements:
   none

Return:
   The value that was displayed in the edit control
*******************************************************************************/
double TNumberEdit::GetValue()
{
   double value;
   char *text;

   text = new char[GetTextLen() + 1];
   GetText(text,GetTextLen()+1);
   if (text[0] == 0) // treat empty controls as zero
      value = 0;
   else
      {
      string valText( string(text).strip(string::Both,' ') );
      strcpy(text,valText.c_str());
      value = atof(text);
      }
   delete[] text;

   return value;
}
