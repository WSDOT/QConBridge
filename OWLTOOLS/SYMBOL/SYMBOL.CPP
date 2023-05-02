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

#include <owltools\symbol.h>

TSymbol::TSymbol ()
{
}

TSymbol::~TSymbol()
{
}

//
// TCircleSymbol
//

TCircleSymbol::TCircleSymbol() :
TSymbol()
{
}

void TCircleSymbol::Draw(TDC &dc,int size,TPoint &point)
{
   TPoint currentPos;

   dc.GetCurrentPosition(currentPos);
   TPoint upLeft(point.x-size/2,point.y-size/2);
   dc.Ellipse(upLeft,TSize(size,size));
   dc.MoveTo(currentPos);
}

//
// TSquareSymbol
//

TSquareSymbol::TSquareSymbol() :
TSymbol()
{
}

void TSquareSymbol::Draw(TDC &dc,int size,TPoint &point)
{
   TPoint p[4];
   TPoint currentPos;

   dc.GetCurrentPosition(currentPos);

   GetSquare(point,size,p);
   dc.Polygon(p,4);
   dc.MoveTo(currentPos);
}

void TSquareSymbol::GetSquare(TPoint &point,int size,TPoint *p)
{
   p[0] = TPoint(point.x-size/2,point.y-size/2);
   p[1] = TPoint(p[0].x + size,p[0].y);
   p[2] = TPoint(p[1].x,p[1].y + size);
   p[3] = TPoint(p[2].x - size,p[2].y);
}

//
// TTickSquare
//

TTickSquareSymbol::TTickSquareSymbol() :
TSymbol()
{
}

void TTickSquareSymbol::Draw(TDC &dc,int size,TPoint &point)
{
   TPoint p[4];
   TPoint currentPos;

   dc.GetCurrentPosition(currentPos);

   size = (size < 10) ? 10 : size;

   GetSquare(point,size,p);
   dc.Polygon(p,4);
   dc.MoveTo(point.x,point.y-size/2);
   dc.LineTo(point.x,point.y+size/2);
   dc.MoveTo(currentPos);
}

void TTickSquareSymbol::GetSquare(TPoint &point,int size,TPoint *p)
{
   int boxSize = size/2;

   p[0] = TPoint(point.x-boxSize/2,point.y-boxSize/2);
   p[1] = TPoint(p[0].x + boxSize,p[0].y);
   p[2] = TPoint(p[1].x,p[1].y + boxSize);
   p[3] = TPoint(p[2].x - boxSize,p[2].y);
}

//
// TDiamondSymbol
//

TDiamondSymbol::TDiamondSymbol() :
TSymbol()
{
}

void TDiamondSymbol::Draw(TDC &dc,int size,TPoint &point)
{
   TPoint p[4];
   TPoint currentPos;

   dc.GetCurrentPosition(currentPos);

   GetDiamond(point,size,p);
   dc.Polygon(p,4);
   dc.MoveTo(currentPos);
}

void TDiamondSymbol::GetDiamond(TPoint &point,int size,TPoint *p)
{
   p[0] = TPoint(point.x-size/2,point.y);
   p[1] = TPoint(point.x,       point.y - size/2);
   p[2] = TPoint(point.x+size/2,point.y);
   p[3] = TPoint(point.x,       point.y + size/2);
}

//
// TUpTriangleSymbol
//

TUpTriangleSymbol::TUpTriangleSymbol() :
TSymbol()
{
}

void TUpTriangleSymbol::Draw(TDC &dc,int size,TPoint &point)
{
   TPoint p[3];
   TPoint currentPos;

   dc.GetCurrentPosition(currentPos);

   GetTriangle(point,size,p);
   dc.Polygon(p,3);
   dc.MoveTo(currentPos);
}

void TUpTriangleSymbol::GetTriangle(TPoint &point,int size,TPoint *p)
{
   p[0] = TPoint(point.x-size/2,point.y+size/2);
   p[1] = TPoint(p[0].x + size,p[0].y);
   p[2] = TPoint(point.x,point.y-size/2);
}

