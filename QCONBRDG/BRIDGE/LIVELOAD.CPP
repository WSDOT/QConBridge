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

#include <bridge\lrfdbrdg.h>
#include <bridge\lrfdspan.h>
#include <float.h>
#include <fem2d\femmodel.h>
#include <code\lrfdcode.h>

#if defined GEN_DEBUG_CODE
#define TRACE_TANDEM       false
#define TRACE_TRUCK        false
#define TRACE_TRUCKTRAIN   false
#define TRACE_TANDEMTRAIN  false
#define TRACE_FATIGUETRUCK false
#define TRACE_PEDESTRIAN   false
#define TRACE_POI          23
#define TRACE_MAXMOMENT    1
#define TRACE_MINMOMENT    2
#define TRACE_MAXSHEAR     3
#define TRACE_MINSHEAR     4
int  traceType = TRACE_MAXMOMENT;
bool traceState;
bool thisPoint;
#endif

DIAG_DEFINE_GROUP(LRFDBridge_Liveload,DISABLED,LEVEL2);



bool LRFDBridge::DualTruckLoadEnabled(int train) const
{
   return (train == DUALTRUCKS) ? dualTruckTrain.enableGeneration
                                : dualTandemTrain.enableGeneration;
}

void LRFDBridge::DualTruckLoadEnabled(int train,bool enable)
{
   if (train == DUALTRUCKS)
      dualTruckTrain.enableGeneration = enable;
   else
      dualTandemTrain.enableGeneration = enable;
}

bool LRFDBridge::DualTruckRangeEnabled(int train,int range)
{
   return (train == DUALTRUCKS) ? dualTruckTrain.rangeEnabled[range]
                                : dualTandemTrain.rangeEnabled[range];
}

void LRFDBridge::DualTruckRangeEnabled(int train,int range,bool enable)
{
   if (train == DUALTRUCKS)
      dualTruckTrain.rangeEnabled[range] = enable;
	else
		dualTandemTrain.rangeEnabled[range] = enable;
}

double LRFDBridge::FromHeadwaySpacing(int train,int range)
{
   return (train == DUALTRUCKS) ? dualTruckTrain.fromHeadwaySpacing[range]
                                : dualTandemTrain.fromHeadwaySpacing[range];
}

void LRFDBridge::FromHeadwaySpacing(int train,int range,double from)
{
   if (train == DUALTRUCKS)
      dualTruckTrain.fromHeadwaySpacing[range] = from;
   else
      dualTandemTrain.fromHeadwaySpacing[range] = from;
}

double LRFDBridge::ToHeadwaySpacing(int train,int range)
{
   return (train == DUALTRUCKS) ? dualTruckTrain.toHeadwaySpacing[range]
                                : dualTandemTrain.toHeadwaySpacing[range];
}

void LRFDBridge::ToHeadwaySpacing(int train,int range,double to)
{
   if (train == DUALTRUCKS)
       dualTruckTrain.toHeadwaySpacing[range] = to;
   else
       dualTandemTrain.toHeadwaySpacing[range] = to;
}

int LRFDBridge::NumHeadwayInc(int train,int range)
{
   return (train == DUALTRUCKS) ? dualTruckTrain.numHeadwayInc[range]
                                : dualTandemTrain.numHeadwayInc[range];
}

void LRFDBridge::NumHeadwayInc(int train,int range,int inc)
{
   if (train == DUALTRUCKS)
      dualTruckTrain.numHeadwayInc[range] = inc;
   else
      dualTandemTrain.numHeadwayInc[range] = inc;
}

LRFDBridge::InfluenceLineRecordContainer *LRFDBridge::CreateInfluenceLineRecords()
{
   // Creates influence lines for all of the influence analysis results
   AnalysisResults *arMoment;
   AnalysisResults *arShear;
   InfluenceLine *moment;
   InfluenceLine *shear;
   InfluenceLineRecordContainer *ilRecords;
   InfluenceLineRecord *ilr;
   PointOfInterest *poi;

#if defined GEN_DEBUG_CODE
   ofstream ilMoment("ilmomdia.log");
	ofstream ilShear("ilshrdia.log");

   ilMoment << "Moment Influence Lines from diagrams" << endl;
   ilShear  << "Shear Influence Lines from diagrams" << endl;
#endif

   ilRecords = new InfluenceLineRecordContainer(10,0,10);

   InfluenceResultsRecordIterator iter(*inflResultsRecords);

   while(iter)
      {
      InfluenceResultsRecord *irr = iter++;

      arMoment = GetAnalysisResults(irr->arMomentId);
      moment = new InfluenceLine(irr->poiId,*arMoment);

      arShear = GetAnalysisResults(irr->arShearId);
      shear = new InfluenceLine(irr->poiId,*arShear);

      poi = GetPointOfInterest(irr->poiId);

      ilr = new InfluenceLineRecord(irr->poiId,poi->spanId,poi->offset,moment,NULL,shear,NULL);
      ilRecords->Add(ilr);

#if defined GEN_DEBUG_CODE
      ilMoment << (*moment) << endl;
      ilShear  << (*shear)  << endl;
#endif
		}


#if defined GEN_DEBUG_CODE
      ilMoment.close();
      ilShear.close();
#endif

   return ilRecords;
}

LRFDBridge::ReactionInfluenceLineRecordContainer *LRFDBridge::CreateReactionInfluenceLineRecords()
{
   // Creates influence lines for all of the influence analysis results
   AnalysisResults *arFx,*arFy,*arMz;
   InfluenceLine *ilFx,*ilFy,*ilMz;

   ReactionInfluenceLineRecordContainer *ilRecords;
   ReactionInfluenceLineRecord *ilr;

#if defined GEN_DEBUG_CODE
   ofstream logFx("ilFxdia.log");
   ofstream logFy("ilFydia.log");
   ofstream logMz("ilMzdia.log");

   logFx << "Fx Influence Lines from diagrams" << endl;
   logFy << "Fy Influence Lines from diagrams" << endl;
   logMz << "Mz Influence Lines from diagrams" << endl;
#endif

   ilRecords = new ReactionInfluenceLineRecordContainer(10,0,10);

   InfluenceReactionRecordIterator iter(*inflReactionRecords);

   while(iter)
      {
      InfluenceReactionRecord *irr = iter++;

      arFx = GetAnalysisResults(irr->arFxId);
		ilFx = new InfluenceLine(irr->supportId,*arFx);

      arFy = GetAnalysisResults(irr->arFyId);
		ilFy = new InfluenceLine(irr->supportId,*arFy);

      arMz = GetAnalysisResults(irr->arMzId);
		ilMz = new InfluenceLine(irr->supportId,*arMz);

      ilr = new ReactionInfluenceLineRecord(irr->supportId,ilFx,ilFy,ilMz);
      ilRecords->Add(ilr);

#if defined GEN_DEBUG_CODE
      logFx << (*arFx) << endl;
      logFy << (*arFy) << endl;
      logMz << (*arMz) << endl;
#endif
		}


#if defined GEN_DEBUG_CODE
      logFx.close();
      logFy.close();
      logMz.close();
#endif

   return ilRecords;
}

