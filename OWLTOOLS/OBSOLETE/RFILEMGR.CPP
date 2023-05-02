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


#include <owltools\rfilemgr.h>
#include <owl\windowev.h>
#include <owl\window.rh>
#include <dir.h>
#include <owltools\rfilemgr.rh>

DEFINE_RESPONSE_TABLE(TRecentFileManager)
   EV_WM_INITMENU,
   EV_COMMAND_AND_ID(CM_OPENRECENTFILE+0,CmOpenRecentFile),
   EV_COMMAND_AND_ID(CM_OPENRECENTFILE+1,CmOpenRecentFile),
   EV_COMMAND_AND_ID(CM_OPENRECENTFILE+2,CmOpenRecentFile),
   EV_COMMAND_AND_ID(CM_OPENRECENTFILE+3,CmOpenRecentFile),
   EV_COMMAND_AND_ID(CM_OPENRECENTFILE+4,CmOpenRecentFile),
   EV_COMMAND_AND_ID(CM_OPENRECENTFILE+5,CmOpenRecentFile),
   EV_COMMAND_AND_ID(CM_OPENRECENTFILE+6,CmOpenRecentFile),
   EV_COMMAND_AND_ID(CM_OPENRECENTFILE+7,CmOpenRecentFile),
   EV_COMMAND_AND_ID(CM_OPENRECENTFILE+8,CmOpenRecentFile),
   EV_COMMAND_AND_ID(CM_OPENRECENTFILE+9,CmOpenRecentFile),
END_RESPONSE_TABLE;


/******************************************************************************
TRecentFileManager

Description:
   Constructor

Arguements:
   section  - ini file section string (don't use the [ & ] in the section name)
   ini      - ini file name
   maxFiles - number of files to track

Return:
   None
*******************************************************************************/
TRecentFileManager::TRecentFileManager(const char *section,const char *ini,int maxFiles)
{
    if (maxFiles <= 0)
       maxFiles = 1;

    if (maxFiles > 9)
       maxFiles = 9;

    recentFiles = new TIStringArray(maxFiles-1,0,0);
    profile = new TProfile(section,ini);

    updateMenu = false;
}

/******************************************************************************
~TRecentFileManager

Description:
   Destructor.

Arguements:
   None

Return:
   None
*******************************************************************************/
TRecentFileManager::~TRecentFileManager()
{
    delete recentFiles;
    delete profile;
}

/******************************************************************************
ReadFromINI

Description:
   Reads the filenames from the ini file. Filenames must be contained in
   the section defined in the c'tor. This section of the ini file must have
   the following format:

   [RecentFileSection]
   1=c:\dir\file1.dat
   2=c:\dir\file2.dat
   ...
   n=c:\dir\filen.dat

   The first maxFiles is read from the ini file, all others are ignored

Arguements:
   None

Return:
   None
*******************************************************************************/
void TRecentFileManager::ReadFromINI()
{
   char *entries;
   char *entry;
   char file[MAXPATH];
   int size;
   int count;
   int maxFiles;

   // Get rid of current list of filenames
   recentFiles->Flush(TShouldDelete::Delete);

   // Get entry names from ini file
   maxFiles = recentFiles->ArraySize();
   size = maxFiles*2 + 2;
   entries = new char[size];
   profile->GetString(0,entries,size);

   // Get the string for each entry, but
   // don't go past the max number of files
   // (this way only the first maxFiles are used,
   // and not the last maxFiles)
   count = 0;
   entry = entries;
   while(*entry && count < maxFiles)
      {
      profile->GetString(entry,file,sizeof(file));
      AddFilename(file,false);
      entry += strlen(entries)+1;
      count++;
      }

   delete[] entries;
   updateMenu  = true;
}

/******************************************************************************
WriteToINI

Description:
   Writes the most recently opened files to the ini file.

Arguements:
   None

Return:
   None
*******************************************************************************/
void TRecentFileManager::WriteToINI()
{
   TIStringIterator i(*recentFiles);
   int count = 1;

   while(i)
      {
      string *fileName = i++;

      char entry[2];
      ostrstream(entry,sizeof(entry)) << count << ends;
      count++;
      if (!profile->WriteString(entry,fileName->c_str()))
         THROW(xmsg("Error writing to ini file") );
      }
}

