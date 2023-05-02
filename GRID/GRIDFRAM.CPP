/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// gridfram.cpp                                                            //
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


// Frame3d ------------------------------------------------------------------
void Frame3d (TDC &dc, TRect &rc, int w, TColor topleft, TColor bottomright)
   {
   Frame3d (dc, rc.left, rc.top, rc.right-1, rc.bottom-1, w, topleft, bottomright);
   }


void Frame3d (TDC &dc, int xorg, int y0, int x1, int y1, int w, TColor topleft, TColor bottomright)
   {
   for (int i = 0; i < w; i++)
      {
      dc.SelectObject (TPen (topleft));
      dc.MoveTo (x1, y0);
      dc.LineTo (xorg, y0);
      dc.LineTo (xorg, y1);
      dc.RestorePen ();

      dc.SelectObject (TPen (bottomright));
      dc.LineTo (x1, y1);
      dc.LineTo (x1, y0);
      dc.RestorePen ();
      if (i < w-1)
         {
         xorg++;
         y0++;
         x1--;
         y1--;
         }
      }
   }


// ------------------------------------------------------------------------//
// TGrid::Client                                                           //
// ------------------------------------------------------------------------//
DEFINE_RESPONSE_TABLE1 (TGrid::Client, TControl)
   EV_WM_CTLCOLOR,
   EV_WM_ERASEBKGND,
   EV_WM_KILLFOCUS,
   EV_WM_LBUTTONDOWN,
   EV_WM_LBUTTONUP,
   EV_WM_LBUTTONDBLCLK,
   EV_WM_RBUTTONDOWN,
   EV_WM_RBUTTONUP,
   EV_WM_RBUTTONDBLCLK,
   EV_WM_SETFOCUS,
END_RESPONSE_TABLE;

TGrid::Client::Client (TGrid *g)
   : TControl (g, -1, 0, 0, 0, 0, 0, 0),
     grid (g)
   {
   Attr.Style &= (~WS_TABSTOP);
   Attr.Style |= WS_BORDER|WS_CLIPSIBLINGS;
   }


bool TGrid::Client::Create ()
   {
   bool rc = TControl::Create ();
   if (rc)
      grid->CalcPos ();
   return rc;
   }


char *TGrid::Client::GetClassName()
   {
   #ifdef __WIN32__
   return "TGrid::Client32";
   #else
   return "TGrid::Client16";
   #endif
   }


void TGrid::Client::Paint (TDC &dc, bool erase, TRect &clip)
   {
   grid->PaintClient (dc, erase, clip);
   }


HBRUSH TGrid::Client::EvCtlColor(HDC hDC, HWND hWndChild, uint ctlType)
   {
   return grid->CtlColor (hDC, hWndChild, ctlType);
   }


void TGrid::Client::EvLButtonDown (uint modKeys, TPoint &point)
   {
   grid->ProcessMouse (modKeys, point, WM_LBUTTONDOWN);
   }


void TGrid::Client::EvLButtonUp (uint modKeys, TPoint &point)
   {
   grid->ProcessMouse (modKeys, point, WM_LBUTTONUP);
   }


void TGrid::Client::EvLButtonDblClk (uint modKeys, TPoint &point)
   {
   grid->ProcessMouse (modKeys, point, WM_LBUTTONDBLCLK);
   }


void TGrid::Client::EvRButtonDown (uint modKeys, TPoint &point)
   {
   grid->ProcessMouse (modKeys, point, WM_RBUTTONDOWN);
   }


void TGrid::Client::EvRButtonUp (uint modKeys, TPoint &point)
   {
   grid->ProcessMouse (modKeys, point, WM_RBUTTONUP);
   }


void TGrid::Client::EvRButtonDblClk (uint modKeys, TPoint &point)
   {
   grid->ProcessMouse (modKeys, point, WM_RBUTTONDBLCLK);
   }


void TGrid::Client::EvKillFocus (HWND hWndGetFocus)
   {
   grid->EvKillFocus (hWndGetFocus);
   }


void TGrid::Client::EvSetFocus (HWND hwndLooseFocus)
   {
   grid->EvSetFocus (hwndLooseFocus);
   }


