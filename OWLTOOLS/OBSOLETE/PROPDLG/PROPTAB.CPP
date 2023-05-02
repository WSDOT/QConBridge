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
// PROPTAB.CPP
//
// this code is #included into PROPDLG.CPP and shouldn't
// be added to the project file directly
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#pragma codeseg PROPTAB

// =========================================================
// TPropertyTab
//
// class which handles drawing and switching of pages
// =========================================================

DEFINE_RESPONSE_TABLE1 (TPropertyTab, TControl)
	EV_WM_LBUTTONDOWN,
	EV_WM_RBUTTONDOWN,
	EV_WM_SIZE,
	EV_WM_SETFOCUS,
	EV_WM_KILLFOCUS,
	EV_WM_KEYDOWN,
	EV_WM_GETDLGCODE,
END_RESPONSE_TABLE;

TPropertyTab::TPropertyTab (TWindow *pParent, Tab::Style style, TResId resBitmap, bool bMask)
	: 	TControl (pParent, 29000, "", 0, 0, 1, 1),
		pages (10, 0, 5)
{
	//
	// define if Windows 95 or not
	//
	bWin95 =(((::GetVersion () & 0xFF) >= 4) ||
			(((::GetVersion () & 0xFF) == 3) &&
			(((::GetVersion () >> 8) & 0xFF) >= 95)));

	//
	// assign tab style
	//
	styleTabs		= style;
	celTabBitmap 	= NULL;
	nTabActive 		= 0;
	bUseMask		= bMask;

	//
	// load up bitmap for placing on tabs (if we are using them)
	//
	if (styleTabs & Tab::UseTabBitmaps)
	{
		TBitmap* bitmap = new TBitmap (*GetApplication(), resBitmap);
		CHECKX(bitmap,"Could not create tab bitmap.");

		if (bitmap)
		{
			celTabBitmap = new TCelArray (bitmap, 1, TSize (0, 0));

			CHECKX (celTabBitmap, "Could not create tab bitmap array.");
		}//TCelArray will delete our bitmap pointer for us
	}

	//
	// if we have a Wizard-style frame, switch off non-compatible styles
	//
	if (styleTabs & Tab::WizardFrame)
	{
		// for wizards tabs, disable stacking, justification and double height
		styleTabs = styleTabs & ~(Tab::Stacked | Tab::Justified | Tab::DoubleHeight);
	}

	//
	// if we have a stacked (multiple row) frame, switch off non-compatible styles
	//
	if (styleTabs & Tab::Stacked)
	{
		// for stacked tabs, disable collapsing and enable justification
		styleTabs = (styleTabs & ~Tab::Collapsing) | Tab::Justified;
	}

	//
	// set up the font used for selected pages
	//
#if USEANSIVARFONT
	pfontNormal = new TFont ((HFONT) GetStockObject (ANSI_VAR_FONT));
#else
	pfontNormal = new TFont ("MS Sans Serif", -11);
#endif

	LOGFONT	lf;

	pfontNormal->GetObject (lf);
	lf.lfWeight = SELECTIONWEIGHT;
	pfontSelect = new TFont (&lf);

	CHECKX (pfontSelect, "Unable to create selected property font");

	//
	// Work out font height (thanks to Paul Winwood)
	//
	TRect rcCalc;

	{
		TClientDC	dc (HWindow);
		dc.SelectObject (*pfontSelect);
		dc.DrawText ("(|&y#", 5, rcCalc, DT_CENTER | DT_CALCRECT);
	}

	nTextHeight = rcCalc.Height();

	//
	// initialise important variables
	//
	nSelect 		= 0;
	nTabHeight		= 8 + nTextHeight + ((styleTabs & Tab::DoubleHeight) ? nTextHeight : 2) + ((ROUNDEDMARGIN - 2) >> 1);
	rcSize.SetEmpty ();

	//
	// set up parameters for scrolling
	//
	nTabOrigin		= 0;
	bLeftScroll 	=
	bRightScroll	= 0;
	bmpLeftArrow	=
	bmpRightArrow	= NULL;

	//
	// hard set the window attributes
	//
	Attr.Style = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN;

	if (!(styleTabs & Tab::WizardFrame))
	{
		Attr.Style |= WS_TABSTOP;
	}

	//
	// define default active tab colour (for Tab::ColorActive style)
	//
#if COLORWHOLETAB
	colorActive = TColor::White;
#else
	colorActive = TColor::Black;
#endif

	//
	// reset row order for stacked tabs
	//
	for (int n = 0; n < MAXROWS; n++)
	{
		nRowOrder[n] = n;
	}

	//
	// set fixed tab width to default
	//
	SetFixedTabWidth (DEFTABWIDTH);
	SetWideMarginWidth (DEFWIDEMARGIN);

	//
	// assign tab colors
	//
	TabColorChange ();
}

TPropertyTab::~TPropertyTab ()
{
	int		nCount = GetEntryCount ();

	for (int n = 0; n < nCount; n++)
	{
		if (pages[n].GetDialog ()->HWindow)
		{
			delete pages[n].GetDialog ();
		}
	}

	delete pfontNormal;
	delete pfontSelect;

	if (bmpLeftArrow)
	{
		delete bmpLeftArrow;
	}

	if (bmpRightArrow)
	{
		delete bmpRightArrow;
	}

	if (celTabBitmap)
	{
		delete celTabBitmap;
	}
}

// =========================================================
// SetupWindow
//
// creates the page dialogs
// =========================================================
void TPropertyTab::SetupWindow ()
{
	TControl::SetupWindow ();

	//
	// create the page dialogs
	//
	int		nCount = GetEntryCount ();
	int		n;

	//
	// If we use tab bitmaps, increase the number of bitmaps to be cut from
	// the TCelArray to match the number of pages
	//
	if (celTabBitmap && nCount)
	{
		celTabBitmap->SetNumCels (GetEntryCount());
		celTabBitmap->SetCelSize (TSize(celTabBitmap->operator TBitmap&().Width() / GetEntryCount(),
										celTabBitmap->operator TBitmap&().Height()));
	}

	//
	// create the pages
	//
	for (n = 0; n < nCount; n++)
	{
		TSSPropertyPage	*pPage = pages[n].GetDialog ();

		if (styleTabs & Tab::AllowDupPages && pPage->HWindow)
		{
			// reusing the same dialog has been allowed, so
			// skip the create call (as it would cause a
			// precondition violation)
			continue;
		}

		pPage->SetParent (this);

		if (!(styleTabs & Tab::CreateOnDemand) && !(styleTabs & Tab::CreateOnDemandAndKeepIt))
		{
			pPage->Create ();

			if (styleTabs & Tab::UseSmallFont)
			{
				UseSmallFont (pPage->HWindow);
			}
		}
	}

	//
	// select the first selectable page
	//
	for (n = 0; n < nCount; n++)
	{
		if (IsTabVisible (n) && IsTabEnabled (n))
		{
			nSelect = n;

			// inform interested parties that the
			// selection has changed
			TYPESAFE_DOWNCAST (Parent, TPropertyDialog)->AfterSelected (nSelect);
			pages[nSelect].GetDialog ()->AfterSelected ();
			break;
		}
	}

	//
	// set the focus on the selected page
	//
	SetPageFocus (false);

/*	if (nCount)
	{
		bool	setFocus 	= (styleTabs & Tab::FocusOnFirstControl) != 0L;
		HWND	hwndFocus 	= SetPageFocus (setFocus);

		if (setFocus && hwndFocus)
		{
		//	::SetFocus (hwndFocus);
		}
	}	*/
}

