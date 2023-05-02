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


#include <dir.h>
#include <owl\statusba.h>
#include <owl\buttonga.h>
#include <owl\gadget.h>

#include <classlib/filename.h>
#include <classlib/cmdline.h>

#include <qconbrdg\qcbapp.h>
#include <qconbrdg\mdiclnt.h>
#include <qconbrdg\mdichild.h>
#include <qconbrdg\bdoc.h>
#include <qconbrdg\editview.h>
#include <qconbrdg\shearv.h>
#include <qconbrdg\momentv.h>
#include <qconbrdg\tblview.h>
#include <qconbrdg\rptview.h>
#include <qconbrdg\bdocmgr.h>
#include <qconbrdg\aboutdlg.h>
#include <winsys\system.h>
#include <owl\messageb.h>
#include <owltools\winplace.h>

#include <esl\units.h>
#pragma hdrstop

#include <qconbrdg\legalwiz.h>
#include <esl\siconst.h>
#include <qconbrdg\newbridg.h>

//
// Generated help file.
//
const char HelpFileName[] = "qconbridge.hlp";

TMyDecoratedMDIFrame::TMyDecoratedMDIFrame(const char far* title,
                       TResId          menuResId,
                       TMDIClient&     clientWnd,
                       bool            trackMenuSelection,
                       TModule*        module) :
    TDecoratedMDIFrame(title,menuResId,clientWnd,trackMenuSelection,module)
{
}

void TMyDecoratedMDIFrame::SetupWindow()
{
   TDecoratedMDIFrame::SetupWindow();

   char regKey[256];
   bool isVisible;
   uint32 size = sizeof(isVisible);
   TQConBridgeApp* app = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);

   app->GetRegistryKey(0,regKey,sizeof(regKey));
   if (TRegKey(TRegKey::CurrentUser(),regKey).QueryValue("StatusBar",0,(uint8*)&isVisible,&size) != ERROR_SUCCESS)
      isVisible = true;

   if (!isVisible)
      {
      TWindow* decoration = ChildWithId(IDW_STATUSBAR);
      if (decoration)
         {
         RemoveChildLayoutMetrics(*decoration);
         decoration->ShowWindow(SW_HIDE);
         Layout();
         }
      }

   TWindowPlacement wp;
   app->GetRegistryKey("Placement",regKey,sizeof(regKey));
   wp.RestoreFromRegistry(this,regKey);
}

void TMyDecoratedMDIFrame::Destroy(int retVal)
{
   TWindow* decoration = ChildWithId(IDW_STATUSBAR);

   char regKey[256];
   TQConBridgeApp* app = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   app->GetRegistryKey(0,regKey,sizeof(regKey));

   if (decoration)
      {
      bool isVisible;

      isVisible = decoration->IsWindowVisible();
      TRegKey(TRegKey::CurrentUser(),regKey).SetValue("StatusBar",REG_BINARY,(uint8*)&isVisible,sizeof(bool));
      }

   TWindowPlacement wp;
   app->GetRegistryKey("Placement",regKey,sizeof(regKey));
   wp.SaveToRegistry(this,regKey);

   TDecoratedMDIFrame::Destroy(retVal);
}

// Drag / Drop support:
TFileDrop::TFileDrop (const char* fileName, TPoint& p, bool inClient, TModule*)
{
    char    exePath[MAXPATH];

    exePath[0] = 0;
    FileName = strcpy(new char[strlen(fileName) + 1], fileName);
    Point = p;
    InClientArea = inClient;
}

TFileDrop::~TFileDrop ()
{
    delete FileName;
}

const char *TFileDrop::WhoAmI ()
{
  return FileName;
}


//{{TQConBridgeApp Implementation}}



//{{DOC_VIEW}}
DEFINE_DOC_TEMPLATE_CLASS(TBridgeDocument, TBridgeEditorView, DocType1);
DEFINE_DOC_TEMPLATE_CLASS(TBridgeDocument, TBridgeShearView,  DocType2);
DEFINE_DOC_TEMPLATE_CLASS(TBridgeDocument, TBridgeMomentView, DocType3);
DEFINE_DOC_TEMPLATE_CLASS(TBridgeDocument, TTableView,        DocType4);
DEFINE_DOC_TEMPLATE_CLASS(TBridgeDocument, TReportView,       DocType5);
//{{DOC_VIEW_END}}

