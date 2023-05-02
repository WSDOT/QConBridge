/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// datacbox.h                                                              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
// Author: René Mayer                                                      //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


#ifndef DATACBOX_H
#define DATACBOX_H

class _GRIDCLASS TGDataComboBox : public TGComboBox
   {
   protected:
      virtual void *GetData ();
      virtual void SetData (void *data);
      virtual void DrawCell (TDC &dc, TRect &rc, const void *p, TGrid::Align align = TGrid::Left);
      int GetSelectionIndex (uint32 data);

   public:
      TGDataComboBox (TWindow *p, int resid, uint textlen=0, TModule *module = 0);
      TGDataComboBox (TWindow *p, int resid, int x, int y, int w, int h, DWORD style, uint textlen=0, TModule *module = 0);

      int AddItem (int itemData, const char far *str);
      uint Transfer (void *buffer, TTransferDirection direction);
   };

#endif

// EOF

