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

#include <owltools\symlist.h>
#include <owl\gdiobjec.h>

TSymbolList::TSymbolList (TWindow* parent, int Id, bool useFilled, TModule* module) :
TComboBox(parent,Id,0,module),
useFilled(useFilled)
{
   symbol[ 0] = new TNullSymbol();
   symbol[ 1] = new TPlusSymbol();
   symbol[ 2] = new TCrossSymbol();
   symbol[ 3] = new TAsteriskSymbol();
   symbol[ 4] = new THourGlassSymbol();
   symbol[ 5] = new TCircleSymbol();
   symbol[ 6] = new TSquareSymbol();
   symbol[ 7] = new TTickSquareSymbol();
   symbol[ 8] = new TDiamondSymbol();
   symbol[ 9] = new TUpTriangleSymbol();
   symbol[10] = new TDnTriangleSymbol();

   TNullSymbol *sym = TYPESAFE_DOWNCAST(symbol[0],TNullSymbol);
   sym->EnableDraw(true);
}

TSymbolList::~TSymbolList()
{
   for (int i = 0; i < 11; i++)
      delete symbol[i];
}

void TSymbolList::SetupWindow()
{
   TComboBox::SetupWindow();

   for (int n = 0; n < 11; n++)
      {
      AddString("");
      SetItemData(n,(uint32)symbol[n]);
      }
   SetSelIndex(0);
}

uint TSymbolList::Transfer(void *buffer,TTransferDirection direction)
{
   TSymbolType *type = (TSymbolType *)buffer;

   if (direction == tdSetData)
      {
      int n;
      for (n = 0; n < GetCount(); n++)
         {
         TSymbol *sym = (TSymbol *)GetItemData(n);
         if (*type == sym->Type())
            break;
         }

      if (n == GetCount())
         return sizeof(TSymbolType);

      SetSelIndex(n);
      }
   else
      {
      *type = ((TSymbol *)GetItemData(GetSelIndex()))->Type();
      }

   return sizeof(TSymbolType);
}

TSymbolType TSymbolList::GetSymbolType()
{
   TSymbolType type;
   Transfer(&type,tdGetData);
   return type;
}

void TSymbolList::SetSymbolType(TSymbolType type)
{
   Transfer(&type,tdSetData);
}

void TSymbolList::ODADrawEntire(DRAWITEMSTRUCT far& drawInfo)
{
   TDC dc(drawInfo.hDC);
   TRect rc(drawInfo.rcItem);

   DrawText(dc,rc,drawInfo);
}

void TSymbolList::ODAFocus(DRAWITEMSTRUCT far& drawInfo)
{
   TDC dc(drawInfo.hDC);
   TRect rc(drawInfo.rcItem);

   if (drawInfo.itemState & ODS_SELECTED)
      ODASelect(drawInfo);

   dc.DrawFocusRect(rc);
}

void TSymbolList::ODASelect(DRAWITEMSTRUCT far& drawInfo)
{
   TDC dc(drawInfo.hDC);
   TRect rc(drawInfo.rcItem);

   if (drawInfo.itemState & ODS_SELECTED)
      {
      dc.SetTextColor (GetSysColor (COLOR_HIGHLIGHTTEXT));
      dc.SetBkColor (GetSysColor (COLOR_HIGHLIGHT));
      dc.FillRect(rc,TBrush(GetSysColor (COLOR_HIGHLIGHT)));
      }
   else
      {
      #ifndef __WIN32__
      dc.FillRect(rc,TBrush((HBRUSH) SendMessage (WM_CTLCOLOR, (WPARAM) drawInfo.hDC, (LPARAM) HWindow)));
      #else
      dc.FillRect(rc,TBrush((HBRUSH) SendMessage (WM_CTLCOLORSTATIC, (WPARAM) drawInfo.hDC, (LPARAM) HWindow)));
      #endif
      }

   DrawText(dc,rc,drawInfo);
}

void TSymbolList::DrawText (TDC& dc, TRect& rc, DRAWITEMSTRUCT far& drawInfo)
{
   TPoint p(rc.left + rc.Width()/2,rc.top + rc.Height()/2);
   int size = rc.Height() - 2;
   dc.SelectObject(TPen(dc.GetTextColor(),1,PS_SOLID));
   if (useFilled)
      dc.SelectObject(TBrush(dc.GetTextColor()));
   else
      dc.SelectStockObject(NULL_BRUSH);

   TSymbol *s = (TSymbol *)drawInfo.itemData;
   s->Draw(dc,size,p);

   dc.RestoreObjects();
}
