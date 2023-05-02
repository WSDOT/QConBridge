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

#if !defined (DOMAIN_H_)
#define DOMAIN_H_

#include <fem2d\femdll.h>

#if !defined( CLASSLIB_ARRAYS_H )
#include <classlib\arrays.h>
#endif

template <class T>
class TIDomainContainer : public TIArrayAsVector<T>
{
public:
   TIDomainContainer(int max,int min,int delta) :
      TIArrayAsVector<T>(max,min,delta) {};

   T *Get(int id);
   void Remove(int id,TShouldDelete::DeleteType dt = TShouldDelete::DefDelete);
};

template <class T>
T* TIDomainContainer<T>::Get(int id)
{
   for (unsigned i = 0; i < ArraySize(); i++)
      {
      if (Data[i] && id == Data[i]->GetID())
         return Data[i];
      }

   return NULL;
}

template <class T>
void TIDomainContainer<T>::Remove(int id,TShouldDelete::DeleteType dt)
{
   T *item;
   item = Get(id);
   Detach(item,dt);
}

#define TIDomainIterator TIArrayAsVectorIterator

#endif // DOMAIN_H_