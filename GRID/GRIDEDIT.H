/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// gridedit.h                                                              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#ifndef GRIDEDIT_H
#define GRIDEDIT_H

#if !defined(OWL_EDIT_H)
#include <owl\edit.h>
#endif

class _GRIDCLASS TGEdit : public TEdit
   {
   friend TGrid;

   COLOR_HEADER
   MOUSE_HANDLER_HEADER

   protected:
      TGrid *grid;
      TColor columnbkg;
      TValidator *savedValidator;

      TColor ColumnBkg () { return columnbkg; }

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
      TGEdit (TWindow* parent, int Id, const char far *text, int x, int y, int w, int h, uint textLen = 0,
              bool multiline = false, TModule* module = 0);
      TGEdit (TWindow *parent, uint resourceID, uint textLen = 0, TModule *module = 0);
     ~TGEdit ();

      virtual int Height () { return Attr.H; }
      virtual int  Width () { return Attr.W; }
      bool Create ();
      void SetColors (TColor t, TColor b, TColor a);
      void SuspendValidator () { savedValidator = Validator; Validator = 0; }
      void RestoreValidator () { Validator = savedValidator; }

      operator string ();
      string &operator = (string &s);

      DECLARE_RESPONSE_TABLE (TGEdit);
   };

#endif

// EOF

