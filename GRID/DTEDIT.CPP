/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// dtedit.cpp                                                              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "griddll.h"
#pragma hdrstop

DEFINE_RESPONSE_TABLE1 (TGDateTimeEdit, TGEdit)
   EV_WM_CHAR,
END_RESPONSE_TABLE;

bool TGDateTimeValidator::IsValidInput (char far *str, bool suppressFill)
   {
   const char *f = owner->Format.c_str();
   string s = str;
   if (s.is_null ())
      return true;
   const char *p = s.c_str();
   const char *p0 = p;
   bool touched = false;

   while (*p)
      {
      if (*f == 0)
         return false;
      else if (strchr (validChar, *f))
         {
         if (!isdigit (*p))
            return false;
         }
      else
         {
         if (*f == '\\')
            f++;
         if (*p != *f)
            {
            if (isdigit (*p))
               {
               if (!suppressFill)
                  {
                  int offset = (int)(p - p0);
                  s.insert (offset, *f);
                  p = &s(offset);
                  IncCaret ++;
                  }
               }
            else
               *(const_cast<char *>(p)) = *f;
            touched = true;
            }
         }
      p++;
      f++;
      }

   if (touched)
      strcpy (str, s.c_str());

   return true;
   }


TGDateTimeEdit::TGDateTimeEdit (TWindow *p, uint resid, const char *fmt,
                                TModule *module)
              : TGEdit (p, resid, strlen (fmt) + 1, module)
   {
   }


TGDateTimeEdit::TGDateTimeEdit (TWindow *p, uint id, int x, int y,
                                int w, int h, const char *fmt,
                                TModule *module)
             : TGEdit (p, id, "", x, y, w, h, strlen (fmt) + 1, false, module)
   {
   }


void TGDateTimeEdit::EvChar(uint key, uint repeatCount, uint flags)
   {
   TGDateTimeValidator *v = dynamic_cast<TGDateTimeValidator *>(Validator);
   if (v)
      v->IncCaret = 0;
   TGEdit::EvChar (key, repeatCount, flags);
   if (v && v->IncCaret)
      {
      uint start, end;
      GetSelection (start, end);
      start += v->IncCaret;
      int l = GetTextLen ();
      if (start > l)
         start = l;
      SetSelection (start, start);
      }
   }

// EOF

