/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// gridckbx.cpp                                                            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "griddll.h"
#pragma hdrstop
#include "grid.rh"

DEFINE_RESPONSE_TABLE1 (TGCheckBox, TCheckBox)
   EV_WM_CHAR,
   EV_WM_GETDLGCODE,
   EV_WM_KEYDOWN,
   EV_WM_KEYUP,
   EV_WM_KILLFOCUS,
   EV_WM_SETFOCUS,
   EV_WM_LBUTTONDOWN,
   EV_WM_LBUTTONUP,
   EV_WM_LBUTTONDBLCLK,
   EV_WM_RBUTTONDOWN,
   EV_WM_RBUTTONUP,
   EV_WM_RBUTTONDBLCLK,
END_RESPONSE_TABLE;

MOUSE_HANDLER_CODE (CheckBox)

TGCheckBox::TGCheckBox (TWindow *p, uint id, TGroupBox *group, TModule *module)
   : TCheckBox (p, id, group, module)
   {
   Init ();
   }


TGCheckBox::TGCheckBox (TWindow *p, uint id, char *title, int x, int y, int w, int h, TGroupBox *group, TModule *module)
   : TCheckBox (p, id, title, x, y, w, h, group, module)
   {
   Init ();
   }


TGCheckBox::~TGCheckBox ()
   {
   delete brush;
   }


void TGCheckBox::Init ()
   {
   grid = 0;

   TApplication *app = GetApplication();
   bool bitmap = (app->BWCCEnabled () || app->Ctl3dEnabled () || isChicago);

   bkg   = TColor(GetSysColor (bitmap ? COLOR_BTNFACE : COLOR_WINDOW));
   text  = TColor(GetSysColor (bitmap ? COLOR_BTNTEXT : COLOR_WINDOWTEXT));
   brush = new TBrush (bkg);
   bwcc = false;
   }


bool TGCheckBox::IsModified ()
   {
   return (GetCheck () != oldstate);
   }


void TGCheckBox::EvKillFocus (HWND hWndGetFocus)
   {
   if (grid)
      {
      bool ok = false;
      bool state = GetCheck ();
      if (state != oldstate)
         ok = grid->DataChanging (grid->GetCurrentLine(), GetId(), static_cast<void *>(&state));
      grid->KillFocusFromChild (this, hWndGetFocus, ok);
      ClearModify ();
      }
   TCheckBox::EvKillFocus (hWndGetFocus);
   }


void TGCheckBox::EvSetFocus (HWND hWndLostFocus)
   {
   if (grid)
      grid->SetFocusOnChild (this, hWndLostFocus);
   ClearModify ();
   TCheckBox::EvSetFocus (hWndLostFocus);
   }

   
GRID_EV_GETDLGCODE(CheckBox)
GRID_EV_KEYDOWN(CheckBox)
GRID_EV_KEYUP(CheckBox)
GRID_EV_CHAR(CheckBox)
COLOR_CODE(TGCheckBox)


bool TGCheckBox::Create ()
   {
   bool rc = TCheckBox::Create ();
   if (rc && grid)
      grid->CalcPos ();
   return rc;
   }


bool TGCheckBox::PreProcessMsg (MSG &msg)
   {
   if (grid &&
       msg.message == WM_KEYDOWN &&
       msg.wParam == VK_ESCAPE   &&
       grid->KeyDown (msg.wParam, LOWORD(msg.lParam)) )
      return true;

   return TCheckBox::PreProcessMsg (msg);
   }


