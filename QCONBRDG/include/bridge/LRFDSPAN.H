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

#if !defined (LRFDSPAN_H)
#define LRFDSPAN_H

#include <bridge\span.h>
#include <classlib\arrays.h>

enum LimitState {lsStrength,lsService,lsExtremeEvent,lsFatigue};

struct LLDistFactors
{
   LLDistFactors() {location = 0;
                    s_gPosMoment = 1;
                    s_gNegMoment = 1;
                    s_gShear = 1;
                    f_gPosMoment = 1;
                    f_gNegMoment = 1;
                    f_gShear = 1;}
   LLDistFactors(double l,double gPM1,double gNM1,double gV1,double gPM2,double gNM2,double gV2)
      {
      location = l;
      s_gPosMoment = gPM1;
      s_gNegMoment = gNM1;
      s_gShear = gV1;
      f_gPosMoment = gPM2;
      f_gNegMoment = gNM2;
      f_gShear = gV2;
      }
   LLDistFactors(LLDistFactors &lldf)
      {
      location = lldf.location;
      s_gPosMoment = lldf.s_gPosMoment;
      s_gNegMoment = lldf.s_gNegMoment;
      s_gShear = lldf.s_gShear;
      f_gPosMoment = lldf.f_gPosMoment;
      f_gNegMoment = lldf.f_gNegMoment;
      f_gShear = lldf.f_gShear;
      }
   double location;
   double s_gPosMoment;
   double s_gNegMoment;
   double s_gShear;
   double f_gPosMoment;
   double f_gNegMoment;
   double f_gShear;

   bool operator == (const LLDistFactors &lldf) const {return location == lldf.location;}
   bool operator <  (const LLDistFactors &lldf) const {return (location < lldf.location);}
};

typedef TISArrayAsVector<LLDistFactors> LLDistFactorsContainer;
typedef TISArrayAsVectorIterator<LLDistFactors> LLDistFactorsIterator;

class _BRIDGECLASS LRFDSpan : public Span
{
public:
   LRFDSpan(double length,TShape *s);
   LRFDSpan(LRFDSpan &span);
   ~LRFDSpan();

   void   SetDuctilityFactor(double n,LimitState ls);
   double GetDuctilityFactor(LimitState ls) const;
   void   SetRedundancyFactor(double n,LimitState ls);
   double GetRedundancyFactor(LimitState ls) const;
   void   SetImportanceFactor(double n,LimitState ls);
   double GetImportanceFactor(LimitState ls) const;

   void AddLLDistFactors(LLDistFactors *lldf);
   LLDistFactors *GetLLDistFactors(int idx) const;
   void ReplaceLLDistFactors(int idx,LLDistFactors *newlldf);
   void RemoveLLDistFactors(int idx);
   LLDistFactors *FindLLDistFactors(double loc) const;
   int NumLLDistFactors() const;

   void Mirror();

protected:
   double nd_strength;
   double nr_strength;
   double ni_strength;
   double nd_service;
   double nr_service;
   double ni_service;

   LLDistFactorsContainer *llDistFactors;
};

#endif