//{{DOC_MANAGER}}
DocType1 __dvt1("QConBridge Files", "*.qcb", 0, "qcb", dtAutoOpen | dtHideReadOnly | dtPathMustExist | dtOverwritePrompt);
DocType2 __dvt2("QConBridge Files (*.qcb)", "*.qcb", 0, "qcb", dtHidden);
DocType3 __dvt3("QConBridge Files (*.qcb)", "*.qcb", 0, "qcb", dtHidden);
DocType4 __dvt4("QConBridge Files (*.qcb)", "*.qcb", 0, "qcb", dtHidden);
DocType5 __dvt5("QConBridge Files (*.qcb)", "*.qcb", 0, "qcb", dtHidden);
//{{DOC_MANAGER_END}}


//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE2(TQConBridgeApp, TApplication, TRecentFiles)
//{{TQConBridgeAppRSP_TBL_BEGIN}}
    EV_OWLVIEW(dnCreate, EvNewView),
    EV_OWLVIEW(dnClose,  EvCloseView),
    EV_COMMAND(CM_FILESEND, CmFileSend),
    EV_COMMAND_ENABLE(CM_FILESEND, CeFileSend),
    EV_COMMAND(CM_HELPABOUT, CmHelpAbout),
    EV_COMMAND(CM_HELPTOPICS,CmHelpTopics),
    EV_COMMAND(IDW_TOOLBAR,CmToggleToolbar),
    EV_COMMAND_ENABLE(IDW_TOOLBAR,CeToggleToolbar),
    EV_COMMAND(CM_LEGALNOTICE,CmLegalNotice),
    EV_WM_DROPFILES,
    EV_WM_WININICHANGE,
    EV_REGISTERED(MruFileMessage,EvFileSelected),
//{{TQConBridgeAppRSP_TBL_END}}
END_RESPONSE_TABLE;

//////////////////////////////////////////////////////////
// TQConBridgeApp
// =====
//
TQConBridgeApp::TQConBridgeApp () :
TApplication("QConBridge"),
TRecentFiles("QConBrdg.ini",4),
HelpFile(::HelpFileName)
{
   HelpState = false;
   ContextHelp = false;
   HelpCursor = 0;
   SetDocManager(new TBridgeDocManager(dmMDI | dmMenu));
   Printer = 0;
   Printing = false;

   dibSplash = new TDib(*this,TResId(BMP_SPLASH));
   Splash = new TSplashWindow(*dibSplash,3000,3000,TSplashWindow::ShrinkToFit | TSplashWindow::CaptureMouse,3500);

   ApxMail = new TMailer();

   m_bShowLegalNotice = true;
}


TQConBridgeApp::~TQConBridgeApp ()
{
    delete Printer;
    delete ApxMail;
//    delete Splash; // Deleting this causes the program to crash. This is new in BC 5.01
//    delete dibSplash; // Splash deletes it via TPictureWindow
}

BOOL TQConBridgeApp::ActivateHelp(TWindow* /*window*/, int helpFileContextId)
{
    return GetMainWindow()->WinHelp(GetHelpFile().c_str(), HELP_CONTEXT, helpFileContextId);
}

BOOL TQConBridgeApp::DeactivateHelp()
{
    return GetMainWindow()->WinHelp(GetHelpFile().c_str(), HELP_QUIT, 0);
}

string& TQConBridgeApp::GetHelpFile()
{
   return HelpFile;
}

