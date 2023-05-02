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

#include <owl\owlpch.h>
#pragma hdrstop

#include "fontdlg.h"
#include "fontdata.h"
#include "fontdlg.rh"

#define CF_NOTTFONTS	0x10000000L

struct ENUMSETUP
{
	int			nMode;
	void		*ptr;
	DWORD		dwFlags;

	// for ENUM_STYLEMATCH
	char		szStyle[LF_FACESIZE];
	LOGFONT		*lplf;

	// built up by ENUM_FONTSTYLES
	TIntArray	*parrayBold;
	TIntArray	*parrayItalic;
};

struct ENUMSIZE
{
	TComboBox	*pComboBox;
	int			nMinSize;
	int			nMaxSize;
};

#define ENUM_FONTNAMES		1
#define ENUM_FONTSTYLES		2
#define ENUM_STYLEMATCH		3

void			FillListFromResource (ENUMSIZE& enmSize, int nID);

#ifdef __WIN32__
  int CALLBACK 	EnumFontFamProc (const LOGFONT FAR* elf, const TEXTMETRIC FAR*, DWORD nType, LPARAM lpData);
  int CALLBACK 	EnumFontSizeProc (const LOGFONT FAR*, const TEXTMETRIC FAR* lptm, DWORD, LPARAM lpData);
#else
  int CALLBACK 	EnumFontFamProc (const ENUMLOGFONT FAR* elf, const NEWTEXTMETRIC FAR*, int nType, LPARAM lpData);
  int CALLBACK 	EnumFontSizeProc (const LOGFONT FAR*, const TEXTMETRIC FAR* lptm, int, LPARAM lpData);
#endif


// =========================================================
// TFontPropertyPage
//
// defines a property page for selecting fonts
// =========================================================
DEFINE_RESPONSE_TABLE1 (TFontPropertyPage, TPropertyPage)
	EV_CHILD_NOTIFY (IDC_FONTNAME, CBN_SELCHANGE, EvFontName),
	EV_CHILD_NOTIFY (IDC_FONTNAME, CBN_EDITCHANGE, EvFontName),
	EV_CHILD_NOTIFY (IDC_FONTSTYLE, CBN_SELCHANGE, EvFontStyle),
	EV_CHILD_NOTIFY (IDC_FONTSTYLE, CBN_EDITCHANGE, EvFontStyle),
	EV_CHILD_NOTIFY (IDC_FONTSIZE, CBN_SELCHANGE, EvFontSize),
	EV_CHILD_NOTIFY (IDC_FONTSIZE, CBN_EDITCHANGE, EvFontSize),
	EV_CHILD_NOTIFY (IDC_COLOR, CBN_SELCHANGE, EvColor),
	EV_CHILD_NOTIFY (IDC_STRIKETHRU, BN_CLICKED, EvAfterChange),
	EV_CHILD_NOTIFY (IDC_UNDERLINE, BN_CLICKED, EvAfterChange),
	EV_MESSAGE (WM_USER, AfterSelection),
	EV_WM_MEASUREITEM,
END_RESPONSE_TABLE;

TFontPropertyPage::TFontPropertyPage (TResId resID, TFontData& fd, TModule *module)
	: 	TPropertyPage (resID, module),
		fontdata (fd),
		fontlist (20, 0, 20),
		arrayBold (4, 0, 4),
		arrayItalic (4, 0, 4)
{
	pcbFontName		= new TFontNameList (this, IDC_FONTNAME, fontlist, module);
	pcbFontStyle	= new TComboBox (this, IDC_FONTSTYLE, LF_FACESIZE, module);
	pcbFontSize		= new TComboBox (this, IDC_FONTSIZE, 4, module);
	pcbColor	 	= new TColorList (this, IDC_COLOR, module);
	peditSample		= new TSampleStatic (this, IDC_SAMPLE, module);

	ptFontSample	= NULL;

	ENUMSETUP	enmSetup;

	enmSetup.nMode		= ENUM_FONTNAMES;
	enmSetup.ptr		= &fontlist;
	enmSetup.dwFlags	= fontdata.Flags;
	nFontNameIndex		= -1;
	nFontPointSize10	= 0;
	bUpdatePending		= FALSE;

	if ((fontdata.Flags & CF_SCREENFONTS) || !(fontdata.Flags & CF_BOTH))
	{
		TScreenDC	dc;

		dc.SetMapMode (MM_TWIPS);

		if (fontdata.Flags & CF_BOTH)
		{
			enmSetup.dwFlags |= CF_NOTTFONTS;
		}

		dc.EnumFontFamilies (NULL, EnumFontFamProc, &enmSetup);
	}

	if ((fontdata.Flags & CF_PRINTERFONTS) && fontdata.DC)
	{
		TDC 		dc (fontdata.DC);

		dc.SetMapMode (MM_TWIPS);

		enmSetup.dwFlags	= fontdata.Flags;
		dc.EnumFontFamilies (NULL, EnumFontFamProc, &enmSetup);
	}
}

