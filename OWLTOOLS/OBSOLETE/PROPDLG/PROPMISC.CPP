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

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// PROPMISC.CPP
//
// this code is #included into PROPDLG.CPP and shouldn't
// be added to the project file directly
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#pragma codeseg PROPMISC

// =========================================================
// TPropertyEntry
//
// class which maintains details of a particular page
// =========================================================

TPropertyEntry::TPropertyEntry ()
{
	pPage		= NULL;
	bEnabled	= false;
	nWidth		= 0;
}

TPropertyEntry::TPropertyEntry (string sTabTextV, TSSPropertyPage *pPageV, bool bEnabledV)
{
	sTabText	= sTabTextV;
	pPage		= pPageV;
	bEnabled	= bEnabledV;
	nWidth		= 0;
}

TPropertyEntry&	TPropertyEntry::operator= (const TPropertyEntry& copy)
{
	sTabText	= copy.sTabText;
	pPage		= copy.pPage;
	bEnabled	= copy.bEnabled;
	nWidth		= copy.nWidth;

	return *this;
}

bool TPropertyEntry::operator== (const TPropertyEntry&)
{
	// only use for "Find", which I don't use...
	return false;
}

// =========================================================
// UseSmallFont
//
// recursively sends WM_SETFONT to all controls in a dialog
// =========================================================
void UseSmallFont (HWND HWindow)
{
	// use small font for all statics EXCEPT SS_NOPREFIX ones (titles)
	for (HWND hwndChild = ::GetWindow (HWindow, GW_CHILD); hwndChild; )
	{
		HWND 	hwndNext = ::GetWindow(hwndChild, GW_HWNDNEXT);
		char	szClass[20];

		GetClassName (hwndChild, szClass, sizeof (szClass));

		if (lstrcmpi (szClass, "STATIC") != 0 ||
			!(GetWindowLong (hwndChild, GWL_STYLE) & SS_NOPREFIX))
		{
			::SendMessage (hwndChild, WM_SETFONT, (WPARAM) GetStockObject (ANSI_VAR_FONT), NULL);
			UseSmallFont (hwndChild);
		}

		hwndChild = hwndNext;
	}
}

// =========================================================
// CenterWindow
//
// centers the given window on the screen
// =========================================================
void CenterWindow (TWindow* wnd)
{
	TRect	rc (wnd->GetWindowRect ());

	wnd->SetWindowPos (	NULL,
						TRect ( (GetSystemMetrics (SM_CXSCREEN) - rc.Width ()) >> 1,
								(GetSystemMetrics (SM_CYSCREEN) - rc.Height ()) >> 1, 0, 0),
						SWP_NOSIZE | SWP_NOZORDER);
}

// =========================================================
// DrawMaskedBitmap
//
// draws a bitmap given containing a mask
// =========================================================
bool DrawMaskedBitmap (HDC hDC, int x, int y, HBITMAP hbm, int XOrigin, int nWidth)
{
	HDC 	hdcShadow;
	HDC 	hdcSource;
	HBITMAP hbmpOld;
	HBITMAP hbmpOld2;
	HBITMAP hbmDraw;
    BITMAP	bm;
	int		nHeight;
	bool	bReturn;

	if (hbm == NULL)
	{
    	return false;
	}

	GetObject (hbm, sizeof (bm), &bm);

	if (nWidth == 0)
	{
		nWidth = bm.bmWidth - XOrigin;
	}

	nHeight = bm.bmHeight >> 1;

	hbmDraw = CreateCompatibleBitmap (hDC, nWidth, nHeight);

    if (hbmDraw == NULL)
	{
    	return false;
	}

	hdcShadow = CreateCompatibleDC (hDC);
	hdcSource = CreateCompatibleDC (hDC);

	hbmpOld 	= (HBITMAP) SelectObject (hdcShadow, hbmDraw);
	hbmpOld2	= (HBITMAP) SelectObject (hdcSource, hbm);

	bReturn		= BitBlt (hdcShadow, 0, 0, nWidth, nHeight, hDC, x, y, SRCCOPY);

	if (bReturn)
    {
		bReturn	= BitBlt (hdcShadow, 0, 0, nWidth, nHeight, hdcSource, XOrigin, nHeight, SRCAND);
	}

	if (bReturn)
    {
		bReturn	= BitBlt (hdcShadow, 0, 0, nWidth, nHeight, hdcSource, XOrigin, 0, SRCPAINT);
	}

	if (bReturn)
    {
		bReturn	= BitBlt (hDC, x, y, nWidth, nHeight, hdcShadow, 0, 0, SRCCOPY);
	}

	SelectObject (hdcShadow, hbmpOld);
	SelectObject (hdcSource, hbmpOld2);

	DeleteDC (hdcShadow);
	DeleteDC (hdcSource);

	DeleteObject (hbmDraw);

	return bReturn;
}