LPSTR TPropertyTab::GetClassName ()
{
	return "tbProperty";
}

void TPropertyTab::GetWindowClass (WNDCLASS& wc)
{
	TControl::GetWindowClass (wc);

	wc.style 		 =	CS_HREDRAW |
						CS_VREDRAW |
						CS_PARENTDC;

	wc.hbrBackground = NULL;	// don't use a background brush
}

void TPropertyTab::DestroyPages ()
{
	int		nCount = GetEntryCount ();

	for (int n = 0; n < nCount; n++)
	{
		if (pages[n].GetDialog ()->HWindow)
		{
			pages[n].GetDialog ()->Destroy ();	// thanks Bob!
		//	delete pages[n].GetDialog ();
		}
	}
}

// =========================================================
// Add
//
// adds a new page to the control
// =========================================================
void TPropertyTab::Add (LPCSTR lpszTab, TSSPropertyPage *pPage, bool bEnabled)
{
	TPropertyEntry	entry (lpszTab, pPage, bEnabled);

	pages.Add (entry);
}

// =========================================================
// SavePageData
//
// calls the SaveData function on each page before the
// dialog is closed (only after the OK button is pressed)
// =========================================================
bool TPropertyTab::SavePageData ()
{
	int		nCount = GetEntryCount ();

	for (int n = 0; n < nCount; n++)
	{
		if (pages[n].GetDialog ()->HWindow && !pages[n].GetDialog ()->SaveData ())
		{
			return false;
		}
	}

	return true;
}

// =========================================================
// CheckCanClose
//
// calls each page's CanPageClose function and returns false
// if any page can't close, eg. because of failed validation
// =========================================================
bool TPropertyTab::CheckCanClose ()
{
	int		nCount = GetEntryCount ();

	for (int n = 0; n < nCount; n++)
	{
		if (!pages[n].GetDialog ()->CanPageClose ())
		{
			pages[n].GetDialog ()->AfterCantClose ();

			return false;
		}
	}

	return true;
}

// =========================================================
// Paint, DrawTabs, DrawStackedTabs, DrawTab
//
// functions for painting the actual tabs
// =========================================================
void TPropertyTab::Paint (TDC& dc, bool, TRect& rcPaint)
{
	if (styleTabs & Tab::WizardFrame)
	{
		DrawWizardFrame (dc);

		return;
	}

	TRect	client;
	TRect	rcGray;

	GetClientRect (client);
	client.bottom--;
	client.right--;
	client.top += nTabHeight;

	dc.SaveDC ();

	// draw and accomodate left/right scroll buttons
	if (bLeftScroll | bRightScroll)
	{
		TRect	rcScroll;

		GetScrollRect (rcScroll);

		if (rcScroll.Touches (rcPaint))
		{
			TMemoryDC 	bmpDC (dc);

			// draw the left-hand scroll button
			GetScrollRect (rcScroll, 0);
			bmpDC.SelectObject (*bmpLeftArrow);
			dc.BitBlt (rcScroll, bmpDC, TPoint(0, 0), SRCCOPY);
			bmpDC.RestoreBitmap();

			// draw the right-hand scroll button
			GetScrollRect (rcScroll, 1);
			bmpDC.SelectObject (*bmpRightArrow);
			dc.BitBlt (rcScroll, bmpDC, TPoint(0, 0), SRCCOPY);
			bmpDC.RestoreBitmap();

			// draw white line under the scroll buttons
			TPen	penWhite (colorWhite);

			GetScrollRect (rcScroll);
			dc.SelectObject (penWhite);
			dc.MoveTo (rcScroll.left - SCROLLMARGIN, nTabHeight);
			dc.LineTo (rcScroll.right, nTabHeight);
			dc.RestorePen ();

			// don't allow tabs to overpaint scroll buttons
			dc.ExcludeClipRect (rcScroll);
			rcScroll.top	= 0;
			rcScroll.left	-= SCROLLMARGIN;

			// grey the surrounding area
			dc.FillRect (rcScroll, TBrush (colorLtGray));
			rcScroll.left	+= 4;

			dc.ExcludeClipRect (rcScroll);
		}
	}

	if (client.Touches (rcPaint))
	{
		//
		// draw the box surround of the enclosed area
		//
		{
			TPen	penWhite (colorWhite);

			dc.SelectObject (penWhite);

			dc.MoveTo (client.left, nTabHeight);
			dc.LineTo (client.left, client.bottom);

			dc.RestorePen ();
		}

		{
			TPen	penBlack (colorBlack);

			dc.SelectObject (penBlack);

			dc.LineTo (client.right, client.bottom);
			dc.LineTo (client.right, nTabHeight);

			dc.RestorePen ();
		}

		{
			TPen	penGray (colorDkGray);

			dc.SelectObject (penGray);

			rcGray = client.InflatedBy (-1, -1);

			dc.MoveTo (rcGray.left, rcGray.bottom);
			dc.LineTo (rcGray.right, rcGray.bottom);
			dc.LineTo (rcGray.right, nTabHeight);

			dc.RestorePen ();
		}

		dc.FillRect (rcGray, TBrush (colorLtGray));
	}

	//
	// now draw the tabs (if necessary)
	//
	SetRect (client);

	if (client.Touches (rcPaint))
	{
		if (styleTabs & Tab::Stacked)
		{
			DrawStackedTabs (dc);
		}
		else
		{
			DrawTabs (dc);
		}
	}

	dc.RestoreObjects ();
	dc.RestoreDC ();
}

void TPropertyTab::DrawTabs (TDC& dc)
{
	TRect	rcTabs;
	int		x = 0;
	int		nCount	= GetEntryCount ();

	SetRect (rcTabs);

	for (int n = nTabOrigin; n < nCount; n++)
	{
		if (IsTabVisible (n))
		{
			int	xOld = x;

			if (x > xMax)	// don't draw out-of-range tabs!!!
			{
				break;
			}

			DrawTab (dc, x, n);

			// draw the white line under this tab
			if (!IsTabSelected (n))
			{
				{
					TPen	penWhite (colorWhite);

					dc.SelectObject (penWhite);

					dc.MoveTo (xOld, nTabHeight);
					dc.LineTo (x, nTabHeight);

					dc.RestorePen ();
				}

			#if NONSELECTEDSPACE
				{
					TPen	penLtGray (colorLtGray);

					dc.SelectObject (penLtGray);

					dc.MoveTo (xOld, nTabHeight - 1);
					dc.LineTo (x, nTabHeight - 1);

					dc.RestorePen ();
				}
			#endif
			}
		}
	}

	if (x < rcTabs.right)
	{
		// draw white line along the top of the enclosed area
		TPen	penWhite (colorWhite);

		dc.SelectObject (penWhite);

		dc.MoveTo (x, nTabHeight);
		dc.LineTo (rcTabs.right, nTabHeight);

		dc.RestorePen ();

		// fill above the white line with grey
		rcTabs.left = x;
		rcTabs.bottom++;

		dc.FillRect (rcTabs, TBrush (colorLtGray));
	}
}

