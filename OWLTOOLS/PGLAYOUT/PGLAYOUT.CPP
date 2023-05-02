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

#include <owltools\pglayout.h>
#include <owltools\fonthelp.h>
#include <stdlib.h>
#include <owl\appdict.h>
#include <owl\applicat.h>
#include <owl\gdiobjec.h>

//
// Header/Footer Items (HF_Item)
//

HF_Item::HF_Item(tLONG line,HFPosition pos) :
line(line),pos(pos)
{
}

tLONG HF_Item::GetLine() const
{
   return line;
}

HFPosition HF_Item::GetPosition() const
{
   return pos;
}

void HF_Item::Draw(TRect &rect,int signY,tLONG cyChar)
{
   tLONG x,y;
   char buffer[256];

   GetText(buffer,sizeof(buffer));

   y = signY*cyChar*line + rect.top;

   switch(pos)
      {
      case hfLeft:
           x = rect.left;
           layout->TextOut(TPoint(x,y),buffer);
           break;

      case hfCenter:
           layout->CenterTextOut(y,buffer);
           break;

      case hfRight:
           x = rect.right;
           layout->RightTextOut(TPoint(x,y),buffer);
           break;
      }
}

void HF_Item::AssociateLayout(TPageLayout *lo)
{
   layout = lo;
}

// -----------------------------

MiscText::MiscText(const char *text,tLONG line,HFPosition pos) :
HF_Item(line,pos),
text(text)
{
}

char *MiscText::GetText(char *buffer,tLONG maxChar)
{
   strncpy(buffer,text.c_str(),maxChar);

   return buffer;
}

// -------------------

DateStamp::DateStamp(tLONG line,HFPosition pos) :
HF_Item(line,pos)
{
}

char *DateStamp::GetText(char *buffer,tLONG maxChar)
{
   TDate date;
   ostrstream osDate;

   osDate << date << ends;
   strncpy(buffer,osDate.str(),maxChar);
   delete[] osDate.str();

   return buffer;
}

// ----------------------

TimeStamp::TimeStamp(tLONG line,HFPosition pos,bool showDate) :
HF_Item(line,pos),
showDate(showDate)
{
}

void TimeStamp::ShowDate(bool show)
{
   showDate = show;
}

char *TimeStamp::GetText(char *buffer,tLONG maxChar)
{
   TTime time;
   ostrstream osTime;

   time.PrintDate(showDate);

   osTime << time << ends;
   strncpy(buffer,osTime.str(),maxChar);
   delete[] osTime.str();

   return buffer;
}

// ----------------------

PageNumber::PageNumber(tLONG line,HFPosition pos) :
HF_Item(line,pos)
{
}

char *PageNumber::GetText(char *buffer,tLONG maxChar)
{
   tLONG page;
   tLONG numPages;
   ostrstream osPage;

   page = layout->GetCurrentPage();
   numPages = layout->GetTotalPages();

   if (numPages > 0)
      osPage << "Page " << page << " of " << numPages << ends;
   else
      osPage << "Page " << page << ends;

   strncpy(buffer,osPage.str(),maxChar);
   delete[] osPage.str();

   return buffer;
}

//
// TPageLayout
//

