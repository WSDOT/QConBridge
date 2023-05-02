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

#include <qconbrdg\qcbapp.h>
#include <classlib/date.h>

//
// It all starts here
//
int OwlMain (int , char* [])
{
#if defined (__USEDROPDEAD)
    TDate today;
    TDate deadDate(1,"June",1997);

    if (deadDate < today)
       {
       ::MessageBox(0,"This version has expired.\nContact WSDOT at bridge_support@wsdot.wa.gov for information on the latest version.","QConBridge",MB_OK | MB_ICONEXCLAMATION);
       return -1;
       }
#endif // __USEDROPDEAD

    try
       {
       int retval = 1;
       TQConBridgeApp app;
       retval = app.Run();
       return retval;
       }
    catch (xmsg& x)
       {
       ::MessageBox(0, x.why().c_str(), "Exception", MB_OK);
       }

    return -1;
}
