///////////////////////////////////////////////////////////////////////
// OWLTools - Extension Library for the Borland OWL Framework
// Copyright (C) 1999  Washington State Department of Transportation
//                     Bridge and Structures Office
//
// This library was developed as part of the Alternate Route Project
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the Alternate Route Library Open Source License as published by 
// the Washington State Department of Transportation, Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but is distributed 
// AS IS, WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
// or FITNESS FOR A PARTICULAR PURPOSE. See the Alternate Route Library Open Source 
// License for more details.
//
// You should have received a copy of the Alternate Route Library Open Source License 
// along with this program; if not, write to the Washington State Department of 
// Transportation, Bridge and Structures Office, 4500 3rd Ave SE - P.O. Box  47340, 
// Olympia, WA 98503, USA or e-mail Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Revision Log
// ============
// May 1997 - Created, Richard Brice
// Oct 1999 - Released as open source

#include <owltools\winplace.h>
#include <winsys\registry.h>
#include <winsys\uimetric.h>

TWindowPlacement::TWindowPlacement()
{
   length = sizeof(WINDOWPLACEMENT);
}

TWindowPlacement::TWindowPlacement(WINDOWPLACEMENT& wp)
{
   length = sizeof(WINDOWPLACEMENT);

   flags = wp.flags;
   showCmd = wp.showCmd;
   ptMinPosition = wp.ptMinPosition;
   ptMaxPosition = wp.ptMaxPosition;
   rcNormalPosition = wp.rcNormalPosition;
}

TWindowPlacement::~TWindowPlacement()
{
}

TWindowPlacementFlag TWindowPlacement::GetFlags() const
{
   return (TWindowPlacementFlag)flags;
}

void TWindowPlacement::SetFlags(TWindowPlacementFlag uFlags)
{
   flags = uFlags;
}

uint TWindowPlacement::GetShowState() const
{
   return showCmd;
}

void TWindowPlacement::SetShowState(uint uShowCmd)
{
   showCmd = uShowCmd;
}

void TWindowPlacement::GetMinPosition(TPoint& ptMin) const
{
    ptMin.x = ptMinPosition.x;
    ptMin.y = ptMinPosition.y;
}

void TWindowPlacement::SetMinPosition(const TPoint& ptMin)
{
    ptMinPosition.x = ptMin.x;
    ptMinPosition.y = ptMin.y;
}

void TWindowPlacement::GetMaxPosition(TPoint& ptMax) const
{
    ptMax.x = ptMaxPosition.x;
    ptMax.y = ptMaxPosition.y;
}

void TWindowPlacement::SetMaxPosition(const TPoint& ptMax)
{
    ptMaxPosition.x = ptMax.x;
    ptMaxPosition.y = ptMax.y;
}

void TWindowPlacement::GetNormalPosition(TRect& rcNormal) const
{
   rcNormal.left   = rcNormalPosition.left;
   rcNormal.right  = rcNormalPosition.right;
   rcNormal.top    = rcNormalPosition.top;
   rcNormal.bottom = rcNormalPosition.bottom;
}

void TWindowPlacement::SetNormalPosition(const TRect& rcNormal)
{
   rcNormalPosition.left   = rcNormal.left;
   rcNormalPosition.right  = rcNormal.right;
   rcNormalPosition.top    = rcNormal.top;
   rcNormalPosition.bottom = rcNormal.bottom;
}

void TWindowPlacement::SaveToRegistry(TWindow* window,const char* regKey)
{
   // Update window placement with current values
   window->GetWindowPlacement(this);

   TRegKey(TRegKey::CurrentUser(),regKey).SetValue("flags",REG_BINARY,(uint8*)&flags,sizeof(flags));
   TRegKey(TRegKey::CurrentUser(),regKey).SetValue("showCmd",REG_BINARY,(uint8*)&showCmd,sizeof(showCmd));
   TRegKey(TRegKey::CurrentUser(),regKey).SetValue("rcNormal",REG_BINARY,(uint8*)&rcNormalPosition,sizeof(rcNormalPosition));
}

void TWindowPlacement::RestoreFromRegistry(TWindow* window,const char* regKey)
{
   uint32 sizeFlags    = sizeof(flags);
   uint32 sizeShowCmd  = sizeof(showCmd);
   uint32 sizeRcNormal = sizeof(rcNormalPosition);

   // Call GetWindowPlacement() so that Windows provides ptMinPosition and ptMaxPosition
   window->GetWindowPlacement(this);

   if ( (TRegKey(TRegKey::CurrentUser(),regKey).QueryValue("flags",NULL,(uint8*)&flags,&sizeFlags) != ERROR_SUCCESS) ||
        (TRegKey(TRegKey::CurrentUser(),regKey).QueryValue("showCmd",NULL,(uint8*)&showCmd,&sizeShowCmd) != ERROR_SUCCESS) ||
        (TRegKey(TRegKey::CurrentUser(),regKey).QueryValue("rcNormal",NULL,(uint8*)&rcNormalPosition,&sizeRcNormal) != ERROR_SUCCESS))
      return;

   // Only place the window if its upper left corner is above the bottom right corner
   // of the screen
   if (rcNormalPosition.left <= (TUIMetric::CxScreen - TUIMetric::CxIcon) &&
       rcNormalPosition.top  <= (TUIMetric::CyScreen - TUIMetric::CyIcon))
      window->SetWindowPlacement(this);
}