TPageLayout::TPageLayout(TPrintDC &pdc):
dc(pdc)
{
   headerItems = new HFContainer(6,0,3);
   footerItems = new HFContainer(6,0,3);

   TSize vExt;
   TSize wExt;

   // Retain the sign of the ratio vExt.cx/wExt.cx and vExt.cy/wExt.cy
   // This ratio deterines if the x/y axes are left/right or up/down, repectively
   vExt = dc.GetViewportExt();
   wExt = dc.GetWindowExt();

   if ( (vExt.cx >= 0 && wExt.cx >= 0) ||
        (vExt.cx <  0 && wExt.cx <  0) )
      signX = 1;
   else
      signX = -1;

   if ( (vExt.cy >= 0 && wExt.cy >= 0) ||
        (vExt.cy <  0 && wExt.cy <  0) )
      signY = 1;
   else
      signY = -1;

   //
   // Default to no header or footer (width of printable area, 0 height)
   //
   rHeader = TRect(0,0,signX*dc.GetDeviceCaps(HORZRES),0);
   rFooter = TRect(0,0,signX*dc.GetDeviceCaps(HORZRES),0);

   //
   // Set margins (default = 1")
   //
   int logPixelsX,logPixelsY;
   logPixelsX = dc.GetDeviceCaps(LOGPIXELSX); // dots per inch
   logPixelsY = dc.GetDeviceCaps(LOGPIXELSY); // dots per inch

   // Map to logical units
   TPoint p(logPixelsX,logPixelsY);
   dc.DPtoLP(&p);

   // Remove sign (margins are absolute measurements)
   p.x = abs(p.x);
   p.y = abs(p.y);

   leftMargin   = p.x;
   topMargin    = p.y;
   rightMargin  = p.x;
   bottomMargin = p.y;

   currentPage = -1;
   totalPages = -1;

   pHeaderFont = CreateSimpleFont(dc);
   pFooterFont = CreateSimpleFont(dc);
   pTextFont   = CreateSimpleFont(dc);
   pCurrentFont = 0;

   UpdatePageLayout();
}

TPageLayout::~TPageLayout()
{
   delete headerItems;
   delete footerItems;
   delete pHeaderFont;
   delete pFooterFont;
   delete pTextFont;
}

void TPageLayout::SetMargins(tLONG lm,tLONG tm,tLONG rm,tLONG bm)
{
   // margins are in logical units
   leftMargin   = lm;
   topMargin    = tm;
   rightMargin  = rm;
   bottomMargin = bm;

   UpdatePageLayout();
}

void TPageLayout::GetMargins(tLONG &lm,tLONG &tm,tLONG &rm,tLONG &bm)
{
   lm = leftMargin;
   tm = topMargin;
   rm = rightMargin;
   bm = bottomMargin;
}

void TPageLayout::SetHeaderFont(const char *typeFace,int pointSize,
                                int weight,uint8 italic,uint8 underline)
{
   delete pHeaderFont;
   pHeaderFont = CreateSimpleFont(dc,typeFace,pointSize,weight,italic,underline);
   UpdatePageLayout();
}

void TPageLayout::SetFooterFont(const char *typeFace,int pointSize,
                                int weight,uint8 italic,uint8 underline)
{
   delete pFooterFont;
   pFooterFont = CreateSimpleFont(dc,typeFace,pointSize,weight,italic,underline);
   UpdatePageLayout();
}

void TPageLayout::SetTextFont(const char *typeFace,int pointSize,
                              int weight,uint8 italic,uint8 underline)
{
   delete pTextFont;
   pTextFont = CreateSimpleFont(dc,typeFace,pointSize,weight,italic,underline);
   UpdatePageLayout();
}

void TPageLayout::SetCurrentPage(tLONG p)
{
   currentPage = p;
}

tLONG TPageLayout::GetCurrentPage() const
{
   return currentPage;
}

void TPageLayout::SetTotalPages(tLONG p)
{
   totalPages = p;
}

tLONG TPageLayout::GetTotalPages() const
{
   return totalPages;
}

void TPageLayout::AddHeaderItem(HF_Item *newItem)
{
   newItem->AssociateLayout(this);
   headerItems->Add(newItem);
   UpdatePageLayout();
}

void TPageLayout::AddFooterItem(HF_Item *newItem)
{
   newItem->AssociateLayout(this);
   footerItems->Add(newItem);
   UpdatePageLayout();
}

void TPageLayout::DrawHeader()
{
   TEXTMETRIC tm;
   tLONG cyChar;

   pCurrentFont = pHeaderFont;
   pHeaderFont->GetTextMetrics(tm,dc);
   cyChar = tm.tmHeight + tm.tmExternalLeading;

   HFIterator iter(*headerItems);
   while(iter)
      {
      HF_Item *item = iter++;
      item->Draw(GetHeaderRect(),signY,cyChar);
      }

   pCurrentFont = 0;
}

