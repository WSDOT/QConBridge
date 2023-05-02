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

#include <qconbrdg\matedit.h>

TMaterialDensityEdit::TMaterialDensityEdit(TWindow* parent, int resourceID,
                     const DerivedUnits::Density DisplayUnit,uint Width,
                     uint Precision,
                     int tagResId,const char *tagText,
                     TModule* module) :
   TDensityEdit(parent,resourceID,DisplayUnit,Width,
                Precision,tagResId,tagText,module)
{
}

void TMaterialDensityEdit::SetValue(double val)
{
   // Incoming value in Force/Length^3, must divide by g
   val /= SIConstants::g.value();

   TDensityEdit::SetValue(val);
}

double TMaterialDensityEdit::GetValue()
{
   // data is in Mass/Length^3, must multiply by g
   DerivedUnits::Density data(TNumberEdit::GetValue() * displayUnit);
   return data.value() * SIConstants::g.value();
}

