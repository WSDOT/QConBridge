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

#ifndef __TRANSFER_ARRAYS
#define __TRANSFER_ARRAYS

#include <cstring.h>
#include <classlib\arrays.h>
#include <classlib\assoc.h>
#include <owl\dialog.h>
#include <owl\listbox.h>
#include <owl\combobox.h>

#if defined (BUILDPROPDLGDLL)
  #define BUILDTFARRAYDLL
#endif

#if defined (PROPDLGDLL)
  #define TFARRAYDLL
#endif

#if defined (BUILDTFARRAYDLL)
  #define TFARRAYEXPORT	__export
#elif defined (TFARRAYDLL)
  #define TFARRAYEXPORT __import
#else
  #define TFARRAYEXPORT
#endif

class TFARRAYEXPORT TfElement
{
	UINT            resID;
	string          sText;
	WORD            wState;
	void            *pvTransfer;

	void    GetFromControl (TControl *ctrl);
	void    StoreToControl (TControl *ctrl);

public:
	TfElement (UINT id = 0);

	// custom transfer buffer definition
	void    SetTransferBuffer (void *transferBuffer) { pvTransfer = transferBuffer; }

	// functions for setting values of controls
	LPCSTR  GetText ()                  { return sText.c_str (); }
	void    SetText (LPCSTR lpszText)   { sText = (lpszText ? lpszText : ""); }

	// functions for getting values of controls
	WORD    GetCheck ()                 { return wState; }
	void    SetCheck (WORD w)           { wState = w; }

	BOOL    operator== (const TfElement& elem) { return (resID == elem.resID); }

	// functions used to transfer to or from the control
	static void TransferFromDlg (TWindow* win, void* param);
	static void TransferToDlg (TWindow* win, void* param);
};

class TFARRAYEXPORT TTransferArray
{
	TArrayAsVector<TfElement>	arrayElems;

	TfElement*			GetElement (UINT resID);

public:
	TTransferArray ();

	void        TransferData (TWindow *win, TTransferDirection direction);

	TfElement&  operator[] (UINT resID)		{ return *GetElement (resID); }

	UINT        GetRadioChecked (UINT resIDfrom, UINT resIDto);
};

class TFARRAYEXPORT TITransferArray
{
	TTransferArray*	array;

public:
	TITransferArray ()											{ array = NULL; }

	void			SetTransferArray (TTransferArray *_array) 	{ array = _array; }
	TTransferArray*	GetTransferArray ()							{ return array; }
};

#define DECLARE_USETRANSFERARRAY(cls,cls1)              \
  private:                                              \
	TITransferArray	userTransfer;						\
  public:                                               \
	TTransferArray	transfer;							\
	void SetTransferArray (TTransferArray *array)		\
		{                                               \
			userTransfer.SetTransferArray (array);      \
		}                                               \
	void TransferData (TTransferDirection direction)    \
		{                                               \
			if (userTransfer.GetTransferArray ())		\
			{                                           \
				userTransfer.GetTransferArray ()->		\
						TransferData (this, direction);\
			}                                           \
			else                                        \
			{                                           \
				transfer.TransferData (this, direction);\
			}                                           \
		}

class TFARRAYEXPORT TTransferDialog : public TDialog
{

public:
	TTransferDialog (TWindow *wnd, TResId resID, TModule *module = NULL)
		: TDialog (wnd, resID, module) { }

	DECLARE_USETRANSFERARRAY(TTransferDialog, TDialog);
};

#endif
