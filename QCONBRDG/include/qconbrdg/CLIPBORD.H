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

#if !defined(__clipbord_h)              // Sentry, use file only if it's not already included.
#define __clipbord_h


#include <owl\clipboar.h>
#include <bridge\lrfdspan.h>
#include <bridge\support.h>
#include <bridge\bload.h>
#include <iostream.h>
#include <qconbrdg\qcbapp.rh>            // Definition of all resources.

struct ClipboardLoadData
{
   int Type;
   int Lcase;
   bool IsFract;
   double Start;
   double End;
   double Mag;
};

//{{TClipboard = TQConBridgeClipboard}}
class TQConBridgeClipboard : public TClipboard {
public:
    TQConBridgeClipboard (HWND hWnd);
    virtual ~TQConBridgeClipboard ();

   void Put(LRFDSpan &span,Support &support,bool isRightSupport);
   void Get(LRFDSpan **span,Support **support,bool &isRightSupport);

   void Put(BridgeLoad &load);
   void Get(BridgeLoad **load,int &lcase);

   bool IsQCBSpanFormatAvailable() const;
   bool IsQCBLoadFormatAvailable() const;

protected:
   uint clipSpanFormat;
   uint clipLoadFormat;

private:
   void WriteSpan(ostream &ofile,LRFDSpan *span);
   void WriteSupport(ostream &ofile,Support *support);
   LRFDSpan* ReadSpan(istream &ifile);
   Support* ReadSupport(istream &ifile);
};    //{{TQConBridgeClipboard}}


#endif                                      // __clipbord_h sentry.

