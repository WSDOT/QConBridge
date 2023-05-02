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
// PROPDLG.H
// header file for implementation of TPropertyDialog and
// TSSPropertyPage
// =========================================================
#ifndef __PROPERTY_DIALOGS
#define __PROPERTY_DIALOGS

#if !defined( CLASSLIB_ARRAYS_H )
#include <classlib\arrays.h>
#endif

#if !defined ( OWL_DIALOG_H )
#include <owl\dialog.h>
#endif

#if !defined(OWL_GDIOBJEC_H)
#include <owl\gdiobjec.h>
#endif

#ifndef USETRANSFERARRAYS
  #define USETRANSFERARRAYS	false
#endif

#define DEFTABWIDTH			64
#define DEFWIDEMARGIN		6

#define CURRENTPAGE			-1

#if defined (BUILDPROPDLGDLL)
  #define PROPDLGEXPORT	__export
#elif defined (PROPDLGDLL)
  #define PROPDLGEXPORT	__import
#else
  #define PROPDLGEXPORT
#endif

#if USETRANSFERARRAYS
  #include "tfarry.h"
#endif

class PROPDLGEXPORT TPropertyTab;
class PROPDLGEXPORT TSSPropertyPage;
class PROPDLGEXPORT TPropertyDialog;

typedef void (*TPageActionFunc)(TSSPropertyPage* page, void* param);
typedef bool (*TPageCondFunc)(TSSPropertyPage* page, void* param);

typedef void (TPropertyDialog::*TPageActionMemFunc)(TSSPropertyPage* page, void* param);
typedef bool (TPropertyDialog::*TPageCondMemFunc)(TSSPropertyPage* page, void* param);

void    CenterWindow (TWindow* wnd);
void	UseSmallFont (HWND HWindow);
bool 	DrawMaskedBitmap (HDC hdc, int x, int y, HBITMAP hbm, int XOrigin, int nWidth);

class PROPDLGEXPORT Tab
{

public:
	class PROPDLGEXPORT Style
	{
		DWORD	style;

	public:
		Style ()					{ style = 0x0L; }
		Style (DWORD dwOther)		{ style = dwOther; }

		operator DWORD () const		{ return style; }
	};

	static const Style	SingleHeight;
	static const Style	VariableWidth;
	static const Style	DoubleHeight;
	static const Style	FixedWidth;
	static const Style	ColorActive;
	static const Style	Collapsing;
	static const Style	Justified;
	static const Style	Stacked;
	static const Style	AllowDupPages;
	static const Style	CreateOnDemand;
	static const Style	WideMargins;
	static const Style	AutoTabResize;
	static const Style	ButtonsOnRight;
	static const Style	UseSmallFont;
	static const Style	SaveOnClosePage;
	static const Style	NoMDIClose;
	static const Style	WizardFrame;
	static const Style	UseTabBitmaps;
	static const Style  CenterDialog;
	static const Style  CreateOnDemandAndKeepIt;
	static const Style  FocusOnFirstControl;
};

class PROPDLGEXPORT TSSPropertyPage : public TDialog
{
	friend	TPropertyDialog;	// allows access to pdlgParent

	int 			nThisTab;

protected:
	TPropertyDialog	*pdlgParent;

	void    		CmOk ();      	// IDOK
	void    		CmCancel ();    // IDCANCEL
	void 			EvSysCommand (UINT cmdType, TPoint& pt);
	bool    		EvInitDialog (HWND hWndFocus);

	bool 			CheckHotKey (TDialog *dlg = NULL);

public:
	TSSPropertyPage (TResId resID, TModule *module = 0);
	TSSPropertyPage (TPropertyDialog *parent, TResId resID, TModule *module = 0);
	~TSSPropertyPage ();

	int             MessageBox (const char far* text,
								const char far* caption = 0,
								UINT            type = MB_OK);
	bool 			PreProcessMsg (MSG& msg);
	HBRUSH  		EvCtlColor (HDC hdc, HWND hwndCtl, uint ctlType);

	TPropertyDialog& GetPropertyDialog ()	{ return *pdlgParent; }
	TWindow			*GetParentWindow ();
	int				ThisTab ()				{ return nThisTab; }

	// tab selection
	virtual bool 	SelectTab (int nTab = CURRENTPAGE);
	virtual bool 	SelectNext ();
	virtual bool 	SelectPrevious ();

