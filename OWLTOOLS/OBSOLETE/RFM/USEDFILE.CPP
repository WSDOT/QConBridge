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

#include "usedfile.h"


/******************************************************************************
UsedFileManager::UsedFileManager

Description:
   Constructor

Arguements:
   hMenu    - Handle of the file menu
   Section  - ini file section string (include [ & ] in the section name)
   ini      - ini file name
   MaxFiles - number of files to track

Return:
   None
*******************************************************************************/
UsedFileManager::UsedFileManager(HMENU hMenu,const char *Section,const char *ini,int MaxFiles) :
profile(Section,ini)
{
   PRECONDITION(0 < MaxFiles && MaxFiles < 10);

   maxFiles = MaxFiles;

   fileMenu = new TMenu(hMenu);
   menuCount = fileMenu->GetMenuItemCount();

   commandId = new int[maxFiles + 1];
   for (int i = 0; i < maxFiles+1; i++)
      commandId[i] = -1;

   fileNames = new StringArray(maxFiles-1,0,0);

   bypassUpdate = false;
}


/******************************************************************************
UsedFileManager::~UsedFileManager

Description:
   Destructor
   Delete filenames and command ids

Arguements:
   None

Return:
   None
*******************************************************************************/
UsedFileManager::~UsedFileManager()
{
   delete[] commandId;

   fileNames->Flush(TShouldDelete::Delete);
   delete fileNames;

   delete fileMenu;
}


/******************************************************************************
UsedFileManager::SetMenuItemIdentifier

Description:
   Sets the menu item identifier for the filename at pos on the
   file menu.

Arguements:
   pos  - Filename position (1 based)
   item - menu item

Return:
   None
*******************************************************************************/
void UsedFileManager::SetMenuItemIdentifier(int pos,int item)
{
   commandId[pos] = item;
}



/******************************************************************************
UsedFileManager::ReadFromIni

Description:
   Reads the most currently opened files from the ini file

Arguements:
   None

Return:
   None
*******************************************************************************/
void UsedFileManager::ReadFromIni()
{
   char *entries;
   char *entry;
   char file[256];
   int size;

   // Get rid of current list of filenames
   fileNames->Flush(TShouldDelete::Delete);

   // Get entry names from ini file
   size = maxFiles*2 + 2;
   entries = new char[size];
   profile.GetString(0,entries,size);

   // bypass menu updating untill all "last used" filenames are
   // recovered from the ini file.
   bypassUpdate = true;

   // Get the string for each entry
   entry = entries;
   while(*entry)
      {
      profile.GetString(entry,file,sizeof(file));
      AddFilename(file,false);
      entry += strlen(entries)+1;
      }

   // turn bypassUpdate off, and force an update now.
   bypassUpdate = false;
   UpdateFileMenu();

   delete[] entries;
}


/******************************************************************************
UsedFileManager::WriteToIni

Description:
   Writes the most currently opened files to the ini file

Arguements:
   None

Return:
   None
*******************************************************************************/
void UsedFileManager::WriteToIni()
{
   StringIterator i(*fileNames);
   int count = 1;

   while(i)
      {
      string *fileName = i++;

      char entry[2];
      ostrstream(entry,sizeof(entry)) << count << ends;
      count++;
      if (!profile.WriteString(entry,fileName->c_str()))
         THROW(xmsg("Error writing to ini file") );
      }
}


/******************************************************************************
UsedFileManager::AddFilename

Description:
   Adds a new filename to the used file manager's database. The filename will
   be placed at the top of the used file list (it is the most recently used
   file). If the list is full, the least recently used file will be dropped.
   If the filename currently exists in the list, the filename is simply
   brought to the top of the list.

Arguements:
   filename - name of the file to be added to the used file manager

Return:
   None
*******************************************************************************/
void UsedFileManager::AddFilename(const char *filename)
{
   AddFilename(filename,true);
}


