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

#if !defined(__qcbview_h)              // Sentry, use file only if it's not already included.
#define __qcbview_h



#include <qconbrdg\iconview.h>
#include <qconbrdg\bdoc.h>
#include <owl\printer.h>


//{{TView = TQConBridgeView}}
class TQConBridgeView : public TIconView {
public:
    TQConBridgeView (TBridgeDocument& doc);
    virtual ~TQConBridgeView ();
    virtual TPrintout *GetPrintout() = 0;
    virtual bool CanPrint() {return true;}
    TBridgeDocument &bridgeDoc;

//{{TQConBridgeViewRSP_TBL_BEGIN}}
protected:
    void CmAnalyze ();
    void CmFastAnalyze ();
    void CeAnalyze (TCommandEnabler &tce);
    void CmUnits ();
//{{TQConBridgeViewRSP_TBL_END}}

protected:
   void AnalyzeBridge();

DECLARE_RESPONSE_TABLE(TQConBridgeView);
};    //{{TQConBridgeView}}


#endif                                      // __qcbview_h sentry.

