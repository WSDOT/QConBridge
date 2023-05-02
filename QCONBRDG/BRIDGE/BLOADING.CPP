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

#include <bridge\bloading.h>

BridgeLoading::BridgeLoading(int id)
{
   ID = id;
   loads = new BridgeLoadContainer(10,0,10);
   nextLoadID = 0;
}

BridgeLoading::~BridgeLoading()
{
   delete loads;
}

int BridgeLoading::GetID() const
{
   return ID;
}

void BridgeLoading::AddLoad(BridgeLoad *load)
{
   load->owner = this;
   load->ID = NextLoadID();
   loads->Add(load);
}

BridgeLoad *BridgeLoading::GetLoad(int id) const
{
   BridgeLoad *load = 0;
   BridgeLoadIterator i(*loads);

   while (i)
      {
      load = i++;
      if (load->GetID() == id)
         return load;
      }

   CHECKX(load != 0,"BridgeLoading::GetLoad() - Load not found");

   return 0;
}

BridgeLoadIterator *BridgeLoading::GetIterator()
{
   return new BridgeLoadIterator(*loads);
}

void BridgeLoading::RemoveLoad(int id)
{
   BridgeLoad *load;
   load = GetLoad(id);

   if (load)
      {
      loads->Destroy(load);
      }
}

void BridgeLoading::RemoveLoad(BridgeLoad *load)
{
    loads->Destroy(load);
}

void BridgeLoading::ReplaceLoad(int id,BridgeLoad *newLoad)
{
   RemoveLoad(id);
   newLoad->owner = this;
   newLoad->ID = id;  // use the id of the load that is being replaced
   loads->Add(newLoad);
}

bool BridgeLoading::operator == (const BridgeLoading &loading) const
{
   return (ID == loading.GetID());
}

int BridgeLoading::NextLoadID()
{
   int id;
   id = nextLoadID;
   nextLoadID++;

   return id;
}
