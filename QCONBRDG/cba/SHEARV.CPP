///////////////////////////////////////////////////////////////////////
// QConBridge - HL93 Live Load Analysis
// Copyright (C) 1999  Washington State Department of Transportation
//                     Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation, 
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but 
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source 
// License along with this program; if not, write to the Washington 
// State Department of Transportation, Bridge and Structures Office, 
// 4500 3rd AVE SE - P.O. Box  47340, Olympia, WA 98503, USA or e-mail 
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Revision Log
// ============
// May 1997 - Created, Richard Brice, PE
// Oct 1999 - Released as Open Source


#include <qconbrdg\shearv.h>
#include <qconbrdg\qcbapp.rh>


//{{TBridgeShearView Implementation}}


TBridgeShearView::TBridgeShearView (TBridgeDocument& doc,TWindow *parent):
    TBridgeResultsView(doc,parent)
{
    // INSERT>> Your constructor code here.
   CreateDisplayObjects();

}


TBridgeShearView::~TBridgeShearView ()
{
    // INSERT>> Your destructor code here.

}


const char far* TBridgeShearView::GetViewName ()
{
    return StaticName();
}

TResId TBridgeShearView::GetIconResourceId() const
{
   return IDI_SHEARVIEW;
}

void TBridgeShearView::CreateDisplayObjects()
{
   int id;
   TDiagramDisplayObject *diagramDispObj;

   TBridgeResultsView::CreateDisplayObjects();

   if (!bridgeDoc.IsAnalyzed())
      return;

    // DC
    diagramDispObj = new TDiagramDisplayObject(resultsDispObjMgr,dcDC);
    id = bridgeDoc.GetBridge().GetDiagramId(dcDC,dtShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    resultsDispObjMgr->AddDiagramDisplayObject(diagramDispObj);

    // DW
    diagramDispObj = new TDiagramDisplayObject(resultsDispObjMgr,dcDW);
    id = bridgeDoc.GetBridge().GetDiagramId(dcDW,dtShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    resultsDispObjMgr->AddDiagramDisplayObject(diagramDispObj);

    // All Envelopes
    diagramDispObj = new TDiagramDisplayObject(resultsDispObjMgr,dcAll);
    id = bridgeDoc.GetBridge().GetDiagramId(dcAll,dtMinShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    id = bridgeDoc.GetBridge().GetDiagramId(dcAll,dtMaxShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    resultsDispObjMgr->AddDiagramDisplayObject(diagramDispObj);

    // Tandem+Lane Envelope
    diagramDispObj = new TDiagramDisplayObject(resultsDispObjMgr,dcDesignTandem);
    id = bridgeDoc.GetBridge().GetDiagramId(dcDesignTandem,dtMinShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    id = bridgeDoc.GetBridge().GetDiagramId(dcDesignTandem,dtMaxShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    resultsDispObjMgr->AddDiagramDisplayObject(diagramDispObj);

    // Truck+Lane Envelope
    diagramDispObj = new TDiagramDisplayObject(resultsDispObjMgr,dcDesignTruck);
    id = bridgeDoc.GetBridge().GetDiagramId(dcDesignTruck,dtMinShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    id = bridgeDoc.GetBridge().GetDiagramId(dcDesignTruck,dtMaxShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    resultsDispObjMgr->AddDiagramDisplayObject(diagramDispObj);

    // Dual Truck+Lane Envelope
    diagramDispObj = new TDiagramDisplayObject(resultsDispObjMgr,dcDualTruckTrain);
    id = bridgeDoc.GetBridge().GetDiagramId(dcDualTruckTrain,dtMinShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    resultsDispObjMgr->AddDiagramDisplayObject(diagramDispObj);

    // Dual Tandem+Lane Envelope
    diagramDispObj = new TDiagramDisplayObject(resultsDispObjMgr,dcDualTandemTrain);
    id = bridgeDoc.GetBridge().GetDiagramId(dcDualTandemTrain,dtMinShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    resultsDispObjMgr->AddDiagramDisplayObject(diagramDispObj);

    // Fatigue+Lane Envelope
    diagramDispObj = new TDiagramDisplayObject(resultsDispObjMgr,dcFatigueTruck);
    id = bridgeDoc.GetBridge().GetDiagramId(dcFatigueTruck,dtMinShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    id = bridgeDoc.GetBridge().GetDiagramId(dcFatigueTruck,dtMaxShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    resultsDispObjMgr->AddDiagramDisplayObject(diagramDispObj);

    // Pedestrian Envelope
    diagramDispObj = new TDiagramDisplayObject(resultsDispObjMgr,dcPedestrian);
    id = bridgeDoc.GetBridge().GetDiagramId(dcPedestrian,dtMinShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    id = bridgeDoc.GetBridge().GetDiagramId(dcPedestrian,dtMaxShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    resultsDispObjMgr->AddDiagramDisplayObject(diagramDispObj);

    // Strength I Envelope
    diagramDispObj = new TDiagramDisplayObject(resultsDispObjMgr,dcStrengthI);
    id = bridgeDoc.GetBridge().GetDiagramId(dcStrengthI,dtMinShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    id = bridgeDoc.GetBridge().GetDiagramId(dcStrengthI,dtMaxShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    resultsDispObjMgr->AddDiagramDisplayObject(diagramDispObj);

    // Service I Envelope
    diagramDispObj = new TDiagramDisplayObject(resultsDispObjMgr,dcServiceI);
    id = bridgeDoc.GetBridge().GetDiagramId(dcServiceI,dtMinShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    id = bridgeDoc.GetBridge().GetDiagramId(dcServiceI,dtMaxShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    resultsDispObjMgr->AddDiagramDisplayObject(diagramDispObj);

    // Service II Envelope
    diagramDispObj = new TDiagramDisplayObject(resultsDispObjMgr,dcServiceII);
    id = bridgeDoc.GetBridge().GetDiagramId(dcServiceII,dtMinShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    id = bridgeDoc.GetBridge().GetDiagramId(dcServiceII,dtMaxShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    resultsDispObjMgr->AddDiagramDisplayObject(diagramDispObj);

    // Service III Envelope
    diagramDispObj = new TDiagramDisplayObject(resultsDispObjMgr,dcServiceIII);
    id = bridgeDoc.GetBridge().GetDiagramId(dcServiceIII,dtMinShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    id = bridgeDoc.GetBridge().GetDiagramId(dcServiceIII,dtMaxShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    resultsDispObjMgr->AddDiagramDisplayObject(diagramDispObj);

    // Fatigue Envelope
    diagramDispObj = new TDiagramDisplayObject(resultsDispObjMgr,dcFatigue);
    id = bridgeDoc.GetBridge().GetDiagramId(dcFatigue,dtMinShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    id = bridgeDoc.GetBridge().GetDiagramId(dcFatigue,dtMaxShear);
    diagramDispObj->AddDiagram(bridgeDoc.GetBridge().GetDiagram(id));
    resultsDispObjMgr->AddDiagramDisplayObject(diagramDispObj);
}

void TBridgeShearView::ConvertToDisplayUnits(double &value)
{
   value = DerivedUnits::Force(value).ConvertTo((bridgeDoc.GetUnits() == unitsSI) ? SIConstants::newton : USConstants::pound).value();
}

