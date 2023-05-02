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

#if defined GEN_DEBUG_CODE
#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>
#endif

DIAG_DECLARE_GROUP(Bridge);
DIAG_DECLARE_GROUP(Bridge_PntsOfInterest);

#undef EPS
#define EPS  1.0e-5
#undef   zero_val
#define  zero_val( x )     (((-EPS)<=(x)) && ((x)<=(EPS)))

bool Bridge::GeneratePOI()
{
   TRACEX(Bridge_PntsOfInterest,LEVEL1,"Entering Bridge::GeneratePOI()");

   //
   // Get ready to generate new POIs. Get rid of any old ones.
   //
   pntsOfInterest->Flush(TShouldDelete::Delete);

   if (!GenerateMinPOI())
      return false;

   if (!GenerateSegmentPOI())
      return false;

   if (!GenerateHingePOI())
      return false;

   if (!GenerateLoadPOI())
      return false;

   TRACEX(Bridge_PntsOfInterest,LEVEL1,"Leaving Bridge::GeneratePOI()");

   return true;
}

bool Bridge::GenerateMinPOI()
{
   int poi;         // point of interest id
   Span *span;      // span
   int i;           // counter
   double length;   // span length
   double offset;   // offset from start of span
   int mbrId;       // fem member id
   double frDist;   // fractional distance from start of member

   //
   // Generate at span nth points
   //
   TRACEX(Bridge_PntsOfInterest,LEVEL2,"*** Generate at span nth points ***");

   for (span = GetFirstSpan(); span; span = span->NextSpan())
      {
      length = span->Length();

      for (i = 0; i < minAnalysisPoints; i++)
         {
         poi = NextAvailPointOfInterest();

         offset = (length*i)/(minAnalysisPoints-1);

         TRACEX(Bridge_PntsOfInterest,LEVEL2,"POI " << poi << " span = " << span->GetID() << " offset = " << offset);

         pntsOfInterest->Add(new PointOfInterest(poi,span->GetID(),offset));
         span->FindMbr(femModel,offset,mbrId,frDist);
         femModel->DefinePOI(poi,mbrId,frDist,true);

         SHOULD_CONTINUE;
         }
      }

   return true;
}

bool Bridge::GenerateSegmentPOI()
{
   int poi;         // point of interest id
   Span *span;      // span
   double offset;   // offset from start of span
   int mbrId;       // fem member id
   double frDist;   // fractional distance from start of member


   //
   // Generate at segment end points
   //
   TRACEX(Bridge_PntsOfInterest,LEVEL2,"*** Generate at segment end points ***");
   for (span = GetFirstSpan(); span; span = span->NextSpan())
      {
      SegmentIterator l(*span->segments);
      while(l)
         {
         Segment *segment = l++;
         offset = segment->GetLocation();

         // Start of segment
         if (!CheckPOIConflict(span->GetID(),offset))
            {
            poi = NextAvailPointOfInterest();
            pntsOfInterest->Add(new PointOfInterest(poi,span->GetID(),offset));
            span->FindMbr(femModel,offset,mbrId,frDist);
            femModel->DefinePOI(poi,mbrId,frDist,true);

            TRACEX(Bridge_PntsOfInterest,LEVEL2,"POI " << poi << " span = " << span->GetID() << " offset = " << offset);
            }

         offset = segment->GetLocation() + segment->GetLength();

         // End of Segment
         if (!CheckPOIConflict(span->GetID(),offset))
            {
            poi = NextAvailPointOfInterest();
            pntsOfInterest->Add(new PointOfInterest(poi,span->GetID(),offset));
            span->FindMbr(femModel,offset,mbrId,frDist);
            femModel->DefinePOI(poi,mbrId,frDist,true);

            TRACEX(Bridge_PntsOfInterest,LEVEL2,"POI " << poi << " span = " << span->GetID() << " offset = " << offset);
            }

         SHOULD_CONTINUE;
         }
      }

   return true;
}

bool Bridge::GenerateHingePOI()
{
   int poi;         // point of interest id
   Span *span;      // span
   double offset;   // offset from start of span
   int mbrId;       // fem member id
   double frDist;   // fractional distance from start of member
   int i;           // general purpose counter
   int nHinges;      // number of hinges


   //
   // Generate at segment end points
   //
   TRACEX(Bridge_PntsOfInterest,LEVEL2,"*** Generate at hinge points ***");
   for (span = GetFirstSpan(); span; span = span->NextSpan())
      {
      nHinges = span->NumHinges();
      for (i = 0; i < nHinges; i++)
         {
         Hinge *aHinge = span->GetHinge(i);
         offset = aHinge->GetLocation();

         if (!CheckPOIConflict(span->GetID(),offset))
            {
            poi = NextAvailPointOfInterest();
            pntsOfInterest->Add(new PointOfInterest(poi,span->GetID(),offset));
            span->FindMbr(femModel,offset,mbrId,frDist);
            femModel->DefinePOI(poi,mbrId,frDist,true);

            TRACEX(Bridge_PntsOfInterest,LEVEL2,"POI " << poi << " span = " << span->GetID() << " offset = " << offset);
            }
         SHOULD_CONTINUE;
         }
      }

   return true;
}

