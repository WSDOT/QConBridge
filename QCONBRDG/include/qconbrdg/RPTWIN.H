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

#if !defined(__rptwin_h)              // Sentry, use file only if it's not already included.
#define __rptwin_h


#include <owl\window.h>
#include <classlib\arrays.h>


//{{TWindow = TReportWindow}}
class TReportWindow : public TWindow {
public:
    TReportWindow (TWindow* parent,const char *filename, const char far* title = 0, TModule* module = 0);
    virtual ~TReportWindow ();

    int NumLines();
    void GetLine(int i,char *buffer,int maxChar);
protected:
    typedef TIArrayAsVector<string> TIStringArray;
    typedef TIArrayAsVectorIterator<string> TIStringIterator;

    string Filename;
    TIStringArray *lines;

    int cxChar;
    int cyChar;

    void ReadFile();
    void OpenReportFile(const char* filename);
    void CloseReportFile();
    bool WriteToFile(const char *filename);

    void UpdateScroller();

//{{TReportWindowVIRTUAL_BEGIN}}
public:
    virtual void Paint (TDC& dc, bool erase, TRect& rect);
    virtual void SetupWindow ();
//{{TReportWindowVIRTUAL_END}}
};    //{{TReportWindow}}


#endif                                      // __rptwin_h sentry.

