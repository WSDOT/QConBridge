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
#include <fem2d\femmodel.h>
#include <float.h>

#if defined GEN_DEBUG_CODE
#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>
#endif

#define __GetSign(a,b) {if ((a) > 0) b = 1; else if ((a) < 0) b = -1; else b = 0;}

DIAG_DECLARE_GROUP(LRFDBridge);
DIAG_DECLARE_GROUP(Bridge_PntsOfInterest);
DIAG_DEFINE_GROUP(LRFDBridge_InfluenceLines,DISABLED,LEVEL1);
DIAG_DEFINE_GROUP(LRFDBridge_Contraflexure, ENABLED,LEVEL2);

bool LRFDBridge::Analyze(ProgressIndicator *pi)
{
   // Clean up before beginning
   diagramIds->Flush();
   analysisResultsIds->Flush();
   inflLoadRecords->Flush();
   inflResultsRecords->Flush();
   ulMomentZones->Flush();

   if (!Bridge::Analyze(pi))
      return false;

   SHOULD_CONTINUE;

   //
   // Dead Loads
   //
   PROGRESS_MSG("Processing Dead Loads");
   CombineDeadLoads();
   StoreAnalysisResults(dcDC,"DC Dead Load",dcId);
   StoreAnalysisResults(dcDW,"DW Dead Load",dwId);
   SHOULD_CONTINUE;

   //
   // Influence Lines
   //
   if (!CreateInfluenceLines())
      return false;

   // After the influence lines have been created and the dead load
   // results extracted, the fem model is no longer needed. Clear the
   // model to free memory
#  if defined GEN_DEBUG_CODE
   femModel->GTStrudl("model.gtb");
#  endif

   femModel->Clear();
   delete femModel;
   femModel = NULL;

   PROGRESS_MSG("Generating Live Load Envelopes");
   if (!GenerateLiveloadEnvelopes())
      return false;

   PROGRESS_MSG("Creating Load Combinations");
   if (!CreateLoadCombinations())
      return false;

   //
   // Diagrams
   //
   StoreAnalysisResultsAsDiagrams(dcDC,dtShear);
   StoreAnalysisResultsAsDiagrams(dcDC,dtMoment);
   StoreAnalysisResultsAsDiagrams(dcDW,dtShear);
   StoreAnalysisResultsAsDiagrams(dcDW,dtMoment);
   StoreAnalysisResultsAsDiagrams(dcAll,dtMinShear);
   StoreAnalysisResultsAsDiagrams(dcAll,dtMaxShear);
   StoreAnalysisResultsAsDiagrams(dcAll,dtMinMoment);
   StoreAnalysisResultsAsDiagrams(dcAll,dtMaxMoment);
   StoreAnalysisResultsAsDiagrams(dcDesignTandem,dtMinShear);
   StoreAnalysisResultsAsDiagrams(dcDesignTandem,dtMaxShear);
   StoreAnalysisResultsAsDiagrams(dcDesignTandem,dtMinMoment);
   StoreAnalysisResultsAsDiagrams(dcDesignTandem,dtMaxMoment);
   StoreAnalysisResultsAsDiagrams(dcDesignTruck,dtMinShear);
   StoreAnalysisResultsAsDiagrams(dcDesignTruck,dtMaxShear);
   StoreAnalysisResultsAsDiagrams(dcDesignTruck,dtMinMoment);
   StoreAnalysisResultsAsDiagrams(dcDesignTruck,dtMaxMoment);
   StoreAnalysisResultsAsDiagrams(dcDualTruckTrain,dtMinShear);
   StoreAnalysisResultsAsDiagrams(dcDualTruckTrain,dtMaxShear);
   StoreAnalysisResultsAsDiagrams(dcDualTruckTrain,dtMinMoment);
   StoreAnalysisResultsAsDiagrams(dcDualTruckTrain,dtMaxMoment);
   StoreAnalysisResultsAsDiagrams(dcDualTandemTrain,dtMinShear);
   StoreAnalysisResultsAsDiagrams(dcDualTandemTrain,dtMaxShear);
   StoreAnalysisResultsAsDiagrams(dcDualTandemTrain,dtMinMoment);
   StoreAnalysisResultsAsDiagrams(dcDualTandemTrain,dtMaxMoment);
   StoreAnalysisResultsAsDiagrams(dcFatigueTruck,dtMinShear);
   StoreAnalysisResultsAsDiagrams(dcFatigueTruck,dtMaxShear);
   StoreAnalysisResultsAsDiagrams(dcFatigueTruck,dtMinMoment);
   StoreAnalysisResultsAsDiagrams(dcFatigueTruck,dtMaxMoment);
   StoreAnalysisResultsAsDiagrams(dcPedestrian,dtMinShear);
   StoreAnalysisResultsAsDiagrams(dcPedestrian,dtMaxShear);
   StoreAnalysisResultsAsDiagrams(dcPedestrian,dtMinMoment);
   StoreAnalysisResultsAsDiagrams(dcPedestrian,dtMaxMoment);
   StoreAnalysisResultsAsDiagrams(dcStrengthI,dtMinShear);
   StoreAnalysisResultsAsDiagrams(dcStrengthI,dtMaxShear);
   StoreAnalysisResultsAsDiagrams(dcStrengthI,dtMinMoment);
   StoreAnalysisResultsAsDiagrams(dcStrengthI,dtMaxMoment);
   StoreAnalysisResultsAsDiagrams(dcServiceI,dtMinShear);
   StoreAnalysisResultsAsDiagrams(dcServiceI,dtMaxShear);
   StoreAnalysisResultsAsDiagrams(dcServiceI,dtMinMoment);
   StoreAnalysisResultsAsDiagrams(dcServiceI,dtMaxMoment);
   StoreAnalysisResultsAsDiagrams(dcServiceII,dtMinShear);
   StoreAnalysisResultsAsDiagrams(dcServiceII,dtMaxShear);
   StoreAnalysisResultsAsDiagrams(dcServiceII,dtMinMoment);
   StoreAnalysisResultsAsDiagrams(dcServiceII,dtMaxMoment);
   StoreAnalysisResultsAsDiagrams(dcServiceIII,dtMinShear);
   StoreAnalysisResultsAsDiagrams(dcServiceIII,dtMaxShear);
   StoreAnalysisResultsAsDiagrams(dcServiceIII,dtMinMoment);
   StoreAnalysisResultsAsDiagrams(dcServiceIII,dtMaxMoment);
   StoreAnalysisResultsAsDiagrams(dcFatigue,dtMinShear);
   StoreAnalysisResultsAsDiagrams(dcFatigue,dtMaxShear);
   StoreAnalysisResultsAsDiagrams(dcFatigue,dtMinMoment);
   StoreAnalysisResultsAsDiagrams(dcFatigue,dtMaxMoment);

#if defined GEN_DEBUG_CODE
   DumpResults();
   DumpDiagrams();
#endif

   return true;
}