//
// TDnTriangleSymbol
//

TDnTriangleSymbol::TDnTriangleSymbol() :
TSymbol()
{
}

void TDnTriangleSymbol::Draw(TDC &dc,int size,TPoint &point)
{
   TPoint p[3];
   TPoint currentPos;

   dc.GetCurrentPosition(currentPos);

   GetTriangle(point,size,p);
   dc.Polygon(p,3);
   dc.MoveTo(currentPos);
}

void TDnTriangleSymbol::GetTriangle(TPoint &point,int size,TPoint *p)
{
   p[0] = TPoint(point.x-size/2,point.y-size/2);
   p[1] = TPoint(p[0].x + size,p[0].y);
   p[2] = TPoint(point.x,point.y+size/2);
}

//
// TPlusSymbol
//

TPlusSymbol::TPlusSymbol() :
TSymbol()
{
}

void TPlusSymbol::Draw(TDC &dc,int size,TPoint &point)
{
   TPoint currentPos;

   dc.GetCurrentPosition(currentPos);
   dc.MoveTo(point.x,point.y-size/2);
   dc.LineTo(point.x,point.y+size/2);
   dc.MoveTo(point.x-size/2,point.y);
   dc.LineTo(point.x+size/2,point.y);
   dc.MoveTo(currentPos);
}

//
// TCrossSymbol
//

TCrossSymbol::TCrossSymbol() :
TSymbol()
{
}

void TCrossSymbol::Draw(TDC &dc,int size,TPoint &point)
{
   TPoint currentPos;

   dc.GetCurrentPosition(currentPos);
   dc.MoveTo(point.x-size/2,point.y-size/2);
   dc.LineTo(point.x+size/2,point.y+size/2);
   dc.MoveTo(point.x+size/2,point.y-size/2);
   dc.LineTo(point.x-size/2,point.y+size/2);
   dc.MoveTo(currentPos);
}

//
// TAsteriskSymbol
//

TAsteriskSymbol::TAsteriskSymbol() :
TSymbol()
{
}

void TAsteriskSymbol::Draw(TDC &dc,int size,TPoint &point)
{
   TPoint currentPos;

   dc.GetCurrentPosition(currentPos);

   // plus
   dc.MoveTo(point.x,point.y-size/2);
   dc.LineTo(point.x,point.y+size/2);
   dc.MoveTo(point.x-size/2,point.y);
   dc.LineTo(point.x+size/2,point.y);
   // cross
   dc.MoveTo(point.x-size/2,point.y-size/2);
   dc.LineTo(point.x+size/2,point.y+size/2);
   dc.MoveTo(point.x+size/2,point.y-size/2);
   dc.LineTo(point.x-size/2,point.y+size/2);

   dc.MoveTo(currentPos);
}

//
// THourGlassSymbol
//

THourGlassSymbol::THourGlassSymbol() :
TSymbol()
{
}

void THourGlassSymbol::Draw(TDC &dc,int size,TPoint &point)
{
   TPoint currentPos;

   dc.GetCurrentPosition(currentPos);
   dc.MoveTo(point.x-size/2,point.y-size/2);
   dc.LineTo(point.x+size/2,point.y+size/2);
   dc.LineTo(point.x-size/2,point.y+size/2);
   dc.LineTo(point.x+size/2,point.y-size/2);
   dc.LineTo(point.x-size/2,point.y-size/2);
   dc.MoveTo(currentPos);
}

//
// TNullSymbol
//

TNullSymbol::TNullSymbol() :
TSymbol()
{
   drawEnabled = false;
}

void TNullSymbol::Draw(TDC &dc,int /*size*/,TPoint &point)
{
   // if drawing is enabled, the "(none)" is drawn...
   // this is convienent if you want to draw symbols in things
   // like listboxes...

   // Drawing not enabled... get the heck outta here
   if (!drawEnabled)
      return;

   string text("(none)");
   TSize textExt = dc.GetTextExtent(text.c_str(),text.length());
   dc.TextOut(point.x - textExt.cx/2,point.y - textExt.cy/2,text.c_str());
}
