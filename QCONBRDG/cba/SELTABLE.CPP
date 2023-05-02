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


#include <qconbrdg\seltable.h>
#include <qconbrdg\qcbapp.h>
#include <Context.hh>

SelTableXferBuffer::SelTableXferBuffer(TModule *module)
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

void SelTableXferBuffer::DrawDiagram(DiagramCatagory dc,bool draw)
{
   if (draw)
      lbData.Select(dc);
}

DiagramCatagory SelTableXferBuffer::GetSelection()
{
   TIntArray selIndices = lbData.GetSelIndices();
   int idx = selIndices[0];
   return (DiagramCatagory)idx;
}

bool SelTableXferBuffer::DrawDiagram(DiagramCatagory dc)
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
DEFINE_RESPONSE_TABLE1(TSelTableDlg, TDialog)
//{{TSelTableDlgRSP_TBL_BEGIN}}
    EV_BN_CLICKED(IDHELP, BNClicked_Help),
//{{TSelTableDlgRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TSelTableDlg Implementation}}


TSelTableDlg::TSelTableDlg (TWindow* parent, TResId resId, TModule* module):
    TDialog(parent, resId, module)
{
    // INSERT>> Your constructor code here.
    lbox = new TListBox(this,IDC_ACTIVE_DIAGRAMS);
}


TSelTableDlg::~TSelTableDlg ()
{
    Destroy();

    // INSERT>> Your destructor code here.
    delete lbox;
}


void TSelTableDlg::BNClicked_Help ()
{
    TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Select_Loadings_for_a_Table);
}

