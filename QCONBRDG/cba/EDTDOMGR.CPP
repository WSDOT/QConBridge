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

#include <qconbrdg\edtdomgr.h>
#include <qconbrdg\editview.h>
#include <esl\units.h>
#include <esl\engnotat.h>
#include <owl\docmanag.h>

TEditorDisplayObjectManager::TEditorDisplayObjectManager(TBridgeEditorView *view) :
TBridgeDisplayObjectManager(view)
{
   selectionMode = smRightSupport;
   selSpanDisplayObject = NULL;
   selSupportDisplayObject = NULL;
   selLoadDisplayObject = NULL;
   SetDimensionColor(TColor::Black);
}

TEditorDisplayObjectManager::~TEditorDisplayObjectManager()
{
}

void TEditorDisplayObjectManager::Clear()
{
   TBridgeDisplayObjectManager::Clear();

   ClearSelection();
}

void TEditorDisplayObjectManager::SelectNext()
{
   // This function needs updating to include loads in the selection loop
   TSelSpanDisplayObject *selSpan;
   Span *span;

   span = (IsSpanSelected()) ? GetSelSpanDisplayObject()->GetSpan() : NULL;

   if (span && span->NextSpan())
      {
      SelectSpan(TYPESAFE_DOWNCAST(GetSpanDisplayObject(span->NextSpan()),TSelSpanDisplayObject));
      }
   else
      {
      // Select the first span
      selSpan = TYPESAFE_DOWNCAST((*spanDisplayObjects)[0],TSelSpanDisplayObject);
      SelectSpan(selSpan);
      return;
      }
}

void TEditorDisplayObjectManager::SelectPrev()
{
   // This function needs updating to include loads in the selection loop
   TSelSpanDisplayObject *selSpan;
   Span *span;

   span = (IsSpanSelected()) ? GetSelSpanDisplayObject()->GetSpan() : NULL;

   if (span && span->PrevSpan())
      {
      // if the span has a prev span, select it
      SelectSpan(TYPESAFE_DOWNCAST(GetSpanDisplayObject(span->PrevSpan()),TSelSpanDisplayObject));
      }
   else
      {
      // Select the last span
      selSpan = TYPESAFE_DOWNCAST((*spanDisplayObjects)[spanDisplayObjects->GetItemsInContainer()-1],TSelSpanDisplayObject);
      SelectSpan(selSpan);
      return;
      }
}

bool TEditorDisplayObjectManager::IsSomethingSelected() const
{
   return (IsSpanSelected() || IsSupportSelected() || IsLoadSelected());
}

bool TEditorDisplayObjectManager::IsSpanSelected() const
{
   return (selSpanDisplayObject != NULL);
}

bool TEditorDisplayObjectManager::IsSupportSelected() const
{
   return (selSupportDisplayObject != NULL);
}

bool TEditorDisplayObjectManager::IsLoadSelected() const
{
   return (selLoadDisplayObject != NULL);
}

void TEditorDisplayObjectManager::SetSelectionMode(SelectionMode mode)
{
   selectionMode = mode;
   if (IsSpanSelected())
      SelectSpan(selSpanDisplayObject);
}

TEditorDisplayObjectManager::SelectionMode TEditorDisplayObjectManager::GetSelectionMode() const
{
   return selectionMode;
}

bool TEditorDisplayObjectManager::Select(TPoint &point)
{
   TDisplayObject *dispObj;
   dispObj = Find(point);

   if (!dispObj)
      return false;

   switch (dispObj->ObjectType())
      {
      case otSpan:
           SelectSpan(TYPESAFE_DOWNCAST(dispObj,TSelSpanDisplayObject));
           break;

      case otSupport:
           SelectSupport(TYPESAFE_DOWNCAST(dispObj,TSelSupportDisplayObject));
           break;

      case otLoad:
           SelectLoad(TYPESAFE_DOWNCAST(dispObj,TSelLoadDisplayObject));
           break;
      }

   return true;
}