/******************************************************************************
UsedFileManager::AddFilename

Description:
   Adds a new filename to the used file manager's database. The filename will
   be placed at the top of the used file list (it is the most recently used
   file). If the list is full, the least recently used file will be dropped.
   If the filename currently exists in the list, the filename is simply
   brought to the top of the list.

Arguements:
   filename - name of the file to be added to the used file manager
   push     - if true, the new name is pushed onto the top of the list
              otherwise it is placed at the end of the list.

Return:
   None
*******************************************************************************/
void UsedFileManager::AddFilename(const char *filename,bool push)
{
   int idx;
   string *fileName;

   // if it is an un-named file, get the heck outta here
   if (strlen(filename) == 0)
      return;

   fileName = new string(filename);
   fileName->to_lower();
   idx = fileNames->Find(fileName);

   if (idx != INT_MAX)
      {
      // The filename already exists in the array.
      // Remove it from the list, so the new filename can be
      // added at the top of the list.
      fileNames->Destroy(idx);
      }

   if (fileNames->IsFull())
      {
      // Array is full, destroy the last item in the array so there
      // will be room for the new filename
      fileNames->Destroy(fileNames->ArraySize() - 1);
      }

   // Add new filename to the start of the array.
   // AddAt pushes the other filenames down to make room for this one.
   if (push)
      fileNames->AddAt(fileName,0);
   else
      fileNames->Add(fileName);

   // Update the menu entires
   UpdateFileMenu();
}


/******************************************************************************
UsedFileManager::GetFilename

Description:
   Retreives a filename corrosponding to a menu item.

Arguements:
   item - menu item

Return:
   filename
*******************************************************************************/
const char *UsedFileManager::GetFilename(int item)
{
   // fileNumber is on a one based indexing system so that the fileNumber
   // corrosponds with the filenumber displayed in the menu.
   string *fileName;

   int i;
   for (i = 1; i <= maxFiles; i++)
      {
      if (commandId[i] == item)
         break;
      }

   fileName = (*fileNames)[i-1];

   return fileName->c_str();
}


/******************************************************************************
UsedFileManager::GetPopupFileMenu

Description:
   Creates a popup menu containing the filenames

Arguements:
   menu - reference to a popup menu

Return:
   None
*******************************************************************************/
void UsedFileManager::GetPopupFileMenu(TPopupMenu &menu) const
{
   if (menu.GetMenuItemCount() > 0)
      menu.AppendMenu(MF_SEPARATOR);

   AppendFileNames(menu);
}


/******************************************************************************
UsedFileManager::UpdateFileMenu

Description:
   Updates the file menu so that the filenames on the menu are in sync with
   the filename list.

Arguements:
   None

Return:
   None
*******************************************************************************/
void UsedFileManager::UpdateFileMenu()
{
   // if we are bypassing the update, get the heck outta here...
   if (bypassUpdate)
      return;

   ClearMenuAdditions();

   // if there aren't any filenames to put on the menu, get the
   // heck outta here.
   if (fileNames->GetItemsInContainer() == 0)
      return;

   fileMenu->AppendMenu(MF_SEPARATOR);
   AppendFileNames(*fileMenu);
}


/******************************************************************************
UsedFileManager::ClearMenuAdditions

Description:
   Clears any additions made to the file menu.

Arguements:
   None

Return:
   None
*******************************************************************************/
void UsedFileManager::ClearMenuAdditions()
{
   // delete the seperator
   fileMenu->DeleteMenu(menuCount,MF_BYPOSITION);

   // delete the filenames
   for (int i = 1; i <= fileNames->GetItemsInContainer(); i++)
      {
      fileMenu->DeleteMenu(commandId[i],MF_BYCOMMAND);
      }
}


/******************************************************************************
UsedFileManager::AppendFilenames

Description:
   Appends the filenames to the supplied menu.

Arguements:
   menu - reference to a menu

Return:
   None
*******************************************************************************/
void UsedFileManager::AppendFileNames(TMenu &menu) const
{
   StringIterator i(*fileNames);
   int count;

   count = 1;
   while(i)
      {
      string *s = i++;

      char buffer[256];

      ostrstream(buffer,sizeof(buffer)) << "&" << count << " "
                                        << s->c_str() << ends;

      if (!menu.AppendMenu(MF_ENABLED,commandId[count],buffer))
         THROW(TMenu::TXMenu(IDS_MENUFAILURE));

      count++;
      }
}