void TPropertyTab::DrawStackedTabs (TDC& dc)
{
	TRect	rcTabs;
	int		y = (nRows - 1) * nTabHeight;

	for (int nRow = 0; nRow < nRows; nRow++)
	{
		int		x = 0;
		int		nRealRow = nRowOrder[nRow];

		for (int iter = ptRanges[nRealRow].Left; iter <= ptRanges[nRealRow].Right; iter++)
		{
			int	xOld = x;

			DrawTab (dc, x, iter, y, nRow == 0);

			// draw the white line under this tab
			if (!nRow && !IsTabSelected (iter))
			{
				{
					TPen	penWhite (colorWhite);

					dc.SelectObject (penWhite);

					dc.MoveTo (xOld, y + nTabHeight);
					dc.LineTo (x, y + nTabHeight);

					dc.RestorePen ();
				}

			#if NONSELECTEDSPACE
				{
					TPen	penLtGray (colorLtGray);

					dc.SelectObject (penLtGray);

					dc.MoveTo (xOld, y + nTabHeight - 1);
					dc.LineTo (x, y + nTabHeight - 1);

					dc.RestorePen ();
				}
			#endif
			}
		}

		y -= nTabHeight;
	}
}

void TPropertyTab::DrawTab (TDC& dc, int& xParam, int nTab, int yParam, bool bIsLast)
{
	WORD	fuFormat;
	int		nTabWidth 	= GetTabWidth (nTab);
#if NONSELECTEDSPACE
	int		yExtra = IsTabSelected (nTab) ? 0 : 2;
#else
	int		yExtra = IsTabSelected (nTab) ? 0 : 1;
#endif
	int		x = xParam + !IsTabSelected (nTab);
	int		x2 = xParam + nTabWidth;
	int		y = yParam + yExtra;
	TRect	rcTab;
	TRect	rcScrollClip;
	TPen*	ppenActive = NULL;

	rcTab.Set (x + 1, y + 1, x2 - 2, yParam + nTabHeight + bIsLast);
	rcScrollClip.SetEmpty ();

	//
	// check to see if this tab needs a right hand "jagged edge"
	//
	if (bLeftScroll | bRightScroll)
	{
		TRect	rcScroll;

		GetScrollRect (rcScroll);
		rcScroll.left	-= SCROLLMARGIN;

		if (rcTab.right > rcScroll.left)
		{
			TPen	penGray (colorDkGray);
			int		yDiff = (rcScroll.bottom - y) / 3;

			//
			// paint the jagged edge
			//
			dc.SelectObject (penGray);

			dc.MoveTo (rcScroll.left + 1, y);
			dc.LineTo (rcScroll.left + 2, y + yDiff);
			dc.LineTo (rcScroll.left + 0, y + yDiff + yDiff);
			dc.LineTo (rcScroll.left + 2, rcScroll.bottom - 2);
			dc.LineTo (rcScroll.left + 1, rcScroll.bottom);

			dc.RestorePen ();

			rcScroll.top	= y;

			rcScrollClip = rcScroll;
			dc.ExcludeClipRect (rcScrollClip);
		}
	}

	//
	// fill the tab
	//
#if COLORWHOLETAB
	if ((styleTabs & Tab::ColorActive) && IsTabSelected (nTab))
	{
		dc.FillRect (rcTab, TBrush (colorActive));
	}
	else
#endif
	{
		dc.FillRect (rcTab, TBrush (colorLtGray));
	}

	if (!bIsLast)
	{
		rcTab.bottom++;
	}

	//
	// draw the frame around the tab
	//
	TPen	penGray (colorLtGray);
	TPen	penDkGray (colorDkGray);
	TPen	penWhite (colorWhite);
	TPen	penBlack (colorBlack);

#if !COLORWHOLETAB
	if ((styleTabs & Tab::ColorActive) && IsTabSelected (nTab))
	{
		ppenActive = new TPen (colorActive);

		dc.SelectObject (*ppenActive);
	}
	else
#endif
	{
		dc.SelectObject (penWhite);
	}

	dc.MoveTo (x, yParam + nTabHeight - yExtra);

	dc.LineTo (x, y + ROUNDEDMARGIN);
	dc.LineTo (x + ROUNDEDMARGIN, y);
	dc.LineTo (x2 - ROUNDEDMARGIN, y);

	if (! ((styleTabs & Tab::ColorActive) && IsTabSelected (nTab)))
	{
		dc.SelectObject (penDkGray);
	}

	dc.MoveTo (x2 - ROUNDEDMARGIN, y + 1);
	dc.LineTo (x2 - 2, y + ROUNDEDMARGIN - 1);
	dc.LineTo (x2 - 2, yParam + nTabHeight + 1 - yExtra);

	//
	// draw the "shadow"
	//
	dc.SelectObject (penBlack);

	rcTab = rcTab.InflatedBy (-1, -1);

	dc.MoveTo (x2 - 1, y + ROUNDEDMARGIN);
	dc.LineTo (x2 - 1, yParam + nTabHeight + 1 - yExtra);

	//
	// destroy the "Tab::ColorActive" pen, if one was created
	//
	if (ppenActive)
	{
		delete ppenActive;
	}

	//
	// blank out previous selection at this position
	//
	if (IsTabSelected (nTab))
	{
		TPoint	pt[3];
		TBrush brush (colorLtGray);

		dc.SelectObject (brush);

		{
			pt[0]	= TPoint (x, y + ROUNDEDMARGIN);
			pt[1]	= TPoint (x, y);
			pt[2]	= TPoint (x + ROUNDEDMARGIN, y);

			TRegion rgn (pt, 3, ALTERNATE);
			dc.PaintRgn (rgn);
		}

		{
			pt[0]	= TPoint (x2, y + ROUNDEDMARGIN);
			pt[1]	= TPoint (x2, y);
			pt[2]	= TPoint (x2 - ROUNDEDMARGIN, y);

			TRegion rgn (pt, 3, ALTERNATE);
			dc.PaintRgn (rgn);
		}

        dc.RestoreBrush ();
	}
	else
	{
		TPoint	pt[7];

		pt[0]	= TPoint (x, y + ROUNDEDMARGIN);
		pt[1]	= TPoint (x + ROUNDEDMARGIN, y);
		pt[2]	= TPoint (x2 - ROUNDEDMARGIN, y);
		pt[3]	= TPoint (x2, y + ROUNDEDMARGIN);
		pt[4]	= TPoint (x2, yParam);
		pt[5]	= TPoint (x, yParam);
		pt[6]	= TPoint (x, y + ROUNDEDMARGIN);

		TBrush brush (colorLtGray);
		TRegion rgn (pt, 7, ALTERNATE);

	#if COLORWHOLETAB
		dc.SelectObject (penGray);

		dc.MoveTo (xParam, yParam);
		dc.LineTo (xParam, yParam + nTabHeight);
	#endif

		dc.SelectObject (brush);
		dc.PaintRgn (rgn);

        dc.RestoreBrush ();
	}

	//
	// restore the stock pen to prevent
    // orphan messages on NT
	//
	dc.SelectStockObject (BLACK_PEN);

	//
	// save away the DC settings prior to clipping the tab
	//
	dc.SaveDC ();

	//
	// define an exclusion area to protect the
	// tab border from overpainting
	//
	{
		TPoint	pt[6];

		pt[0]	= TPoint (x + 1, yParam + nTabHeight + 1 - yExtra);
		pt[1]	= TPoint (pt[0].x, y + ROUNDEDMARGIN);
		pt[2]	= TPoint (x + ROUNDEDMARGIN, y + 1);
		pt[3]	= TPoint (x2 - ROUNDEDMARGIN, pt[2].y);
		pt[4]	= TPoint (x2 - 2, y + ROUNDEDMARGIN - 1);
		pt[5]	= TPoint (pt[4].x, pt[0].y);

		TRegion rgn (pt, 6, ALTERNATE);

		dc.SelectClipRgn (rgn);

		if (!rcScrollClip.IsEmpty ())
		{
			dc.ExcludeClipRect (rcScrollClip);
		}
	}

	//
	// draw the tab text
	//
	{
		if (!IsTabSelected (nTab))
		{
			dc.SelectObject (*pfontNormal);
		}
		else
		{
			dc.SelectObject (*pfontSelect);
		}

		dc.SetBkMode (TRANSPARENT);
		dc.SetTextColor (IsTabEnabled (nTab) ? TColor (GetSysColor (COLOR_WINDOWTEXT)) : TColor::Gray);

		rcTab = rcTab.InflatedBy (-((ROUNDEDMARGIN - 2) >> 1), 0);
		rcTab.top += (ROUNDEDMARGIN - 2) >> 1;

		TRect	rcCalc (rcTab);
		string&	s = pages[nTab].GetText ();

		fuFormat = DT_VCENTER | DT_SINGLELINE;

		dc.DrawText (s.c_str (), s.length (), rcCalc, DT_CENTER | DT_CALCRECT | DT_WORDBREAK);

		if ((styleTabs & Tab::DoubleHeight) && rcCalc.Height () > nTextHeight + 4)
		{
			fuFormat = DT_WORDBREAK;
		}

		//
		// draw tab bitmaps (if in use)
		//
		if (celTabBitmap)
		{
			TRect	TextRect (rcTab);	// For positioning bitmap and text
			TRect 	bmRect (rcTab);
			int		nBmpWidth = (celTabBitmap->operator TBitmap&().Width () / GetEntryCount() );
			int		nBmpHeight = celTabBitmap->operator TBitmap&().Height();

			if (bUseMask)
			{
            	nBmpHeight >>= 1;
			}

			//
			// So bitmap and text won't overlap each other
			//
			TextRect.left += ( nBmpWidth + 12);

			bmRect.left  += 6; //Indent 6 pixels
			bmRect.top   += ( rcTab.Height() - nBmpHeight) / 2;	//center
			bmRect.right  = bmRect.left + nBmpWidth;			//width of bitmap
			bmRect.bottom = bmRect.top + nBmpHeight;			//Height of bitmap

			if (bUseMask)
			{
				DrawMaskedBitmap (dc, bmRect.left, bmRect.top, celTabBitmap->operator TBitmap&(), nTab * nBmpWidth, nBmpWidth);
			}
			else
			{
			   TMemoryDC	dcMem (dc);

			   //Never found out how to use operator TBitmap&() of celTabBitmap
			   //Tried TBitmap&(celTabBitmap), (TBitmap&)celTabBitmap
			   //nothing but this worked
			   dcMem.SelectObject (celTabBitmap->operator TBitmap&());

			   dc.BitBlt ( bmRect, dcMem, celTabBitmap->CelOffset(nTab) );
			}

			dc.DrawText (s.c_str (), s.length (), TextRect, (WORD) (DT_CENTER | fuFormat));
		}
		else //Draw normal text
		{
			dc.DrawText (s.c_str (), s.length (), rcTab, (WORD) (DT_CENTER | fuFormat));
		}

		//
		// draw the dotten focus rectangle
		//
		if ( IsTabSelected (nTab) && HWindow == GetFocus () )
		{
			dc.DrawFocusRect (rcTab);
		}

	}

	//
	// restore the saved DC settings
	//
	dc.RestoreDC ();

	xParam += nTabWidth;
}