bool LRFDBridge::GenerateModel()
{
   return Bridge::GenerateModel();
}

bool LRFDBridge::GeneratePOI()
{
   if (!Bridge::GeneratePOI())
      return false;

   SHOULD_CONTINUE;

   // Apply the unit loading to the structure to find the points
   // of contraflexure. Put Points of interest at these points.
   ApplyUnitLoad();

   // Analyze the structure for that loading
   try
      {
      femModel->SetEquilibriumCheckTolerance(1.e-02);
      femModel->StiffnessAnalysis();
      }
   catch(xmsg xMsg)
      {
      PROGRESS_MSG(xMsg.why().c_str());
      return false;
      }

   // Save the results
   unitLoadShearResultsId = NextAvailAnalysisResults();
   unitLoadMomentResultsId = NextAvailAnalysisResults();
   unitLoadReactionResultsId = NextAvailAnalysisResults();
   StoreAnalysisResults(unitLoadShearResultsId,
                        unitLoadMomentResultsId,
                        unitLoadReactionResultsId,
                        "Unit Load",
                        unitLoadId);

#  if defined GEN_DEBUG_CODE
   AnalysisResults *arMoment;
   arMoment = GetAnalysisResults(unitLoadMomentResultsId);
   ofstream ofile("unitload.log");
   ofile << "Unit load moment results" << endl << endl;
   ofile << (*arMoment) << endl;
   ofile.close();
#  endif

   SHOULD_CONTINUE;

   if (!FindContraflexurePoints())
      return false;

#  if defined GEN_DEBUG_CODE
   DumpContraflexurePoints();
#  endif

   if (!ContraflexurePOI())
      return false;

   SHOULD_CONTINUE;

   // We are done with this loading, get rid of it
   femModel->RemoveLoading(unitLoadId);
   unitLoadId = -1;

   return true;
}

bool LRFDBridge::ContraflexurePOI()
{
   TRACEX(Bridge_PntsOfInterest,LEVEL1,"LRFDBridge::ContraflexurePOI()");

   UnitLoadMomentZoneIterator iter(*ulMomentZones);
   int pnt;
   int poi;
   int mbrId;
   double frDist;
   Span *span;

   while(iter)
      {
      UnitLoadMomentZone *ulmz = iter++;
      for (pnt = 0; pnt < ulmz->nPoints; pnt++)
         {
         if (!CheckPOIConflict(ulmz->spanId,ulmz->loc[pnt]))
            {
            poi = NextAvailPointOfInterest();

            TRACEX(Bridge_PntsOfInterest,LEVEL2,"POI " << poi << " span = " << ulmz->spanId << " offset = " << ulmz->loc[pnt]);

            span = GetSpan(ulmz->spanId);

            pntsOfInterest->Add(new PointOfInterest(poi,span->GetID(),ulmz->loc[pnt]));

            span->FindMbr(femModel,ulmz->loc[pnt],mbrId,frDist);
            femModel->DefinePOI(poi,mbrId,frDist,true);
            }

         SHOULD_CONTINUE;
         } // end of pnt loop
      } // end of while

   return true;
}

bool LRFDBridge::ApplyLoadings()
{
   if (!Bridge::ApplyLoadings())
      return false;

   SHOULD_CONTINUE;

   if (!ApplySuperimposedLoads())
      return false;

   SHOULD_CONTINUE;

   if (!GenerateInfluenceLoadings())
      return false;

   SHOULD_CONTINUE;

   return true;
}

