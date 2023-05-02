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

#ifndef __tooltip_h
#define __tooltip_h

#include <owltools\owltldll.h>
#include <owl\controlb.h>
#include <owl\statusba.h>
#include <owl\buttonga.h>

class _OWLTOOLSCLASS TTipControlBar;
class _OWLTOOLSCLASS TTipStatusBar;

class _OWLTOOLSCLASS TToolTipFont : public TFont
{

public:
	TToolTipFont ();
};

class Tip
{
public:
	enum Style {
		SquareBorder	= 0x00,
		RoundedBorder	= 0x01,
		Shadow			= 0x02,
		};
};

class _OWLTOOLSCLASS TToolTip : private TWindow
{
	friend	TTipControlBar;
	friend	TTipStatusBar;

	UINT	uiTimer;
	bool	bEnabled;
	TSize	sizeText;

	Tip::Style	style;
	TFont* 	font;

	void	KillTipTimer ();
	void	ShowNow ();
	void	PositionTip ();

protected:
	void	GetWindowClass (WNDCLASS &wc);
	LPSTR	GetClassName ();

	void 	Paint (TDC &dc, bool erase, TRect &rc);
	void	EvTimer (UINT uiTimerId);

	void	SetCaption (const char far* title);

public:
	TToolTip (Tip::Style style = Tip::SquareBorder, TFont* font = new TToolTipFont);
	~TToolTip ();

	bool	AreTipsEnabled () 					{ return bEnabled; }
	void	EnableTips (bool bEnable = TRUE) 	{ bEnabled = bEnable; }
	void	HideTip ()							{ SetCaption (NULL) ; }

	DECLARE_RESPONSE_TABLE (TToolTip);
};

class _OWLTOOLSCLASS TTipControlBar : public TControlBar
{
	TToolTip&	tooltip;

public:
	TTipControlBar (TToolTip& tip, TWindow* parent = 0, TTileDirection direction = Horizontal, TFont* font = new TGadgetWindowFont, TModule* module = 0);

	void 	EvMouseMove (UINT modKeys, TPoint& point);
	void 	EvLButtonDown (UINT modKeys, TPoint& point);

	void 	SetHintText (const char *lpszText);

	DECLARE_RESPONSE_TABLE (TTipControlBar);
};

class _OWLTOOLSCLASS TTipStatusBar : public TStatusBar
{
	TToolTip&	tooltip;
	bool		bShowTips;

public:
	TTipStatusBar (	TToolTip& 				tip,
					TWindow*				parent = 0,
					TGadget::TBorderStyle 	borderStyle = TGadget::Recessed,
					UINT                  	modeIndicators = 0,
					TFont*                	font = new TGadgetWindowFont,
					TModule*              	module = 0);

	void 			EvMouseMove (UINT modKeys, TPoint& point);
	void 			EvLButtonDown (UINT modKeys, TPoint& point);

	void 			SetHintText (const char *lpszText);
	virtual void 	SetTextEx (const char *lpszText) { SetText (lpszText); }
	void			UseTips ();

	DECLARE_RESPONSE_TABLE (TTipStatusBar);
};

#endif