bool TQConBridgeApp::CanClose ()
{
    bool result = TApplication::CanClose();

    //
    // Close the help engine if we used it.
    //
    if (result && HelpState)
        HelpState = DeactivateHelp();

   // Save the position of the speedbar into the registry
   //
   char regKey[256];
   bool isVisible;
   uint32 sizeVisible = sizeof(isVisible);
   TAbsLocation location;
   uint32 sizeLocation = sizeof(TAbsLocation);
   TRect rect;
   uint32 sizeRect = sizeof(TRect);

   TDockablePosition* dp = ApxHarbor->GetDockablePosition(IDW_TOOLBAR);
   isVisible = ApxHarbor->IsDockableVisible(IDW_TOOLBAR);
   location = dp->GetLocation();
   rect = dp->GetRect();

   GetRegistryKey("Toolbar",regKey,sizeof(regKey));
   TRegKey(TRegKey::CurrentUser(),regKey).SetValue("Visible",REG_BINARY,(uint8*)&isVisible,sizeVisible);
   TRegKey(TRegKey::CurrentUser(),regKey).SetValue("Location",REG_BINARY,(uint8*)&location,sizeLocation);
   TRegKey(TRegKey::CurrentUser(),regKey).SetValue("Rect",REG_BINARY,(uint8*)&rect,sizeRect);

   GetRegistryKey("Settings",regKey,sizeof(regKey));
   TRegKey(TRegKey::CurrentUser(),regKey).SetValue("LegalNotice",REG_BINARY,(uint8*)&m_bShowLegalNotice,sizeof(m_bShowLegalNotice) );

   return result;
}


void TQConBridgeApp::CreateGadgets(TDockableControlBar* cb, bool server)
{
  if (!server) {
    cb->Insert(*new TButtonGadget(CM_FASTNEW, CM_FASTNEW));
    cb->Insert(*new TButtonGadget(CM_MDIFILEOPEN, CM_MDIFILEOPEN));
    cb->Insert(*new TButtonGadget(CM_FILESAVE, CM_FILESAVE));
    cb->Insert(*new TSeparatorGadget(6));
    }

    cb->Insert(*new TButtonGadget(CM_EDITCUT,     CM_EDITCUT));
    cb->Insert(*new TButtonGadget(CM_EDITCOPY,    CM_EDITCOPY));
    cb->Insert(*new TButtonGadget(CM_PASTEINSERT, CM_PASTEINSERT));
    cb->Insert(*new TButtonGadget(CM_PASTEREPLACE,CM_PASTEREPLACE));
    cb->Insert(*new TButtonGadget(CM_PASTESPECIAL,CM_PASTESPECIAL));

    cb->Insert(*new TSeparatorGadget(6));

    cb->Insert(*new TButtonGadget(CM_ADDSPAN,          CM_ADDSPAN));
    cb->Insert(*new TButtonGadget(CM_CONCENTRATED_LOAD,CM_CONCENTRATED_LOAD));
    cb->Insert(*new TButtonGadget(CM_UNIFORM_LOAD,     CM_UNIFORM_LOAD));

    cb->Insert(*new TSeparatorGadget(6));

    cb->Insert(*new TButtonGadget(CM_SELECTLEFT, CM_SELECTLEFT, TButtonGadget::Exclusive));
    cb->Insert(*new TButtonGadget(CM_SELECTRIGHT,CM_SELECTRIGHT,TButtonGadget::Exclusive));
    cb->Insert(*new TSeparatorGadget(6));
    cb->Insert(*new TButtonGadget(CM_INSERTBEFORE,CM_INSERTBEFORE,TButtonGadget::Exclusive));
    cb->Insert(*new TButtonGadget(CM_INSERTAFTER, CM_INSERTAFTER, TButtonGadget::Exclusive));

    cb->Insert(*new TSeparatorGadget(6));

    cb->Insert(*new TButtonGadget(CM_FASTANALYZE, CM_FASTANALYZE));

    cb->Insert(*new TSeparatorGadget(6));

    cb->Insert(*new TButtonGadget(CM_MOMENTDIAGRAM, CM_MOMENTDIAGRAM));
    cb->Insert(*new TButtonGadget(CM_SHEARDIAGRAM,  CM_SHEARDIAGRAM));
    cb->Insert(*new TButtonGadget(CM_TABULARRESULTS,CM_TABULARRESULTS));
    cb->Insert(*new TButtonGadget(CM_REPORT,        CM_REPORT));

    cb->Insert(*new TSeparatorGadget(6));

    cb->Insert(*new TButtonGadget(CM_FASTPRINT, CM_FASTPRINT));

    cb->Insert(*new TSeparatorGadget(6));

    cb->Insert(*new TButtonGadget(CM_HELPTOPICS, CM_HELPTOPICS));


    // Add caption and fly-over help hints.
    //
    cb->SetCaption("Standard Toolbar");
    cb->SetHintMode(TGadgetWindow::EnterHints);
}