LRESULT TGrid::Client::WindowProc(uint msg, WPARAM wParam, LPARAM lParam)
   {
   // handle WM_SETCURSOR here because BC chokes on the EV_WM_SETCURSOR line in the response table
   if (msg == WM_SETCURSOR)
      EvSetCursor (HWND(wParam), LOWORD(lParam), HIWORD(lParam));

   // Pass all commands through to TGrid itself, so that EV_COMMAND entries can be handled by TGrid directly
   else if (msg == WM_COMMAND)
      Parent->SendMessage (msg, wParam, lParam);

   // default processing
   return TControl::WindowProc (msg, wParam, lParam);
   }


bool TGrid::Client::EvSetCursor (HWND hwnd, uint hitTest, uint)
   {
   if (hwnd == HWindow && hitTest == HTCLIENT)
      {
      TPoint p;
      GetCursorPos (p);
      ScreenToClient (p);
      if (GetClientRect().Contains(p) && grid->SetCursorShape (p))
         return true;
      }

   SetCursor (0,0);  // mouse quitting the control
   return true; /*bool(DefaultProcessing());*/
   }


// ------------------------------------------------------------------------//
// TGrid::Column                                                           //
// ------------------------------------------------------------------------//
TGrid::Column::Column (TControl *c, Column *prev, TGrid *g, const char *t)
   : next (0),
     control (c),
     width(0),
     left(0),
     align (Left),
     grid (g),
     title (t),
     font (0),
     Hidden(0)                     // Charles R. Cordell
   {
   if (prev)
      prev->next = this;
   previous = prev;
   }


TGrid::Column::~Column ()
   {
   if (font)
      delete font;
   if (next)
      delete next;
   }


void TGrid::Column::HideControl ()
   {
   if (control && !Hidden)          // Charles R. Cordell
      {
      if (type == gCombo)
         {
         TComboBox *cb = dynamic_cast<TComboBox *>(control);
         if (cb->GetDroppedState ())
            cb->HideList ();
         }

      control->SetWindowPos (0, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_HIDEWINDOW);
      if (dynamic_cast<TGComboBox *>(control))
         {
         int y = grid->lineHeight*grid->activeLine;
         TRect rc (TPoint (left, y-1), TSize (width, grid->lineHeight));
         grid->client->InvalidateRect (rc, false);
         }
      }
   }


void TGrid::Column::ShowControl ()
   {
   if (control && !control->IsWindowVisible() && grid->nLines > 0 && !Hidden) // Charles R. Cordell
      {
      int x = left;
      int y = grid->lineHeight * grid->activeLine;
      int h = (type == gCombo) ? dynamic_cast<TGComboBox *>(control)->totalHeight : grid->lineHeight-1;
      int w = (type == gCombo) ? width-1 : width;

      // place check boxes in the center of the column if they have no text and a center alignment was required
      if (type == gCheck && align == TGrid::Center)
         {
         int dx = (width-14+1)/2;       // 14 is the pixel width of a check box
         x += dx;
         w -= (dx+1);
         if (h >= 15)                   // 13 is the pixel height of a check box
            {
            y++;
            h -= 2;
            }
         }
      control->SetWindowPos (0, 0,0,0,0, SWP_NOZORDER|SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE);
      control->SetWindowPos (HWND_TOP, x, y, w, h, 0);
      }
   }


bool TGrid::Column::SetFocus ()
   {
   if (!grid->IsWindowEnabled ())
      return false;

   if (control && type != gStatic && control->IsWindow () && control->IsWindowEnabled () && !Hidden) // Charles R. Cordell
      {
      bool isTabStop = (control->Attr.Style & WS_TABSTOP) ? true : false;
      bool self = (grid->current == this) ? true : false;
      int line = grid->GetCurrentLine ();
      Visible v = grid->VisibleCell (line, control->GetId());
      bool locked = (v == gEmpty || v == gLocked);
      if (!locked && isTabStop)
         {
         bool undo = false;
         if (grid->current && !self && grid->current->control->IsWindowVisible ())
            {
            grid->current->HideControl ();
            undo = true;
            }
         ShowControl ();
         if (control->SetFocus ())
            {
            ShowControl ();
            grid->current = this;
            grid->client->UpdateWindow ();
            return true;
            }
         else
            {
            if (grid->current && !self && undo)
               grid->current->ShowControl ();
            if (!self)
               HideControl ();
            grid->client->UpdateWindow ();
            }
         }
      }
   return false;
   }


