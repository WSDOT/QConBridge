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

#if !defined(__mainwnd_h)              // Sentry, use file only if it's not already included.
#define __mainwnd_h

#if !defined(OWL_WINDOW_H)
#include <owl\window.h>
#endif

#include "testapp.rh"            // Definition of all resources.


//{{TWindow = TMainWindow}}
class TMainWindow : public TWindow {
public:
    TMainWindow (TWindow* parent, const char far* title = 0, TModule* module = 0);
    virtual ~TMainWindow ();
    void Paint(TDC &dc,bool erase,TRect &rect);

//{{TMainWindowRSP_TBL_BEGIN}}
protected:
    void CmHLS_RGB_RGB ();
    void CmHLS_Exception ();
    void CmHSV_Exception ();
    void CmHSV_RGB_RGB ();
    void CmTestIncludeExclude ();
    void CmTestRadioGroupBox ();
    void CmTestUnits ();
    void CmTestValidators ();
    void CmTestComboBoxes();
    void CmTestMapper();
//{{TMainWindowRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TMainWindow);
};    //{{TMainWindow}}


#endif                                      // __mainwnd_h sentry.