TDisplayObject *TEditorDisplayObjectManager::Find(TPoint &point)
{
   TISSpanDisplayObjectIterator i(*spanDisplayObjects);
   TISupportDisplayObjectIterator j(*supportDisplayObjects);
   TILoadDisplayObjectIterator k(*loadDisplayObjects);

   // Search for a span
   while(i)
      {
      TSelSpanDisplayObject *dispObj = TYPESAFE_DOWNCAST(i++,TSelSpanDisplayObject);
      if (dispObj->Contains(point))
         return dispObj;
      }


   // Search for a support
   while(j)
      {
      TSelSupportDisplayObject *dispObj = TYPESAFE_DOWNCAST(j++,TSelSupportDisplayObject);
      if (dispObj->Contains(point))
         return dispObj;
      }

   // Search for a load
   while(k)
      {
      TSelLoadDisplayObject *dispObj = TYPESAFE_DOWNCAST(k++,TSelLoadDisplayObject);
      if (dispObj->Contains(point))
         return dispObj;
      }

   // Nothing was found
   return NULL;
}

bool TEditorDisplayObjectManager::SelectSpan(Span *span)
{
   TSelSpanDisplayObject *selSpan;
   selSpan = TYPESAFE_DOWNCAST(GetSpanDisplayObject(span),TSelSpanDisplayObject);
   if (selSpan == NULL)
      return false;

   SelectSpan(selSpan);

   return true;
}

bool TEditorDisplayObjectManager::SelectSupport(Support *support)
{
   TSelSupportDisplayObject *selSupport;
   selSupport = TYPESAFE_DOWNCAST(GetSupportDisplayObject(support),TSelSupportDisplayObject);
   if (selSupport == NULL)
      return false;

   SelectSupport(selSupport);

   return true;
}

bool TEditorDisplayObjectManager::SelectLoad(BridgeLoad *load)
{
   TSelLoadDisplayObject *selLoad;
   selLoad = TYPESAFE_DOWNCAST(GetLoadDisplayObject(load),TSelLoadDisplayObject);
   if (selLoad == NULL)
      return false;

   SelectLoad(selLoad);

   return true;
}

Span *TEditorDisplayObjectManager::GetSelSpan() const
{
   Span *selSpan;
   if (IsSpanSelected())
      selSpan = selSpanDisplayObject->GetSpan();
   else
      selSpan = NULL;

   return selSpan;
}

Support *TEditorDisplayObjectManager::GetSelSupport() const
{
   Support *selSupport;

   if (IsSupportSelected())
      selSupport = selSupportDisplayObject->GetSupport();
   else
      selSupport = NULL;

   return selSupport;
}

BridgeLoad *TEditorDisplayObjectManager::GetSelLoad() const
{
   BridgeLoad *selLoad;

   if (IsLoadSelected())
      selLoad = selLoadDisplayObject->GetLoad();
   else
      selLoad = NULL;

   return selLoad;
}

TSelSpanDisplayObject *TEditorDisplayObjectManager::GetSelSpanDisplayObject() const
{
   return selSpanDisplayObject;
}

TSelSupportDisplayObject *TEditorDisplayObjectManager::GetSelSupportDisplayObject() const
{
   return selSupportDisplayObject;
}

TSelLoadDisplayObject *TEditorDisplayObjectManager::GetSelLoadDisplayObject() const
{
   return selLoadDisplayObject;
}

