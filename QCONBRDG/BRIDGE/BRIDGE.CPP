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

#include <bridge\bridge.h>
#include <values.h>

#include <fem2d\femmodel.h>

#include <code\version.h>

#define InRange(a,x,b) (((a)<(x) || zero_val(x-a)) && ((x)<(b) || zero_val(x-b)))

DIAG_DEFINE_GROUP(Bridge,               DISABLED,LEVEL1);
DIAG_DEFINE_GROUP(Bridge_PntsOfInterest,DISABLED,LEVEL2);
DIAG_DEFINE_GROUP(Bridge_InfluenceLines,DISABLED,LEVEL1);

Bridge::Bridge()
{
   // To change the default code version, change EDITION in COMPILER | DEFINE's
   SetCodeEdition(EDITION);

   firstSpan = NULL;
   lastSpan  = NULL;
   numSpans  = 0;

   InitExtremeValues();
   totalBridgeLength = 0.0;

   firstSupport = new Support(1);
   lastSupport  = firstSupport;
   numSupports  = 1;

   loadings       = new BridgeLoadingContainer(10,0,10);
   diagrams       = new DiagramContainer(10,0,10);
   pntsOfInterest = new PointOfInterestContainer(10,0,10);
   results        = new AnalysisResultsContainer(10,0,10);

   ResetNextAvailCounters();

   minAnalysisPoints = 11;
   femModel          = NULL;
   progressIndicator = NULL;
}

Bridge::~Bridge()
{
   Clear(false); // don't initialize a new model

   delete femModel;
   delete loadings;
   delete diagrams;
   delete pntsOfInterest;
   delete results;
}

//
//
// Model Editing Methods ------------------------------------------------------
//
//

void Bridge::Clear(bool init)
{
   TRACEX(Bridge,LEVEL1,"Bridge::Clear()");
   Span *span,*nextSpan;
   Support *support,*nextSupport;

   loadings->Flush();
   diagrams->Flush();
   results->Flush();
   pntsOfInterest->Flush();

   span = GetFirstSpan();
   while(span)
      {
      nextSpan = span->NextSpan(); // get the next span before we delete this span
      delete span;
      span = nextSpan;
      }

   support = GetFirstSupport();
   while (support)
      {
      nextSupport = support->NextSupport(); // get the next support before we delete this support
      delete support;
      support = nextSupport;
      }

   firstSpan = NULL;
   lastSpan  = NULL;
   firstSupport = NULL;
   lastSupport  = NULL;

   numSpans = 0;
   numSupports = 0;
   InitExtremeValues();
   totalBridgeLength = 0.0;

   delete femModel;
   femModel = NULL;

   if (init)
      {
      firstSupport = new Support(1);
      minSupportHeight = min(minSupportHeight,firstSupport->Height());
      maxSupportHeight = max(maxSupportHeight,firstSupport->Height());

      lastSupport = firstSupport;
      numSupports = 1;

#if defined GEN_DEBUG_CODE
      VerifyMinMaxSpanLengths();
      VerifyMinMaxSupportHeights();
      VerifyTotalBridgeLength();
#endif // GEN_DEBUG_CODE
      }
}

void Bridge::AddSpanSupport(Span *aSpan,Support *aSupport,bool atEnd)
{
   if (numSpans == 0)
      {
      firstSpan = aSpan;
      lastSpan  = aSpan;
      }

   if (atEnd)
      {
      // Add span/support at end of bridge
      if (numSpans > 0)
         {
         lastSpan->NextSpan(aSpan);
         aSpan->PrevSpan(lastSpan);
         }

      aSpan->LeftSupport(lastSupport);
      lastSupport->RightSpan(aSpan);

      aSpan->RightSupport(aSupport);
      aSupport->LeftSpan(aSpan);

      lastSupport->NextSupport(aSupport);
      aSupport->PrevSupport(lastSupport);

      lastSpan = aSpan;
      lastSupport = aSupport;
      }
   else
      {
      // Add span/support at start of bridge

      if (numSpans > 0)
         {
         firstSpan->PrevSpan(aSpan);
         aSpan->NextSpan(firstSpan);
         }

      aSpan->RightSupport(firstSupport);
      firstSupport->LeftSpan(aSpan);

      aSpan->LeftSupport(aSupport);
      aSupport->RightSpan(aSpan);

      firstSupport->PrevSupport(aSupport);
      aSupport->NextSupport(firstSupport);

      firstSpan = aSpan;
      firstSupport = aSupport;
      }

   totalBridgeLength += aSpan->Length();
   maxSpanLength = max(maxSpanLength,aSpan->Length());
   minSpanLength = min(minSpanLength,aSpan->Length());
   maxSupportHeight = max(maxSupportHeight,aSupport->Height());
   minSupportHeight = min(minSupportHeight,aSupport->Height());

   numSpans++;
   numSupports++;

   ReIndexSpans();
   ReIndexSupports();

#if defined GEN_DEBUG_CODE
   TestLinkedLists();
   VerifyMinMaxSpanLengths();
   VerifyMinMaxSupportHeights();
   VerifyTotalBridgeLength();
#endif
}

