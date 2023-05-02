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

#include "lrfdbrdg.h"

#include <float.h>

#if !defined (FEMMODEL_H_)
#include "femmodel.h"
#endif

#if !defined (LRFDSPAN_H)
#include "lrfdspan.h"
#endif

void LRFDBridge::SetupTables(TISResultsContainer *maxMenvelope,
                             TISResultsContainer *minMenvelope,
                             TISResultsContainer *maxVenvelope,
                             TISResultsContainer *minVenvelope)
{
   maxMenvelope->Flush();
   minMenvelope->Flush();
   maxVenvelope->Flush();
   minVenvelope->Flush();
   ResultsData *result;

   POIDataIterator j(*POIs);
   while(j)
      {
      POIData *data = j++;

      result = new ResultsData;
      result->poi = data->id;
      result->x = data->x;
      result->lfValue = 0;
      result->rfValue = 0;
      maxMenvelope->Add(result);

      result = new ResultsData;
      result->poi = data->id;
      result->x = data->x;
      result->lfValue = 0;
      result->rfValue = 0;
      minMenvelope->Add(result);

      result = new ResultsData;
      result->poi = data->id;
      result->x = data->x;
      result->lfValue = 0;
      result->rfValue = 0;
      maxVenvelope->Add(result);

      result = new ResultsData;
      result->poi = data->id;
      result->x = data->x;
      result->lfValue = 0;
      result->rfValue = 0;
      minVenvelope->Add(result);
      }
}

void LRFDBridge::SaveDeadLoads(int ldId)
{
   double lfFx,lfFy,lfMz;
   double rfFx,rfFy,rfMz;

   TISResultsContainer *moments = (ldId == dcId) ? dcMoments : dwMoments;
   TISResultsContainer *shears  = (ldId == dcId) ? dcShears  : dwShears;

   POIDataIterator j(*POIs);
   while(j)
      {
      POIData *data = j++;
      femModel->GetPOIForces(data->id,ldId,mbrRightFace,rfFx,rfFy,rfMz);

      // RightFace = -1 * LeftFace
      // Convert all results to "LeftFace-like" values to provide
      // consistancy
      rfFx *= -1.;
      rfFy *= -1.;
      rfMz *= -1.;

      femModel->GetPOIForces(data->id,ldId,mbrLeftFace,lfFx,lfFy,lfMz);

      ResultsData *moment = new ResultsData;
      moment->poi = data->id;
      moment->x = data->x;
      moment->lfValue = lfMz;
      moment->rfValue = rfMz;
      moments->Add(moment);

      ResultsData *shear = new ResultsData;
      shear->poi = data->id;
      shear->x = data->x;
      shear->lfValue = -lfFy;
      shear->rfValue = -rfFy;
      shears->Add(shear);
      }
}

void LRFDBridge::GenerateLiveloadEnvelopeTables()
{
   BuildTandemTruckEnvelopeTable();
   BuildVariableTruckEnvelopeTable();
   BuildDualTruckEnvelopeTable();
   BuildLiveLoadEnvelopeTable();

   BuildFatigueTruckEnvelopeTable();
   BuildPedestrianEnvelopeTable();
}

void LRFDBridge::BuildTandemTruckEnvelopeTable()
{
   Truck *truck;
   TruckTrain *truckTrain;
   TruckTrainContainer *trains;

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

   CreateLiveloadEnvelopeTable(trains,inflLines,
                               tandemMaxMoment,
                               tandemMinMoment,
                               tandemMaxShear,
                               tandemMinShear);


   delete trains;
   delete truckTrain;
   delete truck;
}

void LRFDBridge::BuildVariableTruckEnvelopeTable()
{
   Truck *truck;
   TruckTrain *truckTrain;
   TruckTrainContainer *trains;
   int inc;
   double rearAxleSpacing;

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

   CreateLiveloadEnvelopeTable(trains,inflLines,
                               truckMaxMoment,
                               truckMinMoment,
                               truckMaxShear,
                               truckMinShear);


   delete trains;
   delete truckTrain;
}

