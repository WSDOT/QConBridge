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

#include <qconbrdg\bdoc.h>
#include <qconbrdg\table.h>
#include <qconbrdg\rptgen.h>

bool TBridgeDocument::Analyze(TProgressDlg *dlg)
{
   TRACE("TBridgeDocument::Analyze()");

   bool retval;
   SetAnalyzed(false);

   ProgressIndicator pi(dlg);
   retval = GetBridge().Analyze(&pi);

   if (retval)
      {
      GenerateReport();
      }

   SetAnalyzed(retval);

   return retval;
}

bool TBridgeDocument::GenerateReport()
{
   DeleteTempReportFile();
#if defined (__FLAT__)
   char buffer[256];
   ::GetTempPath(sizeof(buffer),buffer);
   ::GetTempFileName(buffer,"qcb",0,tmpFile);
#else
   ::GetTempFileName(0,"qcb",0,tmpFile);
#endif

   TReportGenerator rpt(tmpFile,*this);
   return rpt.GenerateReport();
}

TTable *TBridgeDocument::CreateTable(DiagramCatagory dc,bool bReactions)
{
   TTable *tbl;

   switch(dc)
      {
      case dcDC:
      case dcDW:
      	if ( bReactions == true )
         	tbl = CreateReactionTable(dc);
         else
	        	tbl = CreateLoadTable(dc);
         break;

      case dcAll:
      case dcDesignTandem:
      case dcDesignTruck:
      case dcDualTruckTrain:
      case dcDualTandemTrain:
      case dcFatigueTruck:
      case dcPedestrian:
      case dcStrengthI:
      case dcServiceI:
      case dcServiceII:
      case dcServiceIII:
      case dcFatigue:
      	if ( bReactions == true )
         	tbl = CreateReactionEnvelopeTable(dc);
         else
         	tbl = CreateEnvelopeTable(dc);
         break;
      }

   return tbl;
}

TLoadTable *TBridgeDocument::CreateLoadTable(DiagramCatagory dc)
{
   TLoadTable *tbl;
   int spanId;
   double shear;
   double moment;
   int minAnalysisPoints = GetMinAnalysisPoints();
   int pnt;

   tbl = new TLoadTable(GetUnits(),(dc == dcDC) ? "DC Dead Load" : "DW Dead Load");
   tbl->SetRecordGroupCount(GetMinAnalysisPoints());
   for (spanId = 1; spanId <= GetNumSpans(); spanId++)
      {
      for (pnt = 0; pnt < minAnalysisPoints; pnt++)
         {
         MbrFace face = (pnt == minAnalysisPoints-1) ? mbrLeftFace : mbrRightFace;
         shear = GetResult(spanId,pnt,face,dc,dtShear);
         moment = GetResult(spanId,pnt,face,dc,dtMoment);

         tbl->AddEntry(spanId,pnt,shear,moment);
         }
      }

   return tbl;
}


TEnvelopeTable *TBridgeDocument::CreateEnvelopeTable(DiagramCatagory dc)
{
   int spanId;
   double minShear;
   double maxShear;
   double minMoment;
   double maxMoment;
   int minAnalysisPoints = GetMinAnalysisPoints();
   int pnt;
   TEnvelopeTable *tbl;
   string title;

   switch(dc)
      {
      case dcAll:
           title = "Live Load Envelopes (Per Lane)";
           break;

      case dcDesignTandem:
           title = "Design Tandem + Lane Envelopes (Per Lane)";
           break;

      case dcDesignTruck:
           title = "Design Truck + Lane Envelopes (Per Lane)";
           break;

      case dcDualTruckTrain:
           title = "Dual Truck Train + Lane Envelopes (Per Lane)";
           break;

      case dcDualTandemTrain:
           title = "Dual Tandem Train + Lane Envelopes (Per Lane)";
           break;

      case dcFatigueTruck:
           title = "Fatigue Truck Envelopes (Per Lane)";
           break;

      case dcPedestrian:
           title = "Pedestrian Live Load Envelopes";
           break;

      case dcStrengthI:
           title = "Strength I Limit State Envelopes";
           break;

      case dcServiceI:
           title = "Service I Limit State Envelopes";
           break;

      case dcServiceII:
           title = "Service II Limit State Envelopes";
           break;

      case dcServiceIII:
           title = "Service III Limit State Envelopes";
           break;

      case dcFatigue:
           title = "Fatigue Limit State Envelopes";
           break;
      }

   tbl = new TEnvelopeTable(GetUnits(),title.c_str());
   tbl->SetRecordGroupCount(GetMinAnalysisPoints());
   for (spanId = 1; spanId <= GetNumSpans(); spanId++)
      {
      for (pnt = 0; pnt < minAnalysisPoints; pnt++)
         {
         MbrFace face = (pnt == minAnalysisPoints-1) ? mbrLeftFace : mbrRightFace;
         minShear  = GetResult(spanId,pnt,face,dc,dtMinShear);
         maxShear  = GetResult(spanId,pnt,face,dc,dtMaxShear);
         minMoment = GetResult(spanId,pnt,face,dc,dtMinMoment);
         maxMoment = GetResult(spanId,pnt,face,dc,dtMaxMoment);

         tbl->AddEntry(spanId,pnt,minShear,maxShear,minMoment,maxMoment);
         }
      }

   return tbl;
}