Span *Bridge::GetSpan(int id) const
{
   PRECONDITIONX(id >= 0,"Bridge::GetSpan() - Invalid span id");

   Span *span;
   int i;
   span = GetFirstSpan();
   for (i = 1; i < id; i++)
      span = span->NextSpan();

   CHECKX(span,"Bridge::GetSpan() - Span not found");

   return span;
}

Span *Bridge::GetSpan(double x) const
{
   Span *span;
   double startX,startY;
   double endX,endY;

   for (span = GetFirstSpan(); span; span = span->NextSpan())
      {
      span->GetStartPoint(startX,startY);
      span->GetEndPoint(endX,endY);

      //if (startX <= x && x <= endX) // This didn't always evaluate correctly
      if (InRange(startX,x,endX))
         return span;
      }

   return NULL;
}

Span *Bridge::GetFirstSpan() const
{
//   CHECKX(firstSpan,"Bridge::GetFirstSpan() - first span is invalid");
   return firstSpan;
}

Span *Bridge::GetLastSpan() const
{
//   CHECKX(lastSpan,"Bridge::GetLastSpan() - last span is invalid");
   return lastSpan;
}

void Bridge::RemoveSpan(int id,bool removeRightSupport)
{
   // Get the span to be deleted
   Support *support;
   Span *span;
   Span *leftSpan, *rightSpan;
   Support *leftSupport, *rightSupport;

   // If the last span (or support in the case of cantileved spans) is being
   // removed, just clear the model. There is no need to mess with all of
   // pointers
   if (NumSpans() == 1 || NumSupports() == 1)
      {
      Clear();
      return;
      }

   span = GetSpan(id);

   leftSpan = span->PrevSpan();
   rightSpan = span->NextSpan();
   leftSupport = span->LeftSupport();
   rightSupport = span->RightSupport();

   if (leftSpan)
      leftSpan->NextSpan(rightSpan);
   else
      firstSpan = rightSpan;

   if (rightSpan)
      rightSpan->PrevSpan(leftSpan);
   else
      lastSpan = leftSpan;

   if (removeRightSupport)
      {
      if (leftSupport)
         {
         leftSupport->RightSpan(rightSpan);
         leftSupport->NextSupport( rightSupport ? rightSupport->NextSupport() : NULL);
         if (leftSupport->NextSupport())
            {
            leftSupport->NextSupport()->PrevSupport(leftSupport);
            }
         }

      if (rightSpan)
         {
         rightSpan->LeftSupport(leftSupport);
         if (rightSpan->RightSupport())
            rightSpan->RightSupport()->PrevSupport(leftSupport);
         if (leftSupport)
            leftSupport->NextSupport(rightSpan->RightSupport());
         }
      else
         lastSupport = leftSupport;

      support = rightSupport;

      if (support == firstSupport)
         firstSupport = support->NextSupport();
      }
   else // remove left support
      {
      if (rightSupport)
         {
         rightSupport->LeftSpan(leftSpan);
         rightSupport->PrevSupport( leftSupport ? leftSupport->PrevSupport() : NULL );
         if (rightSupport->PrevSupport())
            {
            rightSupport->PrevSupport()->NextSupport(rightSupport);
            }
         }

      if (leftSpan)
         {
         leftSpan->RightSupport(rightSupport);
         if (leftSpan->LeftSupport())
            leftSpan->LeftSupport()->NextSupport(rightSupport);
         if (rightSupport)
            rightSupport->PrevSupport(leftSpan->LeftSupport());
         }
      else
         firstSupport = rightSupport;

      support = leftSupport;

      if (support == lastSupport)
        {
        lastSupport = support->PrevSupport();
        }
      }

   ReIndexSpans();
   ReIndexSupports();

   // If we are about to remove the span that has the min or max span length,
   // we must find a new min or max span length
   if (zero_val(minSpanLength - span->Length()) ||
       zero_val(maxSpanLength - span->Length()))
      UpdateMinMaxSpanLength();

   // Likewise for the support height
   if (support)
      {
      if (zero_val(minSupportHeight - support->Height()) ||
          zero_val(maxSupportHeight - support->Height()))
         UpdateMinMaxSupportHeight();
      }

   totalBridgeLength -= span->Length();

   numSpans--;

   if (support)
      numSupports--;

   delete support;
   delete span;

#if defined GEN_DEBUG_CODE
   TestLinkedLists();
   VerifyMinMaxSpanLengths();
   VerifyMinMaxSupportHeights();
   VerifyTotalBridgeLength();
#endif
}

