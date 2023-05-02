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

#include "propdlg.h"
#include "fontdlg.h"
#include "wizard.h"

#include "test.rh"
#include "fontdlg.rh"
#include "wizard.rh"

// control definitions for address dialog
#include <owl\edit.h>
#include <owl\checkbox.h>
#include <owl\inputdia.h>

#define SHOWPRINTERFONTS		1		// 1=display printer fonts, 0=display screen fonts

// =========================================================
// TestApp
//
// test application object
// =========================================================
class TestApp : public TApplication
{
public:
	TestApp ();
	virtual ~TestApp ();

	virtual void InitMainWindow();
};

// =========================================================
// TMainWindow
//
// main window object
// =========================================================
class TMainWindow : public TFrameWindow
{

public:
	TMainWindow (LPCSTR lpszTitle);

	void		GetWindowClass (WNDCLASS& wc);
	LPSTR		GetClassName ();

	void		TestDialog ();
	void		TestDialog2 ();
	void		FormatDialog ();
	void 		AddressDialog ();
	void		WizardDialog ();
	void		TabTestDialog ();
	void		TabBitmapTest ();
	void		TabBitmapTest2 ();
	void 		EvSysColorChange ();

	DECLARE_RESPONSE_TABLE (TMainWindow);
};

// =========================================================
// TFormatDialog
//
// property dialog for format page
// =========================================================
class TFormatDialog : public TPropertyDialog
{

public:
	TFormatDialog (TWindow *pParent, TResId resID, Tab::Style style);

	void 	CmApplyNow ();

	DECLARE_RESPONSE_TABLE (TFormatDialog);
};

// =========================================================
// TTabTestDialog
//
// property dialog for testing tab sequencing
// =========================================================
class TTabTestDialog : public TPropertyDialog
{
	TEdit*	editTest;

public:
	TTabTestDialog (TWindow *pParent);

	void 	SetupWindow ();
	void	AdjustMargin (TRect& rc);
};

// =========================================================
// TAddressDialog, TAddressPage
//
// overriden property dialogs
// =========================================================
class TAddressPage : public TPropertyPage
{
	int				nLastSaved;

public:
	TAddressPage (TResId resID, TModule *module = 0);

	BOOL			SaveData ();
	void			LoadData (int nTab);
};

class TAddressDialog : public TPropertyDialog
{
	TAddressPage	*Page;
	TTransferArray	transfer[26];

public:
	TAddressDialog (TWindow *pParent, TResId resID, Tab::Style style);

	virtual void	AfterSelected (int nTab);

	TTransferArray&	GetTransferArray (int nTab)	{ return transfer[nTab]; }
};

// =========================================================
// TBasicPropertyPage, TFirstPropertyPage, TSecondPropertyPage etc..
//
// overriden property pages
// =========================================================
class TBasicPropertyPage : public TPropertyPage
{

public:
	TBasicPropertyPage (TResId res);

	void	CmPrevious ();
	void	CmNext ();

	DECLARE_RESPONSE_TABLE (TBasicPropertyPage);
};

class TFirstPropertyPage : public TBasicPropertyPage
{

public:
	TFirstPropertyPage (TResId res)
		: TBasicPropertyPage (res) { }
};

class TSecondPropertyPage : public TBasicPropertyPage
{
	TEdit*	editTest;

public:
	TSecondPropertyPage (TResId res);

	BOOL	BeforeLeaving ();

	void	CmChangeText ();

	DECLARE_RESPONSE_TABLE (TSecondPropertyPage);
};

class TThirdPropertyPage : public TBasicPropertyPage
{

public:
	TThirdPropertyPage (TResId res)
		: TBasicPropertyPage (res) { }

	void	SetupWindow ();
	void	CmCheckClicked ();

	DECLARE_RESPONSE_TABLE (TThirdPropertyPage);
};

