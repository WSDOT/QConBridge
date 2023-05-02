////////////////////////////////////////////////////////////////////////////
//                                                                         //
// dialog.cpp                                                              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "griddll.h"
#pragma hdrstop

DEFINE_RESPONSE_TABLE1 (TGDialog,TDialog)
   EV_WM_CTLCOLOR,
END_RESPONSE_TABLE;


TGDialog::TGDialog (TWindow *p, TResId resId, TColor b, TModule *module)
        : TDialog (p, resId, module)
   {
   brush = new TBrush (bkg = b);
   }


TGDialog::TGDialog (TWindow *p, TResId resId, TModule *module)
        : TDialog (p, resId, module)
   {
   if (GetApplication()->Ctl3dEnabled() || isChicago)
      bkg = GetSysColor (COLOR_BTNFACE);
   else
      bkg = GetSysColor (COLOR_WINDOW);
   brush = new TBrush (bkg);
   }


TGDialog::~TGDialog ()
   {
   delete brush;
   }


HBRUSH TGDialog::EvCtlColor(HDC hDC, HWND hWndChild, uint ctlType)
   {
   TWindow *wptr = GetWindowPtr (hWndChild);

   if (dynamic_cast<TGDialog *>(wptr))
      return dynamic_cast<TGDialog *>(wptr)->CtlColors (hDC);
   if (dynamic_cast<TGEdit *>(wptr))
      return dynamic_cast<TGEdit *>(wptr)->CtlColors (hDC);
   if (dynamic_cast<TGComboBox *>(wptr))
      return dynamic_cast<TGComboBox *>(wptr)->CtlColors (hDC);
   if (dynamic_cast<TGStatic *>(wptr))
      return dynamic_cast<TGStatic *>(wptr)->CtlColors (hDC);
   if (dynamic_cast<TGCheckBox *>(wptr))
      return dynamic_cast<TGCheckBox *>(wptr)->CtlColors (hDC);
   if (GetApplication()->BWCCEnabled () || GetApplication()->Ctl3dEnabled() || isChicago)
      {
      char buffer[32];
      ::GetClassName (hWndChild, buffer, 32);
      if (stricmp (buffer, "Static") == 0)
         {
         TDC (hDC).SetBkColor (bkg);
         return HBRUSH(*brush);
         }
      }

   return TDialog::EvCtlColor (hDC, hWndChild, ctlType);
   }

   
// Called by EvCtlColor (HDC, HWND, CTLCOLOR_DLG)
// sets background color and returns brush
HBRUSH TGDialog::CtlColors (HDC hDC)
   {
   TDC dc(hDC);
   dc.SetBkColor (bkg);
   return *brush;
   }


// Sets background color 
void TGDialog::SetColor (TColor b)
   {
   bkg = b;
   delete brush;
   brush = new TBrush (bkg);
   }

// EOF
