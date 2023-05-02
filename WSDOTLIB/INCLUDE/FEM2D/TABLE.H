///////////////////////////////////////////////////////////////////////
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
// May 1997 - Created, Richard Brice, PE
// Oct 1999 - Released as Open Source

#if !defined (TABLE_H_)
#define TABLE_H_

#include <classlib/arrays.h>
#include "femall.h"

template <class T> class TITableRow : public TIArrayAsVector<T>
{
public:
   TITableRow(int upper,int delta) :
   TIArrayAsVector<T>(upper,0,delta) {};
   int NumberOfColumns() { return UpperBound(); };
   bool operator == (const TITableRow<T> &t)
      {
      return true;
      };
};

template <class T> class TITable
{
private:
   TIDomainContainer<TIResultContainer<T> > *Container;

public:
   TITable(int rowMax,int rowDelta,int colMax,int colDelta);

  ~TITable()
      {
      /*
      for (int i = 0; i < Container->GetItemsInContainer(); i++)
         {
         TITableRow<T> *row = (*Container)[i];
         delete row;
         }
      */
      delete Container;
      };

   void Add(T *t,int row,int col);

   T *Get(int row,int col)
      {
      TIResultContainer<T> *Row = (*Container)[row];
      if (Row)
         return (*Row)[col];
      else
         return NULL;
      };

   int NumberOfRows()
      {
      return Container->UpperBound();
      };
};

template <class T>
TITable<T>::TITable(int rowMax,int rowDelta,int colMax,int colDelta)
{
   Container = new TIDomainContainer<TITableRow<T> >(rowMax,rowDelta);
   for (int i = 0; i < rowMax; i++)
      {
      TITableRow<T> *row = new TITableRow<T>(colMax,colDelta);
      Container->AddAt(row,i);
      }
}

template <class T>
void TITable<T>::Add(T *t,int row,int col)
{
   if (row >= Container->ArraySize())
      {
      int lwr = Container->ArraySize();
      for (int i = lwr; i < row+1; i++)
         Container->AddAt(new TITableRow<T>(10,10),i); // should use colMax and colDelta
      }
   TITableRow<T> *Row = (*Container)[row];
   Row->AddAt(t,col);
}

#endif // TABLE_H_