bool LRFDBridge::GenerateLiveloadEnvelopes()
{
   TRACEX(LRFDBridge_Liveload,LEVEL1,"LRFDBridge::GenerateLiveloadEnvelopes()");

   InfluenceLineRecordContainer *ilRecords = CreateInfluenceLineRecords();
   ReactionInfluenceLineRecordContainer *ilReactionRecords = CreateReactionInfluenceLineRecords();

   #if defined GEN_DEBUG_CODE
   logTruck = TRACE_TANDEM;
   #endif
   if (!CreateDesignTandemEnvelope(ilRecords,ilReactionRecords))
      return false;

   #if defined GEN_DEBUG_CODE
   logTruck = TRACE_TRUCK;
   #endif
   if (!CreateDesignTruckEnvelope(ilRecords,ilReactionRecords))
      return false;

   #if defined GEN_DEBUG_CODE
	logTruck = TRACE_TRUCKTRAIN;
	#endif
   if (!CreateDualTruckEnvelope(ilRecords,ilReactionRecords))
      return false;

   #if defined GEN_DEBUG_CODE
   logTruck = TRACE_TANDEMTRAIN;
   #endif
   if (!CreateDualTandemEnvelope(ilRecords,ilReactionRecords))
      return false;

   if (!CreateLiveLoadEnvelope())
      return false;

   #if defined GEN_DEBUG_CODE
   logTruck = TRACE_FATIGUETRUCK;
   #endif
   if (!CreateFatigueTruckEnvelope(ilRecords,ilReactionRecords))
      return false;

   #if defined GEN_DEBUG_CODE
   logTruck = TRACE_PEDESTRIAN;
   #endif
   if (!CreatePedestrianEnvelope(ilRecords,ilReactionRecords))
      return false;

   delete ilRecords;

   return true;
}

bool LRFDBridge::CreateDesignTandemEnvelope(InfluenceLineRecordContainer *il,ReactionInfluenceLineRecordContainer* ilReactions)
{
   bool retval;
   Truck *truck;
   TruckTrain *truckTrain;
   TruckTrainContainer *trains;


   if (!EnableDesignTandem())
      {
  	   AllocateEnvelopes(dcDesignTandem,"Design Tandem");
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDesignTandem,dtMaxMoment)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDesignTandem,dtMinMoment)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDesignTandem,dtMaxShear)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDesignTandem,dtMinShear)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDesignTandem,dtMaxReaction)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDesignTandem,dtMinReaction)),0);
      return true; // get the heck outta here
      }

   PROGRESS_MSG("Processing Design Tandem");

   // Setup truck train
   truckTrain = new TruckTrain(0);
   truckTrain->SetLaneLoad(9300.);
   truckTrain->SetTruckImpact(GetTruckImpact());
   truckTrain->SetLaneImpact(GetLaneImpact());

   // Setup truck
   truck = new Truck(1);
   truck->AddAxle(0.000,110000.);
   truck->AddAxle(1.200,110000.);

   // Add truck to truck train
   truckTrain->AddTruck(*truck,0.);

   // Setup container of truck trains
   trains = new TruckTrainContainer(2,0,2);
   trains->OwnsElements(false);
   trains->Add(truckTrain);

   retval = CreateEnvelope(dcDesignTandem,
									"Design Tandem",
									trains,
                           il,ilReactions);


   delete trains;
   delete truckTrain;
   delete truck;

   return retval;
}

bool LRFDBridge::CreateDesignTruckEnvelope(InfluenceLineRecordContainer *il,ReactionInfluenceLineRecordContainer* ilReactions)
{
   bool retval;
   Truck *truck;
   TruckTrain *truckTrain;
   TruckTrainContainer *trains;
   int inc;
   double rearAxleSpacing;

   if (!EnableDesignTruck())
      {
  	   AllocateEnvelopes(dcDesignTruck,"Design Truck");
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDesignTruck,dtMaxMoment)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDesignTruck,dtMinMoment)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDesignTruck,dtMaxShear)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDesignTruck,dtMinShear)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDesignTruck,dtMaxReaction)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDesignTruck,dtMinReaction)),0);
      return true; // get the heck outta here
      }

   PROGRESS_MSG("Processing Design Truck");

   // Setup container of truck trains
   trains = new TruckTrainContainer(2,0,2);
   trains->OwnsElements(false);

   // Setup trucks
   for (inc = 0; inc < numAxleInc; inc++)
      {
		if (numAxleInc == 1)
			rearAxleSpacing = 4.300;
      else
         rearAxleSpacing = (9.000 - 4.300)*inc/(numAxleInc-1) + 4.300;

      truck = new Truck(inc);
      truck->AddAxle(0.000, 35000.);
      truck->AddAxle(4.300,145000.);
      truck->AddAxle(rearAxleSpacing,145000.);

      truckTrain = new TruckTrain(VARTRUCK + inc);
      truckTrain->SetLaneLoad(9300.);
      truckTrain->SetTruckImpact(GetTruckImpact());
      truckTrain->SetLaneImpact(GetLaneImpact());
      truckTrain->AddTruck(*truck,0.);
      trains->Add(truckTrain);

      delete truck;
      }

   retval = CreateEnvelope(dcDesignTruck,
                           "Design Truck",
                           trains,
                           il,ilReactions);


   delete trains;
   delete truckTrain;

   return retval;
}

