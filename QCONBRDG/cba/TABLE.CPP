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

#include <qconbrdg\table.h>
#include <string.h>
#include <mem.h>
#include <stdlib.h>
#include <strstrea.h>
#include <iomanip.h>
#include <esl\engnotat.h>

TTable::TTable(SystemOfUnits u,const char *Title)
{
   units = u;
   title = NULL;
   SetTitle(Title);
}

TTable::~TTable()
{
   delete[] title;
}

void TTable::SetUnits(SystemOfUnits newUnits)
{
   units = newUnits;
}

void TTable::SetTitle(const char* buffer)
{
   delete[] title;

   title = new char[strlen(buffer)+1];
   strcpy(title,buffer);
}

char *TTable::GetTitle(char* buffer,unsigned int bufSize) const
{
   memcpy(buffer,title,min(bufSize,strlen(title)+1));
   buffer[bufSize-1] = 0; // null terminate the array

   return buffer;
}

void TTable::SetRecordGroupCount(int c)
{
   recordGrpCount = c;
}

int TTable::GetRecordGroupCount() const
{
   return recordGrpCount;
}

int TTable::GetTableWidth() const
{
   int i;
   int width = 0;

   for (i = 0; i < GetNumFields(); i++)
      width += GetFieldWidth(i);

   return width;
}

ostream& operator << (ostream &os,const TTable &t)
{
   char buffer[256];
   int i,j;
   int numFields;
   int numRecords;

   t.GetTitle(buffer,256);
   os << buffer << endl;

   numFields = t.GetNumFields();
   int *fieldWidth = new int[numFields];

   for (i = 0; i < numFields; i++)
      {
      fieldWidth[i] = t.GetFieldWidth(i);
      t.GetFieldName(i,buffer,256);
      os << setw(fieldWidth[i]) << buffer;
      }
   os << endl;

   numRecords = t.GetNumRecords();
   for (i = 0; i < numRecords; i++)
      {
      for (j = 0; j < numFields; j++)
         {
         t.GetEntry(i,j,buffer,256);
         os << setw(fieldWidth[j]) << buffer;
         }
      os << endl;
      }

   delete fieldWidth;

   return os;
}

//
// TLoadTable
//

TLoadTable::TLoadTable(SystemOfUnits u,const char *Title) :
TTable(u,Title)
{
   data = new TIDataContainer(10,0,10);
}

TLoadTable::~TLoadTable()
{
   delete data;
}

int TLoadTable::GetNumFields() const
{
   return 4;
}

int TLoadTable::GetNumRecords() const
{
   return data->GetItemsInContainer();
}

int TLoadTable::GetFieldWidth(int field) const
{
   int w[4] = {5,6,16,16};
   return w[field];
}

char *TLoadTable::GetFieldName(int field,char *buffer,unsigned int bufSize) const
{
   const char *USheadings[] = {"Span","Point","Shear(lbs)","Moment(ft-lbs)"};
   const char *SIheadings[] = {"Span","Point","Shear(N)","Moment(N-m)"};
   memset(buffer,0,bufSize);
   strncat(buffer,(units == unitsSI) ? SIheadings[field] : USheadings[field],bufSize);

   return buffer;
}

char *TLoadTable::GetEntry(int record,int field,char *buffer,unsigned int bufSize) const
{
   ostrstream os;
   Data *rec = (*data)[record];
   double value;

   switch(field)
      {
      case 0:
           os << rec->spanId << ends;
           break;

      case 1:
           os << rec->poiOffset << ends;
           break;

      case 2:
           value = DerivedUnits::Force(rec->shear).ConvertTo((units == unitsSI) ? SIConstants::newton : USConstants::pound).value();
           os << EngNotation(value,3) << ends;
           break;

      case 3:
           value =  DerivedUnits::Moment(rec->moment).ConvertTo((units == unitsSI) ? SIConstants::newton_meter : USConstants::pound_feet).value();
           os << EngNotation(value,3) << ends;
           break;
      }

   memset(buffer,0,bufSize);
   strncat(buffer,os.str(),bufSize);
   delete[] os.str();

   return buffer;
}

void TLoadTable::AddEntry(int spanId,int poiOffset,double shear,double moment)
{
   Data *record = new Data;
   record->spanId = spanId;
   record->poiOffset = poiOffset;
   record->shear = shear;
   record->moment = moment;

   data->Add(record);
}

//
// TEnvelopeTable
//

