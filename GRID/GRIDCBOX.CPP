/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// gridcbox.cpp                                                            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "griddll.h"
#pragma hdrstop

DEFINE_RESPONSE_TABLE1 (TGComboBox, TComboBox)
   EV_NOTIFY_AT_CHILD (CBN_KILLFOCUS, CbnKillFocus),
   EV_NOTIFY_AT_CHILD (CBN_SETFOCUS, CbnSetFocus),
   EV_NOTIFY_AT_CHILD (CBN_CLOSEUP, CbnCloseUp),
   EV_NOTIFY_AT_CHILD (CBN_DROPDOWN, CbnDropDown),
   EV_WM_CHAR,
   EV_WM_CTLCOLOR,
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

MOUSE_HANDLER_CODE(ComboBox)


TGComboBox::TGComboBox (TWindow *p, int resid, uint textlen, TModule *module)
   : TComboBox (p, resid, textlen, module)
   {
   Init ();
   }


TGComboBox::TGComboBox (TWindow *p, int resid, int x, int y, int w, int h, DWORD style, uint textlen,
                        TModule *module)
          : TComboBox (p, resid, x, y, w, h, style, textlen, module)
   {
   Init ();
   }


TGComboBox::~TGComboBox ()
   {
   delete brush;
   }


void TGComboBox::Init ()
   {
   grid = 0;
   columnbkg   =
   bkg         = GetSysColor (COLOR_WINDOW);
   text        = GetSysColor (COLOR_WINDOWTEXT);
   brush       = new TBrush  (bkg);
   if (grid)
      {
      activeBG    = grid->hilite;
      selectedFG  = grid->hilitetext;
      }

   if (GetApplication()->Ctl3dEnabled() || isChicago)
      columnbkg = GetSysColor (COLOR_BTNFACE);
   }


void TGComboBox::SetupWindow ()
   {
   TComboBox::SetupWindow ();
   TRect rc;
   GetDroppedControlRect(rc);
   totalHeight = Attr.H + rc.Height ();
   }


bool TGComboBox::Create ()
   {
   bool rc = TComboBox::Create ();
   if (rc && grid)
      grid->CalcPos ();
   return rc;
   }


void TGComboBox::CbnDropDown ()
   {
   }


void TGComboBox::CbnCloseUp ()
   {
   if (grid)
      {
      char p[255];
      GetText (p, 255);
      if (oldstr != p)
         {
         void *data = GetData ();
         int line = grid->GetCurrentLine ();
         int id = GetId ();
         bool rc = grid->DataChanging (line, id, data);
         if (rc)
            {
            grid->SetModify (true);
            grid->TransferItem (line, id, data);
            grid->DataChanged (line, id, data);
            ClearModify ();
            }
         if (data)
            delete [] data;
         }
      oldsel = GetSelIndex ();
      }
   }


void TGComboBox::CbnSetFocus ()
   {
   ClearModify ();
   if (grid)
      grid->SetFocusOnChild (this, 0);
   }


void TGComboBox::CbnKillFocus ()
   {
   if (grid)
      grid->KillFocusFromChild (this, 0, false);
   }


void TGComboBox::EvSetFocus (HWND hWndLostFocus)
   {
   if (grid)
      grid->SetFocusOnChild (this, hWndLostFocus);
   oldsel = GetSelIndex ();
   TComboBox::EvSetFocus (hWndLostFocus);
   }


void TGComboBox::EvKillFocus (HWND hWndGetFocus)
   {
   if (grid)
      grid->KillFocusFromChild (this, hWndGetFocus, false);
   TComboBox::EvKillFocus (hWndGetFocus);
   }


void *TGComboBox::GetData ()
   {
   int n = GetSelIndex ();
   int l;
   if (n >= 0)
      l = GetStringLen (n) + 1;
   else
      l = GetTextLen () + 1;

   void *data = static_cast<void *>(new char[l]);

   if (n >= 0)
      GetString (static_cast<char *>(data), n);
   else
      GetText (static_cast<char *>(data), l);

   return data;
   }


void TGComboBox::SetData (void *data)
   {
   SetText (static_cast<const char *>(data));
   }

GRID_EV_GETDLGCODE(ComboBox)


HBRUSH TGComboBox::CtlColors (HDC hDC)
   {
   TDC dc(hDC);
   dc.SetTextColor (text);
   dc.SetBkColor (bkg);
   return *brush;
   }


void TGComboBox::SetColors (TColor t, TColor b)
   {
   text = t;
   bkg = b;
   columnbkg = b;
   delete brush;
   brush = new TBrush (bkg);
   }


void TGComboBox::SetColors (TColor t, TColor b, TColor a)
   {
   text = t;
   bkg = b;
   columnbkg = a;
   delete brush;
   brush = new TBrush (bkg);
   }


// ----------------------------------------------------------------------- //
// this method supports custom colors for the edit portion of the combobox //
// (for editable combo boxes)                                              //
// ----------------------------------------------------------------------- //
HBRUSH TGComboBox::EvCtlColor(HDC hDC, HWND hWndChild, uint)
   {
   if (IsChild (hWndChild))
      return CtlColors (hDC);
   return 0;
   }


