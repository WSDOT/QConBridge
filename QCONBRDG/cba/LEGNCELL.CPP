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

#include <qconbrdg\legncell.h>
#include <owltools\fonthelp.h>

TLegend::TLegend()
{
   cells = new TILegendCellContainer(10,0,10);
}

TLegend::~TLegend()
{
   delete cells;
}

void TLegend::AddCell(TLegendCell *cell)
{
   cells->Add(cell);
}

TLegendCell *TLegend::GetCell(int idx)
{
   return (*cells)[idx];
}

void TLegend::Draw(TDC &dc,TRect &rect)
{
   int cellWidth;
   int cellHeight;
   int numLines;
   int cellsPerLine;
   int numCells;

   // Get the maximum cell height and width
   // We will be making all the cells the same size
   cellWidth = 0;
   cellHeight = 0;
   TILegendCellIterator i(*cells);
   while (i)
      {
      TLegendCell *cell = i++;
      cell->SetWidth(0);
      cellWidth = max(cellWidth,cell->GetWidth(dc));

      cell->SetHeight(0);
      cellHeight = max(cellHeight,cell->GetHeight(dc));
      }

   numCells = cells->GetItemsInContainer();

   cellsPerLine = max(rect.Width()/cellWidth,1); // Min 1 cell per line (even if it doesn't fit)
   numLines = numCells/cellsPerLine;
   numLines += (numCells % cellsPerLine == 0) ? 0 : 1;

   //
   // Draw cells
   //
   for (int line = 0; line < numLines; line++)
      {
      for (int c = 0; c < cellsPerLine; c++)
         {
         int idx = line*cellsPerLine + c;
         if (idx >= numCells)
            break;

         TLegendCell *cell = GetCell(idx);

         cell->SetWidth(cellWidth);
         cell->SetHeight(cellHeight);

         TPoint p(rect.left+c*cellWidth,rect.top+line*cellHeight);

         cell->Draw(dc,p);
         }
      }
}

void TLegend::Clear()
{
   cells->Flush();
}

//
//
//

TLegendCell::TLegendCell()
{
   width = 0;
   height = 0;
}

TLegendCell::~TLegendCell()
{
}

void TLegendCell::Draw(TDC &dc,TPoint &p)
{
   //
   // Draw Border
   //
   dc.SelectStockObject(BLACK_PEN);
   dc.FrameRect(TRect(p,TSize(GetWidth(dc),GetHeight(dc))),TBrush(TColor::Black));
}

int TLegendCell::GetWidth(TDC &/*dc*/)
{
   return width;
}

void TLegendCell::SetWidth(int w)
{
   width = w;
}

int TLegendCell::GetHeight(TDC &/*dc*/)
{
   return height;
}

void TLegendCell::SetHeight(int h)
{
   height = h;
}

//
//
//

TLinestyleCell::TLinestyleCell(const char *Title,TColor color,int penStyle,TSymbol *symbol) :
TLegendCell(),
color(color),
penStyle(penStyle),
symbol(symbol)
{
   title = new char[strlen(Title)+1];
   strcpy(title,Title);

   typeFace = "Courier New";
   pointSize = -8;
}

TLinestyleCell::~TLinestyleCell()
{
   delete[] title;
}

void TLinestyleCell::Draw(TDC &dc,TPoint &p)
{
   int bkMode;
   int textAlign;
   TEXTMETRIC tm;
   int cxChar;
   int cyChar;
   int logPixelsX;
   TPoint start,end,center;
   TPen linePen(color,1,penStyle);
   TPen symbolPen(color,1,PS_SOLID);
   TBrush symbolBrush(color);
   TFont *font;

   TLegendCell::Draw(dc,p);

   //
   // Draw Text
   //
   font = CreateSimpleFont(dc,typeFace.c_str(),pointSize);
   dc.SelectObject(*font);

   dc.GetTextMetrics(tm);
   cxChar = tm.tmAveCharWidth;
   cyChar = tm.tmHeight + tm.tmExternalLeading;

   bkMode = dc.SetBkMode(TRANSPARENT);
   textAlign = dc.SetTextAlign(TA_TOP | TA_LEFT);

   dc.TextOut(p.x + cxChar,p.y,title);

   //
   // Draw Line
   //
   logPixelsX = dc.GetDeviceCaps(LOGPIXELSX); // dots per inch
   TSize titleSize;
   titleSize = dc.GetTextExtent(title,strlen(title));

   start = TPoint(p.x + 2*cxChar + titleSize.cx,p.y + cyChar/2);
   end   = TPoint(p.x +   cxChar + titleSize.cx + logPixelsX/2,p.y + cyChar/2);
   center = TPoint((end.x+start.x)/2,(end.y+start.y)/2);

   dc.SelectObject(linePen);
   dc.MoveTo(start);
   dc.LineTo(end);

   //
   // Draw Symbol
   //
   dc.SelectObject(symbolPen);
   dc.SelectObject(symbolBrush);
   if (symbol)
      symbol->Draw(dc,logPixelsX/16,center);

   //
   // Clean up
   //
   dc.RestoreBrush();
   dc.RestorePen();
   dc.RestoreFont();
   dc.SetTextAlign(textAlign);
   dc.SetBkMode(bkMode);

   delete font;
}