void Bridge::ReplaceSpan(int id,Span *newSpan)
{
   Span *oldSpan;
   Span *prevSpan,*nextSpan;
   Support *leftSupport;
   Support *rightSupport;

   oldSpan = GetSpan(id);
   prevSpan = oldSpan->PrevSpan();
   nextSpan = oldSpan->NextSpan();
   leftSupport = oldSpan->LeftSupport();
   rightSupport = oldSpan->RightSupport();

   newSpan->PrevSpan(prevSpan);
   newSpan->NextSpan(nextSpan);

   newSpan->LeftSupport(leftSupport);
   newSpan->RightSupport(rightSupport);

   if (prevSpan)
      prevSpan->NextSpan(newSpan);

   if (nextSpan)
      nextSpan->PrevSpan(newSpan);

   if (leftSupport)
      leftSupport->RightSpan(newSpan);

   if (rightSupport)
      rightSupport->LeftSpan(newSpan);

   if (firstSpan == oldSpan)
      firstSpan = newSpan;

   if (lastSpan == oldSpan)
      lastSpan = newSpan;

   newSpan->ID = oldSpan->ID;

   totalBridgeLength -= oldSpan->Length();
   totalBridgeLength += newSpan->Length();

   // Make a copy of the min/max span lengths
   double oldMinSpanLength, oldMaxSpanLength;
   oldMinSpanLength = minSpanLength;
   oldMaxSpanLength = maxSpanLength;

   // Update the min/max span lengths
   maxSpanLength = max(maxSpanLength,newSpan->Length());
   minSpanLength = min(minSpanLength,newSpan->Length());

   // if the span that was replaced had the old min/max span length, then
   // we must go through all the spans to find the new extreme values
   if (zero_val(oldSpan->Length() - oldMaxSpanLength) || zero_val(oldSpan->Length() - oldMinSpanLength))
      UpdateMinMaxSpanLength();

   // Move all of the loads from the old span to the new span
   BridgeLoadIterator i(*(oldSpan->loads));
   while(i)
      {
      BridgeLoad *load = i++;
      load->SetSpan(newSpan);

      // The above statements alters the size of the container...
      // the iterator must be restarted. To prevent us from having
      // to go back over all the previously scanned objects, we will
      // just go back one object and start again...
      i.Restart(((i-1) < 0) ? 0 : i-1,oldSpan->loads->GetItemsInContainer());
      }

   delete oldSpan;

#if defined GEN_DEBUG_CODE
   TestLinkedLists();
   VerifyMinMaxSpanLengths();
   VerifyMinMaxSupportHeights();
   VerifyTotalBridgeLength();
#endif
}

int Bridge::NumSpans() const
{
   return numSpans;
}

void Bridge::CantileverFirstSpan(bool cantilever)
{
   Support *support;

   if (cantilever)
      {
      // cantilever the span, if it is not currently cantilevered
      if (!firstSpan->IsCantilevered())
         {
         support = firstSpan->LeftSupport();
         firstSupport = firstSpan->RightSupport();

         support->NextSupport()->PrevSupport(support->PrevSupport());
         firstSpan->LeftSupport(support->PrevSupport());

         // if the support we are deleting had the extreme values, we
         // must search for new extreme values
         if (zero_val(minSupportHeight - support->Height()) ||
             zero_val(maxSupportHeight - support->Height()))
            UpdateMinMaxSupportHeight();

         delete support;
         numSupports--;
         }
      }
   else
      {
      // "un"-cantilever the span, if it is currently cantilevered
      if (firstSpan->IsCantilevered())
         {
         support = new Support();
         support->SupportFixity(fxPinned);

         firstSpan->LeftSupport(support);
         firstSpan->RightSupport()->PrevSupport(support);
         support->RightSpan(firstSpan);
         support->NextSupport(firstSpan->RightSupport());
         support->PrevSupport(NULL);
         firstSupport = support;
         numSupports++;
         }
      }

   ReIndexSupports();

#if defined GEN_DEBUG_CODE
   TestLinkedLists();
   VerifyMinMaxSpanLengths();
   VerifyMinMaxSupportHeights();
   VerifyTotalBridgeLength();
#endif
}


void Bridge::CantileverLastSpan(bool cantilever)
{
   Support *support;

   if (cantilever)
      {
      // cantilever the span
      if (!lastSpan->IsCantilevered())
         {
         support = lastSpan->RightSupport();
         lastSupport = lastSpan->LeftSupport();

         support->PrevSupport()->NextSupport(support->NextSupport());
         lastSpan->RightSupport(NULL);

         // if the support we are deleting had the extreme values, we
         // must search for new extreme values
         if (zero_val(minSupportHeight - support->Height()) ||
             zero_val(maxSupportHeight - support->Height()))
            UpdateMinMaxSupportHeight();

         delete support;
         numSupports--;
         }
      }
   else
      {
      // "un"-cantilever the span
      if (lastSpan->IsCantilevered())
         {
         support = new Support();
         support->SupportFixity(fxPinned);

         lastSpan->RightSupport(support);
         support->LeftSpan(lastSpan);
         lastSpan->LeftSupport()->NextSupport(support);
         support->PrevSupport(lastSpan->LeftSupport());
         support->NextSupport(NULL);
         lastSupport = support;
         numSupports++;
         }
      }

   ReIndexSupports();

#if defined GEN_DEBUG_CODE
   TestLinkedLists();
   VerifyMinMaxSpanLengths();
   VerifyMinMaxSupportHeights();
   VerifyTotalBridgeLength();
#endif
}

int Bridge::NumSupports() const
{
   return numSupports;
}

