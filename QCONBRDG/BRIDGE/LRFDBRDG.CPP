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

DIAG_DEFINE_GROUP(LRFDBridge,ENABLED,LEVEL2);

LRFDBridge::LRFDBridge() :
Bridge()
{
   AddLoading(new BridgeLoading(DW_LOAD));
   AddLoading(new BridgeLoading(DC_LOAD));

   diagramIds         = new DiagramIdContainer(10,0,10);
   analysisResultsIds = new AnalysisResultsIdContainer(10,0,10);
   inflLoadRecords    = new InfluenceLoadRecordContainer(10,0,10);
   inflResultsRecords = new InfluenceResultsRecordContainer(10,0,10);
   inflReactionRecords = new InfluenceReactionRecordContainer(10,0,10);
   ulMomentZones      = new UnitLoadMomentZoneContainer(10,0,10);

   Init();
}

LRFDBridge::~LRFDBridge()
{
   delete inflLoadRecords;
   delete inflResultsRecords;
   delete inflReactionRecords;
   delete diagramIds;
   delete analysisResultsIds;
   delete ulMomentZones;
}

void LRFDBridge::Init()
{
   selfWgtEnabled        = false;
   trafficBarrierEnabled = false;
   wearSurfEnabled       = false;
   utilityEnabled        = false;

   // Init to -1 so debug code can check for value < 0
   selfWgtId        = -1;
   trafficBarrierId = -1;
   wearSurfId       = -1;
   utilityId        = -1;
   dcId             = -1;
   dwId             = -1;
   unitLoadId       = -1;

   trafficBarrierLd = 0.00;
   wearSurfLd       = 0.00;
   utilityLd        = 0.00;
   pedestrianLd     = 0.00;

   // Default to 1.0 so they will have no effect if not defined by the user
   IMtruck   = 1.;
   IMlane    = 1.;
   IMfatigue = 1.;

   // Default to one variable axle increment
   numAxleInc = 1;
   dualTruckTrain.enableGeneration = false;
   dualTandemTrain.enableGeneration = false;

   // Limit State Factors
   StrengthI_dcMin = 1.00;
   StrengthI_dcMax = 1.00;
   StrengthI_dwMin = 1.00;
   StrengthI_dwMax = 1.00;
   StrengthI_ll    = 1.00;

   ServiceI_dc = 1.00;
   ServiceI_dw = 1.00;
   ServiceI_ll = 1.00;

   ServiceII_dc = 1.00;
   ServiceII_dw = 1.00;
   ServiceII_ll = 1.00;

   ServiceIII_dc = 1.00;
   ServiceIII_dw = 1.00;
   ServiceIII_ll = 1.00;

   Fatigue_dc = 1.00;
   Fatigue_dw = 1.00;
   Fatigue_ll = 1.00;

   // Flush records
   diagramIds->Flush();
   analysisResultsIds->Flush();
   inflLoadRecords->Flush();
   inflResultsRecords->Flush();
   inflReactionRecords->Flush();
   ulMomentZones->Flush();
}

void LRFDBridge::Clear(bool init)
{
   Bridge::Clear(init);
   if (init)
      Init();
}

