/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// dateedit.cpp                                                            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "griddll.h"
#pragma hdrstop

DEFINE_RESPONSE_TABLE1 (TGDateEdit, TGDateTimeEdit)
   EV_WM_KILLFOCUS,
   EV_WM_KEYDOWN,
   EV_WM_RBUTTONDBLCLK,
END_RESPONSE_TABLE;


// TGDateValidator //////////////////////////////////////////////////////////
TGDateValidator::TGDateValidator (const IntlDate &min, const IntlDate &max, TGDateEdit *own, bool n)
               : TGDateTimeValidator ("dDmMyY")
   {
   Min = min;
   Max = max;
   owner = own;
   notNull = n;
   }


void TGDateValidator::Error(TWindow* parent)
   {
   string date = language.LoadString (DATE_TITLE);
   const char *d = date.c_str();
   #define FLAGS MB_ICONEXCLAMATION | MB_OK | MB_TASKMODAL

   owner->SuspendValidator ();
   switch (errcode)
      {
      case DATE_IS_NULL:
         parent->MessageBox (language.LoadString(DATE_NODATE).c_str(), d, FLAGS);
         break;

      case DATE_OUT_OF_RANGE:
         {
         string mess = language.LoadString (DATE_OUTOFRANGE);
         size_t pos = mess.find ("%s");
         if (pos != NPOS)
            {
            mess.replace (pos, 2, Min.String (owner->Format));
            pos = mess.find ("%s");
            if (pos != NPOS)
               mess.replace (pos, 2, Max.String (owner->Format));
            }
         parent->MessageBox (mess.c_str(), d, FLAGS);
         }
         break;

      default:
         parent->MessageBox (language.LoadString (DATE_INCORRECT).c_str(),
                            d, FLAGS);
         break;
      }

   owner->RestoreValidator ();
   }


bool TGDateValidator::IsValid (const char * s)
   {
   errcode = DATE_OK;

   if (string(s).is_null ())
      {
      if (notNull)
         {
         errcode = DATE_IS_NULL;
         return false;
         }
      else
         return true;
      }

   IntlDate val (s, owner->Format.c_str());
   if (!val.IsValid ())
      {
      errcode = DATE_FORMAT;
      return false;
      }

   if (val < Min || val > Max)
      {
      errcode = DATE_OUT_OF_RANGE;
      return false;
      }
      
   return true;
   }



TGDateEdit::TGDateEdit (TWindow *p, uint resid, const char *fmt,
                        const IntlDate &min, const IntlDate &max,
                        bool notNull, TModule *module)
          : TGDateTimeEdit (p, resid, fmt, module)
   {
   SetFilter (min, max, fmt, notNull);
   }


TGDateEdit::TGDateEdit (TWindow *p, uint resid, const string &fmt,
                        const IntlDate &min, const IntlDate &max,
                        bool notNull, TModule *module)
          : TGDateTimeEdit (p, resid, fmt.c_str(), module)
   {
   SetFilter (min, max, fmt, notNull);
   }


TGDateEdit::TGDateEdit (TWindow *p, uint id, int x, int y, int w, int h,
                        const char *fmt, const IntlDate &min,
                        const IntlDate &max, bool notNull,
                        TModule *module)
          : TGDateTimeEdit (p, id, x, y, w, h, fmt, module)
   {
   SetFilter (min, max, fmt, notNull);
   }


TGDateEdit::TGDateEdit (TWindow *p, uint id, int x, int y, int w, int h,
                        const string &fmt, const IntlDate &min,
                        const IntlDate &max, bool notNull,
                        TModule *module)
          : TGDateTimeEdit (p, id, x, y, w, h, fmt.c_str(), module)
   {
   SetFilter (min, max, fmt, notNull);
   }


void TGDateEdit::SetMin (const IntlDate &d)
   {
   TGDateValidator *dv = dynamic_cast<TGDateValidator *>(Validator);
   if (dv)
      {
      dv->SetMin (d);
      if (dv->GetMax() < d)
         dv->SetMax (d);
      }
   }


void TGDateEdit::SetMax (const IntlDate &d)
   {
   TGDateValidator *dv = dynamic_cast<TGDateValidator *>(Validator);
   if (dv)
      {
      dv->SetMax (d);
      if (dv->GetMin() > d)
         dv->SetMin (d);
      }
   }