Support *Bridge::GetSupport(int id) const
{
   PRECONDITIONX(id >= 0,"Bridge::GetSupport() - Invalid support id");

   Support *support;
   int i;
   support = firstSupport;
   for (i = 1; i < id; i++)
      support = support->NextSupport();

   CHECKX(support,"Bridge::GetSupport() - Support not found");

   return support;
}

Support *Bridge::GetFirstSupport() const
{
//   CHECKX(firstSupport,"Bridge::GetFirstSupport() - first support is invalid");
   return firstSupport;
}

Support *Bridge::GetLastSupport() const
{
//   CHECKX(lastSupport,"Bridge::GetLastSupport() - last support is invalid");
   return lastSupport;
}

void Bridge::ReplaceSupport(int id,Support *aSupport)
{
   Support *oldSupport;
   Span *leftSpan;
   Span *rightSpan;
   Support *prevSupport;
   Support *nextSupport;

   oldSupport = GetSupport(id);
   leftSpan = oldSupport->LeftSpan();
   rightSpan = oldSupport->RightSpan();
   prevSupport = oldSupport->PrevSupport();
   nextSupport = oldSupport->NextSupport();

   aSupport->LeftSpan(leftSpan);
   aSupport->RightSpan(rightSpan);
   aSupport->PrevSupport(prevSupport);
   aSupport->NextSupport(nextSupport);

   if (leftSpan)
      leftSpan->RightSupport(aSupport);

   if (rightSpan)
      rightSpan->LeftSupport(aSupport);

   if (prevSupport)
      prevSupport->NextSupport(aSupport);

   if (nextSupport)
      nextSupport->PrevSupport(aSupport);

   if (oldSupport == firstSupport)
      firstSupport = aSupport;

   if (oldSupport == lastSupport)
      lastSupport = aSupport;

   aSupport->ID = oldSupport->ID;

   double oldMinSupportHeight = minSupportHeight;
   double oldMaxSupportHeight = maxSupportHeight;

   maxSupportHeight = max(maxSupportHeight,aSupport->Height());
   minSupportHeight = min(minSupportHeight,aSupport->Height());

   // if the support that was replaced had the old min/max height, then
   // we must go through all the supports to find the new extreme values
   if (zero_val(oldMinSupportHeight - oldSupport->Height()) ||
       zero_val(oldMaxSupportHeight - oldSupport->Height()))
      UpdateMinMaxSupportHeight();

   delete oldSupport;

#if defined GEN_DEBUG_CODE
   TestLinkedLists();
   VerifyMinMaxSpanLengths();
   VerifyMinMaxSupportHeights();
   VerifyTotalBridgeLength();
#endif
}

void Bridge::RemoveSupport(int id,bool leftSpan)
{
   Support *support;
   support = GetSupport(id);

   if (leftSpan)
      RemoveSpan(support->LeftSpan()->GetID(),true);
   else
      RemoveSpan(support->RightSpan()->GetID(),false);
}

void Bridge::Insert(int spanId,bool after,Span *newSpan,Support *newSupport,bool isRightSupport)
{
   Span *refSpan;
   refSpan = GetSpan(spanId);

   if (after)
      InsertAfter(refSpan,newSpan,newSupport,isRightSupport);
   else
      InsertBefore(refSpan,newSpan,newSupport,isRightSupport);

   totalBridgeLength += newSpan->Length();
   maxSpanLength = max(maxSpanLength,newSpan->Length());
   minSpanLength = min(minSpanLength,newSpan->Length());
   maxSupportHeight = max(maxSupportHeight,newSupport->Height());
   minSupportHeight = min(minSupportHeight,newSupport->Height());

   numSpans++;
   numSupports++;

   ReIndexSpans();
   ReIndexSupports();

#if defined GEN_DEBUG_CODE
   TestLinkedLists();
   VerifyMinMaxSpanLengths();
   VerifyMinMaxSupportHeights();
   VerifyTotalBridgeLength();
#endif
}

void Bridge::AddLoading(BridgeLoading *loading)
{
   loadings->Add(loading);
}

BridgeLoading *Bridge::GetLoading(int id)
{
   BridgeLoadingIterator i(*loadings);
   BridgeLoading *loading = NULL;

   while(i)
      {
      loading = i++;
      if (loading->GetID() == id)
         break;
      else
         loading = NULL;
      }

   CHECKX(loading,"Bridge::GetLoading() - Loading not found");

   return loading;
}

void Bridge::RemoveLoading(BridgeLoading *loading)
{
   loadings->Detach(loading,TShouldDelete::Delete);
}

//
//
// Structural Analysis Methods ------------------------------------------------
//
//

bool Bridge::Analyze(ProgressIndicator *pi)
{
   PRECONDITIONX(minAnalysisPoints > 1,"Bridge::Analyze() - minAnalysisPoints not > 1");

   progressIndicator = pi;

   diagrams->Flush(TShouldDelete::Delete);
   results->Flush(TShouldDelete::Delete);

   ResetNextAvailCounters();

   PROGRESS_MSG("Building Analytical Model");

   if (!GenerateModel())
      return false;

   if (!GeneratePOI())
      return false;

   #if defined GEN_DEBUG_CODE
      ReportPOI();
   #endif

   if (!ApplyLoadings())
      return false;

   SHOULD_CONTINUE;

   try
      {
      femModel->StiffnessAnalysis();
      }
   catch(xmsg xMsg)
      {
      PROGRESS_MSG(xMsg.why().c_str());
      return false;
      }

   return true;
}


