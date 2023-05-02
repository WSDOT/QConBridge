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

#if !defined(__loadfact_h)              // Sentry, use file only if it's not already included.
#define __loadfact_h


#include <qconbrdg\qcbapp.rh>            // Definition of all resources.

#include <owltools\prpsheet.h>
#include <owltools\nbredit.h>

struct LoadFactorXferBuffer
{
   struct Type1
      {
      double dcMin,dcMax;
      double dwMin,dwMax;
      double ll;
      };

   struct Type2
      {
      double dc;
      double dw;
      double ll;
      };

   Type1 StrengthI;
   Type2 ServiceI;
   Type2 ServiceII;
   Type2 ServiceIII;
   Type2 Fatigue;
};

//{{TDialog = TLoadFactorDlg}}
class TLoadFactorDlg : public TMyPropertySheet {
public:
    TLoadFactorDlg(TWindow* parent,TModule* module = 0);
    virtual ~TLoadFactorDlg ();

    void SetupTransferBuffer(LoadFactorXferBuffer *buffer);

protected:

   TPropertyPage* page1;
   TPropertyPage* page2;
   TPropertyPage* page3;
   TPropertyPage* page4;
   TPropertyPage* page5;
};    //{{TLoadFactorDlg}}


#endif                                      // __loadfact_h sentry.

