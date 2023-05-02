/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// grid.cpp                                                                //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "griddll.h"
#pragma hdrstop
#include <ctl3d.h>
  

// TGrid ----------------------------------------------------------------- //
DEFINE_RESPONSE_TABLE1 (TGrid, TControl)
   EV_WM_ERASEBKGND,
   EV_WM_KILLFOCUS,
   EV_WM_PAINT,
   EV_WM_NCPAINT,
   EV_WM_SETFOCUS,
   EV_WM_SHOWWINDOW,
   EV_WM_WINDOWPOSCHANGING,
   EV_WM_LBUTTONDOWN,
   EV_WM_LBUTTONUP,
   EV_WM_LBUTTONDBLCLK,
   EV_WM_RBUTTONDOWN,
   EV_WM_RBUTTONUP,
   EV_WM_RBUTTONDBLCLK,
END_RESPONSE_TABLE;


// ----------------------------------------------------------------------- //
// constructors, destructor, initialization                                //
// ----------------------------------------------------------------------- //
TGrid::TGrid (TWindow *p, int id, int x0, int y0, int w0, int h0, TGrid::EnterKey ek, TGrid::TabKey tk, TGrid::LineMarker lm,
              int max, int attrib, TModule *module)
    : TControl  (p, id, "", x0, y0, w0, h0, module)
   {
   Init (max, ek, tk, lm, attrib);
   }


TGrid::TGrid (TWindow *p, int id, TGrid::EnterKey ek, TGrid::TabKey tk, TGrid::LineMarker lm, int max, int attrib, TModule *module)
    : TControl  (p, id, module)
   {
   Attr.Style = BS_OWNERDRAW|WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_CLIPSIBLINGS;
   Init (max, ek, tk, lm, attrib);
   }


void TGrid::Init (int max, TGrid::EnterKey ek, TGrid::TabKey tk, TGrid::LineMarker lm, int attrib)
   {
   firstColumn = lastColumn = current = 0;
   nControls = hpos = 0;
   titleHeight = vbarWidth = 0;
   logicalWidth = clientWidth = 0;
   lineHeight = clientHeight = 0;
   firstLine = activeLine = visibleLines = 0;
   selectRow = -1;

   if (max >= 0)
      nLines = max;
   else
      nLines = 0;
   maxLines = max;

   lineColor  = TColor::Gray;
   emptyGrid  = GetSysColor (COLOR_WINDOW);
   hilitetext = GetSysColor (COLOR_HIGHLIGHTTEXT);
   hilite     = GetSysColor (COLOR_HIGHLIGHT);

   calcPos = false;
   vscrollHidden = false;
   hscrollHidden = false;
   tempNoHMove = false;
   staticOnly = true;
   modified = false;

   enterKey = ek;
   tabKey = tk;
   lineMarker = lm;
   visible = gControl;

   lineSwapping   = (attrib & gLineSwapOn)      ? true : false;
   centerTitle    = (attrib & gCenterTitle)     ? true : false;
   disableHScroll = (attrib & gDisableHScroll)  ? true : false;
   disableVScroll = (attrib & gDisableVScroll)  ? true : false;
   hideHScroll    = (attrib & gHideHScroll)     ? true : false;
   hideVScroll    = (attrib & gHideVScroll)     ? true : false;
   sizeColumn     = (attrib & gSizeColumn)      ? true : false;
   horzShrink     = (attrib & gHorzShrink)      ? true : false;
   vertShrink     = (attrib & gVertShrink)      ? true : false;
   cursorType     = (attrib & gArrowCursor)     ? ArrowCursor : DefCursor;

   selectedLine = -1;
   nsel = 0;
   lineSelection = (attrib & gSingleSelection) ? Single : None;
   if (lineSelection == Single)
      {
      if ((attrib & gAutoSelection) != 0)
         lineSelection = SingleAutoSelect;
      selectedLines = 0;
      }
   else if ((attrib & gMultipleSelection) != 0)
      {
      lineSelection = Multiple;
      selectedLines = new TBitField (maxLines ? maxLines : 128);
      }
   else
      selectedLines = 0;

   // setup children
   client = new Client (this);
   vscroll = (attrib & gVScroll) ? new VScroll (this)     : 0;
   hscroll = (attrib & gHScroll) ? new HScroll (this)     : 0;
   vbar    = (attrib & gVBar)    ? new VerticalBar (this) : 0;
   title   = new Title (this, (attrib & gTitle) ? true : false);

   if (vscroll)
      cxvscroll = GetSystemMetrics(SM_CXVSCROLL);
   else
      cxvscroll = 0;
   if (hscroll)
      cyhscroll = GetSystemMetrics(SM_CYHSCROLL);
   else
      cyhscroll = 0;

   gridFont = 0;
   titleFont = 0;

   ComboBmp       =
   CheckBoxesBmp  =
   BwccCheckBmp   =
   BwccUncheckBmp =
   Ctl3dCheckBmp  = 0;

   titleLogFont = 0;
   gridLogFont = new LOGFONT;
   memset (gridLogFont, 0, sizeof(LOGFONT));
   strcpy (gridLogFont->lfFaceName, "MS Sans Serif");
   gridLogFont->lfHeight = MulDiv(TScreenDC().GetDeviceCaps (LOGPIXELSY),9,72);
   gridLogFont->lfWeight = FW_BOLD;

   buttonFace = new TBrush (TColor(GetSysColor (COLOR_BTNFACE)));
   }


TGrid::~TGrid ()
   {
   if (gridLogFont)
      delete gridLogFont;
   if (titleLogFont)
      delete titleLogFont;
   if (firstColumn)
      delete firstColumn;
   if (gridFont)
      delete gridFont;
   if (titleFont)
      delete titleFont;
   if (ComboBmp)
      delete ComboBmp;
   if (CheckBoxesBmp)
      delete CheckBoxesBmp;
   if (BwccCheckBmp)
      delete BwccCheckBmp;
   if (BwccUncheckBmp)
      delete BwccUncheckBmp;
   if (Ctl3dCheckBmp)
      delete Ctl3dCheckBmp;
   if (selectedLines)
      delete selectedLines;
   if (buttonFace)
      delete buttonFace;
   }


void TGrid::PerformCreate (int menuOrId)
   {
   Attr.Style = BS_OWNERDRAW|WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_CLIPSIBLINGS;
   TControl::PerformCreate (menuOrId);
   }


bool TGrid::Create ()
   {
   bool rc = TControl::Create ();

   if (rc)
      CalcPos ();

   return rc;
   }


char *TGrid::GetClassName()
   {
   #ifdef __WIN32__
   return "TGrid32";
   #else
   return "TGrid16";
   #endif
   }


void TGrid::GetWindowClass(WNDCLASS &wc)
   {
   TControl::GetWindowClass(wc);
   wc.style &= ~CS_GLOBALCLASS;
   wc.hCursor = 0;
   }


void TGrid::SetupWindow ()
   {
   TControl::SetupWindow ();

   TApplication *app = GetApplication ();
   if (app && app->Ctl3dEnabled())
      {
      bool FAR PASCAL (*SubclassDlg)(HWND, WORD);
      (FARPROC)SubclassDlg = app->GetCtl3dModule()->GetProcAddress("Ctl3dSubclassDlg");
      if (SubclassDlg)
         SubclassDlg (*this, CTL3D_ALL);
      }
   }


void TGrid::Add (TControl *p, const char *t, Align a, int HIDE)   
   {
   if (p)
      {
      Column *ge = new Column (p, lastColumn, this, t);
      lastColumn = ge;
      if (firstColumn == 0)
         firstColumn = ge;

      if (dynamic_cast<TGDateEdit *>(p))         // must be before TGEdit
         {
         dynamic_cast<TGDateEdit *>(p)->grid = this;
         ge->type = gDateEdit;
         staticOnly = false;
         }
      else if (dynamic_cast<TGTimeEdit *>(p))    // must be before TGEdit
         {
         dynamic_cast<TGTimeEdit *>(p)->grid = this;
         ge->type = gTimeEdit;
         staticOnly = false;
         }
      else if (dynamic_cast<TGDoubleEdit *>(p))  // must be before TGEdit
         {
         dynamic_cast<TGDoubleEdit *>(p)->grid = this;
         ge->type = gDoubleEdit;
         staticOnly = false;
         }
      else if (dynamic_cast<TGLongEdit *>(p))    // must be before TGEdit
         {
         dynamic_cast<TGLongEdit *>(p)->grid = this;
         ge->type = gLongEdit;
         staticOnly = false;
         }
      else if (dynamic_cast<TGShortEdit *>(p))   // must be before TGEdit
         {
         dynamic_cast<TGShortEdit *>(p)->grid = this;
         ge->type = gShortEdit;
         staticOnly = false;
         }
      else if (dynamic_cast<TGEdit *>(p))        // must be before TGStatic
         {
         dynamic_cast<TGEdit *>(p)->grid = this;
         ge->type = gEdit;
         staticOnly = false;
         }
      else if (dynamic_cast<TGCheckBox *>(p))    // must be before TGButton
         {
         dynamic_cast<TGCheckBox *>(p)->grid = this;
         ge->type = gCheck;
         staticOnly = false;
         }
      else if (dynamic_cast<TGButton *>(p))
         {
         dynamic_cast<TGButton *>(p)->grid = this;
         ge->type = gButton;
         staticOnly = false;
         }
      else if (dynamic_cast<TGComboBox *>(p))
         {
         dynamic_cast<TGComboBox *>(p)->grid = this;
         ge->type = gCombo;
         staticOnly = false;
         }
      else if (dynamic_cast<TGStatic *>(p))
         {
         dynamic_cast<TGStatic *>(p)->grid = this;
         ge->type = gStatic;
         }

      nControls++;

      if (ge->type == gButton)
         ge->align = Center;
      else
         ge->align = a;

      if( HIDE )                        
        ge->Hidden = -1;                

      // check if column added after grid creation
      if (calcPos)
         {
         AppendColumn ();
         Refresh ();
         }
      }
   }


