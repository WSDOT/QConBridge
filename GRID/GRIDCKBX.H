/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// gridckbx.h                                                              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


#ifndef GRIDCKBX_H
#define GRIDCKBX_H

#if !defined(OWL_CHECKBOX_H)
#include <owl\checkbox.h>
#endif

class _GRIDCLASS TGCheckBox : public TCheckBox
   {
   friend TGrid;

   COLOR_HEADER
   MOUSE_HANDLER_HEADER

   protected:
      TGrid *grid;
      int oldstate;
      bool bwcc;

      virtual void DrawCell (TDC &dc, TRect &rc, const void *p, TGrid::Align align = TGrid::Left);
      virtual void Init ();

      void EvChar (uint key, uint repeat, uint flags);
      uint EvGetDlgCode (MSG far *msg);
      void EvKeyDown (uint key, uint repeat, uint flags);
      void EvKeyUp (uint key, uint repeat, uint flags);
      void EvKillFocus (HWND hWndGetFocus);
      void EvSetFocus (HWND hWndLostFocus);
      bool PreProcessMsg (MSG &msg);

   public:
      TGCheckBox (TWindow *p, uint id, TGroupBox *group =0,
                  TModule *module =0);
      TGCheckBox (TWindow *p, uint id, char *title, int x, int y, int w, int h,
                  TGroupBox *group =0, TModule *module =0);
     ~TGCheckBox ();

      void SetBwcc (bool flag) { bwcc = flag; }

      void ClearModify ();
      bool IsModified ();
      bool Create        ();
      virtual int Height () { return Attr.H; }
      virtual int  Width () { return Attr.W; }  

      DECLARE_RESPONSE_TABLE (TGCheckBox);
   };

#endif

// EOF