bool TGrid::Column::CanKillFocus ()
   {
   if (grid->current == 0)
      return true;
   TGEdit *c = dynamic_cast<TGEdit *>(grid->current->control);
   if (c)
      return c->IsValid(true);
   else
      return true;
   }


// return the width of control owned by column --------------------------- //
int TGrid::Column::CtrlWidth () const
   {
   if( Hidden )          // Charles R. Cordell
     return 0;           // Charles R. Cordell

   switch (type)
      {
      case gTimeEdit:
         return dynamic_cast<TGTimeEdit *>(control)->Width();
      case gDateEdit:
         return dynamic_cast<TGDateEdit *>(control)->Width();
      case gShortEdit:
      case gLongEdit:
      case gDoubleEdit:
      case gEdit:
         return dynamic_cast<TGEdit *>(control)->Width();
      case gStatic:
         return dynamic_cast<TGStatic *>(control)->Width();
      case gCheck:
         return dynamic_cast<TGCheckBox*>(control)->Width();
      case gCombo:
         return dynamic_cast<TGComboBox*>(control)->Width();
      case gButton:
         return dynamic_cast<TGButton*>(control)->Width();
      }
   return 0;
   }


// return the height of control owned by column -------------------------- //
int TGrid::Column::CtrlHeight () const
   {
   switch (type)
      {
      case gTimeEdit:
         return dynamic_cast<TGTimeEdit *>(control)->Height();
      case gDateEdit:
         return dynamic_cast<TGDateEdit *>(control)->Height();
      case gShortEdit:
      case gLongEdit:
      case gDoubleEdit:
      case gEdit:
         return dynamic_cast<TGEdit *>(control)->Height();
      case gStatic:
         return dynamic_cast<TGStatic *>(control)->Height();
      case gCheck:
         return dynamic_cast<TGCheckBox*>(control)->Height();
      case gCombo:
         return dynamic_cast<TGComboBox*>(control)->Height();
      case gButton:
         return dynamic_cast<TGButton*>(control)->Height();
      }
   return 0;
   }


void TGrid::Column::SetTitle (const char *newtitle)
   {
   title = newtitle;
   if (grid->title && left < grid->clientWidth && left+width >= 0)
      {
      TRect rc (left+1, 1, left+width, grid->titleHeight);
      grid->title->InvalidateRect (rc, false);
      }
   }


void TGrid::Column::SetFont (const char *facename, int height, int weight, bool italic)
   {
   if (facename || height)
      {
      LOGFONT lf;
      if (grid->gridLogFont)
         memcpy (&lf, grid->gridLogFont, sizeof(lf));
      else
         memset (&lf, 0, sizeof(lf));
      strcpy (lf.lfFaceName, facename);
      if (height)
         lf.lfHeight = height;
      lf.lfWeight = weight;
      lf.lfItalic = BYTE(italic);
      font = new TFont (&lf);
      }
   }


void TGrid::Column::SetFont ()
   {
   if (control && control->IsWindow ())
      control->SetWindowFont (HFONT(font ? *font : *grid->gridFont), true);
   }


char *TGrid::Column::GetClassName()
   {
   #ifdef __WIN32__
   return "TGrid::Column32";
   #else
   return "TGrid::Column16";
   #endif
   }


// ------------------------------------------------------------------------//
// TGrid::VScroll                                                          //
// ------------------------------------------------------------------------//
DEFINE_RESPONSE_TABLE1 (TGrid::VScroll, TScrollBar)
   EV_WM_KEYDOWN,
END_RESPONSE_TABLE;

TGrid::VScroll::VScroll (TGrid *g)
     : TScrollBar (g, -1, 0, 0, 0, 0, false),
       grid (g),
       minRange (0),
       maxRange (100),
       factor (1)
   {
   Attr.Style &= (~WS_TABSTOP);
   }


void TGrid::VScroll::EvKeyDown (uint key, uint repeat, uint flags)
   {
   switch (key)
      {
      case VK_SPACE:
         grid->SelectLine (grid->GetCurrentLine ());
         return;

      case VK_RETURN:
         if (grid->KeyDown (VK_RETURN, repeat))
            return;
         break;
      }

   TScrollBar::EvKeyDown (key, repeat, flags);
   }


void TGrid::VScroll::SetLRange (uint min, uint max, bool draw)
   {
   minRange = min;
   maxRange = max;
   if (maxRange >= 0x8000u)
      factor = (maxRange+0x8000u) / 0x8000u;
   if (factor < 1)
      factor = 1;
   ::SetScrollRange (*this, SB_CTL, min/factor, max/factor, draw);
   }


