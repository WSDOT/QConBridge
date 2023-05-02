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

#if !defined(OWLTOOLS_MRUFILES_H_)
#define OWLTOOLS_MRUFILES_H_

#include <owltools\owltldll.h>

#if !defined(OWL_EVENTHAN_H)
# include <owl/eventhan.h>
#endif
class _WSYSCLASS TProfile;
class _WSYSCLASS TRegKey;

//
// Derived classes should catch the following registered message to know
// when an item in the recent list has been selected.
//
#define MruFileMessage "MRUFILEMESSAGE"

// Generic definitions/compiler options (eg. alignment) preceeding the
// definition of classes
#include <services/preclass.h>

//
// class TMRUFiles
// ~~~~~ ~~~~~~~~~~~~
// This class implements a most-recent files list, designed to be mixed-in with
// TApplication.
// The list is appended to the menu with CM_EXIT options.
//
class _OWLTOOLSCLASS TMRUFiles : virtual public TEventHandler
#if defined (BI_NO_RTTI)
  , virtual public TStreamableBase
#endif
{
  public:
    enum { MaxMenuItems = 10 };
    enum { FromIni = 1, FromRegistry = 2 };

    TMRUFiles(const char far* key,int getFrom, int numSavedFiles = MaxMenuItems);
   ~TMRUFiles();

    void          SaveMenuChoice(const char far* text);
    bool          GetMenuText(int id, char far* text, int maxTextLen);
    void          SetMaxMruItems(int max);

  protected:
    void          CeExit(TCommandEnabler& ce);
    void          CmFile(uint id);

    // Do the MRU items already exists in the menu?
    //
    bool          MruItemsInsertedIntoMenu(HMENU hMenu);
    void          RemoveMruItemsFromMenu(HMENU hMenu);
    void          InsertMruItemsToMenu(HMENU hMenu);
    int           GetMenuPos(HMENU hMenu, uint id);
    int           GetExitMenuPos(HMENU hMenu);

    // Profile methods
    //
    int           GetMruCount();
    void          RemoveMruIndex(int index);
    void          AddMruItem(const char far* text);
    bool          ExistMruItem(const char far* text);
    int           GetMruItemIndex(const char far* text);

  private:
    // Read information from the .INI file
    //
    void          Read();

    static uint   MruMessage;
    int           MaxFilesToSave;
    int           GetFrom;
    TProfile*     Profile;
    TRegKey*      RegKey;
    bool          AddedSeparator;
    HMENU         LastHMenu;

  DECLARE_RESPONSE_TABLE(TMRUFiles);
  DECLARE_CASTABLE;
};

// Generic definitions/compiler options (eg. alignment) following the
// definition of classes
#include <services/posclass.h>

#endif  // OWLTOOLS_MRUFILES_H_
