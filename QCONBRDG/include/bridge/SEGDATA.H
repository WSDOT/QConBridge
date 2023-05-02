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

#if !defined segdata_h_
#define segdata_h_

#include "material.h"
#include "section.h"
#include "btypes.h"
#include <classlib\arrays.h>

// Segment data for mapping to fem

struct SegmentData
{
   double startX,startY;
   double endX,endY;
   Section s;
   Material m;
   Continuity startContinuity;
   Continuity endContinuity;

   int operator == (const SegmentData &s) const { return true; }
};

typedef TIArrayAsVector<SegmentData> SegmentDataContainer;
typedef TIArrayAsVectorIterator<SegmentData> SegmentDataIterator;

#endif // segdata_h_