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

#if !defined SUPRTDO_H
#define SUPRTDO_H

#include <qconbrdg\dispobj.h>
#include <qconbrdg\selobj.h>
#include <qconbrdg\bcompdo.h>
#include <bridge\support.h>
#include <owltools\realmapr.h>
#include <winsys\geometry.h>
#include <owl\gdiobjec.h>

#define otSupport 2

class TSupportDisplayObject : public TDisplayObject, public TBridgeComponentDisplayObject
{
public:
   TSupportDisplayObject(TBridgeDisplayObjectManager *doMgr,Support *aSupport);
   ~TSupportDisplayObject();
   void Draw(TDC &dc);
   void Setup(TMapper &mapr);
   int ObjectType() const;

   void SetSupport(Support *newSupport);
   Support *GetSupport() const;

   void SetSymbolColor(TColor newColor);
   TColor GetSymbolColor() const;
   virtual TColor GetSymbolDrawingColor() const;
   TColor GetLabelDrawingColor() const;

   bool operator == (const TSupportDisplayObject &sdo) const;
   bool operator <  (const TSupportDisplayObject &sdo) const;

public:
   TPoint Start;
   TPoint End;

protected:
   Support *support;
   int numSegments;
   TPoint *segments;
   TColor symbolColor;

   void DrawRoller(TDC &dc);
   void DrawPinned(TDC &dc);
   void DrawFixed(TDC &dc,bool isPier);
};

typedef TIArrayAsVector<TSupportDisplayObject> TISupportDisplayObjectContainer;
typedef TIArrayAsVectorIterator<TSupportDisplayObject> TISupportDisplayObjectIterator;

class TSelSupportDisplayObject : public TSupportDisplayObject, public TSelectableObject
{
public:
   TSelSupportDisplayObject(TBridgeDisplayObjectManager *doMgr,Support *support,bool selected = false);
   ~TSelSupportDisplayObject();

   TColor GetDrawingColor() const;
   TColor GetSymbolDrawingColor() const;

   void Setup(TMapper &mapr);
#if defined GEN_DEBUG_CODE
   void Draw(TDC &dc);
#endif
};

#endif