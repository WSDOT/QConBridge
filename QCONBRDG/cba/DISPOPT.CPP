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

#include <qconbrdg\dispopt.h>
#include <qconbrdg\qcbapp.h>
#include <Context.hh>

DisplayOptionsXferBuffer::DisplayOptionsXferBuffer()
{
   decorations.labels     = BF_UNCHECKED;
   decorations.ticks      = BF_UNCHECKED;
   decorations.dimensions = BF_UNCHECKED;
   decorations.invert     = BF_UNCHECKED;

   for (int i = 0; i < NUM_LOADINGS; i++)
      {
      screenPlotStyle[i].SetColor(TColor::Black);
      screenPlotStyle[i].SetPenStyle(PS_SOLID);
      screenPlotStyle[i].SetSymbol(stNull);
      printerPlotStyle[i].SetColor(TColor::Black);
      printerPlotStyle[i].SetPenStyle(PS_SOLID);
      printerPlotStyle[i].SetSymbol(stNull);
      }
}

//
//---------------------------------------------------------------------------
//

class TDisplayOptionsPage1 : public TMyPropertyPage
{
public:
   TDisplayOptionsPage1(TPropertySheet* parent,TModule *module = 0);
   ~TDisplayOptionsPage1();
   void Help(TNotify far&);

protected:
   TCheckBox *labelChk;
   TCheckBox *tickChk;
   TCheckBox *dimensionChk;
   TCheckBox *gridChk;
   TCheckBox *invertChk;
};

TDisplayOptionsPage1::TDisplayOptionsPage1(TPropertySheet* parent,TModule* module) :
TMyPropertyPage(parent,TResId(IDD_DISPLAYOPTIONS_PAGE1),"Decorations",0,module)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   labelChk = new TCheckBox(this,IDC_LABELS);
   tickChk = new TCheckBox(this,IDC_TICKS);
   dimensionChk = new TCheckBox(this,IDC_DIMENSIONS);
   gridChk = new TCheckBox(this,IDC_GRID);
   invertChk = new TCheckBox(this,IDC_INVERT);
}

TDisplayOptionsPage1::~TDisplayOptionsPage1()
{
   delete labelChk;
   delete tickChk;
   delete dimensionChk;
   delete gridChk;
   delete invertChk;
}

void TDisplayOptionsPage1::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Decorations);
}

//
//--------------------------------------
//

class TDisplayOptionsPage2 : public TMyPropertyPage
{
public:
   TDisplayOptionsPage2(TPropertySheet* parent,const char far* title,TModule *module = 0);
   ~TDisplayOptionsPage2();

   void Help(TNotify far&);

protected:
   TColorList    *color[NUM_LOADINGS];
   TPenStyleList *penStyle[NUM_LOADINGS];
   TSymbolList   *symbol[NUM_LOADINGS];
};

TDisplayOptionsPage2::TDisplayOptionsPage2(TPropertySheet* parent,const char far* title,TModule *module) :
TMyPropertyPage(parent,IDD_DISPLAYOPTIONS_PAGE2,title,0,module)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   for (int i = 0; i < NUM_LOADINGS; i++)
      {
      color[i] = new TColorList(this,IDC_COLORBASE + i);
      penStyle[i] = new TPenStyleList(this,IDC_PENBASE + i);
      symbol[i] = new TSymbolList(this,IDC_SYMBOLBASE + i);
      }
}

TDisplayOptionsPage2::~TDisplayOptionsPage2()
{
   for (int i = 0; i < NUM_LOADINGS; i++)
      {
      delete color[i];
      delete penStyle[i];
      delete symbol[i];
      }
}

void TDisplayOptionsPage2::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   long help_topic;
   TMyPropertySheet* pPropSheet = GetParentSheet();
   switch(pPropSheet->IndexOfPage(this))
   {
   case 1:   help_topic = IDH_Screen_Plot_Style; break;
   case 2:   help_topic = IDH_Printer_Plot_Style; break;
   }
   theApp->ActivateHelp(this,help_topic);
}

//
//-----------------------------------------------------
//

TDisplayOptionsDlg::TDisplayOptionsDlg (TWindow* parent,TModule* module) :
TMyPropertySheet(parent,"Display Options",0,false,PSH_DEFAULT | PSH_HASHELP | PSH_NOAPPLYNOW,module)
{
    // INSERT>> Your constructor code here.
    page1 = new TDisplayOptionsPage1(this,module);
    page2 = new TDisplayOptionsPage2(this,"Screen Plot Style",module);
    page3 = new TDisplayOptionsPage2(this,"Printer Plot Style",module);

    AddPage(*page1);
    AddPage(*page2);
    AddPage(*page3);
}


TDisplayOptionsDlg::~TDisplayOptionsDlg ()
{
    Destroy();

    delete page1;
    delete page2;
    delete page3;
}

void TDisplayOptionsDlg::SetupTransferBuffer(DisplayOptionsXferBuffer *buffer)
{
   page1->SetTransferBuffer(&buffer->decorations);
   page2->SetTransferBuffer(&buffer->screenPlotStyle);
   page3->SetTransferBuffer(&buffer->printerPlotStyle);
}