void TPropertyTab::DrawWizardFrame (TDC& dc)
{
	TRect	client;

	TPen	penDkGray (colorDkGray);
	TPen	penWhite (colorWhite);
	TBrush	brushGray (colorLtGray);

	GetClientRect (client);
	client.top = client.bottom - 2;
	dc.FillRect (client, brushGray);

	dc.SelectObject (penWhite);
	client.bottom--;
	dc.MoveTo (WIZARDMARGIN, client.bottom);
	dc.LineTo (client.right - WIZARDMARGIN, client.bottom);

	dc.SelectObject (penDkGray);
	client.bottom--;
	dc.MoveTo (WIZARDMARGIN, client.bottom);
	dc.LineTo (client.right - WIZARDMARGIN, client.bottom);

    dc.RestorePen ();
}

// =========================================================
// SetRect
//
// returns the TRect area that the tabs occupy
// =========================================================
void TPropertyTab::SetRect (TRect& rc, int nRow)
{
	GetClientRect (rc);

	if (nRow == -1)
	{
		rc.bottom = (nTabHeight * nRows) - 1;
	}
	else
	{
		rc.top 		= (nRows - nRow - 1) * nTabHeight;
		rc.bottom 	= rc.top + nTabHeight - 1;
	}

	if (bLeftScroll | bRightScroll)
	{
		TRect	rcScroll;

		GetScrollRect (rcScroll);

		rc.right = rcScroll.left - SCROLLMARGIN;
	}
}

// =========================================================
// IsTabVisible, IsTabSelected, IsTabEnabled
//
// return information about a particular tab
// =========================================================
bool TPropertyTab::IsTabVisible (int nTab) const
{
	if (styleTabs & Tab::Collapsing)
	{
		return IsTabEnabled (nTab);
	}
	else
	{
		return true;
	}
}

bool TPropertyTab::IsTabSelected (int nTab) const
{
	PRECONDITIONX (nTab >= 0 && nTab < pages.GetItemsInContainer (), "IsTabSelected: Invalid index");

	return nTab == nSelect;
}

bool TPropertyTab::IsTabEnabled (int nTab) const
{
	PRECONDITIONX (nTab >= 0 && nTab < pages.GetItemsInContainer (), "IsTabEnabled: Invalid index");

	return pages[nTab].IsEnabled ();
}

// =========================================================
// EvLButtonDown, GetMouseTab
//
// process mouse messages.  GetMouseTab indicates which tab
// the mouse is currently over
// =========================================================
void TPropertyTab::EvLButtonDown (UINT, TPoint& point)
{
	TRect	rc;

	if (bLeftScroll)
	{
		GetScrollRect (rc, 0);

		if (rc.Contains (point))
		{
			NextOrigin (-1);

			return;
		}
	}
	if (bRightScroll)
	{
		GetScrollRect (rc, 1);

		if (rc.Contains (point))
		{
			NextOrigin ();

			return;
		}
	}

	if (bLeftScroll | bRightScroll)
	{
		GetScrollRect (rc);
		rc.left -= SCROLLMARGIN;
		rc.top = 0;

		if (rc.Contains (point))
		{
			// no available
			return;
		}
	}

	SelectTab (GetMouseTab (point));
}

void TPropertyTab::EvRButtonDown (UINT modKeys, TPoint& point)
{
	int nTab = GetMouseTab (point);

	if (nTab != -1)
	{
		pages[nTab].GetDialog ()->OnRightClick (modKeys, point);
	}
}

