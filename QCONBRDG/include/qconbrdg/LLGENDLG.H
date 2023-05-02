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

#if !defined(__llgendlg_h)              // Sentry, use file only if it's not already included.
#define __llgendlg_h


#include <owl\propsht.h>
#include <owltools\prpsheet.h>
#include <owltools\intedit.h>
#include <owltools\unitedit.h>
#include <owl\checkbox.h>

#include <qconbrdg\mytypes.h>

#include <qconbrdg\qcbapp.rh>            // Definition of all resources.

struct LLGenXferBuffer
{
   struct DesignTandem
      {
      int disable;
      } designTandem;

   struct VarAxleTruck
      {
      int disable;
      int numAxleInc;
      } varAxleTruck;

   struct DualTruckTrain
      {
      int disabled;
      double r1From;
      double r1To;
      int r1Inc;
      int range2Enabled;
      double r2From;
      double r2To;
      int r2Inc;
      int range3Enabled;
      double r3From;
      double r3To;
      int r3Inc;
      } dualTruckTrain, dualTandemTrain;

   struct FatigueTruck
      {
      int disable;
      } fatigueTruck;
};


class TLiveLoadGenDlg : public TMyPropertySheet
{
public:
    TLiveLoadGenDlg (TWindow* parent,SystemOfUnits units,TModule* module = 0);
    virtual ~TLiveLoadGenDlg ();

    void SetupTransferBuffer(LLGenXferBuffer *buffer);

protected:
   TPropertyPage* page1;
   TPropertyPage* page2;
   TPropertyPage* page3;
   TPropertyPage* page4;
   TPropertyPage* page5;
};


#endif                                      // __llgendlg_h sentry.

