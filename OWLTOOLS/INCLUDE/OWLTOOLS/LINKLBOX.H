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

#if !defined(__linklbox_h)              // Sentry, use file only if it's not already included.
#define __linklbox_h

#include <owltools\owltldll.h>
#include <owl\listbox.h>
#include <owl\button.h>

class _OWLTOOLSCLASS TLinkedLBoxButton;

class _OWLTOOLSCLASS TLinkedLBox : public TListBox {
public:
    TLinkedLBox (TWindow *parent,int resourceId,TModule *module = 0);
    virtual ~TLinkedLBox ();

protected:
    TLinkedLBox *destLBox;
    TLinkedLBoxButton *extractBtn;

public:
    virtual char far* GetClassName ();
    void Attach(TLinkedLBoxButton *btn,TLinkedLBox *lbox);
    void TransferStrings();
    void EvSetFocus();
    void EvDblClick();

protected:
    bool IsMultipleSel();
    void FocusChanged();
    void MoveSelString();
    void MoveSelStrings();

    friend TLinkedLBox;

DECLARE_RESPONSE_TABLE(TLinkedLBox);

};


class _OWLTOOLSCLASS TLinkedLBoxButton : public TButton {
public:
    TLinkedLBoxButton (TWindow *parent,int resourceId,TModule *module = 0);
    virtual ~TLinkedLBoxButton ();

protected:
    TLinkedLBox *srcLBox;

public:
    virtual char far* GetClassName ();
    virtual void SetupWindow();
    void Attach(TLinkedLBox *lbox);
    void BNClicked();

DECLARE_RESPONSE_TABLE(TLinkedLBoxButton);

};

#endif                                      // __linklbox_h sentry.
