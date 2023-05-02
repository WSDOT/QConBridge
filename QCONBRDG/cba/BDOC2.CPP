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
#include <qconbrdg\reader.h>
#include <qconbrdg\viewev.h>
#include <qconbrdg\qcbapp.h>
#include <qconbrdg\bdocmgr.h>

#include <owl\docmanag.h>
#include <owl\profile.h>
#include <strstrea.h>
#include <iomanip.h>
#include <dir.h>
#include <cstring.h>

void TBridgeDocument::ResetToDefaults()
{
   //
   // Dynamic Load Allowance Factors
   //
   bridge->SetTruckImpact(33.);
   bridge->SetLaneImpact(0.);
   bridge->SetFatigueImpact(15.);

   //
   // Live Load Generation Parameters
   //
   bridge->EnableDesignTandem(true);
   bridge->EnableFatigueTruck(true);

   // Variable axle truck
   bridge->EnableDesignTruck(true);
   bridge->SetNumAxleInc(1);

   // Dual truck train
   bridge->DualTruckLoadEnabled(DUALTRUCKS,true);
   bridge->DualTruckRangeEnabled(DUALTRUCKS,0,true);
   bridge->FromHeadwaySpacing(DUALTRUCKS,0,15);
   bridge->ToHeadwaySpacing(DUALTRUCKS,0,15);
   bridge->NumHeadwayInc(DUALTRUCKS,0,1);
   bridge->DualTruckRangeEnabled(DUALTRUCKS,1,false);
   bridge->FromHeadwaySpacing(DUALTRUCKS,1,15);
   bridge->ToHeadwaySpacing(DUALTRUCKS,1,15);
   bridge->NumHeadwayInc(DUALTRUCKS,1,1);
   bridge->DualTruckRangeEnabled(DUALTRUCKS,2,false);
   bridge->FromHeadwaySpacing(DUALTRUCKS,2,15);
   bridge->ToHeadwaySpacing(DUALTRUCKS,2,15);
   bridge->NumHeadwayInc(DUALTRUCKS,2,1);

   // Dual tandem train
   bridge->DualTruckLoadEnabled(DUALTANDEMS,true);
   bridge->DualTruckRangeEnabled(DUALTANDEMS,0,true);
   bridge->FromHeadwaySpacing(DUALTANDEMS,0,8);
   bridge->ToHeadwaySpacing(DUALTANDEMS,0,12);
   bridge->NumHeadwayInc(DUALTANDEMS,0,1);
   bridge->DualTruckRangeEnabled(DUALTANDEMS,1,false);
   bridge->FromHeadwaySpacing(DUALTANDEMS,1,8);
   bridge->ToHeadwaySpacing(DUALTANDEMS,1,12);
   bridge->NumHeadwayInc(DUALTANDEMS,1,1);
   bridge->DualTruckRangeEnabled(DUALTANDEMS,2,false);
   bridge->FromHeadwaySpacing(DUALTANDEMS,2,8);
   bridge->ToHeadwaySpacing(DUALTANDEMS,2,12);
   bridge->NumHeadwayInc(DUALTANDEMS,2,1);

   //
   // Load Factors
   //
   bridge->SetStrengthILoadFactors(0.90,1.25,0.65,1.50,1.75);
   bridge->SetServiceILoadFactors(1.00,1.00,1.00);
   bridge->SetServiceIILoadFactors(1.00,1.00,1.30);
   bridge->SetServiceIIILoadFactors(1.00,1.00,0.80);
   bridge->SetFatigueLoadFactors(0.00,0.00,0.75);

   //
   // Limit State Factors
   //
   /* for each span... set limit state factors to 1.0 */
}

void TBridgeDocument::DisplayDimensions(bool display)
{
   if (drawDimensions != display)
      NotifyViews(vnDisplayDimensions,display);

   drawDimensions = display;
}

void TBridgeDocument::DisplayLabels(bool display)
{
   if (drawLabels != display)
      NotifyViews(vnDisplayLabels,display);

   drawLabels = display;
}

void TBridgeDocument::DisplaySegmentTicks(bool display)
{
   if (drawSegmentTicks != display)
      NotifyViews(vnDisplaySegmentTicks,display);

   drawSegmentTicks = display;
}