class TFourthPropertyPage : public TBasicPropertyPage
{

public:
	TFourthPropertyPage (TResId res)
		: TBasicPropertyPage (res) { }

	void	CmTestForEach ();

	DECLARE_RESPONSE_TABLE (TFourthPropertyPage);
};

class TFifthPropertyPage : public TBasicPropertyPage
{

public:
	TFifthPropertyPage (TResId res)
		: TBasicPropertyPage (res) { }
};

class TSixthPropertyPage : public TBasicPropertyPage
{

public:
	TSixthPropertyPage (TResId res)
		: TBasicPropertyPage (res) { }
};

// =========================================================

TestApp::TestApp () : TApplication("Test System")
{
	EnableCtl3d (TRUE);
}

TestApp::~TestApp ()
{
}

void TestApp::InitMainWindow ()
{
	SetMainWindow (new TMainWindow (GetName ()));
}

// =========================================================

DEFINE_RESPONSE_TABLE1(TMainWindow, TFrameWindow)
  EV_WM_SYSCOLORCHANGE,
  EV_COMMAND(CM_TEST, TestDialog),
  EV_COMMAND(CM_TEST2, TestDialog2),
  EV_COMMAND(CM_FORMAT, FormatDialog),
  EV_COMMAND(CM_ADDRESS, AddressDialog),
  EV_COMMAND(CM_WIZARD, WizardDialog),
  EV_COMMAND(CM_TABTEST, TabTestDialog),
  EV_COMMAND(CM_TABBITMAPDIALOG, TabBitmapTest),
  EV_COMMAND(CM_TABBITMAPDIALOG2, TabBitmapTest2),
END_RESPONSE_TABLE;

TMainWindow::TMainWindow (LPCSTR lpszTitle)
	: TFrameWindow (NULL, lpszTitle)
{
}

void TMainWindow::GetWindowClass (WNDCLASS& wc)
{
	TFrameWindow::GetWindowClass (wc);

	wc.lpszMenuName = MAKEINTRESOURCE (MENU_1);
}

LPSTR TMainWindow::GetClassName ()
{
	return "Test:Main";
}

void TMainWindow::EvSysColorChange ()
{
	// get TPropertyDialog to update the tab and dialog colors
	TPropertyDialog::TabColorChange (GetApplication ());
}

void TMainWindow::TestDialog ()
{
	TPropertyDialog	Property (this, IDD_TEST,
									Tab::SingleHeight |
									Tab::Stacked |
									Tab::CreateOnDemand |
									Tab::ButtonsOnRight |
									Tab::CenterDialog |
									Tab::ColorActive);

	Property.Add ("&First page", 		new TFirstPropertyPage (CHILD_1));
	Property.Add ("&Another page",		new TSecondPropertyPage (CHILD_2), FALSE);
	Property.Add ("&Yet another page", 	new TThirdPropertyPage (CHILD_3));
	Property.Add ("&Even more pages", 	new TFourthPropertyPage (CHILD_4));
	Property.Add ("New stacked page",	new TFifthPropertyPage (CHILD_5));
	Property.Add ("Fina&l page",		new TSixthPropertyPage (CHILD_5));

	Property.Execute ();
}

void TMainWindow::TabBitmapTest ()
{
	TPropertyDialog	Property (this, IDD_TABBITMAPTEST,
									Tab::SingleHeight |
									Tab::Stacked |
									Tab::ButtonsOnRight |
									Tab::UseTabBitmaps);

	Property.Add ("&Red", 		new TBasicPropertyPage (CHILD_6));
	Property.Add ("&Green",		new TBasicPropertyPage (CHILD_6));
	Property.Add ("&Yellow", 	new TBasicPropertyPage (CHILD_6));
	Property.Add ("&Blue", 		new TBasicPropertyPage (CHILD_6));
	Property.Add ("&Purple",	new TBasicPropertyPage (CHILD_6));
	Property.Add ("&Cyan",		new TBasicPropertyPage (CHILD_6));

	Property.Execute ();
}