void TPageLayout::DrawFooter()
{
   TEXTMETRIC tm;
   tLONG cyChar;

   pCurrentFont = pFooterFont;
   pFooterFont->GetTextMetrics(tm,dc);
   cyChar = tm.tmHeight + tm.tmExternalLeading;

   HFIterator iter(*footerItems);
   while(iter)
      {
      HF_Item *item = iter++;
      item->Draw(GetFooterRect(),signY,cyChar);
      }

   pCurrentFont = 0;
}

TRect TPageLayout::GetHeaderRect()
{
   return rHeader;
}

TRect TPageLayout::GetPageRect()
{
   return rPage;
}

TRect TPageLayout::GetFooterRect()
{
   return rFooter;
}

tLONG TPageLayout::GetLinesPerPage() const
{
   TEXTMETRIC tm;
   tLONG cyChar;
   tLONG pageHeight;
   tLONG linesPerPage;

   pTextFont->GetTextMetrics(tm,dc);
   cyChar = tm.tmHeight + tm.tmExternalLeading;

   pageHeight = rPage.Height();

   linesPerPage = abs(pageHeight)/cyChar;

   return linesPerPage;
}

tLONG TPageLayout::GetAveCharsPerLine() const
{
   TEXTMETRIC tm;
   tLONG cxChar;
   tLONG pageWidth;
   tLONG aveCharsPerLine;

   pTextFont->GetTextMetrics(tm,dc);
   cxChar = tm.tmAveCharWidth;

   pageWidth = rPage.Width();

   aveCharsPerLine = pageWidth/cxChar;

   return aveCharsPerLine;
}

void TPageLayout::Rectangle(TRect &rect)
{
   dc.SelectObject(TPen(TColor::Black));
   dc.SelectStockObject(NULL_BRUSH);
   dc.Rectangle(rect);
   dc.RestoreBrush();
   dc.RestorePen();
}

void TPageLayout::TextOut(TPoint &p,const char *text)
{
   int bkMode;

   bkMode = dc.SetBkMode(TRANSPARENT);
   dc.SelectObject(pCurrentFont ? *pCurrentFont : *pTextFont);
   dc.TextOut(p,text);
   dc.SetBkMode(bkMode);
   dc.RestoreFont();
}

void TPageLayout::TextOut(TPoint &p,const string &text)
{
   TextOut(p,text.c_str());
}

void TPageLayout::LineTextOut(tLONG row,tLONG col,const char *text)
{
   TEXTMETRIC tm;
   tLONG cxChar, cyChar;

   dc.SelectObject(pCurrentFont ? *pCurrentFont : *pTextFont);
   dc.GetTextMetrics(tm);
   cxChar = tm.tmAveCharWidth;
   cyChar = tm.tmHeight;
   dc.RestoreFont();

   TPoint topLeft = rPage.TopLeft();
   TPoint offset(signX*col*cxChar,signY*row*cyChar);
   TPoint textPoint = topLeft + offset;

   TextOut(textPoint,text);
}

void TPageLayout::LineTextOut(tLONG row,tLONG col,const string &text)
{
   LineTextOut(row,col,text.c_str());
}

// Centers text within page rect
void TPageLayout::CenterTextOut(tLONG y,const char *text)
{
   dc.SelectObject(pCurrentFont ? *pCurrentFont : *pTextFont);
   TSize textExt = dc.GetTextExtent(text,strlen(text));
   TPoint p(rPage.left + (rPage.Width() - signX*textExt.cx)/2,y);
   dc.RestoreFont();

   TextOut(p,text);
}

void TPageLayout::CenterTextOut(tLONG y,const string &text)
{
   CenterTextOut(y,text.c_str());
}

// Centers text between two points
void TPageLayout::CenterTextOut(tLONG x1,tLONG x2,tLONG y,const char *text)
{
   dc.SelectObject(pCurrentFont ? *pCurrentFont : *pTextFont);
   TSize textExt = dc.GetTextExtent(text,strlen(text));
   TPoint p((x1+x2)/2 - signX*textExt.cx/2,y);
   dc.RestoreFont();

   TextOut(p,text);
}