TFontPropertyPage::~TFontPropertyPage ()
{
	if (ptFontSample)
	{
		delete ptFontSample;
	}

	fontlist.Flush ();
}

void TFontPropertyPage::SetupWindow ()
{
	TPropertyPage::SetupWindow ();

	if (!(fontdata.Flags & CF_EFFECTS))
	{
		::ShowWindow 	(GetDlgItem (IDC_EFFECTS), SW_HIDE);
		::EnableWindow 	(GetDlgItem (IDC_EFFECTS), FALSE);
		::ShowWindow 	(GetDlgItem (IDC_STRIKETHRU), SW_HIDE);
		::EnableWindow  (GetDlgItem (IDC_STRIKETHRU), FALSE);
		::ShowWindow 	(GetDlgItem (IDC_UNDERLINE), SW_HIDE);
		::EnableWindow  (GetDlgItem (IDC_UNDERLINE), FALSE);
		::ShowWindow 	(GetDlgItem (IDC_COLORTXT), SW_HIDE);
		::EnableWindow  (GetDlgItem (IDC_COLORTXT), FALSE);
		::ShowWindow 	(GetDlgItem (IDC_COLOR), SW_HIDE);
		::EnableWindow 	(GetDlgItem (IDC_COLOR), FALSE);
	}

	if (fontdata.Flags & CF_NOSIMULATIONS)
	{
		::ShowWindow (GetDlgItem (IDC_SAMPLEBOX), SW_HIDE);
		::ShowWindow (GetDlgItem (IDC_SAMPLE), SW_HIDE);
		::ShowWindow (GetDlgItem (IDC_DESCRIPTION), SW_HIDE);
	}
	else
	{
		SendDlgItemMessage (IDC_DESCRIPTION, WM_SETFONT, (WPARAM) (HFONT) GetStockObject (ANSI_VAR_FONT), NULL);
	}

	for (int n = 0; n < fontlist.GetItemsInContainer (); n++)
	{
		pcbFontName->AddString (fontlist[n].GetName ());
	}

	if (fontdata.Flags & CF_INITTOLOGFONTSTRUCT)
	{
		if (!fontdata.LogFont.lfHeight)
		{
			fontdata.LogFont.lfHeight = fontdata.PointSize * 2;

			if (fontdata.LogFont.lfHeight > 0)
			{
				fontdata.LogFont.lfHeight *= -1;
			}
		}

		int		nIndex = pcbFontName->FindExactString (fontdata.LogFont.lfFaceName, -1);
		char	szTemp[LF_FACESIZE];

		if (nIndex != -1)
		{
			// set up name
			pcbFontName->SetSelIndex (nIndex);
			EvFontName ();

			// set up style
			ENUMSETUP	enmSetup;

			enmSetup.nMode		= ENUM_STYLEMATCH;
			enmSetup.ptr		= pcbFontStyle;
			enmSetup.lplf 		= &fontdata.LogFont;

			if (fontlist[nIndex].GetType () & TRUETYPE_FONTTYPE)
			{
				TScreenDC	dc;

				dc.SetMapMode (MM_TWIPS);
				dc.EnumFontFamilies (fontlist[nIndex].GetName (), EnumFontFamProc, &enmSetup);
			}
			else
			{
				int	nIndex = 0;

				if (fontdata.LogFont.lfWeight > FW_NORMAL)
				{
					nIndex += 2;
				}

				if (fontdata.LogFont.lfItalic)
				{
					nIndex++;
				}

				pcbFontStyle->SetSelIndex (nIndex);	// matched!
			}

			// set up size (lfHeight == point size)
			if (fontdata.LogFont.lfHeight)
			{
				int nSize = fontdata.LogFont.lfHeight / 20;

				if (nSize < 0)
				{
					nSize *= -1;
				}

				pcbFontSize->SetText (itoa (nSize, szTemp, 10));

				EvFontSize ();
			}

			// set up effects
			CheckDlgButton (IDC_STRIKETHRU, fontdata.LogFont.lfStrikeOut);
			CheckDlgButton (IDC_UNDERLINE, 	fontdata.LogFont.lfUnderline);
		}

		// search for colour match
		if (fontdata.Flags & CF_EFFECTS)
		{
			int	nColorCount = pcbColor->GetCount ();

			for (int n = 0; n < nColorCount; n++)
			{
				if (fontdata.Color == TColor ((COLORREF) pcbColor->GetItemData (n)))
				{
					pcbColor->SetSelIndex (n);	// matched!
					break;
				}
			}

			peditSample->SetTextColor (fontdata.Color);
		}
	}

	GetApplication ()->PumpWaitingMessages ();	
}