void TBridgeDocument::DisplayGrid(bool display)
{
   if (drawGrid != display)
      NotifyViews(vnDisplayGrid,display);

   drawGrid = display;
}

void TBridgeDocument::InvertMomentDiagrams(bool invert)
{
   if (invertMomentDiagrams != invert)
      NotifyViews(vnInvertMomDiags,invert);

   invertMomentDiagrams = invert;
}

bool TBridgeDocument::DisplayDimensions() const
{
   return drawDimensions;
}

bool TBridgeDocument::DisplayLabels() const
{
   return drawLabels;
}

bool TBridgeDocument::DisplaySegmentTicks() const
{
    return drawSegmentTicks;
}

bool TBridgeDocument::DisplayGrid() const
{
   return drawGrid;
}

bool TBridgeDocument::InvertMomentDiagrams() const
{
   return invertMomentDiagrams;
}

void TBridgeDocument::SetUnits(SystemOfUnits u)
{
   bool notify = false;

   // If units are changing, remember to notify views
   if (units != u)
      notify = true;

   // Change the units
   units = u;

   // If units are changing and analysis results are available
   // then the temp report file must be updated
   if (notify && IsAnalyzed())
      {
      DeleteTempReportFile();
      GenerateReport();
      }

   // If units changed, notify views
   if (notify)
      {
      NotifyViews(vnUnitsChanged,0);
      SetDirty(true);
      }
}

SystemOfUnits TBridgeDocument::GetUnits() const
{
   return units;
}

void TBridgeDocument::AddSpanSupport(Span *newSpan,Support *newSupport,bool atEnd)
{
   SSNOTIFY ssNotify;

   GetBridge().AddSpanSupport(newSpan,newSupport,atEnd);

   ssNotify.newSpan = newSpan;
   ssNotify.newSupport = newSupport;
   NotifyViews(vnAddSpanSupport,(long)&ssNotify);

   SetDirty(true);
   SetAnalyzed(false);
}

void TBridgeDocument::InsertSpanSupport(Span *refSpan,bool after,Span *newSpan,Support *newSupport,bool isRightSupport)
{
   SSNOTIFY ssNotify;

   GetBridge().Insert(refSpan->GetID(),after,newSpan,newSupport,isRightSupport);
   ssNotify.newSpan = newSpan;
   ssNotify.newSupport = newSupport;
   NotifyViews(vnAddSpanSupport,(long)&ssNotify);

   SetDirty(true);
   SetAnalyzed(false);
}

void TBridgeDocument::RemoveSpan(Span *oldSpan,bool removeRightSupport)
{
   TRACE("Entering TBridgeDocument::RemoveSpan()");

   SSNOTIFY ssNotify;
   LDNOTIFY ldNotify;
   //
   // Remove all load display objects for loads on this span
   //
   BridgeLoading *dcLoading = GetBridge().GetLoading(DC_LOAD);
   BridgeLoadIterator *dcIter = dcLoading->GetIterator();
   while (*dcIter)
      {
      BridgeLoad *load = (*dcIter)++;
      if (oldSpan == load->GetSpan())
         {
         ldNotify.load = load;
         NotifyViews(vnRemoveLoad,(long)&ldNotify);
         }
      }
   delete dcIter;

   BridgeLoading *dwLoading = GetBridge().GetLoading(DW_LOAD);
   BridgeLoadIterator *dwIter = dwLoading->GetIterator();
   while (*dwIter)
      {
      BridgeLoad *load = (*dwIter)++;
      if (oldSpan == load->GetSpan())
         {
         ldNotify.load = load;
         NotifyViews(vnRemoveLoad,(long)&ldNotify);
         }
      }
   delete dwIter;

   //
   // Remove the span display object
   //
   ssNotify.oldSpan = oldSpan;
   NotifyViews(vnRemoveSpan,(long)&ssNotify);

   //
   // Remove the support display object
   //
   Support *oldSupport = (removeRightSupport) ? oldSpan->RightSupport()
                                              : oldSpan->LeftSupport();
   ssNotify.oldSpan = NULL;
   ssNotify.oldSupport = oldSupport;
   NotifyViews(vnRemoveSupport,(long)&ssNotify);

   //
   // Remove the actual span and corrosponding support
   //
   GetBridge().RemoveSpan(oldSpan->GetID(),removeRightSupport);


   SetDirty(true);
   SetAnalyzed(false);

   TRACE("Leaving TBridgeDocument::RemoveSpan()");
}

