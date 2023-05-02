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

#if !defined  SUPPORT_H_
#define SUPPORT_H_

#include <bridge\brdgdll.h>
#include <bridge\segment.h>
#include <bridge\hinge.h>
#include <bridge\bload.h>
#include <bridge\bloading.h>
#include <bridge\btypes.h>
#include <fem2d\femdll.h>
#include <classlib/arrays.h>

class _BRIDGECLASS Bridge;
class _BRIDGECLASS Span;
class _FEMCLASS    Joint;

// Support
//
// Represents a supporting member of a bridge structure
class _BRIDGECLASS Span; // partial definition...
class _BRIDGECLASS Support
{
public:
   Support(double h,double bo,TShape *s);
   Support();
   Support(Support &s);
   ~Support();

   double BaseOffset() const;
   double Height() const;

   int  GetID() const;

   void AddSegment(Segment *segment);
   Segment *GetSegment(int idx) const;
   void ReplaceSegment(int idx,Segment *newSegment);
   void RemoveSegment(int idx);
   Segment *FindSegment(double loc) const;
   int NumSegments() const;

   void    SupportFixity(Fixity fixity);
   Fixity  SupportFixity() const;
   void    TopContinuity(Continuity continuity);
   Continuity TopContinuity() const;
   bool    IsPier() const;

   Span *LeftSpan() const;
   Span *RightSpan() const;
   Support *NextSupport() const;
   Support *PrevSupport() const;

   void GetTopPoint(double &x,double &y) const;
   void GetBottomPoint(double &x,double &y) const;

   int  StartJntId() const;
   int  EndJntId() const;

protected:
   int ID;
   SegmentContainer *segments;
   double height;
   double baseOffset;

   void Init();
   void ComputeSegmentLengths();
   void SetID(int id);

   bool OkToModel();

private:
   bool isPier;
   Fixity supportFixity;
   Continuity topContinuity;
   Span *leftSpan;
   Span *rightSpan;
   Support *nextSupport;
   Support *prevSupport;
   int reactionJntId; // id of support joint for reactions                                                      
   int connectionJntId; // id of joint and super/substructure intersection

   Support(int id);

   void LeftSpan(Span *s);
   void RightSpan(Span *s);
   void NextSupport(Support *s);
   void PrevSupport(Support *s);


   void GenerateFemComponents(int refJntId,Bridge *bridge);
   void SupportJoint(Joint *jnt,Fixity fixity);

   struct TInteger {
     int i;
     TInteger() {}
     TInteger(int _i) {i = _i;}
     operator int() const {return i;}
   };
   typedef TArrayAsVector<TInteger> TIntArray;
   typedef TArrayAsVectorIterator<TInteger> TIntIterator;
   TIntArray *members;

   friend Bridge;

private:
//   Support(Support &s);
   Support *operator = (Support *s) const;
   Support &operator = (Support &s) const;
};

#endif // SUPPORT_H_