bool TGComboBox::PreProcessMsg (MSG &msg)
   {
   if (grid && msg.message == WM_KEYDOWN)
      {
      switch (msg.wParam)
         {
         case VK_NEXT:
         case VK_PRIOR:
         case VK_UP:
         case VK_DOWN:
         case VK_HOME:
         case VK_END:
            if (GetDroppedState ())
               break;
         case VK_ESCAPE:
         case VK_INSERT:
         case VK_DELETE:
         case VK_TAB:
         case VK_RETURN:
            if (grid->KeyDown (msg.wParam, LOWORD(msg.lParam)))
               return true;
            break;
         }
      }

   return TComboBox::PreProcessMsg (msg);
   }


GRID_EV_CHAR(ComboBox);


void TGComboBox::EvKeyUp (uint key, uint repeat, uint flags)
   {
   if (grid)
      {
      switch (key)
         {
         case VK_NEXT:
         case VK_PRIOR:
         case VK_UP:
         case VK_DOWN:
         case VK_HOME:
         case VK_END:
            if (GetDroppedState ())
               break;
         case VK_ESCAPE:
         case VK_INSERT:
         case VK_DELETE:
         case VK_TAB:
         case VK_RETURN:
            return;
         }
      }
   TComboBox::EvKeyUp (key, repeat, flags);
   }


void TGComboBox::EvKeyDown (uint key, uint repeat, uint flags)
   {
   if (grid)
      {
      switch (key)
         {
         case VK_NEXT:
         case VK_PRIOR:
         case VK_UP:
         case VK_DOWN:
         case VK_HOME:
         case VK_END:
            if (GetDroppedState ())
               break;
         case VK_ESCAPE:
         case VK_INSERT:
         case VK_DELETE:
         case VK_TAB:
         case VK_RETURN:
            if (grid->KeyDown (key, repeat))
               return;
            break;
         }
      }
   TComboBox::EvKeyDown (key, repeat, flags);
   }


void TGComboBox::MeasureItem (MEASUREITEMSTRUCT &info)
   {
   info.itemHeight = HIWORD(GetDialogBaseUnits ());
   }


void TGComboBox::ODASelect (DRAWITEMSTRUCT &info)
   {
   DrawCBItem (info);
   }


void TGComboBox::ODADrawEntire (DRAWITEMSTRUCT &info)
   {
   DrawCBItem (info);
   }


void TGComboBox::DrawCBItem (DRAWITEMSTRUCT &info)
   {
   TDC dc (info.hDC);
   char *p;
   TRect rect (info.rcItem);
           
   InitDraw (info, dc, p, rect);
   if (p)
      {
      dc.DrawText (p, -1, rect, (WORD)(DT_LEFT|DT_VCENTER|DT_SINGLELINE));
      delete [] p;
      }
   }


void TGComboBox::InitDraw (DRAWITEMSTRUCT &info, TDC &dc, char * &p, TRect &rect)
   {
   int n = info.itemID;
   uint &state = info.itemState;
   TColor b = (state&ODS_SELECTED ? activeBG : bkg);
   dc.FillRect (rect, TBrush(b));
   dc.SetBkColor (b);
   dc.SetTextColor (state&ODS_DISABLED ?
                    text : (state&ODS_SELECTED ? selectedFG : text));

   int l = (n >= 0 ? GetStringLen (n) : 0);
   p = (l ? new char[l+1] : 0);
   if (p)
      GetString (p, n);
   }


int TGComboBox::Height ()
   {
   bool ctl3d     = GetApplication()->Ctl3dEnabled() || isChicago;
   bool ownerdraw = (Attr.Style&(CBS_OWNERDRAWFIXED|CBS_OWNERDRAWVARIABLE))
                  ? true : false;

   TFont *f = 0;
   if (grid)
      f = grid->GetGridFont (GetId ());

   if (IsWindow() && f && !ownerdraw)
      {
      TClientDC dc(*this);
      dc.SelectObject (*grid->gridFont);
      TEXTMETRIC tm;
      dc.GetTextMetrics (tm);
      dc.RestoreFont ();
      int h = tm.tmHeight;
      if      (h <  8) h += 5;
      else if (h < 12) h += 6;
      else if (h < 16) h += 7;
      else if (h < 20) h += 8;
      else             h += 9;
      if (ctl3d)
         h++;
      return h;
      }

   int h = HIWORD(GetDialogBaseUnits());
   if (ownerdraw)
      h = MulDiv (h, 3, 2);
   else
      h = MulDiv (h, 5, 4);
   if (!ctl3d)
      h--;
   return h;
   }