bool LRFDBridge::CreateDualTruckEnvelope(InfluenceLineRecordContainer *il,ReactionInfluenceLineRecordContainer* ilReactions)
{
   bool retval;
   int spanId;
   double lOffset;
   InfluenceLineRecordContainer *tempInfluenceLineRecords;
   ReactionInfluenceLineRecordContainer* tempReactionInfluenceLineRecords;
   InfluenceLine *flatInflLine;
   Truck *truck;
   TruckTrain *truckTrain;
   TruckTrainContainer *trains;
   int inc;
   int range;
   double headway;

   if (!DualTruckLoadEnabled(DUALTRUCKS))
      {
      AllocateEnvelopes(dcDualTruckTrain,"Dual Truck Train");

      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDualTruckTrain,dtMaxMoment)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDualTruckTrain,dtMinMoment)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDualTruckTrain,dtMaxShear)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDualTruckTrain,dtMinShear)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDualTruckTrain,dtMaxReaction)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDualTruckTrain,dtMinReaction)),0);

      return true;
      }

   PROGRESS_MSG("Processing Dual Truck Train");

	// Find all the influence lines that occur between points of unit uniform
	// load contraflexure
   tempInfluenceLineRecords = new InfluenceLineRecordContainer(10,0,10);

   // Create an influence line that will always evaluate to zero.
   // (i.e. no force effect)
   flatInflLine = new InfluenceLine(-1);
   flatInflLine->AddPoint(new Diagram::Point(mbrRightFace,0,0));
   flatInflLine->AddPoint(new Diagram::Point(mbrLeftFace,Length(),0));

   InfluenceLineRecordIterator iter(*il);
   while(iter)
      {
      InfluenceLineRecord *ilr = iter++;
      spanId = ilr->spanId;
      lOffset = ilr->lOffset;

      bool negLeftFace;
      bool negRightFace;

      negLeftFace = UnitLoadNegMomentRegion(spanId,lOffset,mbrLeftFace);
      negRightFace = UnitLoadNegMomentRegion(spanId,lOffset,mbrRightFace);

      TRACEX(LRFDBridge_Liveload,LEVEL2,"POI "        << ilr->poiId
                                     << " Span "      << spanId
                                     << " Offset "    << lOffset
                                     << " LeftFace "  << (negLeftFace ? "Include" : "Exclude")
                                     << " RightFace " << (negRightFace ? "Include" : "Exclude") );

      InfluenceLine *lfMoment, *rfMoment;
      InfluenceLine *ilShear;

      if (negLeftFace)
         {
         // Left face is in neg moment zone, use actual influence line
         lfMoment = new InfluenceLine(ilr->poiId,*ilr->lfMoment);
         }
		else
			{
         // Left face is in a pos moment zone, use a zero influence line
         lfMoment = new InfluenceLine(ilr->poiId,*flatInflLine);
         }

      if (negRightFace)
         {
         // Right face is in neg moment zone, use actual influence line
         // if not used on left face
         if (!negLeftFace)
            rfMoment = new InfluenceLine(ilr->poiId,*ilr->lfMoment);
         else
            rfMoment = NULL;
         }
      else
         {
         // Right face is in a pos moment zone, use a zero influence line
         // unless, left face uses a zero influence line as well
         if (negLeftFace)
            rfMoment = new InfluenceLine(ilr->poiId,*flatInflLine);
         else
            rfMoment = NULL;
         }

      ilShear = new InfluenceLine(ilr->poiId,*ilr->lfShear);

      InfluenceLineRecord *ilrTemp;
      ilrTemp = new InfluenceLineRecord(ilr->poiId,ilr->spanId,ilr->lOffset,
                                        lfMoment, rfMoment,
													 ilShear,NULL);
		tempInfluenceLineRecords->Add(ilrTemp);
      }
   tempReactionInfluenceLineRecords = new ReactionInfluenceLineRecordContainer(10,0,10);
   Support* s;
   ReactionInfluenceLineRecordIterator riter(*ilReactions);
	while(riter)
   {
   	InfluenceLine *Fx, *Fy, *Mz;
   	ReactionInfluenceLineRecord* rilr = riter++;
      int supportId = rilr->supportId;
      s = GetSupport(supportId);
   	if ( s->LeftSpan() != 0 && s->RightSpan() != 0 )
      {
      	// interior support
         Fx = new InfluenceLine(rilr->supportId,*rilr->ilFx);
         Fy = new InfluenceLine(rilr->supportId,*rilr->ilFy);
         Mz = new InfluenceLine(rilr->supportId,*rilr->ilMz);
      }
      else
      {
         Fx = new InfluenceLine(rilr->supportId,*flatInflLine);
         Fy = new InfluenceLine(rilr->supportId,*flatInflLine);
         Mz = new InfluenceLine(rilr->supportId,*flatInflLine);
      }

   	ReactionInfluenceLineRecord* ilrTemp;
      ilrTemp = new ReactionInfluenceLineRecord(rilr->supportId,Fx,Fy,Mz);
      tempReactionInfluenceLineRecords->Add(ilrTemp);
   }

   // Setup container of truck trains
   trains = new TruckTrainContainer(2,0,2);

   // Setup trucks
   truck = new Truck(1);
   truck->AddAxle(0.000,0.90 * 35000.);
   truck->AddAxle(4.300,0.90 * 145000.);
   truck->AddAxle(4.300,0.90 * 145000.);

   // Setup truck trains
   for (range = 0; range < 3; range++)
      {
      if (!DualTruckRangeEnabled(DUALTRUCKS,range))
         continue;

      for (inc = 0; inc < NumHeadwayInc(DUALTRUCKS,range); inc++)
         {
         if (NumHeadwayInc(DUALTRUCKS,range) == 1)
            headway = FromHeadwaySpacing(DUALTRUCKS,range);
         else
            headway = (ToHeadwaySpacing(DUALTRUCKS,range) - FromHeadwaySpacing(DUALTRUCKS,range))*inc/(NumHeadwayInc(DUALTRUCKS,range)-1) + FromHeadwaySpacing(DUALTRUCKS,range);

         truckTrain = new TruckTrain(DUALTRUCKS);
         truckTrain->SetLaneLoad(0.90 * 9300.);
         truckTrain->SetTruckImpact(GetTruckImpact());
         truckTrain->SetLaneImpact(GetLaneImpact());
			truckTrain->AddTruck(*truck,0.);
			truckTrain->AddTruck(*truck,headway);
         trains->Add(truckTrain);
         }
      }

   retval = CreateEnvelope(dcDualTruckTrain,
                           "Dual Truck Train",
                           trains,
                           tempInfluenceLineRecords,
                           tempReactionInfluenceLineRecords,
                           DualTruckLoadEnabled(DUALTRUCKS),
                           false,false,false,
                           DualTruckLoadEnabled(DUALTRUCKS),
                           DualTruckLoadEnabled(DUALTRUCKS));

   delete trains;
   delete truck;
   delete tempInfluenceLineRecords;
   delete flatInflLine;

   return retval;
}

bool LRFDBridge::CreateDualTandemEnvelope(InfluenceLineRecordContainer *il,ReactionInfluenceLineRecordContainer* ilReactions)
{
   bool retval;
   int spanId;
   double lOffset;
   InfluenceLineRecordContainer *tempInfluenceLineRecords;
   ReactionInfluenceLineRecordContainer* tempReactionInfluenceLineRecords;
   InfluenceLine *flatInflLine;
   Truck *truck;
   TruckTrain *truckTrain;
	TruckTrainContainer *trains;
	int inc;
   int range;
   double headway;

   if (!DualTruckLoadEnabled(DUALTANDEMS))
      {
      AllocateEnvelopes(dcDualTandemTrain,"Dual Tandem Train");

      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDualTandemTrain,dtMaxMoment)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDualTandemTrain,dtMinMoment)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDualTandemTrain,dtMaxShear)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDualTandemTrain,dtMinShear)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDualTandemTrain,dtMaxReaction)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcDualTandemTrain,dtMinReaction)),0);

      return true;
      }

   PROGRESS_MSG("Processing Dual Tandem Train");

   // Find all the influence lines that occur between points of unit uniform
   // load contraflexure
   tempInfluenceLineRecords = new InfluenceLineRecordContainer(10,0,10);

   // Create an influence line that will always evaluate to zero.
   // (i.e. no force effect)
   flatInflLine = new InfluenceLine(-1);
   flatInflLine->AddPoint(new Diagram::Point(mbrRightFace,0,0));
   flatInflLine->AddPoint(new Diagram::Point(mbrLeftFace,Length(),0));

   InfluenceLineRecordIterator iter(*il);
	while(iter)
		{
      InfluenceLineRecord *ilr = iter++;
      spanId = ilr->spanId;
      lOffset = ilr->lOffset;

      bool negLeftFace;
      bool negRightFace;

      negLeftFace  = UnitLoadNegMomentRegion(spanId,lOffset,mbrLeftFace);
      negRightFace = UnitLoadNegMomentRegion(spanId,lOffset,mbrRightFace);

      InfluenceLine *lfMoment, *rfMoment;
      InfluenceLine *ilShear;

      if (negLeftFace)
         {
         // Left face is in neg moment zone, use actual influence line
         lfMoment = new InfluenceLine(ilr->poiId,*ilr->lfMoment);
         }
      else
         {
         // Left face is in a pos moment zone, use a zero influence line
         lfMoment = new InfluenceLine(ilr->poiId,*flatInflLine);
         }

      if (negRightFace)
         {
         // Right face is in neg moment zone, use actual influence line
         // if not used on left face
         if (!negLeftFace)
				rfMoment = new InfluenceLine(ilr->poiId,*ilr->lfMoment);
			else
            rfMoment = NULL;
         }
      else
         {
         // Right face is in a pos moment zone, use a zero influence line
         // unless, left face uses a zero influence line as well
         if (negLeftFace)
            rfMoment = new InfluenceLine(ilr->poiId,*flatInflLine);
         else
            rfMoment = NULL;
         }

      ilShear = new InfluenceLine(ilr->poiId,*ilr->lfShear);

      InfluenceLineRecord *ilrTemp;
      ilrTemp = new InfluenceLineRecord(ilr->poiId,ilr->spanId,ilr->lOffset,
                                        lfMoment, rfMoment,
                                        ilShear,NULL);
      tempInfluenceLineRecords->Add(ilrTemp);
      }

   tempReactionInfluenceLineRecords = new ReactionInfluenceLineRecordContainer(10,0,10);
   Support* s;
   ReactionInfluenceLineRecordIterator riter(*ilReactions);
	while(riter)
   {
   	InfluenceLine *Fx, *Fy, *Mz;
   	ReactionInfluenceLineRecord* rilr = riter++;
      int supportId = rilr->supportId;
      s = GetSupport(supportId);
   	if ( s->LeftSpan() != 0 && s->RightSpan() != 0 )
      {
      	// interior support
         Fx = new InfluenceLine(rilr->supportId,*rilr->ilFx);
         Fy = new InfluenceLine(rilr->supportId,*rilr->ilFy);
         Mz = new InfluenceLine(rilr->supportId,*rilr->ilMz);
      }
      else
      {
         Fx = new InfluenceLine(rilr->supportId,*flatInflLine);
         Fy = new InfluenceLine(rilr->supportId,*flatInflLine);
         Mz = new InfluenceLine(rilr->supportId,*flatInflLine);
      }

   	ReactionInfluenceLineRecord* ilrTemp;
      ilrTemp = new ReactionInfluenceLineRecord(rilr->supportId,Fx,Fy,Mz);
      tempReactionInfluenceLineRecords->Add(ilrTemp);
   }

   // Setup container of truck trains
   trains = new TruckTrainContainer(2,0,2);

   // Setup trucks
   truck = new Truck(1);
   truck->AddAxle(0.000,110000.);
   truck->AddAxle(1.200,110000.);

	// Setup truck trains
   for (range = 0; range < 3; range++)
      {
      if (!DualTruckRangeEnabled(DUALTANDEMS,range))
         continue;

      for (inc = 0; inc < NumHeadwayInc(DUALTANDEMS,range); inc++)
         {
         if (NumHeadwayInc(DUALTANDEMS,range) == 1)
            headway = FromHeadwaySpacing(DUALTANDEMS,range);
         else
            headway = (ToHeadwaySpacing(DUALTANDEMS,range) - FromHeadwaySpacing(DUALTANDEMS,range))*inc/(NumHeadwayInc(DUALTANDEMS,range)-1) + FromHeadwaySpacing(DUALTANDEMS,range);

         truckTrain = new TruckTrain(DUALTANDEMS);
         truckTrain->SetLaneLoad( 9300.);
         truckTrain->SetTruckImpact(GetTruckImpact());
         truckTrain->SetLaneImpact(GetLaneImpact());
         truckTrain->AddTruck(*truck,0.);
         truckTrain->AddTruck(*truck,headway);
         trains->Add(truckTrain);
         }
      }

   retval = CreateEnvelope(dcDualTandemTrain,
                           "Dual Tandem Train",
                           trains,
                           tempInfluenceLineRecords,
									tempReactionInfluenceLineRecords,
                           DualTruckLoadEnabled(DUALTANDEMS),
                           false,false,false,
                           DualTruckLoadEnabled(DUALTANDEMS),
                           DualTruckLoadEnabled(DUALTANDEMS));

	delete trains;
   delete truck;
   delete tempInfluenceLineRecords;
   delete flatInflLine;

   return retval;
}