// =========================================================
// GetNextTabKey
//
// locate first/last tab position
// =========================================================
static HWND GetEndTabPosition (TWindow *window, bool bFirst)
{
	// identify last control
	HWND	hwndTab = window->GetNextDlgTabItem (::GetWindow (window->HWindow, GW_CHILD), true);

	if (hwndTab && bFirst)
	{
		// identify first control
		hwndTab = window->GetNextDlgTabItem (hwndTab);
	}

	if (hwndTab)
	{
		//
		// just to make sure it really does have the tabstop style
		//
		if (!(GetWindowLong (hwndTab, GWL_STYLE) & WS_TABSTOP))
		{
			return NULL;
		}
	}

	return hwndTab;
}

// =========================================================
// SetupDialogPtr
//
// set up the page/dialog pointers
// =========================================================
static void SetupDialogPtr (TWindow *window, TPropertyDialog*& dialog, TSSPropertyPage*& page)
{
	dialog	= NULL;
	page	= NULL;

	// see if it is on the page
	page = dynamic_cast<TSSPropertyPage *> (window);

	if (!page)
	{
		// see if it is on the page
		dialog = dynamic_cast<TPropertyDialog *> (window);
	}
}

// =========================================================
// GetDialogPtr
//
// get dialog window pointer from the focus window
// =========================================================
static TWindow* GetDialogPtr (HWND& hwndFocus)
{
	hwndFocus = GetFocus ();

	TWindow *window		= GetWindowPtr (GetParent (hwndFocus));

	// if the focus is in the listbox/edit window of a combobox,
	// use the parent of the combobox
	if (window)
	{
		if (dynamic_cast<TComboBox *> (window))
		{
			hwndFocus = window->HWindow;

			window = window->Parent;
		}
	}
	else
	{
		char	szClassName[16];

		GetClassName (GetParent (hwndFocus), szClassName, sizeof (szClassName));

		if (lstrcmpi (szClassName, "combobox") == 0)
		{
			hwndFocus = GetParent (hwndFocus);

			window = GetWindowPtr (GetParent (hwndFocus));
		}

	}

	return window;
}

// =========================================================
// MoveToControl
//
// move from a control in one dialog to one in another
// =========================================================
static bool MoveToControl (TWindow* dlgFrom, TWindow* dlgTo, HWND hwndControl)
{
	if (hwndControl)
	{
		if (dlgFrom)
		{
			dlgFrom->PostMessage (WM_NEXTDLGCTL, NULL, true);
		}
		dlgTo->PostMessage (WM_NEXTDLGCTL, (WPARAM) hwndControl, true);

		SelectEditText (hwndControl);

		return true;
	}
	else
	{
		//
		// nothing on the page, so skip it
		//
		return false;
	}
}

// =========================================================
// ProcessTabKey
//
// perform tab-key navigation
// =========================================================
static bool ProcessTabKey (TWindow *window, HWND hwndFocus, bool bForwards)
{
	TPropertyDialog	*dialog;
	TSSPropertyPage	*page;
	HWND			hwndLimit;

	SetupDialogPtr (window, dialog, page);

	if (!page && !dialog)
	{
		// can't handle this as it isn't intended for us..
		return false;
	}

	//
	// handle tabbing on and off the TPropertyTab control
	//
	if (dialog)
	{
		TPropertyTab*	tabs = dialog->GetTabControl ();

		if (!tabs)
		{
			return false;
		}

		HWND 		hwndNext = dialog->GetNextDlgTabItem (hwndFocus, !bForwards);
		TWindow* 	newwindow = &dialog->GetPageRef ();

		//
		// tabbing from the tab control
		//
		if (hwndFocus == tabs->HWindow)
		{
			if (bForwards)
			{
				//
				// locate the first tabbable control on the page
				//
				return MoveToControl (window, newwindow, GetEndTabPosition (newwindow, true));
			}
			else
			{
				if (hwndNext != hwndFocus)
				{
					return false;
				}
			}
		}

		//
		// tabbing onto the tab control
		//
		if (hwndNext == tabs->HWindow)
		{
			if (bForwards)
			{
				//
				// tabbing on to the tab control - no special action required
				//
				return false;
			}
			else
			{
				//
				// locate the last tabbable control on the page
				//
				return MoveToControl (window, newwindow, GetEndTabPosition (newwindow, false));
			}
		}
	}

	//
	// handle tabbing from the page down to the property dialog
	//
	if (page)
	{
		// identify last control
		hwndLimit = GetEndTabPosition (window, !bForwards);

		if (!hwndLimit)
		{
			// no tabbable items :-(
			return false;
		}

		if (hwndFocus == hwndLimit)
		{
			TPropertyDialog* newwindow 	= &page->GetPropertyDialog ();
			HWND			 tabWnd		= newwindow->GetTabControl ()->HWindow;

			if (bForwards)
			{
				//
				// jump to the control after the tab control
				//
				return MoveToControl (page, newwindow, newwindow->GetNextDlgTabItem (tabWnd, false));
			}
			else
			{
				//
				// jump to the tab control itself
				//
				return MoveToControl (page, newwindow, tabWnd);
			}
		}
	}

	return false;
}