void TQConBridgeApp::SetupSpeedBar (TDecoratedMDIFrame *frame)
{
   ApxHarbor = new THarborMaster(*frame);

   // Create default toolbar New and associate toolbar buttons with commands.
   //
   TDockableControlBar* cb = new TDockableControlBar(frame);
   CreateGadgets(cb);

   // Setup the toolbar ID used by OLE 2 for toolbar negotiation.
   //
   cb->Attr.Id = IDW_TOOLBAR;

   ApxHarbor->Insert(*cb, alTop);
}


//////////////////////////////////////////////////////////
// TQConBridgeApp
// =====
// Application intialization.
//
void TQConBridgeApp::InitMainWindow ()
{
   Splash->Create();
   Splash->UpdateWindow();

    if (nCmdShow != SW_HIDE)
        nCmdShow = (nCmdShow != SW_SHOWMINNOACTIVE) ? SW_SHOWNORMAL : nCmdShow;

    mdiClient = new TQConBridgeMDIClient(this);
    TDecoratedMDIFrame* frame = new TMyDecoratedMDIFrame(Name, MDI_MENU, *mdiClient, true, this);

    nCmdShow = (nCmdShow != SW_SHOWMINNOACTIVE) ? SW_SHOWNORMAL : nCmdShow;

    //
    // Assign ICON w/ this application.
    //
    frame->SetIcon(this, IDI_MDIAPPLICATION);
    frame->SetIconSm(this, IDI_MDIAPPLICATION);

    //
    // Menu associated with window and accelerator table associated with table.
    //
    frame->AssignMenu(MDI_MENU);

    //
    // Associate with the accelerator table.
    //
    frame->Attr.AccelTable = MDI_MENU;

    SetupSpeedBar(frame);

    TStatusBar *sb = new TStatusBar(frame, TGadget::Recessed, TStatusBar::SizeGrip);
    modifiedTextGadget = new TTextGadget(0,TGadget::Recessed,TTextGadget::Center,10);
    sb->Insert(*modifiedTextGadget );
    TMargins margins(TMargins::BorderUnits,1,1,1,1);
    sb->SetMargins(margins);

    frame->Insert(*sb, TDecoratedFrame::Bottom);

    SetMainWindow(frame);

    frame->SetMenuDescr(TMenuDescr(MDI_MENU));

    //
    // Windows 3-D controls.
    //
    EnableCtl3d(true);
}

long TQConBridgeApp::ShowLegalNoticeAtStartup()
{
   if ( m_bShowLegalNotice )
      return ShowLegalNotice(true);

   return 1;
}

long TQConBridgeApp::ShowLegalNotice(bool bGiveChoice)
{
   TLegalWizard wiz(GetMainWindow());
   wiz.ShowAgain( m_bShowLegalNotice );
   long lResult = wiz.Show(bGiveChoice);
   m_bShowLegalNotice = wiz.ShowAgain();
   return lResult;
}

void TQConBridgeApp::CmLegalNotice()
{
   ShowLegalNotice(false);
}

//////////////////////////////////////////////////////////
// TQConBridgeApp
// =====
// Response Table handlers:
//
void TQConBridgeApp::EvNewView (TView& view)
{
    TIconView *iconView = TYPESAFE_DOWNCAST(&view,TIconView);
    TMDIClient *mdiClient = TYPESAFE_DOWNCAST(GetMainWindow()->GetClientWindow(), TMDIClient);
    if (mdiClient) {
        TQConBridgeMDIChild* child = new TQConBridgeMDIChild(*mdiClient, 0, view.GetWindow());

        // Associate ICON w/ this child window.
        child->SetIcon(this, iconView->GetIconResourceId());
        child->SetIconSm(this, iconView->GetIconResourceId());

        child->Create();
    }
}


