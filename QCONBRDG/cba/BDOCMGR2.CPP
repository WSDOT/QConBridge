///////////////////////////////////////////////////////////////////////
// QConBridge - HL93 Live Load Analysis
// Copyright (C) 1999  Washington State Department of Transportation
//                     Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation, 
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but 
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source 
// License along with this program; if not, write to the Washington 
// State Department of Transportation, Bridge and Structures Office, 
// 4500 3rd AVE SE - P.O. Box  47340, Olympia, WA 98503, USA or e-mail 
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Revision Log
// ============
// May 1997 - Created, Richard Brice, PE
// Oct 1999 - Released as Open Source

#include <qconbrdg\bdocmgr.h>

//
//
//

TRFMBridgeDocManager::TRFMBridgeDocManager (int mode, TDocTemplate*& templateHead):
    TBridgeDocManager(mode, templateHead)
{
    // INSERT>> Your constructor code here.
    rfm = NULL;
}


TRFMBridgeDocManager::~TRFMBridgeDocManager ()
{
    // while there are documents in the DocList, and a recent file manager
    // exists, add the filename to the recent file manager
    TDocument* doc;
    for (doc = DocList.Next(0); doc; doc = DocList.Next(doc))
       {
       if (doc && rfm)
          rfm->AddFilename(doc->GetDocPath());
       }


    // Write the recent file list to the ini file
    if (rfm)
       rfm->WriteToINI();

    // Free up the memory
    delete rfm;
}

void TRFMBridgeDocManager::SetRecentFileManager(TRecentFileManager *rfm)
{
   TRFMBridgeDocManager::rfm = rfm;
}

TRecentFileManager &TRFMBridgeDocManager::GetRecentFileManager()
{
   return *rfm;
}

bool TRFMBridgeDocManager::Find(TEventInfo& eventInfo, TEqualOperator equal)
{
   bool retval;

   retval = (rfm) ? rfm->Find(eventInfo,equal) : false;

   if (!retval)
      retval = TBridgeDocManager::Find(eventInfo,equal);

   return retval;
}

//
//
//


TMyRecentFileManager::TMyRecentFileManager(TRFMBridgeDocManager *docMgr,const char *section,const char *ini,int maxFiles) :
TRecentFileManager(section,ini,maxFiles)
{
   TMyRecentFileManager::docMgr = docMgr;
}

void TMyRecentFileManager::OpenRecentFile(const char *fileName)
{
   int pos;

   pos = GetPosition(fileName);

   RemoveFilename(pos);

   // Create a document for this file
   TDocTemplate* tpl = docMgr->MatchTemplate(fileName);
   if (tpl)
      {
      // Close the current document
      // Can't do this before getting and removing the filename above,
      // because CloseTheDocument calls TDocument::Close, which modifies
      // the file list...
      if (!docMgr->CloseTheDocument())
         {
         // document cannot be closed...
         // the recent file list is out of date because a filename has
         // been removed, but since the document is still open the
         // filename must be put back to where it was.
         AddFilename(pos,fileName);
         }
      else
         {
         tpl->CreateDoc(fileName);
         docMgr->SetMainWindowCaption();
         }
      }
}