BOOL TFontPropertyPage::SaveData ()
{
	if (ptFontSample && nFontNameIndex != -1)
	{
		ptFontSample->GetObject (fontdata.LogFont);

		fontdata.PointSize  = nFontPointSize10;
		fontdata.Color		= (COLORREF) pcbColor->GetItemData (pcbColor->GetSelIndex ());
	}

	return TRUE;
}

// =========================================================
// EvFont...
//
// event handlers for font name, style and size lists
// =========================================================
void TFontPropertyPage::EvFontName ()
{
	int		nIndex = GetSelItem (pcbFontName);

	nFontNameIndex	= nIndex;

	if (nIndex == -1)
	{
		return;
	}

	if (fontdata.DC && fontlist[nIndex].GetType () & DEVICE_FONTTYPE)
	{
		TDC		dc (fontdata.DC);

		FontSelected (dc, nIndex);
	}
	else
	{
		TScreenDC	dc;

		FontSelected (dc, nIndex);
	}
}

void TFontPropertyPage::EvFontStyle ()
{
	int		nIndex = GetSelItem (pcbFontStyle);

	if (nIndex == -1)
	{
		return;
	}

	EvAfterChange ();
}

void TFontPropertyPage::EvFontSize ()
{
	GetSelItem (pcbFontSize);

	EvAfterChange ();
}

// =========================================================
// GetSelItem
//
// get the index of the selected item.  Selects a list item
// if it matches the entered text
// =========================================================
int TFontPropertyPage::GetSelItem (TComboBox *ptList)
{
	int		nIndex = ptList->GetSelIndex ();

	if (nIndex == -1)
	{
		char	szEditText[LF_FACESIZE];
		char	szLBText[LF_FACESIZE];

		ptList->GetText (szEditText, LF_FACESIZE);

		nIndex = ptList->FindExactString (szEditText, -1);

		if (nIndex == -1)
		{
			return -1;
		}

		ptList->GetString (szLBText, nIndex);

		if (lstrcmpi (szLBText, szEditText) != 0)
		{
			return -1;
		}

		ptList->SendMessage (CB_SETCURSEL, nIndex);
	}

	return nIndex;
}