bool LRFDBridge::CreateLiveLoadEnvelope()
{
   // Envelope of envelopes...
   double lfValue;
   double rfValue;
   double Fx,Fy,Mz;
   double FxMin, FyMin, MzMin;
   double FxMax, FyMax, MzMax;
   double lfMinMoment, rfMinMoment;
   double lfMaxMoment, rfMaxMoment;
   double lfMinShear,  rfMinShear;
   double lfMaxShear,  rfMaxShear;
   AnalysisResults *minMomentEnvAll;
   AnalysisResults *maxMomentEnvAll;
   AnalysisResults *minShearEnvAll;
   AnalysisResults *maxShearEnvAll;
   AnalysisResults *minReactionEnvAll;
   AnalysisResults *maxReactionEnvAll;
   AnalysisResults *minMomentEnv;
   AnalysisResults *maxMomentEnv;
   AnalysisResults *minShearEnv;
   AnalysisResults *maxShearEnv;
   AnalysisResults *minReactionEnv;
   AnalysisResults *maxReactionEnv;
   double gOffset;

   TRACEX(LRFDBridge_Liveload,LEVEL1,"LRFDBridge::CreateLiveLoadEnvelope()");

	PROGRESS_MSG("Creating Live Load Envelope");

   AllocateEnvelopes(dcAll,"All");

   maxMomentEnvAll = GetAnalysisResults(GetAnalysisResultsId(dcAll,dtMaxMoment));
   minMomentEnvAll = GetAnalysisResults(GetAnalysisResultsId(dcAll,dtMinMoment));
   maxShearEnvAll  = GetAnalysisResults(GetAnalysisResultsId(dcAll,dtMaxShear));
   minShearEnvAll  = GetAnalysisResults(GetAnalysisResultsId(dcAll,dtMinShear));
   maxReactionEnvAll = GetAnalysisResults(GetAnalysisResultsId(dcAll,dtMaxReaction));
   minReactionEnvAll = GetAnalysisResults(GetAnalysisResultsId(dcAll,dtMinReaction));

   InitializeEnvelopes(maxMomentEnvAll,minMomentEnvAll,maxShearEnvAll,minShearEnvAll,maxReactionEnvAll,minReactionEnvAll);

   PointOfInterestIterator j(*pntsOfInterest);
   while(j)
      {
      PointOfInterest *poi = j++;

      TRACEX(LRFDBridge_Liveload,LEVEL2,"POI " << poi->id);

      maxMomentEnvAll->GetResult(poi->id,gOffset,lfMaxMoment,rfMaxMoment);
      minMomentEnvAll->GetResult(poi->id,gOffset,lfMinMoment,rfMinMoment);
      maxShearEnvAll->GetResult(poi->id,gOffset,lfMaxShear,rfMaxShear);
      minShearEnvAll->GetResult(poi->id,gOffset,lfMinShear,rfMinShear);

      for (DiagramCatagory dc = dcDesignTandem; dc <= dcDualTandemTrain; dc++)
         {
         maxMomentEnv = GetAnalysisResults(GetAnalysisResultsId(dc,dtMaxMoment));
         minMomentEnv = GetAnalysisResults(GetAnalysisResultsId(dc,dtMinMoment));
         maxShearEnv  = GetAnalysisResults(GetAnalysisResultsId(dc,dtMaxShear));
         minShearEnv  = GetAnalysisResults(GetAnalysisResultsId(dc,dtMinShear));


			minMomentEnv->GetResult(poi->id,gOffset,lfValue,rfValue);
         TRACEX(LRFDBridge_Liveload,LEVEL2,minMomentEnv->GetDescription());
         TRACEX(LRFDBridge_Liveload,LEVEL2,"lfValue = " << lfValue << " rfValue = " << rfValue);
         if (lfValue < lfMinMoment)
            lfMinMoment = lfValue;
         if (rfValue < rfMinMoment)
            rfMinMoment = rfValue;

         maxMomentEnv->GetResult(poi->id,gOffset,lfValue,rfValue);
         TRACEX(LRFDBridge_Liveload,LEVEL2,maxMomentEnv->GetDescription());
         TRACEX(LRFDBridge_Liveload,LEVEL2,"lfValue = " << lfValue << " rfValue = " << rfValue);
         if (lfValue > lfMaxMoment)
            lfMaxMoment = lfValue;
         if (rfValue > rfMaxMoment)
            rfMaxMoment = rfValue;

         minShearEnv->GetResult(poi->id,gOffset,lfValue,rfValue);
         TRACEX(LRFDBridge_Liveload,LEVEL2,minShearEnv->GetDescription());
         TRACEX(LRFDBridge_Liveload,LEVEL2,"lfValue = " << lfValue << " rfValue = " << rfValue);
         if (lfValue < lfMinShear)
            lfMinShear = lfValue;
         if (rfValue < rfMinShear)
            rfMinShear = rfValue;

         maxShearEnv->GetResult(poi->id,gOffset,lfValue,rfValue);
         TRACEX(LRFDBridge_Liveload,LEVEL2,maxShearEnv->GetDescription());
         TRACEX(LRFDBridge_Liveload,LEVEL2,"lfValue = " << lfValue << " rfValue = " << rfValue);
         if (lfValue > lfMaxShear)
            lfMaxShear = lfValue;
			if (rfValue > rfMaxShear)
				rfMaxShear = rfValue;

         maxMomentEnvAll->AddResult(poi->id,gOffset,lfMaxMoment,rfMaxMoment);
         minMomentEnvAll->AddResult(poi->id,gOffset,lfMinMoment,rfMinMoment);
         maxShearEnvAll->AddResult(poi->id,gOffset,lfMaxShear,rfMaxShear);
         minShearEnvAll->AddResult(poi->id,gOffset,lfMinShear,rfMinShear);

         SHOULD_CONTINUE;
         }
      }

   Support* s;
   for ( s = firstSupport; s != NULL; s = s->NextSupport() )
   {
   	int pierId = s->GetID();
   	maxReactionEnvAll->GetSupportResult(pierId,FxMax,FyMax,MzMax);
   	minReactionEnvAll->GetSupportResult(pierId,FxMin,FyMin,MzMin);

      for (DiagramCatagory dc = dcDesignTandem; dc <= dcDualTandemTrain; dc++)
         {
         maxReactionEnv = GetAnalysisResults(GetAnalysisResultsId(dc,dtMaxReaction));
         minReactionEnv = GetAnalysisResults(GetAnalysisResultsId(dc,dtMinReaction));

         minReactionEnv->GetSupportResult(pierId,Fx,Fy,Mz);
         if ( Fx < FxMin )
         	FxMin = Fx;
         if ( Fy < FyMin )
         	FyMin = Fy;
         if ( Mz < MzMin )
         	MzMin = Mz;

         maxReactionEnv->GetSupportResult(pierId,Fx,Fy,Mz);
         if ( FxMax < Fx )
         	FxMax = Fx;
         if ( FyMax < Fy )
         	FyMax = Fy;
         if ( MzMax < Mz )
         	MzMax = Mz;

         maxReactionEnvAll->AddSupportResult(pierId,FxMax,FyMax,MzMax);
         minReactionEnvAll->AddSupportResult(pierId,FxMin,FyMin,MzMin);

         SHOULD_CONTINUE;
         }
   }

   return true;
}