void TEditorDisplayObjectManager::SelectSpan(TSelSpanDisplayObject *spanDispObj)
{
   bool isCantilever;

   if (!spanDispObj)
      return;

   isCantilever = spanDispObj->GetSpan()->IsCantilevered();

   // Select the associated support
   TSelSupportDisplayObject *supportDispObj = NULL;
   if (selectionMode == smRightSupport && spanDispObj->GetSpan()->RightSupport())
      supportDispObj = TYPESAFE_DOWNCAST(GetSupportDisplayObject(spanDispObj->GetSpan()->RightSupport()),TSelSupportDisplayObject);
   else if (selectionMode == smLeftSupport && spanDispObj->GetSpan()->LeftSupport())
      supportDispObj = TYPESAFE_DOWNCAST(GetSupportDisplayObject(spanDispObj->GetSpan()->LeftSupport()),TSelSupportDisplayObject);

   // If there is not a selected support associated with this span, then
   // the selection cannot be made. (i.e. spans and supports must be
   // selected in pairs). The exception to this rule is if the span is
   // cantilevered
   if (!supportDispObj && !isCantilever)
      return;

   // Notify current selection that it is being unselected
   ClearSelection();

   // Select the span
   spanDispObj->Selecting();
   bview->InvalidateRgn(spanDispObj->GetDrawRegion());
   selSpanDisplayObject = spanDispObj;

   // Select the support
   if (supportDispObj)
      {
      supportDispObj->Selecting();
      bview->InvalidateRgn(supportDispObj->GetDrawRegion());
      selSupportDisplayObject = supportDispObj;
      }
   else
      selSupportDisplayObject = NULL; // there is not currently selected support
}

void TEditorDisplayObjectManager::SelectSupport(TSelSupportDisplayObject *supportDispObj)
{
   if (!supportDispObj)
      return;

   // Get the associated span
   TSelSpanDisplayObject *spanDispObj = NULL;
   if (selectionMode == smRightSupport && supportDispObj->GetSupport()->LeftSpan())
      spanDispObj = TYPESAFE_DOWNCAST(GetSpanDisplayObject(supportDispObj->GetSupport()->LeftSpan()),TSelSpanDisplayObject);
   else if (selectionMode == smLeftSupport && supportDispObj->GetSupport()->RightSpan())
      spanDispObj = TYPESAFE_DOWNCAST(GetSpanDisplayObject(supportDispObj->GetSupport()->RightSpan()),TSelSpanDisplayObject);

   // If there is not a selected span associated with this support, then
   // the selection cannot be made. (i.e. spans and supports must be
   // selected in pairs)
   if (!spanDispObj)
      return;

   // Notify current selection that it is being unselected
   ClearSelection();

   // Select the support
   supportDispObj->Selecting();
   bview->InvalidateRgn(supportDispObj->GetDrawRegion());
   selSupportDisplayObject = supportDispObj;

   // Select the span
   spanDispObj->Selecting();
   bview->InvalidateRgn(spanDispObj->GetDrawRegion());
   selSpanDisplayObject = spanDispObj;
}

void TEditorDisplayObjectManager::SelectLoad(TSelLoadDisplayObject *loadDispObj)
{
   if (!loadDispObj)
      return;

   // Notify current selection that it is being unselected
   ClearSelection();

   // Select the load
   loadDispObj->Selecting();
   bview->InvalidateRgn(loadDispObj->GetDrawRegion());
   selLoadDisplayObject = loadDispObj;
}

void TEditorDisplayObjectManager::ClearSelection()
{
   if (IsSpanSelected())
      {
      selSpanDisplayObject->UnSelecting();
      bview->InvalidateRgn(selSpanDisplayObject->GetDrawRegion());
      }
   selSpanDisplayObject = NULL;

   if (IsSupportSelected())
      {
      selSupportDisplayObject->UnSelecting();
      bview->InvalidateRgn(selSupportDisplayObject->GetDrawRegion());
      }
   selSupportDisplayObject = NULL;

   if (IsLoadSelected())
      {
      selLoadDisplayObject->UnSelecting();
      bview->InvalidateRgn(selLoadDisplayObject->GetDrawRegion());
      }
   selLoadDisplayObject = NULL;

   bview->UpdateWindow();
}

void TEditorDisplayObjectManager::RemoveSpan(Span *span)
{
   if (span == GetSelSpan())
      {
      selSpanDisplayObject->UnSelecting();
      selSpanDisplayObject = NULL;
      }

   TBridgeDisplayObjectManager::RemoveSpan(span);
}

void TEditorDisplayObjectManager::RemoveSupport(Support *support)
{
   if (support == GetSelSupport())
      {
      selSupportDisplayObject->UnSelecting();
      selSupportDisplayObject = NULL;
      }

   TBridgeDisplayObjectManager::RemoveSupport(support);
}