void TGrid::AppendColumn ()
   {
   int w = lastColumn->CtrlWidth();
   lastColumn->left = logicalWidth;
   lastColumn->width = w+1;

   if (lastColumn->type == gCheck)
      lastColumn->width += 2;

   if (lastColumn->left + lastColumn->width > logicalWidth)
      logicalWidth = lastColumn->left + lastColumn->width;

   lastColumn->control->SetParent (client);
   lastColumn->HideControl ();

   SetWindowPos (0, Attr.X, Attr.Y, Attr.W, Attr.H, SWP_NOZORDER);
   CheckHScroll ();
   }


// addition suggested by David M. Larson. -------------------------------- //
void TGrid::ClearModify (TControl *c)
   {
   TGEdit *ed = dynamic_cast<TGEdit*>(c);
   TGComboBox *cb = dynamic_cast<TGComboBox *>(c);
   TGCheckBox *ck = dynamic_cast<TGCheckBox *>(c);

   if (ed)
      ed->ClearModify ();
   else if (ck)
      ck->ClearModify ();
   else if (cb)
      cb->ClearModify ();
   }


// ----------------------------------------------------------------------- //
// Locating elements (i.e. columns)                                        //
// ----------------------------------------------------------------------- //
TGrid::Column * TGrid::FindColumn (TPoint &p) const
   {
   for (Column *ctrl = firstColumn; ctrl; ctrl = ctrl->next)
      {
      if ( ctrl->Hidden )
        continue;

      if (TRect (ctrl->left, 0, ctrl->left+ctrl->width, clientHeight).Contains (p))
         return ctrl;
      }
   return 0;
   }


TGrid::Column *TGrid::FindColumn (TControl *c) const
   {
   for (Column *ctrl = firstColumn; ctrl; ctrl = ctrl->next)
      {
      if ( ctrl->Hidden )
        continue;

      if (ctrl->control == c)
         return ctrl;
      }

   return 0;
   }


TGrid::Column *TGrid::FindColumn (HWND hwnd) const
   {
   for (Column *ctrl = firstColumn; ctrl; ctrl = ctrl->next)
      {
      if ( ctrl->Hidden )
        continue;

      if (ctrl->control->HWindow == hwnd)
         return ctrl;
      }

   return 0;
   }


TGrid::Column *TGrid::FindColumn (int id) const
   {
   for (Column *ctrl = firstColumn; ctrl; ctrl = ctrl->next)
      {
      if ( ctrl->Hidden )
        continue;                         

      if (ctrl->control->GetId() == id)
         return ctrl;
      }

   return 0;
   }


int TGrid::GetDataLine (int y)
   {
   int y0 = 0;
   for (int i = 0; i < visibleLines; i++, y0 += lineHeight)
      {
      if (y >= y0 && y < y0+lineHeight)
         return i + firstLine; 
      }

   return 0;
   }


void TGrid::FindNearestUnlockedCell (TGrid::Column *e)
   {
   Column *prev = e->previous;
   Column *next = e->next;
   while (prev || next)
      {
      if (prev)
         {
         if (!prev->Hidden && prev->SetFocus ())         
            {
            current = prev;
            return;
            }
         prev = prev->previous;
         }
      if (next)
         {
         if (!next->Hidden && next->SetFocus ())         
            {
            current = next;
            return;
            }
         next = next->next;
         }
      }
   }


// ----------------------------------------------------------------------- //
// Line functions                                                          //
// ----------------------------------------------------------------------- //
void TGrid::SetLineHeight (int h)
   {
   if (h == 0)
      return;
   SetVisibleLines (clientHeight / (lineHeight=h+1));
   }


void TGrid::SetNLines (int n)
   {
   nLines = n;

   if (GetCurrentLine() >= n)
      Go (n-1);

   if (vscroll && vscroll->IsWindow())
      {
      if (maxLines < 0)
         vscroll->SetLRange (0, (nLines*10)/9+1, true);
      else
         vscroll->SetLRange (0, nLines-1, true);
      }
   }


void TGrid::SetVisibleLines (int nl)
   {
   visibleLines = nl;
   if (vscroll)
      vscroll->PageMagnitude = visibleLines;
   }


void TGrid::SetMaxLines (int m)
   {
   if (m < 0)
      nLines = maxLines;
   else
      nLines = m;
   if (nLines < 0)
      nLines = 0;

   if (GetCurrentLine() >= nLines)
      Go (nLines-1);

   maxLines = m;
   if (vscroll && vscroll->IsWindow())
      {
      if (maxLines < 0)
         vscroll->SetLRange (0, (nLines*10)/9+1, true);
      else
         vscroll->SetLRange (0, maxLines-1, true);
      }
   }


void TGrid::ResetPosition ()
   {
   Go (0);
   GotoFirstColumn ();
   }


void TGrid::SwapLines (int l1, int l2)
   {
   if (l1 == l2 || l1 < 0 || l2 < 0)
      return;
   if (maxLines >= 0 && (l1 >= maxLines || l2 >= maxLines))
      return;

   title->SetFocus ();

   for (Column *e = firstColumn; e; e = e->next)
      {
      int id = e->control->GetId ();
      Type colType = e->type;
      if (colType == gCombo && dynamic_cast<TGDataComboBox *>(e->control))
         {
         #ifdef __WIN32__
            colType = gLongEdit;
         #else
            colType = gShortEdit;
         #endif
         }
      switch (colType)
         {
         // strings (variable size)
         case gStatic:
         case gEdit:
         case gCombo:
         case gButton:           // ???
            {
            string t1 = static_cast<const char *>(TransferItem (l1, id));
            string t2 = static_cast<const char *>(TransferItem (l2, id));
            TransferItem (l1, id, t2.c_str());
            TransferItem (l2, id, t1.c_str());
            break;
            }

         // other types (fixed size)
         default:
            {
            int l = e->control->Transfer (0, tdSizeData);
            char *t1 = new char[l];
            char *t2 = new char[l];
            memcpy (t1, TransferItem (l1, id), l);
            memcpy (t2, TransferItem (l2, id), l);
            TransferItem (l1, id, t2);
            TransferItem (l2, id, t1);
            delete [] t1;
            delete [] t2;
            break;
            }
         }
      }

   int line = l1 - firstLine;
   if (line >= 0 && line < visibleLines)
      {
      int y = line*lineHeight;
      client->InvalidateRect (TRect (0, y, clientWidth, y+lineHeight), false);
      }

   line = l2 - firstLine;
   if (line >= 0 && line < visibleLines)
      {
      int y = line*lineHeight;
      client->InvalidateRect (TRect (0, y, clientWidth, y+lineHeight), false);
      }

   client->UpdateWindow ();

   if (current && !current->SetFocus ())
      FindNearestUnlockedCell (current);
   }


void TGrid::Move (int delta, bool showControl)
   {
   Go (GetCurrentLine()+delta, showControl);
   }


void TGrid::Go (int pos, bool showControl)
   {
   // checking pos ------------------------------------------------------- //
   if (pos >= nLines)
      {
      if (maxLines > 0)
         pos = nLines-1;
      else
         {
         // Ask permission to append; -------------------------------------//
         // AppendLine may change pos (passed by reference) ---------------//
         if (!AppendNewLine (pos))
            {
            vscroll->SetLRange (0, nLines-1, true);
            return;
            }

         SetNLines (pos+1);
         }
      }

   if (nLines == 0)
      return;

   if (pos < 0)
      pos = 0;

   // if no change, abort function ----------------------------------------//
   int oldCurrent = GetCurrentLine();
   if (pos == oldCurrent)
      return;

   // check current cell --------------------------------------------------//
   if (!CanKillFocus (current))
      return;

   // check data ----------------------------------------------------------//
   if (!ValidateLine (oldCurrent))
      return;

   // set focus to this, while changing current line ---------------------//
   if (title->SetFocus () == NULL)
      return;     // WM_KILLFOCUS failed

   // remove markers ------------------------------------------------------//
   if (vbar && vbar->IsWindow())
      vbar->ShowText (activeLine);

   // flush controls !!! --------------------------------------------------//
   for (Column *col = firstColumn; col; col = col->next)
      {
      if( col->Hidden )
         continue;                      

      TStatic *s = dynamic_cast<TStatic *>(col->control);
      if (s)
         s->SetText ("");
      else
         {
         TGCheckBox *c = dynamic_cast<TGCheckBox *>(col->control);
         if (c)
            {
            c->SetCheck (BF_GRAYED);
            c->ClearModify ();         /**/
            }
         else
            {
            TComboBox *c = dynamic_cast<TComboBox *>(col->control);
            if (c)
               {
               if ( (Attr.Style & CBS_DROPDOWNLIST) != CBS_DROPDOWNLIST)
                  c->SetSelIndex (-1);
               }
            else
               {
               TButton *b = dynamic_cast<TButton *>(col->control);
               if (b)
                  b->SetCaption ("");
               }
            }
         }
      }

   // compute new position ------------------------------------------------//
   int delta = pos - oldCurrent;
   int oldFirstLine = firstLine;
   if (delta + activeLine >= 0 && delta + activeLine < visibleLines)
      activeLine += delta;

   else
      {
      firstLine += delta;
      if (firstLine+visibleLines > nLines && maxLines > 0)
         firstLine = nLines-visibleLines;
      if (firstLine < 0)
         firstLine = 0;
      activeLine = pos - firstLine;
      if (activeLine < 0)
         activeLine = 0;
      else if (activeLine >= visibleLines)
         activeLine = visibleLines-1;
      }

   // redraw grid or scroll -----------------------------------------------//
   if (firstLine != oldFirstLine)
      {
      int scroll = oldFirstLine-firstLine;
      TRect clip (0, 0, clientWidth, clientHeight);  // ***
      if (scroll >= visibleLines || scroll <= -visibleLines)
         client->InvalidateRect (clip, false);
      else
         {
         int s = scroll*lineHeight;
         client->ScrollWindowEx (0, s, 0, &clip, 0,0, SW_INVALIDATE|SW_SCROLLCHILDREN);
         }
      client->UpdateWindow ();

      if (vbar && vbar->IsWindow())
         {
         TRect vbclip (0, 0, vbarWidth, clientHeight);
         vbar->InvalidateRect (vbclip, false);
         vbar->UpdateWindow ();
         }
      }

   // transfer data from memory -------------------------------------------//
   int newCurrent = GetCurrentLine ();

   // update scroller -----------------------------------------------------//
   if (vscroll && vscroll->IsWindow())
      vscroll->SetLPosition (newCurrent);

   // update vertical bar -------------------------------------------------//
   if (vbar && firstLine == oldFirstLine)
      vbar->ShowMarker (activeLine);

   // reset focus ---------------------------------------------------------//
   if (current)
      {
      if (showControl)
         {
         tempNoHMove = true;
         if (!current->SetFocus ())
            FindNearestUnlockedCell (current);
         tempNoHMove = false;
         }
      else if (current->type == gCombo && current->control)
         RefreshItem (oldCurrent, current->control->GetId ());
      }

   // Notify user ---------------------------------------------------------//
   if (firstLine != oldFirstLine)
      ScrollData (firstLine, firstLine+visibleLines-1);

   if (newCurrent != oldCurrent)
      {
      NewRowSelected (newCurrent, oldCurrent);
      if (lineSelection == SingleAutoSelect)
         {
         if (GetSelectedLine() != newCurrent)
            SelectLine (newCurrent);
         }
      }

   if (vscroll && staticOnly && vscroll->IsWindow())
      Parent->PostMessage (WM_NEXTDLGCTL, WPARAM(vscroll->HWindow), 1);
   }