void LRFDBridge::BuildDualTruckEnvelopeTable()
{
   Truck *truck;
   TruckTrain *truckTrain;
   TruckTrainContainer *trains;
   InflLineDataContainer *il;
   Diagram *dcMoment;
   int inc;
   int range;
   double headway;
   Span *span;
   int poiOffset;
   int poi;

   SetupTables(trkTrainMaxMoment,trkTrainMinMoment,trkTrainMaxShear,trkTrainMinShear);

   // Set unused tables to zero
   for (span = GetFirstSpan(); span; span = span->NextSpan())
      {
      for (poiOffset = 0; poiOffset < minAnalysisPoints; poiOffset++)
         {
         poi = SpanPntToPOI(span->GetID(),poiOffset);

         ResultsData *minShearData = (*trkTrainMinShear)[poi];
         ResultsData *maxShearData = (*trkTrainMaxShear)[poi];
         ResultsData *maxMomentData = (*trkTrainMaxMoment)[poi];

         minShearData->lfValue = 0;
         minShearData->rfValue = 0;
         maxShearData->lfValue = 0;
         maxShearData->rfValue = 0;
         maxMomentData->lfValue = 0;
         maxMomentData->rfValue = 0;

         if (!DualTruckTrainEnabled())
            {
            ResultsData *minMomentData = (*trkTrainMinMoment)[poi];
            minMomentData->lfValue = 0;
            minMomentData->rfValue = 0;
            }
         }
      }

   if (!DualTruckTrainEnabled())
      {
      // Dual trucks not considered
      return;
      }

   // Find all the influence lines that occur where DC is less than zero
   il = new InflLineDataContainer(10,0,10);
   InflLineDataIterator iter(*inflLines);
   dcMoment = GetDiagram(GetDiagramId(dcDC,dtMoment));
   bool inRange = false;
   InflLineData *prev = NULL;
   while(iter)
      {
      InflLineData *inflLineData = iter++;
      InflLineData *temp = new InflLineData(inflLineData->id,inflLineData->x);
      InflLineData *temp2;
      temp->shear->AddPoint(0,0);
      temp->shear->AddPoint(Length(),0);
      if (dcMoment->Evaluate(inflLineData->x) < 0)
         {
         // Copy the influence line
         if (!inRange && inflLineData->x-1.0e-04 >= 0)
            {
            // just now coming into range
            // add a dummy influence line 1.0e-04 before this point
            temp2 = new InflLineData(temp->id,inflLineData->x-1.0e-04);
            temp2->shear->AddPoint(0,0);
            temp2->shear->AddPoint(Length(),0);
            temp2->moment->AddPoint(0,0);
            temp2->moment->AddPoint(Length(),0);
            il->Add(temp2);
            }
         inRange = true;
         delete temp->moment;
         temp->moment = new InfluenceLine(temp->id,*inflLineData->moment);
         }
      else
         {
         if (inRange && prev->x+1.0e-04 <= Length())
            {
            // just now going into range
            // add a dummy influence line 1.0e-04 after this point
            temp2 = new InflLineData(temp->id,prev->x+1.0e-04);
            temp2->shear->AddPoint(0,0);
            temp2->shear->AddPoint(Length(),0);
            temp2->moment->AddPoint(0,0);
            temp2->moment->AddPoint(Length(),0);
            il->Add(temp2);
            }
         inRange = false;
         temp->moment->AddPoint(0,0);
         temp->moment->AddPoint(Length(),0);
         }
      prev = inflLineData;
      il->Add(temp);
      }

   // Setup container of truck trains
   trains = new TruckTrainContainer(2,0,2);
   trains->OwnsElements(false);

   // Setup trucks
   truck = new Truck(1);
   truck->AddAxle(0.000,0.90 * 35000.);
   truck->AddAxle(4.300,0.90 * 145000.);
   truck->AddAxle(4.300,0.90 * 145000.);

   // Setup truck trains
   for (range = 0; range < 3; range++)
      {
      if (!dualTruckRangeEnabled[range])
         continue;

      for (inc = 0; inc < numHeadwayInc[range]; inc++)
         {
         if (numHeadwayInc[range] == 1)
            headway = fromHeadwaySpacing[range];
         else
            headway = (toHeadwaySpacing[range] - fromHeadwaySpacing[range])*inc/(numHeadwayInc[range]-1) + fromHeadwaySpacing[range];

         truckTrain = new TruckTrain(DUALTRUCKS);
         truckTrain->SetLaneLoad(0.90 * 9300.);
         truckTrain->SetTruckImpact(GetTruckImpact());
         truckTrain->SetLaneImpact(GetLaneImpact());
         truckTrain->AddTruck(*truck,0.);
         truckTrain->AddTruck(*truck,headway);
         trains->Add(truckTrain);
         }
      }

   CreateLiveloadEnvelopeTable(trains,il,
                               NULL,
                               trkTrainMinMoment,
                               NULL,
                               NULL);

   delete trains;
   delete truckTrain;
   delete truck;
}

