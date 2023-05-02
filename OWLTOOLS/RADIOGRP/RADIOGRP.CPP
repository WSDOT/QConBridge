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

//----------------------------------------------------------------------------
// For use with ObjectWindows 2.5
//   Implementation of class TRadioGroupBox.
//   Group box with automatic transfer of Auto Radiobuttons
//----------------------------------------------------------------------------

// Notes:
// Originally TTipRadioGroupBox was a derived class of TRadioGroupBox. There
// was a virtual method called CreateRadioButton that returned a pointer to
// a radio button object that was called in SetupControls.  SetupControls
// was called from SetupWindow instead of the constructor to preserve the
// virtual behavior.  This worked great for a while but after some time there
// started to be trouble with the radiobuttons not being constructed in time.
// As such I created two seperate classes. Though this is far less efficient
// that one base class and a derived class, it works. At a later date when
// time and duties permit, these class should be once again combined.

#include <owltools\radiogrp.h>

//
// constructor for a TRadioGroupBox object
//
TRadioGroupBox::TRadioGroupBox( TWindow *parent,
                                int resID,
                                int firstRadioID,
                                int lastRadioID,
                                TModule * module):
    TGroupBox(parent,resID,module),
    FromID(firstRadioID),
    ToID(lastRadioID)
{
   SetupControl(parent,module);
}

TRadioGroupBox::~TRadioGroupBox()
{
   for (int id=FromID; id <= ToID; id++)
      delete RadioButton[id-FromID];

   delete [] RadioButton;
}

void TRadioGroupBox::SetupControl(TWindow* parent,TModule* module)
{
  // Assumes that the first radiobutton has a group stop
  PRECONDITION(ToID > FromID); // At least 2 !
  PRECONDITION(ToID - FromID < 100); // Probably bad call

  // Keep list of radiobuttons
  RadioButton=new TRadioButton*[ToID-FromID+1];
  for (int id=FromID; id <= ToID; id++)
     {
     RadioButton[id-FromID]= new TRadioButton(parent,id,this,module);
     RadioButton[id-FromID]->DisableTransfer(); // Because groupbox handles it
     }
  EnableTransfer(); // Let Groupbox handle the transfer
}

uint TRadioGroupBox::Transfer(void* buffer, TTransferDirection direction)
{
  int id;
  if (direction == tdGetData)
  {
    // scan buttons for the first checked
    for (int id=FromID; id<= ToID; id++)
      if ( RadioButton[id-FromID]->GetCheck())
      {
        *(int*)buffer = id; // Set to first(only?) checked
        break;
      }
  }
  else if (direction == tdSetData)
  {
    id=*(int*)buffer; // Transfer buffer holds active button resID
    if (id < FromID || id > ToID)
       id=FromID; // Take care for default=0

    Parent->CheckRadioButton(FromID,ToID,id);
  }
  return sizeof(int);
}


TRadioButton *TRadioGroupBox::GetRadioButton(uint flag,int id)
{
   TRadioButton *btn;
   switch(flag)
      {
      case RGB_BYPOSITION:
           btn = (id < 0 || id > ToID-FromID) ? 0 : RadioButton[id];
           break;

      case RGB_BYCOMMAND:
           btn = (id < FromID || id > ToID) ? 0 : RadioButton[id-FromID];
           break;

      default:
           CHECK(flag != RGB_BYPOSITION && flag != RGB_BYCOMMAND);
      }

   return btn;
}

void TRadioGroupBox::CheckRadioButton(uint flag,int id)
{
   if (flag == RGB_BYPOSITION)
      id += FromID;

   Parent->CheckRadioButton(FromID,ToID,id);
}

int TRadioGroupBox::GetSelRadioButton() const
{
    for (int id=FromID; id<= ToID; id++)
      {
      if ( RadioButton[id-FromID]->GetCheck())
         {
         return id;
         }
      }
    return -1;
}

bool TRadioGroupBox::EnableWindow(bool enable)
{
    for (int id=FromID; id<= ToID; id++)
       EnableRadioButton(RGB_BYCOMMAND,id,enable);

   return TGroupBox::EnableWindow(enable);
}

bool TRadioGroupBox::ShowWindow(int cmdShow)
{
    for (int id=FromID; id<= ToID; id++)
      RadioButton[id-FromID]->ShowWindow(cmdShow);

   return TGroupBox::ShowWindow(cmdShow);
}