bool LRFDBridge::ApplyUnitLoad()
{
   // Apply a unit uniform load to the superstructure.
   // This load will be used to determine the inflection points
   Span *span;
   Loading *loading;
   Span::TIntIterator *i;
   int mbrId;

   unitLoadId = NextAvailLoading();
   loading = new Loading(unitLoadId);
   femModel->AddLoading(loading);
   for (span = GetFirstSpan(); span; span = span->NextSpan())
      {
      i = new Span::TIntIterator(*span->members);
      while(*i)
         {
         mbrId = (*i)++;
         loading->AddMbrLoad(new UniformLoad(mbrId,0.0,1.0,-1.0,true,ForceY,GlobalProjected));
         SHOULD_CONTINUE;
         }
      delete i;
      }

   return true;
}

bool LRFDBridge::ApplySuperimposedLoads()
{
   Span *span;
   Loading *loading;
   Span::TIntIterator *i;
   int mbrId;

   // generate fem loads for selfweight, traffic barrier, utility, etc...
   if (SelfWgtEnabled())
      {
      selfWgtId = NextAvailLoading();
      femModel->AddLoading(new DeadLoading(selfWgtId));
      SHOULD_CONTINUE;
      }

   if (TrafficBarrierEnabled())
      {
      trafficBarrierId = NextAvailLoading();
      loading = new Loading(trafficBarrierId);
      femModel->AddLoading(loading);
      for (span = GetFirstSpan(); span; span = span->NextSpan())
         {
         i = new Span::TIntIterator(*span->members);
         while(*i)
            {
            mbrId = (*i)++;
            loading->AddMbrLoad(new UniformLoad(mbrId,0.0,1.0,-TrafficBarrierLd(),true,ForceY,GlobalProjected));
            SHOULD_CONTINUE;
            }
         delete i;
         }
      }

   if (WearSurfEnabled())
      {
      wearSurfId = NextAvailLoading();
      loading = new Loading(wearSurfId);
      femModel->AddLoading(loading);
      for (span = GetFirstSpan(); span; span = span->NextSpan())
         {
         i = new Span::TIntIterator(*span->members);
         while(*i)
            {
            mbrId = (*i)++;
            loading->AddMbrLoad(new UniformLoad(mbrId,0.0,1.0,-WearSurfLd(),true,ForceY,GlobalProjected));
            SHOULD_CONTINUE;
            }
         delete i;
         }
      }

   if (UtilityEnabled())
      {
      utilityId = NextAvailLoading();
      loading = new Loading(utilityId);
      femModel->AddLoading(loading);
      for (span = GetFirstSpan(); span; span = span->NextSpan())
         {
         i = new Span::TIntIterator(*span->members);
         while(*i)
            {
            mbrId = (*i)++;
            loading->AddMbrLoad(new UniformLoad(mbrId,0.0,1.0,-UtilityLd(),true,ForceY,GlobalProjected));
            SHOULD_CONTINUE;
            }
         delete i;
         }
      }

   return true;
}

bool LRFDBridge::CombineDeadLoads()
{
   int femId;
   BridgeLoading *loading;
   LoadingCombo *combo;

   // DC
   loading = GetLoading(DC_LOAD);
   femId = loading->GetFemId();
   dcId = NextAvailCombo();
   combo = new LoadingCombo(dcId);
   femModel->AddLoadingCombination(combo);
   combo->AddLoadingData(femId,1.0);
   if (SelfWgtEnabled())
      combo->AddLoadingData(selfWgtId,1.0);
   if (TrafficBarrierEnabled())
      combo->AddLoadingData(trafficBarrierId,1.0);

   SHOULD_CONTINUE;

   // DW
   loading = GetLoading(DW_LOAD);
   femId = loading->GetFemId();
   dwId = NextAvailCombo();
   combo = new LoadingCombo(dwId);
   femModel->AddLoadingCombination(combo);
   combo->AddLoadingData(femId,1.0);
   if (WearSurfEnabled())
      combo->AddLoadingData(wearSurfId,1.0);
   if (UtilityEnabled())
      combo->AddLoadingData(utilityId,1.0);

   SHOULD_CONTINUE;

   femModel->CombineLoads();

   return true;
}

bool LRFDBridge::GenerateInfluenceLoadings()
{
   // Place influence loads directly onto the fem model. If they were generated
   // as bridge loadings, they would not be cleared when the user chooses to
   // analyze the bridge additional times.
   //
   // Place a concentrated load with magnitude -1.0 at each Point Of Interest

   Span *span;
   double offset;
   double frDist;
   int loadId;
   int mbrId;
   Loading *loading;

   TRACEX(LRFDBridge_InfluenceLines,LEVEL1,"LRFDBridge::GenerateInfluenceLoadings()");

   PointOfInterestIterator j(*pntsOfInterest);
   while(j)
      {
      PointOfInterest *poi = j++;

      span = GetSpan(poi->spanId);
      offset = poi->offset;

      loadId = NextAvailLoading();
      inflLoadRecords->Add(new InfluenceLoadRecord(poi->id,loadId));

      TRACEX(LRFDBridge_InfluenceLines,LEVEL2,"Span " << span->GetID() << " Offset " << offset << " Infl Id " << loadId);

      loading = new Loading(loadId);
      femModel->AddLoading(loading);
      span->FindMbr(femModel,offset,mbrId,frDist);

      TRACEX(LRFDBridge_InfluenceLines,LEVEL2,"Mbr " << mbrId << " Fract Dist " << frDist);

      loading->AddMbrLoad(new ConcentratedLoad(mbrId,frDist,-1.0,true,ForceY,Global));

      SHOULD_CONTINUE;
      }

   return true;
}