void TPageLayout::CenterTextOut(tLONG x1,tLONG x2,tLONG y,const string &text)
{
   CenterTextOut(x1,x2,y,text.c_str());
}

void TPageLayout::RightTextOut(TPoint &p,const char *text)
{
   TPoint leftPoint(p);
   dc.SelectObject(pCurrentFont ? *pCurrentFont : *pTextFont);
   TSize textExt = dc.GetTextExtent(text,strlen(text));
   leftPoint.x -= signX*textExt.cx;
   dc.RestoreFont();

   TextOut(leftPoint,text);
}

void TPageLayout::RightTextOut(TPoint &p,const string &text)
{
   RightTextOut(p,text.c_str());
}

void TPageLayout::UpdatePageLayout()
{
   TRect rPrintable;       // printable area of the page
   TPoint pPhysPageSize;   // physical page size
   TPoint pPrintingOffset; // printing offset
   tLONG leftMarginOffset;   // distance between the left printable edge and the left margin
   tLONG topMarginOffset;    // distance between the top printable edge and the top margin
   tLONG rightMarginOffset;  // distance between the right printable edge and the right margin
   tLONG bottomMarginOffset; // distance between the bottom printable edge and the bottommargin
   tLONG numHeaderLines;     // number of lines in the header
   tLONG numFooterLines;     // number of lines in the footer
   tLONG headerHeight;       // height of header in logical units
   tLONG footerHeight;       // height of footer in logical units
   TEXTMETRIC tm;
   tLONG cyChar;

   bool isWin32s;
   OSVERSIONINFO osVerInfo;
   osVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
   ::GetVersionEx(&osVerInfo);
   isWin32s = (osVerInfo.dwPlatformId == VER_PLATFORM_WIN32s);

   //
   // Get information about the paper size and available printing area
   //
#if defined(BI_PLAT_WIN16)
   dc.Escape(GETPHYSPAGESIZE,0,0,&pPhysPageSize); // device units
#else
   if (isWin32s)
      {
      dc.Escape(GETPHYSPAGESIZE,0,0,&pPhysPageSize); // device units
      }
   else
      {
      pPhysPageSize.x = dc.GetDeviceCaps(PHYSICALWIDTH); // device units
      pPhysPageSize.y = dc.GetDeviceCaps(PHYSICALHEIGHT); // device units
      }
#endif

   dc.DPtoLP(&pPhysPageSize); // Map to logical units

#if defined(BI_PLAT_WIN16)
   dc.Escape(GETPRINTINGOFFSET,0,0,&pPrintingOffset); // device units
#else
   if (isWin32s)
      {
      dc.Escape(GETPRINTINGOFFSET,0,0,&pPrintingOffset); // device units
      }
   else
      {
      pPrintingOffset.x = dc.GetDeviceCaps(PHYSICALOFFSETX); // device units
      pPrintingOffset.y = dc.GetDeviceCaps(PHYSICALOFFSETY); // device units
      }
#endif

   dc.DPtoLP(&pPrintingOffset); // Map to logical units

   // Get available printing area (# of pixels)
   TPoint pPrintingSize(dc.GetDeviceCaps(HORZRES),
                        dc.GetDeviceCaps(VERTRES));
   dc.DPtoLP(&pPrintingSize); // Map to logical units

   //
   // Compute margin offsets
   // If offset is less than zero, specified margin is not within the printable area of the page.
   // Move the margin to the edge of the printable area.
   //

   leftMarginOffset = leftMargin - abs(pPrintingOffset.x);
   leftMarginOffset = (leftMarginOffset < 0) ? 0 : leftMarginOffset;

   rightMarginOffset = rightMargin - (abs(pPhysPageSize.x) - abs(pPrintingOffset.x) - abs(pPrintingSize.x));
   rightMarginOffset = (rightMarginOffset < 0) ? 0 : rightMarginOffset;

   topMarginOffset = topMargin - abs(pPrintingOffset.y);
   topMarginOffset = (topMarginOffset < 0) ? 0 : topMarginOffset;

   bottomMarginOffset = bottomMargin - (abs(pPhysPageSize.y) - abs(pPrintingOffset.y) - abs(pPrintingSize.y));
   bottomMarginOffset = (bottomMarginOffset < 0) ? 0 : bottomMarginOffset;

   // Printable Rectangle
   rPrintable = TRect(0,0,pPrintingSize.x,pPrintingSize.y);

   // Adjust header
   numHeaderLines = 0;
   HFIterator headerIter(*headerItems);
   while(headerIter)
      {
      HF_Item *item = headerIter++;
      // +1 because lines use zero based indexing
      numHeaderLines = max(numHeaderLines,item->GetLine()+1);
      }

   dc.SelectObject(*pHeaderFont);
   dc.GetTextMetrics(tm);
   cyChar = tm.tmHeight + tm.tmExternalLeading;
   headerHeight = numHeaderLines * cyChar;
   dc.RestoreFont();

   rHeader.left   = rPrintable.left + signX*leftMarginOffset;
   rHeader.top    = rPrintable.top  + signY*topMarginOffset;
   rHeader.right  = rPrintable.right - signX*rightMarginOffset;
   rHeader.bottom = rHeader.top + signY*headerHeight;

   // Adjust footer
   numFooterLines = 0;
   HFIterator footerIter(*footerItems);
   while(footerIter)
      {
      HF_Item *item = footerIter++;
      // +1 because lines use zero based indexing
      numFooterLines = max(numFooterLines,item->GetLine()+1);
      }

   dc.SelectObject(*pFooterFont);
   dc.GetTextMetrics(tm);
   cyChar = tm.tmHeight + tm.tmExternalLeading;
   footerHeight = numFooterLines * cyChar;
   dc.RestoreFont();

   rFooter.left   = rPrintable.left + signX*leftMarginOffset;
   rFooter.bottom = rPrintable.bottom - signY*bottomMarginOffset;
   rFooter.right  = rPrintable.right - signX*rightMarginOffset;
   rFooter.top    = rFooter.bottom - signY*footerHeight;

   // Adjust page
   rPage.left   = rHeader.left;
   rPage.top    = rHeader.bottom;
   rPage.right  = rHeader.right;
   rPage.bottom = rFooter.top;
}


