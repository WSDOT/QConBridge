/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// gridedit.cpp                                                            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "griddll.h"
#pragma hdrstop

DEFINE_RESPONSE_TABLE1 (TGEdit, TEdit)
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

MOUSE_HANDLER_CODE(Edit)

TGEdit::TGEdit (TWindow* parent, int Id, const char far *txt, int x, int y, int w, int h, uint textLen, bool multiline, TModule* module)
      : TEdit (parent, Id, txt, x, y, w, h, textLen, multiline, module)
   {
   Init ();
   }


TGEdit::TGEdit (TWindow *parent, uint resourceID, uint textLen, TModule *module)
      : TEdit (parent, resourceID, textLen, module)
   {
   Init ();
   }


TGEdit::~TGEdit ()
   {
   delete brush;
   }


void TGEdit::Init ()
   {
   grid = 0;
   columnbkg = bkg = GetSysColor (COLOR_WINDOW);
   brush = new TBrush (bkg);
   text  = GetSysColor (COLOR_WINDOWTEXT);
   if (GetApplication()->Ctl3dEnabled() || isChicago)
      columnbkg = GetSysColor (COLOR_BTNFACE);
   savedValidator = 0;
   }


bool TGEdit::Create ()
   {
   bool rc = TEdit::Create ();
   if (rc && grid)
      grid->CalcPos ();
   return rc;
   }


bool TGEdit::PreProcessMsg (MSG &msg)
   {
   if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
      {
      if (grid && grid->KeyDown (msg.wParam, LOWORD(msg.lParam)) )
         return true;

      // bypass Window's direct close of the dialog when using ML edits.
      else if ((Attr.Style & ES_MULTILINE) != 0)
         {
         TDialog *dlg = dynamic_cast<TDialog *>(Parent);
         if (dlg)
            {
            dlg->SendMessage (WM_COMMAND, IDCANCEL);
            return true;
            }
         }
      }

   return TEdit::PreProcessMsg (msg);
   }


void TGEdit::EvKillFocus (HWND hWndGetFocus)
   {
   if (grid)
      {
      TControl::EvKillFocus (hWndGetFocus);

      bool ok = false;
      if (IsModified ())
         {
         int l = Transfer (0, tdSizeData);
         void *data = static_cast<void *>(new char[l]);
         Transfer (data, tdGetData);
         ok = grid->DataChanging (grid->GetCurrentLine(), GetId(), data);
         delete [] data;
         }

      grid->KillFocusFromChild (this, hWndGetFocus, ok);
      TGrid::Column *e = grid->FindColumn (hWndGetFocus);
      if (e || IsValid (false) == true)
         return;
      }

   TEdit::EvKillFocus (hWndGetFocus);
   }


void TGEdit::EvSetFocus (HWND hWndLostFocus)
   {
   if (grid)
      {
      ClearModify ();
      grid->SetFocusOnChild (this, hWndLostFocus);
      SetSelection (0,-1);
      ClearModify ();
      }
   TEdit::EvSetFocus (hWndLostFocus);
   }


uint TGEdit::EvGetDlgCode (MSG far *msg)
   {
   if (grid)
      return TEdit::EvGetDlgCode (msg) | DLGC_WANTALLKEYS;
   else
      return TEdit::EvGetDlgCode (msg);
   }


HBRUSH TGEdit::CtlColors (HDC hDC)
   {
   TDC dc(hDC);
   dc.SetTextColor (text);
   dc.SetBkColor (bkg);
   return *brush;
   }


void TGEdit::SetColors (TColor t, TColor b)
   {
   text = t;
   columnbkg = bkg = b;
   delete brush;
   brush = new TBrush (bkg);
   }


void TGEdit::SetColors (TColor t, TColor b, TColor a)
   {
   text = t;
   bkg = b;
   columnbkg = a;
   delete brush;
   brush = new TBrush (bkg);
   }


void TGEdit::EvKeyDown (uint key, uint repeat, uint flags)
   {
   if (grid)
      {
      switch (key)
         {
         case VK_HOME:
         case VK_END:
         case VK_TAB:
         case VK_RETURN:
         case VK_UP:
         case VK_DOWN:
         case VK_PRIOR:
         case VK_NEXT:
         case VK_INSERT:
         case VK_DELETE:
         case VK_ESCAPE:
            if (grid->KeyDown (key, repeat))
               return;
            break;
         }
      }
   TEdit::EvKeyDown (key, repeat, flags);
   }


void TGEdit::EvKeyUp (uint key, uint repeat, uint flags)
   {
   if (grid)
      {
      switch (key)
         {
         case VK_HOME:
         case VK_END:
         case VK_TAB:
         case VK_RETURN:
         case VK_UP:
         case VK_DOWN:
         case VK_PRIOR:
         case VK_NEXT:
         case VK_INSERT:
         case VK_DELETE:
         case VK_ESCAPE:
            return;
         }
      }
   TEdit::EvKeyUp (key, repeat, flags);
   }


void TGEdit::EvChar (uint key, uint repeat, uint flags)
   {
   if (grid)
      {
      switch (key)
         {
         case VK_TAB:
         case VK_RETURN:
         case VK_ESCAPE:
            return;
         }
      }
   TEdit::EvChar (key, repeat, flags);
   }


void TGEdit::DrawCell (TDC &dc, TRect &rc, const void *p, TGrid::Align align) 
   {
   TColor bg, fg;
   HBRUSH dummy;

   int line = grid->GetDataLine (rc.top);
   int id = GetId ();

   if (grid->LineSelected (line))
      {
      fg = grid->hilitetext;
      bg = grid->hilite;
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

   dc.SetTextColor (fg);
   dc.SetBkColor   (bg);
   dc.FillRect (rc, TBrush(bg));

   if (p && *static_cast<const char *>(p))
      {
      TRect fmt;
      GetRect (fmt);  // get formatting rectangle
      if (fmt.IsNull ())
         {
         fmt = rc;
         fmt.Inflate (-2,-2);
         }
      rc.left += fmt.left;
      rc.top  += fmt.top;
      rc.right -= fmt.left;
      rc.bottom = rc.top + fmt.Height();
      dc.DrawText (static_cast<const char *>(p), -1, rc, (WORD)(align|DT_NOPREFIX|DT_WORDBREAK));
      }
   }


TGEdit::operator string ()
   {
   int len = GetTextLen() + 1;
   char *buffer = new char[len];
   GetText (buffer, len);
   string s = buffer;
   delete [] buffer;
   return s;
   }

 
string &TGEdit::operator = (string &s)
   {
   SetText (string(s.strip(string::Trailing)).c_str());
   return s;
   }


// EOF

