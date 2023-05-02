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

#include <qconbrdg\spantbl.h>
 
SpanTableData &SpanTableData::operator = (SpanTableData &table)
{
   rows->Flush();

   SpanTableData::RowIterator i(*(table.rows));
   while(i)
      {
      SpanTableData::Row *row = i++;
      rows->Add(new SpanTableData::Row(*row));
      }

   return *this;
}

void SpanTableData::Sort()
{
   // A simple bubble sort is implemented here. If sorting proves to be
   // to slow, a more safisticated method could be employed.
   Row *temp;
   int i,j;

   i = rows->GetItemsInContainer() - 1;

   while(i >= 0)
      {
      for (j = 0; j < i; j++)
         {
         if ( *(*rows)[j+1] < *(*rows)[j] )
            {
            temp = (*rows)[j+1];
            (*rows)[j+1] = (*rows)[j];
            (*rows)[j] = temp;
            }
         }
      i--;
      }
}

SpanTableData::Row::Row(bool isSI)
{
   if (isSI)
      {
      loc = DerivedUnits::Length(0*SIConstants::meter).value();
      Ax = DerivedUnits::Length2(1*SIConstants::millimeter2).value();
      Iz = DerivedUnits::Length4(1*SIConstants::millimeter4).value();
      ModE = DerivedUnits::Pressure(1*SIConstants::Pascal).value();
      unitWeight = DerivedUnits::UnitWeight(SIConstants::g.value()*SIConstants::newton_per_meter3).value();
      }
   else
      {
      loc = DerivedUnits::Length(0*USConstants::feet).value();
      Ax = DerivedUnits::Length2(1*USConstants::inch2).value();
      Iz = DerivedUnits::Length4(1*USConstants::inch4).value();
      ModE = DerivedUnits::Pressure(1*USConstants::kip_per_inch2).value();
      unitWeight = DerivedUnits::UnitWeight(1*USConstants::pound_per_feet3).value();
      }

   s_gMoment = 1;
   s_gShear = 1;
   f_gMoment = 1;
   f_gShear = 1;
}

SpanTableData::Row::Row(const Row &r)
{
   loc = r.loc;
   Ax = r.Ax;
   Iz = r.Iz;
   ModE = r.ModE;
   unitWeight = r.unitWeight;
   s_gMoment = r.s_gMoment;
   s_gShear = r.s_gShear;
   f_gMoment = r.f_gMoment;
   f_gShear = r.f_gShear;
}

