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

#if !defined(__stddload_h)              // Sentry, use file only if it's not already included.
#define __stddload_h


#include <owl\propsht.h>
#include <owltools\prpsheet.h>
#include <owl\checkbox.h>
#include <owltools\unitedit.h>
#include <qconbrdg\mytypes.h>
#include <qconbrdg\stddload.rh>

struct StdDeadLoadXferBuffer
{
   struct Page1
      {
      uint selfWeight;
      uint tbarrier;
      double tbLoad;
      } page1;

   struct Page2
      {
      uint utility;
      double utLoad;
      uint wearSurf;
      double wsLoad;
      } page2;

   StdDeadLoadXferBuffer()
      {
      page1.selfWeight = BF_CHECKED;
      page1.tbarrier   = BF_UNCHECKED;
      page1.tbLoad     = 0.00;
      page2.utility    = BF_UNCHECKED;
      page2.utLoad     = 0.00;
      page2.wearSurf   = BF_UNCHECKED;
      page2.wsLoad     = 0.00;
      }

   void EnableSelfWeight(bool enable)   { page1.selfWeight = (enable) ? BF_CHECKED : BF_UNCHECKED; }
   void EnableTBarrierLoad(bool enable) { page1.tbarrier = (enable) ? BF_CHECKED : BF_UNCHECKED; }
   void EnableUtilityLoad(bool enable)  { page2.utility  = (enable) ? BF_CHECKED : BF_UNCHECKED; }
   void EnableWearSurfLoad(bool enable) { page2.wearSurf = (enable) ? BF_CHECKED : BF_UNCHECKED; }

   bool EnableSelfWeight()   { return page1.selfWeight == BF_CHECKED; }
   bool EnableTBarrierLoad() { return page1.tbarrier   == BF_CHECKED; }
   bool EnableUtilityLoad()  { return page2.utility    == BF_CHECKED; }
   bool EnableWearSurfLoad() { return page2.wearSurf   == BF_CHECKED; }

   double GetTBarrierLoad() { return page1.tbLoad; }
   double GetUtilityLoad()  { return page2.utLoad; }
   double GetWearSurfLoad() { return page2.wsLoad; }
};


class TStdDeadLoadDlg : public TMyPropertySheet {
public:
    TStdDeadLoadDlg(TWindow* parent,
                    SystemOfUnits units,
                    TModule* module = 0);
    virtual ~TStdDeadLoadDlg ();
    virtual void SetupTransferBuffer(StdDeadLoadXferBuffer *buffer);

protected:

   TPropertyPage* page1;
   TPropertyPage* page2;
};


#endif                                      // __stddload_h sentry.

