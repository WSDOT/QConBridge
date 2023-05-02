/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// gridstat.h                                                              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#ifndef GRIDSTAT_H
#define GRIDSTAT_H

#if !defined(OWL_STATIC_H)
#include <owl\static.h>
#endif

class _GRIDCLASS TGStatic : public TStatic
   {
   friend TGrid;

   COLOR_HEADER

   protected:
      TGrid *grid;

      virtual void DrawCell (TDC &dc, TRect &rc, const void *p,
                             TGrid::Align align = TGrid::Left);
      virtual void Init ();

   public:
      TGStatic (TWindow *p, int id, char *title, int x, int y, int w, int h,
                int textlen = 0, TModule *module = 0);
      TGStatic (TWindow *p, int id, int textlen = 0, TModule *module = 0);
     ~TGStatic ();

      bool Create ();
      virtual int Height () { return Attr.H; }
      virtual int  Width () { return Attr.W; }   
   };

#endif

// EOF