void SelectEditText (HWND hwndControl)
{
	LRESULT lResult 	= ::SendMessage (hwndControl, WM_GETDLGCODE, NULL, NULL);

	if (lResult & DLGC_HASSETSEL)
	{
	#if defined(__WIN32__)
		::PostMessage (hwndControl, EM_SETSEL, 0, 32767);
	#else
		::PostMessage (hwndControl, EM_SETSEL, 0, MAKELPARAM (0, 32767));
	#endif
	}
}

// =========================================================
// PropdlgKbdProc
//
// keyboard hook used to perform tab-key navigation
// =========================================================
#include <owl\combobox.h>

LRESULT CALLBACK _export PropdlgKbdProc (int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0 || code == HC_NOREMOVE)
	{
		return CallNextHookEx (hookKbd, code, wParam, lParam);
	}

	HWND	hwndFocus;
	TWindow	*window;
	bool 	bForwards;

	bool result = false;

	if (wParam == VK_TAB)
	{
		int	nAlt = GetKeyState (VK_MENU);

		if (GetKeyState (VK_TAB) < 0 && nAlt >= 0)
		{
			bForwards 	= !(GetKeyState (VK_SHIFT) < 0);

			window = GetDialogPtr (hwndFocus);

			if (window)
			{
				result = ProcessTabKey (window, hwndFocus, bForwards);
			}
		}
	}
	else
	{
		bForwards 	= (GetKeyState (VK_NEXT) < 0);
		int	nCtrl = GetKeyState (VK_CONTROL);

		if (nCtrl < 0 && (GetKeyState (VK_PRIOR) < 0 || bForwards))
		{
			TPropertyDialog	*dialog;
			TSSPropertyPage	*page;

			window = GetDialogPtr (hwndFocus);

			if (window)
			{
				SetupDialogPtr (window, dialog, page);

				if (page)
				{
					if (bForwards)
					{
						page->GetPropertyDialog ().SelectNext ();
					}
					else
					{
						page->GetPropertyDialog ().SelectPrevious ();
					}
				}
				if (dialog)
				{
					if (bForwards)
					{
						dialog->SelectNext ();
					}
					else
					{
						dialog->SelectPrevious ();
					}
				}
			}
		}
	}

	CallNextHookEx (hookKbd, code, wParam, lParam);

	return result;
}

// =========================================================
// GetPressedKey
//
// get the ASCII code of the currently depressed key
// =========================================================
static char GetPressedKey ()
{
	BYTE	byStates[256];
	char	cKey;

	GetKeyboardState (byStates);

	for (int n = 0; n <= 255; n++)
	{
	#ifdef __WIN32__
		WORD	dwKey;
	#else
		DWORD	dwKey;
	#endif

		if (n != VK_MENU && (byStates [n] & 0x80))	// if this key is down
		{
			if (ToAscii (n, NULL, byStates, &dwKey, true))
			{
				cKey = (char) (dwKey & 0xFF);
				cKey = LOWORD ((DWORD) AnsiUpper ((LPSTR) MAKELPARAM (cKey, NULL)));

				return cKey;
			}
		}
	}

	return NULL;
}

// =========================================================
// GetPressedKey
//
// get the ASCII code of the currently depressed key
// =========================================================
static bool MatchesHotKey (LPCSTR lpszPrefix, char cKey)
{
	for (;;)
	{
		// locate the prefix character
		lpszPrefix = strchr (lpszPrefix, '&');

		// if found, and not a '&&' sequence
		if (lpszPrefix == NULL || *(++lpszPrefix) != '&')
		{
			break;
		}

		lpszPrefix++;	// skip the second '&'
	}

	if (lpszPrefix != NULL)
	{
		char	cChar = *lpszPrefix;

		cChar = LOWORD ((DWORD) AnsiUpper ((LPSTR) MAKELPARAM (cChar, NULL)));

		if (cChar == cKey)
		{
			return true;
		}
	}

	return false;
}

// =========================================================
// DetachTab
//
// removes this control from the main tab control list
// =========================================================
void DetachTab (TPropertyTab* tab)
{
	for (int n = 0; n < arrayTabs.GetItemsInContainer (); n++)
	{
		if (arrayTabs[n] == tab)
		{
			arrayTabs.Detach (n);

			break;
		}
	}
}

