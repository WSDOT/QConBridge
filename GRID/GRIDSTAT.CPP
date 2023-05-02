/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// gridstat.cpp                                                            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "griddll.h"
#pragma hdrstop

TGStatic::TGStatic (TWindow *p, int id, int textlen, TModule *module)
        : TStatic (p, id, textlen, module)
   {
   Init ();
   }


TGStatic::TGStatic (TWindow *p, int id, char *title, int x, int y, int w, int h, int textlen, TModule *module)
        : TStatic (p, id, title, x, y, w, h, textlen, module)
   {
   Init ();
   }


void TGStatic::Init ()
   {
   grid = 0;
   bool bwcc = GetApplication()->BWCCEnabled ();
   bool b3D  = GetApplication()->Ctl3dEnabled ();
   bkg   = TColor(GetSysColor (bwcc||isChicago||b3D ? COLOR_BTNFACE : COLOR_WINDOW));
   text  = TColor(GetSysColor (bwcc||isChicago||b3D ? COLOR_BTNTEXT : COLOR_WINDOWTEXT));
   brush = new TBrush (bkg);      
   }


bool TGStatic::Create ()
   {
   bool rc = TStatic::Create ();
   if (rc && grid)
      grid->CalcPos ();
   return rc;
   }


TGStatic::~TGStatic ()
   {
   delete brush;
   }


COLOR_CODE(TGStatic)


void TGStatic::DrawCell (TDC &dc, TRect &rc, const void *p, TGrid::Align align)
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
         bg = bkg;
         }
      }

   dc.SetTextColor (fg);
   dc.SetBkColor   (bg);
   dc.FillRect (rc, TBrush(bg));

   rc.Inflate (-1, -1);

   if (p && *static_cast<const char *>(p))
      {
		WORD alignment = static_cast<WORD>(align);
		alignment |= DT_NOPREFIX;
		if ((Attr.Style & SS_LEFTNOWORDWRAP) == 0 && (Attr.Style & SS_SIMPLE) == 0)
			alignment |= DT_WORDBREAK;
		dc.DrawText (static_cast<const char *>(p), -1, rc, alignment);
      }
   }

// EOF