// =========================================================
// FontSelected
//
// populates style and size lists after a font is selected
// =========================================================
void TFontPropertyPage::FontSelected (TDC& dc, int nIndex)
{
	char	szStyle[LF_FACESIZE];
	int		nStyleSelect;
	char	szSize[4];
	int		nSizeSelect;

	pcbFontStyle->GetText (szStyle, LF_FACESIZE);
	pcbFontSize->GetText (szSize, sizeof (szSize));

	pcbFontStyle->SetText ("");
	pcbFontStyle->ClearList ();
	pcbFontSize->SetText ("");
	pcbFontSize->ClearList ();

	arrayBold.Flush ();
	arrayItalic.Flush ();

	ENUMSETUP	enmSetup;

	enmSetup.nMode			= ENUM_FONTSTYLES;
	enmSetup.ptr			= pcbFontStyle;
	enmSetup.parrayBold		= &arrayBold;
	enmSetup.parrayItalic 	= &arrayItalic;

	int 	nMapMode = dc.SetMapMode (MM_TWIPS);

	dc.SetMapMode (MM_TWIPS);
	dc.EnumFontFamilies (fontlist[nIndex].GetName (), EnumFontFamProc, &enmSetup);

	ENUMSIZE	enmSize;

	enmSize.pComboBox = pcbFontSize;

	if (fontdata.Flags & CF_LIMITSIZE)
	{
		enmSize.nMinSize = fontdata.SizeMin;
		enmSize.nMaxSize = fontdata.SizeMax;
	}
	else
	{
		enmSize.nMinSize = 0;
		enmSize.nMaxSize = 255;
	}

	if ((fontlist[nIndex].GetType () & TRUETYPE_FONTTYPE) ||
		(fontlist[nIndex].GetFamily () & TMPF_VECTOR))
	{
		FillListFromResource (enmSize, IDS_FONTSIZES);
	}
	else
	{
		dc.EnumFonts (fontlist[nIndex].GetName (), EnumFontSizeProc, &enmSize);
	}

	dc.SetMapMode (nMapMode);

	if (pcbFontStyle->GetCount ())
	{
		nStyleSelect = pcbFontStyle->FindExactString (szStyle, -1);

		pcbFontStyle->SetSelIndex (nStyleSelect == -1 ? 0 : nStyleSelect);

		EvFontStyle ();
	}

	if (pcbFontSize->GetCount ())
	{
		nSizeSelect = pcbFontSize->FindExactString (szSize, -1);

		if (nSizeSelect == -1)
		{
			nSizeSelect = pcbFontSize->FindExactString ("10", -1);
		}

		pcbFontSize->SetSelIndex (nSizeSelect == -1 ? 0 : nSizeSelect);

		EvFontSize ();
	}

	char	szDescription[100];

	if (fontlist[nIndex].GetType () & TRUETYPE_FONTTYPE)
	{
		GetModule ()->LoadString (IDS_TRUETYPE, szDescription, sizeof (szDescription));
		SetDlgItemText (IDC_DESCRIPTION, szDescription);
	}
	else
	{
		if (fontlist[nIndex].GetType () & DEVICE_FONTTYPE)
		{
			GetModule ()->LoadString (IDS_DEVICE, szDescription, sizeof (szDescription));
			SetDlgItemText (IDC_DESCRIPTION, szDescription);
		}
		else
		{
			GetModule ()->LoadString (IDS_RASTER, szDescription, sizeof (szDescription));
			SetDlgItemText (IDC_DESCRIPTION, szDescription);
		}
	}
}

// =========================================================
// AfterSelection
//
// rebuilds the internal font representation used to display
// the "sample" window
// =========================================================
LRESULT TFontPropertyPage::AfterSelection (WPARAM, LPARAM)
{
	int		nFontName;
	int		nFontStyle;
	int		nFontSize;
	char	szText[LF_FACESIZE];

	bUpdatePending	= FALSE; // clear message-reentrancy flag
	
	nFontName 	= pcbFontName->GetSelIndex ();
	nFontStyle 	= pcbFontStyle->GetSelIndex ();

	pcbFontSize->GetText (szText, 4);
	nFontSize	= atoi (szText);

	if (nFontName == -1 || nFontStyle == -1 || nFontSize < 4 || nFontSize > 255)
	{
		return FALSE;	// no change
	}

	if (fontdata.Flags & CF_LIMITSIZE)
	{
		if (nFontSize < fontdata.SizeMin || nFontSize > fontdata.SizeMax)
		{
     		return FALSE; // invalid size
		}
	}

	// set up font
	LOGFONT		lf;

	memcpy (&lf, &fontlist[nFontName].GetLogFont (), sizeof (LOGFONT));

	// get bold/italic
	lf.lfItalic = arrayItalic[nFontStyle];
	lf.lfWeight	= arrayBold[nFontStyle];

	// get size
	nFontPointSize10 = 10 * nFontSize;

	lf.lfHeight	= 0 + (nFontSize * -20);	// calculate TWIPS height
	lf.lfWidth = 0;

	// get underline/strikeout
	lf.lfUnderline = (BYTE) IsDlgButtonChecked (IDC_UNDERLINE);
	lf.lfStrikeOut = (BYTE) IsDlgButtonChecked (IDC_STRIKETHRU);

	lstrcpy (lf.lfFaceName, fontlist[nFontName].GetName ());

	if (ptFontSample)
	{
		delete ptFontSample;
	}

	ptFontSample = new TFont (&lf);

	peditSample->SetWindowFont(*ptFontSample, TRUE);
	
	return TRUE;
}