bool Bridge::GenerateModel()
{
   Span *span;
   Support *support;

   //
   // Clear old model
   //
   for (span = firstSpan; span; span = span->NextSpan())
      {
      if (span->OkToModel())
         span->members->Flush();
      else
         {
         ostrstream os;
         os << "Fatal error in Span " << span->GetID() << ends;
         PROGRESS_MSG(os.str());
         delete[] os.str();
         return false;
         }
      }

   for (support = firstSupport; support; support = support->NextSupport())
      {
      if (support->OkToModel())
         support->members->Flush();
      else
         {
         ostrstream os;
         os << "Fatal error in Support " << support->GetID() << ends;
         PROGRESS_MSG(os.str());
         delete[] os.str();
         return false;
         }
      }

   delete femModel;
   femModel = new FemModel();

   //
   // Build the model
   //

   // Create a joint from which the rest of the model will be built around
   int jctJnt = NextAvailJnt();
   femModel->AddJoint(new Joint(jctJnt,0.00,0.00));

   // Build the first support
   if (!firstSpan->IsCantilevered())
      SupportToFem(jctJnt,firstSupport);

   // Build the spans... this also builds the right supports
   for (span = firstSpan; span; span = span->NextSpan())
      {
      jctJnt = SpanToFem(jctJnt,span);
      }

   SHOULD_CONTINUE;

   return true;
}

bool Bridge::ApplyLoadings()
{
   // Apply the loadings to the bridge
   BridgeLoadingIterator i(*loadings);
   while(i)
      {
      BridgeLoading *loading = i++;
      LoadingToFem(loading);

      SHOULD_CONTINUE;
      }

   return true;
}

//
//
// Modeling "Helper" Methods --------------------------------------------------
//
//

void Bridge::InitExtremeValues()
{
   minSpanLength = MAXDOUBLE;
   maxSpanLength = 0.0;

   minSupportHeight = MAXDOUBLE;
   maxSupportHeight = 0.0;
}

void Bridge::UpdateIDs()
{
   // Updating ID's could be more efficient when a single span or support
   // is removed or inserted
   ReIndexSpans();
   ReIndexSupports();
}

// Maybe the reindex functions could have a "Beginning with" argument that
// is a pointer to the span or support the re-indexing begins with? This
// would make the reindexing more efficient.
void Bridge::ReIndexSpans()
{
   Span *curSpan;
   int i = 1;
   for(curSpan = firstSpan; curSpan; curSpan = curSpan->NextSpan())
      curSpan->SetID(i++);
}

void Bridge::ReIndexSupports()
{
   Support *curSupport;
   int i = 1;
   for (curSupport = firstSupport; curSupport; curSupport = curSupport->NextSupport())
      curSupport->SetID(i++);
}

void Bridge::UpdateMinMaxSpanLength()
{
   Span *span;
   minSpanLength = MAXDOUBLE;
   maxSpanLength = -MAXDOUBLE;

   for (span = firstSpan; span; span = span->NextSpan())
      {
      minSpanLength = min(minSpanLength,span->Length());
      maxSpanLength = max(maxSpanLength,span->Length());
      }

   CHECKX(minSpanLength > 0,"Bridge::UpdateMinMaxSpanLength() - min span length < 0");
   CHECKX(maxSpanLength > 0,"Bridge::UpdateMinMaxSpanLength() - max span length < 0");
}

void Bridge::UpdateMinMaxSupportHeight()
{
   Support *support;
   minSupportHeight = MAXDOUBLE;
   maxSupportHeight = -MINDOUBLE;

   for (support = firstSupport; support; support = support->NextSupport())
      {
      minSupportHeight = min(minSupportHeight,support->Height());
      maxSupportHeight = max(maxSupportHeight,support->Height());
      }

   CHECKX(minSupportHeight >= 0,"Bridge::UpdateMinMaxSupportHeight() - min support height < 0");
   CHECKX(maxSupportHeight >= 0,"Bridge::UpdateMinMaxSupportHeight() - max support height < 0");
}