void TBridgeDocument::ReplaceSpan(Span *oldSpan,Span *newSpan)
{
   SSNOTIFY ssNotify;

   // Notify views that span is about to be replaced.
   // Once Bridge::ReplaceSpan is called, oldSpan is no longer valid
   ssNotify.oldSpan = oldSpan;
   ssNotify.newSpan = newSpan;
   NotifyViews(vnReplaceSpan,(long)&ssNotify);

   GetBridge().ReplaceSpan(oldSpan->GetID(),newSpan);

   SetDirty(true);
   SetAnalyzed(false);
}

void TBridgeDocument::ReplaceSupport(Support *oldSupport,Support *newSupport)
{
   SSNOTIFY ssNotify;

   // Notify views that support is about to be replaced.
   // Once Bridge::ReplaceSupport is called, oldSupport is no longer valid
   ssNotify.oldSupport = oldSupport;
   ssNotify.newSupport = newSupport;
   NotifyViews(vnReplaceSupport,(long)&ssNotify);

   GetBridge().ReplaceSupport(oldSupport->GetID(),newSupport);

   SetDirty(true);
   SetAnalyzed(false);
}

void TBridgeDocument::ReplaceLoad(ConcBridgeLoad *oldLoad,ConcLoadXferBuffer *xfer)
{
   LDNOTIFY ldNotify;
   Span *span;
   BridgeLoading *oldLoading;
   BridgeLoading *newLoading;
   BridgeLoad *newLoad;

   // Notify the views that this load is going away
   ldNotify.load = oldLoad;
   NotifyViews(vnRemoveLoad,(long)&ldNotify);

   // Remove the old load
   oldLoading = oldLoad->GetOwner();
   span = oldLoad->GetSpan();
   oldLoading->RemoveLoad(oldLoad);

   // Find the loading for new load
   newLoading = GetBridge().GetLoading(xfer->DCLoading() ? DC_LOAD : DW_LOAD);

   // store away the new load
   newLoad = new ConcBridgeLoad(span,xfer->GetStartLoc(),xfer->GetLoad(),xfer->IsFract());
   newLoading->AddLoad(newLoad);

   // Notify the views that a new load is present
   ldNotify.load = newLoad;
   NotifyViews(vnAddLoad,(long)&ldNotify);

   SetDirty(true);
   SetAnalyzed(false);
}

void TBridgeDocument::ReplaceLoad(UnifBridgeLoad *oldLoad,UnifLoadXferBuffer *xfer)
{
   LDNOTIFY ldNotify;
   Span *span;
   BridgeLoading *oldLoading;
   BridgeLoading *newLoading;
   BridgeLoad *newLoad;

   // Notify the views that this load is going away
   ldNotify.load = oldLoad;
   NotifyViews(vnRemoveLoad,(long)&ldNotify);

   // Remove the old load
   oldLoading = oldLoad->GetOwner();
   span = oldLoad->GetSpan();
   oldLoading->RemoveLoad(oldLoad);

   // Find the loading for new load
   newLoading = GetBridge().GetLoading(xfer->DCLoading() ? DC_LOAD : DW_LOAD);

   // store away the new load
   newLoad = new UnifBridgeLoad(span,xfer->GetStartLoc(),xfer->GetEndLoc(),xfer->GetLoad(),xfer->IsFract());
   newLoading->AddLoad(newLoad);

   // Notify the views that a new load is present
   ldNotify.load = newLoad;
   NotifyViews(vnAddLoad,(long)&ldNotify);

   SetDirty(true);
   SetAnalyzed(false);
}

