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

#if !defined DISPOBJ_H
#define DISPOBJ_H

#include <owl\gdiobjec.h>
#include <owl\color.h>
#include <owltools\mapper.h>

class TBridgeDisplayObjectManager;

class TDisplayObject
{
public:
   TDisplayObject(TBridgeDisplayObjectManager *doMgr,void *pObj);
   virtual ~TDisplayObject();

   void *GetObject() const;
   virtual int ObjectType() const = 0;
   TRegion &GetDrawRegion();
   TColor GetColor() const;
   void SetColor(TColor newColor);
   void SetPenSize(int size);
   int GetPenSize() const;
   virtual void Draw(TDC &dc) = 0;
   virtual void Setup(TMapper &mapr) = 0;

protected:
   void *ptrObject;
   TBridgeDisplayObjectManager *doMgr;
   TRegion drawRegion;
   TColor color;
   int penSize;

   virtual TColor GetDrawingColor() const;
   virtual int GetDrawingPenSize() const;
};

#endif