void TGrid::VScroll::SetLPosition (uint pos)
   {
   if (factor < 1)
      factor = 1;
   TScrollBar::SetPosition (pos/factor);
   }


void TGrid::VScroll::SBBottom ()
   {
   grid->Go (grid->nLines-1);
   }


void TGrid::VScroll::SBLineDown ()
   {
   grid->Move (1);
   }


void TGrid::VScroll::SBLineUp ()
   {
   grid->Move (-1);
   }


void TGrid::VScroll::SBPageDown ()
   {
   grid->Move (grid->visibleLines);
   }


void TGrid::VScroll::SBPageUp ()
   {
   grid->Move (-grid->visibleLines);
   }


void TGrid::VScroll::SBThumbPosition (int pos)
   {
   if (factor < 1)
      factor = 1;
   grid->Go (pos*factor);
   }


void TGrid::VScroll::SBThumbTrack (int pos)
   {
   if (factor < 1)
      factor = 1;
   grid->Go (pos*factor);
   }
               

void TGrid::VScroll::SBTop ()
   {
   grid->Go (0);
   }


// ------------------------------------------------------------------------//
// TGrid::HScroll                                                          //
// ------------------------------------------------------------------------//
TGrid::HScroll::HScroll (TGrid *g)
     : TScrollBar (g, -1, 0, 0, 0, 0, true),
       grid (g)
   {
   Attr.Style &= (~WS_TABSTOP);
   }


void TGrid::HScroll::SBBottom ()
   {
   int min, max;
   GetRange (min, max);
   grid->SetHPos (max);
   }


void TGrid::HScroll::SBLineDown ()
   {
   grid->MoveHPos (LineMagnitude);
   }


void TGrid::HScroll::SBLineUp ()
   {
   grid->MoveHPos (-LineMagnitude);
   }


void TGrid::HScroll::SBPageDown ()
   {
   grid->MoveHPos (PageMagnitude);
   }


void TGrid::HScroll::SBPageUp ()
   {
   grid->MoveHPos (-PageMagnitude);
   }


void TGrid::HScroll::SBThumbPosition (int pos)
   {
   if (grid->hpos != pos)
      grid->SetHPos (pos);
   }


void TGrid::HScroll::SBThumbTrack (int pos)
   {
   if (grid->hpos != pos)
      grid->SetHPos (pos);
   }


void TGrid::HScroll::SBTop ()
   {
   if (grid->hpos != 0)
      grid->SetHPos (0);
   }


// ------------------------------------------------------------------------//
// TGrid::Title                                                            //
// ------------------------------------------------------------------------//
DEFINE_RESPONSE_TABLE1 (TGrid::Title, TControl)
   EV_WM_LBUTTONDOWN,
   EV_WM_LBUTTONUP,
   EV_WM_MOUSEMOVE,
END_RESPONSE_TABLE;

TGrid::Title::Title (TGrid *g, bool show)
   : TControl (g, -1, 0, 0, 0, 0, 0, 0),
     grid (g),
     capture (false),
     visible (show)
   {
   Attr.Style &= (~WS_BORDER);  
   Attr.Style &= (~WS_TABSTOP);
   }


LRESULT TGrid::Title::WindowProc(uint msg, WPARAM wParam, LPARAM lParam)
   {
   if (visible && msg == WM_SETCURSOR)
      EvSetCursor (HWND(wParam), LOWORD(lParam), HIWORD(lParam));

   return TControl::WindowProc (msg, wParam, lParam);
   }


bool TGrid::Title::Create ()
   {
   bool rc = TControl::Create ();
   if (rc)
      grid->CalcPos ();
   return rc;
   }


char *TGrid::Title::GetClassName()
   {
   #ifdef __WIN32__
   return "TGrid::Title32";
   #else
   return "TGrid::Title16";
   #endif
   }


void TGrid::Title::GetWindowClass(WNDCLASS &wc)
   {
   TControl::GetWindowClass(wc);
   wc.style &= ~CS_GLOBALCLASS;
   wc.hCursor = 0;
   }