void TEditorDisplayObjectManager::RemoveLoad(BridgeLoad *load)
{
   if (load == GetSelLoad())
      {
      selLoadDisplayObject->UnSelecting();
      selLoadDisplayObject = NULL;
      }

   TBridgeDisplayObjectManager::RemoveLoad(load);
}


void TEditorDisplayObjectManager::SetupLegend()
{
   delete legend;
   legend = new TLegend;
   UpdateLegend();
}


void TEditorDisplayObjectManager::UpdateLegend()
{
   if (!legend)
      return;

   legend->Clear();

   //
   // Setup the cells
   //
   TBridgeEditorView *eview = TYPESAFE_DOWNCAST(bview,TBridgeEditorView);
   string item;
   int penStyle;
   TColor color;

   // DC load case
   item = eview->GetDocument().GetDocManager().GetApplication()->LoadString(IDS_DC);
   color = eview->bridgeDoc.GetPlotColor(dcDC);
   penStyle = eview->bridgeDoc.GetPenStyle(dcDC);
   legend->AddCell(new TLinestyleCell(item.c_str(),color,penStyle,NULL));

   // DW load case
   item = eview->GetDocument().GetDocManager().GetApplication()->LoadString(IDS_DW);
   color = eview->bridgeDoc.GetPlotColor(dcDW);
   penStyle = eview->bridgeDoc.GetPenStyle(dcDW);
   legend->AddCell(new TLinestyleCell(item.c_str(),color,penStyle,NULL));

   //
   // Standard Dead Loads
   //
   StdDeadLoadXferBuffer xfer;
   char buffer[256];
   string key;
   string value;
   string tag;
   SystemOfUnits units;
   DerivedUnits::LinearForce convFactor;
   double weight;

   units = eview->bridgeDoc.GetUnits();
   if (units == unitsSI)
      {
      tag = " N/m";
      convFactor = SIConstants::newton_per_meter;
      }
   else
      {
      tag = " lbs/ft";
      convFactor = USConstants::pound_per_feet;
      }

   eview->bridgeDoc.GetStdDeadLoads(xfer);

   // Selfweight
   key = "Self Weight :";
   if (xfer.EnableSelfWeight())
      value = "Enabled";
   else
      value = "Disabled";
   legend->AddCell(new TTextCell(key.c_str(),value.c_str()));

   // Traffic Barrier
   key = "Traffic Barrier :";
   if (xfer.EnableTBarrierLoad())
      {
      weight = xfer.GetTBarrierLoad();
      weight = DerivedUnits::LinearForce(weight).ConvertTo(convFactor).value();

      ostrstream(buffer,sizeof(buffer)) << EngNotation(weight,3) << tag << ends;
      value = buffer;
      }
   else
      value = "Disabled";
   legend->AddCell(new TTextCell(key.c_str(),value.c_str()));

   // Utility
   key = "Utility :";
   if (xfer.EnableUtilityLoad())
      {
      weight = xfer.GetUtilityLoad();
      weight = DerivedUnits::LinearForce(weight).ConvertTo(convFactor).value();

      ostrstream(buffer,sizeof(buffer)) << EngNotation(weight,3) << tag << ends;
      value = buffer;
      }
   else
      value = "Disabled";
   legend->AddCell(new TTextCell(key.c_str(),value.c_str()));

   // Overlay
   key = "Overlay :";
   if (xfer.EnableWearSurfLoad())
      {
      weight = xfer.GetWearSurfLoad();
      weight = DerivedUnits::LinearForce(weight).ConvertTo(convFactor).value();

      ostrstream(buffer,sizeof(buffer)) << EngNotation(weight,3) << tag << ends;
      value = buffer;
      }
   else
      value = "Disabled";
   legend->AddCell(new TTextCell(key.c_str(),value.c_str()));

   // Pedestrian Load
   key = "Pedestrian Ld :";
   weight = eview->bridgeDoc.GetPedestrianLoad();
   ostrstream(buffer,sizeof(buffer)) << EngNotation(weight,3) << tag << ends;
   value = buffer;
   legend->AddCell(new TTextCell(key.c_str(),value.c_str()));
}