	// property functions
	string 			GetTabText (int nTab = CURRENTPAGE);
	void			SetTabText (int nTab, LPCSTR lpszText);
	void			EnableTab (int nTab = CURRENTPAGE, bool bEnable = true);
	bool			IsTabEnabled (int nTab = CURRENTPAGE) const;

	// overridable functions
	virtual void	AfterSelected ()							{  }
	virtual bool	BeforeLeaving ()							{ return true; }
	virtual bool	CanPageClose ()								{ return true; }
	virtual void	AfterCantClose ()							{ SelectTab (); }
	virtual void	OnRightClick (UINT modKeys, TPoint& point)	{  }

	virtual bool	SaveData ();

#if USETRANSFERARRAYS
	DECLARE_USETRANSFERARRAY(TSSPropertyPage, TDialog);
#endif

	DECLARE_RESPONSE_TABLE (TSSPropertyPage);
};

class PROPDLGEXPORT TPropertyDialog : public TDialog
{

protected:
	TPropertyTab	*pTabs;
	Tab::Style 		styleTabs;
	static TBrush*	brushGray;

	int				DoExecute ();

	void			EvDestroy ();
	bool    		EvInitDialog (HWND hWndFocus);
	void 			SizeTabWnd (TRect& client);
	void 			EvSysCommand (UINT cmdType, TPoint& pt);

	virtual void	AdjustMargin (TRect& rc);

public:
	TPropertyDialog (TWindow *pParent, TResId resID, Tab::Style style = Tab::SingleHeight | Tab::VariableWidth, TModule *module = 0);
	TPropertyDialog (TWindow *pParent, TResId resID, Tab::Style style, TResId resBitmap, bool bUseMask = false, TModule *module = 0);
	~TPropertyDialog ();

	static void		SetHookInstance (HINSTANCE hInstance);
	static void 	TabColorChange (TApplication *application = NULL);

	void			SetupWindow ();
	void 			Destroy (int retValue = 0);
	void			EvSize (UINT, TSize& size);
	HBRUSH  		EvCtlColor (HDC hdc, HWND hwndCtl, uint ctlType);

	// message routing
	bool 			Find (TEventInfo&, TEqualOperator = 0);
	LRESULT			EvCommand (UINT id, HWND hWndCtl, UINT notifyCode);

	// get reference to a particular page
	TSSPropertyPage&	GetPageRef (int nTab = CURRENTPAGE);
	int				GetEntryCount ();

	// function to add a new tab
	virtual void	Add (LPCSTR lpszTab, TSSPropertyPage *pPage, bool bEnabled = true);

	virtual void	CmOk ();      	// IDOK
	virtual void	CmCancel ();    // IDCANCEL
	virtual void	CloseAction (int retValue);

	// tab selection
	virtual bool 	SelectTab (int nTab);
	virtual bool 	SelectNext ();
	virtual bool 	SelectPrevious ();
	virtual void	AfterSelected (int nTab)	{ /* you can override this -- */ }
	virtual bool	BeforeLeaving (int nTab)	{ return true; /* you can override this -- */ }

	// global properties for every tab
	void 			SetActiveColor (TColor color = TColor::Black);
	void 			SetFixedTabWidth (int nWidth = DEFTABWIDTH);
	void 			SetWideMarginWidth (int nWidth = DEFWIDEMARGIN);

	// page iterators that take function pointers
	TSSPropertyPage*	FirstPageThat (TPageCondFunc test, void* paramList = 0);
	void            ForEachPage (TPageActionFunc action, void* paramList = 0);

	// page-iterators that take pointers to member functions
	TSSPropertyPage*	FirstPageThat (TPageCondMemFunc test, void* paramList = 0);
	void            ForEachPage (TPageActionMemFunc action, void* paramList = 0);

	// similar to ForEach - sends Windows message to each page
	LRESULT 		SendMsgToEachPage (UINT msg, WPARAM wParam = 0, LPARAM lParam = 0);

	// functions for changing the position of the tab control in the tabbing sequence
	void			MoveTabsAfterControl (HWND hwndControl, bool bMoveFocus = true);
	void			MoveTabsAfterControl (TWindow* wndControl)	{ MoveTabsAfterControl (wndControl->HWindow); }

	// function to return a pointer to the actual tab object
	TPropertyTab*	GetTabControl ()							{ return pTabs; }

private:
	static TResponseTableEntry<TPropertyDialog> __RTFAR	__entries[];
	typedef TResponseTableEntry<TPropertyDialog>::PMF   TMyPMF;
	typedef TPropertyDialog								TMyClass;
};

#endif
