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

#if !defined(__editsupp_h)              // Sentry, use file only if it's not already included.
#define __editsupp_h



#include <owltools\prpsheet.h>
#include <owl\checkbox.h>
#include <owltools\unitedit.h>
#include <owltools\radiogrp.h>
#include <owltools\matedit.h>
#include <bridge\bridge.h>
#include <qconbrdg\mytypes.h>
#include <qconbrdg\qcbapp.rh>            // Definition of all resources.

class EditSupportXferBuffer
{
public:
   EditSupportXferBuffer(Support &support);
   Support *GetSupport() const;

private:
   uint    simple;
   double height;
   double offset;
   double ax;
   double iz;
   double modE;
   double unitWeight;
   uint    fixity;
   uint    hinge;
};


class TEditSupportDlg : public TMyPropertySheet
{
public:
   TEditSupportDlg(TWindow* parent,SystemOfUnits units,TModule* module = 0);
   ~TEditSupportDlg();
    virtual void SetupTransferBuffer(EditSupportXferBuffer *buffer);

protected:

   TPropertyPage* page1;
};


#endif                                      // __editsupp_h sentry.

