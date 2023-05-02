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


#define OEMRESOURCE		// needed to load scrollbar bitmaps

#include <owl\combobox.h>

#include <ctype.h>
#include <cstring.h>
#include <checks.h>
#include <owltools\propdlg.h>
#include <owltools\proptab.h>

//
// increase this value to make the 
// tabs appear more rounded
//
#define ROUNDEDMARGIN		4
#define NONSELECTEDSPACE	true

#ifndef ROUNDEDMARGIN
  #define ROUNDEDMARGIN	2
#endif

#define TABMARGIN			8
#define SCROLLMARGIN		8
#define WIZARDMARGIN		12

// default spacing between tab border and OK button
#define ADJUSTBUTTONX 		8
#define ADJUSTBUTTONY 		6

#define USEANSIVARFONT		true
#define SELECTIONWEIGHT		FW_BOLD

// effect of Tab::ColorActive
#define COLORWHOLETAB		true

/* static void EnableParents (TWindow *wnd, bool bEnable); */
static char 	GetPressedKey ();
static bool 	MatchesHotKey (LPCSTR lpszText, char cKey);
static void 	DetachTab (TPropertyTab* tab);
static void 	SelectEditText (HWND hwndControl);

static HWND 	GetEndTabPosition (TWindow *window, bool bFirst);
static bool 	MoveToControl (TWindow* dlgFrom, TWindow* dlgTo, HWND hwndControl);

// =========================================================
// Tab
//
// definitions of tab style constants
// =========================================================
const Tab::Style	Tab::SingleHeight				(0x00000L);
const Tab::Style	Tab::VariableWidth				(0x00000L);
const Tab::Style	Tab::DoubleHeight				(0x00001L);
const Tab::Style	Tab::FixedWidth					(0x00002L);
const Tab::Style	Tab::ColorActive				(0x00004L);
const Tab::Style	Tab::Collapsing					(0x00008L);
const Tab::Style	Tab::Justified					(0x00010L);
const Tab::Style	Tab::Stacked					(0x00020L);
const Tab::Style	Tab::AllowDupPages				(0x00040L);
const Tab::Style	Tab::CreateOnDemand				(0x00080L);
const Tab::Style	Tab::WideMargins				(0x00100L);
const Tab::Style	Tab::AutoTabResize				(0x00200L);
const Tab::Style	Tab::ButtonsOnRight				(0x00400L);
const Tab::Style	Tab::UseSmallFont				(0x00800L);
const Tab::Style	Tab::SaveOnClosePage			(0x01000L);
const Tab::Style	Tab::NoMDIClose					(0x02000L);
const Tab::Style	Tab::WizardFrame				(0x04000L);
const Tab::Style	Tab::UseTabBitmaps				(0x08000L);
const Tab::Style	Tab::CenterDialog				(0x10000L);
const Tab::Style	Tab::CreateOnDemandAndKeepIt	(0x20000L);
const Tab::Style	Tab::FocusOnFirstControl		(0x40000L);

