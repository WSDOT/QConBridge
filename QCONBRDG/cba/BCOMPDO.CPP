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

#include <qconbrdg\bcompdo.h>

TBridgeComponentDisplayObject::TBridgeComponentDisplayObject()
{
   displayLabels = false;
   labelFont = new TFont("Arial",-8);
   labelColor = TColor::LtGray;
   displaySegmentTicks = false;
}

TBridgeComponentDisplayObject::~TBridgeComponentDisplayObject()
{
   delete labelFont;
}

void TBridgeComponentDisplayObject::DisplayLabels(bool display)
{
   displayLabels = display;
}

bool TBridgeComponentDisplayObject::DisplayLabels() const
{
   return displayLabels;
}

void TBridgeComponentDisplayObject::SetLabelFont(TFont &font)
{
   delete labelFont;
   labelFont = new TFont(font);
}

TFont &TBridgeComponentDisplayObject::GetLabelFont() const
{
   return *labelFont;
}

TFont &TBridgeComponentDisplayObject::GetLabelDrawingFont() const
{
   return GetLabelFont();
}


void TBridgeComponentDisplayObject::SetLabelColor(TColor color)
{
   labelColor = color;
}

TColor TBridgeComponentDisplayObject::GetLabelColor() const
{
   return labelColor;
}

TColor TBridgeComponentDisplayObject::GetLabelDrawingColor() const
{
   return GetLabelColor();
}

void TBridgeComponentDisplayObject::DisplaySegmentTicks(bool display)
{
   displaySegmentTicks = display;
}

bool TBridgeComponentDisplayObject::DisplaySegmentTicks()
{
   return displaySegmentTicks;
}