bool LRFDBridge::CreateFatigueTruckEnvelope(InfluenceLineRecordContainer *il,ReactionInfluenceLineRecordContainer* ilReactions)
{
   bool retval;

   Truck *truck;
   TruckTrain *truckTrain;
   TruckTrainContainer *trains;

   if (!EnableFatigueTruck())
      {
  	   AllocateEnvelopes(dcFatigueTruck,"Fatigue Truck");
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcFatigueTruck,dtMaxMoment)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcFatigueTruck,dtMinMoment)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcFatigueTruck,dtMaxShear)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcFatigueTruck,dtMinShear)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcFatigueTruck,dtMaxReaction)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcFatigueTruck,dtMinReaction)),0);
      return true; // get the heck outta here
      }

   PROGRESS_MSG("Processing Fatigue Truck");

   // Setup truck train
   truckTrain = new TruckTrain(0);
   truckTrain->SetLaneLoad(0.);
   truckTrain->SetTruckImpact(GetFatigueImpact());
   truckTrain->SetLaneImpact(GetLaneImpact());

	// Setup truck
   // units: m, N
   truck = new Truck(1);
   truck->AddAxle(0.000,35000.);
   truck->AddAxle(4.300,145000.);
   truck->AddAxle(9.000,145000.);

   // Add truck to truck train
   truckTrain->AddTruck(*truck,0.);

   // Setup container of truck trains
   trains = new TruckTrainContainer(2,0,2);
   trains->OwnsElements(false);
   trains->Add(truckTrain);

   retval = CreateEnvelope(dcFatigueTruck,
                           "Fatigue Truck",
                           trains,
                           il,ilReactions);

   delete trains;
   delete truckTrain;
   delete truck;

   return retval;
}

bool LRFDBridge::CreatePedestrianEnvelope(InfluenceLineRecordContainer *il,ReactionInfluenceLineRecordContainer* ilReactions)
{
	bool retval;
	TruckTrain *truckTrain;
   TruckTrainContainer *trains;
   Truck *truck;

   // If the pedestrian load is zero, just fill in the envelopes with zero
   // No need for analysis
   if (zero_val(pedestrianLd))
      {
  	   AllocateEnvelopes(dcPedestrian,"Pedestrian");
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcPedestrian,dtMaxMoment)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcPedestrian,dtMinMoment)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcPedestrian,dtMaxShear)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcPedestrian,dtMinShear)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcPedestrian,dtMaxReaction)),0);
      InitializeEnvelope(GetAnalysisResults(GetAnalysisResultsId(dcPedestrian,dtMinReaction)),0);
      return true; // get the heck outta here
      }

   PROGRESS_MSG("Processing Pedestrian Load");

   // Setup truck train
   truck = new Truck(1);
   truck->AddAxle(0.000,0.000); // dummy truck (one axle, wgt = 0.000)
   truckTrain = new TruckTrain(0);
   truckTrain->AddTruck(*truck,0.);
   truckTrain->SetLaneLoad(pedestrianLd);
   truckTrain->SetTruckImpact(0.);
   truckTrain->SetLaneImpact(0.);

   // Setup container of truck trains
   trains = new TruckTrainContainer(2,0,2);
   trains->OwnsElements(false);
   trains->Add(truckTrain);

   retval = CreateEnvelope(dcPedestrian,
                           "Pedestrian",
                           trains,
                           il,ilReactions);

   delete trains;
   delete truckTrain;
   delete truck;

	return retval;
}