int TPropertyTab::GetMouseTab (TPoint& point)
{
	TRect	rc;
	int		nCount = GetEntryCount ();

	for (int n = nTabOrigin; n < nCount; n++)
	{
		if (IsTabVisible (n))
		{
			if (GetTabRect (rc, n))
			{
				if (rc.Contains (point))
				{
					if (pages[n].IsEnabled ())
					{
						return n;
					}

					break;	// disabled, so don't continue
				}
			}
		}
	}

	return -1;
}

// =========================================================
// GetTabRect
//
// returns the TRect area of a particular tab
// =========================================================
bool TPropertyTab::GetTabRect (TRect& rc, int nTab, int nOrigin)
{
	int		nFirst;
	int		n;
	int		nCount = GetEntryCount ();

	SetRect (rc);

	if (nOrigin == -1)
	{
		nOrigin = nTabOrigin;
	}

	if (styleTabs & Tab::Stacked)
	{
		n	= GetTabRow (nTab);

		if (n != -1)
		{
			int	nRealRow = nRowOrder[n];

			nFirst = ptRanges[nRealRow].Left;
			nCount = ptRanges[nRealRow].Right + 1;

			rc.top	= (nRows - 1 - n) * nTabHeight;
			rc.bottom = rc.top + nTabHeight;
		}
	}
	else
	{
		nFirst = nOrigin;
		nCount = GetEntryCount ();
	}

	for (n = nFirst; n < nCount; n++)
	{
		int	nTabWidth = GetTabWidth (n);

		if (n == nTab)
		{
			rc.right = rc.left + nTabWidth - 1;

			return IsTabVisible (n);
		}

		if (IsTabVisible (n))
		{
			rc.left += nTabWidth;
		}
	}

	return false;
}

// =========================================================
// GetTabRow
//
// returns the row of a particular tab
// =========================================================
int TPropertyTab::GetTabRow (int nTab)
{
	for (int n = 0; n < nRows; n++)
	{
		int	nRealRow = nRowOrder[n];

		if (nTab >= ptRanges[nRealRow].Left && nTab <= ptRanges[nRealRow].Right)
		{
			return n;
		}
	}

	return -1;
}

// =========================================================
// CalcTabWidth
//
// returns the display width of a particular tab
// =========================================================
int TPropertyTab::CalcTabWidth (TDC& dc, int nTab)
{
	if (styleTabs & Tab::FixedWidth)
	{
		return nFixedTabWidth;
	}

	TSize	sz;
	string&	s = pages[nTab].GetText ();
	LPCSTR	lpszText = s.c_str ();

	dc.SelectObject (*pfontSelect);

	bool 	bAnyBreaks = (strchr (lpszText, ' ') != NULL) ||
						 (strchr (lpszText, '\r') != NULL);

	if ((styleTabs & Tab::DoubleHeight) && bAnyBreaks)
	{
		// keep increasing the width until the tab text
		// will fit on two lines
		sz.cx = nFixedTabWidth;

		for (;;)
		{
			TRect	rc (0, 0, sz.cx, nTextHeight);

			dc.DrawText (lpszText, s.length (), rc, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_CALCRECT);

			if (rc.bottom < nTextHeight * 3)
			{
				sz.cx = rc.right;
				break;
			}

			sz.cx += 8;
		}
	}
	else
	{
		sz.cx = nFixedTabWidth;

		TRect	rc (0, 0, sz.cx, nTextHeight);

		dc.DrawText (lpszText, s.length (), rc, DT_LEFT | DT_TOP | DT_CALCRECT);

		sz.cx = rc.right;
	}

	// allow for borders
	sz.cx += 7 + ROUNDEDMARGIN - 2;

	if (styleTabs & Tab::WideMargins)
	{
		// add wide margin width
		sz.cx += (nWideMarginWidth << 1);
	}

	if (styleTabs & Tab::UseTabBitmaps)
	{
		// allow for the tab bitmaps (if we are using them)
		sz.cx += (celTabBitmap->operator TBitmap& ().Width() / GetEntryCount()) + 12;
	}

	if (sz.cx > nFixedTabWidth)
	{
		return sz.cx;
	}
	else
	{
		return nFixedTabWidth;
	}
}

// ==============================================================
// CalculateTabWidths, CalculateBasicTabWidths, ReassessTabWidths
//
// calculates widths of all tabs
// ==============================================================
void TPropertyTab::CalculateTabWidths ()
{
	//
	// check if tab control properly sized yet, if not then hold back
	//
	TRect	rcTabs;

	SetRect (rcTabs);

	if (rcTabs.right - rcTabs.left <= 1)
	{
		return;
	}

	//
	// calculate basic tab widths
	//
	CalculateBasicTabWidths ();

	//
	// space out tabs if justification required
	//
	if (styleTabs & Tab::Justified)
	{
		int 	iter;
		int		nCount;
		int		xRow;

		if (!(styleTabs & Tab::Stacked) && (bLeftScroll | bRightScroll))
		{
			// can't justify non-stacked controls with scrolling
			return;
		}

		for (int n = 0; n < nRows; n++)
		{
			xRow 	=
			nCount 	= 0;

			//
			// calculate the amount of slack on this row
			//
			for (iter = ptRanges[n].Left; iter <= ptRanges[n].Right; iter++)
			{
				if (IsTabVisible (iter))
				{
					xRow += pages[iter].GetWidth ();
					nCount++;
				}
			}

			xRow = (rcTabs.right - rcTabs.left) - xRow;

			if (xRow < 0)
			{
				return;
			}

			//
			// allocate the slack between all tabs on this row
			//
			for (iter = ptRanges[n].Left; iter <= ptRanges[n].Right; iter++)
			{
				if (!xRow || !nCount)	// nothing more to allocate
				{
					break;
				}

				if (IsTabVisible (iter))
				{
					// add in a little more space to this control
					pages[iter].SetWidth (pages[iter].GetWidth () + (xRow / nCount));

					xRow -= xRow / nCount--;	// remove this space from the allocatable space
				}
			}
		}
	}
}

void TPropertyTab::CalculateBasicTabWidths ()
{
	int		nCount = GetEntryCount ();
	int		n;

	TClientDC	dc (HWindow);

	for (n = 0; n < nCount; n++)
	{
		pages[n].SetWidth (CalcTabWidth (dc, n));
	}

	//
	// work out tab-to-row allocations
	//
	if (styleTabs & Tab::Stacked)
	{
		TRect	rcTabs;

		SetRect (rcTabs);

		for (n = nRows = 0; n < nCount; n++)
		{
			ptRanges[nRows].Left	=
			ptRanges[nRows].Right	= n;

			int	x 		= rcTabs.left;

			for (int iter = n; iter < nCount; iter++)
			{
				if (IsTabVisible (iter))
				{
					x += pages[iter].GetWidth ();
				}

				if (x < rcTabs.right)
				{
					ptRanges[nRows].Right = iter;
					n = iter;
				}
				else
				{
					nRows++;
					break;
				}
			}
		}

		if (nCount)
		{
			nRows++;
		}
	}
	else
	{
		nRows = 1;

		ptRanges[0].Left	= 0;
		ptRanges[0].Right	= GetEntryCount () - 1;
	}
}

