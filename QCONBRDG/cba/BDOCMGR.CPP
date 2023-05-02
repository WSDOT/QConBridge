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
#include <qconbrdg\bdoc.h>
#include <qconbrdg\editview.h>
#include <qconbrdg\newbridg.h>
#include <qconbrdg\prototyp.h>
#include <owl\opensave.h>
#include <owl\rcntfile.h>
#include <owl\profile.h>
#include <winsys\system.h>
#include <qconbrdg\qcbapp.rh>           // Definition of all resources.

// I have been using TDocument* doc = DocList.Next(0); to get the current document
// because GetCurrentDoc() expects there to be an open view. In this application,
// a document can exist without any open views. If there is not an open view,
// TDocManager thinks that there is not a current document

//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(TBridgeDocManager, TDocManager)
//{{TBridgeDocManagerRSP_TBL_BEGIN}}
    EV_COMMAND(CM_FILENEW,CmFileNew),
    EV_COMMAND(CM_FASTNEW,CmFastNew),
    EV_COMMAND(CM_EDITORVIEW, CmEditorView),
    EV_COMMAND_ENABLE(CM_EDITORVIEW, CeEditorView),
    EV_COMMAND(CM_MOMENTDIAGRAM, CmMomentDiagram),
    EV_COMMAND_ENABLE(CM_MOMENTDIAGRAM, CeMomentDiagram),
    EV_COMMAND(CM_SHEARDIAGRAM, CmShearDiagram),
    EV_COMMAND_ENABLE(CM_SHEARDIAGRAM, CeShearDiagram),
    EV_COMMAND(CM_TABULARRESULTS, CmTabularResults),
    EV_COMMAND_ENABLE(CM_TABULARRESULTS, CeTabularResults),
    EV_COMMAND(CM_REPORT, CmReport),
    EV_COMMAND_ENABLE(CM_REPORT, CeReport),
//{{TBridgeDocManagerRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TBridgeDocManager Implementation}}


TBridgeDocManager::TBridgeDocManager (int mode, TDocTemplate*& templateHead):
    TDocManager(mode, templateHead)
{
    // INSERT>> Your constructor code here.

}


TBridgeDocManager::~TBridgeDocManager ()
{
    // INSERT>> Your destructor code here.

}

void TBridgeDocManager::CreateNamedView(const char *matchMe)
{
    TRACE("Entering TBridgeDocManager::CreateNamedView()");

    TDocTemplate *tpl;

    // loop over all templates until we find one with the name matchMe
    for (tpl = GetNextTemplate(0); tpl; tpl = GetNextTemplate(tpl))
       {
       const string name(matchMe);
       const string viewName(tpl->GetViewName());
       if (name == viewName)
          break;
       }

    if (tpl)
       tpl->CreateView(*DocList.Next(0));

    TRACE("Leaving TBridgeDocManager::CreateNamedView()");
}

bool TBridgeDocManager::CloseTheDocument()
{
   // Allows this MDI application to act like SDI while allowing multiple views
   // Call this method before attempting to open any document. If false is returned,
   // do not open the document.
   //
   // Stolen from TDocManager::CmFileClose
   // but can't use CmFileClose because there might not be a current
   // document (all views closed, but DocList still contains the
   // document).
   TDocument* doc = DocList.Next(0);
   if (doc == 0)
      return true; // There is nothing in the DocList

   if (doc->CanClose())
      {  // normally calls back to FlushDoc()
      if (!doc->Close())
         {
         PostDocError(*doc, IDS_UNABLECLOSE);
         return false; // Error closing the document
         }
      else
         delete doc;
      }
   else
      return false; // Can't close the document

   return true; // Document was successfully closed
}

void TBridgeDocManager::UpdateMainWindowCaption()
{
   TRACE("Entering TBridgeDocManager::UpdateMainWindowCaption()");
   TDocument *doc = DocList.Next(0);
   if (doc)
      {
      string docTitle;
      if (doc->GetDocPath() == 0)
         docTitle = doc->GetTitle();
      else
         {
         // Determine if we need to display the extension or not
         if (TSystem::IsWin95() || (TSystem::IsNT() && TSystem::GetMajorVersion() >= 4))
            {
            // If this is Win95, use the system facilities to determine display mode
            SHFILEINFO shFileInfo;
            ::SHGetFileInfo(doc->GetDocPath(),0,&shFileInfo,sizeof(SHFILEINFO),SHGFI_DISPLAYNAME);
            docTitle = shFileInfo.szDisplayName;
            }
         else
            {
            // This is not Win95, show the entire filename + extension
            docTitle = doc->GetDocPath();
            }
         }

      string appName(GetApplication()->GetName());
      appName += " - ";
      appName += docTitle;
      GetApplication()->GetMainWindow()->SetCaption(appName.c_str());
      }
   else
      GetApplication()->GetMainWindow()->SetCaption(GetApplication()->GetName());

   TRACE("Leaving TBridgeDocManager::UpdateMainWindowCaption()");
}


void TBridgeDocManager::CmEditorView ()
{
    // INSERT>> Your code here.
    CreateNamedView("Editor");
}


void TBridgeDocManager::CeEditorView (TCommandEnabler &tce)
{
    // INSERT>> Your code here.
    tce.Enable(TBridgeEditorView::count == 0 && DocList.Next(0));
}


