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


#include <qconbrdg\qcbview.h>
#include <qconbrdg\unitsdlg.h>
#include <qconbrdg\analdlg.h>
#include <owltools\progdlg.h>


//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(TQConBridgeView, TIconView)
//{{TQConBridgeViewRSP_TBL_BEGIN}}
    EV_COMMAND(CM_ANALYZE, CmAnalyze),
    EV_COMMAND(CM_FASTANALYZE, CmFastAnalyze),
    EV_COMMAND_ENABLE(CM_ANALYZE, CeAnalyze),
    EV_COMMAND_ENABLE(CM_FASTANALYZE, CeAnalyze),
    EV_COMMAND(CM_UNITS, CmUnits),
//{{TQConBridgeViewRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TQConBridgeView Implementation}}


TQConBridgeView::TQConBridgeView (TBridgeDocument& doc):
    TIconView(doc),
    bridgeDoc(doc)
{
    // INSERT>> Your constructor code here.

}


TQConBridgeView::~TQConBridgeView ()
{
    // INSERT>> Your destructor code here.

}


void TQConBridgeView::CmAnalyze ()
{
   // Use -1 and +1 so the number looks like nth points. (i.e. for 10th
   // points minAnalysisPoints = 11, but the user wants to enter 10)
   TAnalyzeXferBuffer xfer;
   xfer.minAnalysisPoints = bridgeDoc.GetMinAnalysisPoints() - 1;
   TAnalyzeDialog dlg(GetWindow());

   dlg.SetTransferBuffer(&xfer);

   if (dlg.Execute() == IDOK)
      {
      bridgeDoc.SetMinAnalysisPoints(xfer.minAnalysisPoints + 1);
      AnalyzeBridge();
      }
}

void TQConBridgeView::CmFastAnalyze ()
{
   AnalyzeBridge();
}

void TQConBridgeView::CeAnalyze (TCommandEnabler &tce)
{
   tce.Enable(bridgeDoc.GetNumSpans() > 0);
}

void TQConBridgeView::AnalyzeBridge()
{
   bool retval;
   int max = 500;

   // Simulate a modal dialog by disabling the main window of the application
   GetWindow()->GetApplication()->GetMainWindow()->EnableWindow(false);

   TProgressDlg dlg(GetWindow(),IDD_PROGRESSDIALOG);

   dlg.SetLedRange(0,max);
   dlg.SetLedSpacing(max/20); // use 20% of max range value (rule of thumb)

   dlg.Create();

   retval = bridgeDoc.Analyze(&dlg);

   if (retval)
      {
      dlg.SetProgressMsg("Analysis Completed Successfully");
      dlg.SetGaugeColor(TColor::LtGreen);
      }
   else
      {
      dlg.SetGaugeColor(TColor::LtRed);
      }

   // Finish filling the gauge
   for (int i = dlg.GetGaugeValue(); i < max; i++)
      dlg.IncrementGauge();

   dlg.MakeOkButton();

   // Loop until the ok button is pressed
   // This will simulate modality
   while(!dlg.WasOkPressed())
      {
      GetWindow()->GetApplication()->PumpWaitingMessages();
      }

   // Re-enable the main window
   GetWindow()->GetApplication()->GetMainWindow()->EnableWindow(true);
}

void TQConBridgeView::CmUnits ()
{
    UnitsXferBuffer xfer(bridgeDoc.GetUnits());
    TUnitsDlg dlg(GetWindow());
    dlg.SetTransferBuffer(&xfer);
    if( dlg.Execute() == IDOK )
       {
       bridgeDoc.SetUnits(xfer.GetSystemOfUnits());
       }
}