bool TPageLayout::DrawBitmap(tLONG x, tLONG y, tLONG h, tLONG w, int resourceid)
{
    TPoint p(x,y);
    return DrawBitmap(p,h,w,resourceid);
}

bool TPageLayout::DrawBitmap(TPoint &p,tLONG h,tLONG w,int resourceid)
{
    TPoint pDummy;

	 TDib* dib;
    dib = new TDib(GetApplicationObject()->GetInstance(), TResId(resourceid));
//
//	 try
//	    {
//	 	 dib = new TDib(GetApplicationObject()->GetInstance(), TResId(resourceid));
//   	 }
//	 catch (TGdiBase::TXGdi&)
//   	 {
//	    ::MessageBox(0,"Cannot access bitmap resource!","Error",MB_OK);
//   	 return FALSE;
//	    }
//
   // Convert placement point to device coordinates
   dc.LPtoDP(&p);

   // Convert height and width to device coordinates.
   // If h or w < 0 use the size of the bitmap, otherwise
   // convert from logical to device coordinates
   if (h < 0 && w < 0)
      {
      // Use actual bitmap size
      h = dib->Height(); // in device coordinates
      w = dib->Width(); // in device coordinates
      }
   else if (h > 0 && w > 0)
      {
      // Use size provided, convert to device units
      pDummy = TPoint(w,h);
      dc.LPtoDP(&pDummy);
      w = pDummy.x;
      h = pDummy.y;
      }
   else if (h > 0 && w < 0)
      {
      // Use provided h, compute w such that the aspect ratio of the bitmap
      // is maintained.
      pDummy = TPoint(0,signY*h);
      dc.LPtoDP(&pDummy);
      h = pDummy.y;

      w = ::MulDiv(dib->Width(),h,dib->Height());

      // if bitmap width was even or odd, make sure that w is the same
      if ( (((dib->Width() % 2) == 0) && ((w % 2) != 0)) ||
           (((dib->Width() % 2) != 0) && ((w % 2) == 0)) )
         w++;
      }
   else if (h < 0 && w > 0)
      {
      // Use provided w, compute h such that the aspect ratio of the bitmap
      // is maintained.
      pDummy = TPoint(signX*w,0);
      dc.LPtoDP(&pDummy);
      w = pDummy.x;

      h = ::MulDiv(dib->Height(),w,dib->Width());

      // if bitmap height was even or odd, make sure that h is the same
      if ( (((dib->Height() % 2) == 0) && ((h % 2) != 0)) ||
           (((dib->Height() % 2) != 0) && ((h % 2) == 0)) )
         h++;
      }
   else
      {
      // this case not possible
      }

   // Create dest rectangle in device coordinates
   TRect dest(p,TSize(w,h));

   // Map back to logical units
   dc.DPtoLP((TPoint *)&dest,2);

	TRect src (0, 0, dib->Width(), dib->Height ()); // device units

	dc.StretchDIBits (dest, src, *dib);
	delete dib;

	return true;
}