// ----------------------------------------------------------------------- //
//                                                                         //
// Line selection functions                                                //
//                                                                         //
// enhanced by Dean Crawford                                               //
//                                                                         //
// ----------------------------------------------------------------------- //
bool TGrid::SelectLine (int line, uint modKeys)
   {
   // Get keys state
   bool shifted = (modKeys & MK_SHIFT)   ? true : false;
   bool cntrled = (modKeys & MK_CONTROL) ? true : false;
   
   if (lineSelection == Single || lineSelection == SingleAutoSelect)
      {
      // selection outside range of valid lines
      if (line >= 0 && line >= nLines)
         return false;

      // unselecting currently selected line
      else if (line == selectedLine && line != -1)
         {
         int old = line;
         if (lineSelection != SingleAutoSelect)
            {
            selectedLine = -1;
            if (old >= 0)
               {
               RefreshLine (line);
               LineSelected (old, false);
               }
            }
         }

      // selecting a new line and unselecting the old one
      else
         {
         int old = selectedLine;
         selectedLine = -1;
         RefreshLine (old);
         if (old >= 0)
            LineSelected (old, false);

         selectedLine = line;
         RefreshLine (line);
         if (line >= 0)
            LineSelected (line, true);
         }

      return true;
      }
   else if (lineSelection == Multiple)
      {
      if (shifted && selectRow != -1)
         {
         int lower = min(selectRow+1, line);
         int upper = max(selectRow-1, line);
         for (int srows = lower; srows <= upper; srows++)
            {
            if (selectedLines->Read (srows) == false)
               {
               nsel++;
               selectedLines->Toggle (srows);
               RefreshLine (srows);
               LineSelected (srows, true);
               }
            }
         }
      else
         {
         if (!cntrled)
            UnselectAllLines ();
         bool s = selectedLines->Read (line);
         if (s)
            nsel--;
         else
            nsel++;
         selectedLines->Toggle (line);
         RefreshLine (line);
         LineSelected (line, s);
         }

      selectRow = line;
      return true;
      }

   return false;
   }


bool TGrid::LineSelected (int line) const
   {
   if (selectedLine >= 0 && line == selectedLine)
      return true;
   else if (selectedLines)
      return selectedLines->Read (line);
   else
      return false;
   }


int TGrid::CountSelectedLines () const
   {
   if (selectedLine >= 0)
      return 1;
   else
      return nsel;
   }


int TGrid::GetSelectedLine ()   const
   {
   if (lineSelection != None)
      {
      if (selectedLine >= 0)
         return selectedLine;
      else if (selectedLines)
         {
         ulong n = selectedLines->FindFirst();
         if (n == bitNoFind)
            return -1;
         else
            return int(n);
         }
      }

   return -1;
   }


int TGrid::GetSelectedLines (int *list) const
   {
   if (list == 0)
      return 0;

   if (lineSelection == Single || lineSelection == SingleAutoSelect)
      {
      if (selectedLine >= 0)
         {
         list[0] = selectedLine;
         return 1;
         }
      }
   else if (lineSelection == Multiple)
      {
      int index = 0;
      for (ulong n = selectedLines->FindFirst(); index < nsel && n != bitNoFind; n = selectedLines->FindNext())
         list[index++] = int(n);
      return nsel;
      }

   return 0;
   }


int TGrid::GetSelectedLines (TBitField &bf) const
   {
   if (lineSelection == Single || lineSelection == SingleAutoSelect)
      {
      bf.ClearAll();
      if (selectedLine >= 0)
         {
         bf.Set (selectedLine);
         return 1;
         }
      }
   else if (lineSelection == Multiple)
      {
      bf = *selectedLines;
      return nsel;
      }

   return 0;
   }


void TGrid::UnselectAllLines ()
   {
   if (lineSelection == Single || lineSelection == SingleAutoSelect)
      {
      if (selectedLine >= 0)
         {
         int old = selectedLine;
         selectedLine = -1;
         RefreshLine (old);
         LineSelected (old, false);
         }
      }
   else if (lineSelection == Multiple)
      {
      nsel = 0;
      for (ulong n = selectedLines->FindFirst(); n != bitNoFind; n = selectedLines->FindNext())
         {
         selectedLines->Clear (n);
         RefreshLine (int(n));
         LineSelected (int(n), false);
         }
      selectRow = -1;
      }
   }


bool TGrid::ValidateLine (int line)
   {
   // retrieve data from current control
   TControl *c = current ? current->control : 0;
   if (c)
      {
      // check if data has been touched
      TGEdit *ed = dynamic_cast<TGEdit *>(c);
      TGComboBox *cb = dynamic_cast<TGComboBox *>(c);
      TGCheckBox *ck = dynamic_cast<TGCheckBox *>(c);
      bool touched = false;
      if (ed)
         touched = ed->IsModified ();
      else if (ck)
         touched = ck->IsModified();
      else if (cb)
         touched = cb->IsModified ();

      if (touched)
         {
         void *data = 0;
         if (current->type == gCombo)
            data = dynamic_cast<TGComboBox *>(c)->GetData ();
         else
            {
            int l = c->Transfer (0, tdSizeData);
            if (l > 0)
               {
               data = new char[l];
               c->Transfer (data, tdGetData);
               }
            }

         int id = c->GetId ();
         bool rc = DataChanging (line, id, data);
         if (rc)
            {
            modified = true;
            TransferItem (line, id, data);
            DataChanged (line, id, data);
            ClearModify (c);
            }

         if (data)
            delete [] data;
         if (!rc)
            return false;
         }
      }

   // now, validate line
   return IsValidLine (line);
   }

// ----------------------------------------------------------------------- //
// Keyboard response                                                       //
// ----------------------------------------------------------------------- //
bool TGrid::KeyDown (uint key, int repeat)
   {
   bool ctrl  = (GetKeyState (VK_CONTROL) & 0x8000) ? true : false;
   bool shift = (GetKeyState (VK_SHIFT)   & 0x8000) ? true : false;

   switch (key)
      {
      case VK_ESCAPE:   return VkEscape (ctrl, shift, repeat);
      case VK_INSERT:   return VkInsert (ctrl, shift, repeat);
      case VK_DELETE:   return VkDelete (ctrl, shift, repeat);
      case VK_PRIOR:    return VkPrior  (ctrl, shift, repeat);
      case VK_NEXT:     return VkNext   (ctrl, shift, repeat);
      case VK_UP:       return VkUp     (ctrl, shift, repeat);
      case VK_DOWN:     return VkDown   (ctrl, shift, repeat);
      case VK_RIGHT:    return VkRight  (ctrl, shift, repeat);
      case VK_LEFT:     return VkLeft   (ctrl, shift, repeat);
      case VK_RETURN:   return VkReturn (ctrl, shift, repeat);
      case VK_TAB:      return VkTab    (ctrl, shift, repeat);
      case VK_HOME:     return VkHome   (ctrl, shift, repeat);
      case VK_END:      return VkEnd    (ctrl, shift, repeat);
      }

   return false;
   }


bool TGrid::VkEscape (bool ctrl, bool shift, int)
   {
   if (ctrl == false && shift == false)
      {
      Parent->Destroy (IDCANCEL);
      return true;
      }

   return false;
   }


bool TGrid::VkEnd (bool ctrl, bool, int)
   {
   if (ctrl)
      {
      if (!CanKillFocus (current))
         return true;

      GotoLastColumn ();
      return true;
      }

   return false;
   }


bool TGrid::VkHome (bool ctrl, bool, int)
   {
   if (ctrl)
      {
      if (!CanKillFocus (current))
         return true;

      GotoFirstColumn ();
      return true;
      }

   return false;
   }


bool TGrid::VkRight (bool, bool, int repeat)
   {
   for (int i = 0; i < repeat; i++)
      if (!GotoNextColumn ())
         break;
   return true;
   }


