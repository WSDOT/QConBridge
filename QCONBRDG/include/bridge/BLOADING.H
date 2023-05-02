///////////////////////////////////////////////////////////////////////
// Bridge32 - Bridge Modeling Framework
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

#if !defined BLOADING_H_
#define BLOADING_H_

#include <bridge\brdgdll.h>
#include <bridge\bload.h>

class _BRIDGECLASS Bridge;

class _BRIDGECLASS BridgeLoading
{
public:
   BridgeLoading(int id);
   ~BridgeLoading();
   int GetID() const;
   void AddLoad(BridgeLoad *load);
   BridgeLoad *GetLoad(int id) const;
   void RemoveLoad(int id);
   void RemoveLoad(BridgeLoad *load);
   void ReplaceLoad(int id,BridgeLoad *newLoad);
   BridgeLoadIterator *GetIterator();
   int NumLoads() const { return loads->GetItemsInContainer(); }

   int GetFemId() const { return femId; }

   bool operator == (const BridgeLoading &loading) const;

protected:
   int ID;
   BridgeLoadContainer *loads;
   int nextLoadID;
   int NextLoadID();
   int femId;

   friend Bridge;
};

typedef TIArrayAsVector<BridgeLoading>         BridgeLoadingContainer;
typedef TIArrayAsVectorIterator<BridgeLoading> BridgeLoadingIterator;

#endif // BLOADING_H_