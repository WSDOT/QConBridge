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

#include <bridge\analres.h>
#include <string.h>

#include <bridge\debug.h>

#undef  EPS
#define EPS  1.0e-3
#undef   zero_val
#define  zero_val( x )     (((-EPS)<=(x)) && ((x)<=(EPS)))


AnalysisResults::AnalysisResults(int id,const char *description) :
id(id),
description(description)
{
   results = new PointOfInterestResultContainer(10,0,10);
   supportResults = new SupportResultContainer(10,0,10);
}

AnalysisResults::~AnalysisResults()
{
   delete results;
   delete supportResults;

   results = 0;
   supportResults = 0;
}

int AnalysisResults::GetID() const
{
   return id;
}

char *AnalysisResults::GetDescription(char *desc,int maxChar)
{
   memset(desc,0,maxChar);
   strncat(desc,description.c_str(),maxChar);
   return desc;
}

string AnalysisResults::GetDescription()
{
   return description;
}

void AnalysisResults::Clear()
{
   results->Flush(TShouldDelete::Delete);
   supportResults->Flush(TShouldDelete::Delete);
}

void AnalysisResults::AddResult(int poiId,double gOffset,double lfValue,double rfValue)
{
   PointOfInterestResult findPoint;
   PointOfInterestResult *rp;
   int idx;

   // "Smooth" out the zero-values
   lfValue = zero_val(lfValue) ? 0.00 : lfValue;
   rfValue = zero_val(rfValue) ? 0.00 : rfValue;

   // Search of a previous occurance of this point
   findPoint.poiId = poiId;
   idx = results->Find(&findPoint);

   if (idx == INT_MAX)
      {
      // point was not found, create new
      rp = new PointOfInterestResult;
      rp->poiId   = poiId;
      rp->gOffset = gOffset;
      rp->lfValue = lfValue;
      rp->rfValue = rfValue;

      results->Add(rp);
      }
   else
      {
      // point was found, update values
      rp = (*results)[idx];
      rp->poiId   = poiId;
      rp->gOffset = gOffset;
      rp->lfValue = lfValue;
      rp->rfValue = rfValue;
      }
}

void AnalysisResults::GetResult(int poiId,double &gOffset,double &lfValue,double &rfValue)
{
   PointOfInterestResult findPoint;
   PointOfInterestResult *rp;
   int idx;

   // Search of a previous occurance of this point
   findPoint.poiId = poiId;
   idx = results->Find(&findPoint);

   CHECKX(idx != INT_MAX,"AnalysisResults::GetResult() - Results not found");

   rp = (*results)[idx];

   gOffset = rp->gOffset;
   lfValue = rp->lfValue;
   rfValue = rp->rfValue;
}

void AnalysisResults::GetResultByIdx(int idx,double &gOffset,double &lfValue,double &rfValue)
{
   PointOfInterestResult *rp;

   PRECONDITIONX(idx < results->GetItemsInContainer(),"AnalysisResults::GetResultByIdx() - Invalid Index");

   rp = (*results)[idx];

   gOffset  = rp->gOffset;
   lfValue = rp->lfValue;
   rfValue = rp->rfValue;
}

int AnalysisResults::GetNumResults()
{
   return results->GetItemsInContainer();
}

void AnalysisResults::AddSupportResult(int supportId,double fx,double fy,double mz)
{
   SupportResult findPoint;
   SupportResult *rp;
   int idx;

   // "Smooth" out the zero-values
   fx = zero_val(fx) ? 0.00 : fx;
   fy = zero_val(fy) ? 0.00 : fy;
   mz = zero_val(mz) ? 0.00 : mz;

   // Search of a previous occurance of this point
   findPoint.supportId = supportId;
   idx = supportResults->Find(&findPoint);

   if (idx == INT_MAX)
      {
      // point was not found, create new
      rp = new SupportResult;
      rp->supportId   = supportId;
      rp->fx = fx;
      rp->fy = fy;
      rp->mz = mz;

      supportResults->Add(rp);
      }
   else
      {
      // point was found, update values
      rp = (*supportResults)[idx];
      rp->supportId   = supportId;
      rp->fx = fx;
      rp->fy = fy;
      rp->mz = mz;
      }
}

void AnalysisResults::GetSupportResult(int supportId,double& fx,double& fy,double& mz)
{
   SupportResult findPoint;
	SupportResult *rp;
   int idx;

   // Search of a previous occurance of this point
   findPoint.supportId = supportId;
   idx = supportResults->Find(&findPoint);

   CHECKX(idx != INT_MAX,"AnalysisResults::GetSupportResult() - Results not found");

   rp = (*supportResults)[idx];

   fx = rp->fx;
   fy = rp->fy;
   mz = rp->mz;
}

void AnalysisResults::GetSupportResultByIdx(int idx,double& fx,double& fy,double& mz)
{
   SupportResult *rp;

   PRECONDITIONX(idx < supportResults->GetItemsInContainer(),"AnalysisResults::GetSupportResultByIdx() - Invalid Index");

   rp = (*supportResults)[idx];

   fx = rp->fx;
   fy = rp->fy;
   mz = rp->mz;
}

int AnalysisResults::GetNumSupportResults()
{
	return supportResults->GetItemsInContainer();
}

#if defined GEN_DEBUG_CODE
#include <iomanip.h>

ostream &operator << (ostream &os,AnalysisResults &results)
   {
   char buffer[256];

   os << "Analysis Results for "
      << results.GetDescription(buffer,sizeof(buffer))
      << " ID (" << results.GetID() << ")" << endl;

   AnalysisResults::PointOfInterestResultIterator i(*results.results);

   while (i)
      {
      AnalysisResults::PointOfInterestResult *rp = i++;

      os << "POI " << setw(4) << setiosflags(ios::left) << rp->poiId << " "
         << "gOffset " << setw(8) << setiosflags(ios::right | ios::showpoint)
         << rp->gOffset << " "
         << "Left Face "
         << setiosflags(ios::right | ios::scientific | ios::showpoint)
         << setw(14) << setprecision(4)
         << rp->lfValue << " "
         << "Right Face "
         << setiosflags(ios::right | ios::scientific | ios::showpoint)
         << setw(14) << setprecision(4)
         << rp->rfValue
         << endl;
      }

   return os;
   }
#endif