void TGDateEdit::EvKillFocus (HWND h)
   {
   TGEdit::EvKillFocus (h);
   SetText (IntlDate (*this).String(Format));
   }


void TGDateEdit::EvKeyDown (uint vkey, uint repeat, uint flags)
   {
   TGEdit::EvKeyDown (vkey, repeat, flags);
   if (vkey == VK_F2)
      *this = IntlDate (idToday);
   }


void TGDateEdit::EvRButtonDblClk (uint keys, TPoint &where)
   {
   TGEdit::EvRButtonDblClk (keys, where);
   *this = IntlDate (idToday);
   }


IntlDate TGDateEdit::operator = (const IntlDate &d)
   {
   IntlDate date (d);
   SetText (date.String (Format));
   return date;
   }


TGDateEdit::operator IntlDate ()
   {
   int l = TextLen;
   if (l > 63)
      l = 63;
   char buffer[64];
   GetText (buffer, l+1);
   IntlDate date;
   date.InterpretString (buffer, Format);
   return date;
   }


void TGDateEdit::SetText (const char far *s)
   {
   IntlDate date;
   date.InterpretString (s, Format);
   TGEdit::SetText (s);
   }


void TGDateEdit::SetText (const string &s)
	{
   IntlDate date;
	date.InterpretString (s, Format);
	TGEdit::SetText (s.c_str());
	}


void TGDateEdit::SetFilter (const IntlDate &min, const IntlDate &max,
									 const string &fmt, bool notNull)
	{
	SetFilter (min, max, fmt.c_str(), notNull);
	}


void TGDateEdit::SetFilter (const IntlDate &min, const IntlDate &max,
                            const char *fmt, bool notNull)
   {
   if (Validator)
      {
      delete Validator;
      Validator = 0;
      }

   // Force format to a valid string
	string s;
   while (fmt && *fmt)
      {
      switch (*fmt)
         {
         case 'd': case 'D':
            {
            string token;
            while (*fmt == 'd' || *fmt == 'D')
               token.append (*fmt++);
            if (token.length() == 1)
               s += "dd";
            else
               {
               if (token.length() > 3)
                  token.remove (3, NPOS);
               s += token;
               }
            break;
            }

         case 'm': case 'M':
            {
            string token;
            while (*fmt == 'm' || *fmt == 'M')
               token.append (*fmt++);
            if (token.length() == 1)
               s += "mm";
            else
               {
               if (token.length() > 3)
                  token.remove (3, NPOS);
               s += token;
               }
            break;
            }

         case 'y': case 'Y':
            {
            string token;
            while (*fmt == 'y' || *fmt == 'Y')
               token.append (*fmt++);
            if (token.length() <= 2)
               s += "yy";
            else
               s += "yyyy";
            break;
            }

         case '\'':
         case '\"':
            {
            char delim = *fmt++;
            while (*fmt && *fmt != delim)
               {
               if (*fmt == '\\') // take next char literally
                  fmt++;
               s += *fmt++;
               }
            if (*fmt == delim)
               fmt++;
            break;
            }

         default:
            if (*fmt == '\\')
               fmt++;
            s += *fmt++;
            break;
         }
      }

   Format = s;
   TextLen = s.length() + 1;
   if (IsWindow())
      {
      HandleMessage(EM_LIMITTEXT, TextLen - 1);
      }

   Validator = new TGDateValidator (min, max, this, notNull);
   }


uint TGDateEdit::Transfer (void *buffer, TTransferDirection direction)
   {
   if (buffer)
      {
      switch (direction)
         {
         case tdSetData:
            *this = *static_cast<IntlDate *>(buffer);
            break;

         case tdGetData:
            *static_cast<IntlDate *>(buffer) = IntlDate(*this);
            break;
         }
      }

   return sizeof(IntlDate);
   }


void TGDateEdit::DrawCell (TDC &dc, TRect &rc, const void *p, TGrid::Align align)
   {
   if (p)
      {
      string s = static_cast<IntlDate *>(const_cast<void *>(p))->String (Format);
      TGEdit::DrawCell (dc, rc, s.c_str(), align);
      }
   else
      TGEdit::DrawCell (dc, rc, 0, align);
   }

// EOF