TEnvelopeTable::TEnvelopeTable(SystemOfUnits u,const char *Title) :
TTable(u,Title)
{
   data = new TIDataContainer(10,0,10);
}

TEnvelopeTable::~TEnvelopeTable()
{
   delete data;
}

int TEnvelopeTable::GetNumFields() const
{
   return 6;
}

int TEnvelopeTable::GetNumRecords() const
{
   return data->GetItemsInContainer();
}

int TEnvelopeTable::GetFieldWidth(int field) const
{
   int w[6] = {5,6,16,16,16,16};
   return w[field];
}

char *TEnvelopeTable::GetFieldName(int field,char *buffer,unsigned int bufSize) const
{
   const char *USheadings[] = {"Span","Point","Min Shear(lbs)","Max Shear(lbs)","Min Moment(ft-lbs)","Max Moment(ft-lbs)"};
   const char *SIheadings[] = {"Span","Point","Min Shear(N)","Max Shear(N)","Min Moment(N-m)","Max Moment(N-m)"};
   memset(buffer,0,bufSize);
   strncat(buffer,(units == unitsSI) ? SIheadings[field] : USheadings[field],bufSize);

   return buffer;
}

char *TEnvelopeTable::GetEntry(int record,int field,char *buffer,unsigned int bufSize) const
{
   ostrstream os;
   Data *rec = (*data)[record];
   double value;

   switch(field)
      {
      case 0:
           os << rec->spanId << ends;
           break;

      case 1:
           os << rec->poiOffset << ends;
           break;

      case 2:
           value =  DerivedUnits::Force(rec->minShear).ConvertTo((units == unitsSI) ? SIConstants::newton : USConstants::pound).value();
           os << EngNotation(value,3) << ends;
           break;

      case 3:
           value =  DerivedUnits::Force(rec->maxShear).ConvertTo((units == unitsSI) ? SIConstants::newton : USConstants::pound).value();
           os << EngNotation(value,3) << ends;
           break;

      case 4:
           value =  DerivedUnits::Moment(rec->minMoment).ConvertTo((units == unitsSI) ? SIConstants::newton_meter : USConstants::pound_feet).value();
           os << EngNotation(value,3) << ends;
           break;

      case 5:
           value =  DerivedUnits::Moment(rec->maxMoment).ConvertTo((units == unitsSI) ? SIConstants::newton_meter : USConstants::pound_feet).value();
           os << EngNotation(value,3) << ends;
           break;
      }

   memset(buffer,0,bufSize);
   strncat(buffer,os.str(),bufSize);
   delete[] os.str();

   return buffer;
}

void TEnvelopeTable::AddEntry(int spanId,int poiOffset,double minShear,double maxShear,double minMoment,double maxMoment)
{
   Data *record = new Data;
   record->spanId = spanId;
   record->poiOffset = poiOffset;
   record->minShear  = minShear;
   record->maxShear  = maxShear;
   record->minMoment = minMoment;
   record->maxMoment = maxMoment;

   data->Add(record);
}

///////////////////

TReactionTable::TReactionTable(SystemOfUnits u,const char *Title)  :
TTable(u,Title)
{
   data = new TIDataContainer(10,0,10);
}

TReactionTable::~TReactionTable()
{
	delete data;
}

int TReactionTable::GetNumFields() const
{
	return 4; // pier, fx, fy, mz
}

int TReactionTable::GetNumRecords() const
{
   return data->GetItemsInContainer();
}

int TReactionTable::GetFieldWidth(int field) const
{
	int w[4] = {5,16,16,16};
   return w[field];
}

char *TReactionTable::GetFieldName(int field,char *buffer,unsigned int bufSize) const
{
	const char* USheadings[] = {"Pier","Fx(lbs)","Fy(lbs)","Mz(ft-lbs)"};
	const char* SIheadings[] = {"Pier","Fx(N)","Fy(N)","Mz(N-m)"};
   memset(buffer,0,bufSize);
   strncat(buffer,(units == unitsSI) ? SIheadings[field] : USheadings[field],bufSize);

   return buffer;
}

