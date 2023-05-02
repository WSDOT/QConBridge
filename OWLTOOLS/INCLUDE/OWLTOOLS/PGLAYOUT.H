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

#if !defined (PGLAYOUT_H)
#define PGLAYOUT_H

#include <owltools\owltldll.h>
#include <winsys/geometry.h>
#include <owl\dc.h>
#include <classlib\arrays.h>
#include <classlib\date.h>
#include <classlib\time.h>

#if defined (__FLAT__)
#define tLONG long
#else
#define tLONG int
#endif

class _OWLTOOLSCLASS TPageLayout;

typedef enum HFPosition {hfLeft, hfCenter, hfRight} HFPosition;

class _OWLTOOLSCLASS HF_Item
{
public:
   HF_Item(tLONG line,HFPosition pos);
   void Draw(TRect &rect,int signY,tLONG cyChar);
   tLONG GetLine() const;
   HFPosition GetPosition() const;
   void AssociateLayout(TPageLayout *lo);

   bool operator == (const HF_Item &item) const {return (pos == item.GetPosition() && line == item.GetLine());}

protected:
   tLONG line;
   HFPosition pos;
   TPageLayout *layout;

   virtual char *GetText(char *buffer,tLONG maxChar) = 0;
};

typedef TIArrayAsVector<HF_Item>         HFContainer;
typedef TIArrayAsVectorIterator<HF_Item> HFIterator;

class _OWLTOOLSCLASS MiscText : public HF_Item
{
public:
   MiscText(const char *text,tLONG line,HFPosition pos);

protected:
   string text;
   char *GetText(char *buffer,tLONG maxChar);
};

class _OWLTOOLSCLASS DateStamp : public HF_Item
{
public:
   DateStamp(tLONG line,HFPosition pos);

protected:
   char *GetText(char *buffer,tLONG maxChar);
};

class _OWLTOOLSCLASS TimeStamp : public HF_Item
{
public:
   TimeStamp(tLONG line,HFPosition pos,bool showDate = true);
   void ShowDate(bool show);

protected:
   char *GetText(char *buffer,tLONG maxChar);
   bool showDate;
};

class _OWLTOOLSCLASS PageNumber : public HF_Item
{
public:
   PageNumber(tLONG line,HFPosition pos);

protected:
   char *GetText(char *buffer,tLONG maxChar);
};

//
//
//

class _OWLTOOLSCLASS TPageLayout
{
public:
   TPageLayout(TPrintDC &pdc);
   virtual ~TPageLayout();

   void SetMargins(tLONG lm,tLONG tm,tLONG rm,tLONG bm);
   void GetMargins(tLONG &lm,tLONG &tm,tLONG &rm,tLONG &bm);
   void SetHeaderFont(const char *typeFace,int pointSize,
                      int weight = FW_NORMAL,uint8 italic = false,
                      uint8 underline = false);
   void SetFooterFont(const char *typeFace,int pointSize,
                      int weight = FW_NORMAL,uint8 italic = false,
                      uint8 underline = false);
   void SetTextFont(const char *typeFace,int pointSize,
                    int weight = FW_NORMAL,uint8 italic = false,
                    uint8 underline = false);

   void SetCurrentPage(tLONG p);
   tLONG GetCurrentPage() const;
   void SetTotalPages(tLONG p);
   tLONG GetTotalPages() const;

   void AddHeaderItem(HF_Item *newItem);
   void AddFooterItem(HF_Item *newItem);

   void DrawHeader();
   void DrawFooter();

   TRect GetHeaderRect();
   TRect GetPageRect();
   TRect GetFooterRect();

   tLONG GetLinesPerPage() const;
   tLONG GetAveCharsPerLine() const;

   // Text functions
   void TextOut(TPoint &p,const char *text);
   void TextOut(TPoint &p,const string &text);
   void LineTextOut(tLONG row,tLONG col,const char *text);
   void LineTextOut(tLONG row,tLONG col,const string &text);
   void CenterTextOut(tLONG y,const char *text);
   void CenterTextOut(tLONG y,const string &text);
   void CenterTextOut(tLONG x1,tLONG x2,tLONG y,const char *text);
   void CenterTextOut(tLONG x1,tLONG x2,tLONG y,const string &text);
   void RightTextOut(TPoint &p,const char *text);
   void RightTextOut(TPoint &p,const string &text);

   void Rectangle(TRect &rect);
   void Line(TPoint from,TPoint to);
   bool DrawBitmap(tLONG x, tLONG y,tLONG h,tLONG w, int resourceid);
   bool DrawBitmap(TPoint &p,tLONG h,tLONG w,int resourceid);

protected:
   TPrintDC &dc;

   tLONG currentPage;
   tLONG totalPages;

   TRect rHeader;
   TRect rPage;
   TRect rFooter;

   HFContainer *headerItems;
   HFContainer *footerItems;

   tLONG leftMargin;
   tLONG topMargin;
   tLONG rightMargin;
   tLONG bottomMargin;

   TFont *pHeaderFont;
   TFont *pFooterFont;
   TFont *pTextFont;
   TFont *pCurrentFont;

   int signX; // sign of vExt.cx/wExt.cx
   int signY; // sign of vExt.cy/wExt.cy

   void UpdatePageLayout();
   tLONG  ComputeFontHeight(TFont *pFont);
};

class _OWLTOOLSCLASS TTableLayout : public TPageLayout
{
public:
   TTableLayout(TPrintDC &pdc);
   virtual ~TTableLayout();
   void SetTitle(const char *Title);
   void SetTitleFont(const char *typeFace,int pointSize,int weight = FW_NORMAL,uint8 italic = false,uint8 underline = false);
   void SetHeadingFont(const char *typeFace,int pointSize,int weight = FW_NORMAL,uint8 italic = false,uint8 underline = false);
   void SetCellFont(const char *typeFace,int pointSize,int weight = FW_NORMAL,uint8 italic = false,uint8 underline = false);
   void DefineColumn(const char *Heading,int width);
   void DefineColumn(tLONG numCols,char *Headings[]);
   void DefineColumn(tLONG numCols,char *Headings[],int *ratios);
   tLONG GetRowsPerPage();
   void DrawTable();
   void TableTextOut(tLONG row,tLONG col,const char *text);
   void HorizGridLine(tLONG row,tLONG col = -1);

protected:
   struct TColumn
      {
      TColumn(const char *Heading,TPoint p,tLONG w);
      ~TColumn();

      bool operator == (const TTableLayout::TColumn &c) const {return (true);}

      char *heading;
      TPoint refPoint;
      tLONG width; // width in logical units
      };

   typedef TIArrayAsVector<TTableLayout::TColumn> TIColumnContainer;
   typedef TIArrayAsVectorIterator<TTableLayout::TColumn> TIColumnIterator;

   char *title;
   TFont *titleFont;
   TFont *headingFont;
   TFont *cellFont;
   tLONG titleHeight;
   tLONG headingHeight;
   tLONG cellHeight;
   tLONG tableWidth;
   TIColumnContainer *columns;

   void UpdateTitleHeight();
   void UpdateHeadingHeight();
   void UpdateCellHeight();

   friend TTableLayout::TColumn;
};

#endif
