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

#include "testapp.h"

#if !defined(__mainwnd_h)
#include "mainwnd.h"                        // Definition of client class.
#endif

#if !defined(__tabotdlg_h)
#include "tabotdlg.h"                        // Definition of about dialog.
#endif

//{{TestApp Implementation}}


//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(TestApp, TApplication)
//{{TestAppRSP_TBL_BEGIN}}
    EV_COMMAND(CM_FILENEW, CmFileNew),
    EV_COMMAND(CM_FILEOPEN, CmFileOpen),
    EV_COMMAND(CM_HELPABOUT, CmHelpAbout),
//{{TestAppRSP_TBL_END}}
END_RESPONSE_TABLE;


//////////////////////////////////////////////////////////
// TestApp
// =====
//
TestApp::TestApp () : TApplication("OWLTools Tester")
{

    // Common file file flags and filters for Open/Save As dialogs.  Filename and directory are
    // computed in the member functions CmFileOpen, and CmFileSaveAs.
    FileData.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    FileData.SetFilter("All Files (*.*)|*.*|");

    // INSERT>> Your constructor code here.
}


TestApp::~TestApp ()
{
    // INSERT>> Your destructor code here.
}


//////////////////////////////////////////////////////////
// TestApp
// =====
// Application intialization.
//
void TestApp::InitMainWindow ()
{
    if (nCmdShow != SW_HIDE)
        nCmdShow = (nCmdShow != SW_SHOWMINNOACTIVE) ? SW_SHOWNORMAL : nCmdShow;

    SDIDecFrame *frame = new SDIDecFrame(0, GetName(), 0, false);

    //
    // Assign ICON w/ this application.
    //
    frame->SetIcon(this, IDI_SDIAPPLICATION);

    //
    // Menu associated with window and accelerator table associated with table.
    //
    frame->AssignMenu(SDI_MENU);
    
    //
    // Associate with the accelerator table.
    //
    frame->Attr.AccelTable = SDI_MENU;

  
    SetMainWindow(frame);

    frame->SetMenuDescr(TMenuDescr(SDI_MENU));

    //
    // Borland Windows custom controls.
    //
    EnableBWCC();
}


//////////////////////////////////////////////////////////
// TestApp
// ===========
// Menu File New command
void TestApp::CmFileNew ()
{
}


//////////////////////////////////////////////////////////
// TestApp
// ===========
// Menu File Open command
void TestApp::CmFileOpen ()
{
    //
    // Display standard Open dialog box to select a file name.
    //
    *FileData.FileName = 0;

    TMainWindow *client = TYPESAFE_DOWNCAST(GetMainWindow()->GetClientWindow(), TMainWindow);     // Client window for the frame.
    if (client->CanClose())
        if (TFileOpenDialog(GetMainWindow(), FileData).Execute() == IDOK)
            OpenFile();
}


void TestApp::OpenFile (const char *fileName)
{
    if (fileName)
        strcpy(FileData.FileName, fileName);

}


//{{SDIDecFrame Implementation}}


SDIDecFrame::SDIDecFrame (TWindow *parent, const char far *title, TWindow *clientWnd, bool trackMenuSelection, TModule *module)
    : TDecoratedFrame(parent, title, clientWnd == 0 ? new TMainWindow(0, "") : clientWnd, trackMenuSelection, module)
{
    // INSERT>> Your constructor code here.

}


SDIDecFrame::~SDIDecFrame ()
{
    // INSERT>> Your destructor code here.

}


//////////////////////////////////////////////////////////
// TestApp
// ===========
// Menu Help About test.exe command
void TestApp::CmHelpAbout ()
{
    //
    // Show the modal dialog.
    //
    TAboutDlg(MainWindow).Execute();
}
int OwlMain (int , char* [])
{
    try {
        TestApp    app;
        return app.Run();
    }
    catch (xmsg& x) {
        ::MessageBox(0, x.why().c_str(), "Exception", MB_OK);
    }

    return -1;
}