bool TGrid::VkLeft (bool, bool, int repeat)
   {
   for (int i = 0; i < repeat; i++)
      if (!GotoPreviousColumn ())
         break;
   return true;
   }


bool TGrid::VkUp (bool ctrl, bool, int repeat)
   {
   if (ctrl)
      {
      if (lineSwapping == false)
         return false;
      for (int i = 0; i < repeat; i++)
         {
         int l = firstLine+activeLine;
         if (l <= 0)
            break;
         SwapLines (l-1, l);
         Move (-1);
         }
      }
   else
      Move (-repeat);
   return true;
   }


bool TGrid::VkDown (bool ctrl, bool, int repeat)
   {
   if (ctrl)
      {
      if (lineSwapping == false)
         return false;
      for (int i = 0; i < repeat; i++)
         {
         int l = firstLine+activeLine;
         if (l >= nLines-1)
            break;
         SwapLines (l, l+1);
         Move (1);
         }
      }
   else
      Move (repeat);
   return true;
   }


bool TGrid::VkNext (bool ctrl, bool, int repeat)
   {
   if (ctrl)
      {
      Go (nLines-visibleLines-1, false);
      Go (nLines-1);
      }
   else
      Move (visibleLines*repeat);
   return true;
   }


bool TGrid::VkPrior (bool ctrl, bool, int repeat)
   {
   if (ctrl)
      Go (0);
   else
      Move (-visibleLines*repeat);
   return true;
   }


bool TGrid::VkReturn (bool ctrl, bool shift, int repeat)
   {
   if (ctrl)
      return false;

   else if (shift && lineSelection != None)
      return SelectLine (GetCurrentLine ());

   switch (enterKey)
      {
      case NextLine:
         {
         if (!CanKillFocus (current))
            return true;

         Move (repeat, false);
         GotoFirstColumn ();
         return true;
         }

      case DoNothing:
         return true;

      case EndDialog:
         return false;

      case EnterNextColumn:
         {
         if (!CanKillFocus (current))
            return true;

         for (int i = 0; i < repeat; i++)
            {
            if (!GotoNextColumn ())
               {
               Move (1, false);
               GotoFirstColumn ();
               }
            }
         return true;
         }
      }
   return false;
   }


bool TGrid::VkTab (bool ctrl, bool shift, int repeat)
   {
   if (ctrl || tabKey == NextControl)
      {
      if (ValidateLine(activeLine+firstLine))
         {
         HWND hwnd = Parent->GetNextDlgTabItem (Parent->HWindow, shift);
         if (hwnd)
            {
            if (current)
               current->HideControl ();
            Parent->PostMessage (WM_NEXTDLGCTL, shift);
            }
         }
      return true;
      }
   else
      {
      if (!CanKillFocus (current))
         return true;

      if (tabKey == AutoNextLine)
         {
         if (shift)
            {
            for (int i = 0; i < repeat; i++)
               {
               if (!GotoPreviousColumn ())
                  {
                  Move (-1, false);
                  GotoLastColumn ();
                  }
               }
            }
         else
            {
            for (int i = 0; i < repeat; i++)
               {
               if (!GotoNextColumn ())
                  {
                  Move (1, false);
                  GotoFirstColumn ();
                  }
               }
            }
         return true;
         }
      else
         {
         if (shift)
            return VkLeft (ctrl, shift, repeat);
         else
            return VkRight (ctrl, shift, repeat);
         }
      }
   }


bool TGrid::VkDelete (bool ctrl, bool, int)
   {
   if (ctrl)
      {
      title->SetFocus ();
      if (DeleteLine (activeLine+firstLine) && activeLine >= 0 && activeLine < visibleLines)
         {
         // optimization made by René Mayer
         int y = activeLine*lineHeight;
         client->InvalidateRect (TRect (0, y, clientWidth, clientHeight), false);
         client->UpdateWindow ();
         // end of optimization
         }
      if (current && !current->SetFocus ())
         FindNearestUnlockedCell (current);
      return true;
      }
   return false;
   }


bool TGrid::VkInsert (bool ctrl, bool, int repeat)
   {
   if (ctrl)
      {
      if (!CanKillFocus (current) || !ValidateLine (activeLine+firstLine))
         return true;
      title->SetFocus ();
      for (int i = 0; i < repeat; i++)
         {
         if (!InsertLine (activeLine+firstLine))
            return true;
         }
      // optimization made by René Mayer
      if (activeLine >= 0 && activeLine < visibleLines)
         {
         int y = activeLine*lineHeight;
         client->InvalidateRect (TRect (0, y, clientWidth, clientHeight), false);
         client->UpdateWindow ();
         }
      // end of optimization
      if (current && !current->SetFocus ())
         FindNearestUnlockedCell (current);
      return true;
      }
   return false;
   }


// ----------------------------------------------------------------------- //
// Handling columns                                                        //
// ----------------------------------------------------------------------- //
bool TGrid::GotoPreviousColumn ()
   {
   Column *e = current;
   if (e == 0)
      e = lastColumn;
   else
      e = e->previous;
   while (e && e->Hidden)      
      e = e->previous;
   if (e == 0)
      return false;
   for (; e; e = e->previous)
      {
      if (e->SetFocus ())
         return true;
      }
   if (vscroll && staticOnly && vscroll->IsWindow())
      Parent->PostMessage (WM_NEXTDLGCTL, WPARAM(vscroll->HWindow), 1);
   else
      GotoFirstColumn ();
   return false;
   }


bool TGrid::GotoNextColumn ()
   {
   Column *e = current;
   if (e == 0)
      e = firstColumn;
   else
      e = e->next;
   while (e && e->Hidden)      
      e = e->next;
   if (e == 0)
      return false;
   for (; e; e = e->next)
      {
      if (e->SetFocus ())
         return true;
      }
   if (vscroll && staticOnly && vscroll->IsWindow())
      Parent->PostMessage (WM_NEXTDLGCTL, WPARAM(vscroll->HWindow), 1);
   else
      GotoLastColumn ();
   return false;
   }


void TGrid::GotoLastColumn ()
   {
   if (current)
      current->HideControl ();
   current = lastColumn;
   if (current && !current->SetFocus ())
      GotoPreviousColumn ();
   }


void TGrid::GotoFirstColumn ()
   {
   if (current)
      current->HideControl ();
   current = firstColumn;
   if (current && !current->SetFocus ())
      GotoNextColumn ();
   }


void TGrid::SetHPos (int newpos)
   {
   MoveHPos (newpos - hpos);
   }


void TGrid::MoveHPos (int delta)
   {
   int pos = hpos + delta;
   if (pos < 0)
      pos = 0;
   else if (clientWidth >= logicalWidth)
      pos = 0;
   else if (lastColumn->left+lastColumn->width-delta < clientWidth)
      pos = logicalWidth-clientWidth;
   delta = pos - hpos;

   for (Column *ctrl = firstColumn; ctrl; ctrl = ctrl->next)
      {
      if (ctrl->Hidden)                                          
        continue;

      ctrl->left -= delta;
      }

   TRect clip (0, 0, clientWidth, clientHeight);
   client->ScrollWindowEx (-delta, 0, 0, &clip, 0,0, SW_INVALIDATE|SW_SCROLLCHILDREN);
   client->UpdateWindow ();

   if (title && title->IsWindow() && titleHeight > 0)
      {
      title->Invalidate ();
      title->UpdateWindow ();
      }

   if (hscroll && hscroll->IsWindow())
      hscroll->SetPosition (hpos = pos);
   }


void TGrid::SetColumnWidth (int id, int width)
   {
   Column *e = FindColumn (id);
   if (e)
      {
      int delta = width - e->width;
      if (delta)
         ChangeColWidth (e, delta);
      }
   }


void TGrid::ChangeColWidth (Column *e, int delta, int minw)
   {
   if (e)
      {
      if (e->Hidden)                         
         return;                             

      if (delta < 0 && delta < -(e->width-minw)) // min width is 8 pixels
         delta = -(e->width-minw);

      e->width += delta;
      for (Column *col = e->next; col; col = col->next)
         col->left += delta;
      logicalWidth += delta;

      CheckHScroll ();

      if (delta && IsWindow ())
         {
         int x = e->left+e->width;
         int lx = lastColumn->left+lastColumn->width;
         bool updateRight = true;
         if (lx < clientWidth)
            {
            int delta = clientWidth-lx;
            int pos = hpos + delta;
            if (pos < 0)
               pos = 0;
            else if (clientWidth >= logicalWidth)
               pos = 0;
            else if (lastColumn->left+lastColumn->width-delta < clientWidth)
               pos = logicalWidth-clientWidth;
            delta = pos - hpos;

            if (delta)
               {
               for (Column *ctrl = firstColumn; ctrl; ctrl = ctrl->next)
                  ctrl->left -= delta;

               if (title && title->IsWindow() && titleHeight > 0)
                  title->InvalidateRect (TRect(0,0,e->left+e->width,titleHeight), false);

               if (hscroll && hscroll->IsWindow())
                  hscroll->SetPosition (hpos = pos);

               TRect clip (0, 0, e->left, clientHeight);
               client->ScrollWindowEx (-delta, 0, 0, &clip, 0,0, SW_INVALIDATE|SW_SCROLLCHILDREN);
               updateRight = false;
               }
            }
         if (updateRight)
            {
            if (title && title->IsWindow() && titleHeight > 0)
               title->InvalidateRect (TRect (e->left, 0, clientWidth, titleHeight), false);
            TRect clip (x, 0, clientWidth, clientHeight);
            client->ScrollWindowEx (delta, 0, 0, &clip, 0,0, SW_INVALIDATE|SW_SCROLLCHILDREN);
            }
         }

      client->InvalidateRect (TRect (e->left, 0, e->left+e->width, clientHeight), false);
      if (title && title->IsWindow() && titleHeight > 0)
         title->UpdateWindow ();
      client->UpdateWindow ();

      bool vs = (vscroll && vscroll->IsWindow () && !vscrollHidden) ? true : false;
      if (horzShrink && (logicalWidth < clientWidth || (clientWidth >= logicalWidth && Attr.W > vbarWidth+clientWidth+cxvscroll*vs)))
         {
         ChangeWindowPos (Attr.W, Attr.H, SWP_NOMOVE);
         InvalidateRect (TRect (vbarWidth+clientWidth-2, 0, Attr.W, Attr.H), false);
         UpdateWindow ();
         }
      }
   }


