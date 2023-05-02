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

#if !defined LOADDO_H
#define LOADDO_H

#include <qconbrdg\dispobj.h>
#include <qconbrdg\selobj.h>
#include <bridge\bload.h>
#include <bridge\bloading.h>
#include <bridge\span.h>

#define otLoad 3

class TLoadDisplayObject : public TDisplayObject
{
protected:
   TRect  UnifLoadRect;
   TPoint Top, Bottom;
   BridgeLoad *load;

public:
   TLoadDisplayObject(TBridgeDisplayObjectManager *doMgr,BridgeLoad *Load);
   virtual ~TLoadDisplayObject();
   void Draw(TDC &dc);
   void Setup(TMapper &mapr);
   int ObjectType() const;
   BridgeLoad *GetLoad() const;
   void SetLoad(BridgeLoad *newLoad);

   bool operator == (const TLoadDisplayObject &ldo) const;

protected:
   virtual void ConcLoadSetup(TMapper &mapr);
   virtual void UnifLoadSetup(TMapper &mapr);
   void ConcLoadDraw(TDC &dc);
   void UnifLoadDraw(TDC &dc);
};

typedef TIArrayAsVector<TLoadDisplayObject> TILoadDisplayObjectContainer;
typedef TIArrayAsVectorIterator<TLoadDisplayObject> TILoadDisplayObjectIterator;

class TSelLoadDisplayObject : public TLoadDisplayObject, public TSelectableObject
{
public:
   TSelLoadDisplayObject(TBridgeDisplayObjectManager *doMgr,BridgeLoad *Load,bool selected = false);
   virtual ~TSelLoadDisplayObject();

protected:
   TColor GetDrawingColor() const;
   void ConcLoadSetup(TMapper &mapr);
   void UnifLoadSetup(TMapper &mapr);
};

#endif