void TFontPropertyPage::EvColor ()
{
	COLORREF	rgb = pcbColor->GetItemData (pcbColor->GetSelIndex ());

	peditSample->SetTextColor (rgb);
}

// =========================================================
// EvMeasureItem
//
// sets up the height for the owner draw font name list
// =========================================================
void TFontPropertyPage::EvMeasureItem (UINT, MEASUREITEMSTRUCT far& meas)
{
	TEXTMETRIC	tm;
	HFONT		hFont;
	TScreenDC	dc;

	hFont = GetWindowFont ();
	if (hFont)
	{
		dc.SelectObject (TFont (hFont));
	}

	dc.GetTextMetrics (tm);

	meas.itemHeight = tm.tmHeight;
}

// =========================================================
// TFontSpec
//
// defines an entry in the font array
// =========================================================
TFontSpec::TFontSpec ()
{
	memset (&lf, 0, sizeof (LOGFONT));

	nType = nFamily = 0;
}

TFontSpec::TFontSpec (int nTypeParam, const LOGFONT& lfParam, int nFamParam)
{
	memcpy (&lf, &lfParam, sizeof (LOGFONT));

	nType 	= nTypeParam;
	nFamily	= nFamParam;
}

BOOL TFontSpec::operator< (TFontSpec other) const
{
	return (lstrcmpi (GetName (), other.GetName ()) < 0);
}

BOOL TFontSpec::operator== (TFontSpec other) const
{
	return (lstrcmpi (GetName (), other.GetName ()) == 0);
}

// =========================================================
// TSampleStatic
//
// edit control used to display the sample text
// =========================================================
TSampleStatic::TSampleStatic (TWindow *parent, int resourceID, TModule *module)
	: TStatic (parent, resourceID, 30, module)
{
	colorText = TColor::Black;
}

void TSampleStatic::SetupWindow ()
{
	TStatic::SetupWindow ();

	ClearFlag (wfPredefinedClass);
}

void TSampleStatic::SetTextColor (TColor color)
{
	colorText = color;

	if (HWindow)
	{
		Invalidate ();
	}
}

void TSampleStatic::Paint (TDC& dc, BOOL, TRect&)
{
	TRect	rcClient = GetClientRect ();
	char	szChars[30];

#ifndef __WIN32__
	Parent->SendMessage (WM_CTLCOLOR, (WPARAM) (HDC) dc, MAKELPARAM (HWindow, CTLCOLOR_STATIC));
#else
	Parent->SendMessage (WM_CTLCOLORSTATIC, (WPARAM) (HDC) dc, (LPARAM) HWindow);
#endif
//	dc.SetBkColor (TColor::White);
	dc.ExtTextOut (0, 0, ETO_OPAQUE, &rcClient, 0, 0);

	GetText (szChars, sizeof (szChars));

	dc.SetTextColor (colorText);
	HFONT hfont = GetWindowFont ();

	dc.SetMapMode (MM_TWIPS);
	dc.DPtoLP (&rcClient.TopLeft ());
	dc.DPtoLP (&rcClient.BottomRight ());

	if (hfont)
	{
		dc.SelectObject (TFont (hfont));
	}

	WORD	wFormat;

	if (dc.GetTextExtent (szChars, lstrlen (szChars)).cx > rcClient.right)
	{
	/*	int	nDiff = -20 * 72 / 16;	// move up 1/16 of an inch

		dc.SetTextAlign (TA_LEFT | TA_BASELINE);
		dc.ExtTextOut (0, rcClient.bottom - nDiff, ETO_CLIPPED, &rcClient, szChars, lstrlen (szChars));
		*/
		wFormat = DT_LEFT;
	}
	else
	{
		wFormat = DT_CENTER;
	}

	dc.DrawText (szChars, -1, rcClient, (uint16) (wFormat | DT_VCENTER | DT_SINGLELINE));
}

