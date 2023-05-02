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

#include <owl\button.h>

#include <owltools\wiz.h>
#include <owltools\proptab.h>
#include <owltools\wizard.rh>

// =========================================================
// TWizardDialog
//
// base class for wizard dialogs (page holder)
// =========================================================

DEFINE_RESPONSE_TABLE1(TWizardDialog, TPropertyDialog)
	EV_BN_CLICKED (IDC_WIZ_BACK, CmSelectPrevious),
	EV_BN_CLICKED (IDC_WIZ_NEXT, CmSelectNext),
END_RESPONSE_TABLE;

TWizardDialog::TWizardDialog (TWindow *pParent, TResId resID, Tab::Style style, TModule *module)
	:	TPropertyDialog (pParent, resID, style | Tab::WizardFrame, module)
{
	bEnableNext = bEnablePrev = true;
}

TWizardDialog::~TWizardDialog ()
{
}

void TWizardDialog::SetupWindow ()
{
	TPropertyDialog::SetupWindow ();

	HWND hwndNext = GetDlgItem (IDC_WIZ_NEXT);

	if (hwndNext != NULL)
	{
		PostMessage (WM_NEXTDLGCTL, (WPARAM) hwndNext, true);
	}

}

bool TWizardDialog::SelectNext ()
{
	// only allow next to be selected if we
	// aren't already on the last
	if (bEnableNext)
	{
		return TPropertyDialog::SelectNext ();
	}
	else
	{
		return false;
	}
}

bool TWizardDialog::SelectPrevious ()
{
	// only allow previous to be selected if we
	// aren't already on the first
	if (bEnablePrev)
	{
		return TPropertyDialog::SelectPrevious ();
	}
	else
	{
		return false;
	}
}

static bool IsLastTab (int nTab, TPropertyDialog& dlg)
{
	while (++nTab < dlg.GetEntryCount ())
	{
		if (dlg.GetPageRef (nTab).IsTabEnabled ())
		{
			return false;
		}
	}

	return true;
}

void TWizardDialog::AfterSelected (int nTab)
{
	// enable navigation buttons as necessary...

	// determine if this is the first enabled button
	bool	bFirstTab 	= true;
	int		selId		= 0;

	for (int n = 0; n < nTab; n++)
	{
		if (GetPageRef (n).IsTabEnabled ())
		{
			bFirstTab = false;
			break;
		}

	}

	HWND hwndBackBtn	= GetDlgItem (IDC_WIZ_BACK);
	HWND hwndNextBtn 	= GetDlgItem (IDC_WIZ_NEXT);
	HWND hwndOKBtn 		= GetDlgItem (IDOK);

	if (hwndBackBtn)
	{
		bEnablePrev = !bFirstTab;

		::EnableWindow (hwndBackBtn, bEnablePrev);
	}

	// determine if this is the last enabled button
	bool	bLastTab = IsLastTab (nTab, *this);

	if (hwndNextBtn)
	{
		if (!bLastTab)
		{
			SetDefaultId (selId = IDC_WIZ_NEXT);
		}

		bEnableNext = !bLastTab;

		::EnableWindow (GetDlgItem (IDC_WIZ_NEXT), bEnableNext);
	}

	if (hwndOKBtn)
	{
		::EnableWindow (GetDlgItem (IDOK), bLastTab);
	}

	if (bLastTab && hwndOKBtn)
	{
		SetDefaultId (selId = IDOK);
	}

	if (selId)
	{
		PostMessage (WM_NEXTDLGCTL, (WPARAM) GetDlgItem (selId), true);
	}
	else
	{
		pTabs->SelectFirstControl ();
	}
}

// =========================================================
// TWizardPage
//
// base class for wizard pages
// =========================================================

DEFINE_RESPONSE_TABLE1(TWizardPage, TSSPropertyPage)
	EV_BN_CLICKED (IDC_WIZ_NEXT, CmSelectNext),
END_RESPONSE_TABLE;

TWizardPage::TWizardPage (TResId resID, int idBitmap, TBitmapControl::Style style, TModule *module)
	: TSSPropertyPage (resID, module)
{
	if (idBitmap)
	{
		bitmap = new TBitmapControl (this, idBitmap, style, module);
	}
	else
	{
		bitmap = NULL;
	}
}

TWizardPage::TWizardPage (TPropertyDialog *parent, TResId resID, int idBitmap, TBitmapControl::Style style, TModule *module)
	: TSSPropertyPage (parent, resID, module)
{
	if (idBitmap)
	{
		bitmap = new TBitmapControl (this, idBitmap, style, module);
	}
	else
	{
		bitmap = NULL;
	}
}

void TWizardPage::SetupWindow ()
{
	TSSPropertyPage::SetupWindow ();

	bHadNext = (GetDlgItem (IDC_WIZ_NEXT) != NULL);

	if (!bHadNext)
	{
		TButton* nextButton = new TButton (this, IDC_WIZ_NEXT, "Dummy Next", 0, 0, 60, 30, true);

		nextButton->Attr.Style &= ~WS_VISIBLE;
		nextButton->Create ();
	}
}

void TWizardPage::AfterSelected ()
{
	if (!bHadNext)
	{
		bool bLastTab = IsLastTab (ThisTab (), GetPropertyDialog ());

		SetDefaultId (bLastTab ? IDOK : IDC_WIZ_NEXT);
	}
}