void TGCheckBox::DrawCell (TDC &dc, TRect &rc, const void *p, TGrid::Align align)
   {
   TColor bg, fg;
   HBRUSH dummy;

   int line = grid->GetDataLine(rc.top);
   int id = GetId ();

   if (grid->LineSelected (line))
      {
      bg = grid->hilite;
      fg = grid->hilitetext;
      }
   else
      {
      bool customColors = grid->CellColor (line, id, false, bg, fg, dummy);
      if (!customColors)
         {
         fg = text;
         bg = bkg;
         }
      }

   bool checked = false;
   if (p)
      checked = *static_cast<const bool *>(p);

   TGrid::Visible v = grid->VisibleCell (line, id);

   TModule *bwccMod = 0;
   bool     ctl3d = false;
   TApplication *app = GetApplication ();

   if (app->BWCCEnabled())
      bwccMod = app->GetBWCCModule ();
   if (app->Ctl3dEnabled() || isChicago)
      ctl3d = true;

   dc.SetBkColor (bg);
   dc.SetTextColor (fg);

   int cellWidth = rc.Width();
   int cellHeight = rc.Height ();

   TRect rc1 (rc);
   if (ctl3d && v == TGrid::gControl/* && !bwcc*/)
      {
      dc.FillRect (rc, TBrush (bg));
      #ifndef _GRIDLIB
         #define module GridModule
      #else
         #define module GetModule()
      #endif
      if (grid->Ctl3dCheckBmp == 0)
         {
         HBITMAP hbmp = module->LoadBitmap (TResId(BMP_CHK3D));
         if (hbmp == 0)
            return;
         grid->Ctl3dCheckBmp = new TBitmap (hbmp, AutoDelete);
         }
      TBitmap *bmp = grid->Ctl3dCheckBmp;
      int w = bmp->Width()/2;
      int h = bmp->Height();
      rc1.right = rc1.left+w;
      rc1.bottom = rc1.top+h;
      int dy = (cellHeight - h) / 2;
      int dx = (align == TGrid::Center) ? (cellWidth - w + 1) / 2 : 0;
      rc1.Offset (dx, dy);
      TMemoryDC memDC (dc);
      memDC.SelectObject (*bmp);
      dc.BitBlt (rc1, memDC, TPoint(checked ? w : 0, 0));
      memDC.RestoreBitmap ();
      switch (align)
         {
         case TGrid::Left:
            rc.left = rc1.right+4;
            break;
         default:
            rc.right = rc1.left-4;
            rc.left++;
            break;
         }
      }

   else if (bwccMod && v == TGrid::gControl)
      {
      bg = GetSysColor (COLOR_BTNFACE);
      fg = GetSysColor (COLOR_BTNTEXT);
      dc.FillRect (rc, TBrush (bg));
      TBitmap *bmp;
      HINSTANCE hb = bwccMod->GetInstance ();
      if (checked)
         {
         if (grid->BwccCheckBmp == 0)
            grid->BwccCheckBmp = new TBitmap (hb, 111);
         bmp = grid->BwccCheckBmp;
         }
      else
         {
         if (grid->BwccUncheckBmp == 0)
            grid->BwccUncheckBmp = new TBitmap (hb, 110);
         bmp = grid->BwccUncheckBmp;
         }
      int w = bmp->Width();
      int h = bmp->Height();
      rc1.right = rc1.left+w;
      rc1.bottom = rc1.top+h;
      int dy = (cellHeight - h) / 2;
      int dx = (align == TGrid::Center) ? (cellWidth - w + 1) / 2 : 0;
      rc1.Offset (dx, dy);
      TMemoryDC memDC (dc);
      memDC.SelectObject (*bmp);
      dc.BitBlt (rc1, memDC, TPoint(0, 0));
      memDC.RestoreBitmap ();
      rc.left = rc1.right+6;
      }

   else
      {
      dc.FillRect (rc, TBrush (bg));
      if (grid->CheckBoxesBmp == 0)
         grid->CheckBoxesBmp = new TBitmap (HINSTANCE(0), OBM_CHECKBOXES);
      int w = grid->CheckBoxesBmp->Width()/4;
      int h = grid->CheckBoxesBmp->Height()/3;
      if (align == TGrid::Left)
         rc1.right = rc1.left+w;
      else
         rc1.left = rc1.right-w;
      rc1.bottom = rc1.top+h;
      int dy = (cellHeight - h) / 2;
      int dx = (align == TGrid::Center) ? (cellWidth - w + 1) / 2 : 0;
      rc1.Offset (dx, dy);
      TMemoryDC memDC (dc);
      memDC.SelectObject (*grid->CheckBoxesBmp);
      dc.BitBlt (rc1, memDC, TPoint(checked?w:0, 0));
      memDC.RestoreBitmap ();
      if (align == TGrid::Left)
         rc.left = rc1.right+5;
      else
         {
         rc.right = rc1.left-5;
         rc.left++;
         }
      }

   int len = GetWindowTextLength ()+1;
   if (len > 1)
      {
      char *txt = new char[len];
      GetWindowText (txt, len);
      dc.DrawText (txt, -1, rc, (WORD)(DT_LEFT|DT_VCENTER|DT_SINGLELINE));
      delete [] txt;
      }
   }


void TGCheckBox::ClearModify ()
   {
   oldstate = GetCheck ();
   }

// EOF
