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

/**********************************************************************
Description:
   Base class for a centered dialog. Centers the dialog on the screen.

Reference:
   "Centering dialog boxes in an OWL application"
   Borland C++ Developer's Journal
   July 1995
***********************************************************************/

#include <owl\dialog.h>
#include <owl\framewin.h>

class TCenteredDialog : public TDialog
{
public:
	TCenteredDialog(TWindow *parent,TResId resId,TModule *module = 0) :
      TDialog(parent,resId,module) {}

   void SetupWindow()
      {
      TDialog::SetupWindow();

      TRect rDialog  = GetWindowRect();
      TRect rMainWnd = GetApplication()->GetMainWindow()->GetWindowRect();

      TRect rWorkArea;
      bool bResult = ::SystemParametersInfo(SPI_GETWORKAREA,sizeof(TRect),&rWorkArea,0);
      if (!bResult)
         rWorkArea = TRect(0,0,::GetSystemMetrics(SM_CXSCREEN),::GetSystemMetrics(SM_CYSCREEN));

      TPoint newPosition;

      newPosition.x = rMainWnd.left + ((rMainWnd.Width() - rDialog.Width())/2);
      if (newPosition.x < rWorkArea.left)
         newPosition.x = rWorkArea.left;
      else if (newPosition.x+rDialog.Width() > rWorkArea.right)
         newPosition.x = rWorkArea.right - rDialog.Width();

      newPosition.y = rMainWnd.top + ((rMainWnd.Height() - rDialog.Height())/2);
      if (newPosition.y < rWorkArea.top)
         newPosition.y = rWorkArea.top;
      else if (newPosition.y+rDialog.Height() > rWorkArea.bottom)
         newPosition.y = rWorkArea.bottom - rDialog.Height();

      SetWindowPos(0,TRect(newPosition,rDialog.Size()),SWP_NOSIZE | SWP_NOZORDER);
      /*
      TPoint newOrigin(GetSystemMetrics(SM_CXSCREEN)/2,
                       GetSystemMetrics(SM_CYSCREEN)/2);
      TRect tempRect = GetWindowRect();
      newOrigin.x -= tempRect.Width()/2;
      newOrigin.y -= tempRect.Height()/2;

      MoveWindow(TRect(newOrigin,tempRect.Size()));
      */
      }
};