void TQConBridgeApp::EvCloseView (TView&)
{
}


//////////////////////////////////////////////////////////
// TQConBridgeApp
// =====
// Menu Help Contents command
void TQConBridgeApp::CmHelpTopics ()
{
    //
    // Show the help table of contents.
    //
    HelpState = GetMainWindow()->WinHelp(GetHelpFile().c_str(), HELP_FINDER, 0L);
}


//////////////////////////////////////////////////////////
// TQConBridgeApp
// ===========
// Menu Help About cba.exe command
void TQConBridgeApp::CmHelpAbout ()
{
    TAboutDlg(GetMainWindow()).Execute();
}


void TQConBridgeApp::InitInstance ()
{
    TApplication::InitInstance();

    // Accept files via drag/drop in the frame window.
    GetMainWindow()->DragAcceptFiles(true);
    ProcessCmdLine(lpCmdLine);

   // Read the position of the speedbar after the last run from the registry
   //
   char regKey[256];
   bool isVisible;
   uint32 sizeVisible = sizeof(isVisible);
   TAbsLocation location;
   uint32 sizeLocation = sizeof(TAbsLocation);
   TRect rect;
   uint32 sizeRect = sizeof(TRect);

   GetRegistryKey("Toolbar",regKey,sizeof(regKey));
   if (TRegKey(TRegKey::CurrentUser(),regKey).QueryValue("Visible",0,(uint8*)&isVisible,&sizeVisible) != ERROR_SUCCESS)
      isVisible = true;
   if (TRegKey(TRegKey::CurrentUser(),regKey).QueryValue("Location",0,(uint8*)&location,&sizeLocation) != ERROR_SUCCESS)
      location = alTop;
   if (TRegKey(TRegKey::CurrentUser(),regKey).QueryValue("Rect",0,(uint8*)&rect,&sizeRect) != ERROR_SUCCESS)
      rect = TRect(0,0,0,0);

   GetRegistryKey("Settings",regKey,sizeof(regKey));
   uint32 sizeShowLegalNotice = sizeof(m_bShowLegalNotice);
   if ( TRegKey(TRegKey::CurrentUser(),regKey).QueryValue("LegalNotice",0,(uint8*)&m_bShowLegalNotice,&sizeShowLegalNotice) != ERROR_SUCCESS)
      m_bShowLegalNotice = true;

   ApxHarbor->SetDockablePosition(IDW_TOOLBAR,location,rect);
   if (!isVisible)
      ApxHarbor->HideDockable(IDW_TOOLBAR);

   if ( ShowLegalNoticeAtStartup() == 2 )
   {
      exit(EXIT_SUCCESS);
   }
}

void TQConBridgeApp::ExcessiveFileMessage()
{
   ostrstream os;
   os << GetName() << " can only open one document at a time." << endl
      << "Please try again." << ends;
   GetMainWindow()->MessageBox(os.str(),GetName(),MB_OK | MB_ICONSTOP);
   delete[] os.str();
}


void TQConBridgeApp::EvDropFiles (TDropInfo drop)
{
    // Number of files dropped.
    int totalNumberOfFiles = drop.DragQueryFileCount();
    if (totalNumberOfFiles > 1)
       {
       ExcessiveFileMessage();
       return;
       }


    TFileList* files = new TFileList;

    for (int i = 0; i < totalNumberOfFiles; i++) {
        // Tell DragQueryFile the file interested in (i) and the length of your buffer.
        int     fileLength = drop.DragQueryFileNameLen(i) + 1;
        char    *fileName = new char[fileLength];

        drop.DragQueryFile(i, fileName, fileLength);

        // Getting the file dropped. The location is relative to your client coordinates,
        // and will have negative values if dropped in the non client parts of the window.
        //
        // DragQueryPoint copies that point where the file was dropped and returns whether
        // or not the point is in the client area.  Regardless of whether or not the file
        // is dropped in the client or non-client area of the window, you will still receive
        // the file name.
        TPoint  point;
        bool    inClientArea = drop.DragQueryPoint(point);
        files->Add(new TFileDrop(fileName, point, inClientArea, this));
    }

    // Open the files that were dropped.
    AddFiles(files);

    // Release the memory allocated for this handle with DragFinish.
    drop.DragFinish();
}


