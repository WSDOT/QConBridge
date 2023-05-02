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

#if !defined BLOAD_H_
#define BLOAD_H_

#include <bridge\brdgdll.h>
#include <classlib\arrays.h>

class _BRIDGECLASS BridgeLoading; // placeholder
class _BRIDGECLASS Span; // placeholder

#define blConc 1
#define blUnif 2

class _BRIDGECLASS BridgeLoad
{
public:
   BridgeLoad(Span *aSpan);
   virtual ~BridgeLoad();
   void SetSpan(Span *aSpan);
   Span *GetSpan() const;
   int GetID() const;
   BridgeLoading *GetOwner() const;
   virtual int Type() const = 0;
   bool operator == (const BridgeLoad &load) const;

#if defined GEN_DEBUG_CODE
   bool TestLink(Span *requestingSpan);
#endif

protected:
   void Link();
   void Unlink();

protected:
   int ID;
   BridgeLoading *owner;
   Span *span;

friend BridgeLoading;
};

typedef TIArrayAsVector<BridgeLoad>         BridgeLoadContainer;
typedef TIArrayAsVectorIterator<BridgeLoad> BridgeLoadIterator;

class _BRIDGECLASS ConcBridgeLoad : public BridgeLoad
{
public:
   ConcBridgeLoad(Span *aSpan,double loc,double mag,bool isfract);
   bool IsFract() const;
   double StartLocation() const;
   double Magnitude() const;
   int Type() const {return blConc; }

protected:
   bool isFract;
   double startLoc;
   double magnitude;
};

class _BRIDGECLASS UnifBridgeLoad : public BridgeLoad
{
public:
   UnifBridgeLoad(Span *aSpan,double start,double end,double mag,bool isfract);
   bool IsFract() const;
   double StartLocation() const;
   double EndLocation() const;
   double Magnitude() const;
   int Type() const {return blUnif; }

protected:
   bool isFract;
   double startLoc;
   double endLoc;
   double magnitude;
};

#endif // BLOAD_H_