bool LRFDBridge::CreateInfluenceLines()
{
	CreateSuperstructureInfluenceLines();
   CreateReactionInfluenceLines();
   return true;
}

bool LRFDBridge::CreateSuperstructureInfluenceLines()
{
   double fxLeft,fyLeft,mzLeft;
   double fxRight,fyRight,mzRight;
   double x;
   PointOfInterest *poi;
   PointOfInterest *poiAtLoad;
   AnalysisResults *moment;
   AnalysisResults *shear;
   int arMomentId;
   int arShearId;
   char buffer[256];
   ostrstream os(buffer,sizeof(buffer));
   int idx;

   TRACEX(LRFDBridge_InfluenceLines,LEVEL1,"LRFDBridge::CreateSuperstructureInfluenceLines()");

   PROGRESS_MSG("Creating Superstructure Influence Lines");

#if defined GEN_DEBUG_CODE
   ofstream mfile("inflmnt.log");
   ofstream vfile("inflshr.log");

   mfile << "Moment Influence Values" << endl;
   vfile << "Shear Influence Values" << endl;
#endif

   PointOfInterestIterator i(*pntsOfInterest);
   while(i)
      {
      poi = i++;
      TRACEX(LRFDBridge_InfluenceLines,LEVEL2,"Influence Line for POI " << poi->id);

      //
      // Create new analysis results for influence at this poi
      //
      arMomentId = NextAvailAnalysisResults();
      os.seekp(0);
      os << "Moment Influence Line for POI " << poi->id << " (ar = " << arMomentId << ")" << ends;
      moment = new AnalysisResults(arMomentId,buffer);
      AddAnalysisResults(moment);

      arShearId = NextAvailAnalysisResults();
      os.seekp(0);
      os << "Shear Influence Line for POI " << poi->id << " (ar = " << arShearId << ")" << ends;
      shear = new AnalysisResults(arShearId,buffer);
      AddAnalysisResults(shear);

      inflResultsRecords->Add(new InfluenceResultsRecord(poi->id,arMomentId,arShearId));

      //
      // Get the structural response at this poi from each of a point load
      // at every other poi in the model.
      //
      PointOfInterestIterator j(*pntsOfInterest);

      while (j)
         {
         SHOULD_CONTINUE;

         poiAtLoad = j++;

         // Get Influence line record for the poi where the load is place.
         InfluenceLoadRecord testRec2(poiAtLoad->id,0);
         idx = inflLoadRecords->Find(&testRec2);
         CHECKX(idx != INT_MAX,"LRFDBridge::CreateInfluenceLines() - Influence Line Record Not Found (2)");
         InfluenceLoadRecord *ilRecord = (*inflLoadRecords)[idx];

         TRACEX(LRFDBridge_InfluenceLines,LEVEL2,"Load At POI " << poiAtLoad->id
                               << " span = "     << poiAtLoad->spanId
                               << " offset = "   << poiAtLoad->offset);

         // Get force at "poi->id" cause by a load at "poiAtLoad->id"
         femModel->GetPOIForces(poi->id,ilRecord->loadId,mbrLeftFace,fxLeft,fyLeft,mzLeft);
         TRACEX(LRFDBridge_InfluenceLines,LEVEL2,"Left Face Fy = " << fyLeft << " Mz = " << mzLeft);

         femModel->GetPOIForces(poi->id,ilRecord->loadId,mbrRightFace,fxRight,fyRight,mzRight);
         TRACEX(LRFDBridge_InfluenceLines,LEVEL2,"Right Face Fy = " << fyRight << " Mz = " << mzRight);

         x = GetSuperstructureOffset(poiAtLoad->spanId,poiAtLoad->offset);
         moment->AddResult(poiAtLoad->id,x,mzLeft,-mzRight);
         shear->AddResult(poiAtLoad->id,x,fyRight,-fyLeft); // See note below

         // Note: Why is fyRight used for left face result and fyLeft used for
         //       right face result?
         //
         // Imagine the influence line at 0.5L for shear in a simple span
         // beam. The influence values to the left of 0.5L are < 0 and those
         // to the right are > 0. Now, imagine that a unit load is placed at
         // 0.4L. The shear diagram for this loading is 0.6 left of the load
         // and -0.4 right of the load. The fem model will return -0.6 for the
         // left face value at 0.4L and 0.4 for the right face value at 0.4L.
         // The influence coefficient for shear at 0.5L for a load applied at
         // 0.4L is -0.4. As you can see, this is the right face value as
         // returned by the fem model.
         //
         } // end of poiAtLoad iteration (j)

#if defined GEN_DEBUG_CODE
      // Dump to log file
      mfile << (*moment) << endl;
      vfile << (*shear)  << endl;
#endif
      } // end of poi iteration (i)

   return true;
}

