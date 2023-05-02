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


#include "mainwnd.h"
#include "testapp.h"
#include "iedlg.h"
#include "rgbdlg.h"
#include "unitdlg.h"
#include "validdlg.h"
#include "combotst.h"
#include "maprtest.h"
#include <stdio.h>
#include <owltools\hlscolor.h>
#include <owltools\hsvcolor.h>
#include <owltools\symbol.h>


struct XferBuffer
{
   int preCheck;
   TListBoxData lbd1;
   TListBoxData lbd2;
   int postCheck;

   XferBuffer()
    {
    preCheck = BF_UNCHECKED;
    postCheck = BF_UNCHECKED;
    char buffer[30];
    for (int i = 0; i < 30; i++)
       {
       ostrstream(buffer,sizeof(buffer)) << "Span " << i << ends;
       lbd1.AddString(buffer);
       }
    };
};

//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(TMainWindow, TWindow)
//{{TMainWindowRSP_TBL_BEGIN}}
    EV_COMMAND(CM_HLS_RGB_RGB, CmHLS_RGB_RGB),
    EV_COMMAND(CM_HLS_EXCEPTION, CmHLS_Exception),
    EV_COMMAND(CM_HSV_EXCEPTION, CmHSV_Exception),
    EV_COMMAND(CM_HSV_RGB_RGB, CmHSV_RGB_RGB),
    EV_COMMAND(CM_TEST_INCEXC, CmTestIncludeExclude),
    EV_COMMAND(CM_TESTRGB, CmTestRadioGroupBox),
    EV_COMMAND(CM_TESTUNITS, CmTestUnits),
    EV_COMMAND(CM_TESTVALIDATORS, CmTestValidators),
    EV_COMMAND(CM_TESTCOMBOBOXES, CmTestComboBoxes),
    EV_COMMAND(CM_TESTMAPPER, CmTestMapper),
//{{TMainWindowRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TMainWindow Implementation}}


//////////////////////////////////////////////////////////
// TMainWindow
// ==========
// Construction/Destruction handling.
TMainWindow::TMainWindow (TWindow* parent, const char far* title, TModule* module)
    : TWindow(parent, title, module)
{
    // INSERT>> Your constructor code here.

}


TMainWindow::~TMainWindow ()
{
    Destroy();

    // INSERT>> Your destructor code here.

}


void TMainWindow::CmHLS_RGB_RGB ()
{
    // INSERT>> Your code here.
#if defined GEN_DEBUG_CODE
    SetCursor(0,IDC_WAIT);
    if (!THLSColor::Test())
       MessageBox("THLSColor::Test() failed","Error",MB_OK | MB_ICONEXCLAMATION);
    else
       MessageBox("THLSColor::Test() passed","Success",MB_OK | MB_ICONEXCLAMATION);
    SetCursor(0,IDC_ARROW);
#endif
}


void TMainWindow::CmHLS_Exception ()
{
    // INSERT>> Your code here.

    // hue must be undefined if saturation is zero...
    // send bad data to force an exception to be thrown

    int r,g,b;
    THLSColor::HLStoRGB(1,0.5,0.0,r,g,b);

    MessageBox("Exception not thrown when it should have been thrown","Error",MB_OK | MB_ICONEXCLAMATION);
}



void TMainWindow::CmHSV_Exception ()
{
    // INSERT>> Your code here.

    // hue must be undefined if saturation is zero...
    // send bad data to force an exception to be thrown

    int r,g,b;
    THSVColor::HSVtoRGB(1,0.0,0.5,r,g,b);

    MessageBox("Exception not thrown when it should have been thrown","Error",MB_OK | MB_ICONEXCLAMATION);
}


void TMainWindow::CmHSV_RGB_RGB ()
{
    // INSERT>> Your code here.
#if defined GEN_DEBUG_CODE
    SetCursor(0,IDC_WAIT);
    if (!THSVColor::Test())
       MessageBox("THSVColor::Test() failed","Error",MB_OK | MB_ICONEXCLAMATION);
    else
       MessageBox("THSVColor::Test() passed","Success",MB_OK | MB_ICONEXCLAMATION);
    SetCursor(0,IDC_ARROW);
#endif
}


