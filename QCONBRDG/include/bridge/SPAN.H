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

#if !defined  SPAN_H_
#define SPAN_H_

#include <bridge\brdgdll.h>
#include <bridge\segment.h>
#include <bridge\hinge.h>
#include <bridge\bload.h>
#include <bridge\bloading.h>
#include <bridge\btypes.h>
#include <fem2d\femmodel.h>
#include <classlib/arrays.h>

class _BRIDGECLASS Bridge;
class _BRIDGECLASS LRFDBridge; // find a way to end the friendship
class _BRIDGECLASS Support;

// Span
//
// Represents a span
class _BRIDGECLASS Span
{
public:
   Span(double length,TShape *s);
   Span(Span &s);
   virtual ~Span();

   Support *LeftSupport() const;
   Support *RightSupport() const;
   Span *NextSpan() const;
   Span *PrevSpan() const;

   double Length() const;

   int  GetID() const;

   void AddSegment(Segment *segment);
   Segment *GetSegment(int idx) const;
   void ReplaceSegment(int idx,Segment *newSegment);
   void RemoveSegment(int idx);
   Segment *FindSegment(double loc) const;
   int NumSegments() const;

   void StartContinuity(Continuity continuity);
   Continuity StartContinuity() const;
   void EndContinuity(Continuity continuity);
   Continuity EndContinuity() const;

   void AddHinge(Hinge *aHinge);
   void RemoveHinge(int n);
   void RemoveHinge(Hinge *aHinge);
   Hinge *GetHinge(int n) const;
   int NumHinges() const;

   void GetStartPoint(double &x,double &y) const;
   void GetEndPoint(double &x,double &y) const;

   bool IsCantilevered() const;

   virtual void Mirror();

protected:
   int ID;
   double length;
   Support *leftSupport;
   Support *rightSupport;
   Span *nextSpan;
   Span *prevSpan;
   SegmentContainer *segments;

   Hinge *hinge[2];
   int numHinges;

   Continuity startContinuity;
   Continuity endContinuity;

   BridgeLoadContainer *loads;

   void Init();

   void SetID(int id);

   void AddLoad(BridgeLoad *load);
   void RemoveLoad(BridgeLoad *load);
   BridgeLoad *GetLoad(int id);

   void LeftSupport(Support *ls);
   void RightSupport(Support *rs);
   void NextSpan(Span *next);
   void PrevSpan(Span *prev);

   void ComputeSegmentLengths();
   virtual int GenerateFemComponents(int refJntId,Bridge *bridge);
   virtual void ApplyLoad(int ld,BridgeLoad *load,Bridge *bridge);
   void ApplyConcLoad(int ld,ConcBridgeLoad *cload,Bridge *bridge);
   void ApplyUnifLoad(int ld,UnifBridgeLoad *uload,Bridge *bridge);
   void FindMbr(FemModel *femModel,double loc,int &mbrId,double &dist);

   // FEM member mapping

   struct TInteger {
     int i;
     TInteger() {}
     TInteger(int _i) {i = _i;}
     operator int() const {return i;}
   };
   typedef TArrayAsVector<TInteger> TIntArray;
   typedef TArrayAsVectorIterator<TInteger> TIntIterator;
   TIntArray *members;

   bool OkToModel();

private:
//   Span(Span &s);
   Span *operator = (Span *s) const;
   Span &operator = (Span &s) const;

   friend Bridge;
   friend LRFDBridge; // needed for access to members....
   friend BridgeLoad;
};


#endif // SPAN_H_