bool LRFDBridge::CreateReactionInfluenceLines()
{
   double Fx, Fy, Mz;
   double x;
   Support* s;
   PointOfInterest *poiAtLoad;
   AnalysisResults *reactionFx;
   AnalysisResults *reactionFy;
   AnalysisResults *reactionMz;
   int arReactionFxId;
   int arReactionFyId;
   int arReactionMzId;
   char buffer[256];
   ostrstream os(buffer,sizeof(buffer));
   int idx;

   TRACEX(LRFDBridge_InfluenceLines,LEVEL1,"LRFDBridge::CreateReactionInfluenceLines()");

   PROGRESS_MSG("Creating Reaction Lines");

#if defined GEN_DEBUG_CODE
   ofstream rfile("inflrea.log");

   rfile << "Reaction Influence Values" << endl;
#endif

   for ( s = firstSupport; s != NULL; s = s->NextSupport() )
      {
      int pierId = s->GetID();
      TRACEX(LRFDBridge_InfluenceLines,LEVEL2,"Influence Line for Support " << pierId);

      //
      // Create new analysis results for influence at this pier
      //
      arReactionFxId = NextAvailAnalysisResults();
      os.seekp(0);
      os << "Fx Reaction Influence Line for Pier " << pierId << " (ar = " << arReactionFxId << ")" << ends;
      reactionFx = new AnalysisResults(arReactionFxId,buffer);
      AddAnalysisResults(reactionFx);

      arReactionFyId = NextAvailAnalysisResults();
      os.seekp(0);
      os << "Fy Reaction Influence Line for Pier " << pierId << " (ar = " << arReactionFyId << ")" << ends;
      reactionFy = new AnalysisResults(arReactionFyId,buffer);
      AddAnalysisResults(reactionFy);

      arReactionMzId = NextAvailAnalysisResults();
      os.seekp(0);
      os << "Mz Reaction Influence Line for Pier " << pierId << " (ar = " << arReactionMzId << ")" << ends;
      reactionMz = new AnalysisResults(arReactionMzId,buffer);
      AddAnalysisResults(reactionMz);

      inflReactionRecords->Add(new InfluenceReactionRecord(pierId,arReactionFxId,arReactionFyId,arReactionMzId));

      //
      // Get the structural response at this support from each of a point load
      // at every other poi in the model.
      //
      PointOfInterestIterator j(*pntsOfInterest);

      while (j)
         {
         SHOULD_CONTINUE;

         poiAtLoad = j++;

         // Get Influence line record for the poi where the load is placed.
         InfluenceLoadRecord testRec2(poiAtLoad->id,0);
         idx = inflLoadRecords->Find(&testRec2);
         CHECKX(idx != INT_MAX,"LRFDBridge::CreateInfluenceLines() - Influence Line Record Not Found (2)");
         InfluenceLoadRecord *ilRecord = (*inflLoadRecords)[idx];

         TRACEX(LRFDBridge_InfluenceLines,LEVEL2,"Load At POI " << poiAtLoad->id
                               << " span = "     << poiAtLoad->spanId
                               << " offset = "   << poiAtLoad->offset);

         // Get reaction at this support cause by a load at "poiAtLoad->id"
         int jntId = s->EndJntId();
         femModel->GetReactions(jntId,ilRecord->loadId,Fx,Fy,Mz);
         TRACEX(LRFDBridge_InfluenceLines,LEVEL2,"Reaction: Fx = " << Fx << " Fy = " << Fy << " Mz = " << Mz);

         x = GetSuperstructureOffset(poiAtLoad->spanId,poiAtLoad->offset);
         reactionFx->AddResult(poiAtLoad->id,x,Fx,Fx);
         reactionFy->AddResult(poiAtLoad->id,x,Fy,Fy);
         reactionMz->AddResult(poiAtLoad->id,x,Mz,Mz);
         } // end of poiAtLoad iteration (j)

#if defined GEN_DEBUG_CODE
      // Dump to log file
      rfile << "Reaction Fx: " << (*reactionFx) << endl;
      rfile << "Reaction Fy: " << (*reactionFy) << endl;
      rfile << "Reaction Mz: " << (*reactionMz) << endl;
#endif
      } // end of support loop

   return true;
}

