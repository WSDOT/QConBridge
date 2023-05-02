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

#if !defined(__bdocmgr_h)              // Sentry, use file only if it's not already included.
#define __bdocmgr_h


#include <owl\docmanag.h>

//{{TDocManager = TBridgeDocManager}}
class TBridgeDocManager : public TDocManager
{
public:
    TBridgeDocManager (int mode, TDocTemplate*& templateHead = ::DocTemplateStaticHead);
    virtual ~TBridgeDocManager ();

    bool CloseTheDocument();
    void UpdateMainWindowCaption();

protected:
    void CreateNamedView(const char *matchMe);

//{{TBridgeDocManagerVIRTUAL_BEGIN}}
public:
    virtual void CmFileNew ();
    virtual void CmFastNew ();
    virtual void CmFileOpen ();
    virtual void CmFileClose ();
    virtual void CmFileSaveAs ();
//{{TBridgeDocManagerVIRTUAL_END}}

//{{TBridgeDocManagerRSP_TBL_BEGIN}}
protected:
    void CmEditorView ();
    void CeEditorView (TCommandEnabler &tce);
    void CmMomentDiagram ();
    void CeMomentDiagram (TCommandEnabler &tce);
    void CmShearDiagram ();
    void CeShearDiagram (TCommandEnabler &tce);
    void CmTabularResults ();
    void CeTabularResults (TCommandEnabler &tce);
    void CmReport ();
    void CeReport (TCommandEnabler &tce);
//{{TBridgeDocManagerRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TBridgeDocManager);
};    //{{TBridgeDocManager}}

#endif                                      // __bdocmgr_h sentry.

