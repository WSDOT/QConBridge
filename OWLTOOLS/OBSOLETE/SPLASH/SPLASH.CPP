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


#include <owltools\splash.h>


//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(TSplashWindow, TFrameWindow)
//{{TSplashWindowRSP_TBL_BEGIN}}
    EV_WM_LBUTTONDOWN,
    EV_WM_TIMER,
//{{TSplashWindowRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TSplashWindow Implementation}}


TSplashWindow::TSplashWindow (TWindow* parent,int resId,uint Duration, TModule* module):
    TFrameWindow(parent, 0, new TBitmapWindow(0,resId), true, module)
{
    // INSERT>> Your constructor code here.
   duration = Duration;
   Attr.Style = WS_POPUP | WS_VISIBLE;
}


TSplashWindow::~TSplashWindow ()
{
    Destroy();

    // INSERT>> Your destructor code here.

}

void TSplashWindow::SetupWindow ()
{
    TFrameWindow::SetupWindow();

    // INSERT>> Your code here.

	SetWindowPos(HWND_TOPMOST, TRect(), SWP_NOMOVE | SWP_NOSIZE);

	TRect	SplashRect = GetWindowRect();
	RECT	ScreenRectRECT;
	::GetWindowRect(GetDesktopWindow(),&ScreenRectRECT);
	TRect ScreenRect(ScreenRectRECT.left,
						  ScreenRectRECT.top,
						  ScreenRectRECT.right,
						  ScreenRectRECT.bottom);
	int offsetX = (int) (ScreenRect.Width() - SplashRect.Width()) / 2;
	int offsetY = (int) (ScreenRect.Height() - SplashRect.Height()) / 2;
	SetWindowPos(  HWND_TOPMOST,	//Make it on top of its parent
				   offsetX,			//Center horizontally
				   offsetY,			//Center vertically
				   0,				//N/A
				   0,           	//N/A
				   SWP_NOSIZE); 	//Ignore width and height params

	SetTimer(1, duration, NULL);
	SetCapture();
   Invalidate();
}

void TSplashWindow::EvLButtonDown (uint modKeys, TPoint& point)
{
    TFrameWindow::EvLButtonDown(modKeys, point);

    // INSERT>> Your code here.
	TFrameWindow::CloseWindow();

}


void TSplashWindow::EvTimer (uint timerId)
{
    TFrameWindow::EvTimer(timerId);

    // INSERT>> Your code here.
	TFrameWindow::CloseWindow();
}


void TSplashWindow::CloseWindow (int retVal)
{
    TFrameWindow::CloseWindow(retVal);

    // INSERT>> Your code here.
	KillTimer(1);
	ReleaseCapture();

}