// =========================================================
// TODComboBox
//
// owner draw combobox - DrawItem function handles basic
// owner-drawing mechanism
// =========================================================
void TODComboBox::DrawItem (DRAWITEMSTRUCT far& DrawInfo)
{
	TDC		dc (DrawInfo.hDC);
	TRect	rc (DrawInfo.rcItem);
	HBRUSH	hBrush;			// background brush

	if (DrawInfo.itemState & ODS_SELECTED)
	{
		dc.SetTextColor (GetSysColor (COLOR_HIGHLIGHTTEXT));
		dc.SetBkColor (GetSysColor (COLOR_HIGHLIGHT));
		hBrush = CreateSolidBrush (GetSysColor (COLOR_HIGHLIGHT));
	}
	else
	{
	#ifndef __WIN32__
		hBrush = (HBRUSH) SendMessage (WM_CTLCOLOR, (WPARAM) DrawInfo.hDC, MAKELPARAM (HWindow, CTLCOLOR_LISTBOX));
	#else
		hBrush = (HBRUSH) SendMessage (WM_CTLCOLORSTATIC, (WPARAM) DrawInfo.hDC, (LPARAM) HWindow);
	#endif
	}

	dc.FillRect (rc, hBrush);

	if (DrawInfo.itemState & ODS_SELECTED)
	{
		DeleteObject (hBrush);
	}

	if ((int) DrawInfo.itemID == -1)
	{
		return;
	}

	DrawText (dc, rc, DrawInfo);
}

// =========================================================
// TFontNameList
//
// combobox containing fontnames (with small icons)
// =========================================================
TFontNameList::TFontNameList (TWindow* parent, int Id, TFontList& fonts, TModule* module)
	: 	TODComboBox (parent, Id, LF_FACESIZE, module),
		fontlist (fonts),
		bmpType ((module ? module : parent->GetModule ())->GetInstance (), IBM_FONTTYPE)
{

}

void TFontNameList::DrawText (TDC& dc, TRect& rc, DRAWITEMSTRUCT far& DrawInfo)
{
	int	nType = fontlist[DrawInfo.itemID].GetType ();

	if (nType & TRUETYPE_FONTTYPE)
	{
		nType = 0;
	}
	else
	{
		if (nType & DEVICE_FONTTYPE)
		{
			nType = 20;
		}
		else
		{
			nType = -1;
		}
	}

	if (nType != -1)
	{
		DrawMaskedBitmap (dc, rc.left, rc.top, bmpType, nType, 20);
	}

	rc.left	+= 20;

	dc.DrawText (fontlist[DrawInfo.itemID].GetName (), -1, rc, DT_LEFT | DT_TOP);
}

// =========================================================
// TColorList
//
// combobox containing colors
// =========================================================
void TColorList::SetupWindow ()
{
	TODComboBox::SetupWindow ();

	COLORREF	rgb[] = {	RGB (0x00, 0x00, 0x00),
							RGB (0x80, 0x00, 0x00),
							RGB (0x00, 0X80, 0x00),
							RGB (0x80, 0x80, 0x00),
							RGB (0x00, 0x00, 0x80),
							RGB (0x80, 0x00, 0x80),
							RGB (0x00, 0x80, 0x80),
							RGB (0x80, 0x80, 0x80),
							RGB (0xC0, 0xC0, 0xC0),
							RGB (0xFF, 0x00, 0x00),
							RGB (0x00, 0xFF, 0x00),
							RGB (0xFF, 0xFF, 0x00),
							RGB (0x00, 0x00, 0xFF),
							RGB (0xFF, 0x00, 0xFF),
							RGB (0x00, 0xFF, 0xFF),
							RGB (0xFF, 0xFF, 0xFF) };

	for (int n = 0; n < 16; n++)
	{
		AddString ((LPCSTR) rgb[n]);
	}

	SetSelIndex (0);
}

