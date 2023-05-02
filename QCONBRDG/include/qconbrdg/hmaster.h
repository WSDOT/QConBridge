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

#if !defined OWLTOOLS_HMASTER_H_
#define OWLTOOLS_HMASTER_H_

#include <owl\docking.h>
#include <classlib\arrays.h>

class TDockablePosition
{
public:
   TDockablePosition(uint Id,TAbsLocation loc,TRect r);
   virtual ~TDockablePosition();

   uint GetId() const;
   TAbsLocation GetLocation() const;
   void SetLocation(TAbsLocation loc);
   void SetRect(TRect r);
   TRect GetRect() const;

   bool operator == (const TDockablePosition& other) const;

private:
   uint id;
   TAbsLocation location;
   TRect rect;
};

typedef TIArrayAsVector<TDockablePosition> TIDockPosContainer;
typedef TIArrayAsVectorIterator<TDockablePosition> TIDockPosIter;

class THarborMaster : public THarbor
{
public:
   THarborMaster(TDecoratedFrame& df);
   virtual ~THarborMaster();

   void ShowDockable(uint id);
   void HideDockable(uint id);

   TDockablePosition* GetDockablePosition(uint id);
   void SetDockablePosition(uint id,TAbsLocation loc,TRect rect);

   bool IsDockableVisible(uint id);

private:
   TIDockPosContainer* dockingPositions;

   TDockingSlip* GetDockingSlip(uint id);

   void SaveDockablePosition(uint id);
   void RemoveDockablePosition(uint id);

   TDockable* GetDockableWithId(uint id);
   void ShowDockable(TDockable& dockable,TDockablePosition* dockPos);
   void HideDockable(TDockable& dockable);
};

#endif