void Bridge::InsertAfter(Span *refSpan,Span *newSpan,Support *newSupport,bool isRightSupport)
{
   Span *nextSpan;
   Support *leftSupport;
   Support *rightSupport;

   nextSpan = refSpan->NextSpan();
   leftSupport = refSpan->LeftSupport();
   rightSupport = refSpan->RightSupport();

   if (isRightSupport)
      {
      CHECKX(!(refSpan->IsCantilevered() && refSpan == lastSpan),"Bridge::InsertAfter - cannot insert a right supported span after the last span if it is cantilevered");

      // Insert new span and support after the current span. The span is
      // right supported. The refSpan keeps its right support.

      newSpan->PrevSpan(refSpan);
      refSpan->NextSpan(newSpan);

      newSpan->RightSupport(newSupport);
      newSupport->LeftSpan(newSpan);

      newSpan->NextSpan(nextSpan);
      if(nextSpan)
         nextSpan->PrevSpan(newSpan);

      rightSupport->RightSpan(newSpan);
      newSpan->LeftSupport(rightSupport);

      rightSupport->NextSupport(newSupport);
      newSupport->PrevSupport(rightSupport);

      newSupport->RightSpan(nextSpan);
      if (nextSpan)
         nextSpan->LeftSupport(newSupport);

      if (nextSpan && nextSpan->RightSupport())
         {
         newSupport->NextSupport(nextSpan->RightSupport());
         nextSpan->RightSupport()->PrevSupport(newSupport);
         }

      if (lastSpan == refSpan)
         lastSpan = newSpan;

      if (lastSupport == rightSupport)
         lastSupport = newSupport;
      }
   else
      {
      // Insert new span and support after the current span. The span is
      // left supported. The refSpan takes newSupport as its right support,
      // and the newSpan takes the refSpan's right support as its right support

      newSpan->PrevSpan(refSpan);
      refSpan->NextSpan(newSpan);

      newSpan->LeftSupport(newSupport);
      newSupport->RightSpan(newSpan);

      newSpan->NextSpan(nextSpan);
      if (nextSpan)
         nextSpan->PrevSpan(newSpan);

      newSupport->LeftSpan(refSpan);
      refSpan->RightSupport(newSupport);

      newSpan->RightSupport(rightSupport);
      if (rightSupport)
         rightSupport->LeftSpan(newSpan);

      newSupport->PrevSupport(leftSupport);
      if (leftSupport)
         leftSupport->NextSupport(newSupport);

      newSupport->NextSupport(rightSupport);
      if (rightSupport)
         rightSupport->PrevSupport(newSupport);

      if (lastSpan == refSpan)
         lastSpan = newSpan;

      if (lastSupport == leftSupport || lastSupport == rightSupport)
         lastSupport = newSupport;
      }
}

void Bridge::InsertBefore(Span *refSpan,Span *newSpan,Support *newSupport,bool isRightSupport)
{
   Span *prevSpan;
   Support *leftSupport;
   Support *rightSupport;

   prevSpan = refSpan->PrevSpan();
   leftSupport = refSpan->LeftSupport();
   rightSupport = refSpan->RightSupport();

   if (isRightSupport)
      {
      // Insert new span and support before the current span. The newSpan is
      // right supported. Takes the new support as its left support and the
      // left support becomes the left support for the new span

      newSpan->NextSpan(refSpan);
      refSpan->PrevSpan(newSpan);

      newSpan->RightSupport(newSupport);
      newSupport->LeftSpan(newSpan);

      newSupport->RightSpan(refSpan);
      refSpan->LeftSupport(newSupport);

      newSpan->PrevSpan(prevSpan);
      if (prevSpan)
         prevSpan->NextSpan(newSpan);

      newSpan->LeftSupport(leftSupport);
      if (leftSupport)
         leftSupport->RightSpan(newSpan);

      newSupport->NextSupport(rightSupport);
      if (rightSupport)
         rightSupport->PrevSupport(newSupport);

      newSupport->PrevSupport(leftSupport);
      if (leftSupport)
         leftSupport->NextSupport(newSupport);

      if (firstSpan == refSpan)
         firstSpan = newSpan;

      if (firstSupport == rightSupport)
         firstSupport = newSupport;
      }
   else
      {
      CHECKX(refSpan != firstSpan && !refSpan->IsCantilevered(),"Bridge::InsertBefore - cannot insert a left supported span before the first span if it is cantilevered");

      // Insert new span and support before the current span. The newSpan is
      // left supported. The refSpan retains its left support.

      newSpan->NextSpan(refSpan);
      refSpan->PrevSpan(newSpan);

      newSpan->PrevSpan(prevSpan);
      if (prevSpan)
         prevSpan->NextSpan(newSpan);

      newSpan->LeftSupport(newSupport);
      newSupport->RightSpan(newSpan);

      newSupport->LeftSpan(prevSpan);
      if (prevSpan)
         prevSpan->RightSupport(newSupport);

      newSpan->RightSupport(leftSupport);
      if (leftSupport)
         leftSupport->LeftSpan(newSpan);

      newSupport->NextSupport(leftSupport);
      if (leftSupport)
         leftSupport->PrevSupport(newSupport);

      if (leftSupport && leftSupport->PrevSupport())
         {
         newSupport->PrevSupport(leftSupport->PrevSupport());
         leftSupport->PrevSupport(newSupport);
         }

      if (firstSpan == refSpan)
         firstSpan = newSpan;

      if (leftSupport == firstSupport || rightSupport == firstSupport)
         firstSupport = newSupport;
      }
}

int Bridge::SpanToFem(int jctJnt,Span *span)
{
   int endJnt;

   endJnt = span->GenerateFemComponents(jctJnt,this);
   if (span->RightSupport())
      SupportToFem(endJnt,span->RightSupport());

   return endJnt;
}

