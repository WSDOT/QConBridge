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


#include "griddll.h"
#pragma hdrstop

TGDataComboBox::TGDataComboBox (TWindow *p, int resid, uint textlen, TModule *module)
   : TGComboBox (p, resid, textlen, module)
   {
   }


TGDataComboBox::TGDataComboBox (TWindow *p, int resid, int x, int y, int w, int h, DWORD style, uint textlen, TModule *module)
   : TGComboBox (p, resid, x, y, w, h, style, textlen, module)
   {
   }


int TGDataComboBox::GetSelectionIndex (uint32 data)
   {
   // Looking for the index of the entry which has itemData == data
   int count = GetCount();
   for (int index = 0; index < count; index++)
      {
      if (data == GetItemData (index))
         return index;
      }

   return 0;
   }


void * TGDataComboBox::GetData ()
   {
   // return the itemData value of the selected entry (not its text) as a pointer to type int
   // (AddItem must be used when filling the combo box - NOT AddString)
   uint32 itemData = GetItemData (GetSelIndex());
   int *data = new int;
   *data = int(itemData);
   return static_cast<void *>(data);
   }


void TGDataComboBox::SetData (void *data)
   {
   // Returns the itemData-Value of the selected entry (not its text)
   // (AddItem must be used when filling the combo box - NOT AddString)
   if (data)
      {
      uint32 itemData = static_cast<uint32>(*(int *)data);
      SetSelIndex (GetSelectionIndex (itemData));
      }
   else
      SetSelIndex (0);
   }


int TGDataComboBox::AddItem (int itemData, const char far *str)
   {
   int newItem = AddString (str);
   if (newItem >= 0)
      SetItemData (newItem, static_cast<uint32>(itemData));
   return newItem;
   }


uint TGDataComboBox::Transfer (void *, TTransferDirection)
   {
   return sizeof(int);
   }


void TGDataComboBox::DrawCell (TDC &dc, TRect &rc, const void *p, TGrid::Align align)
   {
   if (p)
      {
      uint32 itemData = static_cast<uint32>(*(int *)p);
      int n = GetSelectionIndex (itemData);
      int l = (n >= 0 ? GetStringLen (n) : 0);
      char *ptext = (l > 0 ? new char[l+1] : 0);
      if (ptext)
         {
         GetString (ptext, n);
         TGComboBox::DrawCell (dc, rc, static_cast<const void *>(ptext), align);
         delete [] ptext;
         }
      return;
      }
   TGComboBox::DrawCell (dc, rc, p, align);
   }

// EOF