int TGrid::Title::CalcHeight ()
   {
   if (!visible)
      return 0;

   int h;
   bool del = false;
   TFont *f = grid->titleFont;
   if (f == 0)
      {
      if (grid->titleLogFont)
         {
         f = new TFont (grid->titleLogFont);
         del = true;
         }
      else
         {
         f = grid->gridFont;
         if (f == 0)
            {
            f = new TFont (grid->gridLogFont);
            del = true;
            }
         }
      }
   TScreenDC dc;
   dc.SelectObject (*f);
   TEXTMETRIC tm;
   dc.GetTextMetrics (tm);
   dc.RestoreFont ();
   h = tm.tmHeight + 6;  // minimum
   if (del)
      delete f;
   if (h < 10)
      h = 10;
   return h;
   }


void TGrid::Title::Paint (TDC &dc, bool, TRect &clip)
   {
   if (!visible)
      return;

   if (grid->titleFont)
      SetWindowFont (HFONT(*grid->titleFont), true);

   dc.FillRect (clip, TBrush (grid->GetTitleColor (TitleBackground)));
   dc.SelectClipRgn (TRegion (clip));
   TFont *font = grid->titleFont;
   if (font == 0)
      font = grid->gridFont;
   dc.SelectObject (*font);
   TPen pen (grid->GetTitleColor (TitleFrame));

   for (Column *ctrl = grid->firstColumn; ctrl; ctrl = ctrl->next)
      {
      if (ctrl->Hidden)    // Charles R. Cordell
         continue;         // Charles R. Cordell

      if (ctrl->previous)
         {
         dc.SelectObject (pen);
         dc.MoveTo (ctrl->left, 0);
         dc.LineTo (ctrl->left, Attr.H);
         dc.RestorePen ();
         }

      Align align = grid->centerTitle ? Center : ctrl->align;
      DrawCell (dc, ctrl->left, ctrl->width, ctrl->title.c_str(), align);
      }

   dc.SelectObject (pen);
   dc.MoveTo (0, 0);
   dc.LineTo (0, Attr.H);
   dc.MoveTo (Attr.W-1, 0);
   dc.LineTo (Attr.W-1, Attr.H);
   dc.MoveTo (0,0);
   dc.LineTo (Attr.W, 0);
   dc.RestorePen ();

   dc.RestoreFont ();
   }


void TGrid::Title::DrawCell (TDC &dc, int x, int w, const char *p, TGrid::Align a)
   {
   w = x+w-1;
   int h = Attr.H-1;
   Frame3d (dc, x+1, 1, w, h, 1, grid->GetTitleColor (TitleHilite), grid->GetTitleColor (TitleShadow));

   if (p)
      {
      TRect rc (x+2, 2, w-1, h-1);
      dc.SetTextColor (grid->GetTitleColor (TitleText));
      dc.SetBkColor (grid->GetTitleColor (TitleBackground));
      dc.DrawText (p, -1, rc, (WORD)(a|DT_SINGLELINE|DT_VCENTER));
      }
   }


bool TGrid::Title::EvSetCursor (HWND hwnd, uint hitTest, uint)
   {
   if (visible && hwnd == HWindow && hitTest == HTCLIENT)
      {
      TPoint p;
      GetCursorPos (p);
      ScreenToClient (p);
      if (GetClientRect ().Contains (p))
         {
         if (MoveMouseCursor ())
            {
            #ifndef _GRIDLIB
               SetCursor (GridModule, HRESIZE);
            #else
               SetCursor (GetModule(), HRESIZE);
            #endif
            }
         else
            SetCursor (0, IDC_ARROW);
         return true;
         }
      }

   SetCursor (0,0);  // mouse quitting the control
   return true;
   }


bool TGrid::Title::MoveMouseCursor ()
   {
   if (grid->sizeColumn == false)
      return false;

   TPoint p;
   GetCursorPos (p);
   ScreenToClient (p);
   int x = p.x+1;

   for (Column *e = grid->firstColumn; e; e = e->next)
      {
      if (e->Hidden)
         continue;
      int right = e->left+e->width;
      if (x >= right-4 && x <= right+4)
         {
         column = e;
         return true;
         }
      else if (x >= e->left && x < right)
         return false;
      }

   return false;
   }


void TGrid::Title::EvLButtonDown (uint modKeys, TPoint &p)
   {
   TControl::EvLButtonDown (modKeys, p);
   if (visible)
      {
      if (capture == false && MoveMouseCursor () && column)
         {
         xorg = p.x;
         SetCapture ();
         capture = true;
         if (grid->current && grid->current->control->IsWindowVisible ())
            {
            showBack = true;
            grid->current->HideControl ();
            }
         else
            showBack = false;
         }
      else
         grid->TitleLButtonDown (grid->FindColumn (p));
      }
   }


