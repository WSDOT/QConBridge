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

#include <qconbrdg\hmaster.h>
#include <winsys\uimetric.h>

TDockablePosition::TDockablePosition(uint Id,TAbsLocation loc,TRect r)
{
   id = Id;
   location = loc;
   rect = r;
}

TDockablePosition::~TDockablePosition()
{
}

uint TDockablePosition::GetId() const
{
   return id;
}

TAbsLocation TDockablePosition::GetLocation() const
{
   return location;
}

TRect TDockablePosition::GetRect() const
{
   return rect;
}

void TDockablePosition::SetLocation(TAbsLocation loc)
{
   location = loc;
}

void TDockablePosition::SetRect(TRect r)
{
   rect = r;
}

bool TDockablePosition::operator== (const TDockablePosition& other) const
{
   return (id == other.GetId());
}

//
//
//

THarborMaster::THarborMaster(TDecoratedFrame& df) :
THarbor(df)
{
   dockingPositions = new TIDockPosContainer(2,0,2);
}

THarborMaster::~THarborMaster()
{
   delete dockingPositions;
}

void THarborMaster::ShowDockable(uint id)
{
   TDockable* dockable;

   dockable = GetDockableWithId(id);
   if (dockable)
      {
      TDockablePosition* dockPos = GetDockablePosition(id);
      ShowDockable(*dockable,dockPos);

      // The dockable position is now old. The user can move the dockable
      // around and dockPos will not get updated. To solve this problem,
      // I am just getting rid of it.
      RemoveDockablePosition(id);
      }
}

void THarborMaster::HideDockable(uint id)
{
   TDockable* dockable;

   dockable = GetDockableWithId(id);
   if (dockable)
      {
      SaveDockablePosition(id);
      HideDockable(*dockable);
      }
}

bool THarborMaster::IsDockableVisible(uint id)
{
   TDockable* dockable = GetDockableWithId(id);
   bool isVisible = false;

   if (dockable)
      isVisible = dockable->GetWindow()->IsWindowVisible();

   return isVisible;
}

bool HasTheDockable(TWindow* p,void* v)
{
   uint* id = (uint*)v;
   return (p->ChildWithId(*id) != 0);
}

TDockingSlip* THarborMaster::GetDockingSlip(uint id)
{
   TDockingSlip* slip = 0;

   // Look in all the edge slips first
   for (TAbsLocation loc = alTop; loc <= alRight; loc++)
      {
      TEdgeSlip* edgeSlip;
      edgeSlip = GetEdgeSlip(loc);
      if (edgeSlip && edgeSlip->ChildWithId(id))
         {
         slip = edgeSlip;
         break;
         }
      }

   if (!slip)
      {
      // Check for floating slips
      TDecoratedFrame* decFrame = TYPESAFE_DOWNCAST(GetParentO(),TDecoratedFrame);
      TWindow* dockableParent = decFrame->FirstThat(HasTheDockable,(void*)&id);
      if (dockableParent)
         slip = TYPESAFE_DOWNCAST(dockableParent,TFloatingSlip);
      }

   return slip;
}

TDockable* THarborMaster::GetDockableWithId(uint id)
{
   TDockingSlip* slip;
   TWindow* win;
   TDockable* dockable = 0;

   slip = GetDockingSlip(id);

   if (slip)
      {
      TEdgeSlip* edgeSlip = TYPESAFE_DOWNCAST(slip,TEdgeSlip);
      TFloatingSlip* floatingSlip = TYPESAFE_DOWNCAST(slip,TFloatingSlip);
      if (edgeSlip)
         {
         win = edgeSlip->ChildWithId(id);
         }
      else if (floatingSlip)
         {
         // TFloatingSlip::GetWindow() returns its client, which is what we are looking for
         win = floatingSlip->GetWindow();
         }
      }
   else
      {
      // Dockable is not in a slip, check if the harbor has it
      win = ChildWithId(id);
      }

   if (win)
      dockable = TYPESAFE_DOWNCAST(win,TDockable);

   return dockable;
}

void THarborMaster::ShowDockable(TDockable& dockable,TDockablePosition* dockPos)
{
   TWindow* window = dockable.GetWindow();
   if (window && window->IsWindow())
      {
      if (dockPos)
         {
         Insert(dockable,dockPos->GetLocation(),&dockPos->GetRect().TopLeft());
         if (dockPos->GetLocation() == alNone)
            {
            TFloatingSlip* slip = TYPESAFE_DOWNCAST(GetDockingSlip(dockPos->GetId()),TFloatingSlip);
            slip->SetWindowPos(0,dockPos->GetRect(),SWP_NOZORDER);
            }
         }
      else
         Insert(dockable,alTop);
      }
}

void THarborMaster::HideDockable(TDockable& dockable)
{
   TWindow* window = dockable.GetWindow();
   if (window && window->IsWindow())
      Remove(dockable);
}

TDockablePosition* THarborMaster::GetDockablePosition(uint id)
{
   TDockable* dockable = GetDockableWithId(id);
   if (dockable && dockable->GetWindow()->IsWindowVisible())
      {
      // dockable position is out of date if the dockable is visible
      SaveDockablePosition(id);
      }

   TIDockPosIter iter(*dockingPositions);
   TDockablePosition* dockPos = 0;
   while(iter)
      {
      TDockablePosition* pos = iter++;
      if (pos->GetId() == id)
         {
         dockPos = pos;
         break;
         }
      }

   return dockPos;
}

void THarborMaster::SaveDockablePosition(uint id)
{
   // If this dockable already has a saved position, destroy it
   TDockablePosition find(id,alTop,TRect(0,0,0,0));
   int idx = dockingPositions->Find(&find);
   if (idx != INT_MAX)
      dockingPositions->Destroy(idx);

   TDockingSlip* slip = GetDockingSlip(id);

   if (!slip)
      return;

   TEdgeSlip* edgeSlip = TYPESAFE_DOWNCAST(slip,TEdgeSlip);
   TFloatingSlip* floatingSlip = TYPESAFE_DOWNCAST(slip,TFloatingSlip);
   TRect rect(0,0,0,0);

   if (edgeSlip)
      rect = edgeSlip->GetWindowRect();
   else if (floatingSlip)
      rect = floatingSlip->GetWindowRect();

   TDockablePosition* pos = new TDockablePosition(id,slip->GetLocation(),rect);
   dockingPositions->Add(pos);
}

void THarborMaster::RemoveDockablePosition(uint id)
{

   TDockablePosition* dockPos = GetDockablePosition(id);

   if (dockPos)
      dockingPositions->Destroy(dockPos);
}

void THarborMaster::SetDockablePosition(uint id,TAbsLocation loc,TRect rect)
{
   bool isVisible = IsDockableVisible(id);

   // If the dockable is currently visible, hide it. This cleans up all
   // slip stuff.
   if (isVisible)
      HideDockable(id);

   TDockablePosition* dp = GetDockablePosition(id);
   if (dp)
      {
      dp->SetLocation(loc);

      // Make sure the dockable is on the screen
      rect.left   = max(rect.left,0L);
      rect.top    = max(rect.top,0L);

      long rightEdge  = TUIMetric::CxScreen;
      long bottomEdge = TUIMetric::CyScreen;
      rect.right  = min(rect.right,rightEdge);
      rect.bottom = min(rect.bottom,bottomEdge);

      dp->SetRect(rect);
      }

   // If the dockable was visible, show it with its new position information
   if (isVisible)
      ShowDockable(*GetDockableWithId(id),dp);
}