bool LRFDBridge::CreateEnvelope(DiagramCatagory dc,
                                const char *desc,
                                TruckTrainContainer *trains,
                                InfluenceLineRecordContainer *il,
			                       ReactionInfluenceLineRecordContainer* ilReactions,
                                bool envMinMoment,
                                bool envMaxMoment,
                                bool envMinShear,
                                bool envMaxShear,
                                bool envMinReaction,
                                bool envMaxReaction)
{
   TruckTrain *train;
   Truck *truck;
   int pivotTruck;
   int pivotAxle;
   int numTrucks;
   int numAxles;
   double lfValue;
   double rfValue;
   double gOffset;
   double Fx,Fy,Mz; // reactions
   AnalysisResults *minMomentEnv;
   AnalysisResults *maxMomentEnv;
   AnalysisResults *minShearEnv;
   AnalysisResults *maxShearEnv;
   AnalysisResults *minReactionEnv;
   AnalysisResults *maxReactionEnv;

   TRACEX(LRFDBridge_Liveload,LEVEL1,"LRFDBridge::CreateEnvelope()");

   #if defined GEN_DEBUG_CODE
   if (logTruck)
		{
		truckLog << endl;
		truckLog << "***************************************************" << endl;
		truckLog << "Creating envelope for " << desc << endl;
		truckLog << "***************************************************" << endl;
		truckLog << endl;
		}
	#endif

	// Prepare envelopes...
	AllocateEnvelopes(dc,desc);

	maxMomentEnv = GetAnalysisResults(GetAnalysisResultsId(dc,dtMaxMoment));
	minMomentEnv = GetAnalysisResults(GetAnalysisResultsId(dc,dtMinMoment));
	maxShearEnv  = GetAnalysisResults(GetAnalysisResultsId(dc,dtMaxShear));
	minShearEnv  = GetAnalysisResults(GetAnalysisResultsId(dc,dtMinShear));
   maxReactionEnv = GetAnalysisResults(GetAnalysisResultsId(dc,dtMaxReaction));
   minReactionEnv = GetAnalysisResults(GetAnalysisResultsId(dc,dtMinReaction));

	InitializeEnvelopes(maxMomentEnv,minMomentEnv,maxShearEnv,minShearEnv,maxReactionEnv,minReactionEnv);

	// Do the enveloping...
	TruckTrainIterator iter(*trains);

	while(iter)
		{
		train = iter++;
		numTrucks = train->GetNumTrucks();

		#if defined GEN_DEBUG_CODE
		if (logTruck)
			truckLog << "Truck Train " << train->GetID() << endl;
		#endif

		InfluenceLineRecordIterator i(*il);
		while(i)
			{
			InfluenceLineRecord *ilr = i++;

			#if defined GEN_DEBUG_CODE
			if (logTruck)
				{
				PointOfInterest *POI = GetPointOfInterest(ilr->poiId);
            thisPoint = (TRACE_POI == POI->id);
				if (thisPoint)
               {
					truckLog << "===> Computing forces at span " << POI->spanId << " offset " << POI->offset << endl;

               switch(traceType)
                  {
                  case TRACE_MAXMOMENT:
                  case TRACE_MINMOMENT:
                       if (ilr->lfMoment)
                          truckLog << "Left Face Influence Line" << endl
                                   << (*ilr->lfMoment) << endl;
                       if (ilr->rfMoment)
                          truckLog << "Right Face Influence Line" << endl
                                   << (*ilr->rfMoment) << endl;
                       break;

                  case TRACE_MAXSHEAR:
                  case TRACE_MINSHEAR:
                       if (ilr->lfShear)
                          truckLog << "Left Face Influence Line" << endl
                                   << (*ilr->lfShear) << endl;
                       if (ilr->rfShear)
                          truckLog << "Right Face Influence Line" << endl
                                   << (*ilr->rfShear) << endl;
                       break;
                  }
               }
				}
			#endif

			CHECKX(numTrucks > 0,"LRFDBridge::CreateLiveloadEnvelopeTable - NumTrucks must be > 0");
			for (pivotTruck = 0; pivotTruck < numTrucks; pivotTruck++)
				{
				truck = train->GetTruck(pivotTruck);
				numAxles = truck->GetNumAxles();

				for (pivotAxle = 0; pivotAxle < numAxles; pivotAxle++)
					{
					#if defined GEN_DEBUG_CODE
					if (logTruck && thisPoint)
						truckLog << "---> Pivot Axle set to Truck " << pivotTruck << " Axle " << pivotAxle << endl;
					#endif

					train->SetPivot(pivotTruck,pivotAxle);

					// Place pivot axle at every point of interest
					PointOfInterestIterator j(*pntsOfInterest);
					while(j)
						{
						PointOfInterest *poi = j++;

						#if defined GEN_DEBUG_CODE
						if (logTruck && thisPoint)
							truckLog << ">>> Pivot axle placed at span " << poi->spanId << " offset " << poi->offset << endl;
						#endif

						train->MoveTo(GetSuperstructureOffset(poi->spanId,poi->offset));

						#if defined GEN_DEBUG_CODE
						if (logTruck && thisPoint && traceType == TRACE_MAXMOMENT)
							truckLog << "Maximum Moment" << endl;
						else
							{
							traceState = logTruck;
							logTruck = false;
							}
						#endif

						if (envMaxMoment)
							{
							// inflLineData corrosponds with a poi
							maxMomentEnv->GetResult(ilr->poiId,gOffset,lfValue,rfValue);
							EnvelopeMaxPoint(train,ilr->lfMoment,ilr->rfMoment,lfValue,rfValue);
							maxMomentEnv->AddResult(ilr->poiId,gOffset,lfValue,rfValue);
							}
						else
							maxMomentEnv->AddResult(ilr->poiId,GetSuperstructureOffset(ilr->spanId,ilr->lOffset),0,0);

						#if defined GEN_DEBUG_CODE
						logTruck = traceState;
						#endif

						#if defined GEN_DEBUG_CODE
						if (logTruck && thisPoint && traceType == TRACE_MINMOMENT)
							truckLog << "Minimum Moment" << endl;
						else
							{
							traceState = logTruck;
							logTruck = false;
							}
						#endif

						if (envMinMoment)
							{
							minMomentEnv->GetResult(ilr->poiId,gOffset,lfValue,rfValue);
							EnvelopeMinPoint(train,ilr->lfMoment,ilr->rfMoment,lfValue,rfValue);
							minMomentEnv->AddResult(ilr->poiId,gOffset,lfValue,rfValue);
							}
						else
							minMomentEnv->AddResult(ilr->poiId,GetSuperstructureOffset(ilr->spanId,ilr->lOffset),0,0);

						#if defined GEN_DEBUG_CODE
						logTruck = traceState;
						#endif

						#if defined GEN_DEBUG_CODE
						if (logTruck && thisPoint && traceType == TRACE_MAXSHEAR)
							truckLog << "Maximum Shear" << endl;
						else
							{
							traceState = logTruck;
							logTruck = false;
							}
						#endif

						if (envMaxShear)
							{
							maxShearEnv->GetResult(ilr->poiId,gOffset,lfValue,rfValue);
							EnvelopeMaxPoint(train,ilr->lfShear,ilr->rfShear,lfValue,rfValue);
							maxShearEnv->AddResult(ilr->poiId,gOffset,lfValue,rfValue);
							}
						else
							maxShearEnv->AddResult(ilr->poiId,GetSuperstructureOffset(ilr->spanId,ilr->lOffset),0,0);

						#if defined GEN_DEBUG_CODE
						logTruck = traceState;
						#endif

						#if defined GEN_DEBUG_CODE
						if (logTruck && thisPoint && traceType == TRACE_MINSHEAR)
							truckLog << "Minimum Shear" << endl;
						else
							{
							traceState = logTruck;
							logTruck = false;
							}
						#endif

						if (envMinShear)
							{
							minShearEnv->GetResult(ilr->poiId,gOffset,lfValue,rfValue);
							EnvelopeMinPoint(train,ilr->lfShear,ilr->rfShear,lfValue,rfValue);
							minShearEnv->AddResult(ilr->poiId,gOffset,lfValue,rfValue);
							}
						else
							minShearEnv->AddResult(ilr->poiId,GetSuperstructureOffset(ilr->spanId,ilr->lOffset),0,0);

						#if defined GEN_DEBUG_CODE
						logTruck = traceState;
						#endif

						SHOULD_CONTINUE;
						} // end of poi iteration
					} // end of axle loop
				} // end of truck loop
			} // end of influence line iteration

      ///////////////////////////////////////////////
      // Compute reaction envelopes
      ///////////////////////////////////////////////
		ReactionInfluenceLineRecordIterator riter(*ilReactions);
		while(riter)
			{
			ReactionInfluenceLineRecord *ilr = riter++;

			CHECKX(numTrucks > 0,"LRFDBridge::CreateLiveloadEnvelopeTable - NumTrucks must be > 0");
			for (pivotTruck = 0; pivotTruck < numTrucks; pivotTruck++)
				{
				truck = train->GetTruck(pivotTruck);
				numAxles = truck->GetNumAxles();

				for (pivotAxle = 0; pivotAxle < numAxles; pivotAxle++)
					{
					train->SetPivot(pivotTruck,pivotAxle);

					// Place pivot axle at every point of interest
               PointOfInterestIterator j(*pntsOfInterest);
               while (j)
						{
                  PointOfInterest* poi = j++;

						train->MoveTo(GetSuperstructureOffset(poi->spanId,poi->offset));

						if (envMaxReaction)
							{
							// inflLineData corrosponds with a poi
							maxReactionEnv->GetSupportResult(ilr->supportId,Fx,Fy,Mz);
							EnvelopeMaxPoint(train,ilr->ilFx,ilr->ilFy,ilr->ilMz,Fx,Fy,Mz);
							maxReactionEnv->AddSupportResult(ilr->supportId,Fx,Fy,Mz);
							}
						else
							maxReactionEnv->AddSupportResult(ilr->supportId,0,0,0);

						if (envMinReaction)
							{
							minReactionEnv->GetSupportResult(ilr->supportId,Fx,Fy,Mz);
							EnvelopeMinPoint(train,ilr->ilFx,ilr->ilFy,ilr->ilMz,Fx,Fy,Mz);
							minReactionEnv->AddSupportResult(ilr->supportId,Fx,Fy,Mz);
							}
						else
							minReactionEnv->AddSupportResult(ilr->supportId,0,0,0);

						SHOULD_CONTINUE;
						} // end of poi iteration

					} // end of axle loop
				} // end of truck loop
			} // end of reaction influence line iteration

		} // end of truck train iteration

	return true;
}

