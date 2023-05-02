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

#if !defined (LRFDSEGM_H)
#define LRFDSEGM_H

#if !defined SEGMENT_H_
#include "segment.h"
#endif

class _BRIDGECLASS LRFDSegment : public Segment
{
public:
   LRFDSegment(double loc,Section &s,Material &m,double s_gM,double s_gV,double f_gM,double f_gV);
   LRFDSegment(LRFDSegment &seg);
   virtual ~LRFDSegment() {}

   double GetStrength_gMoment() const {return s_gMoment;}
   void   SetStrength_gMoment(double g) {s_gMoment = g;}
   double GetStrength_gShear()  const {return s_gShear;}
   void   SetStrength_gShear(double g) {s_gShear = g;}
   double GetFatigue_gMoment()  const {return f_gMoment;}
   void   SetFatigue_gMoment(double g) {f_gMoment = g;}
   double GetFatigue_gShear()   const {return f_gShear;}
   void   SetFatigue_gShear(double g) {f_gShear = g;}

protected:
   double s_gMoment;
   double s_gShear;
   double f_gMoment;
   double f_gShear;
};

#endif