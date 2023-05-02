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

#if !defined SYMBOL_H
#define SYMBOL_H

#include <owl\color.h>
#include <owl\gdiobjec.h>

enum TSymbolType
   {stCircle,
    stFilledCircle,
    stSquare,
    stFilledSquare,
    stUpTriangle,
    stFilledUpTriangle,
    stDnTriangle,
    stFilledDnTriangle,
    stTickSquare,
    stFilledTickSquare,
    stDiamond,
    stFilledDiamond,
    stTick,
    stPlus,
    stCross,
    stAsterisk,
    stHourGlass,
    stNull};

class _OWLCLASS TSymbol
{
public:
   TSymbol ();
   virtual ~TSymbol();
   virtual TSymbolType Type() = 0;
   void Draw(TDC &dc,int size,TColor color,TPoint &point);

protected:
   virtual void DrawMe(TDC &dc,int size,TColor color,TPoint &point) = 0;

private:
   HPEN hOldPen;
   HPEN hNewPen;
   HBRUSH hOldBrush;
   HBRUSH hNewBrush;
   TPoint currentPosition;

   void PreDraw(TDC &dc,TColor color,TPoint &point);
   void PostDraw(TDC &dc);
};

class _OWLCLASS TCircleSymbol : public TSymbol
{
public:
   TCircleSymbol();
   TSymbolType Type() {return stCircle;}

protected:
   void DrawMe(TDC &dc,int size,TColor color,TPoint &point);
};

class _OWLCLASS TFilledCircleSymbol : public TCircleSymbol
{
public:
   TFilledCircleSymbol();
   TSymbolType Type() {return stFilledCircle;}

protected:
   void DrawMe(TDC &dc,int size,TColor color,TPoint &point);
};

class _OWLCLASS TSquareSymbol : public TSymbol
{
public:
   TSquareSymbol();
   TSymbolType Type() {return stSquare;}

protected:
   void DrawMe(TDC &dc,int size,TColor color,TPoint &point);
   void GetSquare(TPoint &point,int size,TPoint *p);
};

class _OWLCLASS TFilledSquareSymbol : public TSquareSymbol
{
public:
   TFilledSquareSymbol();
   TSymbolType Type() {return stFilledSquare;}

protected:
   void DrawMe(TDC &dc,int size,TColor color,TPoint &point);
};

class _OWLCLASS TTickSquareSymbol : public TSymbol
{
public:
   TTickSquareSymbol();
   TSymbolType Type() {return stTickSquare;}

protected:
   void DrawMe(TDC &dc,int size,TColor color,TPoint &point);
   void GetSquare(TPoint &point,int size,TPoint *p);
};

class _OWLCLASS TFilledTickSquareSymbol : public TTickSquareSymbol
{
public:
   TFilledTickSquareSymbol();
   TSymbolType Type() {return stFilledTickSquare;}

protected:
   void DrawMe(TDC &dc,int size,TColor color,TPoint &point);
};

class _OWLCLASS TTickSymbol : public TSymbol
{
public:
   TTickSymbol();
   TSymbolType Type() {return stTick;}

protected:
   void DrawMe(TDC &dc,int size,TColor color,TPoint &point);
};

class _OWLCLASS TDiamondSymbol : public TSymbol
{
public:
   TDiamondSymbol();
   TSymbolType Type() {return stDiamond;}

protected:
   void DrawMe(TDC &dc,int size,TColor color,TPoint &point);
   void GetDiamond(TPoint &point,int size,TPoint *p);
};

class _OWLCLASS TFilledDiamondSymbol : public TDiamondSymbol
{
public:
   TFilledDiamondSymbol();
   TSymbolType Type() {return stFilledDiamond;}

protected:
   void DrawMe(TDC &dc,int size,TColor color,TPoint &point);
};

class _OWLCLASS TUpTriangleSymbol : public TSymbol
{
public:
   TUpTriangleSymbol();
   TSymbolType Type() {return stUpTriangle;}

protected:
   void DrawMe(TDC &dc,int size,TColor color,TPoint &point);
   void GetTriangle(TPoint &point,int size,TPoint *p);
};

class _OWLCLASS TFilledUpTriangleSymbol : public TUpTriangleSymbol
{
public:
   TFilledUpTriangleSymbol();
   TSymbolType Type() {return stFilledUpTriangle;}

protected:
   void DrawMe(TDC &dc,int size,TColor color,TPoint &point);
};

class _OWLCLASS TDnTriangleSymbol : public TSymbol
{
public:
   TDnTriangleSymbol();
   TSymbolType Type() {return stDnTriangle;}

protected:
   void DrawMe(TDC &dc,int size,TColor color,TPoint &point);
   void GetTriangle(TPoint &point,int size,TPoint *p);
};

class _OWLCLASS TFilledDnTriangleSymbol : public TDnTriangleSymbol
{
public:
   TFilledDnTriangleSymbol();
   TSymbolType Type() {return stFilledDnTriangle;}

protected:
   void DrawMe(TDC &dc,int size,TColor color,TPoint &point);
};

class _OWLCLASS TPlusSymbol : public TSymbol
{
public:
   TPlusSymbol();
   TSymbolType Type() {return stPlus;}

protected:
   void DrawMe(TDC &dc,int size,TColor color,TPoint &point);
};

class _OWLCLASS TCrossSymbol : public TSymbol
{
public:
   TCrossSymbol();
   TSymbolType Type() {return stCross;}

protected:
   void DrawMe(TDC &dc,int size,TColor color,TPoint &point);
};

class _OWLCLASS TAsteriskSymbol : public TSymbol
{
public:
   TAsteriskSymbol();
   TSymbolType Type() {return stAsterisk;}

protected:
   void DrawMe(TDC &dc,int size,TColor color,TPoint &point);
};

class _OWLCLASS THourGlassSymbol : public TSymbol
{
public:
   THourGlassSymbol();
   TSymbolType Type() {return stHourGlass;}

protected:
   void DrawMe(TDC &dc,int size,TColor color,TPoint &point);
};

class _OWLCLASS TNullSymbol : public TSymbol
{
public:
   TNullSymbol();
   TSymbolType Type() {return stNull;}
   void EnableDraw(bool enable) {drawEnabled = enable;}

protected:
   void DrawMe(TDC &dc,int size,TColor color,TPoint &point);
   bool drawEnabled;
};

#endif // SYMBOL_H