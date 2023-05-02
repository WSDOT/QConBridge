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

#ifndef __FONTPAGE_PRIVATECLASS
#define __FONTPAGE_PRIVATECLASS

class TSampleStatic : public TStatic
{
	TColor		colorText;

public:
	TSampleStatic (TWindow *, int resourceID, TModule *module = 0);

	void		SetupWindow ();
	void		Paint (TDC&, BOOL, TRect&);

	void		SetTextColor (TColor color);
};

class TODComboBox : public TComboBox
{
	void  		DrawItem (DRAWITEMSTRUCT& drawInfo);

protected:
	virtual void DrawText (TDC& dc, TRect& rc, DRAWITEMSTRUCT& drawInfo) = 0;

public:
	TODComboBox (TWindow* parent, int Id, int nLength, TModule* module = 0)
		: TComboBox (parent, Id, nLength, module) { }
};

class TFontNameList : public TODComboBox
{
	TFontList&	fontlist;
	TBitmap		bmpType;

protected:
	virtual void DrawText (TDC& dc, TRect& rc, DRAWITEMSTRUCT& drawInfo);

public:
	TFontNameList (TWindow* parent, int Id, TFontList& fonts, TModule* module = 0);
};

class TColorList : public TODComboBox
{
protected:
	virtual void DrawText (TDC& dc, TRect& rc, DRAWITEMSTRUCT& drawInfo);

public:
	TColorList (TWindow* parent, int Id, TModule* module = 0)
		: TODComboBox (parent, Id, 0, module) { }

	void		SetupWindow ();
};

#endif