void TMainWindow::TabBitmapTest2 ()
{
	TPropertyDialog	Property (this, IDD_TABBITMAPTEST,
									Tab::SingleHeight |
									Tab::Stacked |
									Tab::ButtonsOnRight |
									Tab::UseTabBitmaps |
									Tab::CreateOnDemandAndKeepIt,
									IBM_TABBITMAP, TRUE);

	Property.Add ("&System", 		new TBasicPropertyPage (CHILD_6));
	Property.Add ("Date/&Time",		new TBasicPropertyPage (CHILD_6));
	Property.Add ("&Add Hardware", 	new TBasicPropertyPage (CHILD_6));
	Property.Add ("&Display", 		new TBasicPropertyPage (CHILD_6));
	Property.Add ("&Mouse",			new TBasicPropertyPage (CHILD_6));
	Property.Add ("Add &Programs",	new TBasicPropertyPage (CHILD_6));

	Property.Execute ();
}

void TMainWindow::TestDialog2 ()
{
	TPropertyDialog	Property (this, IDD_TEST,
									Tab::DoubleHeight |
									Tab::Collapsing |
									Tab::Justified |
									Tab::ButtonsOnRight);

	Property.Add ("First\r\npage", 				new TFirstPropertyPage (CHILD_1));
	Property.Add ("Second (and wide)\r\npage",	new TSecondPropertyPage (CHILD_2), FALSE);
	Property.Add ("Third\r\npage", 				new TThirdPropertyPage (CHILD_3));
	Property.Add ("Fourth\r\npage (the widest)",new TFourthPropertyPage (CHILD_4));
	Property.Add ("Fifth-and-final", 			new TFifthPropertyPage (CHILD_5));

	Property.Execute ();
}

void TMainWindow::FormatDialog ()
{
	TFormatDialog		Format (this, IDD_FORMATDLG, Tab::SingleHeight);
#if SHOWPRINTERFONTS
	TPrinter			printer;
	TPrinterFontData	fontdata (printer);
#else
	TFontData	fontdata;
#endif

	fontdata.Flags	|= CF_INITTOLOGFONTSTRUCT;
	fontdata.PointSize = 240;

	lstrcpy (fontdata.LogFont.lfFaceName, "Arial");
	fontdata.LogFont.lfWeight		= FW_BOLD;
	fontdata.LogFont.lfUnderline	= TRUE;

	fontdata.Flags	|= CF_LIMITSIZE;
	fontdata.SizeMin	= 12;
	fontdata.SizeMax	= 36;

	Format.Add ("Font", 		new TFontPropertyPage (IDD_FONTDLG, fontdata));
	Format.Add ("Another page",	new TFirstPropertyPage (CHILD_2), TRUE);

	Format.Execute ();
}

void TMainWindow::AddressDialog ()
{
	TAddressDialog	Address (this, IDD_ADDRESSTEST,
			Tab::SingleHeight 	|
			Tab::Stacked 		|
			Tab::FixedWidth 	|
			Tab::AllowDupPages 	|
			Tab::UseSmallFont);

	Address.SetFixedTabWidth (30);

	Address.Execute ();
}

void TMainWindow::WizardDialog ()
{
	TWizardDialog	Wizard (this, IDD_WIZARDDLG);

	Wizard.Add ("test #1", new TWizardPage (WIZARDTEST, IBM_WIZARDTEST));
	Wizard.Add ("test #2", new TWizardPage (CHILD_1));
	Wizard.Add ("test #3", new TWizardPage (CHILD_5));
	Wizard.Add ("test #4", new TWizardPage (CHILD_1));
	Wizard.Add ("test #5", new TWizardPage (CHILD_5));
	Wizard.Add ("test #6", new TWizardPage (CHILD_1));
	Wizard.Add ("test #7", new TWizardPage (CHILD_5));

	Wizard.Execute ();
}