void TGrid::Remove (int id, bool destroy)
   {
   Column *e = FindColumn (id);
   if (e)
      Remove (e, destroy);
   }


void TGrid::Remove (Column *e, bool destroy)
   {
   if (e == 0)
      return;

   if ( ! e->Hidden )                
      {
      int w = e->width;
      for (Column *col = e->next; col; col = col->next)
         col->left -= w;
      logicalWidth -= w;

      CheckHScroll ();
      client->InvalidateRect (TRect (e->left, 0, clientWidth, clientHeight), false);
      client->UpdateWindow ();

      if (title && title->IsWindow() && titleHeight > 0)
         {
         title->InvalidateRect (TRect (e->left, 0, clientWidth, titleHeight), false);
         title->UpdateWindow ();
         }

      if (current == e)
         {
         if (e->next)
            current = e->next;
         else if (e->previous)
            current = e->previous;
         else
            current = 0;
         }

      if (e->next)
         e->next->previous = e->previous;
      if (e->previous)
         e->previous->next = e->next;
      if (lastColumn == e)
         lastColumn = e->previous;
      if (firstColumn == e)
         firstColumn = e->next;
      }

   if (destroy)
      delete e->control;

   if (e->type != gStatic)
      {
      staticOnly = true;
      for (Column *c = firstColumn; c; c = c->next)
         if (e->type != gStatic)
            {
            staticOnly = false;
            break;
            }
      }

   e->next = e->previous = 0;
   delete e;
   }


// This section was suggested by Charles R. Cordell
void TGrid::HideColumn(int id)
   {
   Column *e = FindColumn (id);
   if ( e )
      {
      if (current == e)
         FindNearestUnlockedCell (e);     // set current to closest column
      int w = e->width;
      ChangeColWidth (e, -w, 0);
      e->Hidden = w;
      }
   }


void TGrid::UnHideColumn(int id)
   {
   Column *e;
   for (e = firstColumn; e; e = e->next)
      if ( id == e->control->GetId() )
         break;

   if (e && e->Hidden)
     {
     int w = e->Hidden;
     e->Hidden = 0;
     ChangeColWidth (e, w, 0);
     }
   }
// end of section


void TGrid::CheckHScroll ()
   {
   // check if horiz scroll has to be shown/hidden --------------------------
   if (hscroll && hscroll->IsWindow())
      {
      bool scrollable = ((logicalWidth > clientWidth) ? true : false);
      if (hideHScroll)
         {
         bool bVisible = !hscrollHidden;
         if ((bVisible ^ scrollable) != 0)
            SetWindowPos(0,0,0,Attr.W,Attr.H,SWP_NOZORDER|SWP_NOMOVE);  
         }
      else if (disableHScroll)
         {
         bool enabled = (hscroll->IsWindowEnabled () ? true : false);
         if ((enabled ^ scrollable) != 0)
            hscroll->EnableWindow (logicalWidth > clientWidth);
         }
      }

   SetHScrollRange ();
   }


// ----------------------------------------------------------------------- //
// Colors                                                                  //
// ----------------------------------------------------------------------- //
HBRUSH TGrid::CtlColor(HDC hDC, HWND hWndChild, uint ctlType)
   {
   Column *e = FindColumn (hWndChild);
   if (e)
      {
      TWindow *wptr = e->control;
      TColor bg, fg;
      HBRUSH brush;

      if (dynamic_cast<TGButton *>(wptr) != 0 && LineSelected (GetCurrentLine()))
         {
         static HBRUSH brush = 0;
         TDC dc(hDC);
         dc.SetTextColor (hilitetext);
         dc.SetBkColor (hilite);
         brush = TBrush (hilite);
         return brush;
         }

      bool customColors = CellColor (GetCurrentLine(), wptr->GetId(), true, bg, fg, brush);
      if (customColors)
         {
         TDC dc(hDC);
         dc.SetTextColor (fg);
         dc.SetBkColor (bg);
         return brush;
         }

      if (dynamic_cast<TGEdit *>(wptr))
         return dynamic_cast<TGEdit *>(wptr)->CtlColors (hDC);

      if (dynamic_cast<TGComboBox *>(wptr))
         return dynamic_cast<TGComboBox *>(wptr)->CtlColors (hDC);

      if (dynamic_cast<TGStatic *>(wptr))
         return dynamic_cast<TGStatic *>(wptr)->CtlColors (hDC);

      if (dynamic_cast<TGCheckBox *>(wptr))
         return dynamic_cast<TGCheckBox *>(wptr)->CtlColors (hDC);

      TApplication *app = GetApplication ();
      if (app->BWCCEnabled () || app->Ctl3dEnabled())
         {
         char buffer[32];
         ::GetClassName (hWndChild, buffer, 32);
         if (stricmp (buffer, "Static") == 0)
            {
            TDC(hDC).SetBkColor (GetSysColor (COLOR_BTNFACE));
            return HBRUSH(*buttonFace);
            }
         }

      if (app->Ctl3dEnabled())
         {
         HBRUSH WINAPI(*CtlColorEx)(uint, WPARAM, LPARAM);
         (FARPROC)CtlColorEx = app->GetCtl3dModule()->GetProcAddress("Ctl3dCtlColorEx");
         HBRUSH hb = 0;
         if (CtlColorEx)
         #if defined(__WIN32__)
           hb = CtlColorEx(WM_CTLCOLORMSGBOX+ctlType, WPARAM(hDC), LPARAM(hWndChild));
         #else
           hb = CtlColorEx(WM_CTLCOLOR, WPARAM(hDC), MAKELPARAM(hWndChild, ctlType));
         #endif

         if (hb != 0)
            return hb;
         }
      }

   return TWindow::EvCtlColor(hDC, hWndChild, ctlType);
   }


// ----------------------------------------------------------------------- //
// Switching focus                                                         //
// ----------------------------------------------------------------------- //
void TGrid::EvSetFocus (HWND)
   {
   if (current == 0)
      GotoFirstColumn ();
   else
      current->SetFocus ();

   RefreshLine (GetCurrentLine ());

   if (vscroll && staticOnly && vscroll->IsWindow())
      Parent->PostMessage (WM_NEXTDLGCTL, WPARAM(vscroll->HWindow), 1);
   }


void TGrid::EvKillFocus (HWND hWndGetFocus)
   {
   if (current && CanKillFocus (current) && current->control->HWindow != hWndGetFocus)
      current->HideControl ();
   }


void TGrid::SetFocusOnChild (TControl *child, HWND /*loosefocus*/)
   {
   Column *e = FindColumn (child);
   if (e && !e->Hidden)          
      {
      int delta = 0;

      if (e->left < 0)
         delta = e->left;
      else if (e->left+e->width > clientWidth)
         delta = e->left+e->width - clientWidth;
      if (delta && !tempNoHMove)
         MoveHPos (delta);

      int id = child->GetId ();
      int line = GetCurrentLine ();
      void *data = const_cast<void *>(TransferItem (line, id));
      if (data)               // fix made by René Mayer ------------------------
         {
         if (e->type == gCombo)
            dynamic_cast<TGComboBox *>(child)->SetData (data);
         // fix made by René Mayer ---------------------------------------------
         else if (e->type == gButton)
            dynamic_cast<TGButton *>(child)->SetCaption ((char *)data);
         // end of fix ------------------------------------------------------------
         else
            child->Transfer (data, tdSetData);
         }
      e->ShowControl ();
      }
   }


void TGrid::KillFocusFromChild (TControl *c, HWND /*getfocus*/, bool okToChange)
   {
   if (current)
      {
      Column *ctrl = FindColumn (c);
      if (ctrl)
         {
         void *data = 0;
         if (ctrl->type == gCombo)
            data = dynamic_cast<TGComboBox *>(ctrl->control)->GetData ();
         else
            {
            int l = ctrl->control->Transfer (0, tdSizeData);
            if (l > 0)
               {
               data = new char[l];
               ctrl->control->Transfer (data, tdGetData);
               }
            }
         if (okToChange)
            {
            modified = true;
            int l = GetCurrentLine ();
            int id = c->GetId ();
            TransferItem (l, id, data);
            DataChanged (l, id, data);
            ClearModify (c);
            }
         if (data)
            delete [] data;
         }
      current->HideControl ();
      client->UpdateWindow ();
      }
   }


bool TGrid::CanKillFocus (Column *e)
   {
   if (e)
      return e->CanKillFocus ();
   else
      return true;
   }

// ----------------------------------------------------------------------- //
// Painting                                                                //
// ----------------------------------------------------------------------- //
bool TGrid::EvEraseBkgnd (HDC)
   {
   bool hs = (hscroll && hscroll->IsWindow () && !vscrollHidden) ? true : false;
   bool vs = (vscroll && vscroll->IsWindow () && !hscrollHidden) ? true : false;

   // upper right corner --------------------------------------------------//
   if (cxvscroll && titleHeight && vs)
      InvalidateRect (TRect (TPoint(vbarWidth+clientWidth, 0), TSize(cxvscroll, titleHeight)), false);

   // lower left corner ---------------------------------------------------//
   if (vbarWidth && cyhscroll && hs)
      InvalidateRect (TRect (TPoint(0, titleHeight+clientHeight), TSize(vbarWidth, cyhscroll)), false);

   // lower right corner --------------------------------------------------//
   if (cxvscroll && cyhscroll && vs && hs)
      InvalidateRect (TRect (TPoint(vbarWidth+clientWidth, titleHeight+clientHeight), TSize(cxvscroll, cyhscroll)), false);

   UpdateWindow ();

   return true;
   }