void TMainWindow::CmTestIncludeExclude ()
{
    // INSERT>> Your code here.
    static XferBuffer xfer;

    TIncExcDialog *dlg = new TIncExcDialog(this);
    dlg->SetTransferBuffer(&xfer);
    dlg->Execute();
    delete dlg;
}


void TMainWindow::CmTestRadioGroupBox ()
{
    // INSERT>> Your code here.
    RadioGrpTestDlg dlg(this);
    static RGBXferBuffer xfer;
    char buffer[50];

    dlg.SetTransferBuffer(&xfer);
    if (dlg.Execute() == IDOK)
       {
       ostrstream(buffer,sizeof(buffer)) << "Button # " << xfer.id << " was selected" << ends;
       MessageBox(buffer,"Info",MB_OK);
       }
}


void TMainWindow::CmTestUnits ()
{
    // INSERT>> Your code here.
    static double dist = USConstants::Length(1*USConstants::feet).value(); // 1 feet
    UnitDlg dlg(this);

    dlg.SetTransferBuffer(&dist);
    dlg.Execute();

    char buffer[30];
    ostrstream(buffer,sizeof(buffer)) << dist << ends;
    MessageBox(buffer);
}


void TMainWindow::CmTestValidators ()
{
    // INSERT>> Your code here.
    ValidatorXferBuffer xfer;
    ValidatorDlg dlg(this);

    xfer.value1 = 6;
    xfer.value2 = 11;
    xfer.value3 = -11;
    xfer.value4 = -11;
    xfer.value5 = 11;
    xfer.value6 = 11;
    xfer.value7 = -4;
    xfer.value8 = -11;
    xfer.value9 = -11;

    dlg.SetTransferBuffer(&xfer);
    dlg.Execute();

    ostrstream os;
    os << xfer.value1 << endl
       << xfer.value2 << endl
       << xfer.value3 << endl
       << xfer.value4 << endl
       << xfer.value5 << endl
       << xfer.value6 << endl
       << xfer.value7 << endl
       << xfer.value8 << endl
       << xfer.value9 << endl
       << ends;
    char *msg = os.str();
    MessageBox(msg);
    delete msg;
}

void TMainWindow::CmTestComboBoxes()
{
   TComboTesterDlg dlg(this);
   static ComboTesterXferBuffer xfer;

   dlg.SetTransferBuffer(&xfer);
   dlg.Execute();

   char buffer[100];
   ostrstream os(buffer,sizeof(buffer));
   os << "Spin Value = " << xfer.spinValue << ends;
   ::MessageBox(0,buffer,"Message",MB_OK);
}

void TMainWindow::CmTestMapper()
{
   TMapperTesterDlg dlg(this);
   dlg.Execute();
}

void TMainWindow::Paint(TDC &dc,bool erase,TRect &rect)
{
   TWindow::Paint(dc,erase,rect);

   TRect clientRect = GetClientRect();
   TPoint center(clientRect.TopLeft() + TPoint(clientRect.Width()/2,clientRect.Height()/2));
   TPoint p[5];
   int w,h;
   TSquareSymbol symbol;
   TPen linePen(TColor::LtBlue);
   TPen symbolPen(TColor::LtRed);

   dc.SelectObject(linePen);
   for (int i = 1; i < 11; i++)
      {

      w = (clientRect.Width()*i)/10;
      h = (clientRect.Height()*i)/10;

      p[0] = TPoint(center.x - w/2,center.y-h/2);
      p[1] = TPoint(center.x + w/2,center.y-h/2);
      p[2] = TPoint(center.x + w/2,center.y+h/2);
      p[3] = TPoint(center.x - w/2,center.y+h/2);
      p[4] = TPoint(center.x - w/2,center.y-h/2);

      dc.MoveTo(p[0]);

      for (int j = 1; j < 5; j++)
         {
         dc.SelectObject(linePen);
         dc.LineTo(p[j]);

         dc.SelectObject(symbolPen);
         dc.SelectStockObject(NULL_BRUSH);
         symbol.Draw(dc,10,p[j]);
         }
      }
   dc.RestorePen();
}
