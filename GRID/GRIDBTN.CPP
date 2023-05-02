/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// gridbtn.cpp                                                             //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "griddll.h"
#pragma hdrstop
                 

DEFINE_RESPONSE_TABLE1 (TGButton, TButton)
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

MOUSE_HANDLER_CODE(Button)

TGButton::TGButton (TWindow *p, int id, TModule *module)
        : TButton (p, id, module)
   {
   grid = 0;
   }

   
TGButton::TGButton (TWindow *p, int id, char *text, int x, int y,
                    int w, int h, bool isdefault, TModule *module)
        : TButton (p, id, text, x, y, w, h, isdefault, module)
   {
   grid = 0;
   }


void TGButton::EvSetFocus (HWND hWndLostFocus)
   {
   if (grid)
      grid->SetFocusOnChild (this, hWndLostFocus);
   TButton::EvSetFocus (hWndLostFocus);                                  
   }


void TGButton::EvKillFocus (HWND hWndGetFocus)
   {
   if (grid)
      grid->KillFocusFromChild (this, hWndGetFocus, true);

   TButton::EvKillFocus (hWndGetFocus);
   }


GRID_EV_GETDLGCODE(Button)
GRID_EV_KEYUP(Button)
GRID_EV_CHAR(Button)
GRID_EV_KEYDOWN(Button) 


bool TGButton::Create ()
   {
   bool rc = TButton::Create ();
   if (rc && grid)
      grid->CalcPos ();
   return rc;
   }


bool TGButton::PreProcessMsg (MSG &msg)
   {
   if (grid                      &&
       msg.message == WM_KEYDOWN &&
       msg.wParam == VK_ESCAPE   &&
       grid->KeyDown (msg.wParam, LOWORD(msg.lParam)) )
      return true;

   return TButton::PreProcessMsg (msg);
   }


void TGButton::DrawCell (TDC &dc, TRect &rc, const void *p, TGrid::Align)
   {
   TColor bg, fg;
   HBRUSH dummy;

   int line = grid->GetDataLine(rc.top);
   int id = GetId ();

   bool customColors = grid->CellColor (line, id, false, bg, fg, dummy);
   if (!customColors)
      {
      bg = GetSysColor (COLOR_BTNFACE);
      fg = GetSysColor (COLOR_BTNTEXT);
      }

   switch (grid->VisibleCell (line, id))
      {
      case TGrid::gControl:
         {
         dc.SelectObject (TBrush (bg));
         dc.SelectObject (TPen (fg));
         dc.Rectangle (rc);
         dc.RestorePen ();
         dc.RestoreBrush ();

         if (p && *static_cast<const char *>(p))
            {
            dc.SetBkColor (bg);
            dc.SetTextColor (fg);
            dc.DrawText (static_cast<const char *>(p), -1, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
            }

         int x0 = rc.left+1;
         int y0 = rc.top+1;
         int x1 = rc.right-2;
         int y1 = rc.bottom-2;
         Frame3d (dc, x0, y0, x1, y1, 2,
            GetSysColor (COLOR_BTNHIGHLIGHT), GetSysColor (COLOR_BTNSHADOW));
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
            dc.DrawText (static_cast<const char *>(p), -1, rc,
                                         DT_CENTER|DT_VCENTER|DT_SINGLELINE);
            }
         break;
         }
      }
   }

uint TGButton::Transfer (void *buffer, TTransferDirection direction)
   {
   if (buffer)
      {
      switch (direction)
         {
         case tdSetData:
            SetCaption (const_cast<char *>(static_cast<char *>(buffer)));
            break;
         }
      }

   return 0;
   }

// EOF