//
//
// "Helper" Functions --------------------------------------------------------
//
//

void LRFDBridge::EnvelopeMaxPoint(TruckTrain *train,InfluenceLine *lfInflLine,InfluenceLine *rfInflLine,
                                  double &lfValue,double &rfValue)
{
   double lfTestValue, rfTestValue;
   TruckTrain::Direction dir;


   if (lfInflLine)
      lfInflLine->SetMode(InfluenceLine::ExcludeNegValues);

   if (rfInflLine)
      rfInflLine->SetMode(InfluenceLine::ExcludeNegValues);

   for (dir = TruckTrain::Forward; dir <= TruckTrain::Backward; dir++)
      {
      #if defined GEN_DEBUG_CODE
      if (logTruck && thisPoint)
         truckLog << "Setting direction to " << ((dir == TruckTrain::Forward) ? "Forward" : "Backward") << endl;
      #endif

      train->SetDirection(dir);
      if (lfInflLine)
         {
         lfTestValue = train->Evaluate(*lfInflLine,mbrLeftFace);

         if (!rfInflLine)
            rfTestValue = train->Evaluate(*lfInflLine,mbrRightFace);
         }

      if (rfInflLine)
         {
         rfTestValue = train->Evaluate(*rfInflLine,mbrRightFace);

         if (!lfInflLine)
            lfTestValue = train->Evaluate(*rfInflLine,mbrLeftFace);
         }

      if (lfTestValue > lfValue)
         lfValue = lfTestValue;

   	if (rfTestValue > rfValue)
         rfValue = rfTestValue;

      // If there is only one influence line, then the same extreme value is
      // used for the left and right faces
      if ( (lfInflLine && !rfInflLine) || (!lfInflLine && rfInflLine) )
         {
         if (lfValue > rfValue)
            rfValue = lfValue;

         if (rfValue > lfValue)
           lfValue = rfValue;
          }
      }
/*
   // Forward
   #if defined GEN_DEBUG_CODE
   if (logTruck && thisPoint)
      truckLog << "Set direction to Forward" << endl;
   #endif
   train->SetDirection(TruckTrain::Forward);
   if (lfInflLine)
      {
      lfTestValue = train->Evaluate(*lfInflLine,mbrLeftFace);

      if (!rfInflLine)
         rfTestValue = train->Evaluate(*lfInflLine,mbrRightFace);
      }

   if (rfInflLine)
      {
      rfTestValue = train->Evaluate(*rfInflLine,mbrRightFace);

      if (!lfInflLine)
         lfTestValue = train->Evaluate(*rfInflLine,mbrLeftFace);
      }

   if (lfTestValue > lfValue)
      lfValue = lfTestValue;

	if (rfTestValue > rfValue)
      rfValue = rfTestValue;

   // If there is only one influence line, then the same extreme value is
   // used for the left and right faces
   if ( (lfInflLine && !rfInflLine) || (!lfInflLine && rfInflLine) )
      {
      if (lfValue > rfValue)
         rfValue = lfValue;

      if (rfValue > lfValue)
         lfValue = rfValue;
      }

   // Backward
   #if defined GEN_DEBUG_CODE
   if (logTruck && thisPoint)
      truckLog << "Set direction to Backward" << endl;
   #endif
   train->SetDirection(TruckTrain::Backward);
   if (lfInflLine)
      {
      lfTestValue = train->Evaluate(*lfInflLine,mbrLeftFace);

      if (!rfInflLine)
         rfTestValue = train->Evaluate(*lfInflLine,mbrRightFace);
      }

   if (rfInflLine)
      {
      rfTestValue = train->Evaluate(*rfInflLine,mbrRightFace);

      if (!lfInflLine)
         lfTestValue = train->Evaluate(*rfInflLine,mbrLeftFace);
      }

   if (lfTestValue > lfValue)
      lfValue = lfTestValue;

   if (rfTestValue > rfValue)
      rfValue = rfTestValue;
*/
}

void LRFDBridge::EnvelopeMinPoint(TruckTrain *train,InfluenceLine *lfInflLine,InfluenceLine *rfInflLine,
                                  double &lfValue,double &rfValue)
{
   double lfTestValue, rfTestValue;
   TruckTrain::Direction dir;


   if (lfInflLine)
      lfInflLine->SetMode(InfluenceLine::ExcludePosValues);

   if (rfInflLine)
      rfInflLine->SetMode(InfluenceLine::ExcludePosValues);

   for (dir = TruckTrain::Forward; dir <= TruckTrain::Backward; dir++)
      {
      #if defined GEN_DEBUG_CODE
      if (logTruck && thisPoint)
         truckLog << "Setting direction to " << ((dir == TruckTrain::Forward) ? "Forward" : "Backward") << endl;
      #endif

      train->SetDirection(dir);
      if (lfInflLine)
         {
         lfTestValue = train->Evaluate(*lfInflLine,mbrLeftFace);

         if (!rfInflLine)
            rfTestValue = train->Evaluate(*lfInflLine,mbrRightFace);
         }

      if (rfInflLine)
         {
         rfTestValue = train->Evaluate(*rfInflLine,mbrRightFace);

         if (!lfInflLine)
            lfTestValue = train->Evaluate(*rfInflLine,mbrLeftFace);
         }

      if (lfTestValue < lfValue)
         lfValue = lfTestValue;

   	if (rfTestValue < rfValue)
         rfValue = rfTestValue;

      // If there is only one influence line, then the same extreme value is
      // used for the left and right faces
      if ( (lfInflLine && !rfInflLine) || (!lfInflLine && rfInflLine) )
         {
         if (lfValue < rfValue)
            rfValue = lfValue;

         if (rfValue < lfValue)
           lfValue = rfValue;
          }
      }
/*
	double lfTestValue, rfTestValue;

	if (lfInflLine)
		lfInflLine->SetMode(InfluenceLine::ExcludePosValues);

	if (rfInflLine)
		rfInflLine->SetMode(InfluenceLine::ExcludePosValues);

	// Forward
   #if defined GEN_DEBUG_CODE
   if (logTruck && thisPoint)
      truckLog << "Set direction to Forward" << endl;
   #endif
	train->SetDirection(TruckTrain::Forward);
	if (lfInflLine)
		{
		lfTestValue = train->Evaluate(*lfInflLine,mbrLeftFace);

		if (!rfInflLine)
			rfTestValue = train->Evaluate(*lfInflLine,mbrRightFace);
		}

	if (rfInflLine)
		{
		rfTestValue = train->Evaluate(*rfInflLine,mbrRightFace);

		if (!lfInflLine)
			lfTestValue = train->Evaluate(*rfInflLine,mbrLeftFace);
		}

	if (lfTestValue < lfValue)
		lfValue = lfTestValue;

	if (rfTestValue < rfValue)
		rfValue = rfTestValue;

	// Backward
   #if defined GEN_DEBUG_CODE
   if (logTruck && thisPoint)
      truckLog << "Set direction to Backward" << endl;
   #endif
	train->SetDirection(TruckTrain::Backward);
	if (lfInflLine)
		{
		lfTestValue = train->Evaluate(*lfInflLine,mbrLeftFace);

      if (!rfInflLine)
         rfTestValue = train->Evaluate(*lfInflLine,mbrRightFace);
      }

   if (rfInflLine)
      {
      rfTestValue = train->Evaluate(*rfInflLine,mbrRightFace);

      if (!lfInflLine)
         lfTestValue = train->Evaluate(*rfInflLine,mbrLeftFace);
      }

   if (lfTestValue < lfValue)
      lfValue = lfTestValue;

   if (rfTestValue < rfValue)
      rfValue = rfTestValue;
*/
}

