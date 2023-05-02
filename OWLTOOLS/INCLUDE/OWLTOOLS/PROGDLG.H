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

#if !defined PROGDLG_H
#define PROGDLG_H

#include <owltools\owltldll.h>
#include <owltools\progress.rh>
#include <owl\dialog.h>
#include <owl\static.h>
#include <owl\gauge.h>

class _OWLTOOLSCLASS TProgressDlg : public TDialog
{
public:
   TProgressDlg(TWindow *parent,int resId);
   virtual ~TProgressDlg();
   void SetupWindow();
   void MakeOkButton();
   void MakeCancelButton();
   void BNClicked_ProgressBtn();
   bool WasOkPressed();
   virtual bool ShouldContinue();
   void CmCancel();

   // This should be in a derived dlg class
   void SetProgressMsg(const char *msg);
   void SetGaugeValue(int value);
   int  GetGaugeValue() const;
   void SetGaugeColor(TColor color);
   void IncrementGauge();
   void DecrementGauge();
   void SetLedSpacing(int spacing);
   void SetLedRange(int min,int max);

protected:
   TStatic *progressMsg;
   TGauge *progressGauge;
   bool shouldContinue;
   bool okPressed;

   DECLARE_RESPONSE_TABLE(TProgressDlg);
};

#endif // PROGDLG_H