void TBridgeDocument::AddLoad(Span *selSpan,ConcLoadXferBuffer *xfer)
{
   LDNOTIFY ldNotify;
   BridgeLoading *loading;
   ConcBridgeLoad *newLoad;

   // Find the loading for new load
   loading = GetBridge().GetLoading(xfer->DCLoading() ? DC_LOAD : DW_LOAD);

   if (xfer->AllSpans())
      {
      Span *span;
      for (span = GetBridge().GetFirstSpan(); span; span = span->NextSpan())
         {
         newLoad = new ConcBridgeLoad(span,xfer->GetStartLoc(),xfer->GetLoad(),xfer->IsFract());
         loading->AddLoad(newLoad);

         // Notify the views that a new load is present
         ldNotify.load = newLoad;
         NotifyViews(vnAddLoad,(long)&ldNotify);
         }
      }
   else
      {
      newLoad = new ConcBridgeLoad(selSpan,xfer->GetStartLoc(),xfer->GetLoad(),xfer->IsFract());
      loading->AddLoad(newLoad);

      // Notify the views that a new load is present
      ldNotify.load = newLoad;
      NotifyViews(vnAddLoad,(long)&ldNotify);
      }

   SetDirty(true);
   SetAnalyzed(false);
}

void TBridgeDocument::AddLoad(Span *selSpan,UnifLoadXferBuffer *xfer)
{
   LDNOTIFY ldNotify;
   BridgeLoading *loading;
   UnifBridgeLoad *newLoad;

   // Find the loading for new load
   loading = GetBridge().GetLoading(xfer->DCLoading() ? DC_LOAD : DW_LOAD);

   if (xfer->AllSpans())
      {
      Span *span;
      for (span = GetBridge().GetFirstSpan(); span; span = span->NextSpan())
         {
         newLoad = new UnifBridgeLoad(span,xfer->GetStartLoc(),xfer->GetEndLoc(),xfer->GetLoad(),xfer->IsFract());
         loading->AddLoad(newLoad);

         // Notify the views that a new load is present
         ldNotify.load = newLoad;
         NotifyViews(vnAddLoad,(long)&ldNotify);
         }
      }
   else
      {
      newLoad = new UnifBridgeLoad(selSpan,xfer->GetStartLoc(),xfer->GetEndLoc(),xfer->GetLoad(),xfer->IsFract());
      loading->AddLoad(newLoad);

      // Notify the views that a new load is present
      ldNotify.load = newLoad;
      NotifyViews(vnAddLoad,(long)&ldNotify);
      }

   SetDirty(true);
   SetAnalyzed(false);
}

void TBridgeDocument::InsertLoad(BridgeLoad *newLoad,int lcase)
{
   BridgeLoading *loading;
   LDNOTIFY ldNotify;

   loading = GetBridge().GetLoading(lcase);
   loading->AddLoad(newLoad);
   ldNotify.load = newLoad;
   NotifyViews(vnAddLoad,(long)&ldNotify);
   SetDirty(true);
   SetAnalyzed(false);
}

void TBridgeDocument::RemoveLoad(BridgeLoad *load)
{
   LDNOTIFY ldNotify;

   ldNotify.load = load;
   NotifyViews(vnRemoveLoad,(long)&ldNotify);
   BridgeLoading *loading = load->GetOwner();
   loading->RemoveLoad(load);
   SetDirty(true);
   SetAnalyzed(false);
}

void TBridgeDocument::ReplaceLoad(BridgeLoad *oldLoad,BridgeLoad *newLoad,int lcase)
{
   BridgeLoading *oldLoading;
   BridgeLoading *newLoading;
   Span *span;
   LDNOTIFY ldNotify;

   // Notify views that load is being removed
   ldNotify.load = oldLoad;
   NotifyViews(vnRemoveLoad,(long)&ldNotify);

   // Remove the load from the old loading
   oldLoading = oldLoad->GetOwner();
   span = oldLoad->GetSpan();
   oldLoading->RemoveLoad(oldLoad);

   // Add the new load to the new loading
   newLoading = GetBridge().GetLoading(lcase);
   newLoad->SetSpan(span);
   newLoading->AddLoad(newLoad);

   // Notify views of the new load
   ldNotify.load = newLoad;
   NotifyViews(vnAddLoad,(long)&ldNotify);

   SetDirty(true);
   SetAnalyzed(false);
}

void TBridgeDocument::SetMinAnalysisPoints(int minAnalysisPoints)
{
   GetBridge().SetMinAnalysisPoints(minAnalysisPoints);
}

int TBridgeDocument::GetMinAnalysisPoints()
{
   return GetBridge().GetMinAnalysisPoints();
}

