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

#if !defined TABLES_H
#define TABLES_H

#include <classlib\arrays.h>
//#include "mytypes.h"
#include <qconbrdg\bdoc.h> // for some reason, I can't include "mytypes.h", but
                  // if I include "bdoc.h", which includes "mytypes.h",
                  // everything is ok...

class TTable
{
public:
   TTable(SystemOfUnits u,const char *Title);
   virtual ~TTable();

   void SetTitle(const char *buffer);
   char *GetTitle(char *buffer,unsigned int bufSize) const;
   void SetUnits(SystemOfUnits newUnits);

   int GetTableWidth() const;
   virtual int GetNumFields() const = 0;
   virtual int GetNumRecords() const = 0;
   virtual int GetFieldWidth(int field) const = 0;
   virtual char *GetFieldName(int field,char *buffer,unsigned int bufSize) const = 0;
   virtual char *GetEntry(int record,int field,char *buffer,unsigned int bufSize) const = 0;
   void SetRecordGroupCount(int c);
   int GetRecordGroupCount() const;

   friend ostream& operator << (ostream &os,const TTable &m);

protected:
   char *title;
   SystemOfUnits units;
   int recordGrpCount;
};

class TLoadTable : public TTable
{
public:
   TLoadTable(SystemOfUnits u,const char *Title);
   ~TLoadTable();

   int GetNumFields() const;
   int GetNumRecords() const;
   int GetFieldWidth(int field) const;
   char *GetFieldName(int field,char *buffer,unsigned int bufSize) const;
   char *GetEntry(int record,int field,char *buffer,unsigned int bufSize) const;

   void AddEntry(int spanId,int poiOffset,double shear,double moment);

protected:
   struct Data
      {
      int spanId;
      int poiOffset;
      double shear;
      double moment;
      bool operator == (const Data &) const {return true;}
      };
   typedef TIArrayAsVector<Data> TIDataContainer;
   typedef TIArrayAsVectorIterator<Data> TIDataIterator;

   TIDataContainer *data;
};


class TEnvelopeTable : public TTable
{
public:
   TEnvelopeTable(SystemOfUnits u,const char *Title);
   ~TEnvelopeTable();

   int GetNumFields() const;
   int GetNumRecords() const;
   int GetFieldWidth(int field) const;
   char *GetFieldName(int field,char *buffer,unsigned int bufSize) const;
   char *GetEntry(int record,int field,char *buffer,unsigned int bufSize) const;

   void AddEntry(int spanId,int poiOffset,double minShear,double maxShear,double minMoment,double maxMoment);

protected:
   struct Data
      {
      int spanId;
      int poiOffset;
      double minShear;
      double maxShear;
      double minMoment;
      double maxMoment;
      bool operator == (const Data &) const {return true;}
      };
   typedef TIArrayAsVector<Data> TIDataContainer;
   typedef TIArrayAsVectorIterator<Data> TIDataIterator;

   TIDataContainer *data;
};

class TReactionTable : public TTable
{
public:
   TReactionTable(SystemOfUnits u,const char *Title);
   ~TReactionTable();

   virtual int GetNumFields() const;
   virtual int GetNumRecords() const;
   virtual int GetFieldWidth(int field) const;
   virtual char *GetFieldName(int field,char *buffer,unsigned int bufSize) const;
   virtual char *GetEntry(int record,int field,char *buffer,unsigned int bufSize) const;

   void AddEntry(int pierId,double fx,double fy,double mz);
protected:
   struct Data
      {
      int pierId;
      double fx,fy,mz;
      bool operator == (const Data &) const {return true;}
      };
   typedef TIArrayAsVector<Data> TIDataContainer;
   typedef TIArrayAsVectorIterator<Data> TIDataIterator;

   TIDataContainer *data;
};

class TReactionEnvelopeTable : public TTable
{
public:
   TReactionEnvelopeTable(SystemOfUnits u,const char *Title);
   ~TReactionEnvelopeTable();

   virtual int GetNumFields() const;
   virtual int GetNumRecords() const;
   virtual int GetFieldWidth(int field) const;
   virtual char *GetFieldName(int field,char *buffer,unsigned int bufSize) const;
   virtual char *GetEntry(int record,int field,char *buffer,unsigned int bufSize) const;

   void AddEntry(int pierId,double fx_min,double fx_max,double fy_min,double fy_max,double mz_min,double mz_max);
protected:
   struct Data
      {
      int pierId;
      double fx_min,fy_min,mz_min;
      double fx_max,fy_max,mz_max;
      bool operator == (const Data &) const {return true;}
      };
   typedef TIArrayAsVector<Data> TIDataContainer;
   typedef TIArrayAsVectorIterator<Data> TIDataIterator;

   TIDataContainer *data;
};

#endif