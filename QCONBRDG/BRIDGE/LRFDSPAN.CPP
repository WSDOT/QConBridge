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

#include <bridge\lrfdspan.h>

LRFDSpan::LRFDSpan(double length,TShape *s) :
Span(length,s)
{
   SetDuctilityFactor(1.0,lsStrength);
   SetRedundancyFactor(1.0,lsStrength);
   SetImportanceFactor(1.0,lsStrength);

   SetDuctilityFactor(1.0,lsService);
   SetRedundancyFactor(1.0,lsService);
   SetImportanceFactor(1.0,lsService);

   llDistFactors = new LLDistFactorsContainer(10,0,10);
}

LRFDSpan::LRFDSpan(LRFDSpan &span) :
Span(span)
{
   SetDuctilityFactor(span.GetDuctilityFactor(lsStrength),lsStrength);
   SetRedundancyFactor(span.GetRedundancyFactor(lsStrength),lsStrength);
   SetImportanceFactor(span.GetImportanceFactor(lsStrength),lsStrength);

   SetDuctilityFactor(span.GetDuctilityFactor(lsService),lsService);
   SetRedundancyFactor(span.GetRedundancyFactor(lsService),lsService);
   SetImportanceFactor(span.GetImportanceFactor(lsService),lsService);

   llDistFactors = new LLDistFactorsContainer(10,0,10);

   // Copy live load distribution factors
   int i, numLLDF;
   numLLDF = span.NumLLDistFactors();

   for (i = 0; i < numLLDF; i++)
      {
      LLDistFactors *lldf = span.GetLLDistFactors(i);
      LLDistFactors *newLLDF = new LLDistFactors(*lldf);
      AddLLDistFactors(newLLDF);
      }
}

LRFDSpan::~LRFDSpan()
{
   delete llDistFactors;
}

void LRFDSpan::SetDuctilityFactor(double n,LimitState ls)
{
   switch(ls)
      {
      case lsStrength:
           nd_strength = n;
           break;

      case lsService:
      case lsExtremeEvent:
      case lsFatigue:
           nd_service = n;
           break;
      }
}

double LRFDSpan::GetDuctilityFactor(LimitState ls) const
{
   double n;

   switch(ls)
      {
      case lsStrength:
           n = nd_strength;
           break;

      case lsService:
      case lsExtremeEvent:
      case lsFatigue:
           n = nd_service;
           break;
      }

   return n;
}

void LRFDSpan::SetRedundancyFactor(double n,LimitState ls)
{
   switch(ls)
      {
      case lsStrength:
           nr_strength = n;
           break;

      case lsService:
      case lsExtremeEvent:
      case lsFatigue:
           nr_service = n;
           break;
      }
}

double LRFDSpan::GetRedundancyFactor(LimitState ls) const
{
   double n;

   switch(ls)
      {
      case lsStrength:
           n = nr_strength;
           break;

      case lsService:
      case lsExtremeEvent:
      case lsFatigue:
           n = nr_service;
           break;
      }

   return n;
}

void LRFDSpan::SetImportanceFactor(double n,LimitState ls)
{
   switch(ls)
      {
      case lsStrength:
           ni_strength = n;
           break;

      case lsService:
      case lsExtremeEvent:
      case lsFatigue:
           ni_service = n;
           break;
      }
}

double LRFDSpan::GetImportanceFactor(LimitState ls) const
{
   double n;

   switch(ls)
      {
      case lsStrength:
           n = ni_strength;
           break;

      case lsService:
      case lsExtremeEvent:
      case lsFatigue:
           n = ni_service;
           break;
      }

   return n;
}


void LRFDSpan::AddLLDistFactors(LLDistFactors *lldf)
{
   llDistFactors->Add(lldf);
}


LLDistFactors *LRFDSpan::GetLLDistFactors(int idx) const
{
   return (*llDistFactors)[idx];
}


void LRFDSpan::ReplaceLLDistFactors(int idx,LLDistFactors *newlldf)
{
   RemoveLLDistFactors(idx);
   AddLLDistFactors(newlldf);
}


void LRFDSpan::RemoveLLDistFactors(int idx)
{
   llDistFactors->Detach(idx,TShouldDelete::Delete);
}


LLDistFactors *LRFDSpan::FindLLDistFactors(double loc) const
{
   LLDistFactorsIterator i(*llDistFactors);
   LLDistFactors *prev,*next;

   prev = i++;
   next = NULL;

   while(i)
   {
      next = i++;
      if (prev->location <= loc && loc < next->location)
      {
         return prev;
      }

      prev = next;
   }

   if ( next == NULL )
   {
      // this is only one distribution factor (never got inside the loop)
      next = prev;
   }

   // if we got this far, loc must be after the start location
   // of the last distribution factor... return the last factor
   return next;
}


int LRFDSpan::NumLLDistFactors() const
{
   return llDistFactors->GetItemsInContainer();
}

void LRFDSpan::Mirror()
{
   Span::Mirror();

   // Mirror the live load distribution factors
   double spanLength = Length();
   LLDistFactorsIterator i(*llDistFactors);
   LLDistFactors *lldf;

   double loc = 0.;
   while(i)
      {
      lldf = i++;
      lldf->location = loc;
      loc = spanLength - lldf->location;
      }

}