void TPageLayout::Line(TPoint from,TPoint to)
{
   dc.MoveTo(from);
   dc.LineTo(to);
}

tLONG TPageLayout::ComputeFontHeight(TFont *pFont)
{
   TEXTMETRIC tm;
   int height;

   dc.SelectObject(*pFont);
   dc.GetTextMetrics(tm);
   height = tm.tmHeight + tm.tmExternalLeading;
   dc.RestoreFont();

   return height;
}


//
// TTableLayout
//

TTableLayout::TTableLayout(TPrintDC &pdc) :
TPageLayout(pdc)
{
   tableWidth = 0;

   columns = new TIColumnContainer(5,0,5);

   title = new char[1];
   title[0] = 0;

   titleFont = NULL;
   titleFont = CreateSimpleFont(dc,"Times New Roman");
   UpdateTitleHeight();

   headingFont = NULL;
   headingFont = CreateSimpleFont(dc,"Times New Roman");
   UpdateHeadingHeight();

   cellFont = NULL;
   cellFont = CreateSimpleFont(dc,"Times New Roman");
   UpdateCellHeight();
}

TTableLayout::~TTableLayout()
{
   delete[] title;
   delete columns;
}

void TTableLayout::SetTitle(const char *Title)
{
   delete[] title;
   title = new char[strlen(Title)+1];
   strcpy(title,Title);
}

void TTableLayout::SetTitleFont(const char *typeFace,int pointSize,int weight,uint8 italic,uint8 underline)
{
   titleFont = CreateSimpleFont(dc,typeFace,pointSize,weight,italic,underline);
   UpdateTitleHeight();
}

void TTableLayout::SetHeadingFont(const char *typeFace,int pointSize,int weight,uint8 italic,uint8 underline)
{
   headingFont = CreateSimpleFont(dc,typeFace,pointSize,weight,italic,underline);
   UpdateHeadingHeight();
}

void TTableLayout::SetCellFont(const char *typeFace,int pointSize,int weight,uint8 italic,uint8 underline)
{
   cellFont = CreateSimpleFont(dc,typeFace,pointSize,weight,italic,underline);
   UpdateCellHeight();
}

void TTableLayout::DefineColumn(const char *Heading,int width)
{
   TTableLayout::TColumn *column;
   TPoint refPoint(rPage.left + signX*tableWidth,rPage.top + signY*titleHeight);

   column = new TTableLayout::TColumn(Heading,refPoint,width);
   columns->Add(column);

   tableWidth += width;
}

void TTableLayout::DefineColumn(tLONG numCols,char *Headings[])
{
   tLONG col;
   tLONG width;

   width = abs(rPage.Width())/numCols;

   for (col = 0; col < numCols; col++)
      DefineColumn(Headings[col],width);
}