void LRFDBridge::BuildLiveLoadEnvelopeTable()
{
// Envelope of envelopes...
   SetupEnvelopeTables(llMaxMoment,llMinMoment,llMaxShear,llMinShear);
   double value;
   Span *span;
   int poiOffset;
   int poi;

   for (span = GetFirstSpan(); span; span = span->NextSpan())
      {
      for (poiOffset = 0; poiOffset < minAnalysisPoints; poiOffset++)
         {
         poi = SpanPntToPOI(span->GetID(),poiOffset);

         ResultsData *minShearData = (*llMinShear)[poi];
         ResultsData *maxShearData = (*llMaxShear)[poi];
         ResultsData *minMomentData = (*llMinMoment)[poi];
         ResultsData *maxMomentData = (*llMaxMoment)[poi];

         // Min Moments
         value = GetResult(span->GetID(),poiOffset,mbrLeftFace,dcDesignTandem,dtMinMoment);
         if (value < minMomentData->lfValue)
            minMomentData->lfValue = value;

         value = GetResult(span->GetID(),poiOffset,mbrRightFace,dcDesignTandem,dtMinMoment);
         if (value < minMomentData->rfValue)
            minMomentData->rfValue = value;

         value = GetResult(span->GetID(),poiOffset,mbrLeftFace,dcDesignTruck,dtMinMoment);
         if (value < minMomentData->lfValue)
            minMomentData->lfValue = value;

         value = GetResult(span->GetID(),poiOffset,mbrRightFace,dcDesignTruck,dtMinMoment);
         if (value < minMomentData->rfValue)
            minMomentData->rfValue = value;

         value = GetResult(span->GetID(),poiOffset,mbrLeftFace,dcDualTruckTrain,dtMinMoment);
         if (value < minMomentData->lfValue)
            minMomentData->lfValue = value;

         value = GetResult(span->GetID(),poiOffset,mbrRightFace,dcDualTruckTrain,dtMinMoment);
         if (value < minMomentData->rfValue)
            minMomentData->rfValue = value;

         // Max Moments
         value = GetResult(span->GetID(),poiOffset,mbrLeftFace,dcDesignTandem,dtMaxMoment);
         if (value > maxMomentData->lfValue)
            maxMomentData->lfValue = value;

         value = GetResult(span->GetID(),poiOffset,mbrRightFace,dcDesignTandem,dtMaxMoment);
         if (value > maxMomentData->rfValue)
            maxMomentData->rfValue = value;

         value = GetResult(span->GetID(),poiOffset,mbrLeftFace,dcDesignTruck,dtMaxMoment);
         if (value > maxMomentData->lfValue)
            maxMomentData->lfValue = value;

         value = GetResult(span->GetID(),poiOffset,mbrRightFace,dcDesignTruck,dtMaxMoment);
         if (value > maxMomentData->rfValue)
            maxMomentData->rfValue = value;

         value = GetResult(span->GetID(),poiOffset,mbrLeftFace,dcDualTruckTrain,dtMaxMoment);
         if (value > maxMomentData->lfValue)
            maxMomentData->lfValue = value;

         value = GetResult(span->GetID(),poiOffset,mbrRightFace,dcDualTruckTrain,dtMaxMoment);
         if (value > maxMomentData->rfValue)
            maxMomentData->rfValue = value;


         // Min Shears
         value = GetResult(span->GetID(),poiOffset,mbrLeftFace,dcDesignTandem,dtMinShear);
         if (value < minShearData->lfValue)
            minShearData->lfValue = value;

         value = GetResult(span->GetID(),poiOffset,mbrRightFace,dcDesignTandem,dtMinShear);
         if (value < minShearData->rfValue)
            minShearData->rfValue = value;

         value = GetResult(span->GetID(),poiOffset,mbrLeftFace,dcDesignTruck,dtMinShear);
         if (value < minShearData->lfValue)
            minShearData->lfValue = value;

         value = GetResult(span->GetID(),poiOffset,mbrRightFace,dcDesignTruck,dtMinShear);
         if (value < minShearData->rfValue)
            minShearData->rfValue = value;

         value = GetResult(span->GetID(),poiOffset,mbrLeftFace,dcDualTruckTrain,dtMinShear);
         if (value < minShearData->lfValue)
            minShearData->lfValue = value;

         value = GetResult(span->GetID(),poiOffset,mbrRightFace,dcDualTruckTrain,dtMinShear);
         if (value < minShearData->rfValue)
            minShearData->rfValue = value;

         // Max Shears
         value = GetResult(span->GetID(),poiOffset,mbrLeftFace,dcDesignTandem,dtMaxShear);
         if (value > maxShearData->lfValue)
            maxShearData->lfValue = value;

         value = GetResult(span->GetID(),poiOffset,mbrRightFace,dcDesignTandem,dtMaxShear);
         if (value > maxShearData->rfValue)
            maxShearData->rfValue = value;

         value = GetResult(span->GetID(),poiOffset,mbrLeftFace,dcDesignTruck,dtMaxShear);
         if (value > maxShearData->lfValue)
            maxShearData->lfValue = value;

         value = GetResult(span->GetID(),poiOffset,mbrRightFace,dcDesignTruck,dtMaxShear);
         if (value > maxShearData->rfValue)
            maxShearData->rfValue = value;

         value = GetResult(span->GetID(),poiOffset,mbrLeftFace,dcDualTruckTrain,dtMaxShear);
         if (value > maxShearData->lfValue)
            maxShearData->lfValue = value;

         value = GetResult(span->GetID(),poiOffset,mbrRightFace,dcDualTruckTrain,dtMaxShear);
         if (value > maxShearData->rfValue)
            maxShearData->rfValue = value;
         }
      }

}

void LRFDBridge::BuildFatigueTruckEnvelopeTable()
{
   Truck *truck;
   TruckTrain *truckTrain;
   TruckTrainContainer *trains;

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
   trains->OwnsElements(0); // No
   trains->Add(truckTrain);

   CreateLiveloadEnvelopeTable(trains,inflLines,
                               fatigueTrkMaxMoment,
                               fatigueTrkMinMoment,
                               fatigueTrkMaxShear,
                               fatigueTrkMinShear);


   delete trains;
   delete truckTrain;
   delete truck;
}

void LRFDBridge::BuildPedestrianEnvelopeTable()
{
   TruckTrain *truckTrain;
   TruckTrainContainer *trains;
   Truck *truck;

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
   trains->OwnsElements(0); // No
   trains->Add(truckTrain);

   CreateLiveloadEnvelopeTable(trains,inflLines,
                               pedestrianMaxMoment,
                               pedestrianMinMoment,
                               pedestrianMaxShear,
                               pedestrianMinShear);


   delete trains;
   delete truckTrain;
   delete truck;
}