void TGrid::EvNCPaint ()
   {
   }

void TGrid::EvPaint ()
   {
   TPaintDC dc(*this);
   Paint(dc, dc.Ps.fErase, *(TRect*)&dc.Ps.rcPaint);
   }

void TGrid::Paint (TDC &dc, bool, TRect &)
   {
   bool hs = (hscroll && hscroll->IsWindow () && !hscrollHidden) ? true : false;
   bool vs = (vscroll && vscroll->IsWindow () && !vscrollHidden) ? true : false;

   int cx = cxvscroll*vs;
   int cy = cyhscroll*hs;

   dc.FrameRect (TRect (0,0,vbarWidth+clientWidth+cx,titleHeight+clientHeight+cy), TBrush(TColor(GetTitleColor (TitleFrame))));

   // upper left corner -------------------------------------------------- //
   if (vbarWidth && titleHeight)
      DrawCorner (dc, TRect (TPoint(1, 1), TSize(vbarWidth-2, titleHeight-1)));

   // upper right corner --------------------------------------------------//
   if (cx && titleHeight)
      DrawCorner (dc, TRect (TPoint(vbarWidth+clientWidth-1, 1), TSize(cxvscroll, titleHeight-1)));

   // lower left corner ---------------------------------------------------//
   if (vbarWidth && cy)
      DrawCorner (dc, TRect (TPoint(1, titleHeight+clientHeight), TSize(vbarWidth-2, cyhscroll)));

   // lower right corner --------------------------------------------------//
   if (cx && cy)
      DrawCorner (dc, TRect (TPoint(vbarWidth+clientWidth-1, titleHeight+clientHeight), TSize(cxvscroll, cyhscroll)));

   TPen pen (GetTitleColor (TitleFrame));
   dc.SelectObject (pen);
   if (vbarWidth)
      {
      dc.MoveTo (vbarWidth-1, 0);
      dc.LineTo (vbarWidth-1, titleHeight+clientHeight+cy);
      }
   if (cx)
      {
      dc.MoveTo (vbarWidth+clientWidth-2, 0);
      dc.LineTo (vbarWidth+clientWidth-2, titleHeight+clientHeight+cy);
      }
   if (titleHeight)
      {
      dc.MoveTo (0, titleHeight);
      dc.LineTo (vbarWidth+clientWidth+cx, titleHeight);
      }
   if (cy)
      {
      dc.MoveTo (0, titleHeight+clientHeight-1);
      dc.LineTo (vbarWidth+clientWidth+cx, titleHeight+clientHeight-1);
      }
   dc.RestorePen ();

   dc.SaveDC ();
   TRegion rgn (GetClientRect());
   TRegion excl (TRect(0, 0, vbarWidth+clientWidth+cx, titleHeight+clientHeight+cy));
   rgn -= excl;
   TGDialog *p = dynamic_cast<TGDialog *>(Parent);
   TBrush *hbr;
   if (p)
      hbr = new TBrush(p->BkgBrush());
   else
      hbr = new TBrush(TColor(GetSysColor(COLOR_BTNFACE)));
   dc.SelectClipRgn (rgn);
   dc.FillRect (GetClientRect(), *hbr);
   delete hbr;
   dc.RestoreDC ();
   }


void TGrid::DrawCorner (TDC &dc, TRect &rc)
   {
   dc.FillRect (rc, TBrush (GetTitleColor (TitleBackground)));
   Frame3d (dc, rc, 1, GetTitleColor (TitleHilite), GetTitleColor (TitleShadow));
   }


void TGrid::PaintClient (TDC &dc, bool, TRect &clip)
   {
   int i;
   // clip drawing ----------------------------------------------------------
   dc.SelectClipRgn (TRegion(clip));

   // No column to paint ? --------------------------------------------------
   if (firstColumn == 0)
      {
      dc.FillRect (clip, TBrush (emptyGrid));
      return;
      }

   // draw lines ------------------------------------------------------------
   int y = 0;

   for (i = 0; i < visibleLines; i++, y += lineHeight)
      {
      int pos = firstLine+i;
      if (maxLines < 0 && pos >= nLines)
         {
         if (!AppendNewLine (pos))
            {
            TRect rc (0, y, clientWidth, clientHeight);
            dc.FillRect (rc, TBrush (emptyGrid));
            vscroll->SetLRange (0, nLines-1, true);
            break;
            }
         SetNLines (pos+1);
         }

      TRect currect (0, y, clientWidth-1, y+lineHeight-1);
      if (!currect.Touches (clip))
         continue;

      // text ---------------------------------------------------------------
      for (Column *ctrl = firstColumn; ctrl; ctrl = ctrl->next)
         {
         if( ctrl->Hidden )
           continue;                        

         // Select font -----------------------------------------------------
         if (ctrl->font)
            dc.SelectObject (*ctrl->font);
         else
            dc.SelectObject (*gridFont);

         // do not draw elements scrolled out horizontally ------------------
         if (ctrl->left + ctrl->width <= 0)
            continue;
         if (ctrl->left >= clientWidth)
            continue;

         // drawing area ----------------------------------------------------
         TRect rc (ctrl->left, y, ctrl->left+ctrl->width-1, y+lineHeight-1);
         if (rc.Touches (clip))
            {
            TControl *c = ctrl->control;
            const void *data;
            Visible v;
            if (maxLines > 0 && pos >= maxLines)
               {
               data = 0;
               v = gEmpty;
               }
            else
               {
               data = TransferItem (i+firstLine, c->GetId ());
               v = VisibleCell (i+firstLine, ctrl->control->GetId());
               }
            if (v == gEmpty)
               dc.FillRect (rc, TBrush (emptyGrid));
            else
               {
               switch (ctrl->type)
                  {
                  case gTimeEdit:
                  case gDateEdit:
                  case gShortEdit:
                  case gLongEdit:
                  case gDoubleEdit:
                  case gEdit:
                     dynamic_cast<TGEdit *>(c)->DrawCell (dc, rc, data, ctrl->align);
                     break;

                  case gStatic:
                     dynamic_cast<TGStatic *>(c)->DrawCell (dc, rc, data, ctrl->align);
                     break;

                  case gCombo:
                     dynamic_cast<TGComboBox *>(c)->DrawCell (dc, rc, data);
                     break;

                  case gCheck:
                     dynamic_cast<TGCheckBox *>(c)->DrawCell (dc, rc, data, ctrl->align);
                     break;

                  case gButton:
                     dynamic_cast<TGButton *>(c)->DrawCell (dc, rc, data);
                     break;
                  }
               }
            }

         dc.RestoreFont ();
         }
      }

	// draw grid -------------------------------------------------------------
   // this section was enhanced by Dean Crawford ----------------------------
   int vh = visibleLines*lineHeight;
   if (logicalWidth <= clientWidth)
      {
      TRect rc (logicalWidth, 0, clientWidth, clientHeight);
      dc.FillRect (rc, TBrush(emptyGrid));
	   dc.SelectObject (TPen (GetSysColor (COLOR_WINDOWFRAME)));
      int x = logicalWidth;
      dc.MoveTo (x, 0);
      dc.LineTo (x, vh);
      dc.RestorePen ();
      }
   clip.Inflate (1);
	dc.SelectClipRgn (TRegion(clip));
	dc.SelectObject (TPen (lineColor));
   y = lineHeight-1;

   if (vh < clientHeight)
      {
      TRect rc (0, vh-1, clientWidth, clientHeight);
      dc.FillRect (rc, TBrush (emptyGrid));
      }

   for (i = 0; i <= visibleLines; i++, y += lineHeight)
      {
      dc.MoveTo (0, y);
      dc.LineTo (clientWidth, y);
      }

   for (Column *ctrl = firstColumn; ctrl; ctrl = ctrl->next)
      {
      if ( ctrl->Hidden )
         continue;                       

      int x = ctrl->left-1;
      if (x >= 0 && x < clientWidth)
         {
         dc.MoveTo (x, 0);
         dc.LineTo (x, vh);
         }
      }

   dc.RestorePen ();
   }


void TGrid::RefreshLine (int line)
   {
   if (line < firstLine || line >= firstLine+visibleLines)
      return;

   int gridline = line - firstLine;
   int y = lineHeight * gridline;
   TRect clip (0, y, clientWidth, y+lineHeight);
   client->InvalidateRect (clip, false);
   client->UpdateWindow ();

   // update vbar -----------------------------------------------------------
   RefreshItem (line, 0);
   }


void TGrid::RefreshItem (int line, int id)
   {
   if (line < firstLine || line >= firstLine+visibleLines)
      return;

   int gridline = line - firstLine;

   if (gridline == activeLine)
      {
      Column *e = FindColumn (id);
      if (e)
         {
         void *data = const_cast<void *>(TransferItem (line, id));
         if (e->type == gCombo)
            dynamic_cast<TGComboBox *>(e->control)->SetData (data);
         else
            e->control->Transfer (data, tdSetData);
         }
      }

   int y = lineHeight * gridline;
   if (id != 0)   // update client area
      {
      Column *e = FindColumn (id);
      if (e)
         {
         TRect clip (e->left, y, e->left+e->width, y+lineHeight);
         if (clip.right < 0 || clip.left >= clientWidth)
            return;
         if (clip.left < 0)
            clip.left = 0;
         if (clip.right >= clientWidth-1)
            clip.right = clientWidth;
         if (!clip.IsEmpty ())
            {
            client->InvalidateRect (clip, true);
            client->UpdateWindow ();
            }
         }
      }
   else if (vbar && vbar->IsWindow())  // update left vertical bar
      {
      TRect clip (0, y, vbarWidth, y+lineHeight);
      vbar->InvalidateRect (clip, false);
      vbar->UpdateWindow ();
      }
   }


