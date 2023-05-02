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

#if !defined SPANDO_H
#define SPANDO_H

#include <qconbrdg\dispobj.h>
#include <qconbrdg\selobj.h>
#include <qconbrdg\bcompdo.h>
#include <bridge\span.h>
#include <classlib\arrays.h>

#define otSpan 1

class TSpanDisplayObject : public TDisplayObject, public TBridgeComponentDisplayObject
{
public:
   TPoint Start;
   TPoint End;

protected:
   Span *span;
   int numHinges;
   TPoint hingePnt[2];
   int numSegments;
   TPoint *segments;

public:
   TSpanDisplayObject(TBridgeDisplayObjectManager *doMgr,Span *aSpan);
   ~TSpanDisplayObject();
   void Draw(TDC &dc);
   void Setup(TMapper &map);
   int ObjectType() const;
   Span *GetSpan() const;
   void SetSpan(Span *newSpan);
   TColor GetLabelDrawingColor() const;

   bool operator == (const TSpanDisplayObject &sdo) const;
   bool operator <  (const TSpanDisplayObject &sdo) const;
};

typedef TISArrayAsVector<TSpanDisplayObject> TISSpanDisplayObjectContainer;
typedef TISArrayAsVectorIterator<TSpanDisplayObject> TISSpanDisplayObjectIterator;

class TSelSpanDisplayObject : public TSpanDisplayObject, public TSelectableObject
{
public:
   TSelSpanDisplayObject(TBridgeDisplayObjectManager *doMgr,Span *span,bool selected = false);
   ~TSelSpanDisplayObject();

   TColor GetDrawingColor() const;
   void Setup(TMapper &mapr);
#if defined GEN_DEBUG_CODE
   void Draw(TDC &dc);
#endif
};

#endif
