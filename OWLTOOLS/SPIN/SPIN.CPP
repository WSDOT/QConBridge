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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <owl\gdiobjec.h>
#include <owl\menu.h>
#include <owltools\spin.h>

#define MARGIN	2
#define WIDTH	(1 + ((5 + MARGIN) << 1))

#define idTIMER1		101
#define idTIMER2		102
#define TIMERDELAY1		(250 + (GetProfileInt ("Windows", "KeyboardDelay", 2) * 3))
#define TIMERDELAY2		(500 - (GetProfileInt ("Windows", "KeyboardSpeed", 31) * 15))

// =========================================================
// TSPINNER
// spinner control
// =========================================================
class TSpinner : public TControl
{
	TSpinEdit*	edit;
	bool		bDrawState[2];
	bool		bEnabled[2];
	RANGEVALUE	vIncrement;
	
	int			nCapture;
	UINT		uiTimer;

	void 		DrawArrow (TDC& dc, bool bTop, bool bDown = FALSE);
	TRect 		GetArrowRect (bool bTop);
	int			GetArrowAtPoint (TPoint& pt);

public:
	TSpinner (	TWindow*        parent,
				TSpinEdit*      edit,
				const char far* title,
				int x, int y, int w, int h,
				TModule*        module = 0);

	LPSTR		GetClassName ();
	void		GetWindowClass (WNDCLASS& wc);

	void		SetupWindow ();
	void 		Paint (TDC& dc, bool, TRect&);

	void		EvLButtonDown (UINT, TPoint& pt);
	void		EvLButtonUp (UINT, TPoint& pt);
	void		EvMouseMove (UINT, TPoint& pt);
	void 		EvRButtonDown (UINT, TPoint&);
	void		EvTimer (UINT uiTimerId);

	void		EnableButton (bool bTop, bool bEnable = TRUE);

	DECLARE_RESPONSE_TABLE (TSpinner);
};

DEFINE_RESPONSE_TABLE1 (TSpinner, TControl)
	EV_WM_LBUTTONDOWN,
	EV_WM_LBUTTONUP,
	EV_WM_RBUTTONDOWN,
	EV_WM_MOUSEMOVE,
	EV_WM_TIMER,
END_RESPONSE_TABLE;

// ---------------------------------------------------------
// CONSTRUCTOR
//
// initialise the spin-control
// ---------------------------------------------------------
TSpinner::TSpinner(TWindow*        parent,
				   TSpinEdit*      _edit,
				   const char far* title,
				   int x, int y, int w, int h,
				   TModule*        module)
	: TControl (parent, 32679, title, x, y, w, h, module)
{
	Attr.Style &= ~WS_TABSTOP;

	edit 		= _edit;

	bEnabled[SPINTOP]		= TRUE;
	bEnabled[SPINBOTTOM]	= TRUE;

	nCapture	= -1;
	uiTimer 	= NULL;
}

// ---------------------------------------------------------
// GETCLASSNAME
//
// return unique name for this window class
// ---------------------------------------------------------
LPSTR TSpinner::GetClassName ()
{
	return "ss.TSpinner";
}

// ---------------------------------------------------------
// GETWINDOWCLASS
//
// define the window class
// ---------------------------------------------------------
void TSpinner::GetWindowClass (WNDCLASS& wc)
{
	TControl::GetWindowClass (wc);

	wc.style &= ~CS_DBLCLKS;
}