bool LRFDBridge::FindContraflexurePoints()
{
   Span *span;
   int momentSign;
   int prevMomentSign;
   double gOffset;
   double lOffset;
   double prevOffset;
   double lfMoment, rfMoment;
   double lfPrevMoment, rfPrevMoment;
   UnitLoadMomentZone *ulmz;
   AnalysisResults *ar;

   TRACEX(LRFDBridge_Contraflexure,LEVEL1,"LRFDBridge::FindContraflexurePoints()");

   ar = GetAnalysisResults(unitLoadMomentResultsId);

   for (span = GetFirstSpan(); span; span = span->NextSpan())
      {
      TRACEX(LRFDBridge_Contraflexure,LEVEL2,"Searching for contraflexure point in span " << span->GetID());

      bool pointFound = false;
      ulmz = new UnitLoadMomentZone(span->GetID());
      ulMomentZones->Add(ulmz);

      if (span->IsCantilevered())
         {
         pointFound = true;
         if (span == GetFirstSpan())
            {
            TRACEX(LRFDBridge_Contraflexure,LEVEL2,"Span is first span and is cantilevered");
            ulmz->SetContraflexurePoint(0.00,0,-1);
            }
         else
            {
            TRACEX(LRFDBridge_Contraflexure,LEVEL2,"Span is last span and is cantilevered");
            ulmz->SetContraflexurePoint(span->Length(),-1,0);
            }
         }
      else
         {
         TRACEX(LRFDBridge_Contraflexure,LEVEL2,"Span is not cantilevered");

         SortedPointOfInterestIterator iter(span->GetID(),*pntsOfInterest);

         // Initial prev point with first poi in span
         PointOfInterest *poi = iter++;
         ar->GetResult(poi->id,gOffset,lfPrevMoment,rfPrevMoment);
         prevOffset = poi->offset;
         __GetSign(rfPrevMoment,prevMomentSign);

         while(iter)
            {
            poi = iter++;

            TRACEX(LRFDBridge_Contraflexure,LEVEL2,"Current POI = " << poi->id);

            lOffset = poi->offset;
            ar->GetResult(poi->id,gOffset,lfMoment,rfMoment);

            __GetSign(lfMoment,momentSign);

            // Check if the sign changes between the previous poi and
            // this one
            TRACEX(LRFDBridge_Contraflexure,LEVEL2,"Testing of zero point between poi");
            TRACEX(LRFDBridge_Contraflexure,LEVEL2,"rfPrevMoment " << rfPrevMoment);
            TRACEX(LRFDBridge_Contraflexure,LEVEL2,"lfMoment     " << lfMoment);
            if ( (prevMomentSign < 0 && momentSign >= 0) ||
                 (prevMomentSign > 0 && momentSign <= 0) )
               {
               TRACEX(LRFDBridge_Contraflexure,LEVEL2,"Sign change has occured");

               // Sign change occurs between this poi and the previous one
               // Approximate zero point with linear interpolation
               double DX,DY, dx;
               DX = lOffset - prevOffset;
               DY = lfMoment - rfPrevMoment;
               dx = lfMoment * DX/DY;

               if (momentSign == 0)
                  {
                  // If there is a poi after this one, get the sign of the
                  // moment.
                  if (iter)
                     {
                     poi = iter.Current();
                     double lfNextMoment, rfNextMoment;
                     ar->GetResult(poi->id,gOffset,lfNextMoment,rfNextMoment);
                     __GetSign(lfNextMoment,momentSign);
                     }
                  }

               ulmz->SetContraflexurePoint(lOffset-dx,prevMomentSign,momentSign);
               pointFound = true;
               }

            // Check if the sign changes at this poi (discontinuous moment diagram)
            int lfSign, rfSign;
            __GetSign(lfMoment,lfSign);
            __GetSign(rfMoment,rfSign);

            TRACEX(LRFDBridge_Contraflexure,LEVEL2,"Testing for sign reversal at this poi");
            TRACEX(LRFDBridge_Contraflexure,LEVEL2,"lfMoment   " << lfMoment);
            TRACEX(LRFDBridge_Contraflexure,LEVEL2,"rfMoment   " << rfMoment);
            if ( (lfSign <= 0 && rfSign > 0) ||
                 (lfSign >= 0 && rfSign < 0) )
               {
               TRACEX(LRFDBridge_Contraflexure,LEVEL2,"Moment diagram is discontinuous at this poi");
               ulmz->SetContraflexurePoint(lOffset,lfSign,rfSign);
               pointFound = true;
               }

            // These poi values become the previous values
            lfPrevMoment = lfMoment;
            rfPrevMoment = rfMoment;
            prevOffset = lOffset;
            prevMomentSign = rfSign;

            SHOULD_CONTINUE;
            } // end of while
         } // end of else

      if (!pointFound)
         {
         TRACEX(LRFDBridge_Contraflexure,LEVEL2,"A zero point was not found in this span");
         // A point of zero moment was not found in this span

         // put a point a mid-span for the first or last span
         if (span == GetFirstSpan())
            {
            TRACEX(LRFDBridge_Contraflexure,LEVEL2,"Putting point at mid span of first span");
            ulmz->SetContraflexurePoint(span->Length()/2,1,-1);
            }
         else if (span == GetLastSpan())
            {
            TRACEX(LRFDBridge_Contraflexure,LEVEL2,"Putting point at mid span of last span");
            ulmz->SetContraflexurePoint(span->Length()/2,-1,1);
            }
         else
            {
            // Interior span...
            // Moment values are either all positive or all negative
            //
            // If the last point was < 0 then the all points must be < 0
            if (prevMomentSign < 0)
               {
               TRACEX(LRFDBridge_Contraflexure,LEVEL2,"All points < 0 in this span");
               ulmz->SetContraflexurePoint(0,0,-1);
               ulmz->SetContraflexurePoint(span->Length(),-1,0);
               }
            else
               {
               TRACEX(LRFDBridge_Contraflexure,LEVEL2,"All points >= 0 in this span");
               // All positive
               ulmz->SetContraflexurePoint(0,0,1);
               ulmz->SetContraflexurePoint(span->Length(),1,0);
               }
            }
         }

      } // end of span loop

   return true;
}

