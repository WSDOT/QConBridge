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

#include <qconbrdg\selobj.h>

TSelectableObject::TSelectableObject(bool selected)
{
   isSelected = selected;
   selColor = TColor::LtRed;
}

TSelectableObject::~TSelectableObject()
{
}

void TSelectableObject::SetSelColor(TColor color)
{
   selColor = color;
}

TColor TSelectableObject::GetSelColor() const
{
   return selColor;
}

void TSelectableObject::Selecting()
{
   isSelected = true;
}

void TSelectableObject::UnSelecting()
{
   isSelected = false;
}

bool TSelectableObject::Contains(TPoint &point) const
{
   return pickRegion.Contains(point);
}

bool TSelectableObject::IsSelected() const
{
   return isSelected;
}

TRegion &TSelectableObject::GetPickRegion()
{
   return pickRegion;
}

