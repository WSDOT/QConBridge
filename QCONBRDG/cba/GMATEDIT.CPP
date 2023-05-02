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

#include <qconbrdg\gmatedit.h>

TGMaterialDensityEdit::TGMaterialDensityEdit(TWindow* parent, int resourceID,
                      const DerivedUnits::Density DisplayUnit,uint Width,
                      uint Precision,
                      int tagResId,const char *tagText,
                      TModule* module) :
   TGDensityEdit(parent,resourceID,DisplayUnit,Width,Precision,tagResId,tagText,module)
{
}

void TGMaterialDensityEdit::SetValue(double val)
{
   // val is coming in as internal base units... (Force/Length^3)
   // must convert to display units (Mass/Length^3) before converting to string
   // and displaying it.
   string text;
   double value;

   // Convert to display units
   val /= SIConstants::g.value(); // now in Mass/Length^3
   DerivedUnits::Density data(val);
   value = data.ConvertTo(displayUnit).value(); // Now in display units

   GetValueString(value,text);
   SetText(text.c_str());
}

double TGMaterialDensityEdit::GetValue()
{
   // data is in Mass/Length^3, must multiply by g to get
   // Force/Length^3
   double value;
   value = TGDensityEdit::GetValue();
   return value * SIConstants::g.value();
}

void TGMaterialDensityEdit::DrawCell (TDC &dc, TRect &rc, const void *p, TGrid::Align align)
{
   // Because of the way the cells are drawn, the cell value is passed to this
   // function in internal base units (Newtons and meters) and not in display
   // units. To work around this problem we will have to draw the cell
   // ourselves.

   // Get the value
   double value = *static_cast<const double *>(p);

   // Convert to display units
   value /= SIConstants::g.value(); // now in Mass/Length^3
   DerivedUnits::Density data(value);
   value = data.ConvertTo(displayUnit).value(); // Now in display units

   // TGDoubleEdit does not provide for scientific notation so
   // setup the display string here and send it to TGEdit.
   // Notice that TGDoubleEdit::DrawCell() is complete bypassed.
   if (value == 0 && blankNull == true)
      TGEdit::DrawCell(dc,rc,0,align);
   else
      {
      string text;
      GetValueString(value,text);
      TGEdit::DrawCell(dc,rc,text.c_str(),align);
      }
}
