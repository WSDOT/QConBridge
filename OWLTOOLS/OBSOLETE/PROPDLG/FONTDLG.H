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

#ifndef __FONT_PROPDLG
#define __FONT_PROPDLG

#include <owl\combobox.h>
#include <owl\static.h>
#include <owl\choosefo.h>
#include <owl\printer.h>
#include <owl\listbox.h>
#include "propdlg.h"

class TFontNameList;
class TColorList;
class TSampleStatic;

// ---------------------------------------------------
// private definitions
// ---------------------------------------------------
class TFontSpec
{
	int		nType;
	int		nFamily;
	LOGFONT	lf;

public:
	TFontSpec ();
	TFontSpec (int nType, const LOGFONT& lf, int nFamily);

	LPCSTR			GetName () const	{ return lf.lfFaceName; }
	int				GetType () const	{ return nType; }
	int				GetFamily () const	{ return nFamily; }
	const LOGFONT&	GetLogFont () const	{ return lf; }

	BOOL	operator< (TFontSpec) const;
	BOOL	operator== (TFontSpec) const;
};

typedef TSArrayAsVector<TFontSpec>	TFontList;

// ---------------------------------------------------
// public definitions
// ---------------------------------------------------
class PROPDLGEXPORT TFontData : public TChooseFontDialog::TData
{
public:
	TFontData ();
};

class PROPDLGEXPORT TPrinterFontData : public TFontData
{
	TPrintDC		*pdc;
public:
	TPrinterFontData (TPrinter& printer);
	~TPrinterFontData ();
};

class PROPDLGEXPORT TFontPropertyPage : public TPropertyPage
{
	TFontData&		fontdata;
	TFontList		fontlist;
	TFont			*ptFontSample;

	TFontNameList	*pcbFontName;
	TComboBox		*pcbFontStyle;
	TComboBox		*pcbFontSize;
	TColorList		*pcbColor;
	TSampleStatic	*peditSample;
	BOOL			bUpdatePending;

	TIntArray		arrayBold;
	TIntArray		arrayItalic;
	int				nFontNameIndex;
	int				nFontPointSize10;

	void			FontSelected (TDC& dc, int nIndex);
	LRESULT			AfterSelection (WPARAM, LPARAM);
	void			EvAfterChange ()	{ if (!bUpdatePending) { PostMessage (WM_USER); bUpdatePending++; } }
	int				GetSelItem (TComboBox *ptList);

	void  			EvMeasureItem(UINT, MEASUREITEMSTRUCT far& measureInfo);

protected:
	virtual void	EvFontName ();
	virtual void	EvFontStyle ();
	virtual void	EvFontSize ();
	virtual void	EvColor ();

public:
	TFontPropertyPage (TResId resID, TFontData& fontdata, TModule *module = 0);
	~TFontPropertyPage ();

	virtual void	SetupWindow ();
	virtual BOOL	SaveData ();

	DECLARE_RESPONSE_TABLE (TFontPropertyPage);
};

#endif
