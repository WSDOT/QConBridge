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

#if !defined SEGMENT_H_
#define SEGMENT_H_

#include <bridge\brdgdll.h>
#include <shape\shape.h>
#include <material\material.h>
#include <classlib/arrays.h>

class _BRIDGECLASS Span;
class _BRIDGECLASS Support;

// Segment
//
// Represents a descrete portion of a component
class _BRIDGECLASS Segment
{
public:
   Segment(double loc,TShape *s);
   Segment(Segment &seg);
   ~Segment();

   TShape* GetSection() const;

   void StartJntId(int id);
   int  StartJntId() const;
   void EndJntId(int id);
   int  EndJntId() const;
   void MemberId(int mbr,int id);
   int  MemberId(int mbr) const;

   double GetLocation() const;
   double GetLength() const;

   bool operator == (const Segment &s) const;
   bool operator <  (const Segment &s) const;

private:
   double location;
   double length;
   TShape* section;
   int MbrId[3];
   int startJntId;
   int endJntId;

   void SetLocation(double loc);
   void SetLength(double l);

   friend Span;
   friend Support;

private:
//   Segment(Segment &s);
   Segment *operator = (Segment *s) const;
   Segment &operator = (Segment &s) const;
};

typedef TISArrayAsVector<Segment>         SegmentContainer;
typedef TISArrayAsVectorIterator<Segment> SegmentIterator;

#endif // SEGMENT_H_