void TColorList::DrawText (TDC& dc, TRect& rc, DRAWITEMSTRUCT far& DrawInfo)
{
	rc = rc.InflatedBy (-2, -1);

	dc.FillRect (rc, TBrush	((COLORREF) DrawInfo.itemData));

	if (DrawInfo.itemData == RGB (0xFF, 0xFF, 0xFF))
	{
		dc.FrameRect (rc, (HBRUSH) GetStockObject (BLACK_BRUSH));
	}
}

// =========================================================
// TFontData
//
// holds information about the font
// =========================================================
TFontData::TFontData ()
{
	Flags		= CF_EFFECTS | CF_FORCEFONTEXIST;
	Error		= NULL;
	DC			= NULL;
	PointSize	= 10;
	Color		= TColor::Black;
	Style		= NULL;
	FontType	= NULL;
	SizeMin		= 0;
	SizeMax		= 127;

	memset (&LogFont, 0, sizeof (LOGFONT));
};

TPrinterFontData::TPrinterFontData (TPrinter& printer)
{
	const char *pszDriverName = printer.GetSetup ().GetDriverName();

	if (pszDriverName != NULL)
	{
		pdc = new TPrintDC (pszDriverName, printer.GetSetup ().GetDeviceName(),
							printer.GetSetup ().GetOutputName(), printer.GetSetup ().GetDevMode());

		DC		= *pdc;
		Flags	|= CF_PRINTERFONTS;
	}
	else
	{
		pdc = NULL;
		Flags	|= CF_TTONLY;
	}
}

TPrinterFontData::~TPrinterFontData ()
{
	if (pdc)
	{
		delete pdc;
	}
}