void TMainWindow::TabTestDialog ()
{
	TTabTestDialog (this).Execute ();
}

// =========================================================
// TFormatDialog
//
// property dialog for format page
// =========================================================
DEFINE_RESPONSE_TABLE1 (TFormatDialog, TPropertyDialog)
	EV_BN_CLICKED (IDC_APPLYNOW, CmApplyNow),
END_RESPONSE_TABLE;

TFormatDialog::TFormatDialog (TWindow *pParent, TResId resID, Tab::Style style)
	: TPropertyDialog (pParent, resID, style)
{
}

void TFormatDialog::CmApplyNow ()
{
	MessageBox ("\'Apply Now\' button pressed", "Format", MB_OK | MB_ICONINFORMATION);
}

// =========================================================
// TAddressDialog, TAddressPage
//
// overriden property dialogs for address book example
// =========================================================
TAddressDialog::TAddressDialog (TWindow *pParent, TResId resID, Tab::Style style)
	: 	TPropertyDialog (pParent, resID, style | Tab::FocusOnFirstControl)
{
	Page = new TAddressPage (IDD_ADDRESS);

	char	szTitle[2];

	szTitle[1] = '\0';

	for (char cLetter = 'A'; cLetter <= 'Z'; cLetter++)
	{
		szTitle[0] = cLetter;
		Add (szTitle, Page);
	}

	transfer[0][IDC_EDIT1].SetText ("Hello World");
	transfer[1][IDC_EDIT2].SetText ("Address on page 2");
	transfer[2][IDC_EDIT2].SetText ("Address on page 3");
	transfer[2][IDC_CHECKBOX1].SetCheck (1);	// set the checkbox on page 3
}

void TAddressDialog::AfterSelected (int nTab)
{
	Page->SaveData ();	// ask page to save its current data

	Page->LoadData (nTab);
}

TAddressPage::TAddressPage (TResId resID, TModule *module)
	: TPropertyPage (resID, module)
{
	new TEdit (this, IDC_EDIT1);
	new TEdit (this, IDC_EDIT2);
	new TCheckBox (this, IDC_CHECKBOX1);
	
	nLastSaved = -1;
}

BOOL TAddressPage::SaveData ()
{
	if (nLastSaved == -1)
	{
		return TRUE;
	}

	// save away the fields here
	dynamic_cast<TAddressDialog &> (GetPropertyDialog ()).GetTransferArray (nLastSaved).TransferData (this, tdGetData);

	nLastSaved = -1;

	return TRUE;
}

void TAddressPage::LoadData (int nTab)
{
	SaveData ();

	nLastSaved = nTab;

	// load the new fields here
	dynamic_cast<TAddressDialog &> (GetPropertyDialog ()).GetTransferArray (nTab).TransferData (this, tdSetData);
}

// =========================================================
// TBasicPropertyPage
//
// overriden property page to provide Next/Previous
// button functionality
// =========================================================
DEFINE_RESPONSE_TABLE1(TBasicPropertyPage, TPropertyPage)
  EV_BN_CLICKED(IDC_PREVIOUS, CmPrevious),
  EV_BN_CLICKED(IDC_NEXT, CmNext),
//  EV_BN_CLICKED(IDC_CHECKBOX1, CmCheckClicked),
//  EV_BN_CLICKED(IDC_TEST, CmTestForEach),
END_RESPONSE_TABLE;

TBasicPropertyPage::TBasicPropertyPage (TResId res)
	: TPropertyPage (res)
{
}

void TBasicPropertyPage::CmPrevious ()
{
	SelectPrevious ();
}

void TBasicPropertyPage::CmNext ()
{
	SelectNext ();
}

// ==========================================================
// TSecondPropertyPage
//
// overriden property page to test the BeforeLeaving function
// ==========================================================
DEFINE_RESPONSE_TABLE1(TSecondPropertyPage, TBasicPropertyPage)
  EV_BN_CLICKED(IDC_CHANGETEXT, CmChangeText),