void LRFDBridge::EnvelopeMaxPoint(TruckTrain *train,InfluenceLine *ilFx,InfluenceLine *ilFy,InfluenceLine *ilMz,
                                  double &Fx,double &Fy,double &Mz)
{
	double FxTest, FyTest, MzTest;
   TruckTrain::Direction dir;

   if (ilFx) ilFx->SetMode(InfluenceLine::ExcludeNegValues);
   if (ilFy) ilFy->SetMode(InfluenceLine::ExcludeNegValues);
   if (ilMz) ilMz->SetMode(InfluenceLine::ExcludeNegValues);

   for (dir = TruckTrain::Forward; dir <= TruckTrain::Backward; dir++)
   {
      train->SetDirection(dir);
      if (ilFx)
      {
        FxTest = train->Evaluate(*ilFx,mbrLeftFace);
        if ( Fx < FxTest )
        	Fx = FxTest;
      }

      if (ilFy)
      {
        FyTest = train->Evaluate(*ilFy,mbrLeftFace);
        if ( Fy < FyTest )
        	Fy = FyTest;
      }

      if (ilMz)
      {
        MzTest = train->Evaluate(*ilMz,mbrLeftFace);
        if ( Mz < MzTest )
        	Mz = MzTest;
      }
   }
}

void LRFDBridge::EnvelopeMinPoint(TruckTrain *train,InfluenceLine *ilFx,InfluenceLine *ilFy,InfluenceLine *ilMz,
                                  double &Fx,double &Fy,double &Mz)
{
	double FxTest, FyTest, MzTest;
   TruckTrain::Direction dir;

   if (ilFx) ilFx->SetMode(InfluenceLine::ExcludePosValues);
   if (ilFy) ilFy->SetMode(InfluenceLine::ExcludePosValues);
   if (ilMz) ilMz->SetMode(InfluenceLine::ExcludePosValues);

   for (dir = TruckTrain::Forward; dir <= TruckTrain::Backward; dir++)
   {
      train->SetDirection(dir);
      if (ilFx)
      {
        FxTest = train->Evaluate(*ilFx,mbrLeftFace);
        if ( FxTest < Fx )
        	Fx = FxTest;
      }

      if (ilFy)
      {
        FyTest = train->Evaluate(*ilFy,mbrLeftFace);
        if ( FyTest < Fy )
        	Fy = FyTest;
      }

      if (ilMz)
      {
        MzTest = train->Evaluate(*ilMz,mbrLeftFace);
        if ( MzTest < Mz )
        	Mz = MzTest;
      }
   }
}

void LRFDBridge::AllocateEnvelopes(DiagramCatagory dc,const char *desc)
{
	int id;
   AnalysisResults *minMomentEnv;
   AnalysisResults *maxMomentEnv;
   AnalysisResults *minShearEnv;
   AnalysisResults *maxShearEnv;
   AnalysisResults *minReactionEnv;
   AnalysisResults *maxReactionEnv;

   string descMinMoment = string(desc) + " - Min Moment";
   string descMaxMoment = string(desc) + " - Max Moment";
   string descMinShear  = string(desc) + " - Min Shear";
   string descMaxShear  = string(desc) + " - Max Shear";
   string descMinReaction = string(desc) + " - Min Reaction";
   string descMaxReaction = string(desc) + " - Max Reaction";

   // Setup envelopes
   id = NextAvailAnalysisResults();
   maxMomentEnv = new AnalysisResults(id,descMaxMoment.c_str());
   SetAnalysisResultsId(dc,dtMaxMoment,id);
   AddAnalysisResults(maxMomentEnv);

   id = NextAvailAnalysisResults();
   minMomentEnv = new AnalysisResults(id,descMinMoment.c_str());
   SetAnalysisResultsId(dc,dtMinMoment,id);
   AddAnalysisResults(minMomentEnv);

   id = NextAvailAnalysisResults();
   maxShearEnv = new AnalysisResults(id,descMaxShear.c_str());
   SetAnalysisResultsId(dc,dtMaxShear,id);
   AddAnalysisResults(maxShearEnv);

   id = NextAvailAnalysisResults();
   minShearEnv = new AnalysisResults(id,descMinShear.c_str());
   SetAnalysisResultsId(dc,dtMinShear,id);
   AddAnalysisResults(minShearEnv);

   id = NextAvailAnalysisResults();
   maxReactionEnv = new AnalysisResults(id,descMaxReaction.c_str());
   SetAnalysisResultsId(dc,dtMaxReaction,id);
   AddAnalysisResults(maxReactionEnv);

   id = NextAvailAnalysisResults();
   minReactionEnv = new AnalysisResults(id,descMinReaction.c_str());
   SetAnalysisResultsId(dc,dtMinReaction,id);
   AddAnalysisResults(minReactionEnv);
}


void LRFDBridge::InitializeEnvelopes(AnalysisResults *maxMomentEnv,
                                     AnalysisResults *minMomentEnv,
                                     AnalysisResults *maxShearEnv,
                                     AnalysisResults *minShearEnv,
                                     AnalysisResults *maxReactionEnv,
                                     AnalysisResults *minReactionEnv)
{
   double maxValue = -DBL_MAX/2;
   double minValue =  DBL_MAX/2;

   // Clear all result points

   if (maxMomentEnv)
      InitializeEnvelope(maxMomentEnv,maxValue);

   if (minMomentEnv)
      InitializeEnvelope(minMomentEnv,minValue);

   if (maxShearEnv)
      InitializeEnvelope(maxShearEnv,maxValue);

   if (minShearEnv)
      InitializeEnvelope(minShearEnv,minValue);

   if (maxReactionEnv)
   	InitializeEnvelope(maxReactionEnv,maxValue);

   if (minReactionEnv)
   	InitializeEnvelope(minReactionEnv,minValue);
}

void LRFDBridge::InitializeEnvelope(AnalysisResults *envelope,double value)
{
   double offset;

   PRECONDITIONX(envelope,"LRFDBridge::InitializeEnvelope() - envelope is NULL");

   envelope->Clear();

   PointOfInterestIterator j(*pntsOfInterest);
   while(j)
      {
      PointOfInterest *poi = j++;

      offset = GetSuperstructureOffset(poi->spanId,poi->offset);
      envelope->AddResult(poi->id,offset,value,value);
      }

   Support* s;
   for ( s = firstSupport; s != NULL; s = s->NextSupport() )
   {
   	int pierId = s->GetID();
   	envelope->AddSupportResult(pierId,value,value,value);
   }

}