// =========================================================
// utility functions
// =========================================================
#ifdef __WIN32__
int CALLBACK EnumFontFamProc (const LOGFONT FAR* plf, const TEXTMETRIC FAR* lptm, DWORD nType, LPARAM lpData)
{
	ENUMLOGFONT *elf = (ENUMLOGFONT *) plf;
#else
int CALLBACK EnumFontFamProc (const ENUMLOGFONT FAR* elf, const NEWTEXTMETRIC FAR* lptm, int nType, LPARAM lpData)
{
#endif
	ENUMSETUP	*setup = (ENUMSETUP *) lpData;

	switch (setup->nMode)
	{
		case ENUM_FONTNAMES :
		{
			BYTE	tmPitchAndFamily = (BYTE) (lptm->tmPitchAndFamily & 0xF);

			if (setup->dwFlags & CF_NOTTFONTS && (nType & TRUETYPE_FONTTYPE))
			{
				break;		// _IS_ a TrueType font (internal usage)
			}

			if (setup->dwFlags & CF_TTONLY && !(nType & TRUETYPE_FONTTYPE))
			{
				break;		// not a TrueType font
			}

			if (setup->dwFlags & CF_WYSIWYG && nType != (TRUETYPE_FONTTYPE | DEVICE_FONTTYPE))
			{
				break;		// not available on both
			}

			if (setup->dwFlags & CF_NOVECTORFONTS && tmPitchAndFamily == TMPF_VECTOR)
			{
				break;		// is a vector font
			}

			if (setup->dwFlags & CF_SCALABLEONLY)
			{
				if (!( 	(tmPitchAndFamily & TMPF_VECTOR) ||
						(tmPitchAndFamily & TMPF_TRUETYPE)))
				{
					break;	// not a scalable font
				}
			}

			if (setup->dwFlags & CF_FIXEDPITCHONLY)
			{
				if ((elf->elfLogFont.lfPitchAndFamily & 0xF) != FIXED_PITCH)
				{
					break;	// not a fixed font
				}
			}

			int	nCharSet = elf->elfLogFont.lfCharSet;

			if (setup->dwFlags & CF_ANSIONLY && nCharSet != ANSI_CHARSET)
			{
				break;		// not an ANSI font
			}

			if (setup->dwFlags & CF_NOOEMFONTS && nCharSet == OEM_CHARSET)
			{
				break;		// is an OEM font
			}

			TFontList	*pFontList = (TFontList *) setup->ptr;

			pFontList->Add (TFontSpec (nType, elf->elfLogFont, lptm->tmPitchAndFamily));
			break;
		}
		case ENUM_FONTSTYLES :
		{
			TComboBox	*ptCombo = (TComboBox *) setup->ptr;

			if (nType & TRUETYPE_FONTTYPE)
			{
				ptCombo->AddString ((LPCSTR) elf->elfStyle);
			}
			else
			{
				int		nID = IDS_STYLEREG;
				char	szTemp[20];

				if (nType & DEVICE_FONTTYPE)
				{
					if (elf->elfLogFont.lfItalic)
					{
						nID++;
					}

					if (elf->elfLogFont.lfWeight > FW_NORMAL)
					{
						nID += 2;
					}
					ptCombo->GetModule ()->LoadString (nID, szTemp, sizeof (szTemp));

					if (ptCombo->FindExactString (szTemp, -1) != -1)
					{
						return FALSE;
					}

					ptCombo->AddString (szTemp);
				}
				else
				{
					while (nID <= IDS_STYLEBLDITA)
					{
						int	nIndex = setup->parrayBold->GetItemsInContainer ();

						setup->parrayBold->Add (nIndex >= 2 ? FW_BOLD : FW_NORMAL);
						setup->parrayItalic->Add (nIndex & 1);

						ptCombo->GetModule ()->LoadString (nID++, szTemp, sizeof (szTemp));
						ptCombo->AddString (szTemp);
					}

                    // return immediately
					return FALSE;
				}
			}

			setup->parrayBold->Add (elf->elfLogFont.lfWeight);
			setup->parrayItalic->Add (elf->elfLogFont.lfItalic);

			break;
		}

		case ENUM_STYLEMATCH :
		{
			TComboBox	*ptCombo = (TComboBox *) setup->ptr;

			if (nType & TRUETYPE_FONTTYPE)
			{
				if ((setup->lplf->lfItalic != 0) == (elf->elfLogFont.lfItalic != 0))
				{
					if ((setup->lplf->lfWeight == 0 && elf->elfLogFont.lfWeight == FW_NORMAL) ||
						setup->lplf->lfWeight == elf->elfLogFont.lfWeight)
					{
						ptCombo->SetSelString ((LPCSTR) elf->elfStyle, -1);
						return FALSE;
					}
				}
			}
			break;
		}
	}

	return TRUE;
}

#ifdef __WIN32__
 int CALLBACK EnumFontSizeProc (const LOGFONT FAR*, const TEXTMETRIC FAR* lptm, DWORD, LPARAM lpData)
#else
 int CALLBACK EnumFontSizeProc (const LOGFONT FAR*, const TEXTMETRIC FAR* lptm, int, LPARAM lpData)
#endif
{
	ENUMSIZE *enmSize = (ENUMSIZE *) lpData;
	char     szTemp[10];
	int		nSize = lptm->tmHeight + lptm->tmExternalLeading;

	nSize = (nSize + 10) / 20;

	// line removed by B. Zink
// 	itoa (nSize, szTemp, 10);  // round to nearest point and convert to text
	// line added by B. Zink
	wsprintf( szTemp, "%2d", nSize );

	if (nSize >= enmSize->nMinSize && nSize <= enmSize->nMaxSize)
	{
		// Dont add it if already there - added by B. Zink
		if ( enmSize->pComboBox->FindString(szTemp, -1 ) < 0)
		{
			enmSize->pComboBox->AddString (szTemp);
		}
	}

   	return TRUE;
}
// =========================================================
// FillListFromResource
//
// populates a lists from a string resource
// =========================================================
void FillListFromResource (ENUMSIZE& enmSize, int nID)
{
	char	szText[100];
	char	*pszToken;

	enmSize.pComboBox->GetModule ()->LoadString (nID, szText, sizeof (szText));

	pszToken = strtok (szText, ",");

	while (pszToken)
	{
		int	nSize = atoi (pszToken);

		if (nSize >= enmSize.nMinSize && nSize <= enmSize.nMaxSize)
		{
			enmSize.pComboBox->AddString (pszToken);
		}

		pszToken = strtok (NULL, ",");
	}
}

#if defined (BUILDPROPDLGDLL) && defined (__DLL__)
	#pragma warn -wuse
	void dummyGenerateFontArray ()
	{
		TISArrayAsVector<TFontSpec>	dummy1 (1, 0, 1);
	}
#endif
