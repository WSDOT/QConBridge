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

#include <owltools\penstyle.h>
#include <owl\gdiobjec.h>

TPenStyleList::TPenStyleList (TWindow* parent, int Id, TModule* module) :
TComboBox(parent,Id,0,module)
{
}

TPenStyleList::~TPenStyleList()
{
}

void TPenStyleList::SetupWindow()
{
   TComboBox::SetupWindow();

   int penStyle[] = {PS_SOLID,
                     PS_DASH,
                     PS_DOT,
                     PS_DASHDOT,
                     PS_DASHDOTDOT
                    };

   for (int n = 0; n < 5; n++)
      {
      AddString( (LPCSTR)penStyle[n]);
      }

	SetSelIndex(0);
}

uint TPenStyleList::Transfer(void *buffer,TTransferDirection direction)
{
   int *penStyle = (int *)buffer;

   if (direction == tdSetData)
      {
      int n;
      for (n = 0; n < GetCount(); n++)
         {
         if (*penStyle == GetItemData(n))
            break;
         }

      if (n == GetCount())
         return sizeof(int);

      SetSelIndex(n);
      }
   else
      {
      *penStyle = GetItemData(GetSelIndex());
      }

   return sizeof(int);
}

int TPenStyleList::GetPenStyle()
{
   int style;
   Transfer(&style,tdGetData);
   return style;
}

void TPenStyleList::SetPenStyle(int style)
{
   Transfer(&style,tdSetData);
}

void TPenStyleList::ODADrawEntire(DRAWITEMSTRUCT far& drawInfo)
{
   TDC dc(drawInfo.hDC);
   TRect rc(drawInfo.rcItem);

   DrawText(dc,rc,drawInfo);
}

void TPenStyleList::ODAFocus(DRAWITEMSTRUCT far& drawInfo)
{
   TDC dc(drawInfo.hDC);
   TRect rc(drawInfo.rcItem);

   if (drawInfo.itemState & ODS_SELECTED)
      ODASelect(drawInfo);

   dc.DrawFocusRect(rc);
}

void TPenStyleList::ODASelect(DRAWITEMSTRUCT far& drawInfo)
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

void TPenStyleList::DrawText (TDC& dc, TRect& rc, DRAWITEMSTRUCT far& drawInfo)
{
   rc = rc.InflatedBy(-8,0);

   dc.SelectObject(TPen(dc.GetTextColor(),1,(int)drawInfo.itemData));
   dc.MoveTo(rc.left,rc.top+rc.Height()/2);
   dc.LineTo(rc.right,rc.top+rc.Height()/2);
   dc.RestorePen();
}
