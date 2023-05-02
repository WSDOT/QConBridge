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

#if !defined (ANALRES_H_)
#define ANALRES_H_

#include <classlib\arrays.h>

#include <fem2d\femall.h> // for MbrFace

class AnalysisResults
{
protected:
   struct PointOfInterestResult
      {
      int    poiId;   // point of interest
      double gOffset; // Global offset (from start of bridge)
      double lfValue; // left face value
      double rfValue; // right face value

      bool operator <  (const PointOfInterestResult& r) const {return poiId <  r.poiId;}
      bool operator == (const PointOfInterestResult& r) const {return poiId == r.poiId;}
      };

   struct SupportResult
   {
   	int supportId;
      double fx,fy,mz;

      bool operator <  (const SupportResult& r) const { return supportId <  r.supportId; }
      bool operator == (const SupportResult& r) const { return supportId == r.supportId; }
   };

   typedef TISArrayAsVector<PointOfInterestResult> PointOfInterestResultContainer;
   typedef TISArrayAsVectorIterator<PointOfInterestResult> PointOfInterestResultIterator;

   typedef TISArrayAsVector<SupportResult> SupportResultContainer;
   typedef TISArrayAsVectorIterator<SupportResult> SupportResultIterator;

   int id; // unique identifier
   string description; // description of loading that caused these results
   PointOfInterestResultContainer *results; // results at each poi for this loading
   SupportResultContainer* supportResults; // results at each support for this loading

public:
   AnalysisResults(int id,const char *description = 0);
   ~AnalysisResults();

   int GetID() const;
   char *GetDescription(char *desc,int maxChar = -1);
   string GetDescription();

   void Clear();

   void AddResult(int poiId,double gOffset,double lfValue,double rfValue);
   void GetResult(int poiId,double &gOffset,double &lfValue,double &rfValue);
   void GetResultByIdx(int idx,double &gOffset,double &lfValue,double &rfValue);
   int  GetNumResults();

   void AddSupportResult(int supportId,double fx,double fy,double mz);
   void GetSupportResult(int supportId,double& fx,double& fy,double& mz);
   void GetSupportResultByIdx(int idx,double& fx,double& fy,double& mz);
   int  GetNumSupportResults();

   bool operator == (const AnalysisResults &ar) const {return ar.GetID() == id;}

#if defined GEN_DEBUG_CODE
   friend ostream &operator << (ostream &os,AnalysisResults &results);
#endif
};

typedef TIArrayAsVector<AnalysisResults> AnalysisResultsContainer;
typedef TIArrayAsVectorIterator<AnalysisResults> AnalysisResultsIterator;

#if defined GEN_DEBUG_CODE
   ostream &operator << (ostream &os,AnalysisResults &results);
#endif

#endif
