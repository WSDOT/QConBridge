///////////////////////////////////////////////////////////////////////
// OWLTools - Extension Library for the Borland OWL Framework
// Copyright (C) 1999  Washington State Department of Transportation
//                     Bridge and Structures Office
//
// This library was developed as part of the Alternate Route Project
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the Alternate Route Library Open Source License as published by 
// the Washington State Department of Transportation, Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but is distributed 
// AS IS, WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
// or FITNESS FOR A PARTICULAR PURPOSE. See the Alternate Route Library Open Source 
// License for more details.
//
// You should have received a copy of the Alternate Route Library Open Source License 
// along with this program; if not, write to the Washington State Department of 
// Transportation, Bridge and Structures Office, 4500 3rd Ave SE - P.O. Box  47340, 
// Olympia, WA 98503, USA or e-mail Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Revision Log
// ============
// May 1997 - Created, Richard Brice
// Oct 1999 - Released as open source

#include <owltools\arrow.h>

TArrowHead::TArrowHead(int l,int w)
{
   length = l;
   width = w;
}

void TArrowHead::Resize(int l,int w)
{
   length = l;
   width = w;
}

void TArrowHead::Draw(TPoint &point,TArrowHead::Direction dir,TDC &dc)
{
   switch(dir)
      {
      case arrowUp:
           DrawUpArrow(point,dc);
           break;

      case arrowDown:
           DrawDownArrow(point,dc);
           break;

      case arrowLeft:
           DrawLeftArrow(point,dc);
           break;

      case arrowRight:
           DrawRightArrow(point,dc);
           break;
      }
}

void TArrowHead::DrawUpArrow(TPoint &point,TDC &dc)
{
   TPoint pnts[3];

   pnts[0] = point;
   pnts[1] = TPoint(point.x-width/2,point.y + length/2);
   pnts[2] = TPoint(point.x+width/2,point.y + length/2);
   dc.Polygon(pnts,3);
}

void TArrowHead::DrawDownArrow(TPoint &point,TDC &dc)
{
   TPoint pnts[3];

   pnts[0] = point;
   pnts[1] = TPoint(point.x-width/2,point.y - length/2);
   pnts[2] = TPoint(point.x+width/2,point.y - length/2);
   dc.Polygon(pnts,3);
}

void TArrowHead::DrawLeftArrow(TPoint &point,TDC &dc)
{
   TPoint pnts[3];

   pnts[0] = point;
   pnts[1] = TPoint(point.x+length/2,point.y + width/2);
   pnts[2] = TPoint(point.x+length/2,point.y - width/2);
   dc.Polygon(pnts,3);
}

void TArrowHead::DrawRightArrow(TPoint &point,TDC &dc)
{
   TPoint pnts[3];

   pnts[0] = point;
   pnts[1] = TPoint(point.x-length/2,point.y + width/2);
   pnts[2] = TPoint(point.x-length/2,point.y - width/2);
   dc.Polygon(pnts,3);
}

