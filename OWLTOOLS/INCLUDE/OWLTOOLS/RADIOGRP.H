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
// Add-on for ObjectWindows 2.5
//   Defines class TRadioGroupBox.
//   Group box with automatic transfer of Auto Radiobuttons
//----------------------------------------------------------------------------
#if !defined OWLTOOLS_RADIOGROUP_H
#define OWLTOOLS_RADIOGROUP_H

#include <owltools\owltldll.h>
#include <owl\groupbox.h>
#include <owl\radiobut.h>
#include <owltools\tiprbtn.h>

#define RGB_BYPOSITION 1
#define RGB_BYCOMMAND  2

//
//  class TRadioGroupBox
//  ----- --------------
//
class _OWLTOOLSCLASS TRadioGroupBox: public TGroupBox
{
public:
    TRadioGroupBox(TWindow *parent,
                   int resID,         // Groupbox ID
                   int firstRadioID,  // First ID in Group
                   int lastRadioID,   // last included(!) ID in Group
                   TModule *module=0);
    ~TRadioGroupBox();

    uint Transfer(void* buffer, TTransferDirection direction);
    TRadioButton *GetRadioButton(uint flag,int id);
    void CheckRadioButton(uint flag,int id);
    int GetNumButtons() const { return ToID - FromID + 1; }
    int GetSelRadioButton() const;
    bool EnableWindow(bool enable);
    bool ShowWindow(int cmdShow);
    bool EnableRadioButton(uint flag,int id,bool enabled);
    bool ShowRadioButton(uint flag,int id,int cmdShow);

  protected:
    int
      FromID,  // First in a sequence of radio buttons
      ToID;    // Last in a sequence of radio buttons
      TRadioButton ** RadioButton; // List of radio buttons ptrs

  private:
    //
    // hidden to prevent accidental copying or assignment
    //
    TRadioGroupBox(const TRadioGroupBox&);
    TRadioGroupBox& operator=(const TRadioGroupBox&);

    void SetupControl(TWindow *parent,TModule *module);

  // Streaming not implemented !
  //DECLARE_STREAMABLE(_OWLTOOLSCLASS, TRadioGroupBox, 1);
};

class _OWLTOOLSCLASS TTipRadioGroupBox: public TGroupBox
{
public:
    TTipRadioGroupBox(TWindow *parent,
                      int resID,         // Groupbox ID
                      int firstRadioID,  // First ID in Group
                      int lastRadioID,   // last included(!) ID in Group
                      int firstMsgID = -1,
                      TModule *module=0);

    ~TTipRadioGroupBox();

    uint Transfer(void* buffer, TTransferDirection direction);
    TRadioButton *GetRadioButton(uint flag,int id);
    void CheckRadioButton(uint flag,int id);
    int GetNumButtons() const { return ToID - FromID + 1; }
    int GetSelRadioButton() const;
    bool EnableWindow(bool enable);
    bool ShowWindow(int cmdShow);
    bool EnableRadioButton(uint flag,int id,bool enabled);
    bool ShowRadioButton(uint flag,int id,int cmdShow);

  protected:
    int
      FirstMsgID, // First in a sequence of messages
      FromID,  // First in a sequence of radio buttons
      ToID;    // Last in a sequence of radio buttons
      TRadioButton ** RadioButton; // List of radio buttons ptrs

  private:
    //
    // hidden to prevent accidental copying or assignment
    //
    TTipRadioGroupBox(const TRadioGroupBox&);
    TTipRadioGroupBox& operator=(const TRadioGroupBox&);

    void SetupControl(TWindow *parent,TModule *module);

  // Streaming not implemented !
  //DECLARE_STREAMABLE(_OWLTOOLSCLASS, TTipRadioGroupBox, 1);
};

#endif  // __OWL_GROUPBOX_H

