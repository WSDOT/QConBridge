/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// gridbtn.h                                                               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#ifndef GRIDBTN_H
#define GRIDBTN_H

#if !defined(OWL_BUTTON_H)
#include <owl\button.h>
#endif

class _GRIDCLASS TGButton : public TButton
   {
   friend TGrid;

   MOUSE_HANDLER_HEADER

   protected:
      TGrid *grid;

      virtual void DrawCell (TDC &dc, TRect &rc, const void *p, TGrid::Align align = TGrid::Left);

      void EvChar (uint key, uint repeat, uint flags);
      uint EvGetDlgCode (MSG far *msg);
      void EvKeyDown (uint key, uint repeat, uint flags);
      void EvKeyUp (uint key, uint repeat, uint flags);
      void EvKillFocus (HWND hWndGetFocus);
      void EvSetFocus (HWND hWndLostFocus);
      bool PreProcessMsg (MSG &msg);
      uint Transfer (void *buffer, TTransferDirection direction);

   public:
      TGButton (TWindow *p, int id, TModule *module = 0);
      TGButton (TWindow *p, int id, char *text, int x, int y, int w, int h, bool isdefault = false, TModule *module = 0);

      virtual int Height () { return Attr.H; }
      virtual int  Width () { return Attr.W; }
      bool Create ();

      DECLARE_RESPONSE_TABLE (TGButton);
   };
 
#endif

// EOF
