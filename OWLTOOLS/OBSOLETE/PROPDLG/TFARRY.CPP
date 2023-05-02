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

#include "tfarry.h"

static int IsButtonType (TControl *ctrl)
{
	char    szClassName[20];

	//
	// function modified to support BWCC buttons too!
	//
	char    *pszButton = ":BUTTON:BORCHECK:BORRADIO:"; // token names

	// get class surrounded by ':' as token separators
	szClassName[0] = ':';
	::GetClassName (ctrl->HWindow, szClassName + 1, sizeof (szClassName));
	lstrcat (szClassName, ":");

	// convert to upper case
	AnsiUpper (szClassName);

	return (strstr (pszButton, szClassName) != NULL);
}

// =========================================================

TfElement::TfElement (UINT id)
{
	resID   = id;

	wState  = NULL;

	SetTransferBuffer (NULL);
}

void TfElement::GetFromControl (TControl *ctrl)
{
	if (pvTransfer)
	{
		ctrl->Transfer (pvTransfer, tdGetData);
	}
	else
	{
		if (IsButtonType (ctrl))
		{
			SetCheck ((WORD) ctrl->Parent->IsDlgButtonChecked (ctrl->Attr.Id));
		}
		else
		{
			int     nLength     = ctrl->GetWindowTextLength ();
			LPSTR   lpszText    = new char[++nLength];

			ctrl->GetWindowText (lpszText, nLength);
			SetText (lpszText);

			delete[] lpszText;
		}
	}
}

void TfElement::StoreToControl (TControl *ctrl)
{
	if (pvTransfer)
	{
		ctrl->Transfer (pvTransfer, tdSetData);
	}
	else
	{
		if (IsButtonType (ctrl))
		{
			ctrl->Parent->CheckDlgButton (ctrl->Attr.Id, GetCheck ());
		}
		else
		{
			ctrl->SetWindowText (GetText ());
		}
	}
}

// =========================================================

void TfElement::TransferToDlg (TWindow* win, void* param)
{
	if (!win->IsFlagSet (wfTransfer))
	{
		return; // no action
	}

	TControl        *pCtrl      = TYPESAFE_DOWNCAST (win, TControl);

	if (pCtrl == NULL)
	{
		return; // no action
	}

	TTransferArray  *pArray     = (TTransferArray   *) param;

	(*pArray)[win->Attr.Id].StoreToControl (pCtrl);
}

void TfElement::TransferFromDlg (TWindow* win, void* param)
{
	if (!win->IsFlagSet (wfTransfer))
	{
		return; // no action
	}

	TControl        *pCtrl      = TYPESAFE_DOWNCAST (win, TControl);

	if (pCtrl == NULL)
	{
		return; // no action
	}

	TTransferArray  *pArray     = (TTransferArray   *) param;

	(*pArray)[win->Attr.Id].GetFromControl (pCtrl);
}

// =========================================================

TTransferArray::TTransferArray () : arrayElems (10, 0, 10)
{
}

TfElement *TTransferArray::GetElement (UINT resID)
{
	TfElement   elem (resID);
	int			n;

	for (n = 0; n < arrayElems.GetItemsInContainer (); n++)
	{
		if (arrayElems[n] == elem)
		{
			break;
		}
	}

	if (n >= arrayElems.GetItemsInContainer ())
	{
		arrayElems.Add (TfElement (resID));
	}

	TfElement   *elemFound = &arrayElems[n];

	return elemFound;
}

void TTransferArray::TransferData (TWindow *win, TTransferDirection direction)
{
	switch (direction)
	{
		case tdGetData :
		{
			win->ForEach (TfElement::TransferFromDlg, this);
			break;
		}

		case tdSetData :
		{
			win->ForEach (TfElement::TransferToDlg, this);
			break;
		}

		case tdSizeData :
		{
			break;
		}
	}
}

UINT TTransferArray::GetRadioChecked (UINT resIDfrom, UINT resIDto)
{
	for (UINT resID = resIDfrom; resID <= resIDto; resID++)
	{
		if (GetElement (resID)->GetCheck ())
		{
			return resID;
		}
	}

	return NULL;    // none checked
}

