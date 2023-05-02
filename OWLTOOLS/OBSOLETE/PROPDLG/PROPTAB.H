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
// PROPTAB.H
// header file for implementation of TPropertyTab
// =========================================================
#ifndef __PROPERTY_TAB
#define __PROPERTY_TAB

#define MAXROWS				20

#if !defined(OWL_CELARRAY_H)
  #include <owl/celarray.h>		//definition of TCelArray
#endif

// =========================================================
// TPropertyEntry
//
// holder class for page information
// =========================================================
class TPropertyEntry
{
	string			sTabText;
	TSSPropertyPage	*pPage;
	int				nWidth;
	bool			bEnabled;

public:
	TPropertyEntry ();
	TPropertyEntry (string sTabText, TSSPropertyPage *pPage, bool bEnabled = TRUE);

	TPropertyEntry&	operator= (const TPropertyEntry&);
	bool			operator== (const TPropertyEntry&);

	string&			GetText ()					{ return sTabText; }
	TSSPropertyPage	*GetDialog ()				{ return pPage; }
	int				GetWidth ()					{ return nWidth; }
	void			SetWidth (int n)			{ nWidth = n; }
	void			SetText (LPCSTR lpszText)	{ sTabText = lpszText; }
	bool			IsEnabled ()				{ return bEnabled; }
	void			EnableTab (bool b)			{ bEnabled = b; }
};

typedef TArrayAsVector<TPropertyEntry>		TPropertyArray;

class TRange
{
public:
	int		Left;
	int		Right;
};

// =========================================================
// TPropertyTab
//
// class which handles drawing and switching of pages
// =========================================================
class PROPDLGEXPORT TPropertyTab : public TControl
{
	TPropertyArray	pages;
	TFont			*pfontNormal;
	TFont			*pfontSelect;
	TColor			colorActive;

	int				nSelect;
	Tab::Style 		styleTabs;

	// tab/area size parameters
	int				nTabHeight;
	int				nTextHeight;
	int				nFixedTabWidth;
	int				nWideMarginWidth;
	TRect			rcSize;

	// variables for stacked tabs
	int				nRows;
	TRange			ptRanges[MAXROWS];
	int				nRowOrder[MAXROWS];

	// variables for scrolling
	int				nTabOrigin;
	bool			bLeftScroll;
	bool			bRightScroll;
	int				xMax;
	TBitmap			*bmpLeftArrow;
	TBitmap			*bmpRightArrow;

	// system colors
	TColor			colorWhite;
	TColor			colorLtGray;
	TColor			colorDkGray;
	TColor			colorBlack;

	// Tab bitmap support
	TCelArray*		celTabBitmap;
	bool			bUseMask;

	// general parameters
	int				nTabActive;

	bool 			AdjustOrigin (int nTab);
	void 			DrawTab (TDC& dc, int& x, int nTab, int yParam = 0, bool bIsLast = TRUE);
	void			DrawTabs (TDC& dc);
	void			DrawStackedTabs (TDC& dc);
	void 			DrawWizardFrame (TDC& dc);
	int				GetLastTab (bool bVisible);
	int 			GetMouseTab (TPoint& point);
	bool 			GetTabRect (TRect& rc, int nTab, int nOrigin = -1);
	int				GetTabRow (int nTab);
	int				GetTabWidth (int nTab)				{ return pages[nTab].GetWidth (); }
	int				CalcTabWidth (TDC& dc, int nTab);
	void			CalculateTabWidths ();
	void			CalculateBasicTabWidths ();
	void			ReassessTabWidths ();
	void			RedrawDialogs ();

	// scrolling functions
	bool			AssessScroll ();
	void 			GetScrollRect (TRect& rcTabs, int nButton = -1);
	void			NextOrigin (int nDir = 1);

	void 			SetRect (TRect& rc, int nRow = -1);

	void 			EvLButtonDown(UINT modKeys, TPoint& point);
	void 			EvRButtonDown(UINT modKeys, TPoint& point);

public:
	TPropertyTab (TWindow *pParent, Tab::Style style, TResId resBitmap, bool bUseMask = FALSE);
	virtual ~TPropertyTab ();

	void			SetupWindow ();
	void			Paint (TDC& dc, bool erase, TRect& rc);
	void			EvSize (UINT, TSize& size);
	LRESULT			EvCommand (UINT id, HWND hWndCtl, UINT notifyCode);
	void 			TabColorChange ();

	void			EvSetFocus (HWND hwndLostFocus);
	void			EvKillFocus (HWND hwndGetFocus);
	void			EvKeyDown (UINT key, UINT repeatCount, UINT flags);
	UINT			EvGetDlgCode (MSG far *);

	LPSTR			GetClassName ();
	void			GetWindowClass (WNDCLASS& wc);

	void			Add (LPCSTR lpszTab, TSSPropertyPage *pPage, bool bEnabled = TRUE);
	void			DestroyPages ();

	int				GetEntryCount ()		{ return pages.GetItemsInContainer (); }
	TSSPropertyPage 	*GetPage (int n)		{ return pages[n].GetDialog (); }
	int				GetSelectedTab ()		{ return nSelect; }
	Tab::Style		GetTabStyle ()			{ return styleTabs; }

	bool			SavePageData ();
	bool			CheckCanClose ();

	bool 			SelectTab (int nTab);
	bool 			SelectNext (int nDir = 1);
	bool			SelectTabByKey (char cKey);
	void			SelectFirstControl ();
	HWND			GetFirstControl ();
	HWND			SetPageFocus (bool bSetFocus = TRUE);

	bool 			IsTabEnabled (int nTab) const;
	bool 			IsTabSelected (int nTab) const;
	bool 			IsTabVisible (int nTab) const;

	void 			EnableTab (int nTab, bool bEnable);
	int				GetTabHeight ()			{ return nTabHeight; }
	string			GetTabText (int nTab);
	void 			SetActiveColor (TColor color);
	void 			SetFixedTabWidth (int nWidth);
	void			SetTabText (int nTab, LPCSTR lpszText);
	void 			SetWideMarginWidth (int nWidth);

	DECLARE_RESPONSE_TABLE(TPropertyTab);
};

#endif