void TGrid::Refresh ()
   {
   for (int i = 0; i < visibleLines; i++)
      RefreshLine (firstLine+i);
   }


void TGrid::EvShowWindow (bool show, uint)
   {
   if (show)
      {
      SetWindowPos (0, Attr.X, Attr.Y, Attr.W, Attr.H, SWP_NOZORDER|SWP_SHOWWINDOW); 
      UpdateWindow ();
      }
   else
      SetWindowPos (0, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW);
   }


// ----------------------------------------------------------------------- //
// Changing grid size                                                      //
// ----------------------------------------------------------------------- //
void TGrid::EvWindowPosChanging (WINDOWPOS far &wpos)
   {
   ChangeWindowPos (wpos.cx, wpos.cy, wpos.flags);
   TControl::EvWindowPosChanging (wpos);
   }


// Utility functions added by René Mayer
bool TGrid::IsHScrollbarNeeded ()
   {
   bool willNeedHScroll = false;
   if (hscroll)
      {
      willNeedHScroll = true;
      if (hideHScroll && logicalWidth <= clientWidth)
         willNeedHScroll = false;
      }
   return willNeedHScroll;
   }


bool TGrid::IsVScrollbarNeeded ()
   {
   bool willNeedVScroll = false;
   if (vscroll)
      {
      willNeedVScroll = true;
      if (hideVScroll && lineHeight && maxLines > 0 && (clientHeight / lineHeight) >= maxLines)
         willNeedVScroll = false;
      }
   return willNeedVScroll;
   }
// end of update by René Mayer


void TGrid::ChangeWindowPos (int &w, int &h, uint flags)
   {
   // if there are no columns in TGrid, don't do anything yet.
   if (firstColumn == 0)
      return;

   int oldFirstLine = firstLine;
   int oldVisibleLines = visibleLines;

   bool Move = ((flags & SWP_NOMOVE) == 0 ? true : false);
   bool Size = ((flags & SWP_NOSIZE) == 0 ? true : false);

   // get size of elements
   titleHeight = title->CalcHeight();
   vbarWidth   = vbar  ? vbar ->CalcWidth()  : 0;
   clientWidth = w - vbarWidth - 2;
   clientHeight = h - titleHeight - 2;

   // check if scroll bars are visible (Update by René Mayer)
   bool willNeedVScroll = IsVScrollbarNeeded ();
   bool willNeedHScroll = IsHScrollbarNeeded ();
   if (willNeedVScroll)
      {
      clientWidth -= cxvscroll;
      if (IsHScrollbarNeeded ())
         clientHeight -= cyhscroll;
      }
   else if (willNeedHScroll)
      {
      clientHeight -= cyhscroll;
      if (IsVScrollbarNeeded ())
         clientWidth -= cxvscroll;
      }
   // end of update by René Mayer

   if (lastColumn && firstColumn && lastColumn->left+lastColumn->width < clientWidth && firstColumn->left <= 0)
      MoveHPos (lastColumn->left+lastColumn->width - clientWidth);

   if (Size)
      {
      if (hscroll && hscroll->IsWindow())
         {
         if (logicalWidth <= clientWidth)
            {
            if (disableHScroll)
               hscroll->EnableWindow (false);
            if (hideHScroll)
               {
               if (!hscrollHidden)
                  {
                  hscroll->ShowWindow (SW_HIDE);
                  hscrollHidden = true;
                  }
               }
            }
         else
            {
            if (hscrollHidden)
               {
               hscroll->ShowWindow (SW_SHOW);
               hscrollHidden = false;
               }
            hscroll->EnableWindow (true);
            }
         }

      }

   if (Move || Size)
      {
      uint flag = SWP_NOZORDER;

      if (!Size)
         flag |= SWP_NOSIZE;
      else if (!Move)
         flag |= SWP_NOMOVE;

      // adjust to integral line height -------------------------------------
      if (h && lineHeight)
         {
         int old = visibleLines;
         int curline = firstLine+activeLine;
         SetVisibleLines (clientHeight / lineHeight);
         if (maxLines > 0 && visibleLines >= maxLines)
            {
            if (vscroll && vscroll->IsWindow())
               {
               if (!vscrollHidden)
                  {
                  if (disableVScroll)
                     vscroll->EnableWindow (false);
                  if (hideVScroll)
                     {
                     vscroll->ShowWindow (SW_HIDE);
                     vscrollHidden = true;
                     }
                  }
               }
            if (vertShrink)
               SetVisibleLines (maxLines);
            }
         else if (vscroll && vscroll->IsWindow())
            {
            if (vscrollHidden)
               {
               vscroll->ShowWindow (SW_SHOW);
               vscrollHidden = false;
               }
            vscroll->EnableWindow (true);
            }

         if (visibleLines <= 0)
            SetVisibleLines (1);

         if (visibleLines != old)
            {
            Go (0);
            if (curline)
               Go (curline);
            }

         clientHeight = visibleLines * lineHeight;
         }

      // adjust related controls -----------------------------------------------
      if (horzShrink && clientWidth > logicalWidth)
         clientWidth = logicalWidth;

      flag &= (~SWP_NOMOVE);   

      client->SetWindowPos (0, vbarWidth, titleHeight, clientWidth-1, clientHeight, flag);

      if (vbar && vbar->IsWindow())
         vbar->SetWindowPos (0, 0, titleHeight, vbarWidth, clientHeight, flag);

      if (title && title->IsWindow())
         {
         if (titleHeight > 0)
            title->SetWindowPos (0, vbarWidth, 0, clientWidth-1, titleHeight, flag);
         else
            title->SetWindowPos (0, 0, 0, 0, 0, flag|SWP_HIDEWINDOW);
         }
      if (vscroll && vscroll->IsWindow() && !vscrollHidden)
         vscroll->SetWindowPos (0, vbarWidth+clientWidth-1, titleHeight, cxvscroll, clientHeight, flag);

      if (hscroll && hscroll->IsWindow() && !hscrollHidden)
         {
         hscroll->SetWindowPos (0, vbarWidth, titleHeight+clientHeight, clientWidth-1, cyhscroll, flag);
         SetHScrollRange ();
         }
      }

   if (firstLine != oldFirstLine || visibleLines != oldVisibleLines)
      ScrollData (firstLine, firstLine+visibleLines-1);
   }


bool TGrid::CalcPos ()
   {
   // If the grid has been created but the border controls aren't created yet,
   // we have to create them ourselves here                                 //
   if (IsWindow())
      {
      if (!client->IsWindow ())
         client->Create ();
      if (vbar && !vbar->IsWindow ())
         vbar->Create ();
      if (title && !title->IsWindow ())
         title->Create ();
      if (hscroll && !hscroll->IsWindow ())
         hscroll->Create ();
      if (vscroll && !vscroll->IsWindow ())
         vscroll->Create ();
      }

   // abort if:                                                             //
   //    - TGrid's HWindow is not valid                                     //
   // or - title's HWindow is not valid                                     //
   // or - vertical bar'sHWindow is not valid                               //
   // or - CalcPos has been successfully executed previously                //
   // Note: we need to check for multiple successful executions of CalcPos  //
   //       because CalcPos is called from Create(), which can be (and is)  //
   //       called more than once.                                          //
   if (calcPos || !IsWindow())
      return false;

   Column *ctrl;
	for (ctrl = firstColumn; ctrl; ctrl = ctrl->next)
		{
      if (!ctrl->control->IsWindow ())
         return false;
      }

   calcPos = true;

   // create the grid's TFont -----------------------------------------------
   if (gridFont == 0)
      {
      SetFont ();
      if (titleLogFont)
         SetTitleFont ();
      }

   // line height &  column width -------------------------------------------
   logicalWidth = 0;
   int wasHidden = 0;                      // Charles R. Cordell

	for (ctrl = firstColumn; ctrl; ctrl = ctrl->next)
		{
      // section added by Charles R. Cordell
      if ( ctrl->Hidden == -1)
         {
         ctrl->Hidden = 0;   //unhide temp.
         wasHidden = true;   // remember
         }
      // end of contribution

      int h = ctrl->CtrlHeight ();
      if (h >= lineHeight)
         SetLineHeight (h);

		int w = ctrl->CtrlWidth();
      ctrl->left = logicalWidth;
      ctrl->width = w+1;

      if (ctrl->type == gCheck)
         ctrl->width += 2;

      if (ctrl->left + ctrl->width > logicalWidth)
         logicalWidth = ctrl->left + ctrl->width;

      // make the control a child of TGrid to clip it inside the grid -------
      ctrl->control->SetParent (client);
      ctrl->HideControl ();

      // section added by Charles R. Cordell
      if ( wasHidden )
         {
         wasHidden = 0;
         for (Column *col = ctrl->next; col; col = col->next)
            col->left -= ctrl->width;
         logicalWidth -= ctrl->width;
         ctrl->Hidden = ctrl->width;
         ctrl->width = 0;
         }
      // end of contribution
      }

   if (vscroll)
      {
      if (maxLines < 0)
         vscroll->SetLRange (0, (nLines*10)/9+1, true);
      else
         vscroll->SetLRange (0, maxLines-1, true);
      vscroll->SetLPosition (0);
      }

   if (hscroll)
      {
      hscroll->LineMagnitude = LOWORD(GetDialogBaseUnits());// approx. 1 char
      hscroll->SetPosition (0);
      }

   // SetWindowPos will make sure everything is in place --------------------
   SetWindowPos (0, Attr.X, Attr.Y, Attr.W, Attr.H, SWP_NOZORDER);  
   GotoFirstColumn ();

   // Line selection --------------------------------------------------------
   if (lineSelection == Single || lineSelection == SingleAutoSelect)
      selectedLine = 0;

   return true;
   }