void TPropertyTab::ReassessTabWidths ()
{
	if (styleTabs & Tab::Justified)
	{
		//
		// recalculate tab widths with/without scroll buttons
		//
		CalculateTabWidths ();

		TRect	rcTabs;

		SetRect (rcTabs);
		rcTabs.right++;
		rcTabs.bottom += 2;
		InvalidateRect (rcTabs);
	}
}

// =========================================================
// GetLastTab
//
// param = true - returns last fully-visible enabled tab
// param = false- returns last enabled tab
// =========================================================
int TPropertyTab::GetLastTab (bool bVisible)
{
	int		nTabs = GetEntryCount ();
	TRect	rcTab;

	while (--nTabs >= 0)
	{
		if (bVisible)
		{
			GetTabRect (rcTab, nTabs);

			if (rcTab.right > xMax)
			{
				continue;
			}
		}

#if COLLAPSEITEMS
		if (IsTabEnabled (nTabs))
#endif
		{
			break;
		}
	}

	if (nTabs < 0)
	{
		nTabs = 0;
	}

	return nTabs;
}


// =========================================================
// SelectTab, SelectNext, SelectTabByKey
//
// select another tab and redraw to reflect the change
// =========================================================
bool TPropertyTab::SelectTab (int nTab)
{
	if (nTab == -1)
	{
		return false;
	}

	if (nTab != nSelect)
	{
		PRECONDITIONX (nTab >= 0 && nTab < pages.GetItemsInContainer (), "SelectTab: Invalid index");

		// call the BeforeLeaving function
		static bool bEverSelected = false;
		if (bEverSelected)
		{
			if (!TYPESAFE_DOWNCAST (Parent, TPropertyDialog)->BeforeLeaving (nTab) ||
				!pages[nSelect].GetDialog ()->BeforeLeaving ())
			{
				return false;
			}
		}
		bEverSelected = true;

		// prevent errors for first selection
		if (nSelect < 0)
		{
			nSelect = 0;
		}

		TRect	rcTab;
		bool	bOriginChanged = AdjustOrigin (nTab);
		int		nOldSelect = nSelect;

		if (pages[nSelect].GetDialog () != pages[nTab].GetDialog ())
		{
			// get the existing invalid region
			TRegion	rgn;

			// store the existing invalid region
			GetUpdateRgn (rgn, false);

			// hide only if actual dialog box is changing
			// (prevents flickers when using Tab::AllowDupPages)
			SendMessage (WM_SETREDRAW, false);

			if (pages[nSelect].GetDialog ()->HWindow)
			{
				if (styleTabs & Tab::CreateOnDemand)
				{
					pages[nSelect].GetDialog ()->SaveData ();
					pages[nSelect].GetDialog ()->Destroy ();
				}
				else
				{
					if (styleTabs & Tab::SaveOnClosePage)
					{
						pages[nSelect].GetDialog ()->SaveData ();
					}

					pages[nSelect].GetDialog ()->Show (SW_HIDE);
				}
			}

			// re-enable painting
			SendMessage (WM_SETREDRAW, true);

			// and re-invalidate any originally invalidated area
			// (WM_SETREDRAW clears the 'invalid' flag of the window)
			InvalidateRgn (rgn, false);
		}

		if (styleTabs & Tab::Stacked)
		{
			int	nTabRow 	= GetTabRow (nTab);
			int nRealRow 	= nRowOrder[nTabRow];

			//
			// do I need to swap rows
			//
			if (nTabRow != 0)
			{
				TRect		rcRow;
				int			nTemp = nRowOrder[0];

				// swap the rows
				nRowOrder[0] 		= nRealRow;
				nRowOrder[nTabRow] 	= nTemp;

				// redraw bottom row
				SetRect (rcRow, 0);
				rcRow.bottom += 2;
				InvalidateRect (rcRow);

				// redraw original row
				SetRect (rcRow, nTabRow);
				rcRow.bottom += 2;
				InvalidateRect (rcRow);
			}
		}

		nSelect = nTab;

		if (!bOriginChanged)
		{
			GetTabRect (rcTab, nOldSelect);
			rcTab.right++;
			rcTab.bottom += 2;

			InvalidateRect (rcTab, false);

			GetTabRect (rcTab, nTab);
			rcTab.right++;
			rcTab.bottom += 2;
			InvalidateRect (rcTab, false);
		}
		else
		{
			TRect	rcScroll;

			GetScrollRect (rcScroll);

			rcScroll -= SCROLLMARGIN;
			rcScroll.top = 0;

			InvalidateRect (rcScroll);

			AssessScroll ();
		}

		// set the focus to the first child
		SetPageFocus ();

		// inform interested parties that the
		// selection has changed
		TYPESAFE_DOWNCAST (Parent, TPropertyDialog)->AfterSelected (nTab);
		pages[nSelect].GetDialog ()->AfterSelected ();
	}
	else
	{
		SetPageFocus ();
	}

	nTabActive = nTab;

	return true;
}

bool TPropertyTab::SelectNext (int nDir)
{
	// +-------------------------------------------------
	// select next/previous tab
	// +-------------------------------------------------
	int	nNew = nSelect;

	do
	{
		nNew += nDir;

		if (nNew < 0)
		{
			nNew = GetEntryCount () - 1;
		}
		else
		{
			if (nNew >= GetEntryCount ())
			{
				nNew = 0;
			}
		}

		if (IsTabVisible (nNew) && IsTabEnabled (nNew))
		{
			if (SelectTab (nNew))
			{
				UpdateWindow ();

				return true;
			}

			break;
		}
	}
	while (nNew != nSelect);

	return false;
}

bool TPropertyTab::SelectTabByKey (char cKey)
{
	// +-------------------------------------------------
	// select a tab according to some hot key combination
	// +-------------------------------------------------
	int		nCount = GetEntryCount ();

	if (cKey == 0)
	{
		return false;
	}

	for (int nTab = 0; nTab < nCount; nTab++)
	{
		if (IsTabEnabled (nTab))
		{
			if (MatchesHotKey (pages[nTab].GetText ().c_str (), cKey))
			{
				return SelectTab (nTab);
			}
		}
	}

	return false;
}

bool TPropertyTab::AdjustOrigin (int nTab)
{
	int		nOldOrigin = nTabOrigin;

	//
	// check to see if the origin needs to change
	//
	if (nTab < nTabOrigin)
	{
		nTabOrigin = nTab;
	}
	else
	{
		while (nTabOrigin < nTab)
		{
			TRect	rcTab;

			if (GetTabRect (rcTab, nTab))
			{
				if (rcTab.right >= xMax)
				{
					nTabOrigin++;

					continue;	// go around again
				}
			}

			break;
		}
	}

	if (nOldOrigin == nTabOrigin)
	{
		return false;
	}

	TRect	rcAllTabs;

	SetRect (rcAllTabs);
	rcAllTabs.right++;
	rcAllTabs.bottom += 2;
	InvalidateRect (rcAllTabs, false);

	return true;
}

