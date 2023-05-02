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

#include <owltools\arrowbtn.h>
#include <owl\gdiobjec.h>
#include <winsys\geometry.h>

// Taken from "windows.h"
#define OBM_UPARROW         32753
#define OBM_DNARROW         32752
#define OBM_UPARROWD        32743
#define OBM_DNARROWD        32742

TArrowButton::TArrowButton(TWindow* parent,int id,bool up,int X, int Y, int W, int H,TModule* module) :
TButton(parent,id,0,X,Y,W,H,false,module)
{
   Attr.Style |= BS_OWNERDRAW;

   Init(up);
}

TArrowButton::TArrowButton(TWindow *parent,int resourceId,bool up,TModule *module) :
TButton(parent,resourceId,module)
{
   Init(up);
}

void TArrowButton::Init(bool up)
{
   if (up)
      {
      hArrowUpBmp = ::LoadBitmap(NULL,MAKEINTRESOURCE(OBM_UPARROW));
      hArrowDnBmp = ::LoadBitmap(NULL,MAKEINTRESOURCE(OBM_UPARROWD));
      }
   else
      {
      hArrowUpBmp = ::LoadBitmap(NULL,MAKEINTRESOURCE(OBM_DNARROW));
      hArrowDnBmp = ::LoadBitmap(NULL,MAKEINTRESOURCE(OBM_DNARROWD));
      }
}

TArrowButton::~TArrowButton()
{
   DeleteObject(hArrowUpBmp);
   DeleteObject(hArrowDnBmp);
}

void TArrowButton::SetupWindow()
{
   TRACE("TArrowButton::SetupWindow");
   PRECONDITIONX((Attr.Style & BS_OWNERDRAW) == BS_OWNERDRAW,"Button is not owner draw");

   TButton::SetupWindow();
   TBitmap bmp( hArrowUpBmp );
   TRect rClient = GetClientRect();
   rClient.right = rClient.left + bmp.Width();
   rClient.bottom = rClient.top + bmp.Height();

   SetWindowPos(0,rClient,SWP_NOZORDER | SWP_NOMOVE);
}

void TArrowButton::ODADrawEntire(DRAWITEMSTRUCT far& drawInfo)
{
   TDC dc(drawInfo.hDC);
   TMemoryDC memDC(dc);
   TBitmap bmp( (drawInfo.itemState & ODS_SELECTED) ? hArrowDnBmp : hArrowUpBmp );

   memDC.SelectObject(bmp);
   dc.BitBlt(TRect(drawInfo.rcItem),memDC,TPoint(0,0));
   memDC.RestoreBitmap();

   if (drawInfo.itemState & ODS_FOCUS)
      ODAFocus(drawInfo);
}

void TArrowButton::ODAFocus(DRAWITEMSTRUCT far& drawInfo)
{
   TRect rect(drawInfo.rcItem);
   rect.Inflate(-4,-4);

   if (drawInfo.itemState & ODS_SELECTED)
      rect.Offset(1,1);

   ::DrawFocusRect(drawInfo.hDC,&rect);
}

void TArrowButton::ODASelect(DRAWITEMSTRUCT far& drawInfo)
{
   ODADrawEntire(drawInfo);
}
