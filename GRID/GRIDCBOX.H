/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// gridcbox.h                                                              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


#ifndef GRIDCBOX_H
#define GRIDCBOX_H

#if !defined(OWL_COMBOBOX_H)
#include <owl\combobox.h>
#endif

class _GRIDCLASS TGComboBox : public TComboBox
   {
   friend TGrid;
   friend TGrid::Column;

   COLOR_HEADER
   MOUSE_HANDLER_HEADER

   protected:
      TColor activeBG;
      TColor selectedFG;
      TGrid *grid;
      int oldsel;
      string oldstr;
      TColor columnbkg;
      int totalHeight;

      TColor ColumnBkg () { return columnbkg; }                      

      virtual void CbnCloseUp    ();
      virtual void CbnDropDown   ();
      virtual void CbnKillFocus  ();
      virtual void CbnSetFocus   ();
      virtual void DrawCBItem    (DRAWITEMSTRUCT &info);
      virtual void DrawCell      (TDC &dc, TRect &rc, const void *p, TGrid::Align align = TGrid::Left);
      virtual void *GetData      ();
      virtual void Init          ();
      virtual void InitDraw      (DRAWITEMSTRUCT &info, TDC &dc, char * &p, TRect &rect);
      virtual void MeasureItem   (MEASUREITEMSTRUCT &info);
      virtual void SetData       (void *);

      void EvChar        (uint key, uint repeat, uint flags);
      HBRUSH EvCtlColor  (HDC hDC, HWND hWndChild, uint);
      uint EvGetDlgCode  (MSG far *msg);
      void EvKeyDown     (uint key, uint repeat, uint flags);
      void EvKeyUp       (uint key, uint repeat, uint flags);
      void EvKillFocus   (HWND hWndGetFocus);
      void EvSetFocus    (HWND hWndLostFocus);
      void ODADrawEntire (DRAWITEMSTRUCT &info);
      void ODASelect     (DRAWITEMSTRUCT &info);
      bool PreProcessMsg (MSG &msg);
      void SetupWindow   ();

   public:
      TGComboBox (TWindow *p, int resid, uint textlen = 0, TModule *module = 0);
      TGComboBox (TWindow *p, int resid, int x, int y, int w, int h, DWORD style, uint textlen = 0, TModule *module = 0);
     ~TGComboBox ();

      virtual int Height ();
      virtual int Width  () { return Attr.W; }
      void SetColors     (TColor t, TColor b, TColor a);
      void ClearModify   ();
      bool IsModified    ();
      bool Create        ();

      DECLARE_RESPONSE_TABLE (TGComboBox);
   };

#endif

// EOF
