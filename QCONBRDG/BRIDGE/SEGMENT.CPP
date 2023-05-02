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

#include <bridge\segment.h>
#include <stdlib.h>

DIAG_DECLARE_GROUP(Bridge);

Segment::Segment(double loc,TShape *s)
{
   location = loc;
   section = s;
   MbrId[0] = MbrId[1] = MbrId[2] = -1;
   startJntId = -1;
   endJntId   = -1;
}

Segment::Segment(Segment &seg)
{
   SetLocation(seg.GetLocation());

   section  = seg.GetSection()->Clone();

   MbrId[0] = MbrId[1] = MbrId[2] = -1;
   startJntId = -1;
   endJntId   = -1;
}

Segment::~Segment()
{
   delete section;
}

TShape* Segment::GetSection() const
{
   return section;
}

void Segment::StartJntId(int id)
{
   startJntId = id;
}

int  Segment::StartJntId() const
{
   return startJntId;
}

void Segment::EndJntId(int id)
{
    endJntId = id;
}

int  Segment::EndJntId() const
{
   return endJntId;
}

void Segment::MemberId(int mbr,int id)
{
   MbrId[mbr] = id;
}

int Segment::MemberId(int mbr) const
{
   return MbrId[mbr];
}

void Segment::SetLength(double l)
{
   length = l;
}

double Segment::GetLength() const
{
   return length;
}

void Segment::SetLocation(double loc)
{
   location = loc;
}

double Segment::GetLocation() const
{
   return location;
}

bool Segment::operator == (const Segment &s) const
{
   return (s.GetLocation() == location);
}

bool Segment::operator <  (const Segment &s) const
{
   return (location < s.GetLocation());
}