void TGrid::Title::EvLButtonUp (uint modKeys, TPoint &p)
   {
   TControl::EvLButtonUp (modKeys, p);
   if (visible)
      {
      if (capture)
         {
         ReleaseCapture ();
         capture = false;
         grid->client->Invalidate ();
         if (showBack)
            grid->current->ShowControl ();
         }
      else
         grid->TitleLButtonUp (grid->FindColumn (p));
      }
   }


void TGrid::Title::EvMouseMove (uint modKeys, TPoint &p)
   {
   TControl::EvMouseMove (modKeys, p);
   if (visible && capture)
      {
      if (p.x < column->left+8)
         p.x = column->left+8;
      grid->ChangeColWidth (column, p.x-xorg);
      xorg = p.x;
      }
   }


// ------------------------------------------------------------------------//
// TGrid::VerticalBar                                                      //
// ------------------------------------------------------------------------//
DEFINE_RESPONSE_TABLE1 (TGrid::VerticalBar, TControl)
   EV_WM_LBUTTONDOWN,
   EV_WM_LBUTTONUP,
   EV_WM_LBUTTONDBLCLK,
   EV_WM_RBUTTONDOWN,
   EV_WM_RBUTTONUP,
   EV_WM_RBUTTONDBLCLK,
END_RESPONSE_TABLE;

TGrid::VerticalBar::VerticalBar (TGrid *g)
   : TControl (g, -1, 0, 0, 0, 0, 0, 0),
     grid (g),
     lineNumber (true),
     charWidth (4)
   {
   Attr.Style |= WS_BORDER;
   Attr.Style &= (~WS_TABSTOP);
   }


bool TGrid::VerticalBar::Create ()
   {
   bool rc = TControl::Create ();
   if (rc)
      grid->CalcPos ();
   return rc;
   }


int TGrid::VerticalBar::CalcWidth ()
   {
   int w = 16;
   bool del = false;
   TFont *f = grid->titleFont;
   if (f == 0)
      f = grid->gridFont;
   if (f == 0)
      {
      f = new TFont (grid->gridLogFont);
      del = true;
      }
   if (f)
      {
      TScreenDC dc;
      dc.SelectObject (*f);
      TEXTMETRIC tm;
      dc.GetTextMetrics (tm);
      dc.RestoreFont ();
      w = tm.tmAveCharWidth * charWidth + 8;
      }
   if (del)
      delete f;
   if (w < 16)
      w = 16;   // default
   return w;
   }


void TGrid::VerticalBar::SetUserText (int cwidth)
   {
   if (!IsWindow ())
      {
      lineNumber = false;
      if (cwidth == 0)
         cwidth = 4;
      charWidth = cwidth;
      }
   }


void TGrid::VerticalBar::Paint (TDC &dc, bool, TRect &clip)
   {
   if (grid->titleFont)
      SetWindowFont (HFONT(*grid->titleFont), true);

   dc.SelectClipRgn (TRegion (clip));
   dc.FillRect (clip, TBrush (grid->GetTitleColor (TitleBackground)));

   int lh = grid->lineHeight;
   int x0 = 0;
   int x1 = Attr.W;
   int y0 = 0;
   int y1 = lh-1;
   TPen pen (grid->GetTitleColor (TitleFrame));
   for (int i = 0; i < grid->visibleLines; i++, y0 += lh, y1 += lh)
      {
      TRect rc (x0, y0, x1, y1);
      if (rc.Touches (clip))
         {
         DrawCell (dc, rc, i);
         dc.SelectObject (pen);
         dc.MoveTo (0, rc.bottom);
         dc.LineTo (Attr.W, rc.bottom);
         dc.RestorePen ();
         }
      }
   }


const char *TGrid::VerticalBar::Info (int line) const
   {
   static char string[10];
   const char *info;
   if (lineNumber)
      {
      sprintf (string, "%d", grid->firstLine+line+1);
      info = string;
      }
   else
      info = static_cast<const char *>(grid->TransferItem (grid->firstLine+line, 0));

   return info;
   }