int TLinestyleCell::GetWidth(TDC &dc)
{
   // if the cell width is set to a non-zero value, that value is used
   // otherwise, it is computed
   if (width != 0)
      return width;

   TEXTMETRIC tm;
   TSize extents;
   int cxChar;
   int logPixelsX;
   int w;
   TFont *font;

   font = CreateSimpleFont(dc,typeFace.c_str(),pointSize);
   dc.SelectObject(*font);

   dc.GetTextMetrics(tm);
   cxChar = tm.tmAveCharWidth;

   extents = dc.GetTextExtent(title,strlen(title));

   logPixelsX = dc.GetDeviceCaps(LOGPIXELSX); // dots per inch

   w = extents.cx + 3*cxChar + logPixelsX/2;

   dc.RestoreFont();
   delete font;

   return w;
}

void TLinestyleCell::SetWidth(int w)
{
   width = w;
}

int TLinestyleCell::GetHeight(TDC &dc)
{
   if (height != 0)
      return height;

   TEXTMETRIC tm;
   int cyChar;
   int h;
   TFont *font;

   font = CreateSimpleFont(dc,typeFace.c_str(),pointSize);
   dc.SelectObject(*font);

   dc.GetTextMetrics(tm);
   cyChar = tm.tmHeight + tm.tmExternalLeading;

   h = cyChar;

   dc.RestoreFont();
   delete font;

   return h;
}

void TLinestyleCell::SetHeight(int h)
{
   height = h;
}

//
//
//

TTextCell::TTextCell(const char *Title,const char *Value) :
TLegendCell()
{
   title = new char[strlen(Title)+1];
   strcpy(title,Title);

   value = new char[strlen(Value)+1];
   strcpy(value,Value);

   typeFace = "Courier New";
   pointSize = -8;
}

TTextCell::~TTextCell()
{
   delete[] title;
   delete[] value;
}

void TTextCell::Draw(TDC &dc,TPoint &p)
{
   int bkMode;
   int textAlign;
   TEXTMETRIC tm;
   int cxChar;
   TSize titleExtents;
   TFont *font;

   TLegendCell::Draw(dc,p);

   //
   // Draw Text
   //
   font = CreateSimpleFont(dc,typeFace.c_str(),pointSize);
   dc.SelectObject(*font);

   dc.GetTextMetrics(tm);
   cxChar = tm.tmAveCharWidth;

   bkMode = dc.SetBkMode(TRANSPARENT);
   textAlign = dc.SetTextAlign(TA_TOP | TA_LEFT);

   dc.TextOut(p.x + cxChar,p.y,title);

   titleExtents = dc.GetTextExtent(title,strlen(title));

   dc.TextOut(p.x + cxChar + titleExtents.cx + cxChar, p.y, value);

   //
   // Clean up
   //
   dc.RestoreFont();
   dc.SetTextAlign(textAlign);
   dc.SetBkMode(bkMode);

   delete font;
}

void TTextCell::SetValue(const char *Value)
{
   delete[] value;

   value = new char[strlen(Value)+1];
   strcpy(value,Value);
}

int TTextCell::GetWidth(TDC &dc)
{
   if (width != 0)
      return width;

   TEXTMETRIC tm;
   TSize extents;
   int w,w1,w2;
   int cxChar;
   TFont *font;

   font = CreateSimpleFont(dc,typeFace.c_str(),pointSize);
   dc.SelectObject(*font);

   dc.GetTextMetrics(tm);
   cxChar = tm.tmAveCharWidth;

   extents = dc.GetTextExtent(title,strlen(title));
   w1 = extents.cx;

   extents = dc.GetTextExtent(value,strlen(value));
   w2 = extents.cx;

   w = w1 + 3*cxChar + w2;

   dc.RestoreFont();
   delete font;

   return w;

}

void TTextCell::SetWidth(int w)
{
   width = w;
}

int TTextCell::GetHeight(TDC &dc)
{
   if (height != 0)
      return height;

   TEXTMETRIC tm;
   int cyChar;
   int h;
   TFont *font;

   font = CreateSimpleFont(dc,typeFace.c_str(),pointSize);
   dc.SelectObject(*font);

   dc.GetTextMetrics(tm);
   cyChar = tm.tmHeight + tm.tmExternalLeading;

   h = cyChar;

   dc.RestoreFont();
   delete font;

   return h;
}

void TTextCell::SetHeight(int h)
{
   height = h;
}