void LRFDBridge::CreateLiveloadEnvelopeTable(TruckTrainContainer *trains,
                                             InflLineDataContainer *il,
                                             TISResultsContainer *maxMenvelope,
                                             TISResultsContainer *minMenvelope,
                                             TISResultsContainer *maxVenvelope,
                                             TISResultsContainer *minVenvelope)
{
   TruckTrain *train;
   Truck *truck;
   int poiOffset;
   double position;
   double x,y;
   Span *span;
   int pivotTruck;
   int pivotAxle;
   int numTrucks;
   int numAxles;

   TruckTrainIterator iter(*trains);

   SetupEnvelopeTables(maxMenvelope,minMenvelope,maxVenvelope,minVenvelope);

   while(iter)
      {
      train = iter++;
      numTrucks = train->GetNumTrucks();

      for (span = GetFirstSpan(); span; span = span->NextSpan())
         {
         span->GetStartPoint(x,y);

         InflLineDataIterator i(*il);
         while(i)
            {
            double moment;
            double shear;
            InflLineData *inflLineData = i++;
            InfluenceLine *inflLine;

            for (poiOffset = 0; poiOffset < minAnalysisPoints; poiOffset++)
               {
               position = x + span->Length()*poiOffset/(minAnalysisPoints-1);

               CHECKX(numTrucks > 0,"LRFDBridge::CreateLiveloadEnvelopeTable - NumTrucks must be > 0");

               for (pivotTruck = 0; pivotTruck < numTrucks; pivotTruck++)
                  {
                  truck = train->GetTruck(pivotTruck);
                  numAxles = truck->GetNumAxles();

                  for (pivotAxle = 0; pivotAxle < numAxles; pivotAxle++)
                     {
                     train->SetPivot(pivotTruck,pivotAxle);

                     train->MoveTo(position);

                     if (maxMenvelope)
                        {
                        ResultsData *maxMomentData = (*maxMenvelope)[inflLineData->id];
                        //
                        // Max Moment
                        //
                        inflLine = inflLineData->moment;
                        inflLine->SetMode(InfluenceLine::ExcludeNegValues);

                        // Forward
                        train->SetDirection(TruckTrain::Forward);
                        moment = train->Evaluate(*inflLine);
                        if (moment > maxMomentData->lfValue)
                           maxMomentData->lfValue = moment;
                        if (moment > maxMomentData->rfValue)
                           maxMomentData->rfValue = moment;

                        // Backward
                        train->SetDirection(TruckTrain::Backward);
                        moment = train->Evaluate(*inflLine);
                        if (moment > maxMomentData->lfValue)
                           maxMomentData->lfValue = moment;
                        if (moment > maxMomentData->rfValue)
                           maxMomentData->rfValue = moment;
                        }

                     if (minMenvelope)
                        {
                        ResultsData *minMomentData = (*minMenvelope)[inflLineData->id];
                        //
                        // Min Moment
                        //
                        inflLine = inflLineData->moment;
                        inflLine->SetMode(InfluenceLine::ExcludePosValues);

                        // Forward
                        train->SetDirection(TruckTrain::Forward);
                        moment = train->Evaluate(*inflLine);
                        if (moment < minMomentData->lfValue)
                           minMomentData->lfValue = moment;
                        if (moment < minMomentData->rfValue)
                           minMomentData->rfValue = moment;

                        // Backward
                        train->SetDirection(TruckTrain::Backward);
                        moment = train->Evaluate(*inflLine);
                        if (moment < minMomentData->lfValue)
                           minMomentData->lfValue = moment;
                        if (moment < minMomentData->rfValue)
                           minMomentData->rfValue = moment;
                        }

                     if (maxVenvelope)
                        {
                        ResultsData *maxShearData = (*maxVenvelope)[inflLineData->id];
                        //
                        // Max Shear
                        //
                        inflLine = inflLineData->shear;
                        inflLine->SetMode(InfluenceLine::ExcludeNegValues);

                        // Forward
                        train->SetDirection(TruckTrain::Forward);
                        shear = train->Evaluate(*inflLine);
                        if (shear > maxShearData->lfValue)
                           maxShearData->lfValue = shear;
                        if (shear > maxShearData->rfValue)
                           maxShearData->rfValue = shear;

                        // Backward
                        train->SetDirection(TruckTrain::Backward);
                        shear = train->Evaluate(*inflLine);
                        if (shear > maxShearData->lfValue)
                           maxShearData->lfValue = shear;
                        if (shear > maxShearData->rfValue)
                           maxShearData->rfValue = shear;
                        }

                     if (minVenvelope)
                        {
                        ResultsData *minShearData = (*minVenvelope)[inflLineData->id];
                        //
                        // Min Shear
                        //
                        inflLine = inflLineData->shear;
                        inflLine->SetMode(InfluenceLine::ExcludePosValues);

                        // Forward
                        train->SetDirection(TruckTrain::Forward);
                        shear = train->Evaluate(*inflLine);
                        if (shear < minShearData->lfValue)
                           minShearData->lfValue = shear;
                        if (shear < minShearData->rfValue)
                           minShearData->rfValue = shear;

                        // Backward
                        train->SetDirection(TruckTrain::Backward);
                        shear = train->Evaluate(*inflLine);
                        if (shear < minShearData->lfValue)
                           minShearData->lfValue = shear;
                        if (shear < minShearData->rfValue)
                           minShearData->rfValue = shear;
                        }
                     }
                  }
               }
            }
         }
      }
}