void TGrid::VerticalBar::DrawCell (TDC &dc, TRect &rc, int line)
   {
   TRect clip = dc.GetClipBox ();
   dc.SelectClipRgn (TRegion (rc));
   Frame3d (dc, rc, 1, grid->GetTitleColor (TitleHilite),
                       grid->GetTitleColor (TitleShadow));
   if (line == grid->activeLine)
      {
      if (grid->lineMarker != ArrowOnly)
         ShowText (line);
      ShowMarker (line);
      }
   else
      ShowText (line);
   dc.SelectClipRgn (TRegion (clip));
   }


void TGrid::VerticalBar::ShowText (int line) const
   {
   TClientDC dc (HWindow);
   TRect rc = FillRect (dc, line, true);
   if (grid->lineMarker == ReverseColor)
      {
      TRect rc1 (rc);
      rc1.Inflate (2, 2);
      Frame3d (dc, rc1, 1, grid->GetTitleColor (TitleHilite),
                           grid->GetTitleColor (TitleShadow));
      }

   const char *info = Info (line);
   if (info)
      {
      TFont *font = grid->titleFont;
      if (font == 0)
         font = grid->gridFont;
      dc.SelectObject (*font);
      dc.SetTextColor (grid->GetTitleColor (TitleText));
      dc.SetBkColor (grid->GetTitleColor (TitleBackground));
      dc.DrawText (info, -1, rc, (WORD)(DT_RIGHT|DT_VCENTER|DT_SINGLELINE));
      dc.RestoreFont ();
      }
   }


void TGrid::VerticalBar::ShowMarker (int line) const
   {
   TClientDC dc (HWindow);
   TRect rc = FillRect (dc, line, false);
   if (grid->lineMarker == ReverseColor)
      {
      TRect rc1 (rc);
      rc1.Inflate (2, 2);
      dc.InvertRect (rc1);
      }
   else
      {
      dc.SelectObject (TPen (grid->GetTitleColor (TitleText)));
      int x = rc.left+4;
      int y0 = rc.top+2;
      int y1 = rc.bottom-2;
      while (y0 < y1 && x < rc.right)
         {
         dc.MoveTo (x, y0++);
         dc.LineTo (x++, y1--);
         }
      dc.RestorePen ();
      }
   }


TRect TGrid::VerticalBar::FillRect (TDC &dc, int line, bool erase) const
   {
   int lh = grid->lineHeight;
   int y = line*lh;
   TRect rc (1, y+1, Attr.W-3, y+lh-2);
   if (erase || grid->lineMarker == ArrowOnly)
      dc.FillRect (rc, TBrush(grid->GetTitleColor (TitleBackground)));
   return rc.InflatedBy(-1,-1);
   }

   
void TGrid::VerticalBar::EvLButtonDown (uint modKeys, TPoint &point)
   {
   if (grid->lineHeight > 0)
      {
      // calc cell where mouse was clicked
      switch (grid->lineSelection)
         {
         case Single:
         case Multiple:
         case SingleAutoSelect:
            grid->SelectLine ((point.y / grid->lineHeight) + grid->firstLine, modKeys | MK_TGRID);
            break;

         case None:
            break;
         }
      }

   grid->current->HideControl ();
   grid->ProcessMouse (modKeys, point, WM_LBUTTONDOWN, false);
   }


void TGrid::VerticalBar::EvLButtonUp (uint modKeys, TPoint &point)
   {
   grid->ProcessMouse (modKeys, point, WM_LBUTTONUP);
   }


void TGrid::VerticalBar::EvLButtonDblClk (uint modKeys, TPoint &point)
   {
   grid->ProcessMouse (modKeys, point, WM_LBUTTONDBLCLK);
   }


void TGrid::VerticalBar::EvRButtonDown (uint modKeys, TPoint &point)
   {
   grid->ProcessMouse (modKeys, point, WM_RBUTTONDOWN);
   }


void TGrid::VerticalBar::EvRButtonUp (uint modKeys, TPoint &point)
   {
   grid->ProcessMouse (modKeys, point, WM_RBUTTONUP);
   }


void TGrid::VerticalBar::EvRButtonDblClk (uint modKeys, TPoint &point)
   {
   grid->ProcessMouse (modKeys, point, WM_RBUTTONDBLCLK);
   }


char *TGrid::VerticalBar::GetClassName()
   {
   #ifdef __WIN32__
   return "TGrid::VerticalBar32";
   #else
   return "TGrid::VerticalBar16";
   #endif
   }


// EOF
