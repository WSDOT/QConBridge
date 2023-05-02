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

#include <bridge\bload.h>
#include <bridge\bloading.h>
#include <bridge\span.h>

BridgeLoad::BridgeLoad(Span *aSpan)
{
   span = aSpan;

   Link();
}

BridgeLoad::~BridgeLoad()
{
   owner = NULL;
   Unlink();
}

void BridgeLoad::SetSpan(Span *aSpan)
{
   Unlink();
   span = aSpan;
   Link();
}

Span *BridgeLoad::GetSpan() const
{
   return span;
}

int BridgeLoad::GetID() const
{
   return ID;
}

BridgeLoading *BridgeLoad::GetOwner() const
{
   return owner;
}

void BridgeLoad::Link()
{
   if (span)
      span->AddLoad(this);
}

void BridgeLoad::Unlink()
{
   if (span)
      span->RemoveLoad(this);

   span = NULL;
}

bool BridgeLoad::operator == (const BridgeLoad &load) const
{
   return (load.GetID() == ID && load.GetOwner() == owner);
}

#if defined GEN_DEBUG_CODE
bool BridgeLoad::TestLink(Span *requestingSpan)
{
   return (requestingSpan->GetID() == span->GetID());
}
#endif

//
//
//
ConcBridgeLoad::ConcBridgeLoad(Span *aSpan,double loc,double mag,bool isfract) :
BridgeLoad(aSpan)
{
   startLoc = loc;
   magnitude = mag;
   isFract = isfract;
}

bool ConcBridgeLoad::IsFract() const
{
   return isFract;
}

double ConcBridgeLoad::StartLocation() const
{
   return startLoc;
}

double ConcBridgeLoad::Magnitude() const
{
   return magnitude;
}

//
//
//
UnifBridgeLoad::UnifBridgeLoad(Span *aSpan,double start,double end,double mag,bool isfract) :
BridgeLoad(aSpan)
{
   isFract = isfract;
   startLoc = start;
   endLoc = end;
   magnitude = mag;
}

bool UnifBridgeLoad::IsFract() const
{
   return isFract;
}

double UnifBridgeLoad::StartLocation() const
{
   return startLoc;
}

double UnifBridgeLoad::EndLocation() const
{
   return endLoc;
}

double UnifBridgeLoad::Magnitude() const
{
   return magnitude;
}