END_RESPONSE_TABLE;

TSecondPropertyPage::TSecondPropertyPage (TResId res)
	: TBasicPropertyPage (res)
{
	editTest = new TEdit (this, IDC_EDIT1, 20);
}

BOOL TSecondPropertyPage::BeforeLeaving ()
{
	char	szBuffer[20];

	editTest->GetText (szBuffer, sizeof (szBuffer));

	if (lstrcmpi (szBuffer, "HELLO") != 0)
	{
		MessageBox ("Please type \'Hello\' in the edit box",
					"BeforeLeaving sample",
					MB_OK | MB_ICONEXCLAMATION);

		PostMessage (WM_NEXTDLGCTL, (WPARAM) GetDlgItem (IDC_EDIT1), TRUE);

		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

void TSecondPropertyPage::CmChangeText ()
{
	char	szNewText[100];

	lstrcpy (szNewText, GetTabText ().c_str ());

	if (TInputDialog (GetParentWindow (), "Change Tab Text", "Text", szNewText, sizeof (szNewText)).Execute () == IDOK)
	{
		SetTabText (CURRENTPAGE, szNewText);
	}
}

// =========================================================
// TThirdPropertyPage
//
// overriden property page to catch the
// "Enable second page" checkbox
// =========================================================
DEFINE_RESPONSE_TABLE1(TThirdPropertyPage, TBasicPropertyPage)
  EV_BN_CLICKED(IDC_CHECKBOX1, CmCheckClicked),
END_RESPONSE_TABLE;

void TThirdPropertyPage::SetupWindow ()
{
	TBasicPropertyPage::SetupWindow ();

	CheckDlgButton (IDC_CHECKBOX1, IsTabEnabled (1));
}

void TThirdPropertyPage::CmCheckClicked ()
{
	EnableTab (1, IsDlgButtonChecked (IDC_CHECKBOX1));
}

// =========================================================
// TFourthPropertyPage
//
// overriden property page which implements the ForEachPage
// function (in this case to set the background of the tab
// to green, purely as an example)
// =========================================================
DEFINE_RESPONSE_TABLE1(TFourthPropertyPage, TBasicPropertyPage)
  EV_BN_CLICKED(IDC_TEST, CmTestForEach),
END_RESPONSE_TABLE;

void SetPageColour (TPropertyPage *pPage, void *param)
{
	pPage->SetBkgndColor ((COLORREF) param);

	if (pPage->IsWindowVisible ())
	{
		pPage->Invalidate ();
	}
}

void TFourthPropertyPage::CmTestForEach ()
{
	GetPropertyDialog ().ForEachPage (SetPageColour, (void *) RGB (0, 255, 0));
}

// =========================================================

TTabTestDialog::TTabTestDialog (TWindow *pParent)
	: TPropertyDialog (pParent, IDD_TABTEST)
{
	editTest = new TEdit (this, IDC_TESTEDIT);

	Add ("&First page", 		new TFirstPropertyPage (CHILD_1));
	Add ("&Another page",		new TSecondPropertyPage (CHILD_2), FALSE);
	Add ("&Yet another page", 	new TThirdPropertyPage (CHILD_3));
	Add ("Fina&l page",			new TFourthPropertyPage (CHILD_5));
}

void TTabTestDialog::SetupWindow ()
{
	TPropertyDialog::SetupWindow ();

    MoveTabsAfterControl (editTest);
}

void TTabTestDialog::AdjustMargin (TRect& rc)
{
	TPropertyDialog::AdjustMargin (rc);

	TRect	rcEdit;

	::GetWindowRect (editTest->HWindow, &rcEdit);

	ScreenToClient (rcEdit.BottomRight ());

	rc.top = rcEdit.bottom + 6;
}

// =========================================================

int OwlMain (int , char* [])
{
	TestApp		App;
	int			result;

	result = App.Run();

	return result;
}