void TBridgeDocument::SetLiveLoadGenParameters(LLGenXferBuffer &xfer)
{
   GetBridge().EnableDesignTandem(xfer.designTandem.disable == BF_UNCHECKED);

   GetBridge().EnableDesignTruck(xfer.varAxleTruck.disable == BF_UNCHECKED);
   GetBridge().SetNumAxleInc(xfer.varAxleTruck.numAxleInc);

   GetBridge().DualTruckLoadEnabled(DUALTRUCKS,xfer.dualTruckTrain.disabled != BF_CHECKED);
   GetBridge().DualTruckRangeEnabled(DUALTRUCKS,0,true);
   GetBridge().FromHeadwaySpacing(DUALTRUCKS,0,xfer.dualTruckTrain.r1From);
   GetBridge().ToHeadwaySpacing(DUALTRUCKS,0,xfer.dualTruckTrain.r1To);
   GetBridge().NumHeadwayInc(DUALTRUCKS,0,xfer.dualTruckTrain.r1Inc);
   GetBridge().DualTruckRangeEnabled(DUALTRUCKS,1,xfer.dualTruckTrain.range2Enabled == BF_CHECKED);
   GetBridge().FromHeadwaySpacing(DUALTRUCKS,1,xfer.dualTruckTrain.r2From);
   GetBridge().ToHeadwaySpacing(DUALTRUCKS,1,xfer.dualTruckTrain.r2To);
   GetBridge().NumHeadwayInc(DUALTRUCKS,1,xfer.dualTruckTrain.r2Inc);
   GetBridge().DualTruckRangeEnabled(DUALTRUCKS,2,xfer.dualTruckTrain.range3Enabled == BF_CHECKED);
   GetBridge().FromHeadwaySpacing(DUALTRUCKS,2,xfer.dualTruckTrain.r3From);
   GetBridge().ToHeadwaySpacing(DUALTRUCKS,2,xfer.dualTruckTrain.r3To);
   GetBridge().NumHeadwayInc(DUALTRUCKS,2,xfer.dualTruckTrain.r3Inc);

   GetBridge().DualTruckLoadEnabled(DUALTANDEMS,xfer.dualTandemTrain.disabled != BF_CHECKED);
   GetBridge().DualTruckRangeEnabled(DUALTANDEMS,0,true);
   GetBridge().FromHeadwaySpacing(DUALTANDEMS,0,xfer.dualTandemTrain.r1From);
   GetBridge().ToHeadwaySpacing(DUALTANDEMS,0,xfer.dualTandemTrain.r1To);
   GetBridge().NumHeadwayInc(DUALTANDEMS,0,xfer.dualTandemTrain.r1Inc);
   GetBridge().DualTruckRangeEnabled(DUALTANDEMS,1,xfer.dualTandemTrain.range2Enabled == BF_CHECKED);
   GetBridge().FromHeadwaySpacing(DUALTANDEMS,1,xfer.dualTandemTrain.r2From);
   GetBridge().ToHeadwaySpacing(DUALTANDEMS,1,xfer.dualTandemTrain.r2To);
   GetBridge().NumHeadwayInc(DUALTANDEMS,1,xfer.dualTandemTrain.r2Inc);
   GetBridge().DualTruckRangeEnabled(DUALTANDEMS,2,xfer.dualTandemTrain.range3Enabled == BF_CHECKED);
   GetBridge().FromHeadwaySpacing(DUALTANDEMS,2,xfer.dualTandemTrain.r3From);
   GetBridge().ToHeadwaySpacing(DUALTANDEMS,2,xfer.dualTandemTrain.r3To);
   GetBridge().NumHeadwayInc(DUALTANDEMS,2,xfer.dualTandemTrain.r3Inc);

   GetBridge().EnableFatigueTruck(xfer.fatigueTruck.disable == BF_UNCHECKED);

   SetDirty(true);
   SetAnalyzed(false);
}