bool Bridge::GenerateLoadPOI()
{
   int poi;         // point of interest id
   double offset;   // offset from start of span
   int mbrId;       // fem member id
   double frDist;   // fractional distance from start of member

   //
   // Generate at load application points
   //
   TRACEX(Bridge_PntsOfInterest,LEVEL2,"*** Generate at load application points ***");
   BridgeLoadingIterator j(*loadings);
   while(j)
      {
      BridgeLoading *loading = j++;

      BridgeLoadIterator k(*loading->loads);
      while (k)
         {
         BridgeLoad *load = k++;
         ConcBridgeLoad *cload;
         UnifBridgeLoad *uload;
         switch(load->Type())
            {
            case blConc:
                 cload = TYPESAFE_DOWNCAST(load,ConcBridgeLoad);
                 offset = cload->StartLocation();
                 if (cload->IsFract())
                    offset *= cload->GetSpan()->Length();

                 if (!CheckPOIConflict(cload->GetSpan()->GetID(),offset))
                    {
                    poi = NextAvailPointOfInterest();
                    pntsOfInterest->Add(new PointOfInterest(poi,cload->GetSpan()->GetID(),offset));
                    cload->GetSpan()->FindMbr(femModel,offset,mbrId,frDist);
                    femModel->DefinePOI(poi,mbrId,frDist,true);

                    TRACEX(Bridge_PntsOfInterest,LEVEL2,"POI " << poi << " span = " << cload->GetSpan()->GetID() << " offset = " << offset);
                    }
                 break;

            case blUnif:
                 uload = TYPESAFE_DOWNCAST(load,UnifBridgeLoad);
                 offset = uload->StartLocation();
                 if (uload->IsFract())
                    offset *= uload->GetSpan()->Length();

                 // Start of load
                 if (!CheckPOIConflict(uload->GetSpan()->GetID(),offset))
                    {
                    poi = NextAvailPointOfInterest();
                    pntsOfInterest->Add(new PointOfInterest(poi,uload->GetSpan()->GetID(),offset));
                    uload->GetSpan()->FindMbr(femModel,offset,mbrId,frDist);
                    femModel->DefinePOI(poi,mbrId,frDist,true);

                    TRACEX(Bridge_PntsOfInterest,LEVEL2,"POI " << poi << " span = " << uload->GetSpan()->GetID() << " offset = " << offset);
                    }

                 // End of load
                 offset = uload->EndLocation();
                 if (uload->IsFract())
                    offset *= uload->GetSpan()->Length();

                 if (!CheckPOIConflict(uload->GetSpan()->GetID(),offset))
                    {
                    poi = NextAvailPointOfInterest();
                    pntsOfInterest->Add(new PointOfInterest(poi,uload->GetSpan()->GetID(),offset));
                    uload->GetSpan()->FindMbr(femModel,offset,mbrId,frDist);
                    femModel->DefinePOI(poi,mbrId,frDist,true);
                    TRACEX(Bridge_PntsOfInterest,LEVEL2,"POI " << poi << " span = " << uload->GetSpan()->GetID() << " offset = " << offset);
                    }
                 break;
            } // end of switch

         SHOULD_CONTINUE;
         }
      }

   return true;
}

bool Bridge::CheckPOIConflict(int spanId,double offset)
{
   PointOfInterestIterator i(*pntsOfInterest);

   while(i)
      {
      PointOfInterest *poi = i++;
      if ( (spanId == poi->spanId) &&
           zero_val(offset - poi->offset) )
         return true;
      }

   return false;
}

bool Bridge::PointOfInterest::operator == (const PointOfInterest &poi) const
{
   return (poi.id == id);
}

bool Bridge::PointOfInterest::operator <  (const PointOfInterest &poi) const
{
   bool retval;
   if (spanId == poi.spanId)
      retval = (offset < poi.offset);
   else if (spanId < poi.spanId)
      retval = true;
   else
      retval = false;

   return retval;
}

int Bridge::SpanPntToPOI(int spanId,int spanPnt)
{
   Span *span;
   double offset;
   PointOfInterestIterator i(*pntsOfInterest);

   span = GetSpan(spanId);
   offset = span->Length()*spanPnt/(minAnalysisPoints - 1);

   while(i)
      {
      PointOfInterest *poi = i++;
      if ( (spanId == poi->spanId) &&
            zero_val(offset-poi->offset) )
         return poi->id;
      }

   return -1;
}

Bridge::PointOfInterest *Bridge::GetPointOfInterest(int poiId)
{
   PointOfInterest find(poiId,-1,-1);
   int idx;

   idx = pntsOfInterest->Find(&find);

   CHECKX(idx != INT_MAX,"Bridge::GetPointOfInterest() - poi not found");

   return (*pntsOfInterest)[idx];
}

#if defined GEN_DEBUG_CODE
void Bridge::ReportPOI()
{
   ofstream ofile("poi.log");

   ofile << "Points of Interest Report" << endl << endl;

   PointOfInterestIterator iter(*pntsOfInterest);
   while(iter)
      {
      PointOfInterest *poi = iter++;

      ofile << "Poi "     << setw(3) << setiosflags(ios::right) << poi->id
            << " Span "   << setw(3) << setiosflags(ios::right) << poi->spanId
            << " Offset " << setw(10) << setprecision(4) << setiosflags(ios::showpoint | ios::floatfield)
            << poi->offset << endl;
      }

   ofile.close();
}
#endif