void LRFDBridge::SetupEnvelopeTables(TISResultsContainer *maxMenvelope,
                                     TISResultsContainer *minMenvelope,
                                     TISResultsContainer *maxVenvelope,
                                     TISResultsContainer *minVenvelope)
{
   if (maxMenvelope)
      maxMenvelope->Flush();

   if (minMenvelope)
      minMenvelope->Flush();

   if (maxVenvelope)
      maxVenvelope->Flush();

   if (minVenvelope)
      minVenvelope->Flush();

   Span *span;
   int poiOffset;
   int poi;
   double x,y;
   double position;
   double maxValue = -DBL_MAX;
   double minValue =  DBL_MAX;
   ResultsData *data;

   for (span = GetFirstSpan(); span; span = span->NextSpan())
      {
      span->GetStartPoint(x,y);
      for (poiOffset = 0; poiOffset < minAnalysisPoints; poiOffset++)
         {
         poi = SpanPntToPOI(span->GetID(),poiOffset);
         position = x + span->Length()*poiOffset/(minAnalysisPoints-1);

         if (maxMenvelope)
            {
            data = new ResultsData;
            data->poi = poi;
            data->x = position;
            data->lfValue = maxValue;
            data->rfValue = maxValue;
            maxMenvelope->Add(data);
            }

         if (minMenvelope)
            {
            data = new ResultsData;
            data->poi = poi;
            data->x = position;
            data->lfValue = minValue;
            data->rfValue = minValue;
            minMenvelope->Add(data);
            }

         if (maxVenvelope)
            {
            data = new ResultsData;
            data->poi = poi;
            data->x = position;
            data->lfValue = maxValue;
            data->rfValue = maxValue;
            maxVenvelope->Add(data);
            }

         if (minVenvelope)
            {
            data = new ResultsData;
            data->poi = poi;
            data->x = position;
            data->lfValue = minValue;
            data->rfValue = minValue;
            minVenvelope->Add(data);
            }
         }
      }
}

