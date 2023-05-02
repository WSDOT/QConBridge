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

#if !defined(__wsdotprt_h)              // Sentry, use file only if it's not already included.
#define __wsdotprt_h

#include <owl\printer.h>

//{{TPrintout = TWSDOTPrintout}}
class TWSDOTPrintout : public TPrintout {
public:
    TWSDOTPrintout (TPrinter *printer,const char far* title,int lm = 127,int tm = 127,int bm = 127,int rm = 127);
    virtual ~TWSDOTPrintout ();

protected:
   int leftMargin;
   int topMargin;
   int rightMargin;
   int bottomMargin;
   TRect usableRect; // total usable area
   TRect headerRect; // area where header is printed
   TRect footerRect; // area where footer is printed
   TRect pageRect;   // area that can be used for reporting (usableRect-headerRect)
   TPrinter *Printer;
   void ComputeUsableRect();
   virtual void ComputeHeaderRect();
   virtual void ComputeFooterRect();
   void ComputePageRect();

   void PrintBorder();
   virtual void PrintHeader();
   virtual void PrintFooter();

//{{TWSDOTPrintoutVIRTUAL_BEGIN}}
public:
    virtual void PrintPage (int page, TRect& rect, unsigned flags);
    virtual void BeginDocument (int startPage, int endPage, unsigned flags);
//{{TWSDOTPrintoutVIRTUAL_END}}
};    //{{TWSDOTPrintout}}


#endif                                      // __wsdotprt_h sentry.
