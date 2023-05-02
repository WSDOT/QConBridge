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

#include <owltools\colorbox.h>
#include <owl\gdiobjec.h>


TColorList::TColorList (TWindow* parent, int Id, TModule* module) :
TComboBox (parent, Id, 0, module)
{
}

TColorList::~TColorList()
{
}

void TColorList::SetupWindow ()
{
	TComboBox::SetupWindow ();

	COLORREF	rgb[] = {RGB (0x00, 0x00, 0x00), // black
							RGB (0x00, 0x00, 0xFF), // blue
							RGB (0x00, 0xFF, 0xFF), // cyan
							RGB (0x00, 0xFF, 0x00), // green
							RGB (0xFF, 0x00, 0xFF), // magenta
							RGB (0xFF, 0x00, 0x00), // red
							RGB (0xFF, 0xFF, 0x00), // yellow
							RGB (0xFF, 0xFF, 0xFF), // white
							RGB (0x00, 0x00, 0x80), // dk blue
							RGB (0x00, 0x80, 0x80), // dk cyan
							RGB (0x00, 0X80, 0x00), // dk green
							RGB (0x80, 0x00, 0x80), // dk magenta
							RGB (0x80, 0x00, 0x00), // dk red
							RGB (0x80, 0x80, 0x00), // dk yellow
							RGB (0x80, 0x80, 0x80), // dk gray
							RGB (0xC0, 0xC0, 0xC0)  // lt gray
                     };

   string color[] = {"Black",
                     "Blue",
                     "Cyan",
                     "Green",
                     "Magenta",
                     "Red",
                     "Yellow",
                     "White",
                     "Dk Blue",
                     "Dk Cyan",
                     "Dk Green",
                     "Dk Magenta",
                     "Dk Red",
                     "Dk Yellow",
                     "Dk Gray",
                     "Lt Gray"};

	for (int n = 0; n < 16; n++)
	   {
      AddString(color[n].c_str());
      SetItemData(n,rgb[n]);
	   }

	SetSelIndex(0);
}

uint TColorList::Transfer(void *buffer,TTransferDirection direction)
{
   if (direction == tdSetData)
      {
      int n;
      TColor *color = (TColor *)buffer;
      COLORREF colorRef = *color;

      for (n = 0; n < GetCount(); n++)
         {
         if (colorRef == GetItemData(n))
            break;
         }

      if (n == GetCount())
         return sizeof(TColor);

      SetSelIndex(n);
      }
   else
      {
      TColor *color = (TColor *)buffer;
      *color = GetItemData(GetSelIndex());
      }

   return sizeof(TColor);
}

TColor TColorList::GetColor()
{
   TColor color;
   Transfer(&color,tdGetData);
   return color;
}

void TColorList::SetColor(TColor color)
{
   Transfer(&color,tdSetData);
}

void TColorList::ODADrawEntire(DRAWITEMSTRUCT far& drawInfo)
{
   TDC dc(drawInfo.hDC);
   TRect colorBox(GetColorBox(drawInfo));
   TRect textBox(GetTextBox(drawInfo));

   DrawText(dc,colorBox,textBox,drawInfo);
}

void TColorList::ODAFocus(DRAWITEMSTRUCT far& drawInfo)
{
   TDC dc(drawInfo.hDC);

   if (drawInfo.itemState & ODS_SELECTED)
      ODASelect(drawInfo);

   dc.DrawFocusRect(GetTextBox(drawInfo));
}

void TColorList::ODASelect(DRAWITEMSTRUCT far& drawInfo)
{
   TDC dc(drawInfo.hDC);
   TRect colorBox(GetColorBox(drawInfo));
   TRect textBox(GetTextBox(drawInfo));
   TColor oldTextColor;
   TColor oldBkColor;

   // Setup colors for selected item
   // Draw the background rectangle, filled in the correct color
   if (drawInfo.itemState & ODS_SELECTED)
      {
      dc.SetTextColor (::GetSysColor (COLOR_HIGHLIGHTTEXT));
      dc.SetBkColor (::GetSysColor (COLOR_HIGHLIGHT));
      dc.FillRect(textBox,TBrush(::GetSysColor (COLOR_HIGHLIGHT)));
      }
   else
      {
      #ifndef __WIN32__
      dc.FillRect(textBox,TBrush((HBRUSH) SendMessage (WM_CTLCOLOR, (WPARAM) drawInfo.hDC, (LPARAM) HWindow)));
      #else
      dc.FillRect(textBox,TBrush((HBRUSH) SendMessage (WM_CTLCOLORSTATIC, (WPARAM) drawInfo.hDC, (LPARAM) HWindow)));
      #endif
      }

   DrawText(dc,colorBox,textBox,drawInfo);
}

TRect TColorList::GetColorBox(DRAWITEMSTRUCT far& drawInfo)
{
   TRect rc(drawInfo.rcItem);
	rc = rc.InflatedBy (-2, -2);

   TRect colorBox(rc.left,rc.top,rc.left+rc.Height(),rc.top+rc.Height());

   return colorBox;
}

TRect TColorList::GetTextBox(DRAWITEMSTRUCT far& drawInfo)
{
   TRect rc(drawInfo.rcItem);
	rc = rc.InflatedBy (-2, -2);

   TRect textBox(rc.left + rc.Height() + 2,rc.top,rc.right,rc.bottom);

   return textBox;
}

void TColorList::DrawText (TDC& dc, TRect& colorBox,TRect &textBox, DRAWITEMSTRUCT far& drawInfo)
{
   TColor bkColor = dc.GetBkColor();

   // Draw the color box
	dc.FillRect (colorBox, TBrush	((COLORREF) drawInfo.itemData));

   // If the list is not dropped and the color and bk color are the same
   // frame the color box in a contrasting color
   if ((COLORREF)bkColor == (COLORREF)drawInfo.itemData && !GetDroppedState())
      {
      if (drawInfo.itemData == TColor::White)
         dc.FrameRect (colorBox, TBrush(TColor::Black));
      else
         dc.FrameRect (colorBox, TBrush(TColor::White));
      }
   else if (drawInfo.itemData == ::GetSysColor(COLOR_WINDOW))
      dc.FrameRect (colorBox, TBrush(TColor::Black));

   // Draw the text
   dc.SetTextAlign(TA_TOP | TA_LEFT);
   dc.SetBkMode(TRANSPARENT);
   int len = GetStringLen(drawInfo.itemID);
   char *name = new char[len+1];
   GetString(name,drawInfo.itemID);
   dc.TextOut(textBox.TopLeft(),name,len);
   delete[] name;
}


