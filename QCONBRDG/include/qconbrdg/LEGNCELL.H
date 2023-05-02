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

#if !defined (LEGNCELL_H)
#define LEGNCELL_H

#include <owl\gdiobjec.h>
#include <owl\dc.h>
#include <owl\color.h>
#include <owltools\symbol.h>
#include <cstring.h>
#include <classlib\arrays.h>

class TLegendCell
{
public:
   TLegendCell();
   ~TLegendCell();
   virtual void Draw(TDC &dc,TPoint &p);

   virtual int GetWidth(TDC &dc);
   virtual void SetWidth(int w);

   virtual int GetHeight(TDC &dc);
   virtual void SetHeight(int h);
   bool operator == (const TLegendCell &) const {return true;}

protected:
   int width;
   int height;
};

class TLinestyleCell : public TLegendCell
{
public:
   TLinestyleCell(const char *Title,TColor color,int penStyle,TSymbol *symbol);
   ~TLinestyleCell();
   void Draw(TDC &dc,TPoint &p);

   int GetWidth(TDC &dc);
   void SetWidth(int w);

   int GetHeight(TDC &dc);
   void SetHeight(int h);

protected:
   char *title;
   TColor color;
   int penStyle;
   TSymbol *symbol;
   string typeFace;
   int pointSize;
};

class TTextCell : public TLegendCell
{
public:
   TTextCell(const char *Title,const char *Value);
   ~TTextCell();

   void Draw(TDC &dc,TPoint &p);

   void SetValue(const char *Value);

   int GetWidth(TDC &dc);
   void SetWidth(int w);

   int GetHeight(TDC &dc);
   void SetHeight(int h);

protected:
   char *title;
   char *value;
   string typeFace;
   int pointSize;
};

class TLegend
{
public:
   TLegend();
   virtual ~TLegend();
   void AddCell(TLegendCell *cell);
   TLegendCell *GetCell(int idx);
   void Draw(TDC &dc,TRect &rect);
   void Clear();

protected:
   typedef TIArrayAsVector<TLegendCell> TILegendCellContainer;
   typedef TIArrayAsVectorIterator<TLegendCell> TILegendCellIterator;

   TILegendCellContainer *cells;
};

#endif