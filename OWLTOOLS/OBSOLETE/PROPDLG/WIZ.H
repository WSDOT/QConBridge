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
// WIZARD.H
// header file for implementation of TWizardDialog and
// TWizardPage
// =========================================================
#ifndef __WIZARDPROPERTY_DIALOG
#define __WIZARDPROPERTY_DIALOG

#include <owltools\propdlg.h>
#include <owltools\bmpctl.h>

class PROPDLGEXPORT TWizardDialog : public TPropertyDialog
{
	bool			bEnableNext;
	bool			bEnablePrev;

public:
	TWizardDialog (TWindow *pParent, TResId resID, Tab::Style style = Tab::UseSmallFont, TModule *module = 0);
	~TWizardDialog ();

	void			SetupWindow ();

	bool			SelectNext ();
	bool			SelectPrevious ();

	void			CmSelectNext ()			{ SelectNext (); }
	void			CmSelectPrevious ()		{ SelectPrevious (); }

	virtual void	AfterSelected (int nTab);

	DECLARE_RESPONSE_TABLE(TWizardDialog);
};

class PROPDLGEXPORT TWizardPage : public TSSPropertyPage
{
	TBitmapControl*	bitmap;
	bool			bHadNext;

public:
	TWizardPage (TResId resID, int idBitmap = NULL, TBitmapControl::Style style = TBitmapControl::Recessed, TModule *module = 0);
	TWizardPage (TPropertyDialog *parent, TResId resID, int idBitmap = NULL, TBitmapControl::Style style = TBitmapControl::Recessed, TModule *module = 0);

	void			SetupWindow ();

	virtual void	AfterSelected ();

	void			CmSelectNext ()			{ SelectNext (); }

	DECLARE_RESPONSE_TABLE(TWizardPage);
};

#endif
