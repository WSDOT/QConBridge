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

#if !defined COLORBOX_H
#define COLORBOX_H

// ComboBox style must include CBS_OWNERDRAWFIXED | CBS_HASSTRINGS

#include <owltools\owltldll.h>
#include <owl\combobox.h>
#include <owl\color.h>

class _OWLTOOLSCLASS TColorList : public TComboBox
{
public:
	TColorList (TWindow* parent, int Id, TModule* module = 0);
   ~TColorList();

   void ODADrawEntire(DRAWITEMSTRUCT far& drawInfo);
   void ODAFocus(DRAWITEMSTRUCT far& drawInfo);
   void ODASelect(DRAWITEMSTRUCT far& drawInfo);
	void SetupWindow ();
   uint Transfer(void *buffer,TTransferDirection direction);

   TColor GetColor();
   void SetColor(TColor color);

protected:
	void DrawText (TDC& dc, TRect& colorBox, TRect& textBox, DRAWITEMSTRUCT& drawInfo);
   TRect GetColorBox(DRAWITEMSTRUCT far& drawInfo);
   TRect GetTextBox(DRAWITEMSTRUCT far& drawInfo);
};

#endif