// =========================================================
// TPropertyDialog
//
// base class for property dialog (holder) class
// =========================================================
DEFINE_RESPONSE_TABLE_ENTRIES (TPropertyDialog)
	EV_COMMAND(IDOK, CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
	EV_WM_DESTROY,
	EV_WM_SIZE,
	EV_WM_SYSCOMMAND,
	EV_WM_CTLCOLOR,
END_RESPONSE_TABLE;

static TIArrayAsVector<TPropertyTab> arrayTabs (5, 0, 5);
static HINSTANCE	hInstProc = NULL;
static HHOOK		hookKbd = NULL;
static bool 		bWin95	= false;

TBrush*	TPropertyDialog::brushGray = NULL;

LRESULT CALLBACK PropdlgKbdProc (int, WPARAM, LPARAM);

TPropertyDialog::TPropertyDialog (TWindow *pParent, TResId resID, Tab::Style style, TModule *module)
	:	TDialog (pParent, resID, module)
{
	pTabs 		= new TPropertyTab (this, style, resID);
	IsModal 	= false;	// not modal by default
	styleTabs	= style;

	CHECKX(pTabs, "Tab control could not be created");
}

TPropertyDialog::TPropertyDialog (TWindow *pParent, TResId resID, Tab::Style style, TResId resBitmap, bool bUseMask, TModule *module)
	:	TDialog (pParent, resID, module)
{
	style = style | Tab::UseTabBitmaps;

	pTabs 		= new TPropertyTab (this, style, resBitmap, bUseMask);
	IsModal 	= false;	// not modal by default
	styleTabs	= style;

	CHECKX(pTabs, "Tab control could not be created");
}

TPropertyDialog::~TPropertyDialog ()
{
	if (pTabs)
	{
		if (!arrayTabs.GetItemsInContainer () && hookKbd)
		{
			UnhookWindowsHookEx (hookKbd);

			hookKbd = NULL;

			delete brushGray;
            brushGray = NULL;
		}
	}
}

void TPropertyDialog::SetHookInstance (HINSTANCE hInstance)
{
	hInstProc = hInstance;
}

// =========================================================
// SetupWindow, EvInitDialog
//
// initialise this dialog box.
// EvInitDialog is overriden so I can control the focus
// =========================================================
void TPropertyDialog::SetupWindow ()
{
	TDialog::SetupWindow ();

	if (styleTabs & Tab::UseSmallFont)
	{
		UseSmallFont (HWindow);
	}

	if (GetEntryCount ())
	{
		if (!arrayTabs.GetItemsInContainer ())
		{
			brushGray = new TBrush (GetSysColor (COLOR_BTNFACE));

			if (!hInstProc)
			{
				hInstProc = GetModule ()->GetInstance ();
			}

		#ifndef __WIN32__
			hookKbd = SetWindowsHookEx (WH_KEYBOARD, PropdlgKbdProc, hInstProc, GetCurrentTask ());
		#else
			hookKbd = SetWindowsHookEx (WH_KEYBOARD, PropdlgKbdProc, hInstProc, GetCurrentThreadId ());
		#endif
		}

		arrayTabs.Add (pTabs);

		SizeTabWnd (GetClientRect ());

		MoveTabsAfterControl (HWND_TOP);

		if (styleTabs & Tab::FocusOnFirstControl)
		{
			pTabs->SetPageFocus (true);
		}
	}
	else
	{
		if (pTabs)
		{
			DetachTab (pTabs);

			pTabs->Destroy ();
			delete pTabs;
			pTabs = NULL;
		}
	}

    if (styleTabs & Tab::CenterDialog)
	{
        CenterWindow (this);
	}
}

bool TPropertyDialog::EvInitDialog (HWND hWndFocus)
{
	TDialog::EvInitDialog (hWndFocus);

	// don't set focus if a page was displayed
	return (pTabs == NULL);
}

HBRUSH TPropertyDialog::EvCtlColor (HDC hDC, HWND hWndChild, uint ctlType)
{
	if (bWin95 && brushGray)
	{
		switch (ctlType)
		{
			case CTLCOLOR_LISTBOX :
			case CTLCOLOR_EDIT :
			case CTLCOLOR_MSGBOX :
			{
				break;
			}

			default :
			{
				if (::IsWindowEnabled (hWndChild))
				{
					SetTextColor (hDC, 0L);
				}
				else
				{
					SetTextColor (hDC, GetSysColor (COLOR_GRAYTEXT));
				}

				SetBkColor (hDC, GetSysColor (COLOR_BTNFACE));

				return *brushGray;
			}
		}
	}

	return TDialog::EvCtlColor (hDC, hWndChild, ctlType);
}

void TPropertyDialog::EvDestroy ()
{
	if (pTabs)
	{
		pTabs->DestroyPages ();
		DetachTab (pTabs);
	}

	TDialog::EvDestroy ();
}

// =========================================================
// EvSysCommand
//
// picks up requests for Alt+letter hot-key switching
// =========================================================
void TPropertyDialog::EvSysCommand (UINT cmdType, TPoint& pt)
{
	if (cmdType == SC_KEYMENU && GetEntryCount ())
	{
		if (GetPageRef ().CheckHotKey ())
		{
			// hotkey to a control on the parent
			return;
		}
	}

	TDialog::EvSysCommand (cmdType, pt);
}

// =========================================================
// Add
//
// add a property page to this dialog
// =========================================================
void TPropertyDialog::Add (LPCSTR lpszTab, TSSPropertyPage *pPage, bool bEnabled)
{
	PRECONDITIONX(lpszTab, "Invalid property page name supplied");
	PRECONDITIONX(pPage, "Invalid property page dialog supplied");

	pPage->pdlgParent	= this;
	pPage->nThisTab		= GetEntryCount ();

	pTabs->Add (lpszTab, pPage, bEnabled);
}

// =========================================================
// AdjustMargin
//
// allow a derived dialog a change to override the position
// of the actual tab area (to allow for comment areas etc.)
// =========================================================
void TPropertyDialog::AdjustMargin (TRect& rc)
{
	HWND	hwndOK = GetDlgItem (IDOK);

	if (hwndOK)
	{
		TRect	rcOK;

		::GetWindowRect (hwndOK, &rcOK);

		ScreenToClient (rcOK.TopLeft ());

		if (pTabs->GetTabStyle () & Tab::ButtonsOnRight)
		{
			rc.right = rcOK.left - ADJUSTBUTTONX;
		}
		else
		{
			rc.bottom = rcOK.top - ADJUSTBUTTONY;
		}
	}
}

void TPropertyDialog::EvSize (UINT sizeType, TSize& size)
{
	TDialog::EvSize (sizeType, size);

	if (sizeType != SIZE_MINIMIZED && pTabs && (pTabs->GetTabStyle () & Tab::AutoTabResize))
	{
		// move the tab window
		TRect	client;

		GetClientRect (client);

		SizeTabWnd (client);
	}
}

void TPropertyDialog::SizeTabWnd (TRect& client)
{
	if (pTabs)
	{
		if (pTabs->GetTabStyle () & Tab::WizardFrame)
		{
			client.left		-= 2;
			client.right	+= 2;
			client.bottom	+= 2;

			client.top		-= pTabs->GetTabHeight ();
		}
		else
		{
			client = client.InflatedBy (-TABMARGIN, -TABMARGIN);
		}

		AdjustMargin (client);

		pTabs->SetWindowPos (NULL, client, SWP_NOZORDER);
		pTabs->Invalidate ();
	}
}

// =========================================================
// MoveTabsAfterControl
//
// moves tab control after another control in the dialog box
// =========================================================
void TPropertyDialog::MoveTabsAfterControl (HWND hwndControl, bool bMoveFocus)
{
	pTabs->SetWindowPos (hwndControl, TRect (), SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	if (bMoveFocus)
	{
		MoveToControl (NULL, this, GetEndTabPosition (this, true));
	}
}

// =========================================================
// SetActiveColour
//
// defines the colour of the active tab
// (only if using Tab::ColorActive style)
// =========================================================
void TPropertyDialog::SetActiveColor (TColor color)
{
	if (pTabs)
	{
		pTabs->SetActiveColor (color);
	}
}

// =========================================================
// SetFixedTabWidth
//
// defines the width of tabs when using "FixedWidth" style
// =========================================================
void TPropertyDialog::SetFixedTabWidth (int nWidth)
{
	if (pTabs)
	{
		pTabs->SetFixedTabWidth (nWidth);
	}
}

// =========================================================
// SetWideMarginWidth
//
// defines the extra space added to the each side of a tab
// defined with the "WideMargin" style
// =========================================================
void TPropertyDialog::SetWideMarginWidth (int nWidth)
{
	pTabs->SetWideMarginWidth (nWidth);
}

// =========================================================
// CmOk
//
// handler for OK button.  Asks all pages to save their data
// then closes the dialog
// =========================================================
void TPropertyDialog::CmOk ()
{
	if (styleTabs & Tab::NoMDIClose)
	{
		return;
	}

	if (pTabs == NULL || (pTabs->CheckCanClose () && pTabs->SavePageData ()))
	{
		CloseAction (IDOK);
	}
}

// =========================================================
// CmCancel
//
// handler for Cancel button
// =========================================================
void TPropertyDialog::CmCancel ()
{
	CloseAction (IDCANCEL);
}

// =========================================================
// CloseAction
//
// requests a dialog be closed (can be overriden where the
// dialog is the client to a TMDIChild dialog)
// =========================================================
void TPropertyDialog::CloseAction (int retValue)
{
	if (styleTabs & Tab::NoMDIClose)
	{
		return;
	}

	switch (retValue)
	{
		case IDOK :
			TDialog::CmOk ();
			break;

		case IDCANCEL :
			TDialog::CmCancel ();
			break;

		default :
			CloseWindow (retValue);
			break;
	}
}

// =========================================================
// Destroy
//
// called when the window is destroyed
// =========================================================
void TPropertyDialog::Destroy (int retValue)
{
	if (retValue && IsModal)
	{
	//	EnableParents (Parent, true);

		GetApplication ()->EndModal (retValue);
	}

	if (pTabs)
	{
		pTabs->DestroyPages ();
		DetachTab (pTabs);
	}

	// make sure OWL doesn't get confused by my
	// setting of the modal variable
	IsModal = false;

	TDialog::Destroy (retValue);
}

/*
static void EnableParents (TWindow *wnd, bool bEnable)
{
	while (wnd)
	{
		if (!(wnd->GetWindowLong (GWL_STYLE) & WS_CHILD))
		{
			wnd->EnableWindow (bEnable);
		}

		wnd = wnd->Parent;
	}
}	*/

// =========================================================
// DoExecute
//
// overriden to make the modeless dialog appear modal to the
// caller, whilst allowing me to retain control of messages!
// =========================================================
int TPropertyDialog::DoExecute()
{
	Create ();
	Show (SW_SHOW);

	IsModal = true;	// make the dialog appear modal

/*	EnableParents (Parent, false); */

	return GetApplication ()->BeginModal (Parent);
}

// =========================================================
// SelectTab
//
// select a specific page
// =========================================================
bool TPropertyDialog::SelectTab (int nTab)
{
	if (pTabs)
	{
		return pTabs->SelectTab (nTab);
	}
	else
	{
		return false;
	}
}

// =========================================================
// SelectNext, SelectPrevious
//
// select the previous/next page
// =========================================================
bool TPropertyDialog::SelectNext ()
{
	if (pTabs)
	{
		return pTabs->SelectNext ();
	}
	else
	{
		return false;
	}
}

bool TPropertyDialog::SelectPrevious ()
{
	if (pTabs)
	{
		return pTabs->SelectNext (-1);
	}
	else
	{
		return false;
	}
}

// =========================================================
// FirstPageThat
//
// functions that identify the first page meeting some
// criteria (determined by the specific function)
// =========================================================
TSSPropertyPage* TPropertyDialog::FirstPageThat (TPageCondFunc test, void* paramList)
{
	int	nCount = GetEntryCount ();

	for (int n = 0; n < nCount; n++)
	{
		TSSPropertyPage	*pPage = pTabs->GetPage (n);

		if (test (pPage, paramList))
		{
			return pPage;
		}
	}

	return NULL;
}

TSSPropertyPage* TPropertyDialog::FirstPageThat (TPageCondMemFunc test, void* paramList)
{
	int	nCount = GetEntryCount ();

	for (int n = 0; n < nCount; n++)
	{
		TSSPropertyPage	*pPage = pTabs->GetPage (n);

		if ((this->*test) (pPage, paramList))
		{
			return pPage;
		}
	}

	return NULL;
}

// =========================================================
// ForEachPage
//
// functions called for every page
// =========================================================
void TPropertyDialog::ForEachPage (TPageActionFunc action, void* paramList)
{
	int	nCount = GetEntryCount ();

	for (int n = 0; n < nCount; n++)
	{
		action (pTabs->GetPage (n), paramList);
	}
}

void TPropertyDialog::ForEachPage (TPageActionMemFunc action, void* paramList)
{
	int	nCount = GetEntryCount ();

	for (int n = 0; n < nCount; n++)
	{
		(this->*action) (pTabs->GetPage (n), paramList);
	}
}

// =========================================================
// GetPageRef
//
// get reference to a particular page
// =========================================================
TSSPropertyPage& TPropertyDialog::GetPageRef (int nTab)
{
	if (nTab == CURRENTPAGE)
	{
		nTab = pTabs->GetSelectedTab ();
	}

	PRECONDITIONX (nTab >= 0 && nTab < GetEntryCount (), "GetPage: Invalid index");

	return *(pTabs->GetPage (nTab));
}

// =========================================================
// GetEntryCount
//
// returns the number of pages in the property dialog
// =========================================================
int TPropertyDialog::GetEntryCount ()
{
	if (pTabs == NULL)
	{
		return NULL;
	}

	return pTabs->GetEntryCount ();
}

// =========================================================
// SendMsgToEachPage
//
// sends the given message to each page
// returns the return value of the last page
// =========================================================
LRESULT TPropertyDialog::SendMsgToEachPage (UINT msg, WPARAM wParam, LPARAM lParam)
{
	int		nCount = GetEntryCount ();
	LRESULT	lResult = NULL;

	for (int n = 0; n < nCount; n++)
	{
		lResult = pTabs->GetPage (n)->SendMessage (msg, wParam, lParam);
	}

	return lResult;
}


// =========================================================
// TabColorChange
//
// picks up colors to use from Control Panel
// =========================================================
static void TabColorChangeIter (TPropertyTab& tab, void *)
{
	tab.TabColorChange ();
}

void TPropertyDialog::TabColorChange (TApplication *application)
{
	arrayTabs.ForEach (TabColorChangeIter, NULL);

	if (application && application->Ctl3dEnabled ())
	{
		if (brushGray)
		{
			delete brushGray;
			brushGray	= new TBrush ((HBRUSH) GetSysColor (COLOR_BTNFACE));
		}

		FARPROC	proc = application->GetCtl3dModule ()->GetProcAddress ("Ctl3dColorChange");

		if (proc)
		{
			proc ();
		}
	}
}

// =========================================================
// Find, Dispatch
//
// message routing for command enablers
// =========================================================

bool TPropertyDialog::Find (TEventInfo& eventInfo, TEqualOperator equal)
{
	if (pTabs && (eventInfo.Msg == WM_COMMAND_ENABLE || eventInfo.Msg == WM_COMMAND))
	{
		if (GetPageRef ().Find (eventInfo, equal))
		{
			return true;
		}
	}

	eventInfo.Object = (GENERIC*)this;

	return 	SearchEntries ((TGenericTableEntry __RTFAR*) __entries, eventInfo, equal) ||
			TDialog::Find (eventInfo, equal);
}

// =========================================================
// EvCommand
//
// redirect menu (& gadget) messages to current page if it
// has a handler for them
// =========================================================
LRESULT TPropertyDialog::EvCommand (UINT id, HWND hWndCtl, UINT notifyCode)
{
	if (!hWndCtl && id > IDCANCEL)
	{
		TEventInfo  eventInfo(notifyCode, id);

		if (pTabs && GetPageRef ().Find (eventInfo))
		{
			return GetPageRef ().EvCommand (id, hWndCtl, notifyCode);
		}
	}

	return TDialog::EvCommand (id, hWndCtl, notifyCode);
}

// ======================================================= //

// =========================================================
// TSSPropertyPage
//
// base class for a property page inside a property dialog
// =========================================================

DEFINE_RESPONSE_TABLE1 (TSSPropertyPage, TDialog)
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
  EV_WM_SYSCOMMAND,
  EV_WM_CTLCOLOR,
END_RESPONSE_TABLE;

TSSPropertyPage::TSSPropertyPage (TResId resID, TModule *module)
	:	TDialog (0, resID, module)
{
}

TSSPropertyPage::TSSPropertyPage (TPropertyDialog *parent, TResId resID, TModule *module)
	:	TDialog (parent, resID, module)
{
}

TSSPropertyPage::~TSSPropertyPage ()
{
}

bool TSSPropertyPage::EvInitDialog (HWND hWndFocus)
{
	TDialog::EvInitDialog (hWndFocus);

	return false;
}

HBRUSH TSSPropertyPage::EvCtlColor (HDC hDC, HWND hWndChild, uint ctlType)
{
	return GetPropertyDialog ().EvCtlColor (hDC, hWndChild, ctlType);
}

void TSSPropertyPage::CmOk ()
{
	// pass it on to the TPropertyDialog
	pdlgParent->ForwardMessage ();
}

void TSSPropertyPage::CmCancel ()
{
	// pass it on to the TPropertyDialog
	pdlgParent->ForwardMessage ();
}

bool TSSPropertyPage::SaveData ()
{
	// use transfer buffer to save data
	// (if available)
	TransferData (tdGetData);

	// continue by default
	return true;
}

bool TSSPropertyPage::PreProcessMsg (MSG& msg)
{
	//
	// this code is to prevent multiple line edit controls
	// with the ES_WANTRETURN from closing just this page
	// when ESCAPE is pressed (a bug in Win3.1)
	//
	if (msg.message == WM_CHAR && msg.wParam == (WPARAM) VK_ESCAPE)
	{
		char	szName[20];

		::GetClassName (msg.hwnd, szName, sizeof (szName));

		if (!lstrcmpi (szName, "EDIT"))
		{
			pdlgParent->PostMessage (WM_COMMAND, IDCANCEL);
		}

		return true;
	}

	return TDialog::PreProcessMsg (msg);
}

// =========================================================
// EvSysCommand
//
// picks up requests for Alt+letter hot-key switching
// =========================================================
void TSSPropertyPage::EvSysCommand (UINT cmdType, TPoint& pt)
{
	if (cmdType == SC_KEYMENU)
	{
		if (CheckHotKey (pdlgParent))
		{
			// hotkey to a control on the parent
			return;
		}
	}

	TDialog::EvSysCommand (cmdType, pt);
}

// =========================================================
// Select..., EnableTab
//
// access functions passed on to the tab control
// =========================================================
bool TSSPropertyPage::SelectTab (int nTab)
{
	if (nTab == CURRENTPAGE)
	{
		nTab = nThisTab;
	}

	return TYPESAFE_DOWNCAST (Parent, TPropertyTab)->SelectTab (nTab);
}

bool TSSPropertyPage::SelectNext ()
{
	return TYPESAFE_DOWNCAST (Parent, TPropertyTab)->SelectNext ();
}

bool TSSPropertyPage::SelectPrevious ()
{
	return TYPESAFE_DOWNCAST (Parent, TPropertyTab)->SelectNext (-1);
}

void TSSPropertyPage::EnableTab (int nTab, bool bEnable)
{
	if (nTab == CURRENTPAGE)
	{
		nTab = nThisTab;
	}

	TYPESAFE_DOWNCAST (Parent, TPropertyTab)->EnableTab (nTab, bEnable);
}

bool TSSPropertyPage::IsTabEnabled (int nTab) const
{
	if (nTab == CURRENTPAGE)
	{
		nTab = nThisTab;
	}

	return TYPESAFE_DOWNCAST (Parent, TPropertyTab)->IsTabEnabled (nTab);
}

string TSSPropertyPage::GetTabText (int nTab)
{
	if (nTab == CURRENTPAGE)
	{
		nTab = nThisTab;
	}

	return TYPESAFE_DOWNCAST (Parent, TPropertyTab)->GetTabText (nTab);
}

void TSSPropertyPage::SetTabText (int nTab, LPCSTR lpszText)
{
	if (nTab == CURRENTPAGE)
	{
		nTab = nThisTab;
	}

	TYPESAFE_DOWNCAST (Parent, TPropertyTab)->SetTabText (nTab, lpszText);
}

// =========================================================
// MessageBox
//
// focus-safe way to display a messagebox from inside a page
// =========================================================
int TSSPropertyPage::MessageBox (const char far* text, const char far* caption, UINT type)
{
	return GetParentWindow ()->MessageBox (text, caption, type);
}

// =========================================================
// GetParentWindow
//
// returns a window that can be used as a parent to a
// popup window from this page
// =========================================================
TWindow *TSSPropertyPage::GetParentWindow ()
{
	TWindow*	parent;

	for (	parent = pdlgParent;
			parent != NULL && !(parent->Attr.Style & WS_CAPTION);
			parent = parent->Parent) ;

	return parent;
}

// =========================================================
// CheckHotKey
//
// check to see if current alt-letter combination is a
// hot-key either for a tab or in the 'other' dialog (ie. if
// we are currently on a page, check the main property
// dialog, otherwise check the page)
// =========================================================
bool TSSPropertyPage::CheckHotKey (TDialog *dlg)
{
	// work out which key was pressed
	char	cKey = GetPressedKey ();

	if (TYPESAFE_DOWNCAST (Parent, TPropertyTab)->SelectTabByKey (cKey))
	{
		// page hotkey, so don't pass to Windows
		return true;
	}

	if (dlg == NULL)
	{
		dlg = this;
	}

	if (!cKey)
	{
		return false;
	}

	// walk through the childlist looking for a match
	HWND	hwndFirst = ::GetWindow (dlg->HWindow, GW_CHILD);

	for (HWND hwndChild = hwndFirst; hwndChild != NULL; hwndChild = ::GetWindow (hwndChild, GW_HWNDNEXT))
	{
		char	szText[100];

		::GetWindowText (hwndChild, szText, sizeof (szText));

		if (::IsWindowEnabled (hwndChild) && MatchesHotKey (szText, cKey))
		{
			// if I can't go here, find the next available tabbable control
			if (!(::GetWindowLong (hwndChild, GWL_STYLE) & WS_TABSTOP))
			{
				hwndChild = dlg->GetNextDlgTabItem (hwndChild, false);
			}

			// spoof a mouse-click
			::SendMessage (hwndChild, WM_LBUTTONDOWN, MK_LBUTTON, NULL);
			::SendMessage (hwndChild, WM_LBUTTONUP, MK_LBUTTON, NULL);

			SelectEditText (hwndChild);
			return true;
		}
	}

	return false;
}

// ======================================================= //

#include "propdlg\proptab.cpp"		// implementation of TPropertyTab
#include "propdlg\propmisc.cpp"		// miscellaneous stuff