void TQConBridgeApp::AddFiles (TFileList* files)
{
    // Attempt to close the open document
    TBridgeDocManager *docManager;
    docManager = TYPESAFE_DOWNCAST(GetDocManager(),TBridgeDocManager);
    if (!docManager->CloseTheDocument())
       return;

    // Open all files dragged in.
    TFileListIter fileIter(*files);

    while (fileIter) {
        TDocTemplate* tpl = docManager->MatchTemplate(fileIter.Current()->WhoAmI());

        if (tpl)
            {
            docManager->CreateDoc(tpl,fileIter.Current()->WhoAmI(),0);
            docManager->UpdateMainWindowCaption();
            }
        fileIter++;
    }
}

bool TQConBridgeApp::ProcessAppMsg (MSG& msg)
{
    if (msg.message == WM_COMMAND) {
        if (ContextHelp || (::GetKeyState(VK_F1) < 0)) {
            ContextHelp = false;
            uint32 help_topic = msg.wParam;
            switch(help_topic)
            {
            case CM_FASTNEW:
                 help_topic = CM_FILENEW;
                 break;

            case CM_FASTANALYZE:
                 help_topic = CM_ANALYZE;
                 break;

            case CM_FASTPRINT:
                 help_topic = CM_PRINT;
                 break;
            }
            HelpState = GetMainWindow()->WinHelp(GetHelpFile().c_str(), HELP_CONTEXT, help_topic);
            return true;
        }
    } else
        switch (msg.message) {
        case WM_KEYDOWN:
            if (msg.wParam == VK_F1) {
                // If the Shift/F1 then set the help cursor and turn on the modal help state.
                if (::GetKeyState(VK_SHIFT) < 0) {
                    ContextHelp = true;
                    HelpCursor = ::LoadCursor(GetMainWindow()->GetModule()->GetInstance(), MAKEINTRESOURCE(IDC_HELPCURSOR));
                    ::SetCursor(HelpCursor);
                    return true;        // Gobble up the message.
                } else {
                    // If F1 w/o the Shift key then bring up help's main index.
                    HelpState = GetMainWindow()->WinHelp(GetHelpFile().c_str(),HELP_FINDER,0L);
                    return true;        // Gobble up the message.
                }
            } else {
                if (ContextHelp && (msg.wParam == VK_ESCAPE)) {
                    if (HelpCursor)
                        ::DestroyCursor(HelpCursor);
                    ContextHelp = false;
                    HelpCursor = 0;
                    GetMainWindow()->SetCursor(0, IDC_ARROW);
                    return true;    // Gobble up the message.
                }
            }
            break;

        case WM_MOUSEMOVE:
        case WM_NCMOUSEMOVE:
            if (ContextHelp) {
                ::SetCursor(HelpCursor);
                return true;        // Gobble up the message.
            }
            break;

        case WM_INITMENU:
            if (ContextHelp) {
                ::SetCursor(HelpCursor);
                return true;        // Gobble up the message.
            }
            break;

        case WM_ENTERIDLE:
            if (msg.wParam == MSGF_MENU)
                if (GetKeyState(VK_F1) < 0) {
                    ContextHelp = true;
                    GetMainWindow()->PostMessage(WM_KEYDOWN, VK_RETURN, 0L);
                    return true;       // Gobble up the message.
                }
            break;

        default:
            ;
        };  // End of switch

    // Continue normal processing.

    return TApplication::ProcessAppMsg(msg);
}

bool TQConBridgeApp::IdleAction (long idleCount)
{
    bool result;

    result = TApplication::IdleAction(idleCount);

    // INSERT>> Your code here.
    TDocManager *docMgr = GetDocManager();
    if (idleCount == 0 && docMgr)
       {
       TDocument *doc = docMgr->GetCurrentDoc();
       if (doc && doc->IsDirty())
          modifiedTextGadget->SetText("Modified");
       else
          modifiedTextGadget->SetText("");
       }

    return result;
}