void TGComboBox::DrawCell (TDC &dc, TRect &rc, const void *p, TGrid::Align)
   {
   // get any custom colors -------------------------------------------------
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
         bg = columnbkg;
         }
      }

   switch (grid->VisibleCell (line, id))
      {
      case TGrid::gControl:
         {
         // flags -----------------------------------------------------------
         bool Editable = (Attr.Style&CBS_DROPDOWNLIST)==CBS_DROPDOWNLIST
                       ? false:true;
         bool ctl3d    = GetApplication()->Ctl3dEnabled() || isChicago;

         // colors ----------------------------------------------------------
         TColor btnFace  = GetSysColor (COLOR_BTNFACE);
         TColor btnText  = GetSysColor (COLOR_BTNTEXT);
         TColor wndFrame = GetSysColor (COLOR_WINDOWFRAME);
         TColor window   = GetSysColor (COLOR_WINDOW);

         // base width ------------------------------------------------------
         int w = LOWORD(GetDialogBaseUnits());

         // Adjust height of rectangle rc -----------------------------------
         int ch = Height ();
         if (rc.Height () > ch)
            {
            TRect r (rc);
            r.top += ch;
            dc.FillRect (r, TBrush(bg));
            rc.bottom = r.top;
            }

         // split rc into rc (text) and rc1 (button) ------------------------
         TRect rc1 (rc);
         rc1.left = rc1.right-w-w-1;
         rc.right = rc1.left+1;

         // draw text background --------------------------------------------
         dc.FillRect (rc, TBrush(bg));
         if (Editable)
            rc.right -= w;

         // button rectangle ------------------------------------------------
         if (ctl3d)
            rc1.bottom--;
         dc.SelectObject (TPen (btnText));
         dc.SelectObject (TBrush (btnFace));
         dc.Rectangle (rc1);
         dc.RestoreBrush ();
         dc.RestorePen ();

         // drop down button frame ------------------------------------------
         int x0 = rc1.left+1;
         int y0 = rc1.top+1;
         int x1 = rc1.right-2;
         int y1 = rc1.bottom-2;
         TColor shadow = GetSysColor (COLOR_BTNSHADOW);
         Frame3d (dc, x0, y0, x1, y1, 1, GetSysColor (COLOR_BTNHIGHLIGHT),
                  shadow);
         x0++, y0++, x1--, y1--;
         dc.SelectObject (TPen (shadow));
         dc.MoveTo (x0, y1);
         dc.LineTo (x1, y1);
         dc.LineTo (x1, y0);
         dc.RestorePen ();

         // draw bitmap -----------------------------------------------------
         dc.SetBkColor (btnFace);
         dc.SetTextColor (btnText);
         TMemoryDC memDC (dc);
         if (grid->ComboBmp == 0)
            grid->ComboBmp = new TBitmap (HINSTANCE(0), OBM_COMBO);
         int bmpw = grid->ComboBmp->Width ();
         int bmph = grid->ComboBmp->Height ();
         int x = rc1.left+(rc1.Width()-bmpw)/2;
         int y = rc1.top+(Height()-bmph)/2;
         if (ctl3d)
            y--;
         memDC.SelectObject (*grid->ComboBmp);
         rc1.Set (x, y, x+bmpw, y+bmph);
         dc.BitBlt (rc1, memDC, TPoint(0,0));
         memDC.RestoreBitmap ();

         // text ------------------------------------------------------------
         if (p && static_cast<const char *>(p))
            {
            HWND hChild;
            if (Editable && (hChild = GetDlgItem (1001)) != 0)
               {
               RECT rect;
               ::SendMessage (hChild, EM_GETRECT, 0, LPARAM(&rect));
               rc.left += rect.left;
               rc.top  += rect.top;
               rc.right -= rect.left;
               rc.bottom = rc.top + rect.bottom - rect.top;
               }
            else
               rc.Inflate (-3,-2);
            WORD align = (WORD)(DT_LEFT|DT_VCENTER|DT_SINGLELINE);
            dc.SetTextColor (fg);
            dc.SetBkColor   (bg);
            dc.DrawText (static_cast<const char *>(p), -1, rc, align);
            }
         break;
         }

      case TGrid::gFlat:
      case TGrid::gLocked:
         {
         dc.FillRect (rc, TBrush (bg));
         if (p && *static_cast<const char *>(p))
            {
            dc.SetBkColor (bg);
            dc.SetTextColor (fg);
            dc.DrawText (static_cast<const char *>(p), -1, rc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
            }
         break;
         }
      }
   }


bool TGComboBox::IsModified ()
   {
   bool Editable = (Attr.Style&CBS_DROPDOWNLIST)==CBS_DROPDOWNLIST ? false:true;
   if (Editable)
      {
      char p[255];
      GetText (p, 255);
      if (oldstr != p)
         return true;
      else
         return false;
      }
   else
      {
      int index = GetSelIndex ();
      if (oldsel != index)
         return true;
      else
         return false;
      }
   }


// addition suggested by David M. Larson. -------------------------------- //
void TGComboBox::ClearModify ()
   {
   bool Editable = (Attr.Style&CBS_DROPDOWNLIST)==CBS_DROPDOWNLIST ? false:true;
   if (Editable)
      {
      char p[255];
      GetText (p, 255);
      oldstr = p;
      }
   oldsel = GetSelIndex ();
   }


// EOF