bool
TRadioGroupBox::EnableRadioButton(uint flag,int id,bool enable)
{
   TRadioButton* rb = GetRadioButton(flag,id);
   return rb->EnableWindow(enable);
}

bool
TRadioGroupBox::ShowRadioButton(uint flag,int id,int cmdShow)
{
   TRadioButton* rb = GetRadioButton(flag,id);
   return rb->ShowWindow(cmdShow);
}

//
// ----------------------------------------------------
//
TTipRadioGroupBox::TTipRadioGroupBox( TWindow *parent,
                                int resID,
                                int firstRadioID,
                                int lastRadioID,
                                int firstMsgId,
                                TModule * module):
    TGroupBox(parent,resID,module),
    FirstMsgID(firstMsgId),
    FromID(firstRadioID),
    ToID(lastRadioID)
{
   SetupControl(parent,module);
}

TTipRadioGroupBox::~TTipRadioGroupBox()
{
   for (int id=FromID; id <= ToID; id++)
      delete RadioButton[id-FromID];

   delete [] RadioButton;
}

void TTipRadioGroupBox::SetupControl(TWindow* parent,TModule* module)
{
  // Assumes that the first radiobutton has a group stop
  PRECONDITION(ToID > FromID); // At least 2 !
  PRECONDITION(ToID - FromID < 100); // Probably bad call

  // Keep list of radiobuttons
  RadioButton=new TRadioButton*[ToID-FromID+1];
  for (int id=FromID; id <= ToID; id++)
     {
     int msgId = (FirstMsgID < 0) ? id : (FirstMsgID + id - FromID);
     RadioButton[id-FromID]= new TTipRadioButton(parent,id,msgId,this,module);
     RadioButton[id-FromID]->DisableTransfer(); // Because groupbox handles it
     }
  EnableTransfer(); // Let Groupbox handle the transfer
}

uint TTipRadioGroupBox::Transfer(void* buffer, TTransferDirection direction)
{
  int id;
  if (direction == tdGetData)
  {
    // scan buttons for the first checked
    for (int id=FromID; id<= ToID; id++)
      if ( RadioButton[id-FromID]->GetCheck())
      {
        *(int*)buffer = id; // Set to first(only?) checked
        break;
      }
  }
  else if (direction == tdSetData)
  {
    id=*(int*)buffer; // Transfer buffer holds active button resID
    if (id < FromID || id > ToID)
       id=FromID; // Take care for default=0

    Parent->CheckRadioButton(FromID,ToID,id);
  }
  return sizeof(int);
}


TRadioButton *TTipRadioGroupBox::GetRadioButton(uint flag,int id)
{
   TRadioButton *btn;
   switch(flag)
      {
      case RGB_BYPOSITION:
           btn = (id < 0 || id > ToID-FromID) ? 0 : RadioButton[id];
           break;

      case RGB_BYCOMMAND:
           btn = (id < FromID || id > ToID) ? 0 : RadioButton[id-FromID];
           break;

      default:
           CHECK(flag != RGB_BYPOSITION && flag != RGB_BYCOMMAND);
      }

   return btn;
}

void TTipRadioGroupBox::CheckRadioButton(uint flag,int id)
{
   if (flag == RGB_BYPOSITION)
      id += FromID;

   Parent->CheckRadioButton(FromID,ToID,id);
}

int TTipRadioGroupBox::GetSelRadioButton() const
{
    for (int id=FromID; id<= ToID; id++)
      {
      if ( RadioButton[id-FromID]->GetCheck())
         {
         return id;
         }
      }
    return -1;
}

bool TTipRadioGroupBox::EnableWindow(bool enable)
{
    for (int id=FromID; id<= ToID; id++)
       EnableRadioButton(RGB_BYCOMMAND,id,enable);

   return TGroupBox::EnableWindow(enable);
}

bool TTipRadioGroupBox::ShowWindow(int cmdShow)
{
    for (int id=FromID; id<= ToID; id++)
      RadioButton[id-FromID]->ShowWindow(cmdShow);

   return TGroupBox::ShowWindow(cmdShow);
}

bool
TTipRadioGroupBox::EnableRadioButton(uint flag,int id,bool enable)
{
   TRadioButton* rb = GetRadioButton(flag,id);
   return rb->EnableWindow(enable);
}

bool
TTipRadioGroupBox::ShowRadioButton(uint flag,int id,int cmdShow)
{
   TRadioButton* rb = GetRadioButton(flag,id);
   return rb->ShowWindow(cmdShow);
}