TPrinter *TQConBridgeApp::GetPrinter()
{
   if (!Printer)
      Printer = new TPrinter(this);

   return Printer;
}

void TQConBridgeApp::EvWinIniChange (char far* section)
{
    if (strcmp(section, "windows") == 0) {
        // If the device changed in the WIN.INI file then the printer
        // might have changed.  If we have a TPrinter (Printer) then
        // check and make sure it's identical to the current device
        // entry in WIN.INI.
        if (Printer) {
            char printDBuffer[255];
            LPSTR printDevice = printDBuffer;
            LPSTR devName;
            LPSTR driverName = 0;
            LPSTR outputName = 0;

            if (::GetProfileString("windows", "device", "", printDevice, sizeof(printDevice))) {
                // The string which should come back is something like:
                //
                //      HP LaserJet III,hppcl5a,LPT1:
                //
                // Where the format is:
                //
                //      devName,driverName,outputName
                //
                devName = printDevice;
                while (*printDevice) {
                    if (*printDevice == ',') {
                        *printDevice++ = 0;
                        if (!driverName)
                            driverName = printDevice;
                        else
                            outputName = printDevice;
                    } else
                        printDevice = ::AnsiNext(printDevice);
                }

                if ((Printer->GetSetup().Error != 0)                                ||
                    (strcmp(devName, Printer->GetSetup().GetDeviceName()) != 0)    ||
                    (strcmp(driverName, Printer->GetSetup().GetDriverName()) != 0) ||
                    (strcmp(outputName, Printer->GetSetup().GetOutputName()) != 0)) {

                    // New printer installed so get the new printer device now.
                    delete Printer;
                    Printer = new TPrinter(this);
                }
            } else {
                // No printer installed (GetProfileString failed).
                delete Printer;
                Printer = new TPrinter(this);
            }
        }
    }
}

TResult TQConBridgeApp::EvFileSelected(TParam1 p1,TParam2)
{
   char filename[MAXPATH];
   TBridgeDocManager *docManager;

   docManager = TYPESAFE_DOWNCAST(GetDocManager(),TBridgeDocManager);

   // Don't open a new one if the current document couldn't be closed.
   if (!docManager->CloseTheDocument())
      return 0;

  GetMenuText(p1,filename,sizeof(filename));

   TDocTemplate* tpl = docManager->MatchTemplate(filename);
   if (tpl)
      {
      tpl->CreateDoc(filename);
      docManager->UpdateMainWindowCaption();
      }

   SaveMenuChoice(filename);

   return 0;
}


void TQConBridgeApp::CeFileSend(TCommandEnabler& ce)
{
  ce.Enable((GetDocManager()->GetCurrentDoc() != 0)
            && ApxMail->IsMAPIAvailable());
}


void TQConBridgeApp::CmFileSend ()
{
  // Check to see if a document exists
  //
  TDocument* currentDoc = GetDocManager()->GetCurrentDoc();

  if (currentDoc) {
    TAPointer<char> savedPath = new char[_MAX_PATH];
    TAPointer<char> docName = new char[_MAX_PATH];

    bool dirtyState = currentDoc->IsDirty();

    if (currentDoc->GetDocPath())
      strcpy(savedPath, currentDoc->GetDocPath());
    else
      strcpy(savedPath, "");

    if (currentDoc->GetTitle())
      strcpy(docName, currentDoc->GetTitle());
    else
      strcpy(docName, "Document");

    TFileName tempFile(TFileName::TempFile);

    currentDoc->SetDocPath(tempFile.Canonical().c_str());
    currentDoc->Commit(true);

    currentDoc->SetDocPath(savedPath);
    currentDoc->SetDirty(dirtyState);

    ApxMail->SendDocuments(GetMainWindow(), tempFile.Canonical().c_str(), docName, false);

    tempFile.Remove();
  }
}


