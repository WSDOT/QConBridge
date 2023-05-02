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
#include <code\lrfdcode.h>
#include <float.h>


DIAG_DEFINE_GROUP(LRFDBridge_Combinations,ENABLED,LEVEL1);

bool LRFDBridge::CreateLoadCombinations()
{
   double lfMinMoment, lfMaxMoment;
   double rfMinMoment, rfMaxMoment;
   double lfMinShear,  lfMaxShear;
   double rfMinShear,  rfMaxShear;
   double nd, nr, ni;  // limit state factors
   double QMaxMoment;  // max moment force effect
   double QMinMoment;  // min moment force effect
   double QMaxShear;   // max shear force effect
   double QMinShear;   // min shear force effect
   double QFxMin, QFyMin, QMzMin;
   double QFxMax, QFyMax, QMzMax;
   double lfDCmin;     // load factor for DC (min)
   double lfDCmax;     // load factor for DC (max)
   double lfDWmin;     // load factor for DW (min)
   double lfDWmax;     // load factor for DW (max)
   double lfLL;        // load factor for liveload
   LLDistFactors *lldf; // Live load distribution factors
   double offset;
   double gPosMoment, gNegMoment, gShear;
   LRFDSpan *span;
   AnalysisResults *maxMomentEnv;
   AnalysisResults *minMomentEnv;
   AnalysisResults *maxShearEnv;
   AnalysisResults *minShearEnv;
   AnalysisResults *maxReactionEnv;
   AnalysisResults *minReactionEnv;
   LoadCombiner *combineMinMoment;
   LoadCombiner *combineMaxMoment;
   LoadCombiner *combineMinShear;
   LoadCombiner *combineMaxShear;
   LoadCombiner *combineFxMin, *combineFyMin, *combineMzMin;
   LoadCombiner *combineFxMax, *combineFyMax, *combineMzMax;
   LoadCombiner::LimitState limitState;
   string desc;

   TRACEX(LRFDBridge_Combinations,LEVEL1,"Creating Load Combinations");

   combineMinMoment = new LoadCombiner;
   combineMaxMoment = new LoadCombiner;
   combineMinShear  = new LoadCombiner;
   combineMaxShear  = new LoadCombiner;
   combineFxMin = new LoadCombiner;
   combineFyMin = new LoadCombiner;
   combineMzMin = new LoadCombiner;
   combineFxMax = new LoadCombiner;
   combineFyMax = new LoadCombiner;
   combineMzMax = new LoadCombiner;

   for (DiagramCatagory dc = dcStrengthI; dc <= dcFatigue; dc++)
      {
      switch(dc)
         {
         case dcStrengthI:
              PROGRESS_MSG("Creating Strength I Limit State Combination");
              desc = "Strength I";
              break;

         case dcServiceI:
              PROGRESS_MSG("Creating Service I Limit State Combination");
              desc = "Service I";
              break;

         case dcServiceII:
              PROGRESS_MSG("Creating Service II Limit State Combination");
              desc = "Service II";
              break;

         case dcServiceIII:
              PROGRESS_MSG("Creating Service III Limit State Combination");
              desc = "Service III";
              break;

         case dcFatigue:
              PROGRESS_MSG("Creating Fatigue Limit State Combination");
              desc = "Fatigue";
              break;
         }

      // Prepare envelopes...
      AllocateEnvelopes(dc,desc.c_str());

      maxMomentEnv = GetAnalysisResults(GetAnalysisResultsId(dc,dtMaxMoment));
      minMomentEnv = GetAnalysisResults(GetAnalysisResultsId(dc,dtMinMoment));
      maxShearEnv  = GetAnalysisResults(GetAnalysisResultsId(dc,dtMaxShear));
      minShearEnv  = GetAnalysisResults(GetAnalysisResultsId(dc,dtMinShear));
      maxReactionEnv  = GetAnalysisResults(GetAnalysisResultsId(dc,dtMaxReaction));
      minReactionEnv  = GetAnalysisResults(GetAnalysisResultsId(dc,dtMinReaction));

      InitializeEnvelopes(maxMomentEnv,minMomentEnv,maxShearEnv,minShearEnv,maxReactionEnv,minReactionEnv);

      // Do the combinations
      PointOfInterestIterator j(*pntsOfInterest);
      while(j)
         {
         PointOfInterest *poi = j++;

         for (MbrFace face = mbrLeftFace; face <= mbrRightFace; face++)
            {
            // Get Dead Loads
            combineMinMoment->SetLoading(LoadCombiner::loadingDC,GetResult(poi->id,face,dcDC,dtMoment));
            combineMaxMoment->SetLoading(LoadCombiner::loadingDC,GetResult(poi->id,face,dcDC,dtMoment));
            combineMinMoment->SetLoading(LoadCombiner::loadingDW,GetResult(poi->id,face,dcDW,dtMoment));
            combineMaxMoment->SetLoading(LoadCombiner::loadingDW,GetResult(poi->id,face,dcDW,dtMoment));
            combineMinShear->SetLoading(LoadCombiner::loadingDC,GetResult(poi->id,face,dcDC,dtShear));
            combineMaxShear->SetLoading(LoadCombiner::loadingDC,GetResult(poi->id,face,dcDC,dtShear));
            combineMinShear->SetLoading(LoadCombiner::loadingDW,GetResult(poi->id,face,dcDW,dtShear));
            combineMaxShear->SetLoading(LoadCombiner::loadingDW,GetResult(poi->id,face,dcDW,dtShear));

            span = TYPESAFE_DOWNCAST(GetSpan(poi->spanId),LRFDSpan);
            lldf = span->FindLLDistFactors(poi->offset);

            switch(dc)
               {
               case dcStrengthI:
                    limitState = LoadCombiner::StrengthI;
                    nd = span->GetDuctilityFactor(lsStrength);
                    nr = span->GetRedundancyFactor(lsStrength);
                    ni = span->GetImportanceFactor(lsStrength);

                    combineMinMoment->SetLoading(LoadCombiner::loadingLLIM,GetResult(poi->id,face,dcAll,dtMinMoment));
                    combineMaxMoment->SetLoading(LoadCombiner::loadingLLIM,GetResult(poi->id,face,dcAll,dtMaxMoment));
                    combineMinShear->SetLoading(LoadCombiner::loadingLLIM,GetResult(poi->id,face,dcAll,dtMinShear));
                    combineMaxShear->SetLoading(LoadCombiner::loadingLLIM,GetResult(poi->id,face,dcAll,dtMaxShear));

                    combineMinMoment->SetLoading(LoadCombiner::loadingPL,GetResult(poi->id,face,dcPedestrian,dtMinMoment));
                    combineMaxMoment->SetLoading(LoadCombiner::loadingPL,GetResult(poi->id,face,dcPedestrian,dtMaxMoment));
                    combineMinShear->SetLoading(LoadCombiner::loadingPL,GetResult(poi->id,face,dcPedestrian,dtMinShear));
                    combineMaxShear->SetLoading(LoadCombiner::loadingPL,GetResult(poi->id,face,dcPedestrian,dtMaxShear));

                    GetStrengthILoadFactors(lfDCmin,lfDCmax,lfDWmin,lfDWmax,lfLL);
                    gPosMoment = lldf->s_gPosMoment;
                    gNegMoment = lldf->s_gNegMoment;
                    gShear     = lldf->s_gShear;
                    break;

               case dcServiceI:
                    limitState = LoadCombiner::ServiceI;
                    nd = span->GetDuctilityFactor(lsService);
                    nr = span->GetRedundancyFactor(lsService);
                    ni = span->GetImportanceFactor(lsService);

                    combineMinMoment->SetLoading(LoadCombiner::loadingLLIM,GetResult(poi->id,face,dcAll,dtMinMoment));
                    combineMaxMoment->SetLoading(LoadCombiner::loadingLLIM,GetResult(poi->id,face,dcAll,dtMaxMoment));
                    combineMinShear->SetLoading(LoadCombiner::loadingLLIM,GetResult(poi->id,face,dcAll,dtMinShear));
                    combineMaxShear->SetLoading(LoadCombiner::loadingLLIM,GetResult(poi->id,face,dcAll,dtMaxShear));

                    combineMinMoment->SetLoading(LoadCombiner::loadingPL,GetResult(poi->id,face,dcPedestrian,dtMinMoment));
                    combineMaxMoment->SetLoading(LoadCombiner::loadingPL,GetResult(poi->id,face,dcPedestrian,dtMaxMoment));
                    combineMinShear->SetLoading(LoadCombiner::loadingPL,GetResult(poi->id,face,dcPedestrian,dtMinShear));
                    combineMaxShear->SetLoading(LoadCombiner::loadingPL,GetResult(poi->id,face,dcPedestrian,dtMaxShear));

                    GetServiceILoadFactors(lfDCmin,lfDWmin,lfLL);
                    lfDCmax = lfDCmin;
                    lfDWmax = lfDWmin;
                    gPosMoment = lldf->s_gPosMoment;
                    gNegMoment = lldf->s_gNegMoment;
                    gShear     = lldf->s_gShear;
                    break;

               case dcServiceII:
                    limitState = LoadCombiner::ServiceII;
                    nd = span->GetDuctilityFactor(lsService);
                    nr = span->GetRedundancyFactor(lsService);
                    ni = span->GetImportanceFactor(lsService);

                    combineMinMoment->SetLoading(LoadCombiner::loadingLLIM,GetResult(poi->id,face,dcAll,dtMinMoment));
                    combineMaxMoment->SetLoading(LoadCombiner::loadingLLIM,GetResult(poi->id,face,dcAll,dtMaxMoment));
                    combineMinShear->SetLoading(LoadCombiner::loadingLLIM,GetResult(poi->id,face,dcAll,dtMinShear));
                    combineMaxShear->SetLoading(LoadCombiner::loadingLLIM,GetResult(poi->id,face,dcAll,dtMaxShear));

                    combineMinMoment->SetLoading(LoadCombiner::loadingPL,GetResult(poi->id,face,dcPedestrian,dtMinMoment));
                    combineMaxMoment->SetLoading(LoadCombiner::loadingPL,GetResult(poi->id,face,dcPedestrian,dtMaxMoment));
                    combineMinShear->SetLoading(LoadCombiner::loadingPL,GetResult(poi->id,face,dcPedestrian,dtMinShear));
                    combineMaxShear->SetLoading(LoadCombiner::loadingPL,GetResult(poi->id,face,dcPedestrian,dtMaxShear));

                    GetServiceIILoadFactors(lfDCmin,lfDWmin,lfLL);
                    lfDCmax = lfDCmin;
                    lfDWmax = lfDWmin;
                    gPosMoment = lldf->s_gPosMoment;
                    gNegMoment = lldf->s_gNegMoment;
                    gShear     = lldf->s_gShear;
                    break;

               case dcServiceIII:
                    limitState = LoadCombiner::ServiceIII;
                    nd = span->GetDuctilityFactor(lsService);
                    nr = span->GetRedundancyFactor(lsService);
                    ni = span->GetImportanceFactor(lsService);

                    combineMinMoment->SetLoading(LoadCombiner::loadingLLIM,GetResult(poi->id,face,dcAll,dtMinMoment));
                    combineMaxMoment->SetLoading(LoadCombiner::loadingLLIM,GetResult(poi->id,face,dcAll,dtMaxMoment));
                    combineMinShear->SetLoading(LoadCombiner::loadingLLIM,GetResult(poi->id,face,dcAll,dtMinShear));
                    combineMaxShear->SetLoading(LoadCombiner::loadingLLIM,GetResult(poi->id,face,dcAll,dtMaxShear));

                    combineMinMoment->SetLoading(LoadCombiner::loadingPL,GetResult(poi->id,face,dcPedestrian,dtMinMoment));
                    combineMaxMoment->SetLoading(LoadCombiner::loadingPL,GetResult(poi->id,face,dcPedestrian,dtMaxMoment));
                    combineMinShear->SetLoading(LoadCombiner::loadingPL,GetResult(poi->id,face,dcPedestrian,dtMinShear));
                    combineMaxShear->SetLoading(LoadCombiner::loadingPL,GetResult(poi->id,face,dcPedestrian,dtMaxShear));

                    GetServiceIIILoadFactors(lfDCmin,lfDWmin,lfLL);
                    lfDCmax = lfDCmin;
                    lfDWmax = lfDWmin;
                    gPosMoment = lldf->s_gPosMoment;
                    gNegMoment = lldf->s_gNegMoment;
                    gShear     = lldf->s_gShear;
                    break;

               case dcFatigue:
                    limitState = LoadCombiner::Fatigue;
                    nd = span->GetDuctilityFactor(lsFatigue);
                    nr = span->GetRedundancyFactor(lsFatigue);
                    ni = span->GetImportanceFactor(lsFatigue);

                    GetFatigueLoadFactors(lfDCmin,lfDWmin,lfLL);
                    lfDCmax = lfDCmin;
                    lfDWmax = lfDWmin;

                    combineMinMoment->SetLoading(LoadCombiner::loadingLLIM,GetResult(poi->id,face,dcFatigueTruck,dtMinMoment));
                    combineMaxMoment->SetLoading(LoadCombiner::loadingLLIM,GetResult(poi->id,face,dcFatigueTruck,dtMaxMoment));
                    combineMinShear->SetLoading(LoadCombiner::loadingLLIM,GetResult(poi->id,face,dcFatigueTruck,dtMinShear));
                    combineMaxShear->SetLoading(LoadCombiner::loadingLLIM,GetResult(poi->id,face,dcFatigueTruck,dtMaxShear));

                    combineMinMoment->SetLoading(LoadCombiner::loadingPL,0);
                    combineMaxMoment->SetLoading(LoadCombiner::loadingPL,0);
                    combineMinShear->SetLoading(LoadCombiner::loadingPL,0);
                    combineMaxShear->SetLoading(LoadCombiner::loadingPL,0);

                    gPosMoment = lldf->f_gPosMoment;
                    gNegMoment = lldf->f_gNegMoment;
                    gShear     = lldf->f_gShear;
                    break;
               }

            TRACEX(LRFDBridge_Combinations,LEVEL2,
               "Combining Loads for " << desc << " at the "
               << ((face == mbrLeftFace) ? "Left" : "Right")
               << " of POI " << poi->id);


            combineMinMoment->SetLimitStateFactors(nd,nr,ni);
            combineMaxMoment->SetLimitStateFactors(nd,nr,ni);
            combineMinShear->SetLimitStateFactors(nd,nr,ni);
            combineMaxShear->SetLimitStateFactors(nd,nr,ni);

            combineMinMoment->SetLoadFactor(LoadCombiner::dcMin,limitState,lfDCmin);
            combineMaxMoment->SetLoadFactor(LoadCombiner::dcMin,limitState,lfDCmin);
            combineMinShear->SetLoadFactor(LoadCombiner::dcMin,limitState,lfDCmin);
            combineMaxShear->SetLoadFactor(LoadCombiner::dcMin,limitState,lfDCmin);

            combineMinMoment->SetLoadFactor(LoadCombiner::dcMax,limitState,lfDCmax);
            combineMaxMoment->SetLoadFactor(LoadCombiner::dcMax,limitState,lfDCmax);
            combineMinShear->SetLoadFactor(LoadCombiner::dcMax,limitState,lfDCmax);
            combineMaxShear->SetLoadFactor(LoadCombiner::dcMax,limitState,lfDCmax);

            combineMinMoment->SetLoadFactor(LoadCombiner::dwMin,limitState,lfDWmin);
            combineMaxMoment->SetLoadFactor(LoadCombiner::dwMin,limitState,lfDWmin);
            combineMinShear->SetLoadFactor(LoadCombiner::dwMin,limitState,lfDWmin);
            combineMaxShear->SetLoadFactor(LoadCombiner::dwMin,limitState,lfDWmin);

            combineMinMoment->SetLoadFactor(LoadCombiner::dwMax,limitState,lfDWmax);
            combineMaxMoment->SetLoadFactor(LoadCombiner::dwMax,limitState,lfDWmax);
            combineMinShear->SetLoadFactor(LoadCombiner::dwMax,limitState,lfDWmax);
            combineMaxShear->SetLoadFactor(LoadCombiner::dwMax,limitState,lfDWmax);

            combineMinMoment->SetLoadFactor(LoadCombiner::llim,limitState,lfLL);
            combineMaxMoment->SetLoadFactor(LoadCombiner::llim,limitState,lfLL);
            combineMinShear->SetLoadFactor(LoadCombiner::llim,limitState,lfLL);
            combineMaxShear->SetLoadFactor(LoadCombiner::llim,limitState,lfLL);

            combineMinMoment->SetLiveloadDistFactor(gNegMoment);
            combineMaxMoment->SetLiveloadDistFactor(gPosMoment);
            combineMinShear->SetLiveloadDistFactor(gShear);
            combineMaxShear->SetLiveloadDistFactor(gShear);

            QMinMoment = combineMinMoment->CombineLoads(limitState);
            QMaxMoment = combineMaxMoment->CombineLoads(limitState);
            QMinShear  = combineMinShear->CombineLoads(limitState);
            QMaxShear  = combineMaxShear->CombineLoads(limitState);

            if (face == mbrLeftFace)
               {
               lfMinMoment = QMinMoment;
               lfMaxMoment = QMaxMoment;
               lfMinShear  = QMinShear;
               lfMaxShear  = QMaxShear;
               }
            else
               {
               rfMinMoment = QMinMoment;
               rfMaxMoment = QMaxMoment;
               rfMinShear  = QMinShear;
               rfMaxShear  = QMaxShear;
               }

            SHOULD_CONTINUE;

            } // End of mbrFace

         offset = GetSuperstructureOffset(poi->spanId,poi->offset);
         maxMomentEnv->AddResult(poi->id,offset,lfMaxMoment,rfMaxMoment);
         minMomentEnv->AddResult(poi->id,offset,lfMinMoment,rfMinMoment);
         maxShearEnv->AddResult(poi->id,offset,lfMaxShear,rfMaxShear);
         minShearEnv->AddResult(poi->id,offset,lfMinShear,rfMinShear);

         } // end of poi iteration

		   Support* s;
		   for ( s = firstSupport; s != NULL; s = s->NextSupport() )
		   {
   			int pierId = s->GetID();

            // Get Dead Loads
            combineFxMin->SetLoading(LoadCombiner::loadingDC,GetReaction(pierId,dcDC,rtFx));
            combineFxMin->SetLoading(LoadCombiner::loadingDW,GetReaction(pierId,dcDW,rtFx));
            combineFyMin->SetLoading(LoadCombiner::loadingDC,GetReaction(pierId,dcDC,rtFy));
            combineFyMin->SetLoading(LoadCombiner::loadingDW,GetReaction(pierId,dcDW,rtFy));
            combineMzMin->SetLoading(LoadCombiner::loadingDC,GetReaction(pierId,dcDC,rtMz));
            combineMzMin->SetLoading(LoadCombiner::loadingDW,GetReaction(pierId,dcDW,rtMz));

            combineFxMax->SetLoading(LoadCombiner::loadingDC,GetReaction(pierId,dcDC,rtFx));
            combineFxMax->SetLoading(LoadCombiner::loadingDW,GetReaction(pierId,dcDW,rtFx));
            combineFyMax->SetLoading(LoadCombiner::loadingDC,GetReaction(pierId,dcDC,rtFy));
            combineFyMax->SetLoading(LoadCombiner::loadingDW,GetReaction(pierId,dcDW,rtFy));
            combineMzMax->SetLoading(LoadCombiner::loadingDC,GetReaction(pierId,dcDC,rtMz));
            combineMzMax->SetLoading(LoadCombiner::loadingDW,GetReaction(pierId,dcDW,rtMz));

            span = TYPESAFE_DOWNCAST(s->RightSpan(),LRFDSpan);
            int segmentIdx = 0;
            if ( span == NULL )
            {
            	span = TYPESAFE_DOWNCAST(s->LeftSpan(),LRFDSpan);
               segmentIdx = span->NumSegments()-1;
            }
            lldf = span->FindLLDistFactors(segmentIdx);

            switch(dc)
               {
               case dcStrengthI:
                    limitState = LoadCombiner::StrengthI;
                    nd = span->GetDuctilityFactor(lsStrength);
                    nr = span->GetRedundancyFactor(lsStrength);
                    ni = span->GetImportanceFactor(lsStrength);

                    combineFxMin->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtFxMin));
                    combineFxMin->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtFxMin));
                    combineFyMin->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtFyMin));
                    combineFyMin->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtFyMin));
                    combineMzMin->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtMzMin));
                    combineMzMin->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtMzMin));

                    combineFxMax->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtFxMax));
                    combineFxMax->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtFxMax));
                    combineFyMax->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtFyMax));
                    combineFyMax->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtFyMax));
                    combineMzMax->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtMzMax));
                    combineMzMax->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtMzMax));

                    GetStrengthILoadFactors(lfDCmin,lfDCmax,lfDWmin,lfDWmax,lfLL);
                    gShear     = lldf->s_gShear;
                    break;

               case dcServiceI:
                    limitState = LoadCombiner::ServiceI;
                    nd = span->GetDuctilityFactor(lsService);
                    nr = span->GetRedundancyFactor(lsService);
                    ni = span->GetImportanceFactor(lsService);

                    combineFxMin->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtFxMin));
                    combineFxMin->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtFxMin));
                    combineFyMin->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtFyMin));
                    combineFyMin->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtFyMin));
                    combineMzMin->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtMzMin));
                    combineMzMin->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtMzMin));

                    combineFxMax->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtFxMax));
                    combineFxMax->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtFxMax));
                    combineFyMax->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtFyMax));
                    combineFyMax->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtFyMax));
                    combineMzMax->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtMzMax));
                    combineMzMax->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtMzMax));

                    GetServiceILoadFactors(lfDCmin,lfDWmin,lfLL);
                    lfDCmax = lfDCmin;
                    lfDWmax = lfDWmin;
                    gShear     = lldf->s_gShear;
                    break;

               case dcServiceII:
                    limitState = LoadCombiner::ServiceII;
                    nd = span->GetDuctilityFactor(lsService);
                    nr = span->GetRedundancyFactor(lsService);
                    ni = span->GetImportanceFactor(lsService);

                    combineFxMin->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtFxMin));
                    combineFxMin->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtFxMin));
                    combineFyMin->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtFyMin));
                    combineFyMin->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtFyMin));
                    combineMzMin->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtMzMin));
                    combineMzMin->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtMzMin));

                    combineFxMax->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtFxMax));
                    combineFxMax->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtFxMax));
                    combineFyMax->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtFyMax));
                    combineFyMax->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtFyMax));
                    combineMzMax->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtMzMax));
                    combineMzMax->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtMzMax));

                    GetServiceIILoadFactors(lfDCmin,lfDWmin,lfLL);
                    lfDCmax = lfDCmin;
                    lfDWmax = lfDWmin;
                    gShear     = lldf->s_gShear;
                    break;

               case dcServiceIII:
                    limitState = LoadCombiner::ServiceIII;
                    nd = span->GetDuctilityFactor(lsService);
                    nr = span->GetRedundancyFactor(lsService);
                    ni = span->GetImportanceFactor(lsService);

                    combineFxMin->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtFxMin));
                    combineFxMin->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtFxMin));
                    combineFyMin->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtFyMin));
                    combineFyMin->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtFyMin));
                    combineMzMin->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtMzMin));
                    combineMzMin->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtMzMin));

                    combineFxMax->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtFxMax));
                    combineFxMax->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtFxMax));
                    combineFyMax->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtFyMax));
                    combineFyMax->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtFyMax));
                    combineMzMax->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcAll,rtMzMax));
                    combineMzMax->SetLoading(LoadCombiner::loadingPL,GetReaction(pierId,dcPedestrian,rtMzMax));

                    GetServiceIIILoadFactors(lfDCmin,lfDWmin,lfLL);
                    lfDCmax = lfDCmin;
                    lfDWmax = lfDWmin;
                    gShear     = lldf->s_gShear;
                    break;

               case dcFatigue:
                    limitState = LoadCombiner::Fatigue;
                    nd = span->GetDuctilityFactor(lsFatigue);
                    nr = span->GetRedundancyFactor(lsFatigue);
                    ni = span->GetImportanceFactor(lsFatigue);

                    GetFatigueLoadFactors(lfDCmin,lfDWmin,lfLL);
                    lfDCmax = lfDCmin;
                    lfDWmax = lfDWmin;

                    combineFxMin->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcFatigueTruck,rtFxMin));
                    combineFxMin->SetLoading(LoadCombiner::loadingPL,0);
                    combineFyMin->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcFatigueTruck,rtFyMin));
                    combineFyMin->SetLoading(LoadCombiner::loadingPL,0);
                    combineMzMin->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcFatigueTruck,rtMzMin));
                    combineMzMin->SetLoading(LoadCombiner::loadingPL,0);

                    combineFxMax->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcFatigueTruck,rtFxMax));
                    combineFxMax->SetLoading(LoadCombiner::loadingPL,0);
                    combineFyMax->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcFatigueTruck,rtFyMax));
                    combineFyMax->SetLoading(LoadCombiner::loadingPL,0);
                    combineMzMax->SetLoading(LoadCombiner::loadingLLIM,GetReaction(pierId,dcFatigueTruck,rtMzMax));
                    combineMzMax->SetLoading(LoadCombiner::loadingPL,0);

                    gShear     = lldf->f_gShear;
                    break;
               }

            TRACEX(LRFDBridge_Combinations,LEVEL2,
               "Combining Loads for " << desc << " at pier " << pierId);


            combineFxMin->SetLimitStateFactors(nd,nr,ni);
            combineFyMin->SetLimitStateFactors(nd,nr,ni);
            combineMzMin->SetLimitStateFactors(nd,nr,ni);

            combineFxMax->SetLimitStateFactors(nd,nr,ni);
            combineFyMax->SetLimitStateFactors(nd,nr,ni);
            combineMzMax->SetLimitStateFactors(nd,nr,ni);

            combineFxMin->SetLoadFactor(LoadCombiner::dcMin,limitState,lfDCmin);
            combineFyMin->SetLoadFactor(LoadCombiner::dcMin,limitState,lfDCmin);
            combineMzMin->SetLoadFactor(LoadCombiner::dcMin,limitState,lfDCmin);
            combineFxMax->SetLoadFactor(LoadCombiner::dcMin,limitState,lfDCmin);
            combineFyMax->SetLoadFactor(LoadCombiner::dcMin,limitState,lfDCmin);
            combineMzMax->SetLoadFactor(LoadCombiner::dcMin,limitState,lfDCmin);

            combineFxMin->SetLoadFactor(LoadCombiner::dcMax,limitState,lfDCmax);
            combineFyMin->SetLoadFactor(LoadCombiner::dcMax,limitState,lfDCmax);
            combineMzMin->SetLoadFactor(LoadCombiner::dcMax,limitState,lfDCmax);
            combineFxMax->SetLoadFactor(LoadCombiner::dcMax,limitState,lfDCmax);
            combineFyMax->SetLoadFactor(LoadCombiner::dcMax,limitState,lfDCmax);
            combineMzMax->SetLoadFactor(LoadCombiner::dcMax,limitState,lfDCmax);

            combineFxMin->SetLoadFactor(LoadCombiner::dwMin,limitState,lfDWmin);
            combineFyMin->SetLoadFactor(LoadCombiner::dwMin,limitState,lfDWmin);
            combineMzMin->SetLoadFactor(LoadCombiner::dwMin,limitState,lfDWmin);
            combineFxMax->SetLoadFactor(LoadCombiner::dwMin,limitState,lfDWmin);
            combineFyMax->SetLoadFactor(LoadCombiner::dwMin,limitState,lfDWmin);
            combineMzMax->SetLoadFactor(LoadCombiner::dwMin,limitState,lfDWmin);

            combineFxMin->SetLoadFactor(LoadCombiner::dwMax,limitState,lfDWmax);
            combineFyMin->SetLoadFactor(LoadCombiner::dwMax,limitState,lfDWmax);
            combineMzMin->SetLoadFactor(LoadCombiner::dwMax,limitState,lfDWmax);
            combineFxMax->SetLoadFactor(LoadCombiner::dwMax,limitState,lfDWmax);
            combineFyMax->SetLoadFactor(LoadCombiner::dwMax,limitState,lfDWmax);
            combineMzMax->SetLoadFactor(LoadCombiner::dwMax,limitState,lfDWmax);

            combineFxMin->SetLoadFactor(LoadCombiner::llim,limitState,lfLL);
            combineFyMin->SetLoadFactor(LoadCombiner::llim,limitState,lfLL);
            combineMzMin->SetLoadFactor(LoadCombiner::llim,limitState,lfLL);
            combineFxMax->SetLoadFactor(LoadCombiner::llim,limitState,lfLL);
            combineFyMax->SetLoadFactor(LoadCombiner::llim,limitState,lfLL);
            combineMzMax->SetLoadFactor(LoadCombiner::llim,limitState,lfLL);

            combineFxMin->SetLiveloadDistFactor(gShear);
            combineFyMin->SetLiveloadDistFactor(gShear);
            combineMzMin->SetLiveloadDistFactor(gShear);
            combineFxMax->SetLiveloadDistFactor(gShear);
            combineFyMax->SetLiveloadDistFactor(gShear);
            combineMzMax->SetLiveloadDistFactor(gShear);

            QFxMin = combineFxMin->CombineLoads(limitState);
            QFyMin = combineFyMin->CombineLoads(limitState);
            QMzMin = combineMzMin->CombineLoads(limitState);
            QFxMax = combineFxMax->CombineLoads(limitState);
            QFyMax = combineFyMax->CombineLoads(limitState);
            QMzMax = combineMzMax->CombineLoads(limitState);

	         minReactionEnv->AddSupportResult(pierId,QFxMin,QFyMin,QMzMin);
   	      maxReactionEnv->AddSupportResult(pierId,QFxMax,QFyMax,QMzMax);
		   } // end of support loop

      } // end of dc loop

   delete combineMinMoment;
   delete combineMaxMoment;
   delete combineMinShear;
   delete combineMaxShear;
   delete combineFxMin;
   delete combineFyMin;
   delete combineMzMin;
   delete combineFxMax;
   delete combineFyMax;
   delete combineMzMax;
   return true;
}

