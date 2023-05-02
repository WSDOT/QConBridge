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

#if !defined(credit_h)
#define credit_h

#include <owltools\owltldll.h>
#include <owl\window.h>
#include <owl\gdiobjec.h>

class _OWLTOOLSCLASS TCreditWindow : public TWindow {
public:
    TBitmap *Bitmap;
    BITMAP BitSize;
    int ScrollUnit;
    int ScrollRate;
    int ScrollPos;
    int FontHeight;
    LPSTR* StringList;
    int StringCount;

    TCreditWindow(TWindow *parent,
                  TResId bitmapResId,
                  LPSTR AStringList[],
                  TModule *module = 0);
    virtual ~TCreditWindow();
    virtual char far* GetClassName() {return "TCreditWindow";};
    virtual void GetWindowClass(WNDCLASS& WC);
    virtual void SetupWindow();           //First place HWindow is valid
    virtual void Destroy(int retVal);
    virtual void DrawBitmap(TDC &DC,int X, int Y, int W, int H);
    virtual void Paint(TDC &dc,bool erase,TRect &rect);
    virtual void ShowCredits();
    virtual void EvTimer(uint timerId);

    DECLARE_RESPONSE_TABLE(TCreditWindow);
};

#endif // credit_h