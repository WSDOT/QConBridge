///////////////////////////////////////////////////////////////////////
// QConBridge - HL93 Live Load Analysis
// Copyright (C) 1999  Washington State Department of Transportation
//                     Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation, 
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but 
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source 
// License along with this program; if not, write to the Washington 
// State Department of Transportation, Bridge and Structures Office, 
// 4500 3rd AVE SE - P.O. Box  47340, Olympia, WA 98503, USA or e-mail 
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Revision Log
// ============
// May 1997 - Created, Richard Brice, PE
// Oct 1999 - Released as Open Source


#include <qconbrdg\mdiclnt.h>
#include <qconbrdg\mdichild.h>
#include <qconbrdg\qcbview.h>
#include <qconbrdg\qcbapp.h>
#include <qconbrdg\qcbapp.rh>


//{{TQConBridgeMDIClient Implementation}}


//
// Build a response table for all messages/commands handled
// by TQConBridgeMDIClient derived from TMDIClient.
//
DEFINE_RESPONSE_TABLE1(TQConBridgeMDIClient, TMDIClient)
//{{TQConBridgeMDIClientRSP_TBL_BEGIN}}
    EV_WM_DROPFILES,
    EV_COMMAND(CM_PRINT, CmPrint),
    EV_COMMAND(CM_FASTPRINT, CmFastPrint),
    EV_COMMAND(CM_PRINTERSETUP, CmPrinterSetup),
    EV_COMMAND_ENABLE(CM_PRINT, CePrintEnable),
    EV_COMMAND_ENABLE(CM_FASTPRINT, CePrintEnable),
    EV_COMMAND_ENABLE(CM_PRINTERSETUP, CePrintEnable),
    EV_WM_ERASEBKGND,
    EV_WM_SIZE,
//{{TQConBridgeMDIClientRSP_TBL_END}}
END_RESPONSE_TABLE;



DEFINE_HELPCONTEXT(TQConBridgeMDIClient)
  HCENTRY_MENU(CM_ADDSPAN,CM_ADDSPAN),
  HCENTRY_MENU(CM_EDITSPAN,CM_EDITSPAN),
  HCENTRY_MENU(CM_EDITSUPPORT,CM_EDITSUPPORT),
  HCENTRY_MENU(CM_MAKECANTILEVER,CM_MAKECANTILEVER),
  HCENTRY_MENU(CM_SUPPORTCANTILEVER,CM_SUPPORTCANTILEVER),
END_HELPCONTEXT;


//////////////////////////////////////////////////////////
// TQConBridgeMDIClient
// ===========
// Construction/Destruction handling.
TQConBridgeMDIClient::TQConBridgeMDIClient (TModule* module)
 : TMDIClient (module)
{
    ChildCount = 0;

    // INSERT>> Your constructor code here.
    bitmap = NULL;
    pal = NULL;

}


TQConBridgeMDIClient::~TQConBridgeMDIClient ()
{
    Destroy();

    // INSERT>> Your destructor code here.
    delete bitmap;
    delete pal;
}


//////////////////////////////////////////////////////////
// TQConBridgeMDIClient
// ===========
// MDIClient site initialization.
void TQConBridgeMDIClient::SetupWindow ()
{
    // Default SetUpWindow processing.
    TMDIClient::SetupWindow ();

    // Accept files via drag/drop in the client window.
    DragAcceptFiles(true);

    try
       {
       TDib dib(GetApplication()->GetInstance(),TResId(BMP_BKGROUND));
       pal = new TPalette(dib);
       bitmap = new TBitmap(dib,pal);
       }
    catch (TXOwl&)
       {
       // Bitmap creation failed.
       // Silently clean up
       delete pal;
       pal = NULL;

       delete bitmap;
       bitmap = NULL;
       }
}

void TQConBridgeMDIClient::CleanupWindow ()
{
   TWindow::CleanupWindow();
}

void TQConBridgeMDIClient::EvDropFiles (TDropInfo)
{
    Parent->ForwardMessage();
}

void TQConBridgeMDIClient::CmFastPrint()
{
   PrintIt(false);
}

void TQConBridgeMDIClient::CmPrint ()
{
   PrintIt(true);
}

void TQConBridgeMDIClient::PrintIt(bool prompt)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   if (theApp)
      {
      TPrinter *Printer = theApp->GetPrinter();

      TQConBridgeView *view = TYPESAFE_DOWNCAST(GetActiveMDIChild()->GetClientWindow(),TQConBridgeView);
      CHECKX(view,"TQConBridgeMDIClient::PrintIt() - bad view");

      theApp->Printing = true;
      TPrintout *printout = view->GetPrintout();

      TWindow *activeWindow = GetWindowPtr(GetActiveWindow());
      Printer->Print(activeWindow,*printout,prompt);
      theApp->Printing = false;
      delete printout;
      activeWindow->Invalidate();
      }
}

void TQConBridgeMDIClient::CmPrinterSetup ()
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   if (theApp)
      {
      TPrinter *Printer = theApp->GetPrinter();
      Printer->Setup(this);
      }
}


void TQConBridgeMDIClient::CePrintEnable (TCommandEnabler &tce)
{
   if (GetActiveMDIChild())
      {
      TQConBridgeView *view = TYPESAFE_DOWNCAST(GetActiveMDIChild()->GetClientWindow(),TQConBridgeView);
      CHECKX(view,"TQConBridgeMDIClient::CePrintEnable() - bad view");

      TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
      CHECKX(theApp,"TQConBridgeMDIClient::CePrintEnable() - bad app");

      if (theApp && view->CanPrint())
         {
         TPrinter *Printer = theApp->GetPrinter();
         tce.Enable(Printer->GetSetup().Error == 0);
         }
      else
         tce.Enable(false);
      }
   else
      tce.Enable(false);
}


bool TQConBridgeMDIClient::EvEraseBkgnd (HDC hdc)
{
    bool result;

    result = TMDIClient::EvEraseBkgnd(hdc);

    // INSERT>> Your code here.
    if (!bitmap)
       return result;


    TDC dc(hdc);
    TRect rClient = GetClientRect();

    // Fill the background in gray
    dc.FillRect(rClient,TBrush(TColor::LtGray));

    // Here is the bitmap drawing part
    dc.SelectObject(*pal);
    dc.RealizePalette();
    TMemoryDC memdc(dc);
    memdc.SelectObject(*pal);
    memdc.RealizePalette();
    memdc.SelectObject(*bitmap);

    TRect rBitmap(TPoint(0,0),TSize(bitmap->Width(),bitmap->Height()));
    if (rBitmap.Width() > rClient.Width() ||
        rBitmap.Height() > rClient.Height())
       {
       int edge = min(rClient.Height(),rClient.Width());
       TPoint p(rClient.Width()/2 - edge/2,rClient.Height()/2 - edge/2);
       dc.StretchBlt(TRect(p,TSize(edge,edge)),memdc,rBitmap,SRCCOPY);
       }
    else
       {
       int x = (rClient.Width()/2)  - (bitmap->Width()/2);
       int y = (rClient.Height()/2) - (bitmap->Height()/2);

       dc.BitBlt(x,y,bitmap->Width(),bitmap->Height(),memdc,0,0,SRCCOPY);
       }

    return true;
}


void TQConBridgeMDIClient::EvSize (uint sizeType, TSize& size)
{
    TMDIClient::EvSize(sizeType, size);

    // INSERT>> Your code here.
    Invalidate(); // Force a redraw so that the background bitmap is centered
}