// =========================================================
// EnableTab
//
// used to enable/disable specific tabs
// =========================================================
void TPropertyTab::EnableTab (int nTab, bool bEnable)
{
	PRECONDITIONX (nTab >= 0 && nTab < pages.GetItemsInContainer (), "EnableTab: Invalid index");

	if (IsTabEnabled (nTab) == bEnable)
	{
		// nothing to do
		return;
	}

	pages[nTab].EnableTab (bEnable);

	TRect	rcTab;

	GetTabRect (rcTab, nTab);
	rcTab.right++;
	rcTab.bottom += 2;

	if (styleTabs & Tab::Collapsing)
	{
		TRect	client;

		SetRect (client);

		rcTab.right = client.right;

		if (!AdjustOrigin (nSelect))
		{
			AssessScroll ();

			if (styleTabs & Tab::Justified)
			{
				ReassessTabWidths ();
			}

			InvalidateRect (rcTab, true);
		}
	}
	else
	{
		InvalidateRect (rcTab);
	}
}

// =========================================================
// GetTabText, SetTabText
//
// used to get and set the text on a tab
// =========================================================
string TPropertyTab::GetTabText (int nTab)
{
	PRECONDITIONX (nTab >= 0 && nTab < pages.GetItemsInContainer (), "GetTabText: Invalid index");

	return pages[nTab].GetText ();
}

void TPropertyTab::SetTabText (int nTab, LPCSTR lpszText)
{
	PRECONDITIONX (nTab >= 0 && nTab < pages.GetItemsInContainer (), "SetTabText: Invalid index");

	if (lpszText == NULL)
	{
		// change to ""
		lpszText = "";
	}

	pages[nTab].SetText (lpszText);

	TRect	rcTab;

	if (styleTabs & Tab::Stacked)
	{
		TRect	rcTabOld;
		SetRect (rcTabOld, -1);

		CalculateTabWidths ();

		SetRect (rcTab, -1);

        // check to see the number of rows changed..
		if (rcTab.bottom != rcTabOld.bottom)
		{
			RedrawDialogs ();

			if (rcTab.bottom < rcTabOld.bottom)
			{
				rcTabOld.top = rcTab.bottom + 1;

                InvalidateRect (rcTabOld);
			}
		}
	}
	else
	{
		CalculateBasicTabWidths ();
		AssessScroll ();
		CalculateTabWidths ();

		GetTabRect (rcTab, nTab);
		rcTab.right = GetClientRect ().right;
		rcTab.bottom += 2;
	}

	InvalidateRect (rcTab, true);
}

// =========================================================
// EvSize
//
// called when a WM_SIZE message is received.
// resizes all page dialogs to fit correctly in the tab area
// =========================================================
void TPropertyTab::EvSize (UINT sizeType, TSize& size)
{
	TControl::EvSize (sizeType, size);

	//
	// calculate all tab widths
	//
	CalculateTabWidths ();
	AssessScroll ();

	RedrawDialogs ();
}

void TPropertyTab::RedrawDialogs ()
{
	int		nCount = GetEntryCount ();
	TRect	rcClient (GetClientRect ());

	rcSize.Set (1, (nTabHeight * nRows) + 1, rcClient.right - 2, rcClient.bottom - 2);

	//
	// calculate all tab widths
	//
	for (int n = 0; n < nCount; n++)
	{
		if (pages[n].GetDialog ()->HWindow)
		{
			pages[n].GetDialog ()->SetWindowPos (NULL, rcSize, SWP_NOZORDER);
		}
	}
}

// =========================================================
// SetPageFocus
//
// sets the focus to the first valid control on a page
// =========================================================
HWND TPropertyTab::SetPageFocus (bool bSetFocus)
{
	if ((styleTabs & Tab::CreateOnDemand || styleTabs & Tab::CreateOnDemandAndKeepIt) &&
		!pages[nSelect].GetDialog ()->HWindow)
	{
		pages[nSelect].GetDialog ()->Create ();
		pages[nSelect].GetDialog ()->SetWindowPos (NULL, rcSize, SWP_NOZORDER);

		if (styleTabs & Tab::UseSmallFont)
		{
			UseSmallFont (pages[nSelect].GetDialog ()->HWindow);
		}
	}

	//
	// move the focus
	//
	HWND	hwndFocus = HWindow;

	if (bSetFocus)
	{
		GetApplication ()->PumpWaitingMessages ();

		//
		// if focus should be put on first control, locate that control
		//
		if (styleTabs & Tab::FocusOnFirstControl)
		{
			hwndFocus = GetFirstControl ();

			//
			// no focusable controls on the dialog, so put
			// the focus on the tabs as originally planned
			//
			if (!hwndFocus)
			{
				hwndFocus = HWindow;
			}

			Parent->PostMessage (WM_NEXTDLGCTL, (WPARAM) hwndFocus, true);
		}
		else
		{
			if (GetFocus () != HWindow)
			{
				Parent->PostMessage (WM_NEXTDLGCTL, (WPARAM) HWindow, true);
			}
		}
	}

	GetApplication ()->PumpWaitingMessages ();
	pages[nSelect].GetDialog ()->Show (SW_SHOW);

	return hwndFocus;
}

HWND TPropertyTab::GetFirstControl ()
{
	// this code causes the focus being set back to the
	// first control of the page-dialog

	HWND	hwndChild 	= ::GetWindow (pages[nSelect].GetDialog ()->HWindow, GW_CHILD);

	if (hwndChild)
	{
		HWND	hwndLast	= ::GetWindow (hwndChild, GW_HWNDLAST);

		if (hwndLast)
		{
			return pages[nSelect].GetDialog ()->GetNextDlgTabItem (hwndLast);
		}
	}

	return NULL;
}

void TPropertyTab::SelectFirstControl ()
{
	HWND	hwndCtrl = GetFirstControl ();

	if (hwndCtrl)
	{
		pages[nSelect].GetDialog ()->PostMessage (WM_NEXTDLGCTL, (WPARAM) hwndCtrl, true);

		SelectEditText (hwndCtrl);
	}
}

// =========================================================
// AssessScroll
//
// works out whether to display scroll arrows or not
// =========================================================
bool TPropertyTab::AssessScroll ()
{
	if (styleTabs & Tab::Stacked)
	{
		xMax = GetClientRect ().right;

		return false;
	}

	TRect	rcTab;
	TRect	rcScroll;
	bool	bHadLeftScroll	= bLeftScroll;
	bool	bHadRightScroll	= bRightScroll;
	bool	bEitherScroll;
	TRect	rcTabs 	= GetClientRect ();
	int		nTabs;

	// should I show the left scroll button ?
	bLeftScroll		= nTabOrigin > 0;

	// work out the last selectable tab
	nTabs = GetLastTab (false);

	// should I show the right scroll button ?
	if (bLeftScroll)
	{
		SetRect (rcTabs);
	}

	GetTabRect (rcTab, nTabs);
	bRightScroll	= rcTab.right > rcTabs.right;

	bEitherScroll	= (bLeftScroll | bRightScroll);

	// get tab area of rightmost VISIBLE tab
	nTabs	= GetEntryCount ();

	while (--nTabs >= 0)
	{
		if (GetTabRect (rcTab, nTabs, 0))
		{
			break;
		}
	}

	// get tab area of rightmost VISIBLE tab
	GetScrollRect (rcScroll);

	xMax			= bEitherScroll ? rcScroll.left - SCROLLMARGIN : GetClientRect ().right;

	if (bEitherScroll)
	{
		// load the left-scroll bitmap
		if (bLeftScroll != bHadLeftScroll || bRightScroll)
		{
			if (bmpLeftArrow)
			{
				delete bmpLeftArrow;
			}

			bmpLeftArrow	= new TBitmap (::LoadBitmap (NULL,
								MAKEINTRESOURCE (bLeftScroll ? OBM_LFARROW : OBM_LFARROWI)),
								AutoDelete);
		}

		// load the right-scroll bitmap
		if (bRightScroll != bHadRightScroll || bLeftScroll)
		{
			if (bmpRightArrow)
			{
				delete bmpRightArrow;
			}

			bmpRightArrow 	= new TBitmap (::LoadBitmap (NULL,
								MAKEINTRESOURCE (bRightScroll ? OBM_RGARROW : OBM_RGARROWI)),
								AutoDelete);
		}
	}
	else
	{
		delete bmpLeftArrow;
		bmpLeftArrow	= NULL;

		delete bmpRightArrow;
		bmpRightArrow 	= NULL;
	}

	// get Windows to redraw the scroll buttons
	if (bLeftScroll != bHadLeftScroll || bRightScroll != bHadRightScroll)
	{
		GetScrollRect (rcScroll);
		rcScroll.top = 0;
		rcScroll.left -= SCROLLMARGIN;
		InvalidateRect (rcScroll);

		ReassessTabWidths ();
	}

	return bEitherScroll;
}