/******************************************************************************
AddFilename

Description:
   Adds a new filename to the recent file list. The filename will
   be placed at the top of the recent file list (it is the most recently used
   file). If the list is full, the least recently used file will be dropped.
   If the filename currently exists in the list, the filename is simply
   brought to the top of the list.

Arguements:
   filename - name of the file to be added to the recent file list
   push     - if true, the new name is pushed onto the top of the list
              otherwise it is placed at the bottom of the list.

Return:
   None
*******************************************************************************/
void TRecentFileManager::AddFilename(const char *filename,bool push)
{
   int idx;
   string *fileName;

   // if it is an un-named file, get the heck outta here
   if (strlen(filename) == 0)
      return;

   fileName = new string(filename);
   idx = recentFiles->Find(fileName);

   if (idx != INT_MAX)
      {
      // The filename already exists in the array.
      // Remove it from the list, so the new filename can be
      // added at the top of the list.
      RemoveFilename(idx);
      }

   if (recentFiles->IsFull())
      {
      // Array is full, remove the last item in the array so there
      // will be room for the new filename
      RemoveFilename(recentFiles->ArraySize()-1);
      }

   // Add new filename to the start of the array.
   // AddAt pushes the other filenames down to make room for this one.
   if (push)
      recentFiles->AddAt(fileName,0);
   else
      recentFiles->Add(fileName);

   updateMenu  = true;
}


/******************************************************************************
AddFilename

Description:
   Adds a filename to the recent file list at the position indicated by pos.
   If the list is full, the filename at position pos will be dropped.
   If the filename currently exists in the list, it is moved to pos and
   files in pos, pos+1,... are moved down one.

Arguements:
   pos      - position to add filename
   filename - name of the file to be added to the recent file list

Return:
   None
*******************************************************************************/
void TRecentFileManager::AddFilename(int pos,const char *filename)
{
   int idx;
   string *fileName;

   // ignore this request if pos is not valid
   if (pos < 0 || recentFiles->ArraySize() <= pos)
      return;

   // if it is an un-named file, get the heck outta here
   if (strlen(filename) == 0)
      return;

   fileName = new string(filename);
   idx = recentFiles->Find(fileName);

   if (idx != INT_MAX)
      {
      // The filename already exists in the array.
      // Remove it from the list, so the new filename can be
      // added at the top of the list.
      RemoveFilename(idx);
      }

   if (recentFiles->IsFull())
      {
      // Array is full, remove the filename at the position requested
      RemoveFilename(pos);
      }

   recentFiles->AddAt(fileName,pos);

   updateMenu  = true;
}


/******************************************************************************
RemoveFilename

Description:
   Removes a file from the recent file list. File names beneath this
   filename are moved up one.

   Finds the position of filename and calls RemoveFilename(pos).

Arguements:
   filename - name of the file to be removed from the recent file list

Return:
   None
*******************************************************************************/
void TRecentFileManager::RemoveFilename(const char *filename)
{
   RemoveFilename(GetPosition(filename));
}


/******************************************************************************
GetPosition

Description:
   Returns the relative position of a filename within the recent file list.
   Returns < 0 if the file cannot be found.

Arguements:
   filename - name of the file

Return:
   file position or < 0 if file is not found
*******************************************************************************/
int TRecentFileManager::GetPosition(const char *filename)
{
   string *fileName;
   string matchMe(filename);
   TIStringIterator i(*recentFiles);
   int pos;
   bool found = false;

   // Find the position of this filename in the list
   pos = 0;
   while(i)
      {
      fileName = i++;
      if (*fileName == matchMe)
         {
         found = true;
         break;
         }
      pos++;
      }

   if (!found)
      pos = -1;

   return pos;
}


/******************************************************************************
RemoveFilename

Description:
   Removes a file from the recent file list. Filenames at position pos+1,
   pos+2,... are moved to pos,pos+1,...

Arguements:
   pos - position from where the filename is to be removed

Return:
   None
*******************************************************************************/
void TRecentFileManager::RemoveFilename(int pos)
{
   TRACE("Removing filename from index = " << pos);

   // ignore this request if pos is not valid
   if (pos < 0 || recentFiles->ArraySize() <= pos)
      return;

   string *filename = (*recentFiles)[pos];
   recentFiles->RemoveEntry(pos);
   delete filename;

   updateMenu = true;
}