// ---------------------------------------------------------
// SETUPWINDOW
//
// reposition the spin control and resize the edit control
// ---------------------------------------------------------
void TSpinner::SetupWindow ()
{
	TControl::SetupWindow ();

	TRect	rc = edit->GetClientRect ();
	TRect	rcThis = rc;

	rcThis.left	= rcThis.right - WIDTH;
	rc.right = rcThis.left - 0;

	edit->MapWindowPoints (Parent->HWindow, rcThis, 2);
	rcThis = rcThis.InflatedBy (0, 1);

	SetWindowPos (edit->HWindow, rcThis, SWP_NOACTIVATE);
	edit->SetWindowPos (NULL, 0, 0, rc.right, rc.bottom, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

// ---------------------------------------------------------
// ENABLEBUTTON
//
// enable/disable a spin button
// ---------------------------------------------------------
void TSpinner::EnableButton (bool bTop, bool bEnable)
{
	if (bEnable != bEnabled[bTop])
	{
		bEnabled[bTop] = bEnable;

		if (HWindow)
		{
			TClientDC	dc (HWindow);

			DrawArrow (dc, bTop, FALSE);
		}
	}
}

// ---------------------------------------------------------
// PAINT
//
// paint the spin-control
// ---------------------------------------------------------
void TSpinner::Paint (TDC& dc, bool, TRect&)
{
	{
		TPen	penDkGrey (GetSysColor (COLOR_BTNSHADOW));
		TPen	penLtGrey (GetSysColor (COLOR_BTNFACE));
		TPen	penWhite (GetSysColor (COLOR_BTNHIGHLIGHT));
		TPen	penBlack (GetSysColor (COLOR_WINDOWFRAME));
		TRect	client (GetClientRect ());

		dc.SelectObject (penDkGrey);
		dc.MoveTo (client.left, client.bottom - 2);
		dc.LineTo (client.left, client.top);
		dc.LineTo (client.right - 1, client.top);

		dc.SelectObject (penWhite);
		dc.LineTo (client.right - 1, client.bottom - 1);
		dc.LineTo (client.left - 1, client.bottom - 1);

	#if (MARGIN == 2)
		client = client.InflatedBy (-1, -1);

		dc.SelectObject (penBlack);
		dc.MoveTo (client.left, client.bottom - 2);
		dc.LineTo (client.left, client.top);
		dc.LineTo (client.right - 1, client.top);

		dc.SelectObject (penLtGrey);
		dc.LineTo (client.right - 1, client.bottom - 1);
		dc.LineTo (client.left - 1, client.bottom - 1);	
	#endif

		dc.SelectStockObject (BLACK_PEN);
	}

	{
		DrawArrow (dc, TRUE);
		DrawArrow (dc, FALSE);
	}
}

// ---------------------------------------------------------
// DRAWARROW
//
// draw a single arrow (either the up or down arrow) button
// ---------------------------------------------------------
void TSpinner::DrawArrow (TDC& dc, bool bTop, bool bDown)
{
	TRect	client (GetArrowRect (bTop));
	TPoint	pt[3];
	TPoint	ptCentre (	client.left + (client.Width () >> 1),
						client.top + (client.Height () >> 1));

	bDrawState[bTop] = bDown;
							
	TPen	penDkGrey (GetSysColor (COLOR_BTNSHADOW));
	TPen	penWhite (GetSysColor (COLOR_BTNHIGHLIGHT));

	if (bDown == FALSE)
	{
		dc.SelectObject (penWhite);
		dc.MoveTo (client.left, client.bottom - 2);
		dc.LineTo (client.left, client.top);
		dc.LineTo (client.right - 1, client.top);

		dc.SelectObject (penDkGrey);
		dc.LineTo (client.right - 1, client.bottom - 1);
		dc.LineTo (client.left - 1, client.bottom - 1);

		client = client.InflatedBy (-1, -1);
	}
	else
	{
		dc.SelectObject (penDkGrey);
		dc.MoveTo (client.left, client.bottom - 1);
		dc.LineTo (client.left, client.top);
		dc.LineTo (client.right, client.top);

		client.left++;
		client.top++;
	}

	dc.FillRect (client, TBrush (GetSysColor (COLOR_BTNFACE)));

	if (bDown)
	{
		ptCentre = ptCentre.OffsetBy (1, 1);
	}

	if (bTop)
	{
		pt[0].x	= ptCentre.x;
		pt[0].y	= ptCentre.y - 2;

		pt[1].x	= ptCentre.x - 3;
		pt[1].y	= ptCentre.y + 1;

		pt[2].x	= ptCentre.x + 3;
		pt[2].y	= ptCentre.y + 1;
	}
	else
	{
		pt[0].x	= ptCentre.x;
		pt[0].y	= ptCentre.y + 2;

		pt[1].x	= ptCentre.x - 3;
		pt[1].y	= ptCentre.y - 1;

		pt[2].x	= ptCentre.x + 3;
		pt[2].y	= ptCentre.y - 1;
	}

	TPen	penBlack (GetSysColor (COLOR_BTNTEXT));

	if (bEnabled[bTop])
	{
		TBrush	brushBlack (GetSysColor (COLOR_BTNTEXT));

		dc.SelectObject (penBlack);
		dc.SelectObject (brushBlack);
		dc.Polygon (pt, 3);

		dc.SelectStockObject (BLACK_BRUSH);
	}
	else
	{
		if (bTop)
		{
			dc.SelectObject (penBlack);
			dc.MoveTo (pt[0].x, pt[0].y);
			dc.LineTo (pt[1].x, pt[1].y);

			dc.SelectObject (penWhite);
			dc.LineTo (pt[2].x + 1, pt[2].y);
		}
		else
		{
			dc.SelectObject (penWhite);
			dc.MoveTo (pt[0].x, pt[0].y);
			dc.LineTo (pt[2].x, pt[2].y);

			dc.SelectObject (penBlack);
			dc.LineTo (pt[1].x - 1, pt[1].y);
		}

		dc.SelectStockObject (BLACK_PEN);
	}

	dc.SelectStockObject (BLACK_PEN);
}

// ---------------------------------------------------------
// GETARROWRECT
//
// get the TRect area of a particular button
// ---------------------------------------------------------
TRect TSpinner::GetArrowRect (bool bTop)
{
	TRect	rc (GetClientRect ().InflatedBy (-MARGIN, -MARGIN));
	int		y = MARGIN + (rc.Height () >> 1);

	if (bTop)
	{
		rc.bottom = y;
	}
	else
	{
		rc.top = y;
	}

	return rc;
}

// ---------------------------------------------------------
// GETARROWATPOINT
//
// get the arrow at a particular location
// ---------------------------------------------------------
int TSpinner::GetArrowAtPoint (TPoint& pt)
{
	if (GetArrowRect (SPINTOP).Contains (pt) && bEnabled[SPINTOP])
	{
		return SPINTOP;
	}

	if (GetArrowRect (SPINBOTTOM).Contains (pt) && bEnabled[SPINBOTTOM])
	{
		return SPINBOTTOM;
	}

	return -1;
}

// ---------------------------------------------------------
// EVLBUTTONDOWN
//
// detect a mouse click on the spin button
// ---------------------------------------------------------
void TSpinner::EvLButtonDown (UINT uiModKeys, TPoint& pt)
{
	int	nArrow = GetArrowAtPoint (pt);

	if (nArrow != -1)
	{
		if (uiModKeys & MK_CONTROL)
		{
			if (uiModKeys & MK_SHIFT)
			{
				// will move to limit of range
				vIncrement = edit->Spinner.GetHigh ();
			}
			else
			{
				// use a larger increment
				vIncrement = edit->Spinner.GetLargeIncrement ();
			}
		}
		else
		{
			// use a normal increment
			vIncrement = edit->GetIncrement ();
		}

		SetCapture ();

		nCapture = nArrow;

		TClientDC	dc (HWindow);

		DrawArrow (dc, nCapture, TRUE);

		Parent->SendMessage (WM_NEXTDLGCTL, (WPARAM) edit->HWindow, TRUE);

		uiTimer		= SetTimer (idTIMER1, TIMERDELAY1);
	}
}

// ---------------------------------------------------------
// EVLBUTTONUP
//
// detect a mouse button release on the spin button
// ---------------------------------------------------------
void TSpinner::EvLButtonUp (UINT, TPoint& pt)
{
	if (nCapture != -1)
	{
		{
			TClientDC	dc (HWindow);

			DrawArrow (dc, nCapture, FALSE);	// draw it in an UP state
		}

		if (GetArrowAtPoint (pt) == nCapture)
		{
			if (nCapture == SPINTOP)
			{
				edit->SpinUp (vIncrement);
			}
			else
			{
				edit->SpinDown (vIncrement);
			}
		}

		ReleaseCapture ();
		nCapture = -1;
		KillTimer (uiTimer);
		uiTimer = NULL;
	}
}

// ---------------------------------------------------------
// EVMOUSEMOVE
//
// detect a mouse movement over spin button
// ---------------------------------------------------------
void TSpinner::EvMouseMove (UINT, TPoint& pt)
{
	if (nCapture != -1)
	{
		if (bDrawState[nCapture] && GetArrowAtPoint (pt) != nCapture)	// if drawn down, but not over
		{
			TClientDC	dc (HWindow);

			DrawArrow (dc, nCapture, FALSE);	// draw it in an UP state
		}

		if (!bDrawState[nCapture] && GetArrowAtPoint (pt) == nCapture)	// if drawn UP, but over again
		{
			TClientDC	dc (HWindow);

			DrawArrow (dc, nCapture, TRUE);		// draw it in a DOWN state
		}
	}
}

// ---------------------------------------------------------
// EVRBUTTONDOWN
//
// detect a RIGHT mouse click on the spin button
// ---------------------------------------------------------
void TSpinner::EvRButtonDown (UINT, TPoint&)
{
	// forward it to the edit control
	edit->ForwardMessage ();
}

// ---------------------------------------------------------
// EVTIMER
//
// autorepeat timer when spinbutton is pressed
// ---------------------------------------------------------
void TSpinner::EvTimer (UINT uiTimerId)
{
	if (uiTimerId == idTIMER1)
	{
		KillTimer (uiTimer);
		uiTimer = SetTimer (idTIMER2, TIMERDELAY2);
	}
	else
	{
		TPoint	pt;

		GetCursorPos (pt);
		ScreenToClient (pt);
		
		if (GetArrowAtPoint (pt) == nCapture)
		{
			switch (nCapture)
			{
				case SPINTOP :
				{
					edit->SpinUp (vIncrement);
					break;
				}
				case SPINBOTTOM :
				{
					edit->SpinDown (vIncrement);
					break;
				}
			}
		}
	}
}

// =========================================================
// TSPINEDIT
// edit control attached to a spinner
// =========================================================
DEFINE_RESPONSE_TABLE1 (TSpinEdit, TEdit)
	EV_WM_CHAR,
	EV_WM_KEYDOWN,
	EV_WM_KEYUP,
	EV_WM_KILLFOCUS,
	EV_WM_RBUTTONDOWN,
	EV_WM_ENABLE,
END_RESPONSE_TABLE;

TSpinEdit::TSpinEdit (	TWindow*        parent,
						int             id,
						const char far* text,
						int x, int y, int w, int h,
						UINT            textLen,
						TModule*        module)
	: TEdit (parent, id, text, x, y, w, h, textLen, FALSE, module)
{
	Init ();
}

TSpinEdit::TSpinEdit (	TWindow*   		parent,
						int        		resourceId,
						UINT       		textLen,
						TModule*   		module)
	: TEdit (parent, resourceId, textLen, module)
{
	Init ();
}

void TSpinEdit::Init ()
{
	spinner = new TSpinner (Parent, this, "", 0, 0, 0, 0, GetModule ());

}

// ---------------------------------------------------------
// SPINUP
//
// called when the up spinbutton is pressed
// ---------------------------------------------------------
void TSpinEdit::SpinUp (RANGEVALUE)
{
	MessageBeep (MB_ICONEXCLAMATION);
}

// ---------------------------------------------------------
// SPINDOWN
//
// called when the down spinbutton is pressed
// ---------------------------------------------------------
void TSpinEdit::SpinDown (RANGEVALUE)
{
	MessageBeep (MB_ICONEXCLAMATION);
}

// ---------------------------------------------------------
// ENABLEBUTTON
//
// enable/disable one of the spinbuttons
// ---------------------------------------------------------
void TSpinEdit::EnableButton (bool bTop, bool bEnable)
{
	spinner->EnableButton (bTop, bEnable);
}


// ---------------------------------------------------------
// EVCHAR
//
// WM_CHAR handler called when a key is pressed.
//
// used to assess whether the spin buttons should be enabled
// when a value is being typed in
// ---------------------------------------------------------
void TSpinEdit::EvChar (UINT key, UINT repeatCount, UINT flags)
{
	TEdit::EvChar (key, repeatCount, flags);

	AssessEnable ();
}

// ---------------------------------------------------------
// EVKEYDOWN
//
// WM_KEYDOWN handler called when a key is pressed.
//
// used to assess whether the spin buttons should be enabled
// when a value is being typed in
// ---------------------------------------------------------
void TSpinEdit::EvKeyDown (UINT key, UINT repeatCount, UINT flags)
{
	RANGEVALUE vIncrement;

	if (key == VK_UP || key == VK_DOWN)
	{
		int	stateControl 	= GetKeyState (VK_CONTROL);
		int stateShift		= GetKeyState (VK_SHIFT);

		if (stateControl < 0)
		{
			if (stateShift < 0)
			{
				// will move to limit of range
				vIncrement = Spinner.GetHigh ();
			}
			else
			{
				// use a larger increment
				vIncrement = Spinner.GetLargeIncrement ();
			}
		}
		else
		{
			// use a normal increment
			vIncrement = GetIncrement ();
		}
	}

	switch (key)
	{
		case VK_UP :
		{
			SpinUp (vIncrement);
			break;
		}
		case VK_DOWN :
		{
			SpinDown (vIncrement);
			break;
		}
		default :
		{
			TEdit::EvKeyDown (key, repeatCount, flags);

			AssessEnable ();
			break;
		}
	}
}

// ---------------------------------------------------------
// EVKEYUP
//
// WM_KEYUP handler called when a key is released.
//
// used to assess whether the spin buttons should be enabled
// when a value is being typed in
// ---------------------------------------------------------
void TSpinEdit::EvKeyUp (UINT key, UINT repeatCount, UINT flags)
{
	TEdit::EvKeyUp (key, repeatCount, flags);

	AssessEnable ();
}

// ---------------------------------------------------------
// EVKILLFOCUS
//
// WM_KILLFOCUS handler called when the controls loses focus
//
// uses to give a change for a derived class to adjust the
// value
// ---------------------------------------------------------
void TSpinEdit::EvKillFocus (HWND hwndGetFocus)
{
	if (Spinner.GetMatchOnLeaving ())
	{
		WhenLeaving ();
	}

	TEdit::EvKillFocus (hwndGetFocus);
}

// ---------------------------------------------------------
// EVRBUTTONDOWN
//
// detect a RIGHT mouse click on the spin button
// ---------------------------------------------------------
void TSpinEdit::EvRButtonDown (UINT, TPoint&)
{
	if (Spinner.GetShowMenu ())
	{
		ShowPopup ();
	}
}

// ---------------------------------------------------------
// EVRBUTTONDOWN
//
// detect a RIGHT mouse click on the spin button
// ---------------------------------------------------------
void TSpinEdit::EvEnable (bool bEnable)
{
	spinner->EnableWindow (bEnable);
	AssessEnable ();

	DefaultProcessing ();
}

// =========================================================
// TRANGESPINEDIT
// spin-edit which works between two numeric values
// =========================================================
TRangeSpinEdit::TRangeSpinEdit (TWindow*        parent,
								int             id,
								const char far* text,
								int x, int y, int w, int h,
								UINT            textLen,
								TModule*        module)
	: TSpinEdit (parent, id, text, x, y, w, h, textLen, module)
{
}

TRangeSpinEdit::TRangeSpinEdit (TWindow*   		parent,
								int        		resourceId,
								UINT            textLen,
								TModule*   		module)
	: TSpinEdit (parent, resourceId, textLen, module)
{
}

// ---------------------------------------------------------
// SETUPWINDOW
//
// set the initial value of the spin edit control
// ---------------------------------------------------------
void TRangeSpinEdit::SetupWindow ()
{
	TSpinEdit::SetupWindow ();

	SetValue (Spinner.GetInitial (), FALSE, FALSE);
}

// ---------------------------------------------------------
// SPINUP
//
// determine the next setting when the Up button is pressed
// ---------------------------------------------------------
void TRangeSpinEdit::SpinUp (RANGEVALUE increment)
{
	RANGEVALUE	nValue = GetCurrentValue ();

	if (nValue < Spinner.GetHigh ())
	{
		nValue += increment;

		SetValue (nValue < Spinner.GetHigh () ? nValue : Spinner.GetHigh ());
	}
	else
	{
		AssessEnable ();
	}
}

// ----------------------------------------------------------
// SPINDOWN
//
// determine the next setting when the Down button is pressed
// ----------------------------------------------------------
void TRangeSpinEdit::SpinDown (RANGEVALUE increment)
{
	RANGEVALUE	nValue = GetCurrentValue ();

	if (nValue > Spinner.GetLow ())
	{
		nValue -= increment;

		SetValue (nValue > Spinner.GetLow () ? nValue : Spinner.GetLow ());
	}
	else
	{
		AssessEnable ();
	}
}

// ---------------------------------------------------------
// GETCURRENTVALUE
//
// get the current value of the edit control
// ---------------------------------------------------------
RANGEVALUE TRangeSpinEdit::GetCurrentValue (bool* pValid)
{
	char		szText[10];
	RANGEVALUE	v;

	GetText (szText, sizeof (szText));
	v = atof (szText);

	if (pValid)
	{
		*pValid = (v >= Spinner.GetLow () && v <= Spinner.GetHigh ());
	}

	return v;
}

// ---------------------------------------------------------
// SETVALUE
//
// set the value of the edit control
// ---------------------------------------------------------
void TRangeSpinEdit::SetValue (RANGEVALUE nValue, bool bSelect, bool bNotify)
{
	SetText (GetValueText (nValue).c_str ());

	if (bSelect)
	{
		SetSelection (0, GetValueText (nValue).length ());
	}
	
	AssessEnable ();

	if (bNotify)
	{
	#ifdef __WIN32__
		Parent->HandleMessage (WM_COMMAND, MAKEWPARAM (Attr.Id, EN_CHANGE), (LPARAM) HWindow);
	#else
		Parent->HandleMessage (WM_COMMAND, (WPARAM) Attr.Id, MAKELPARAM (HWindow, EN_CHANGE));
	#endif
	}
}

// ---------------------------------------------------------
// GETVALUETEXT
//
// get the formatted string for a particular value
// ---------------------------------------------------------
string TRangeSpinEdit::GetValueText (RANGEVALUE nValue)
{
	char	szText[30];
	string	s = GetFormat ();

	if (s[1] == '.')
	{
		sprintf (szText, s.c_str (), nValue);
	}
	else
	{
		sprintf (szText, s.c_str (), (long) nValue);
	}

	s = szText;

	return s;
}

// ---------------------------------------------------------
// ASSESSENABLE
//
// enable/disable the spin buttons
// ---------------------------------------------------------
void TRangeSpinEdit::AssessEnable ()
{
	RANGEVALUE	nValue 		= GetCurrentValue ();
	bool		bIsEnabled 	= IsWindowEnabled ();

	Spinner.SetInitial (nValue);

	EnableButton (SPINTOP, bIsEnabled && nValue < Spinner.GetHigh ());
	EnableButton (SPINBOTTOM, bIsEnabled && nValue > Spinner.GetLow ());
}

// ---------------------------------------------------------
// GETFORMAT
//
//
// ---------------------------------------------------------
string TSpinEdit::GetFormat ()
{
	string	s ("%");
	LPCSTR	lpszFmt = Spinner.GetFormat ();

	if (*lpszFmt == '.')
	{
		char	szTemp[2];

		szTemp[1] = '\0';

		s += ".";
		lpszFmt++;

		while (isdigit (*lpszFmt))
		{
			char	szTemp[2];

			szTemp[0] = *(lpszFmt++);;
			szTemp[1] = '\0';
			s += szTemp;
		}

		s += "f";
	}
	else
	{
		s += "ld";
	}

	s += lpszFmt;

	return s;
}

// ---------------------------------------------------------
// WHENLEAVING
//
// when losing focus, reset value if out of range
// ---------------------------------------------------------
void TRangeSpinEdit::WhenLeaving ()
{
	RANGEVALUE	nValue = GetCurrentValue ();

	if (nValue < Spinner.GetLow ())
	{
		nValue = Spinner.GetLow ();
	}

	if (nValue > Spinner.GetHigh ())
	{
		nValue = Spinner.GetHigh ();
	}

	SetValue (nValue);
}

// ---------------------------------------------------------
// SHOWPOPUP
//
//
// ---------------------------------------------------------
void TRangeSpinEdit::ShowPopup ()
{
	TPopupMenu	popup;
	TPoint		ptCursor;
	RANGEVALUE	cur = GetCurrentValue ();
	RANGEVALUE	inc = GetIncrement ();
	RANGEVALUE	inc2 = inc / 2;
	RANGEVALUE	v = Spinner.GetLow ();
	int			nRows = GetSystemMetrics (SM_CYSCREEN) / GetSystemMetrics (SM_CYCAPTION);
	UINT		uiFlags;

	for (int n = 0; v < (Spinner.GetHigh () + inc); n++)
	{
		uiFlags = (v > (cur - inc2) && v < (cur + inc2)) ? MF_CHECKED : MF_UNCHECKED;

		if (n && !(n % nRows))
		{
			uiFlags |= MF_MENUBARBREAK;
		}

		popup.AppendMenu (MF_STRING | uiFlags, 10000 + n, GetValueText (v).c_str ());

		v += inc;
	}

	GetCursorPos (ptCursor);

	popup.TrackPopupMenu (TPM_CENTERALIGN | TPM_RIGHTBUTTON, ptCursor, NULL, HWindow);
}

// ---------------------------------------------------------
// EVCOMMAND
//
// handle WM_COMMAND messages from popup menu
// ---------------------------------------------------------
LRESULT TRangeSpinEdit::EvCommand (UINT id, HWND hWndCtl, UINT notifyCode)
{
	if (id >= 10000)
	{
		int 		n = id - 10000;
		RANGEVALUE	v = Spinner.GetLow () + (GetIncrement () * n);

		if (v >= Spinner.GetLow () && v <= Spinner.GetHigh ())
		{
			SetValue (v);

			return TRUE;
		}
	}

	return TSpinEdit::EvCommand (id, hWndCtl, notifyCode);
}

uint TRangeSpinEdit::Transfer(void* buffer, TTransferDirection direction)
{
   TSpinEdit::Transfer(buffer,direction);

   if (direction == tdSetData)
      {
      SetValue(*(RANGEVALUE *)buffer);
      }
   else if (direction == tdGetData)
      {
      *(RANGEVALUE *)buffer = GetCurrentValue();
      }

   return sizeof(RANGEVALUE);
}

// =========================================================
// TLISTSPINEDIT
// spin-edit which works from an array of string values
// =========================================================
TListSpinEdit::TListSpinEdit (	TWindow*        parent,
								int             id,
								const char far* text,
								int x, int y, int w, int h,
								UINT            textLen,
								TModule*        module)
	: 	TSpinEdit (parent, id, text, x, y, w, h, textLen, module),
		arrayString (10, 0, 10)
{
	Spinner.SetShowMenu (TRUE);
}

TListSpinEdit::TListSpinEdit (	TWindow*   		parent,
								int        		resourceId,
								UINT       		textLen,
								TModule*   		module)
	: 	TSpinEdit (parent, resourceId, textLen, module),
		arrayString (10, 0, 10)
{
	Spinner.SetShowMenu (TRUE);
}

// ---------------------------------------------------------
// SETUPWINDOW
//
// set the initial value of the spin edit control
// ---------------------------------------------------------
void TListSpinEdit::SetupWindow ()
{
	TSpinEdit::SetupWindow ();

	SetValue (Spinner.GetInitial (), FALSE, FALSE);
}

// ---------------------------------------------------------
// ADDSTRING
//
// add a string to the list of possible values
// ---------------------------------------------------------
void TListSpinEdit::AddString (LPCSTR lpszText)
{
	arrayString.Add (lpszText);
}

// ---------------------------------------------------------
// SPINUP
//
// determine the next setting when the Up button is pressed
// ---------------------------------------------------------
void TListSpinEdit::SpinUp (RANGEVALUE increment)
{
	int nValue = GetCurrentValue ();

	if (nValue)
	{
		nValue -= increment;

		SetValue (nValue > 0 ? nValue : 0);
	}
	else
	{
		AssessEnable ();
	}
}

// ----------------------------------------------------------
// SPINDOWN
//
// determine the next setting when the Down button is pressed
// ----------------------------------------------------------
void TListSpinEdit::SpinDown (RANGEVALUE increment)
{
	int	nValue = GetCurrentValue ();
	int nMax = arrayString.GetItemsInContainer () - 1;

	if (nValue < nMax)
	{
		nValue += increment;

		SetValue (nValue < nMax ? nValue : nMax);
	}
	else
	{
		AssessEnable ();
	}
}

// ---------------------------------------------------------
// GETCURRENTVALUE
//
// get the current value of the edit control.
//
// the return value is the index to the string in the array
// of strings, or -1 if no match is found
// ---------------------------------------------------------
int TListSpinEdit::GetCurrentValue (bool *pValid)
{
	char	szText[100];

	GetText (szText, sizeof (szText));
	AnsiUpper (szText);

	if (pValid)
	{
		*pValid = FALSE;
	}
   int n;
	for (n = 0; n < arrayString.GetItemsInContainer (); n++)
	{
		string	sTemp = arrayString[n];

		sTemp.to_upper ();

		if (sTemp == szText)
		{
			if (pValid)
			{
				*pValid = TRUE;
			}
			break;
		}
	}

	return n;
}

// ---------------------------------------------------------
// SETVALUE
//
// set the value of the edit control
//
// the nValue is the index to the string in the array
// ---------------------------------------------------------
void TListSpinEdit::SetValue (int nValue, bool bSelect, bool bNotify)
{
	if (nValue >= 0 && nValue < arrayString.GetItemsInContainer ())
	{
		SetText (arrayString[nValue].c_str ());

		if (bSelect)
		{
			SetSelection (0, arrayString[nValue].length ());
		}

		if (bNotify)
		{
		#ifdef __WIN32__
			Parent->HandleMessage (WM_COMMAND, MAKEWPARAM (Attr.Id, EN_CHANGE), (LPARAM) HWindow);
		#else
			Parent->HandleMessage (WM_COMMAND, (WPARAM) Attr.Id, MAKELPARAM (HWindow, EN_CHANGE));
		#endif
		}
	}

	AssessEnable ();
}

// ---------------------------------------------------------
// ASSESSENABLE
//
// enable/disable the spin buttons
// ---------------------------------------------------------
void TListSpinEdit::AssessEnable ()
{
	bool	bValid;
	int		nValue 		= GetCurrentValue (&bValid);
	bool	bIsEnabled 	= IsWindowEnabled ();

	Spinner.SetInitial (bValid ? nValue : -1);

	EnableButton (SPINTOP, bIsEnabled && bValid && nValue > 0);
	EnableButton (SPINBOTTOM, bIsEnabled && bValid && ++nValue < arrayString.GetItemsInContainer ());
}

// ---------------------------------------------------------
// WHENLEAVING
//
// when losing focus, reset value if out of range
// ---------------------------------------------------------
void TListSpinEdit::WhenLeaving ()
{
	int	nValue = GetCurrentValue ();

	if (nValue >= arrayString.GetItemsInContainer ())	// no exact match, so try to match what we have
	{
		char	szText[100];
		int		nLength;

		nLength = GetText (szText, sizeof (szText));

		if (nLength)
		{
			AnsiUpper (szText);

			for (int n = 0; n < arrayString.GetItemsInContainer (); n++)
			{
				string	sTemp = arrayString[n];
				sTemp.to_upper ();

				if (strncmp (szText, sTemp.c_str (), nLength) == 0)
				{
				//	SetText (arrayString[n].c_str ());
					SetValue (n);
					break;
				}
			}
		}
	}
}

// ---------------------------------------------------------
// SHOWPOPUP
//
// display popup menu on right mouse click
// ---------------------------------------------------------
void TListSpinEdit::ShowPopup ()
{
	TPopupMenu	popup;
	TPoint		ptCursor;
	int			nRows = GetSystemMetrics (SM_CYSCREEN) / GetSystemMetrics (SM_CYCAPTION);
	int			cur = GetCurrentValue ();
	UINT		uiFlags;

	for (int n = 0; n < arrayString.GetItemsInContainer (); n++)
	{
		uiFlags = (n == cur) ? MF_CHECKED : MF_UNCHECKED;

		if (n && !(n % nRows))
		{
			uiFlags |= MF_MENUBARBREAK;
		}

		popup.AppendMenu (MF_STRING | uiFlags, 10000 + n, arrayString[n].c_str ());
	}

	GetCursorPos (ptCursor);
	popup.TrackPopupMenu (TPM_CENTERALIGN | TPM_RIGHTBUTTON, ptCursor, NULL, HWindow);
}

// ---------------------------------------------------------
// EVCOMMAND
//
// handle WM_COMMAND messages from popup menu
// ---------------------------------------------------------
LRESULT TListSpinEdit::EvCommand (UINT id, HWND hWndCtl, UINT notifyCode)
{
	if (id >= 10000)
	{
		int n = id - 10000;

		if (n < arrayString.GetItemsInContainer ())
		{
			SetValue (n);

			return TRUE;
		}
	}

	return TSpinEdit::EvCommand (id, hWndCtl, notifyCode);
}

// =========================================================
// TSPINATTR
// data holder for the spinner
// =========================================================
TSpinAttr::TSpinAttr ()
{
	SetFormat (NULL);
	SetRange (0, 100);
	SetMatchOnLeaving (TRUE);
	SetShowMenu (FALSE);
}

// ---------------------------------------------------------
// SETFORMAT
//
// define a new format string.  Defines the number of
// decimal places that get displayed, and the default
// increment
// ---------------------------------------------------------
void TSpinAttr::SetFormat (LPCSTR _lpszFormat)
{
	lpszFormat 	= _lpszFormat;
	decimals	= 0;

	if (lpszFormat == NULL)
	{
		increment	= 1;
		lpszFormat	= "";
	}
	else
	{
		if (*lpszFormat == '.')
		{
		#if defined (__TINY__) || defined (__SMALL__) || defined (__MEDIUM__)
			decimals = 0;

			for (int n = 1; isdigit (lpszFormat[n]); n++)
			{
				decimals *= 10;
				decimals += lpszFormat[n] - '0';
			}
		#else
			decimals = atoi (lpszFormat + 1);
		#endif
		}

		RANGEVALUE v = pow10 (decimals);

		increment = 1.00 / v;
	}

	increment2 = increment * 5.0;
}

// ---------------------------------------------------------
// SETRANGE
//
// set the range and default initial value of the spinner
// ---------------------------------------------------------
void TSpinAttr::SetRange (RANGEVALUE l, RANGEVALUE h)
{
	low 	= l;
	high	= h;
	initial	= l;
}