bool LRFDBridge::UnitLoadNegMomentRegion(int spanId,double offset,MbrFace face)
{
   UnitLoadMomentZone test(spanId);
   UnitLoadMomentZone *ulmz;
   int idx;

   TRACEX(LRFDBridge_Contraflexure,LEVEL1,"LRFDBridge::UnitLoadNegMomentRegion()");

   idx = ulMomentZones->Find(&test);

   CHECKX(idx != INT_MAX,"LRFDBridge::UnitLoadNegMomentRegion() - record not found");

   ulmz = (*ulMomentZones)[idx];

   return ulmz->NegMomentRegion(offset,face);
}

LRFDBridge::UnitLoadMomentZone::UnitLoadMomentZone(int spanId) :
spanId(spanId)
{
   nPoints = 0;

   loc[0] = -1;
   loc[1] = -1;
   leftSign[0] = -2;
   leftSign[1] = -2;
   rightSign[0] = -2;
   rightSign[1] = -2;
}

bool LRFDBridge::UnitLoadMomentZone::NegMomentRegion(double offset,MbrFace face)
{
   int i;
   int Sign;

   TRACEX(LRFDBridge_Contraflexure,LEVEL1,"LRFDBridge::UnitLoadMomentZone::NegMomentRegion()");

   // Find index of first record where loc >= offset
   for (i = 0; i < nPoints; i++)
      {
      if (offset <= loc[i])
         break;
      }

   CHECKX(i < 3,"LRFDBridge::UnitLoadMomentZone::NegMomentRegion() - zone not found");

   if (i == nPoints)
      {
      // offset is beyond last stored point
      // use sign to the right of the last stored point
      Sign = rightSign[i-1];
      }
   else if (zero_val(offset-loc[i]))
      {
      // offset is exactly at a stored point
      if (face == mbrLeftFace)
         Sign = leftSign[i];
      else
         Sign = rightSign[i];
      }
   else
      Sign = leftSign[i];

   CHECKX(Sign == 1 || Sign == -1 || Sign == 0,"LRFDBridge::UnitLoadMomentZone::NegMomentRegion() - bad moment sign");
   TRACEX(LRFDBridge_Contraflexure,LEVEL2,"Sign at span " << spanId << " offset " << offset << " is " << Sign);

   return (Sign < 0);
}

void LRFDBridge::UnitLoadMomentZone::SetContraflexurePoint(double offset,int _leftSign,int _rightSign)
{
   TRACEX(LRFDBridge_Contraflexure,LEVEL1,"LRFDBridge::UnitLoadMomentZone::SetContraflexurePoint()");

   CHECKX(nPoints < 3,"LRFDBridge::UnitLoadMomentZone::SetContraflexurePoint() - Attempting to add too many points");

   TRACEX(LRFDBridge_Contraflexure,LEVEL2,"Setting contraflexure point for span " << spanId);
   TRACEX(LRFDBridge_Contraflexure,LEVEL2,"Offset     = " << offset);
   TRACEX(LRFDBridge_Contraflexure,LEVEL2,"Left Sign  = " << _leftSign);
   TRACEX(LRFDBridge_Contraflexure,LEVEL2,"Right Sign = " << _rightSign);

   loc[nPoints] = offset;
   leftSign[nPoints]  = _leftSign;
   rightSign[nPoints] = _rightSign;

   nPoints++;
}

//
//
// Debugging Code -------------------------------------------------------------
//
//

#if defined GEN_DEBUG_CODE
void LRFDBridge::DumpContraflexurePoints()
{
   ofstream ofile("zeromom.log");
   UnitLoadMomentZoneIterator iter(*ulMomentZones);
   int i;

   while(iter)
      {
      UnitLoadMomentZone *ulmz = iter++;
      ofile << ulmz->spanId << endl;
      for (i = 0; i < ulmz->nPoints; i++)
         {
         ofile << setw(6)
               << "Loc"
               << setw(10)
               << setiosflags(ios::showpoint)
               << setprecision(3)
               << ulmz->loc[i]
               << setw(15)
               << "Left Sign"
               << setw(4)
               << ulmz->leftSign[i]
               << setw(15)
               << "Right Sign"
               << setw(4)
               << ulmz->rightSign[i]
               << endl;
         }
      }

   ofile.close();
}

