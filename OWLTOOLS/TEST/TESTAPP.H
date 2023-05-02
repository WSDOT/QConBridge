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

#if !defined(__testapp_h)              // Sentry, use file only if it's not already included.
#define __testapp_h


#if !defined(OWL_DECFRAME_H)
#include <owl\decframe.h>
#endif

#if !defined(OWL_APPLICAT_H)
#include <owl\applicat.h>
#endif

#if !defined(OWL_OPENSAVE_H)
#include <owl\opensave.h>
#endif

#include "testapp.rh"            // Definition of all resources.


//
// FrameWindow must be derived to override Paint for Preview and Print.
//
//{{TDecoratedFrame = SDIDecFrame}}
class SDIDecFrame : public TDecoratedFrame {
public:
    SDIDecFrame (TWindow *parent, const char far *title, TWindow *clientWnd, bool trackMenuSelection = false, TModule *module = 0);
    ~SDIDecFrame ();
};    //{{SDIDecFrame}}


//{{TApplication = TestApp}}
class TestApp : public TApplication {
private:

public:
    TestApp ();
    virtual ~TestApp ();

    TOpenSaveDialog::TData FileData;                    // Data to control open/saveas standard dialog.
    void OpenFile (const char *fileName = 0);
//{{TestAppVIRTUAL_BEGIN}}
public:
    virtual void InitMainWindow();
//{{TestAppVIRTUAL_END}}

//{{TestAppRSP_TBL_BEGIN}}
protected:
    void CmFileNew ();
    void CmFileOpen ();
    void CmHelpAbout ();
//{{TestAppRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TestApp);
};    //{{TestApp}}


#endif                                      // __testapp_h sentry.