void LRFDBridge::CreateLRFDCombinationTables()
{
   int spanId;
   int poiOffset;
   double n;         // limit state factor
   double QPMoment;  // max moment force effect
   double QNMoment;  // min moment force effect
   double QPShear;   // max shear force effect
   double QNShear;   // min shear force effect
   double DCmoment;  // moment from DC loading
   double DCshear;   // shear from DC loading
   double DWmoment;  // moment from DW loading
   double DWshear;   // shear from DW loading
   double LLPMoment; // moment from live load
   double LLNMoment; // moment from live load
   double LLPShear;  // shear from live load
   double LLNShear;  // shear from live load
   double PLPMoment; // moment from pedestrian load
   double PLNMoment; // moment from pedestrian load
   double PLPShear;  // shear from pedestrian load
   double PLNShear;  // shear from pedestrian load
   double FLPMoment; // moment from fatigue truck
   double FLNMoment; // moment from fatigue truck
   double FLPShear;  // shear from fatigue truck
   double FLNShear;  // shear from fatigue truck
   double lfDCmin;   // load factor for DC (min)
   double lfDCmax;   // load factor for DC (max)
   double lfDWmin;   // load factor for DW (min)
   double lfDWmax;   // load factor for DW (max)
   double lfLL;      // load factor for liveload
   Diagram *dcMoment;
   Diagram *dcShear;
   Diagram *dwMoment;
   Diagram *dwShear;
   Diagram *llPMoment;
   Diagram *llNMoment;
   Diagram *llPShear;
   Diagram *llNShear;
   Diagram *pedPMoment;
   Diagram *pedNMoment;
   Diagram *pedPShear;
   Diagram *pedNShear;
   Diagram *fatPMoment;
   Diagram *fatNMoment;
   Diagram *fatPShear;
   Diagram *fatNShear;

   //
   // Setup tables
   //
   SetupTables(strengthIMaxMoment,strengthIMinMoment,strengthIMaxShear,strengthIMinShear);
   SetupTables(serviceIMaxMoment,serviceIMinMoment,serviceIMaxShear,serviceIMinShear);
   SetupTables(serviceIIMaxMoment,serviceIIMinMoment,serviceIIMaxShear,serviceIIMinShear);
   SetupTables(serviceIIIMaxMoment,serviceIIIMinMoment,serviceIIIMaxShear,serviceIIIMinShear);
   SetupTables(fatigueMaxMoment,fatigueMinMoment,fatigueMaxShear,fatigueMinShear);

   //
   // Get diagrams that will be needed
   //
   dcMoment = GetDiagram(GetDiagramId(dcDC,dtMoment));
   dcShear  = GetDiagram(GetDiagramId(dcDC,dtShear));

   dwMoment = GetDiagram(GetDiagramId(dcDW,dtMoment));
   dwShear  = GetDiagram(GetDiagramId(dcDC,dtShear));

   llPMoment = GetDiagram(GetDiagramId(dcAll,dtMaxMoment));
   llNMoment = GetDiagram(GetDiagramId(dcAll,dtMinMoment));
   llPShear  = GetDiagram(GetDiagramId(dcAll,dtMaxShear));
   llNShear  = GetDiagram(GetDiagramId(dcAll,dtMinShear));

   fatPMoment = GetDiagram(GetDiagramId(dcFatigueTruck,dtMaxMoment));
   fatNMoment = GetDiagram(GetDiagramId(dcFatigueTruck,dtMinMoment));
   fatPShear  = GetDiagram(GetDiagramId(dcFatigueTruck,dtMaxShear));
   fatNShear  = GetDiagram(GetDiagramId(dcFatigueTruck,dtMinShear));

   pedPMoment = GetDiagram(GetDiagramId(dcPedestrian,dtMaxMoment));
   pedNMoment = GetDiagram(GetDiagramId(dcPedestrian,dtMinMoment));
   pedPShear  = GetDiagram(GetDiagramId(dcPedestrian,dtMaxShear));
   pedNShear  = GetDiagram(GetDiagramId(dcPedestrian,dtMinShear));


   //
   // Create combinations at each POI
   //
   POIDataIterator j(*POIs);
   while(j)
      {
      POIData *data = j++;
      LRFDSpan *span;
      double startX,startY;
      LLDistFactors *lldf;


      span = TYPESAFE_DOWNCAST(GetSpan(data->x),LRFDSpan);
      span->GetStartPoint(startX,startY);

      lldf = span->FindLLDistFactors(data->x - startX);

      spanId = POItoSpan(data->id);
      poiOffset = POIwithinSpan(data->id);

      for (MbrFace face = mbrLeftFace; face <= mbrRightFace; face++)
         {
         if (data->id >= dcMoments->GetItemsInContainer())
            DCmoment = dcMoment->Evaluate(data->x);
         else
            DCmoment = GetResult(spanId,poiOffset,face,dcDC,dtMoment);

         if (data->id >= dcShears->GetItemsInContainer())
            DCshear = dcShear->Evaluate(data->x);
         else
            DCshear = GetResult(spanId,poiOffset,face,dcDC,dtShear);

         if (data->id >= dwMoments->GetItemsInContainer())
            DWmoment = dwMoment->Evaluate(data->x);
         else
            DWmoment = GetResult(spanId,poiOffset,face,dcDW,dtMoment);

         if (data->id >= dwShears->GetItemsInContainer())
            DWshear  = dwShear->Evaluate(data->x);
         else
            DWshear = GetResult(spanId,poiOffset,face,dcDW,dtShear);

         if (data->id >= llMinMoment->GetItemsInContainer())
            LLNMoment = llNMoment->Evaluate(data->x);
         else
            LLNMoment = GetResult(spanId,poiOffset,face,dcAll,dtMinMoment);

         if (data->id >= llMaxMoment->GetItemsInContainer())
            LLPMoment = llPMoment->Evaluate(data->x);
         else
            LLPMoment = GetResult(spanId,poiOffset,face,dcAll,dtMaxMoment);

         if (data->id >= llMinShear->GetItemsInContainer())
            LLNShear  = llNShear->Evaluate(data->x);
         else
            LLNShear = GetResult(spanId,poiOffset,face,dcAll,dtMinShear);

         if (data->id >= llMaxShear->GetItemsInContainer())
            LLPShear  = llPShear->Evaluate(data->x);
         else
            LLPShear = GetResult(spanId,poiOffset,face,dcAll,dtMaxShear);

         if (data->id >= pedestrianMinMoment->GetItemsInContainer())
            PLNMoment = pedNMoment->Evaluate(data->x);
         else
            PLNMoment = GetResult(spanId,poiOffset,face,dcPedestrian,dtMinMoment);

         if (data->id >= pedestrianMaxMoment->GetItemsInContainer())
            PLPMoment = pedPMoment->Evaluate(data->x);
         else
            PLPMoment = GetResult(spanId,poiOffset,face,dcPedestrian,dtMaxMoment);

         if (data->id >= pedestrianMinShear->GetItemsInContainer())
            PLNShear  = pedNShear->Evaluate(data->x);
         else
            PLNShear = GetResult(spanId,poiOffset,face,dcPedestrian,dtMinShear);

         if (data->id >= pedestrianMaxShear->GetItemsInContainer())
            PLPShear  = pedPShear->Evaluate(data->x);
         else
            PLPShear = GetResult(spanId,poiOffset,face,dcPedestrian,dtMaxShear);

         if (data->id >= fatigueMinMoment->GetItemsInContainer())
            FLNMoment = fatNMoment->Evaluate(data->x);
         else
            FLNMoment = GetResult(spanId,poiOffset,face,dcFatigue,dtMinMoment);

         if (data->id >= fatigueMaxMoment->GetItemsInContainer())
            FLPMoment = fatPMoment->Evaluate(data->x);
         else
            FLPMoment = GetResult(spanId,poiOffset,face,dcFatigue,dtMaxMoment);

         if (data->id >= fatigueMinShear->GetItemsInContainer())
            FLNShear  = fatNShear->Evaluate(data->x);
         else
            FLNShear = GetResult(spanId,poiOffset,face,dcFatigue,dtMaxShear);

         if (data->id >= fatigueMaxShear->GetItemsInContainer())
            FLPShear  = fatPShear->Evaluate(data->x);
         else
            FLPShear = GetResult(spanId,poiOffset,face,dcFatigue,dtMaxShear);

         //
         // Strength Limit States
         //

         n = span->GetLimitStateFactor(lsStrength);

         // Strength I
         GetStrengthILoadFactors(lfDCmin,lfDCmax,lfDWmin,lfDWmax,lfLL);

         QNMoment = CombineLoads(n,lfDCmin,lfDCmax,DCmoment,lfDWmin,lfDWmax,DWmoment,
                                 lfLL,lldf->s_gMoment,LLNMoment,PLNMoment);
         QPMoment = CombineLoads(n,lfDCmin,lfDCmax,DCmoment,lfDWmin,lfDWmax,DWmoment,
                                 lfLL,lldf->s_gMoment,LLPMoment,PLPMoment);
         QNShear = CombineLoads(n,lfDCmin,lfDCmax,DCshear,lfDWmin,lfDWmax,DWshear,
                                lfLL,lldf->s_gShear,LLNShear,PLNShear);
         QPShear = CombineLoads(n,lfDCmin,lfDCmax,DCshear,lfDWmin,lfDWmax,DWshear,
                                lfLL,lldf->s_gShear,LLPShear,PLPShear);

         if (face == mbrLeftFace)
            ((*strengthIMinMoment)[data->id])->lfValue = QNMoment;
         else
            ((*strengthIMinMoment)[data->id])->rfValue = QNMoment;

         if (face == mbrLeftFace)
            ((*strengthIMaxMoment)[data->id])->lfValue = QPMoment;
         else
            ((*strengthIMaxMoment)[data->id])->rfValue = QPMoment;

         if (face == mbrLeftFace)
            ((*strengthIMinShear)[data->id])->lfValue = QNShear;
         else
            ((*strengthIMinShear)[data->id])->rfValue = QNShear;

         if (face == mbrLeftFace)
            ((*strengthIMaxShear)[data->id])->lfValue = QPShear;
         else
            ((*strengthIMaxShear)[data->id])->rfValue = QPShear;

         //
         // Service Limit States
         //

         n = span->GetLimitStateFactor(lsService);

         // Service I
         GetServiceILoadFactors(lfDCmin,lfDWmin,lfLL);
         lfDCmax = lfDCmin;
         lfDWmax = lfDWmin;

         QNMoment = CombineLoads(n,lfDCmin,lfDCmax,DCmoment,lfDWmin,lfDWmax,DWmoment,
                                 lfLL,lldf->s_gMoment,LLNMoment,PLNMoment);
         QPMoment = CombineLoads(n,lfDCmin,lfDCmax,DCmoment,lfDWmin,lfDWmax,DWmoment,
                                 lfLL,lldf->s_gMoment,LLPMoment,PLPMoment);
         QNShear = CombineLoads(n,lfDCmin,lfDCmax,DCshear,lfDWmin,lfDWmax,DWshear,
                                lfLL,lldf->s_gShear,LLNShear,PLNShear);
         QPShear = CombineLoads(n,lfDCmin,lfDCmax,DCshear,lfDWmin,lfDWmax,DWshear,
                                lfLL,lldf->s_gShear,LLPShear,PLPShear);

         if (face == mbrLeftFace)
            ((*serviceIMinMoment)[data->id])->lfValue = QNMoment;
         else
            ((*serviceIMinMoment)[data->id])->rfValue = QNMoment;

         if (face == mbrLeftFace)
            ((*serviceIMaxMoment)[data->id])->lfValue = QPMoment;
         else
            ((*serviceIMaxMoment)[data->id])->rfValue = QPMoment;

         if (face == mbrLeftFace)
            ((*serviceIMinShear)[data->id])->lfValue = QNShear;
         else
            ((*serviceIMinShear)[data->id])->rfValue = QNShear;

         if (face == mbrLeftFace)
            ((*serviceIMaxShear)[data->id])->lfValue = QPShear;
         else
            ((*serviceIMaxShear)[data->id])->rfValue = QPShear;

         // Service II
         GetServiceIILoadFactors(lfDCmin,lfDWmin,lfLL);
         lfDCmax = lfDCmin;
         lfDWmax = lfDWmin;

         QNMoment = CombineLoads(n,lfDCmin,lfDCmax,DCmoment,lfDWmin,lfDWmax,DWmoment,
                                 lfLL,lldf->s_gMoment,LLNMoment,PLNMoment);
         QPMoment = CombineLoads(n,lfDCmin,lfDCmax,DCmoment,lfDWmin,lfDWmax,DWmoment,
                                 lfLL,lldf->s_gMoment,LLPMoment,PLPMoment);
         QNShear = CombineLoads(n,lfDCmin,lfDCmax,DCshear,lfDWmin,lfDWmax,DWshear,
                                lfLL,lldf->s_gShear,LLNShear,PLNShear);
         QPShear = CombineLoads(n,lfDCmin,lfDCmax,DCshear,lfDWmin,lfDWmax,DWshear,
                                lfLL,lldf->s_gShear,LLPShear,PLPShear);

         if (face == mbrLeftFace)
            ((*serviceIIMinMoment)[data->id])->lfValue = QNMoment;
         else
            ((*serviceIIMinMoment)[data->id])->rfValue = QNMoment;

         if (face == mbrLeftFace)
            ((*serviceIIMaxMoment)[data->id])->lfValue = QPMoment;
         else
            ((*serviceIIMaxMoment)[data->id])->rfValue = QPMoment;

         if (face == mbrLeftFace)
            ((*serviceIIMinShear)[data->id])->lfValue = QNShear;
         else
            ((*serviceIIMinShear)[data->id])->rfValue = QNShear;

         if (face == mbrLeftFace)
            ((*serviceIIMaxShear)[data->id])->lfValue = QPShear;
         else
            ((*serviceIIMaxShear)[data->id])->rfValue = QPShear;

         // Service III
         GetServiceIIILoadFactors(lfDCmin,lfDWmin,lfLL);
         lfDCmax = lfDCmin;
         lfDWmax = lfDWmin;

         QNMoment = CombineLoads(n,lfDCmin,lfDCmax,DCmoment,lfDWmin,lfDWmax,DWmoment,
                                 lfLL,lldf->s_gMoment,LLNMoment,PLNMoment);
         QPMoment = CombineLoads(n,lfDCmin,lfDCmax,DCmoment,lfDWmin,lfDWmax,DWmoment,
                                 lfLL,lldf->s_gMoment,LLPMoment,PLPMoment);
         QNShear = CombineLoads(n,lfDCmin,lfDCmax,DCshear,lfDWmin,lfDWmax,DWshear,
                                lfLL,lldf->s_gShear,LLNShear,PLNShear);
         QPShear = CombineLoads(n,lfDCmin,lfDCmax,DCshear,lfDWmin,lfDWmax,DWshear,
                                lfLL,lldf->s_gShear,LLPShear,PLPShear);

         if (face == mbrLeftFace)
            ((*serviceIIIMinMoment)[data->id])->lfValue = QNMoment;
         else
            ((*serviceIIIMinMoment)[data->id])->rfValue = QNMoment;

         if (face == mbrLeftFace)
            ((*serviceIIIMaxMoment)[data->id])->lfValue = QPMoment;
         else
            ((*serviceIIIMaxMoment)[data->id])->rfValue = QPMoment;

         if (face == mbrLeftFace)
            ((*serviceIIIMinShear)[data->id])->lfValue = QNShear;
         else
            ((*serviceIIIMinShear)[data->id])->rfValue = QNShear;

         if (face == mbrLeftFace)
            ((*serviceIIIMaxShear)[data->id])->lfValue = QPShear;
         else
            ((*serviceIIIMaxShear)[data->id])->rfValue = QPShear;

         //
         // Fatigue Limit States
         //

         n = span->GetLimitStateFactor(lsFatigue);


         // Fatigue
         GetFatigueLoadFactors(lfDCmin,lfDWmin,lfLL);
         lfDCmax = lfDCmin;
         lfDWmax = lfDWmin;

         QNMoment = CombineLoads(n,lfDCmin,lfDCmax,DCmoment,lfDWmin,lfDWmax,DWmoment,
                                 lfLL,lldf->f_gMoment,FLNMoment,0);
         QPMoment = CombineLoads(n,lfDCmin,lfDCmax,DCmoment,lfDWmin,lfDWmax,DWmoment,
                                 lfLL,lldf->f_gMoment,FLPMoment,0);
         QNShear = CombineLoads(n,lfDCmin,lfDCmax,DCshear,lfDWmin,lfDWmax,DWshear,
                                lfLL,lldf->f_gShear,FLNShear,0);
         QPShear = CombineLoads(n,lfDCmin,lfDCmax,DCshear,lfDWmin,lfDWmax,DWshear,
                                lfLL,lldf->f_gShear,FLPShear,0);

         if (face == mbrLeftFace)
            ((*fatigueMinMoment)[data->id])->lfValue = QNMoment;
         else
            ((*fatigueMinMoment)[data->id])->rfValue = QNMoment;

         if (face == mbrLeftFace)
            ((*fatigueMaxMoment)[data->id])->lfValue = QPMoment;
         else
            ((*fatigueMaxMoment)[data->id])->rfValue = QPMoment;

         if (face == mbrLeftFace)
            ((*fatigueMinShear)[data->id])->lfValue = QNShear;
         else
            ((*fatigueMinShear)[data->id])->rfValue = QNShear;

         if (face == mbrLeftFace)
            ((*fatigueMaxShear)[data->id])->lfValue = QPShear;
         else
            ((*fatigueMaxShear)[data->id])->rfValue = QPShear;
         }
      }
}