/******************************************************************************
UpdateMenu

Description:
   Updates menu by clearing any existing filenames and then placing the
   current list of recent files on the menu. This method should be called
   in response to a WM_INITMENU message.

Arguements:
   menu - Reference to the popup file menu.

Return:
   None
*******************************************************************************/
void TRecentFileManager::UpdateMenu(TMenu &menu)
{
   TIStringIterator i(*recentFiles);
   int count;

   if (!updateMenu)
      return;

   // Clear out old list
   ClearMenu(menu);

   // Add a seperator, if there is something in the recent file list
   if (recentFiles->GetItemsInContainer() > 0)
      menu.AppendMenu(MF_SEPARATOR);

   // Add the filenames
   count = 1;
   while(i)
      {
      string *s = i++;

      char buffer[256];

      ostrstream(buffer,sizeof(buffer)) << "&" << count << " "
                                        << s->c_str() << ends;

      if (!menu.AppendMenu(MF_BYCOMMAND | MF_ENABLED,CM_OPENRECENTFILE+count-1,buffer))
         THROW(TMenu::TXMenu(IDS_MENUFAILURE));

      count++;
      }

   updateMenu = false;
}


/******************************************************************************
ClearMenu

Description:
   Clears the recent file list from the menu.

Arguements:
   menu - Reference to the popup file menu.

Return:
   None
*******************************************************************************/
void TRecentFileManager::ClearMenu(TMenu &menu)
{
   int numItems;
   int i;

   // Find the Exit menu item...
   // Search from bottom of menu... Exit is usually at the bottom
   numItems = menu.GetMenuItemCount();
   for (i = numItems-1; i >= 0; i--)
      {
      if (menu.GetMenuItemID(i) == CM_EXIT)
         break;
      }

   // Exit not found
   if (i < 0)
      return;

   // Move down one item
   i++;

   // Delete everything below Exit to the end of the menu
   for (int j = numItems-1; j >= i; j--)
      {
      menu.DeleteMenu(j,MF_BYPOSITION);
      }
}


/******************************************************************************
CmOpenRecentFile

Description:
   Function that respondes to a recent file being selected from the File menu.
   This function determines which file is to be opened and passes the filename
   to the virtual function OpenRecentFile

Arguements:
   commandId - menu command identifier

Return:
   None
*******************************************************************************/
void TRecentFileManager::CmOpenRecentFile(uint commandId)
{
   string *fileName;
   int fileIndex;

   // Crack filename for this event
   fileIndex = commandId - CM_OPENRECENTFILE;
   fileName = (*recentFiles)[fileIndex];

   // Call virtual function so derived classes can control
   // the opening behavior
   char *filename = new char[fileName->length() + 1];
   strcpy(filename,fileName->c_str());
   filename[fileName->length()] = 0;
   OpenRecentFile(filename);
   delete[] filename;
}


/******************************************************************************
OpenRecentFile

Description:
   This virtual function gets called by CmOpenRecentFile. By default this
   function does nothing. Override this function in a derived class to
   provide meaningful file opening behavior

Arguements:
   fileName - name of the file to be opened.

Return:
   None
*******************************************************************************/
void TRecentFileManager::OpenRecentFile(const char */*fileName*/)
{
}


/******************************************************************************
EvInitMenu

Description:
   Respondes to the WM_INITMENU message by updating the file menu.

Arguements:
   hMenu - handle of the main or system menu.

Return:
   None
*******************************************************************************/
void TRecentFileManager::EvInitMenu (HMENU hMenu)
{
   char cFirstItem[9];
   string strFirstItem;
   string restore("&Restore");

   TMenu mainMenu(hMenu);

   // Check if the first item on the menu is Restore. If it is, then
   // this is the system menu and we don't want to modify it.
   mainMenu.GetMenuString(0,cFirstItem,8,MF_BYPOSITION);
   strFirstItem = cFirstItem;

   if (strFirstItem == restore)
      return;

   HMENU hFileMenu = mainMenu.GetSubMenu(0);
   TMenu fileMenu(hFileMenu);
   UpdateMenu(fileMenu);
}