void TBridgeDocManager::CmMomentDiagram ()
{
    // INSERT>> Your code here.
    CreateNamedView("Moment Diagram");
}


void TBridgeDocManager::CeMomentDiagram (TCommandEnabler &tce)
{
    // INSERT>> Your code here.
    tce.Enable(DocList.Next(0) != 0);
}


void TBridgeDocManager::CmShearDiagram ()
{
    // INSERT>> Your code here.
    CreateNamedView("Shear Diagram");
}


void TBridgeDocManager::CeShearDiagram (TCommandEnabler &tce)
{
    // INSERT>> Your code here.
    tce.Enable(DocList.Next(0) != 0);
}


void TBridgeDocManager::CmTabularResults ()
{
    // INSERT>> Your code here.

    CreateNamedView("Tabular Results");
}


void TBridgeDocManager::CeTabularResults (TCommandEnabler &tce)
{
    // INSERT>> Your code here.
    tce.Enable(DocList.Next(0) != 0);
}

void TBridgeDocManager::CmReport ()
{
    // INSERT>> Your code here.
    CreateNamedView("Report");
}


void TBridgeDocManager::CeReport (TCommandEnabler &tce)
{
    // INSERT>> Your code here.
    tce.Enable(DocList.Next(0) != 0);
}

void TBridgeDocManager::CmFileNew ()
{
   TRACE("TBridgeDocManager::CmFileNew()");

   SystemOfUnits units;
   TBridgeDocument *bridgeDoc = TYPESAFE_DOWNCAST(DocList.Next(0),TBridgeDocument);
   BridgePrototype *prototype;

   if (!CloseTheDocument())
      return;

   TDocManager::CmFileNew(); // create a new document

   if (bridgeDoc)
      units = bridgeDoc->GetUnits();
   else
      {
      TProfile sysUnits("Units","qconbrdg.ini");
      units = (SystemOfUnits)sysUnits.GetInt("Units",unitsSI);
      }

   TNewBridgeWizard newBridgeWiz(GetApplication()->GetMainWindow(),units);
   TNewBridgeXferBuffer newBridgeXfer(units);

   newBridgeWiz.SetupTransferBuffer(&newBridgeXfer);

   if (newBridgeWiz.Execute() == IDOK)
      {
      TBridgeDocument *bridgeDoc = TYPESAFE_DOWNCAST(DocList.Next(0),TBridgeDocument);
      if (!bridgeDoc)
         return;

      prototype = newBridgeXfer.GetPrototype();
      bridgeDoc->CreateFromPrototype(prototype);
      bridgeDoc->SetUnits(newBridgeXfer.step1.GetUnits());

      LRFDBridge &bridge(bridgeDoc->GetBridge());

      // Dual tandem train is optional... Disable it by default
      bridge.DualTruckLoadEnabled(DUALTANDEMS,false);

      // Disable dual truck train generation if simple spans
      if (newBridgeXfer.step1.IsSimple())
         {
         bridge.DualTruckLoadEnabled(DUALTRUCKS,false);
         }

      UpdateMainWindowCaption();

      delete prototype;
      }

}

void TBridgeDocManager::CmFastNew()
{
   if (!CloseTheDocument())
      return;

   TDocManager::CmFileNew();

   UpdateMainWindowCaption();
}

void TBridgeDocManager::CmFileOpen ()
{
   if (!CloseTheDocument())
      return;

    OSVERSIONINFO osVersionInfo;
    osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osVersionInfo);
    TDocTemplate *tpl = GetNextTemplate(0);
    switch(osVersionInfo.dwPlatformId)
       {
       case VER_PLATFORM_WIN32s: //Win32s on Windows 3.1
            tpl->ClearFlag(OFN_EXPLORER);
            break;

       case VER_PLATFORM_WIN32_NT: // WindowsNT 3.51
            tpl->ClearFlag(OFN_EXPLORER);
            tpl->SetFlag(OFN_LONGNAMES);
            break;

       case VER_PLATFORM_WIN32_WINDOWS: // Windows95
            tpl->SetFlag(OFN_EXPLORER);
            break;
       }

    TDocManager::CmFileOpen();
    UpdateMainWindowCaption();
}


void TBridgeDocManager::CmFileClose ()
{
    TDocManager::CmFileClose();
    UpdateMainWindowCaption();
}

void TBridgeDocManager::CmFileSaveAs ()
{
    OSVERSIONINFO osVersionInfo;
    osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osVersionInfo);
    TDocTemplate *tpl = GetNextTemplate(0);
    switch(osVersionInfo.dwPlatformId)
       {
       case VER_PLATFORM_WIN32s: //Win32s on Windows 3.1
            tpl->ClearFlag(OFN_EXPLORER);
            break;

       case VER_PLATFORM_WIN32_NT: // WindowsNT 3.51
            tpl->ClearFlag(OFN_EXPLORER);
            tpl->SetFlag(OFN_LONGNAMES);
            break;

       case VER_PLATFORM_WIN32_WINDOWS: // Windows95
            tpl->SetFlag(OFN_EXPLORER);
            break;
       }

   TDocManager::CmFileSaveAs();
   UpdateMainWindowCaption();
}

