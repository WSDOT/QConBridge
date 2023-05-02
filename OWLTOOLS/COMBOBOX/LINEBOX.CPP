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

#include "linebox.h"
#include <owl\gdiobjec.h>

TLineList::TLineList (TWindow* parent, int Id, TModule* module) :
TComboBox(parent,Id,0,module)
{
}

TLineList::~TLineList()
{
}

void TLineList::SetupWindow()
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
}

void TLineList::ODADrawEntire(DRAWITEMSTRUCT far& drawInfo)
{
   TDC dc(drawInfo.hDC);
   TRect rc(drawInfo.rcItem);

   DrawText(dc,rc,drawInfo);
}

void TLineList::ODAFocus(DRAWITEMSTRUCT far& drawInfo)
{
   TDC dc(drawInfo.hDC);
   TRect rc(drawInfo.rcItem);

   dc.DrawFocusRect(rc);

   DrawText(dc,rc,drawInfo);
}

void TLineList::ODASelect(DRAWITEMSTRUCT far& drawInfo)
{
   TDC dc(drawInfo.hDC);
   TRect rc(drawInfo.rcItem);
   TColor oldTextColor;
   TColor oldBkColor;

   if (drawInfo.itemState & ODS_SELECTED)
      {
      oldTextColor = dc.SetTextColor (GetSysColor (COLOR_HIGHLIGHTTEXT));
      oldBkColor = dc.SetBkColor (GetSysColor (COLOR_HIGHLIGHT));
      dc.FillRect(rc,TBrush(GetSysColor (COLOR_HIGHLIGHT)));
      }
   else
      {
	#ifndef __WIN32__
      dc.FillRect(rc,TBrush((HBRUSH) SendMessage (WM_CTLCOLOR, (WPARAM) drawInfo.hDC, (LPARAM) HWindow)));
	#else
      dc.FillRect(rc,TBrush((HBRUSH) SendMessage (WM_CTLCOLORSTATIC, (WPARAM) drawInfo.hDC, (LPARAM) HWindow)));
	#endif
      oldTextColor = dc.GetTextColor();
      oldBkColor = dc.GetBkColor();
      }

   DrawText(dc,rc,drawInfo);
   dc.SetBkColor(oldBkColor);
   dc.SetTextColor(oldTextColor);
}

void TLineList::DrawText (TDC& dc, TRect& rc, DRAWITEMSTRUCT far& drawInfo)
{
   rc = rc.InflatedBy(-8,0);

   TColor oldTextColor = dc.GetTextColor();

   if (drawInfo.itemState & ODS_SELECTED)
      oldTextColor = dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));

   dc.SelectObject(TPen(dc.GetTextColor(),1,(int)drawInfo.itemData));
   dc.MoveTo(rc.left,rc.top+rc.Height()/2);
   dc.LineTo(rc.right,rc.top+rc.Height()/2);
   dc.RestorePen();
   dc.SetTextColor(oldTextColor);
}