void TBridgeDocument::GetLiveLoadGenParameters(LLGenXferBuffer &xfer)
{
    xfer.designTandem.disable         = !GetBridge().EnableDesignTandem();

    xfer.varAxleTruck.disable         = !GetBridge().EnableDesignTruck();
    xfer.varAxleTruck.numAxleInc      = GetBridge().GetNumAxleInc();

    xfer.dualTruckTrain.disabled      = !GetBridge().DualTruckLoadEnabled(DUALTRUCKS);
    xfer.dualTruckTrain.r1From        = GetBridge().FromHeadwaySpacing(DUALTRUCKS,0);
    xfer.dualTruckTrain.r1To          = GetBridge().ToHeadwaySpacing(DUALTRUCKS,0);
    xfer.dualTruckTrain.r1Inc         = GetBridge().NumHeadwayInc(DUALTRUCKS,0);
    xfer.dualTruckTrain.range2Enabled = GetBridge().DualTruckRangeEnabled(DUALTRUCKS,1);
    xfer.dualTruckTrain.r2From        = GetBridge().FromHeadwaySpacing(DUALTRUCKS,1);
    xfer.dualTruckTrain.r2To          = GetBridge().ToHeadwaySpacing(DUALTRUCKS,1);
    xfer.dualTruckTrain.r2Inc         = GetBridge().NumHeadwayInc(DUALTRUCKS,1);
    xfer.dualTruckTrain.range3Enabled = GetBridge().DualTruckRangeEnabled(DUALTRUCKS,2);
    xfer.dualTruckTrain.r3From        = GetBridge().FromHeadwaySpacing(DUALTRUCKS,2);
    xfer.dualTruckTrain.r3To          = GetBridge().ToHeadwaySpacing(DUALTRUCKS,2);
    xfer.dualTruckTrain.r3Inc         = GetBridge().NumHeadwayInc(DUALTRUCKS,2);

    xfer.dualTandemTrain.disabled      = !GetBridge().DualTruckLoadEnabled(DUALTANDEMS);
    xfer.dualTandemTrain.r1From        = GetBridge().FromHeadwaySpacing(DUALTANDEMS,0);
    xfer.dualTandemTrain.r1To          = GetBridge().ToHeadwaySpacing(DUALTANDEMS,0);
    xfer.dualTandemTrain.r1Inc         = GetBridge().NumHeadwayInc(DUALTANDEMS,0);
    xfer.dualTandemTrain.range2Enabled = GetBridge().DualTruckRangeEnabled(DUALTANDEMS,1);
    xfer.dualTandemTrain.r2From        = GetBridge().FromHeadwaySpacing(DUALTANDEMS,1);
    xfer.dualTandemTrain.r2To          = GetBridge().ToHeadwaySpacing(DUALTANDEMS,1);
    xfer.dualTandemTrain.r2Inc         = GetBridge().NumHeadwayInc(DUALTANDEMS,1);
    xfer.dualTandemTrain.range3Enabled = GetBridge().DualTruckRangeEnabled(DUALTANDEMS,2);
    xfer.dualTandemTrain.r3From        = GetBridge().FromHeadwaySpacing(DUALTANDEMS,2);
    xfer.dualTandemTrain.r3To          = GetBridge().ToHeadwaySpacing(DUALTANDEMS,2);
    xfer.dualTandemTrain.r3Inc         = GetBridge().NumHeadwayInc(DUALTANDEMS,2);

    xfer.fatigueTruck.disable          = !GetBridge().EnableFatigueTruck();
}

void TBridgeDocument::Cantilever(bool removeSupport,bool isFirstSpan)
{
   Span *span;
   SSNOTIFY ssNotify;

   span = (isFirstSpan) ? GetBridge().GetFirstSpan() : GetBridge().GetLastSpan();

   if (removeSupport && !span->IsCantilevered())
      {
      // remove a support from a span that is not cantilevered
      if (isFirstSpan)
         {
         ssNotify.oldSupport = GetBridge().GetFirstSupport();
         NotifyViews(vnRemoveSupport,(long)&ssNotify);
         GetBridge().CantileverFirstSpan(true);
         }
      else
         {
         ssNotify.oldSupport = GetBridge().GetLastSupport();
         NotifyViews(vnRemoveSupport,(long)&ssNotify);
         GetBridge().CantileverLastSpan(true);
         }
      }
   else if (!removeSupport && span->IsCantilevered())
      {
      // add a support to a span that is cantilevered
      if (isFirstSpan)
         {
         GetBridge().CantileverFirstSpan(false);
         ssNotify.newSupport = GetBridge().GetFirstSupport();
         NotifyViews(vnAddSupport,(long)&ssNotify);
         }
      else
         {
         GetBridge().CantileverLastSpan(false);
         ssNotify.newSupport = GetBridge().GetLastSupport();
         NotifyViews(vnAddSupport,(long)&ssNotify);
         }
      }
   else
      return; // cannot remove support from cantilevered span or
              // add support to supported span

   SetDirty(true);
   SetAnalyzed(false);
}

