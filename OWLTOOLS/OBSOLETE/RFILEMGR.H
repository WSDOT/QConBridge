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

#if !defined (RFILEMGR_H_)
#define RFILEMGR_H_

#include <owltools\owltldll.h>
#include <classlib\arrays.h>
#include <cstring.h>
#include <owl\profile.h>
#include <owl\menu.h>
#include <owl\eventhan.h>

class _OWLTOOLSCLASS TRecentFileManager : public TEventHandler
{
public:
   TRecentFileManager(const char *section,const char *ini,int maxFiles = 4);
   ~TRecentFileManager();

   // INI file methods
   void ReadFromINI();
   void WriteToINI();

   void AddFilename(const char *filename,bool push = true);
   void AddFilename(int pos,const char *filename);
   void RemoveFilename(const char *filename);
   void RemoveFilename(int pos);
   int  GetPosition(const char *filename);

protected:
   typedef TIArrayAsVector<string> TIStringArray;
   typedef TIArrayAsVectorIterator<string> TIStringIterator;

   TIStringArray *recentFiles;  // list of recent files
   TProfile *profile;           // ini file profile
   bool updateMenu;             // menu update flag

   void UpdateMenu(TMenu &menu);
   void ClearMenu(TMenu &menu);
   void EvInitMenu (HMENU hMenu);

   void CmOpenRecentFile(uint commandId);
   virtual void OpenRecentFile(const char *fileName);

   DECLARE_RESPONSE_TABLE(TRecentFileManager);
};

#endif