void TPropertyTab::NextOrigin (int nDir)
{
	int	nNew = nTabOrigin;

	do
	{
		nNew += nDir;

		if (nNew >= GetEntryCount () - 1)
		{
			nNew = GetEntryCount () - 1;
			break;
		}

		if (nNew <= 0)
		{
			nNew = 0;

			break;
		}
	}
	while (!IsTabEnabled (nNew));

	if (nNew == nTabOrigin)
	{
		return;
	}

	TRect	rcInvalid;

	SetRect (rcInvalid);
	rcInvalid.bottom += 2;
	InvalidateRect (rcInvalid);

	nTabOrigin = nNew;
	AssessScroll ();

	UpdateWindow ();
}

// =========================================================
// GetScrollRect
//
// get the area of the two scroll buttons
// =========================================================
void TPropertyTab::GetScrollRect (TRect& rcTabs, int nButton)
{
	GetClientRect (rcTabs);

	rcTabs.bottom = nTabHeight;

	rcTabs.top 	= rcTabs.bottom - GetSystemMetrics (SM_CYHSCROLL);
	rcTabs.left = rcTabs.right  - GetSystemMetrics (SM_CXHSCROLL);

	// nButton can be:
	//	0 	- left button
	//	1 	- right button
	//	-1 	- both buttons (default)
	if (nButton != 1)
	{
		if (nButton == 0)
		{
			rcTabs.right = rcTabs.left;
		}

		rcTabs.left -= GetSystemMetrics (SM_CXHSCROLL) - 1;
	}
}

// =========================================================
// EvSetFocus/EvKillFocus
//
// gaining/losing focus event handlers
// =========================================================
void TPropertyTab::EvSetFocus (HWND hwndLostFocus)
{
	TControl::EvSetFocus (hwndLostFocus);

	TRect	rcTab;

	GetTabRect (rcTab, nSelect);
	rcTab.right++;
	rcTab.bottom += 2;

	InvalidateRect (rcTab, false);
}

void TPropertyTab::EvKillFocus (HWND hwndGetFocus)
{
	TControl::EvSetFocus (hwndGetFocus);

	TRect	rcTab;

	GetTabRect (rcTab, nSelect);
	rcTab.right++;
	rcTab.bottom += 2;

	InvalidateRect (rcTab, false);
}

// =========================================================
// EvKeyDown
//
// cursor key handler
// =========================================================
void TPropertyTab::EvKeyDown (UINT key, UINT repeatCount, UINT flags)
{
	// check CONTROL state to prevent reissuing commands
	// already processed by the keyboard hook 
	if (GetKeyState (VK_CONTROL) >= 0)
	{
		switch (key)
		{
			case VK_LEFT :
			case VK_UP :
			case VK_PRIOR :
				SelectNext (-1);
				break;

			case VK_RIGHT :
			case VK_DOWN :
			case VK_NEXT :
				SelectNext (1);
				break;

			default:
				TControl::EvChar (key, repeatCount, flags);
				break;
		}
	}
}

// =========================================================
// EvGetDlgCode
//
// tells dialog box we want to receive cursor keys
// =========================================================
UINT TPropertyTab::EvGetDlgCode (MSG far *msg)
{
	UINT	uiDefault = TControl::EvGetDlgCode (msg);

	return uiDefault | DLGC_WANTARROWS;
}

// =========================================================
// EvCommand
//
// redirect menu (& gadget) messages up to TPropertyDialog
// =========================================================
LRESULT TPropertyTab::EvCommand (UINT id, HWND hWndCtl, UINT notifyCode)
{
	if (!hWndCtl)
	{
		return Parent->EvCommand (id, hWndCtl, notifyCode);
	}

	return TControl::EvCommand (id, hWndCtl, notifyCode);
}

// =========================================================
// SetActiveColour
//
// defines the colour of the active tab
// (only if using Tab::ColorActive style)
// =========================================================
void TPropertyTab::SetActiveColor (TColor color)
{
	colorActive = color;

	if (HWindow)
	{
		TRect	rcTab;

		if (GetTabRect (rcTab, nSelect))
		{
			InvalidateRect (rcTab);
		}
	}
}

// =========================================================
// SetFixedTabWidth
//
// defines the width of tabs when using "FixedWidth" style
// =========================================================
void TPropertyTab::SetFixedTabWidth (int nWidth)
{
	if (nWidth)
	{
		nFixedTabWidth = nWidth;
	}
	else
	{
		nFixedTabWidth = DEFTABWIDTH;
	}

	if (HWindow)
	{
		CalculateTabWidths ();

		TRect	rcTabs;

		SetRect (rcTabs);
		InvalidateRect (rcTabs);
	}
}

// =========================================================
// SetWideMarginWidth
//
// defines the extra space added to the each side of a tab
// defined with the "WideMargin" style
// =========================================================
void TPropertyTab::SetWideMarginWidth (int nWidth)
{
	nWideMarginWidth = nWidth;

	if (HWindow)
	{
		CalculateTabWidths ();

		TRect	rcTabs;

		SetRect (rcTabs);
		InvalidateRect (rcTabs);
	}
}

// =========================================================
// TabColorChange
//
// picks up colors to use from Control Panel
// =========================================================
void TPropertyTab::TabColorChange ()
{
	// set up system colors
	if (bWin95 ||
		GetApplication ()->Ctl3dEnabled () ||
		GetApplication ()->BWCCEnabled ())
	{
		colorWhite  = GetSysColor (COLOR_BTNHIGHLIGHT);
		colorLtGray = GetSysColor (COLOR_BTNFACE);
		colorDkGray = GetSysColor (COLOR_BTNSHADOW);
		colorBlack  = GetSysColor (COLOR_WINDOWFRAME);
	}
	else
	{
		colorWhite  = GetSysColor (COLOR_WINDOWFRAME);
		colorLtGray = GetSysColor (COLOR_WINDOW);
		colorDkGray = GetSysColor (COLOR_WINDOWFRAME);
		colorBlack  = GetSysColor (COLOR_WINDOWFRAME);
	}

	// if the window is already on-screen
	// then redraw it
	if (HWindow)
	{
		Invalidate ();
	}
}
