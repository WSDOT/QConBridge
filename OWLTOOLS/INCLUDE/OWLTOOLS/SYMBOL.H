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

#include <owltools\owltldll.h>
#include <owl\color.h>
#include <owl\gdiobjec.h>

enum TSymbolType
   {stCircle,
    stSquare,
    stUpTriangle,
    stDnTriangle,
    stTickSquare,
    stDiamond,
    stPlus,
    stCross,
    stAsterisk,
    stHourGlass,
    stNull};

class _OWLTOOLSCLASS TSymbol
{
public:
   TSymbol ();
   virtual ~TSymbol();
   virtual TSymbolType Type() = 0;
   virtual void Draw(TDC &dc,int size,TPoint &point) = 0;
};

class _OWLTOOLSCLASS TCircleSymbol : public TSymbol
{
public:
   TCircleSymbol();
   TSymbolType Type() {return stCircle;}
   void Draw(TDC &dc,int size,TPoint &point);
};

class _OWLTOOLSCLASS TSquareSymbol : public TSymbol
{
public:
   TSquareSymbol();
   TSymbolType Type() {return stSquare;}
   void Draw(TDC &dc,int size,TPoint &point);

   protected:
   void GetSquare(TPoint &point,int size,TPoint *p);
};

class _OWLTOOLSCLASS TTickSquareSymbol : public TSymbol
{
public:
   TTickSquareSymbol();
   TSymbolType Type() {return stTickSquare;}
   void Draw(TDC &dc,int size,TPoint &point);

protected:
   void GetSquare(TPoint &point,int size,TPoint *p);
};

class _OWLTOOLSCLASS TDiamondSymbol : public TSymbol
{
public:
   TDiamondSymbol();
   TSymbolType Type() {return stDiamond;}
   void Draw(TDC &dc,int size,TPoint &point);

protected:
   void GetDiamond(TPoint &point,int size,TPoint *p);
};

class _OWLTOOLSCLASS TUpTriangleSymbol : public TSymbol
{
public:
   TUpTriangleSymbol();
   TSymbolType Type() {return stUpTriangle;}
   void Draw(TDC &dc,int size,TPoint &point);

protected:
   void GetTriangle(TPoint &point,int size,TPoint *p);
};

class _OWLTOOLSCLASS TDnTriangleSymbol : public TSymbol
{
public:
   TDnTriangleSymbol();
   TSymbolType Type() {return stDnTriangle;}
   void Draw(TDC &dc,int size,TPoint &point);

protected:
   void GetTriangle(TPoint &point,int size,TPoint *p);
};

class _OWLTOOLSCLASS TPlusSymbol : public TSymbol
{
public:
   TPlusSymbol();
   TSymbolType Type() {return stPlus;}
   void Draw(TDC &dc,int size,TPoint &point);
};

class _OWLTOOLSCLASS TCrossSymbol : public TSymbol
{
public:
   TCrossSymbol();
   TSymbolType Type() {return stCross;}
   void Draw(TDC &dc,int size,TPoint &point);
};

class _OWLTOOLSCLASS TAsteriskSymbol : public TSymbol
{
public:
   TAsteriskSymbol();
   TSymbolType Type() {return stAsterisk;}
   void Draw(TDC &dc,int size,TPoint &point);
};

class _OWLTOOLSCLASS THourGlassSymbol : public TSymbol
{
public:
   THourGlassSymbol();
   TSymbolType Type() {return stHourGlass;}
   void Draw(TDC &dc,int size,TPoint &point);
};

class _OWLTOOLSCLASS TNullSymbol : public TSymbol
{
public:
   TNullSymbol();
   TSymbolType Type() {return stNull;}
   void EnableDraw(bool enable) {drawEnabled = enable;}
   void Draw(TDC &dc,int size,TPoint &point);

protected:
   bool drawEnabled;
};

#endif // SYMBOL_H