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

#if !defined BRIDGE_H_
#define BRIDGE_H_

#include <bridge\brdgdll.h>
#include <bridge\span.h>
#include <bridge\support.h>
#include <bridge\bloading.h>
#include <bridge\diagram.h>
#include <bridge\analres.h>
#include <bridge\debug.h>
#include <owltools\progind.h>

#define PROGRESS_MSG(msg) {if(progressIndicator) progressIndicator->SetProgressMsg((msg));}
#define SHOULD_CONTINUE   {if(progressIndicator) \
                              {                  \
                              progressIndicator->IncrementGauge(); \
                              if (!progressIndicator->ShouldContinue())\
                                 { \
                                 PROGRESS_MSG("Analysis Canceled"); \
                                 return false; \
                                 }\
                              }}

class _FEMCLASS    FemModel;

// Bridge
//
// Represents a bridge structure
class _BRIDGECLASS Bridge
{
public:
   Bridge();
   virtual ~Bridge();

   virtual void Clear(bool init = true);

   void AddSpanSupport(Span *aSpan,Support *aSupport,bool atEnd);
   Span *GetSpan(int id) const;
   Span *GetSpan(double x) const;
   Span *GetFirstSpan() const;
   Span *GetLastSpan() const;
   void RemoveSpan(int id,bool removeRightSupport);
   void ReplaceSpan(int id,Span *aSpan);
   int  NumSpans() const;

   void CantileverFirstSpan(bool cantilever);
   void CantileverLastSpan(bool cantilever);

   Support *GetSupport(int id) const;
   Support *GetFirstSupport() const;
   Support *GetLastSupport() const;
   void RemoveSupport(int id,bool leftSpan);
   void ReplaceSupport(int id,Support *aSupport);
   int  NumSupports() const;

   void Insert(int spanId,bool after,Span *newSpan,Support *newSupport,bool isRightSupport);

   void AddLoading(BridgeLoading *loading);
   BridgeLoading *GetLoading(int id);
   void RemoveLoading(BridgeLoading *loading);
   int NumLoadings() const { return loadings->GetItemsInContainer(); }

   Diagram *GetDiagram(int id);
   AnalysisResults *GetAnalysisResults(int id);

   void SetMinAnalysisPoints(int n) {minAnalysisPoints = n;}
   int  GetMinAnalysisPoints() const {return minAnalysisPoints;}

   virtual bool Analyze(ProgressIndicator *pi = NULL);

   double Length() const;
   double MaxSpanLength() const;
   double MinSpanLength() const;
   double MaxSupportHeight() const;
   double MinSupportHeight() const;

   int SpanPntToPOI(int spanId,int spanPnt);
   double GetSuperstructureOffset(int spanId,double offset);
   void GetSpanOffset(double gOffset,int &spanId,double &lOffset);
protected:
   ProgressIndicator *progressIndicator;

   int minAnalysisPoints;
   unsigned numSpans;
   unsigned numSupports;

   double minSpanLength, maxSpanLength;
   double minSupportHeight, maxSupportHeight;
   double totalBridgeLength;

   FemModel *femModel;
   Span *firstSpan;
   Span *lastSpan;
   Support *firstSupport;
   Support *lastSupport;

   BridgeLoadingContainer *loadings;

   DiagramContainer *diagrams;
   AnalysisResultsContainer *results;

   // Analysis Steps
   virtual bool GenerateModel();
   virtual bool ApplyLoadings();
   virtual bool GeneratePOI();
   virtual FemModel *GetModel() const { return femModel; }

   //
   // Numbering schemes
   //
   int nextAvailJnt;
   int nextAvailMbr;
   int nextAvailLoading;
   int nextAvailDiagram;
   int nextAvailAnalysisResults;
   int nextAvailPointOfInterest;

   void ResetNextAvailCounters();
   int NextAvailJnt();
   int NextAvailMbr();
   int NextAvailLoading();
   int NextAvailCombo();
   int NextAvailDiagram();
   int NextAvailAnalysisResults();
   int NextAvailPointOfInterest();

   virtual int  SpanToFem(int jctJnt,Span *span);
   virtual void SupportToFem(int jctJnt,Support *support);
   virtual void LoadingToFem(BridgeLoading *loading);

   void AddDiagram(Diagram *dia);
   void RemoveDiagram(int id);

   void AddAnalysisResults(AnalysisResults *analysisResults);
   void RemoveAnalysisResults(int id);

protected:
   struct PointOfInterest
      {
      PointOfInterest(int id,int spanId,double offset) :
                     id(id),spanId(spanId),offset(offset) {}
      int id;        // unique identifier
      int spanId;    // span id
      double offset; // offset from start of span

      bool operator == (const PointOfInterest &poi) const;
      bool operator <  (const PointOfInterest &poi) const;
      };
   typedef TIArrayAsVector<PointOfInterest> PointOfInterestContainer;
   typedef TIArrayAsVectorIterator<PointOfInterest> PointOfInterestIterator;

   class SortedPointOfInterestIterator
      {
      public:
         SortedPointOfInterestIterator(int spanId,PointOfInterestContainer &pntsOfInterest);
         ~SortedPointOfInterestIterator();
         PointOfInterest* Current() const;
         operator int() const;
         PointOfInterest* operator ++ ( int );
         PointOfInterest* operator ++ ();
         void Restart( unsigned start, unsigned stop );
         PointOfInterest* Next() const;
         PointOfInterest* Prev() const;

      protected:
         TISArrayAsVector<PointOfInterest> *sortedContainer;
         int Cur;
         int Upper;
         int Lower;
      };

   PointOfInterestContainer *pntsOfInterest;
   bool CheckPOIConflict(int spanId,double offset);
   PointOfInterest *GetPointOfInterest(int poiId);

protected:
   // Model building "helper" function
   void UpdateIDs();
   void ReIndexSpans();
   void ReIndexSupports();

   void UpdateMinMaxSpanLength();
   void UpdateMinMaxSupportHeight();

   void InsertAfter(Span *refSpan,Span *newSpan,Support *newSupport,bool isRightSupport);
   void InsertBefore(Span *refSpan,Span *newSpan,Support *newSupport,bool isRightSupport);

   void InitExtremeValues();

private:
   // POI Generation "helper" function
   bool GenerateMinPOI();
   bool GenerateSegmentPOI();
   bool GenerateHingePOI();
   bool GenerateLoadPOI();

   friend Span;
   friend Support;

   Bridge(Bridge &b);
   Bridge *operator = (Bridge *b) const;
   Bridge &operator = (Bridge &b) const;

#if defined GEN_DEBUG_CODE
   void TestLinkedLists() const;
   void VerifyMinMaxSpanLengths() const;
   void VerifyMinMaxSupportHeights() const;
   void VerifyTotalBridgeLength() const;
   void ReportPOI();
#endif // GEN_DEBUG_CODE
};

#endif // BRIDGE_H_