double LRFDBridge::GetResult(int spanId,int poiOffset,MbrFace face,DiagramCatagory c,DiagramType t)
{
   TISResultsContainer *momentContainers[] = {dcMoments,dwMoments,0,0,0,0,0,0,0,0,0,0,0};
   TISResultsContainer *minMomentContainers[] = {0,0,llMinMoment,tandemMinMoment,truckMinMoment,trkTrainMinMoment,fatigueTrkMinMoment,pedestrianMinMoment,strengthIMinMoment,serviceIMinMoment,serviceIIMinMoment,serviceIIIMinMoment,fatigueMinMoment};
   TISResultsContainer *maxMomentContainers[] = {0,0,llMaxMoment,tandemMaxMoment,truckMaxMoment,trkTrainMaxMoment,fatigueTrkMaxMoment,pedestrianMaxMoment,strengthIMaxMoment,serviceIMaxMoment,serviceIIMaxMoment,serviceIIIMaxMoment,fatigueMaxMoment};
   TISResultsContainer *shearContainers[]  = {dcShears,dwShears,0,0,0,0,0,0,0,0,0,0,0};
   TISResultsContainer *minShearContainers[] = {0,0,llMinShear,tandemMinShear,truckMinShear,trkTrainMinShear,fatigueTrkMinShear,pedestrianMinShear,strengthIMinShear,serviceIMinShear,serviceIIMinShear,serviceIIIMinShear,fatigueMinShear};
   TISResultsContainer *maxShearContainers[] = {0,0,llMaxShear,tandemMaxShear,truckMaxShear,trkTrainMaxShear,fatigueTrkMaxShear,pedestrianMaxShear,strengthIMaxShear,serviceIMaxShear,serviceIIMaxShear,serviceIIIMaxShear,fatigueMaxShear};
   TISResultsContainer *results;
   ResultsData *data;
   int poi;
   double value;

   poi = SpanPntToPOI(spanId,poiOffset);

   switch(t)
      {
      case dtMoment:
           results = momentContainers[c];
           break;

      case dtShear:
           results = shearContainers[c];
           break;

      case dtMinMoment:
           results = minMomentContainers[c];
           break;

      case dtMaxMoment:
           results = maxMomentContainers[c];
           break;

      case dtMinShear:
           results = minShearContainers[c];
           break;

      case dtMaxShear:
           results = maxShearContainers[c];
           break;
      }

   data = (*results)[poi];

   if (face == mbrLeftFace)
      value = data->lfValue;
   else
      value = data->rfValue;

   return value;
}