void Bridge::SupportToFem(int jctJnt,Support *support)
{
   support->GenerateFemComponents(jctJnt,this);
}

void Bridge::LoadingToFem(BridgeLoading *loading)
{
   BridgeLoadIterator i(*loading->loads);
   Loading *femLoading;
   int ld;

   ld = NextAvailLoading();
   loading->femId = ld;

   femLoading = new Loading(ld);
   femModel->AddLoading(femLoading);

   while(i)
      {
      BridgeLoad *load = i++;
      load->GetSpan()->ApplyLoad(ld,load,this);
      }
}

//
// Counters
//

void Bridge::ResetNextAvailCounters()
{
   nextAvailJnt = 1;
   nextAvailMbr = 1;
   nextAvailLoading = 1;
   nextAvailDiagram = 0;
   nextAvailAnalysisResults = 0;
   nextAvailPointOfInterest = 0;
}

int Bridge::NextAvailJnt()
{
   int id = nextAvailJnt;
   nextAvailJnt++;
   return id;
}

int Bridge::NextAvailMbr()
{
   int id = nextAvailMbr;
   nextAvailMbr++;
   return id;
}

int Bridge::NextAvailLoading()
{
   int id = nextAvailLoading;
   nextAvailLoading++;
   return id;
}

int Bridge::NextAvailCombo()
{
   return NextAvailLoading();
}

int Bridge::NextAvailDiagram()
{
   int id = nextAvailDiagram;
   nextAvailDiagram++;
   return id;
}

int Bridge::NextAvailAnalysisResults()
{
   int id = nextAvailAnalysisResults;
   nextAvailAnalysisResults++;
   return id;
}

int Bridge::NextAvailPointOfInterest()
{
   int id = nextAvailPointOfInterest;
   nextAvailPointOfInterest++;
   return id;
}

//
//
// Bridge Statistics ----------------------------------------------------------
//
//

double Bridge::Length() const
{
   return totalBridgeLength;
}

double Bridge::MaxSpanLength() const
{
   return maxSpanLength;
}

double Bridge::MinSpanLength() const
{
   return minSpanLength;
}

double Bridge::MaxSupportHeight() const
{
   return maxSupportHeight;
}

double Bridge::MinSupportHeight() const
{
   return minSupportHeight;
}

//
//
// Misc Methods ---------------------------------------------------------------
//
//

Diagram *Bridge::GetDiagram(int id)
{
   DiagramIterator i(*diagrams);
   Diagram *diagram = NULL;

   while(i)
      {
      diagram = i++;
      if (diagram->GetID() == id)
         break;
      else
         diagram = NULL;
      }

   CHECKX(diagram,"Bridge::GetDiagram() - diagram not found");

   return diagram;
}

void Bridge::AddDiagram(Diagram *dia)
{
   diagrams->Add(dia);
}

void Bridge::RemoveDiagram(int id)
{
   diagrams->Destroy(GetDiagram(id));
}

AnalysisResults *Bridge::GetAnalysisResults(int id)
{
   AnalysisResultsIterator i(*results);
   AnalysisResults *result = NULL;

   while(i)
      {
      result = i++;
      if (result->GetID() == id)
         break;
      else
         result = NULL;
      }

   CHECKX(result,"Bridge::GetAnalysisResults() - analysis results not found");

   return result;
}

void Bridge::AddAnalysisResults(AnalysisResults *result)
{
   results->Add(result);
}

void Bridge::RemoveAnalysisResults(int id)
{
   results->Destroy(GetAnalysisResults(id));
}

double Bridge::GetSuperstructureOffset(int spanId,double offset)
{
   Span *span;
   double x,y;

   span = GetSpan(spanId);

   CHECKX(span,"Bridge::GetSuperstructureOffset() - span not found");

   span->GetStartPoint(x,y);

   return x + offset;
}

void Bridge::GetSpanOffset(double gOffset,int &spanId,double &lOffset)
{
   Span *span;
   double x,y;

   span = GetSpan(gOffset);

   CHECKX(span,"Bridge::GetSpanOffset() - span not found");

   span->GetStartPoint(x,y);

   spanId = span->GetID();
   lOffset = gOffset - x;
}

Bridge::SortedPointOfInterestIterator::SortedPointOfInterestIterator(int spanId,PointOfInterestContainer &pntsOfInterest)
{
   sortedContainer = new TISArrayAsVector<PointOfInterest>(10,0,10);
   sortedContainer->OwnsElements(false);

   PointOfInterestIterator iter(pntsOfInterest);
   while(iter)
      {
      PointOfInterest *poi = iter++;
      if (poi->spanId == spanId)
         sortedContainer->Add(poi);
      }

   Restart(0,sortedContainer->GetItemsInContainer());
}

Bridge::SortedPointOfInterestIterator::~SortedPointOfInterestIterator()
{
   delete sortedContainer;
}

Bridge::PointOfInterest* Bridge::SortedPointOfInterestIterator::Current() const
{
   return (*sortedContainer)[Cur];
}

Bridge::SortedPointOfInterestIterator::operator int() const
{
   return Cur < Upper;
}

