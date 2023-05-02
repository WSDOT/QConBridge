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


#include <qconbrdg\selload.h>
#include <qconbrdg\qcbapp.h>
#include <Context.hh>

SelLoadingXferBuffer::SelLoadingXferBuffer(TModule *module)
{
   int key[NUM_LOADINGS] = {IDS_DC,
                  IDS_DW,
                  IDS_LL_ENV,
                  IDS_TANDEM_ENV,
                  IDS_TRUCK_ENV,
                  IDS_TRKTRAIN_ENV,
                  IDS_TANDEMTRAIN_ENV,
                  IDS_FATIGUETRK_ENV,
                  IDS_PEDESTRIAN_ENV,
                  IDS_STRENGTH_I_ENV,
                  IDS_SERVICE_I_ENV,
                  IDS_SERVICE_II_ENV,
                  IDS_SERVICE_III_ENV,
                  IDS_FATIGUE_ENV};
   string loadTitle;

   for (DiagramCatagory dc = dcDC; dc <= dcFatigue; dc++)
      {
      loadTitle = module->LoadString(key[dc]);
      lbData.AddString(loadTitle.c_str());
      }
}

void SelLoadingXferBuffer::DrawDiagram(DiagramCatagory dc,bool draw)
{
   if (draw)
      lbData.Select(dc);
}

bool SelLoadingXferBuffer::DrawDiagram(DiagramCatagory dc)
{
   TIntArrayIterator i(lbData.GetSelIndices());

   while (i)
      {
      TInt idx = i++;
      if (idx == dc)
         return true;
      }

   return false;
}

//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(TSelLoadingDlg, TDialog)
//{{TSelLoadingDlgRSP_TBL_BEGIN}}
    EV_BN_CLICKED(IDC_MARKALL, BNClicked_MarkAll),
    EV_BN_CLICKED(IDC_CLEARALL, BNClicked_ClearAll),
    EV_BN_CLICKED(IDHELP, BNClicked_Help),
//{{TSelLoadingDlgRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TSelLoadingDlg Implementation}}


TSelLoadingDlg::TSelLoadingDlg (TWindow* parent, TResId resId, TModule* module):
    TDialog(parent, resId, module)
{
    // INSERT>> Your constructor code here.
    lbox = new TListBox(this,IDC_ACTIVE_DIAGRAMS);
}


TSelLoadingDlg::~TSelLoadingDlg ()
{
    Destroy();

    // INSERT>> Your destructor code here.
    delete lbox;
}


void TSelLoadingDlg::BNClicked_MarkAll ()
{
//   lbox->SetSel(-1,true);  // This doesn't seem to work anymore
  lbox->SendMessage(LB_SETSEL,true,-1);
}

void TSelLoadingDlg::BNClicked_ClearAll ()
{
//   lbox->SetSel(-1,false);
  lbox->SendMessage(LB_SETSEL,false,-1);
}


void TSelLoadingDlg::BNClicked_Help ()
{
    TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Select_Loadings);
}

