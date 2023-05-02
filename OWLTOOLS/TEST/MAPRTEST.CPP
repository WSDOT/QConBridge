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

#include "maprtest.h"
#include <owltools\mapper.h>
#include <iomanip.h>

DEFINE_RESPONSE_TABLE1(TMapperTesterDlg, TDialog)
   EV_BN_CLICKED(IDC_CALCULATE,BNClicked_Calculate),
END_RESPONSE_TABLE;

TMapperTesterDlg::TMapperTesterDlg (TWindow* parent, TResId resId, TModule* module) :
TDialog(parent,resId,module)
{
   minX = new TNumberEdit(this,IDC_XMIN,11,3);
   minY = new TNumberEdit(this,IDC_YMIN,11,3);
   maxX = new TNumberEdit(this,IDC_XMAX,11,3);
   maxY = new TNumberEdit(this,IDC_YMAX,11,3);
   left   = new TIntEdit(this,IDC_LEFT,true);
   top    = new TIntEdit(this,IDC_TOP,true);
   right  = new TIntEdit(this,IDC_RIGHT,true);
   bottom = new TIntEdit(this,IDC_BOTTOM,true);
   mapModes = new TRadioGroupBox(this,IDC_MAPPINGMODE,IDC_ANISOTROPIC,IDC_ISOTROPIC);
   bestFit = new TCheckBox(this,IDC_BESTFIT,mapModes);
   sX = new TNumberEdit(this,IDC_SX,11,3);
   sY = new TNumberEdit(this,IDC_SY,11,3);
   testX = new TNumberEdit(this,IDC_TESTX,11,3);
   testY = new TNumberEdit(this,IDC_TESTY,11,3);
}

TMapperTesterDlg::~TMapperTesterDlg()
{
   delete minX;
   delete minY;
   delete maxX;
   delete maxY;
   delete left;
   delete top;
   delete right;
   delete bottom;
   delete mapModes;
   delete bestFit;
   delete sX;
   delete sY;
   delete testX;
   delete testY;
}

void TMapperTesterDlg::SetupWindow()
{
   TDialog::SetupWindow();

   // Setup suggested device size
   TRect deviceRect = GetClientRect();
   left->SetValue(deviceRect.left);
   top->SetValue(deviceRect.top);
   right->SetValue(deviceRect.right);
   bottom->SetValue(deviceRect.bottom);
}


void TMapperTesterDlg::BNClicked_Calculate()
{
   TDC dc(::GetDC(HWindow));
//   TMapper mapr(dc);
   TMapper mapr;
   TMapMode mm;
   char buffer[256];
   TFPoint worldPoint(testX->GetValue(),testY->GetValue());
   TPoint drawPoint;

   mapr.AddScaleX(0.100);
   mapr.AddScaleX(0.125);
   mapr.AddScaleX(0.150);
   mapr.AddScaleX(0.200);
   mapr.AddScaleX(0.250);
   mapr.AddScaleX(0.500);

   mapr.AddScaleY(0.100);
   mapr.AddScaleY(0.125);
   mapr.AddScaleY(0.150);
   mapr.AddScaleY(0.200);
   mapr.AddScaleY(0.250);
   mapr.AddScaleY(0.500);
/*
   mapr.SetWorldRect(minX->GetValue(),minY->GetValue(),maxX->GetValue(),maxY->GetValue());
   mapr.SetDeviceRect(left->GetValue(),top->GetValue(),right->GetValue(),bottom->GetValue());
   mapr.SetFactorX(sX->GetValue());
   mapr.SetFactorY(sY->GetValue());
*/
   mapr.SetWorldOrg(minX->GetValue(),minY->GetValue());
   mapr.SetWorldExt(maxX->GetValue(),maxY->GetValue());
   mapr.SetDeviceOrg(left->GetValue(),top->GetValue());
   mapr.SetDeviceExt(sX->GetValue()*(double)right->GetValue(),
                     sY->GetValue()*(double)bottom->GetValue());
   mapr.SetLogPixelsX(dc.GetDeviceCaps(LOGPIXELSX));
   mapr.SetLogPixelsY(dc.GetDeviceCaps(LOGPIXELSY));

   if (mapModes->GetSelRadioButton() == IDC_ANISOTROPIC)
      {
      mm = mmAnisotropic | ((bestFit->GetCheck() == BF_CHECKED) ? mmBestFitX | mmBestFitY : 0x000);
      }
   else
      {
      mm = mmIsotropic | ((bestFit->GetCheck() == BF_CHECKED) ? mmBestFitX : 0x000);
      }
   mapr.SetMapMode(mm);

   mapr.WPtoDP(worldPoint,drawPoint);

   ostrstream(buffer,sizeof(buffer)) << "Device Ext" << endl << mapr.GetDeviceExt() << ends;
   SetDlgItemText(IDC_DRAWRECT,buffer);

   ostrstream(buffer,sizeof(buffer)) << setprecision(2) << "Scale X : " << mapr.GetScaleX() << ends;
   SetDlgItemText(IDC_SCALEX,buffer);

   ostrstream(buffer,sizeof(buffer)) << setprecision(2) << "Scale Y : " << mapr.GetScaleY() << ends;
   SetDlgItemText(IDC_SCALEY,buffer);

   ostrstream(buffer,sizeof(buffer)) << "LogPixelsX : " << mapr.GetLogPixelsX() << ends;
   SetDlgItemText(IDC_LOGPIXELSX,buffer);

   ostrstream(buffer,sizeof(buffer)) << "LogPixelsY : " << mapr.GetLogPixelsY() << ends;
   SetDlgItemText(IDC_LOGPIXELSY,buffer);

   ostrstream(buffer,sizeof(buffer)) << "Draw Point : " << drawPoint << ends;
   SetDlgItemText(IDC_DRAWPOINT,buffer);

   ::ReleaseDC(HWindow,dc);

   Invalidate();
}
