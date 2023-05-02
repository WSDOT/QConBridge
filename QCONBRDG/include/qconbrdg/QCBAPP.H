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

#if !defined(__qcbapp_h)              // Sentry, use file only if it's not already included.
#define __qcbapp_h



#include <classlib\bags.h>
#include <owl\applicat.h>
#include <owl\decmdifr.h>
#include <owl\controlb.h>
#include <owl\textgadg.h>
#include <owl\printer.h>
#include <owl\splashwi.h>
#include <owl\docking.h>

#include <qconbrdg\mdiclnt.h>

#include <owl\hlpmanag.h>
#include <owl\rcntfile.h>
#include <owl/mailer.h>

#include <qconbrdg\hmaster.h>


// TFileDrop class Maintains information about a dropped file, its name, where it was dropped,
// and whether or not it was in the client area
class TFileDrop {
public:
    operator == (const TFileDrop& other) const {return this == &other;}

    char*   FileName;
    TPoint  Point;
    bool    InClientArea;

    TFileDrop (const char*, TPoint&, bool, TModule*);
    ~TFileDrop ();

    const char* WhoAmI ();
private:
    //
    // hidden to prevent accidental copying or assignment
    //
    TFileDrop (const TFileDrop&);
    TFileDrop & operator = (const TFileDrop&);
};

typedef TIBagAsVector<TFileDrop> TFileList;
typedef TIBagAsVectorIterator<TFileDrop> TFileListIter;


//{{TApplication = TQConBridgeApp}}
class TQConBridgeApp : public TApplication, public TRecentFiles
{
private:
    string          HelpFile;
    bool            HelpState;                          // Has the help engine been used.
    bool            ContextHelp;                        // SHIFT-F1 state (context sensitive HELP).
    HCURSOR         HelpCursor;                         // Context sensitive help cursor.
    bool            m_bShowLegalNotice;

    void SetupSpeedBar (TDecoratedMDIFrame *frame);
    TTextGadget    *modifiedTextGadget;
    void ProcessCmdLine(char * CmdLine);
    void RegisterInfo();
    void UnRegisterInfo();
    long ShowLegalNoticeAtStartup();
    long ShowLegalNotice(bool bGiveChoice);

public:
    TQConBridgeApp ();
    virtual ~TQConBridgeApp ();

    void CreateGadgets(TDockableControlBar* cb, bool server = false);

    TQConBridgeMDIClient  *mdiClient;
    bool Printing;
    TPrinter *GetPrinter();
    char* GetRegistryKey(const char* subKey,char* buffer,int buflen);
    void AddFiles (TFileList* files);

    ActivateHelp(TWindow* window, int helpFileContextId);
    DeactivateHelp();
    string& GetHelpFile();

//{{TQConBridgeAppVIRTUAL_BEGIN}}
public:
    virtual void InitMainWindow ();
    virtual void InitInstance ();
    virtual bool CanClose ();
    virtual bool ProcessAppMsg (MSG& msg);
    virtual bool IdleAction (long idleCount);
//{{TQConBridgeAppVIRTUAL_END}}

private:
   void ExcessiveFileMessage();
   TPrinter* Printer;
   TMailer*  ApxMail;
   THarborMaster*        ApxHarbor;
   TSplashWindow* Splash;
   TDib* dibSplash;

//{{TQConBridgeAppRSP_TBL_BEGIN}}
protected:
    void EvNewView (TView& view);
    void EvCloseView (TView& view);
    void CeFileSend(TCommandEnabler& ce);
    void CmFileSend();
    void CmHelpAbout();
    void CmHelpTopics();
    void CmToggleToolbar();
    void CeToggleToolbar(TCommandEnabler& ce);
    void CmLegalNotice();
    void EvDropFiles (TDropInfo drop);
    void EvWinIniChange (char far* section);
    TResult EvFileSelected(TParam1 p1,TParam2);
//{{TQConBridgeAppRSP_TBL_END}}

DECLARE_RESPONSE_TABLE(TQConBridgeApp);
};    //{{TQConBridgeApp}}


class TMyDecoratedMDIFrame : public TDecoratedMDIFrame
{
public:
    TMyDecoratedMDIFrame(const char far* title,
                       TResId          menuResId,
                       TMDIClient&     clientWnd = *new TMDIClient,
                       bool            trackMenuSelection = false,
                       TModule*        module = 0);

   void SetupWindow();  // HWND is first valid here
   void Destroy(int retVal);
   
   char far* GetClassName() {return "QConBridge";}
};

#endif                                      // __qcbapp_h sentry.
