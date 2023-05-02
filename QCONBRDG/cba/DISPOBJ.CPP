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

#include <qconbrdg\dispobj.h>
#include <qconbrdg\qcbapp.h>
#include <qconbrdg\bdomgr.h>
#include <qconbrdg\bview.h>
#include <owl\docmanag.h>

TDisplayObject::TDisplayObject(TBridgeDisplayObjectManager *doMgr,void *pObj) :
ptrObject(pObj),
doMgr(doMgr)
{
   color = TColor::Black;
   penSize = 2;
}

TDisplayObject::~TDisplayObject()
{
}

void *TDisplayObject::GetObject() const
{
   return ptrObject;
}

TRegion &TDisplayObject::GetDrawRegion()
{
   return drawRegion;
}

TColor TDisplayObject::GetColor() const
{
   return color;
}

void TDisplayObject::SetColor(TColor newColor)
{
   color = newColor;
}

TColor TDisplayObject::GetDrawingColor() const
{
   TColor color;
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(doMgr->bview->bridgeDoc.GetDocManager().GetApplication(),
                                              TQConBridgeApp);
   if(theApp->Printing)
      color = TColor::Black;
   else
      color = GetColor();

   return color;
}

void TDisplayObject::SetPenSize(int size)
{
   penSize = size;
}

int TDisplayObject::GetPenSize() const
{
   return penSize;
}

int TDisplayObject::GetDrawingPenSize() const
{
   return GetPenSize();
}