void TGrid::SetHScrollRange ()
   {
   if (hscroll && hscroll->IsWindow())
      {
      ::SetScrollRange (*hscroll, SB_CTL, 0, logicalWidth - clientWidth -1, true);
      hscroll->PageMagnitude = clientWidth;
      }
   }

// ----------------------------------------------------------------------- //
// Data messages                                                           //
// ----------------------------------------------------------------------- //
bool TGrid::CanClose ()
	{
	int line = GetCurrentLine ();
   if (current)
      return (CanKillFocus (current) && ValidateLine (line));
   else
      return (ValidateLine (line));
	}


// ----------------------------------------------------------------------- //
// Mouse functions                                                         //
// ----------------------------------------------------------------------- //
bool TGrid::SetCursorShape (TPoint &p) // p is the mouse cursor position in client coordinates inside the 'client' control
   {
   if (cursorType == DefCursor)
      {
      int x = p.x+1;

      for (Column *e = firstColumn; e; e = e->next)
         {
         int right = e->left+e->width;
         if (x >= e->left && x < right)
            {
            TControl *c = e->control;
            switch (e->type)
               {
               default:
                  client->SetCursor (0, IDC_ARROW);
                  break;
               case gCombo:
                  {
                  int w = LOWORD(GetDialogBaseUnits());
                  if (x < right-w-w && (c->Attr.Style & CBS_DROPDOWNLIST) != CBS_DROPDOWNLIST)
                     client->SetCursor (0, IDC_IBEAM);
                  else
                     client->SetCursor (0, IDC_ARROW);
                  break;
                  }
               case gDateEdit:
               case gTimeEdit:
               case gShortEdit:
               case gLongEdit:
               case gDoubleEdit:
               case gEdit:
                  client->SetCursor (0, IDC_IBEAM);
                  break;
               }
            client->ClientToScreen (p);
            SetCursorPos (p.x, p.y);
            return true;
            }
         }
      }
   else
      {
      // if cursor is on the call with focus, use standard behaviour
      if (current && current->control && current->control->IsWindowVisible ())
         {
         TRect rc = current->control->GetWindowRect ();
         client->ClientToScreen (p);
         if (rc.Contains (p))
            return false;
         }
      client->SetCursor (0, IDC_ARROW);
      return true;
      }

   return false;
   }


void TGrid::EvLButtonDown (uint modKeys, TPoint &point)
   {
   MapWindowPoints (*client, &point, 1);
   ProcessMouse (modKeys, point, WM_LBUTTONDOWN);
   }


void TGrid::EvLButtonUp (uint modKeys, TPoint &point)
   {
   MapWindowPoints (*client, &point, 1);
   ProcessMouse (modKeys, point, WM_LBUTTONUP);
   }


void TGrid::EvLButtonDblClk (uint modKeys, TPoint &point)
   {
   MapWindowPoints (*client, &point, 1);
   ProcessMouse (modKeys, point, WM_LBUTTONDBLCLK);
   }


void TGrid::EvRButtonDown (uint modKeys, TPoint &point)
   {
   MapWindowPoints (*client, &point, 1);
   ProcessMouse (modKeys, point, WM_RBUTTONDOWN);
   }


void TGrid::EvRButtonUp (uint modKeys, TPoint &point)
   {
   MapWindowPoints (*client, &point, 1);
   ProcessMouse (modKeys, point, WM_RBUTTONUP);
   }


void TGrid::EvRButtonDblClk (uint modKeys, TPoint &point)
   {
   MapWindowPoints (*client, &point, 1);
   ProcessMouse (modKeys, point, WM_RBUTTONDBLCLK);
   }


void TGrid::ProcessMouse (uint modKeys, TPoint &point, uint message, bool sendToControl)
   {
   static bool echo = false;

   if (echo)
      {
      echo = false;
      return;
      }

   if (message == WM_LBUTTONDOWN)
      {
      int old = activeLine;
      int line = point.y / lineHeight;
      Column *e = FindColumn (point);
      client->ClientToScreen (point);

      if (line != activeLine)
         {
         if (!CanKillFocus (current))
            return;
         Go (firstLine+line, false);
         }

      if (e)
         {
         TControl *c = e->control;
         int ln = GetCurrentLine ();
         int id = c->GetId ();
         Visible v = VisibleCell (ln, id);
         if (e->type != gStatic && v != gEmpty && v != gLocked)
            {
            bool tabStop = ( (c->Attr.Style & WS_TABSTOP) ? true : false);
            if (dynamic_cast<TGCheckBox *>(c) && !tabStop)
               {
               bool val = *static_cast<const bool *>(TransferItem (ln, id));
               val ^= 1;
               if (DataChanging (ln, id, static_cast<void *>(&val)))
                  {
                  modified = true;
                  TransferItem (ln, id, static_cast<void *>(&val));
                  DataChanged (ln, id, static_cast<void *>(&val));
                  ClearModify (c);
                  }
               RefreshItem (ln, id);
               }
            else
               {
               if (e != current || line != old)
                  e->SetFocus ();
               if (sendToControl)
                  {
                  echo = true;
                  c->ScreenToClient (point);
                  c->SendMessage (message, modKeys, MAKELONG(point.x,point.y));
                  }
               }
            }
         }

      if (staticOnly && lineSelection != SingleAutoSelect)
         SelectLine (GetCurrentLine (), modKeys);   // enhanced by Dean Crawford
      }
   }


// ----------------------------------------------------------------------- //
// Borders and attributes                                                  //
// ----------------------------------------------------------------------- //
void TGrid::ChangeTitle (int id, const char *newTitle)
   {
   Column *e = FindColumn (id);
   if (e)
      e->SetTitle (newTitle);
   }


const char *TGrid::GetTitle (int id) const
   {
   Column *e = FindColumn (id);
   if (e)
      return e->title.c_str();
   else
      return 0;
   }


void TGrid::SetLineMarker (TGrid::LineMarker lm)
   {
   if (lm == lineMarker)
      return;

   // remove old markers ----------------------------------------------------
   if (vbar)
      vbar->ShowText (activeLine);

   lineMarker = lm;

   if (vbar)
      vbar->ShowMarker (activeLine);
   }


void TGrid::SetVBarText (int cwidth)
   {
   if (vbar)
      vbar->SetUserText (cwidth);
   }


void TGrid::SetFont (int id, const char *facename, int height, int weight, bool italic)
   {
   if (!IsWindow())
      {
      Column *e = FindColumn (id);
      if (e)
         e->SetFont (facename, height, weight, italic);
      }
   }


void TGrid::SetFont (const char *facename, int height, int weight, bool italic)
   {
   if (!IsWindow())
      {
      strcpy (gridLogFont->lfFaceName, facename);
      if (height)
         gridLogFont->lfHeight = height;
      gridLogFont->lfWeight = weight;
      gridLogFont->lfItalic = BYTE(italic);
      }
   }


void TGrid::SetTitleFont (const char *facename, int height, int weight, bool italic)
   {
   if (!IsWindow ())
      {
      if (titleLogFont == 0)
         {
         titleLogFont = new LOGFONT;
         memcpy (titleLogFont, gridLogFont, sizeof(LOGFONT));
         }
      strcpy (titleLogFont->lfFaceName, facename);
      if (height)
         titleLogFont->lfHeight = height;
      titleLogFont->lfWeight = weight;
      titleLogFont->lfItalic = BYTE(italic);
      }
   }


void TGrid::SetFont ()
   {
   if (gridFont)
      delete gridFont;
   gridFont = new TFont (gridLogFont);

   SetWindowFont (*gridFont, true);

   for (Column *ctrl = firstColumn; ctrl; ctrl = ctrl->next)
      ctrl->SetFont ();
   }


void TGrid::SetTitleFont ()
   {
   if (titleFont)
      delete titleFont;
   if (titleLogFont == 0)
      {
      titleLogFont = new LOGFONT;
      memcpy (titleLogFont, gridLogFont, sizeof(LOGFONT));
      }
   titleFont = new TFont (titleLogFont);
   }


TColor TGrid::GetTitleColor (TGrid::TitleColor part) const
   {
   switch (part)
      {
      case TitleText:
         return GetSysColor (COLOR_BTNTEXT);
      case TitleBackground:
         return GetSysColor (COLOR_BTNFACE);
      case TitleHilite:
         return GetSysColor (COLOR_BTNHIGHLIGHT);
      case TitleShadow:
         return GetSysColor (COLOR_BTNSHADOW);
      case TitleFrame:
         return GetSysColor (COLOR_WINDOWFRAME);
      default:
         return TColor::Black;
      }
   }


TFont *TGrid::GetGridFont (int id) const
   {
   Column *e = FindColumn (id);
   if (e->font)
      return e->font;
   else
      return gridFont;
   }



void TGrid::SetVisibleCells (Visible v)
   {
   Visible old = visible;
   visible = v;
   if (old != v)
      Refresh ();
   }


void TGrid::HorzShrink ()
   {
   if (!IsWindow() || logicalWidth >= clientWidth)
      return;

   int delta = clientWidth - logicalWidth;
   SetWindowPos (0, 0, 0, Attr.W-delta, Attr.H, SWP_NOZORDER|SWP_NOMOVE);
   }


void TGrid::SetTitle (int id, const char *text)
   {
   Column *c = FindColumn (id);
   if (c)
      c->SetTitle (text);
   }


int TGrid::GetColumnWidth (int id) const
   {
   Column *c = FindColumn (id);
   if (c)
      return c->width;
   else
      return 0;
   }

// EOF