TReactionTable* TBridgeDocument::CreateReactionTable(DiagramCatagory dc)
{
   TReactionTable *tbl;
   int pierId;
   int nPiers = GetNumSupports();
   double fx,fy,mz;

   tbl = new TReactionTable(GetUnits(),(dc == dcDC) ? "DC Dead Load" : "DW Dead Load");
   tbl->SetRecordGroupCount(nPiers);
   for (pierId = 1; pierId <= nPiers; pierId++)
      {
      fx = GetReaction(pierId,dc,rtFx);
      fy = GetReaction(pierId,dc,rtFy);
      mz = GetReaction(pierId,dc,rtMz);
      tbl->AddEntry(pierId,fx,fy,mz);
      }
   return tbl;
}

TReactionEnvelopeTable* TBridgeDocument::CreateReactionEnvelopeTable(DiagramCatagory dc)
{
	int pierId;
   int nPiers = GetNumSupports();
   double fx_min, fx_max;
   double fy_min, fy_max;
   double mz_min, mz_max;

   TReactionEnvelopeTable *tbl;
   string title;

   switch(dc)
      {
      case dcAll:
           title = "Live Load Envelopes (Per Lane)";
           break;

      case dcDesignTandem:
           title = "Design Tandem + Lane Envelopes (Per Lane)";
           break;

      case dcDesignTruck:
           title = "Design Truck + Lane Envelopes (Per Lane)";
           break;

      case dcDualTruckTrain:
           title = "Dual Truck Train + Lane Envelopes (Per Lane)";
           break;

      case dcDualTandemTrain:
           title = "Dual Tandem Train + Lane Envelopes (Per Lane)";
           break;

      case dcFatigueTruck:
           title = "Fatigue Truck Envelopes (Per Lane)";
           break;

      case dcPedestrian:
           title = "Pedestrian Live Load Envelopes";
           break;

      case dcStrengthI:
           title = "Strength I Limit State Envelopes";
           break;

      case dcServiceI:
           title = "Service I Limit State Envelopes";
           break;

      case dcServiceII:
           title = "Service II Limit State Envelopes";
           break;

      case dcServiceIII:
           title = "Service III Limit State Envelopes";
           break;

      case dcFatigue:
           title = "Fatigue Limit State Envelopes";
           break;
      }

   tbl = new TReactionEnvelopeTable(GetUnits(),title.c_str());

   for (pierId = 1; pierId <= nPiers; pierId++)
      {
	      fx_min = GetReaction(pierId,dc,rtFxMin);
   	   fy_min = GetReaction(pierId,dc,rtFyMin);
      	mz_min = GetReaction(pierId,dc,rtMzMin);
	      fx_max = GetReaction(pierId,dc,rtFxMax);
   	   fy_max = GetReaction(pierId,dc,rtFyMax);
      	mz_max = GetReaction(pierId,dc,rtMzMax);

         tbl->AddEntry(pierId,fx_min,fx_max,fy_min,fy_max,mz_min,mz_max);
      }

   return tbl;
}