char *TReactionTable::GetEntry(int record,int field,char *buffer,unsigned int bufSize) const
{
   ostrstream os;
   Data *rec = (*data)[record];
   double value;
   switch(field)
      {
      case 0:
           os << rec->pierId << ends;
           break;

      case 1:
           value =  DerivedUnits::Force(rec->fx).ConvertTo((units == unitsSI) ? SIConstants::newton : USConstants::pound).value();
           os << EngNotation(value,3) << ends;
           break;

      case 2:
           value = DerivedUnits::Force(rec->fy).ConvertTo((units == unitsSI) ? SIConstants::newton : USConstants::pound).value();
           os << EngNotation(value,3) << ends;
           break;

      case 3:
           value =  DerivedUnits::Moment(rec->mz).ConvertTo((units == unitsSI) ? SIConstants::newton_meter : USConstants::pound_feet).value();
           os << EngNotation(value,3) << ends;
           break;
      }

   memset(buffer,0,bufSize);
   strncat(buffer,os.str(),bufSize);
   delete[] os.str();

   return buffer;
}

void TReactionTable::AddEntry(int pierId,double fx,double fy,double mz)
{
   Data *record = new Data;
   record->pierId = pierId;
   record->fx = fx;
   record->fy = fy;
   record->mz = mz;

   data->Add(record);
}

/////////////////////////////////////////
TReactionEnvelopeTable::TReactionEnvelopeTable(SystemOfUnits u,const char *Title) :
TTable(u,Title)
{
   data = new TIDataContainer(10,0,10);
}

TReactionEnvelopeTable::~TReactionEnvelopeTable()
{
	delete data;
}

int TReactionEnvelopeTable::GetNumFields() const
{
	return 7; // pier, fxmin,fxmax,fymin,fymax,mzmin,mzmax
}

int TReactionEnvelopeTable::GetNumRecords() const
{
   return data->GetItemsInContainer();
}

int TReactionEnvelopeTable::GetFieldWidth(int field) const
{
   int w[7] = {5,14,14,14,14,14,14};
   return w[field];
}

char *TReactionEnvelopeTable::GetFieldName(int field,char *buffer,unsigned int bufSize) const
{
	const char* USheadings[] = {"Pier","FxMin(lbs)","FxMax(lbs)","FyMin(lbs)","FyMax(lbs)","MzMin(ft-lbs)","MzMax(ft-lbs)"};
	const char* SIheadings[] = {"Pier","FxMin(N)","FxMax(N)","FyMin(N)","FyMax(N)","MzMin(N-m)","MzMax(N-m)"};
   memset(buffer,0,bufSize);
   strncat(buffer,(units == unitsSI) ? SIheadings[field] : USheadings[field],bufSize);

   return buffer;
}

char *TReactionEnvelopeTable::GetEntry(int record,int field,char *buffer,unsigned int bufSize) const
{
   ostrstream os;
   Data *rec = (*data)[record];
   double value;

   switch(field)
      {
      case 0:
           os << rec->pierId << ends;
           break;

      case 1:
           value =  DerivedUnits::Force(rec->fx_min).ConvertTo((units == unitsSI) ? SIConstants::newton : USConstants::pound).value();
           os << EngNotation(value,3) << ends;
           break;

      case 2:
           value =  DerivedUnits::Force(rec->fx_max).ConvertTo((units == unitsSI) ? SIConstants::newton : USConstants::pound).value();
           os << EngNotation(value,3) << ends;
           break;

      case 3:
           value =  DerivedUnits::Force(rec->fy_min).ConvertTo((units == unitsSI) ? SIConstants::newton : USConstants::pound).value();
           os << EngNotation(value,3) << ends;
           break;

      case 4:
           value =  DerivedUnits::Force(rec->fy_max).ConvertTo((units == unitsSI) ? SIConstants::newton : USConstants::pound).value();
           os << EngNotation(value,3) << ends;
           break;

      case 5:
           value =  DerivedUnits::Moment(rec->mz_min).ConvertTo((units == unitsSI) ? SIConstants::newton_meter : USConstants::pound_feet).value();
           os << EngNotation(value,3) << ends;
           break;

      case 6:
           value =  DerivedUnits::Moment(rec->mz_max).ConvertTo((units == unitsSI) ? SIConstants::newton_meter : USConstants::pound_feet).value();
           os << EngNotation(value,3) << ends;
           break;
      }

   memset(buffer,0,bufSize);
   strncat(buffer,os.str(),bufSize);
   delete[] os.str();

   return buffer;
}

void TReactionEnvelopeTable::AddEntry(int pierId,double fx_min,double fx_max,double fy_min,double fy_max,double mz_min,double mz_max)
{
   Data *record = new Data;
   record->pierId = pierId;

   record->fx_min = fx_min;
   record->fy_min = fy_min;
   record->mz_min = mz_min;

   record->fx_max = fx_max;
   record->fy_max = fy_max;
   record->mz_max = mz_max;

   data->Add(record);
}