void TTableLayout::DefineColumn(tLONG numCols,char *Headings[],int *ratios)
{
   tLONG col;
   tLONG width;
   tLONG totalWidth;

   // Compute the total width
   totalWidth = 0;
   for (col = 0; col < numCols; col++)
      {
      totalWidth += ratios[col];
      }

   // Compute the width of each column and define it
   for (col = 0; col < numCols; col++)
      {
      width = ::MulDiv(ratios[col],abs(rPage.Width()),totalWidth);
      DefineColumn(Headings[col],width);
      }
}

tLONG TTableLayout::GetRowsPerPage()
{
   tLONG availRowHeight = abs(rPage.Height());
   tLONG numRows;

   // deduct for title
   availRowHeight -= titleHeight;

   // deduct for headings
   availRowHeight -= headingHeight;

   numRows = availRowHeight/cellHeight;

   return numRows;
}

void TTableLayout::DrawTable()
{
   TPoint left,right;

   // Border the entire table
   Rectangle(rPage);

   // Draw title
   dc.SelectObject(*titleFont);
   CenterTextOut(rPage.top,title);
   dc.RestoreFont();

   // Draw line below title (locate with resptect to the page)
   left = TPoint(rPage.left,rPage.top + signY*titleHeight);
   right = TPoint(rPage.right,rPage.top + signY*titleHeight);
   Line(left,right);

   // Draw column headings
   TPoint top(rPage.left,rPage.top + signY*titleHeight);
   TPoint bottom(rPage.left,rPage.bottom);
   Line(top,bottom);

   dc.SelectObject(*headingFont);
   TIColumnIterator iter(*columns);
   while(iter)
      {
      TTableLayout::TColumn *column = iter++;
      CenterTextOut(column->refPoint.x,column->refPoint.x+column->width,column->refPoint.y,column->heading);

      TPoint offset(signX*column->width,0);
      top += offset;
      bottom += offset;
      Line(top,bottom);
      }
   dc.RestoreFont();

   left = TPoint(rPage.left,rPage.top + signY*(titleHeight+headingHeight));
   right = TPoint(rPage.right,rPage.top + signY*(titleHeight+headingHeight));
   Line(left,right);
}

void TTableLayout::TableTextOut(tLONG row,tLONG col,const char *text)
{
   TTableLayout::TColumn *column = (*columns)[col];
   TPoint p;

   p.x = column->refPoint.x + signX*(column->width);
   p.y = column->refPoint.y + signY*(row*cellHeight + headingHeight);

   dc.SelectObject(*cellFont);
   RightTextOut(p,text);
   dc.RestoreFont();
}

void TTableLayout::HorizGridLine(tLONG row,tLONG col)
{
   TTableLayout::TColumn *column = (*columns)[(col < 0) ? 0 : col];
   TPoint pFrom, pTo;

   pFrom.y = column->refPoint.y + signY*((row+1)*cellHeight + headingHeight);
   pTo.y = pFrom.y;

   pFrom.x = rPage.left;
   pTo.x = rPage.right;

   Line(pFrom,pTo);
}

void TTableLayout::UpdateTitleHeight()
{
   titleHeight = ComputeFontHeight(titleFont);

   // Update column reference points for new font height...
   TIColumnIterator iter(*columns);
   tLONG cumWidth = 0;
   while(iter)
      {
      TTableLayout::TColumn *column = iter++;
      column->refPoint = TPoint(rPage.left + signX*cumWidth,
                                rPage.top  + signY*titleHeight);
      cumWidth += column->width;
      }
}

void TTableLayout::UpdateHeadingHeight()
{
   headingHeight = ComputeFontHeight(headingFont);
}

void TTableLayout::UpdateCellHeight()
{
   cellHeight = ComputeFontHeight(cellFont);
}

//
// TTableLayout::TColumn
//

TTableLayout::TColumn::TColumn(const char *Heading,TPoint p,tLONG w)
{
   refPoint = p;
   heading = new char[strlen(Heading)+1];
   strcpy(heading,Heading);
   width = w;
}

TTableLayout::TColumn::~TColumn()
{
   delete[] heading;
}

