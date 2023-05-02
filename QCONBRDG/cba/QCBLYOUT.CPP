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

#include <qconbrdg\qcblyout.h>

TQConBridgeLayoutMixIn::TQConBridgeLayoutMixIn(TPageLayout &layout,const char* path) :
layout(layout),
path(path)
{
}

TQConBridgeLayoutMixIn::~TQConBridgeLayoutMixIn()
{
}

void TQConBridgeLayoutMixIn::SetupPage(TModule::THandle module)
{
   // Setup margins
   layout.SetMargins(500,500,500,500);

   // Header
   layout.SetHeaderFont("Arial",-8,FW_BOLD);
   layout.AddHeaderItem(new TimeStamp(0,hfRight));
   layout.AddHeaderItem(new PageNumber(1,hfRight));
   layout.AddHeaderItem(new MiscText(path.c_str(),2,hfRight));

   const char far* company;
   const char far* division;
   const char far* product;
   const char far* version;
   const char far* relDate;
   TModuleVersionInfo verInfo(module);

   verInfo.GetInfoString("CompanyName",company);
   verInfo.GetInfoString("Division",division);
   verInfo.GetProductName(product);
   verInfo.GetProductVersion(version);
   verInfo.GetInfoString("ReleaseDate",relDate);

   char buffer[80];
   ostrstream os(buffer,sizeof(buffer));
   os << product << " " << version << " " << relDate << ends;

   layout.AddHeaderItem(new MiscText(company,0,hfLeft));
   layout.AddHeaderItem(new MiscText(division,1,hfLeft));
   layout.AddHeaderItem(new MiscText(buffer,2,hfLeft));
}

//
//
//

TQConBridgePageLayout::TQConBridgePageLayout(TPrintDC &pdc,const char* path,TModule::THandle module) :
TPageLayout(pdc),TQConBridgeLayoutMixIn(*this,path)
{
   SetupPage(module);
}

//
//
//

TQConBridgeTableLayout::TQConBridgeTableLayout(TPrintDC &pdc,const char* path,TModule::THandle module) :
TTableLayout(pdc),TQConBridgeLayoutMixIn(*this,path)
{
   SetupPage(module);
}