Bridge::PointOfInterest* Bridge::SortedPointOfInterestIterator::operator ++ ( int )
{
   PointOfInterest* temp = Current();
   Cur++;
   return temp;
}

Bridge::PointOfInterest* Bridge::SortedPointOfInterestIterator::operator ++ ()
{
   Cur++;
   return Current();
}

void Bridge::SortedPointOfInterestIterator::Restart( unsigned start, unsigned stop )
{
   Cur = Lower = start;
   Upper = stop;
}

Bridge::PointOfInterest* Bridge::SortedPointOfInterestIterator::Next() const
{
   return (*sortedContainer)[Cur+1];
}

Bridge::PointOfInterest* Bridge::SortedPointOfInterestIterator::Prev() const
{
   return (*sortedContainer)[Cur-1];
}

//
//
// Debugging Code -------------------------------------------------------------
//
//

#if defined GEN_DEBUG_CODE

void Bridge::TestLinkedLists() const
{
   Span *s;
   for(s = firstSpan; s; s = s->NextSpan())
      {
      if (s != firstSpan && s != lastSpan)
         {
         CHECKX(s->PrevSpan() != NULL,"Bridge::TestLinkedLists - 1");
         CHECKX(s->NextSpan() != NULL,"Bridge::TestLinkedLists - 2");
         CHECKX(s->LeftSupport() != NULL,"Bridge::TestLinkedLists - 3");
         CHECKX(s->RightSupport() != NULL,"Bridge::TestLinkedLists - 4");
         }

      if (s == firstSpan)
         {
         CHECKX(s->PrevSpan() == NULL,"Bridge::TestLinkedLists - 5");
         }

      if (s->LeftSupport() && s->RightSupport())
         {
         CHECKX(s == s->LeftSupport()->RightSpan(),"Bridge::TestLinkedLists - 6");
         CHECKX(s == s->RightSupport()->LeftSpan(),"Bridge::TestLinkedLists - 7");
         CHECKX(s->LeftSupport() == s->RightSupport()->PrevSupport(),"Bridge::TestLinkedLists - 8");
         CHECKX(s->RightSupport() == s->LeftSupport()->NextSupport(),"Bridge::TestLinkedLists - 9");
         }

      if (s->PrevSpan())
         {
         CHECKX(s->PrevSpan() == s->LeftSupport()->LeftSpan(),"Bridge::TestLinkedLists - 10");
         CHECKX(s == s->PrevSpan()->NextSpan(),"Bridge::TestLinkedLIsts - 11");
         }

      if (s->NextSpan())
         {
         CHECKX(s->NextSpan() == s->RightSupport()->RightSpan(),"Bridge::TestLinkedLists - 12");
         CHECKX(s == s->NextSpan()->PrevSpan(),"Bridge::TestLinkedLIsts - 13");
         }

      if (s == lastSpan)
         {
         CHECKX(s->NextSpan() == NULL,"Bridge::TestLinkedLists - 14");
         }
      else
         {
         CHECKX(s->NextSpan() == s->RightSupport()->RightSpan(),"Bridge::TestLinkedLists - 15");
         }

      }

   int _numSpans = 0;
   int _numSupports = 0;

   for (Span *span = firstSpan; span; span = span->NextSpan())
      _numSpans++;

   CHECKX((_numSpans - numSpans) == 0,"Bridge::TestLinkedList - # of spans test failed");

   for (Support *support = firstSupport; support; support = support->NextSupport())
      _numSupports++;

   CHECKX((_numSupports - numSupports) == 0,"Bridge::TestLinkedList - # of supports test failed");

}

void Bridge::VerifyMinMaxSpanLengths() const
{
   Span *span;
   double _max,_min;

   _max = 0;
   _min = MAXDOUBLE;

   for (span = firstSpan; span; span = span->NextSpan())
      {
      _max = max(_max,span->Length());
      _min = min(_min,span->Length());
      }

   CHECKX(zero_val(_max-maxSpanLength),"Bridge::VerifyMinMaxSpanLengths - test failed");
   CHECKX(zero_val(_min-minSpanLength),"Bridge::VerifyMinMaxSpanLengths - test failed");
}

void Bridge::VerifyMinMaxSupportHeights() const
{
   Support *support;
   double _max,_min;

   _max = 0;
   _min = MAXDOUBLE;

   for (support = firstSupport; support; support = support->NextSupport())
      {
      _max = max(_max,support->Height());
      _min = min(_min,support->Height());
      }

   CHECKX(zero_val(_max-maxSupportHeight),"Bridge::VerifyMinMaxSupportHeights - test failed");
   CHECKX(zero_val(_min-minSupportHeight),"Bridge::VerifyMinMaxSupportHeights - test failed");
}

void Bridge::VerifyTotalBridgeLength() const
{
   double _length = 0.0;
   Span *span;

   for (span = firstSpan; span; span = span->NextSpan())
      _length += span->Length();

   CHECKX(zero_val(_length-totalBridgeLength),"Bridge::VerifyTotalBridgeLength() - test failed");
}

#endif // GEN_DEBUG_CODE