void LRFDBridge::DumpResults()
{
   ofstream ofile("results.log");

   AnalysisResults *ar;

   ar = GetAnalysisResults(GetAnalysisResultsId(dcDC,dtMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcDC,dtShear));
   ofile << (*ar) << endl;

   ar = GetAnalysisResults(GetAnalysisResultsId(dcDW,dtMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcDW,dtShear));
   ofile << (*ar) << endl;

   ar = GetAnalysisResults(GetAnalysisResultsId(dcAll,dtMinMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcAll,dtMaxMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcAll,dtMinShear));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcAll,dtMaxShear));
   ofile << (*ar) << endl;

   ar = GetAnalysisResults(GetAnalysisResultsId(dcDesignTandem,dtMinMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcDesignTandem,dtMaxMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcDesignTandem,dtMinShear));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcDesignTandem,dtMaxShear));
   ofile << (*ar) << endl;

   ar = GetAnalysisResults(GetAnalysisResultsId(dcDesignTruck,dtMinMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcDesignTruck,dtMaxMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcDesignTruck,dtMinShear));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcDesignTruck,dtMaxShear));
   ofile << (*ar) << endl;

   ar = GetAnalysisResults(GetAnalysisResultsId(dcDualTruckTrain,dtMinMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcDualTruckTrain,dtMaxMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcDualTruckTrain,dtMinShear));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcDualTruckTrain,dtMaxShear));
   ofile << (*ar) << endl;

   ar = GetAnalysisResults(GetAnalysisResultsId(dcFatigueTruck,dtMinMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcFatigueTruck,dtMaxMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcFatigueTruck,dtMinShear));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcFatigueTruck,dtMaxShear));
   ofile << (*ar) << endl;

   ar = GetAnalysisResults(GetAnalysisResultsId(dcPedestrian,dtMinMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcPedestrian,dtMaxMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcPedestrian,dtMinShear));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcPedestrian,dtMaxShear));
   ofile << (*ar) << endl;

   ar = GetAnalysisResults(GetAnalysisResultsId(dcStrengthI,dtMinMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcStrengthI,dtMaxMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcStrengthI,dtMinShear));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcStrengthI,dtMaxShear));
   ofile << (*ar) << endl;

   ar = GetAnalysisResults(GetAnalysisResultsId(dcServiceI,dtMinMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcServiceI,dtMaxMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcServiceI,dtMinShear));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcServiceI,dtMaxShear));
   ofile << (*ar) << endl;

   ar = GetAnalysisResults(GetAnalysisResultsId(dcServiceII,dtMinMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcServiceII,dtMaxMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcServiceII,dtMinShear));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcServiceII,dtMaxShear));
   ofile << (*ar) << endl;

   ar = GetAnalysisResults(GetAnalysisResultsId(dcServiceIII,dtMinMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcServiceIII,dtMaxMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcServiceIII,dtMinShear));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcServiceIII,dtMaxShear));
   ofile << (*ar) << endl;

   ar = GetAnalysisResults(GetAnalysisResultsId(dcFatigue,dtMinMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcFatigue,dtMaxMoment));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcFatigue,dtMinShear));
   ofile << (*ar) << endl;
   ar = GetAnalysisResults(GetAnalysisResultsId(dcFatigue,dtMaxShear));
   ofile << (*ar) << endl;

   ofile.close();
}

void LRFDBridge::DumpDiagrams()
{
   ofstream ofile("diagrams.log");
   Diagram *diagram;

   diagram = GetDiagram(GetDiagramId(dcDC,dtMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcDC,dtShear));
   ofile << (*diagram) << endl;

   diagram = GetDiagram(GetDiagramId(dcDW,dtMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcDW,dtShear));
   ofile << (*diagram) << endl;

   diagram = GetDiagram(GetDiagramId(dcAll,dtMinMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcAll,dtMaxMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcAll,dtMinShear));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcAll,dtMaxShear));
   ofile << (*diagram) << endl;

   diagram = GetDiagram(GetDiagramId(dcDesignTandem,dtMinMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcDesignTandem,dtMaxMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcDesignTandem,dtMinShear));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcDesignTandem,dtMaxShear));
   ofile << (*diagram) << endl;

   diagram = GetDiagram(GetDiagramId(dcDesignTruck,dtMinMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcDesignTruck,dtMaxMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcDesignTruck,dtMinShear));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcDesignTruck,dtMaxShear));
   ofile << (*diagram) << endl;

   diagram = GetDiagram(GetDiagramId(dcDualTruckTrain,dtMinMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcDualTruckTrain,dtMaxMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcDualTruckTrain,dtMinShear));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcDualTruckTrain,dtMaxShear));
   ofile << (*diagram) << endl;

   diagram = GetDiagram(GetDiagramId(dcFatigueTruck,dtMinMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcFatigueTruck,dtMaxMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcFatigueTruck,dtMinShear));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcFatigueTruck,dtMaxShear));
   ofile << (*diagram) << endl;

   diagram = GetDiagram(GetDiagramId(dcPedestrian,dtMinMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcPedestrian,dtMaxMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcPedestrian,dtMinShear));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcPedestrian,dtMaxShear));
   ofile << (*diagram) << endl;

   diagram = GetDiagram(GetDiagramId(dcStrengthI,dtMinMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcStrengthI,dtMaxMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcStrengthI,dtMinShear));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcStrengthI,dtMaxShear));
   ofile << (*diagram) << endl;

   diagram = GetDiagram(GetDiagramId(dcServiceI,dtMinMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcServiceI,dtMaxMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcServiceI,dtMinShear));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcServiceI,dtMaxShear));
   ofile << (*diagram) << endl;

   diagram = GetDiagram(GetDiagramId(dcServiceII,dtMinMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcServiceII,dtMaxMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcServiceII,dtMinShear));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcServiceII,dtMaxShear));
   ofile << (*diagram) << endl;

   diagram = GetDiagram(GetDiagramId(dcServiceIII,dtMinMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcServiceIII,dtMaxMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcServiceIII,dtMinShear));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcServiceIII,dtMaxShear));
   ofile << (*diagram) << endl;

   diagram = GetDiagram(GetDiagramId(dcFatigue,dtMinMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcFatigue,dtMaxMoment));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcFatigue,dtMinShear));
   ofile << (*diagram) << endl;
   diagram = GetDiagram(GetDiagramId(dcFatigue,dtMaxShear));
   ofile << (*diagram) << endl;

   ofile.close();
}
#endif