char* TQConBridgeApp::GetRegistryKey(const char* subKey,char* buffer,int buflen)
{
   TModuleVersionInfo verInfo(*this);
   const char far* company;
   const char far* product;
   const char far* version;
   ostrstream os;

   verInfo.GetInfoString("CompanyName",company);
   verInfo.GetProductName(product);
   verInfo.GetProductVersion(version);

   os << "Software" << "\\"
      << company    << "\\"
      << product    << "\\"
      << version;

   if (subKey)
      os << "\\" << subKey;

   os << ends;

   strncpy(buffer,os.str(),buflen);
   delete[] os.str();

   return buffer;
}

//--------------------------------------------------------
// TQConBridgeApp
// ~~~~~
// Process command line parameters.
//
void TQConBridgeApp::ProcessCmdLine(char * CmdLine)
{
  TCmdLine cmd(CmdLine);
  TFileList* files = new TFileList;
  char file[_MAX_PATH];

  while (cmd.Kind != TCmdLine::Done)
    {
    if (cmd.Kind == TCmdLine::Name)
      {
//      strncpy(file,cmd.Token.cmd.TokenLen);
// Long File Name.Ext is parsed ad three seperate names
      strcpy(file,CmdLine);
      files->Add(new TFileDrop(file,TPoint(0,0),true,this));
      break;
      }
    cmd.NextToken();
    }

  // If there are files to be opened, submit them to AddFiles now.
  if (files->GetItemsInContainer() > 0)
     AddFiles(files);
  delete files;
}


//--------------------------------------------------------
// TQConBridgeApp
// ~~~~~
// Register application info.
//
void TQConBridgeApp::RegisterInfo()
{
  TAPointer<char> buffer = new char[_MAX_PATH];
  char appIdentifier[] = "QConBridge";
  char humanName[] = "WSDOT QConBridge";
  char openCommand[] = "QConBridge\\shell\\open\\command";

  GetModuleFileName(buffer, _MAX_PATH);

  TRegKey(TRegKey::GetClassesRoot(),".qcb").SetDefValue(0,REG_SZ,appIdentifier,strlen(appIdentifier));
  TRegKey(TRegKey::GetClassesRoot(),appIdentifier).SetDefValue(0,REG_SZ,humanName,strlen(humanName));
  strcat(buffer," %1");
  TRegKey(TRegKey::GetClassesRoot(),openCommand).SetDefValue(0,REG_SZ,buffer,strlen(buffer));
}


//--------------------------------------------------------
// TQConBridgeApp
// ~~~~~
// Unregister application info.
//
void TQConBridgeApp::UnRegisterInfo()
{
  TAPointer<char> buffer = new char[_MAX_PATH];
  char appIdentifier[] = "QConBridge";

  GetModuleFileName(buffer, _MAX_PATH);

  TRegKey::GetClassesRoot().DeleteKey("QConBridge\\shell\\open\\command");
  TRegKey::GetClassesRoot().DeleteKey("QConBridge\\shell\\open");
  TRegKey::GetClassesRoot().DeleteKey("QConBridge\\shell");
  TRegKey::GetClassesRoot().DeleteKey(appIdentifier);

  char key[256];
  TRegKey::GetCurrentUser().NukeKey(GetRegistryKey(0,key,sizeof(key)));

  Splash->CloseWindow();
  GetMainWindow()->MessageBox("QConBridge has been removed from the System Registry\nand will now terminate.","QConBridge - Unregister",MB_OK | MB_ICONEXCLAMATION);
  ::PostQuitMessage(WM_QUIT);
}

void TQConBridgeApp::CmToggleToolbar()
{
   // should be doing this with an ID instead of a reference like decframe does
   if (ApxHarbor->IsDockableVisible(IDW_TOOLBAR))
      ApxHarbor->HideDockable(IDW_TOOLBAR);
   else
      ApxHarbor->ShowDockable(IDW_TOOLBAR);
}

void TQConBridgeApp::CeToggleToolbar(TCommandEnabler& ce)
{
   ce.SetCheck(ApxHarbor->IsDockableVisible(IDW_TOOLBAR) ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

