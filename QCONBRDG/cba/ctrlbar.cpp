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

#include <qconbrdg\ctrlbar.h>
#include <winsys\uimetric.h>
#include <winsys\registry.h>

TStickyHarbor::TStickyHarbor(const char* regKey,TDecoratedFrame& df) :
THarbor(df)
{
   mRegKey = new char[strlen(regKey)+1];
   strcpy(mRegKey,regKey);
}

TStickyHarbor::~TStickyHarbor()
{
   delete[] mRegKey;
}

void TStickyHarbor::SetupWindow()
{
   THarbor::SetupWindow();
   RestorePlacement();
}

void TStickyHarbor::Destroy(int retVal)
{
   SavePlacement();
   THarbor::Destroy(retVal);
}

void TStickyHarbor::SavePlacement()
{
   WINDOWPLACEMENT wp;
   wp.length = sizeof(WINDOWPLACEMENT);
   GetWindowPlacement(&wp);

   TRegKey(TRegKey::CurrentUser(),mRegKey).SetValue("flags",REG_BINARY,(uint8*)&wp.flags,sizeof(wp.flags));
   TRegKey(TRegKey::CurrentUser(),mRegKey).SetValue("showCmd",REG_BINARY,(uint8*)&wp.showCmd,sizeof(wp.showCmd));
   TRegKey(TRegKey::CurrentUser(),mRegKey).SetValue("rcNormal",REG_BINARY,(uint8*)&wp.rcNormalPosition,sizeof(wp.rcNormalPosition));
}

void TStickyHarbor::RestorePlacement()
{
   WINDOWPLACEMENT wp;
   uint32 sizeFlags    = sizeof(wp.flags);
   uint32 sizeShowCmd  = sizeof(wp.showCmd);
   uint32 sizeRcNormal = sizeof(wp.rcNormalPosition);

   wp.length = sizeof(WINDOWPLACEMENT);
   GetWindowPlacement(&wp);

   TRegKey(TRegKey::CurrentUser(),mRegKey).QueryValue("flags",NULL,(uint8*)&wp.flags,&sizeFlags);
   TRegKey(TRegKey::CurrentUser(),mRegKey).QueryValue("showCmd",NULL,(uint8*)&wp.showCmd,&sizeShowCmd);
   TRegKey(TRegKey::CurrentUser(),mRegKey).QueryValue("rcNormal",NULL,(uint8*)&wp.rcNormalPosition,&sizeRcNormal);

   // Only place the window if its upper left corner is above the bottom right corner
   // of the screen
   if (wp.rcNormalPosition.left <= (TUIMetric::CxScreen - TUIMetric::CxIcon) &&
       wp.rcNormalPosition.top  <= (TUIMetric::CyScreen - TUIMetric::CyIcon))
      SetWindowPlacement(&wp);
}