void TBridgeDocument::SetStdDeadLoads(StdDeadLoadXferBuffer &xfer)
{
   GetBridge().SelfWgtEnabled(xfer.EnableSelfWeight());

   GetBridge().TrafficBarrierEnabled(xfer.EnableTBarrierLoad());
   if (xfer.EnableTBarrierLoad())
      GetBridge().TrafficBarrierLd(xfer.GetTBarrierLoad());

   GetBridge().UtilityEnabled(xfer.EnableUtilityLoad());
   if (xfer.EnableUtilityLoad())
      GetBridge().UtilityLd(xfer.GetUtilityLoad());

   GetBridge().WearSurfEnabled(xfer.EnableWearSurfLoad());
   if (xfer.EnableWearSurfLoad())
      GetBridge().WearSurfLd(xfer.GetWearSurfLoad());

   NotifyViews(vnStdDeadLoadsChanged,0);
   SetDirty(true);
   SetAnalyzed(false);
}

void TBridgeDocument::GetStdDeadLoads(StdDeadLoadXferBuffer &xfer)
{
   xfer.EnableSelfWeight(GetBridge().SelfWgtEnabled());

   xfer.EnableTBarrierLoad(GetBridge().TrafficBarrierEnabled());
   xfer.page1.tbLoad = GetBridge().TrafficBarrierLd();

   xfer.EnableUtilityLoad(GetBridge().UtilityEnabled());
   xfer.page2.utLoad = GetBridge().UtilityLd();

   xfer.EnableWearSurfLoad(GetBridge().WearSurfEnabled());
   xfer.page2.wsLoad = GetBridge().WearSurfLd();
}

void TBridgeDocument::SetLoadFactors(LoadFactorXferBuffer &xfer)
{
   GetBridge().SetStrengthILoadFactors(xfer.StrengthI.dcMin,
                                       xfer.StrengthI.dcMax,
                                       xfer.StrengthI.dwMin,
                                       xfer.StrengthI.dwMax,
                                       xfer.StrengthI.ll);

   GetBridge().SetServiceILoadFactors(xfer.ServiceI.dc,
                                      xfer.ServiceI.dw,
                                      xfer.ServiceI.ll);

   GetBridge().SetServiceIILoadFactors(xfer.ServiceII.dc,
                                       xfer.ServiceII.dw,
                                       xfer.ServiceII.ll);

   GetBridge().SetServiceIIILoadFactors(xfer.ServiceIII.dc,
                                        xfer.ServiceIII.dw,
                                        xfer.ServiceIII.ll);

   GetBridge().SetFatigueLoadFactors(xfer.Fatigue.dc,
                                     xfer.Fatigue.dw,
                                     xfer.Fatigue.ll);

   SetDirty(true);
   SetAnalyzed(false);
}

void TBridgeDocument::GetLoadFactors(LoadFactorXferBuffer &xfer)
{
   GetBridge().GetStrengthILoadFactors(xfer.StrengthI.dcMin,
                                       xfer.StrengthI.dcMax,
                                       xfer.StrengthI.dwMin,
                                       xfer.StrengthI.dwMax,
                                       xfer.StrengthI.ll);

   GetBridge().GetServiceILoadFactors(xfer.ServiceI.dc,
                                      xfer.ServiceI.dw,
                                      xfer.ServiceI.ll);

   GetBridge().GetServiceIILoadFactors(xfer.ServiceII.dc,
                                       xfer.ServiceII.dw,
                                       xfer.ServiceII.ll);

   GetBridge().GetServiceIIILoadFactors(xfer.ServiceIII.dc,
                                        xfer.ServiceIII.dw,
                                        xfer.ServiceIII.ll);

   GetBridge().GetFatigueLoadFactors(xfer.Fatigue.dc,
                                     xfer.Fatigue.dw,
                                     xfer.Fatigue.ll);
}

