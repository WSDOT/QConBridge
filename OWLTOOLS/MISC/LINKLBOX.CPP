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


#include <owltools\linklbox.h>

DEFINE_RESPONSE_TABLE1(TLinkedLBox, TListBox)
    EV_NOTIFY_AT_CHILD(LBN_SETFOCUS,EvSetFocus),
    EV_NOTIFY_AT_CHILD(LBN_DBLCLK,EvDblClick),
END_RESPONSE_TABLE;

TLinkedLBox::TLinkedLBox(TWindow *parent,int resourceId,TModule *module) :
    TListBox(parent,resourceId,module)
{
   Attr.Style &= ~LBS_NOTIFY;
   extractBtn = NULL;
   destLBox = NULL;
}

TLinkedLBox::~TLinkedLBox()
{
   extractBtn = NULL;
   destLBox = NULL;
}

char far* TLinkedLBox::GetClassName ()
{
    return "LINKEDLBOX";
}

void TLinkedLBox::Attach(TLinkedLBoxButton *btn,TLinkedLBox *lbox)
{
   extractBtn = btn;
   destLBox = lbox;

   extractBtn->EnableWindow(false);
}

bool TLinkedLBox::IsMultipleSel()
{
   return ((Attr.Style & LBS_MULTIPLESEL) == LBS_MULTIPLESEL);
}


void TLinkedLBox::EvSetFocus()
{
   destLBox->FocusChanged();
   extractBtn->EnableWindow(true);
}

void TLinkedLBox::EvDblClick()
{
   TransferStrings();
}

void TLinkedLBox::FocusChanged()
{
   if (IsMultipleSel())
      SetSelIndexes(0,-1,false);
   else
      SetSelIndex(-1);

   extractBtn->EnableWindow(false);
}


void TLinkedLBox::TransferStrings()
{
    if (IsMultipleSel())
       MoveSelStrings();
    else
       MoveSelString();
}

void TLinkedLBox::MoveSelStrings()
{
    int count;
    int *idx;
    char far **string;
    int strLen, maxStrLen;
    int i;

    count = GetSelCount();
    idx = new int[count];
    GetSelIndexes(idx,count);
    strLen = 0;
    maxStrLen = 0;
    for (i = 0; i < count; i++)
       {
       strLen = GetStringLen(idx[i]);
       maxStrLen = max(maxStrLen,strLen);
       }

    string = new char far*[count];
    for (i = 0; i < count; i++)
       {
       string[i] = new char far[maxStrLen];
       }
    GetSelStrings(string,count,maxStrLen);

    for (i = 0; i < count; i++)
       {
       DeleteString(idx[i]-i);
       destLBox->AddString(string[i]);
       }

    for (i = 0; i < count; i++)
       delete[] string[i];
    delete[] string;
    delete[] idx;
}


void TLinkedLBox::MoveSelString()
{
    int idx;
    char *string;
    int strLen;

    idx = GetSelIndex();
    if (idx < 0)
       return;
    strLen = GetStringLen(idx);
    string = new char[strLen];
    GetSelString(string,strLen);

    DeleteString(idx);
    destLBox->AddString(string);

    delete[] string;
}

//
//
//
DEFINE_RESPONSE_TABLE1(TLinkedLBoxButton, TButton)
    EV_NOTIFY_AT_CHILD(BN_CLICKED,BNClicked),
END_RESPONSE_TABLE;

TLinkedLBoxButton::TLinkedLBoxButton(TWindow *parent,int resourceId,TModule *module) :
TButton(parent,resourceId,module)
{
   srcLBox = NULL;
}

TLinkedLBoxButton::~TLinkedLBoxButton()
{
   srcLBox = NULL;
}

void TLinkedLBoxButton::SetupWindow()
{
   TButton::SetupWindow();

   EnableWindow(false);
}

char far* TLinkedLBoxButton::GetClassName ()
{
    return "LINKEDLBOXBUTTON";
}

void TLinkedLBoxButton::Attach(TLinkedLBox *lbox)
{
   srcLBox = lbox;
}

void TLinkedLBoxButton::BNClicked()
{
   srcLBox->TransferStrings();
   EnableWindow(false);
}
