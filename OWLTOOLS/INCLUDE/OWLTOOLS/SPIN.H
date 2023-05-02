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

// =========================================================
// SPINNA.H
//
// spin-edit controls
// =========================================================

#ifndef __SPINEDIT_H
#define __SPINEDIT_H

#include <owltools\owltldll.h>
#include <owl\edit.h>
#include <classlib\arrays.h>
#include <cstring.h>

#define SPINTOP		1
#define SPINBOTTOM	0

class _OWLTOOLSCLASS TSpinner;

typedef int RANGEVALUE;

// =========================================================
// TSPINATTR
// data holder for the spinner
// =========================================================
class _OWLTOOLSCLASS TSpinAttr
{
	LPCSTR		lpszFormat;			// format of the spinner
	int			decimals;			// no. of decimal places in lpszFormat
	RANGEVALUE	increment;			// normal button increment
	RANGEVALUE	increment2;			// second increment (SHIFT+key)
	RANGEVALUE	low;				// low range value
	RANGEVALUE	high;				// high range value
	RANGEVALUE	initial;			// initial value
	bool		MatchOnLeaving;		// if value should be match to a valid entry
									// when the focus is lost
	bool		ShowMenu;			// if menu should be shown on right mouse click

public:
	TSpinAttr ();

	LPCSTR		GetFormat ()						{ return lpszFormat; }
	int			GetDecimals ()						{ return decimals; }
	RANGEVALUE	GetIncrement ()						{ return increment; }
	RANGEVALUE	GetLargeIncrement ()				{ return increment2; }
	RANGEVALUE	GetLow ()							{ return low; }
	RANGEVALUE	GetHigh ()							{ return high; }
	RANGEVALUE	GetInitial ()						{ return initial; }
	bool		GetMatchOnLeaving ()				{ return MatchOnLeaving; }
	bool		GetShowMenu ()						{ return ShowMenu; }

	void		SetFormat (LPCSTR lpszFormat);
	void		SetIncrement (RANGEVALUE n)			{ increment = n; }
	void		SetLargeIncrement (RANGEVALUE n)	{ increment2 = n; }
	void		SetLow (RANGEVALUE n)				{ low = n; }
	void		SetHigh (RANGEVALUE n)				{ high = n; }
	void		SetInitial (RANGEVALUE n)			{ initial = n; }
	void		SetRange (RANGEVALUE l, RANGEVALUE h);
	void		SetMatchOnLeaving (bool b = TRUE)	{ MatchOnLeaving = b; }
	void		SetShowMenu (bool m = TRUE)			{ ShowMenu = m; }
};

// =========================================================
// TSPINEDIT
// edit control attached to a spinner
// =========================================================
class _OWLTOOLSCLASS TSpinEdit : public TEdit
{
	TSpinner*		spinner;

	void 			Init ();

public:
	TSpinAttr		Spinner;


protected:
	void 			EvChar (UINT key, UINT repeatCount, UINT flags);
	void 			EvKeyDown (UINT key, UINT repeatCount, UINT flags);
	void 			EvKeyUp (UINT key, UINT repeatCount, UINT flags);
	void 			EvKillFocus (HWND hwndGetFocus);
	void 			EvRButtonDown (UINT, TPoint&);
	void			EvEnable (bool);

public:
	TSpinEdit (	TWindow*        parent,
				int             id,
				const char far* text,
				int x, int y, int w, int h,
				UINT            textLen = 0,
				TModule*        module = 0);

	TSpinEdit (	TWindow*   		parent,
				int        		resourceId,
				UINT       		textLen = 0,
				TModule*   		module = 0);

	void				EnableButton (bool bTop, bool bEnable = TRUE);

	virtual string		GetFormat ();
	virtual void		ShowPopup () = 0;
	virtual RANGEVALUE	GetIncrement ()	{ return Spinner.GetIncrement (); }
	virtual void		SpinUp (RANGEVALUE increment = 1) = 0;
	virtual void		SpinDown (RANGEVALUE increment = 1) = 0;
	virtual void		AssessEnable ()	{ }
	virtual void		WhenLeaving ()	{ }

	DECLARE_RESPONSE_TABLE (TSpinEdit);
};

// =========================================================
// TRANGESPINEDIT
// spin-edit which works between two numeric values
// =========================================================
class _OWLTOOLSCLASS TRangeSpinEdit : public TSpinEdit
{
public:
	TRangeSpinEdit (TWindow*        parent,
					int             id,
					const char far* text,
					int x, int y, int w, int h,
					UINT            textLen = 0,
					TModule*        module = 0);

	TRangeSpinEdit (TWindow*   		parent,
					int        		resourceId,
					UINT       		textLen = 0,
					TModule*   		module = 0);

	void			SetupWindow ();
	LRESULT			EvCommand (UINT id, HWND hWndCtl, UINT notifyCode);

	RANGEVALUE		GetCurrentValue (bool* pValid = NULL);
	void			SetValue (RANGEVALUE nValue, bool bSelect = TRUE, bool bNotify = TRUE);
	string 			GetValueText (RANGEVALUE nValue);
   uint Transfer(void* buffer, TTransferDirection direction);

	virtual void	ShowPopup ();

	virtual void	SpinUp (RANGEVALUE increment = 1);
	virtual void	SpinDown (RANGEVALUE increment = 1);
	virtual void	AssessEnable ();
	virtual void	WhenLeaving ();
};

// =========================================================
// TLISTSPINEDIT
// spin-edit which works from an array of string values
// =========================================================
class _OWLTOOLSCLASS TListSpinEdit : public TSpinEdit
{
	TArrayAsVector<string>	arrayString;

public:
	TListSpinEdit (	TWindow*        parent,
					int             id,
					const char far* text,
					int x, int y, int w, int h,
					UINT            textLen = 0,
					TModule*        module = 0);

	TListSpinEdit (	TWindow*   		parent,
					int        		resourceId,
					UINT       		textLen = 0,
					TModule*   		module = 0);

	void			SetupWindow ();
	LRESULT			EvCommand (UINT id, HWND hWndCtl, UINT notifyCode);

	void			AddString (LPCSTR lpszText);

	int				GetCurrentValue (bool* pValid = NULL);
	void			SetValue (int nValue, bool bSelect = TRUE, bool bNotify = TRUE);
	virtual void	ShowPopup ();

	virtual void	SpinUp (RANGEVALUE increment = 1);
	virtual void	SpinDown (RANGEVALUE increment = 1);
	virtual void	AssessEnable ();
	virtual void	WhenLeaving ();
};

#endif