void TBridgeDocument::SetPlotColor(TPlotLocation loc,DiagramCatagory dc,TColor color)
{
   if (plotStyleData[loc][dc].color == color)
      return;

   plotStyleData[loc][dc].color = color;

   if (loc == plScreen)
      NotifyViews(vnPlotStyleChanged,dc);
}

TColor TBridgeDocument::GetPlotColor(DiagramCatagory dc)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetDocManager().GetApplication(),TQConBridgeApp);
   TPlotLocation loc = (theApp->Printing) ? plPrinter : plScreen;
   return plotStyleData[loc][dc].color;
}

TColor TBridgeDocument::GetPlotColor(TPlotLocation loc,DiagramCatagory dc)
{
   return plotStyleData[loc][dc].color;
}

void TBridgeDocument::SetPenStyle(TPlotLocation loc,DiagramCatagory dc,int ps)
{
   if (plotStyleData[loc][dc].penStyle == ps)
      return;

   plotStyleData[loc][dc].penStyle = ps;

   if (loc == plScreen)
      NotifyViews(vnPlotStyleChanged,dc);
}

int TBridgeDocument::GetPenStyle(DiagramCatagory dc)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetDocManager().GetApplication(),TQConBridgeApp);
   TPlotLocation loc = (theApp->Printing) ? plPrinter : plScreen;
   return plotStyleData[loc][dc].penStyle;
}

int TBridgeDocument::GetPenStyle(TPlotLocation loc,DiagramCatagory dc)
{
   return plotStyleData[loc][dc].penStyle;
}

void TBridgeDocument::SetPlotSymbolType(TPlotLocation loc,DiagramCatagory dc,TSymbolType symbol)
{
   if (plotStyleData[loc][dc].symbol == symbol)
      return;

   plotStyleData[loc][dc].symbol = symbol;

   if (loc == plScreen)
      NotifyViews(vnPlotStyleChanged,dc);
}

TSymbolType TBridgeDocument::GetPlotSymbolType(DiagramCatagory dc)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetDocManager().GetApplication(),TQConBridgeApp);
   TPlotLocation loc = (theApp->Printing) ? plPrinter : plScreen;
   return plotStyleData[loc][dc].symbol;
}

TSymbolType TBridgeDocument::GetPlotSymbolType(TPlotLocation loc,DiagramCatagory dc)
{
   return plotStyleData[loc][dc].symbol;
}

TSymbol *TBridgeDocument::GetPlotSymbol(DiagramCatagory dc)
{
   return symbol[GetPlotSymbolType(dc)];
}


double TBridgeDocument::GetResult(int spanId,int poiOffset,MbrFace face,DiagramCatagory c,DiagramType t)
{
   double value;

   value = GetBridge().GetResult(spanId,poiOffset,face,c,t);
   return value;
}

double TBridgeDocument::GetReaction(int pierId,DiagramCatagory c,DiagramType t)
{
	double value = GetBridge().GetReaction(pierId,c,t);
   return value;
}

void TBridgeDocument::SetPedestrianLoad(double pLoad)
{
   GetBridge().PedestrianLd(pLoad);
   NotifyViews(vnPedestrianLoadChanged,0);
   SetDirty(true);
   SetAnalyzed(false);
}

double TBridgeDocument::GetPedestrianLoad()
{
   return GetBridge().PedestrianLd();
}

void TBridgeDocument::GetDynamicLoadAllowance(double &IMtruck,double &IMlane,double &IMfatigue)
{
   IMtruck   = GetBridge().GetTruckImpact();
   IMlane    = GetBridge().GetLaneImpact();
   IMfatigue = GetBridge().GetFatigueImpact();
}

void TBridgeDocument::SetDynamicLoadAllowance(double IMtruck,double IMlane,double IMfatigue)
{
   GetBridge().SetTruckImpact(IMtruck);
   GetBridge().SetLaneImpact(IMlane);
   GetBridge().SetFatigueImpact(IMfatigue);

   SetDirty(true);
   SetAnalyzed(false);
}

double TBridgeDocument::GetLength()
{
   return GetBridge().Length();
}
