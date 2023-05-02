/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// timeedit.h                                                              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "griddll.h"
#pragma hdrstop

DEFINE_RESPONSE_TABLE1 (TGTimeEdit, TGDateTimeEdit)
   EV_WM_KILLFOCUS,
END_RESPONSE_TABLE;


// TGTimeValidator //////////////////////////////////////////////////////////
TGTimeValidator::TGTimeValidator (const TGTime &min, const TGTime &max,
                                  TGTimeEdit *own)
               : TGDateTimeValidator ("HhMmSs")
   {
   Min = min;
   Max = max;
   owner = own;
   }


void TGTimeValidator::Error(TWindow* parent)
   {
   string time = language.LoadString (TIME_TITLE);
   const char *d = time.c_str();
   #define FLAGS MB_ICONEXCLAMATION | MB_OK | MB_TASKMODAL

   owner->SuspendValidator ();

   switch (errcode)
      {
      case TIME_OUT_OF_RANGE:
         {
         string mess = language.LoadString (TIME_OUTOFRANGE);
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
         parent->MessageBox (language.LoadString (TIME_INCORRECT).c_str(),
                            d, FLAGS);
         break;
      }

   owner->RestoreValidator ();
   }


bool TGTimeValidator::IsValid (const char * s)
   {
   errcode = TIME_OK;

   if (string(s).is_null () && Min == 0)
      return true;

   TGTime val (s, owner->Format.c_str());
   if (!val.IsValid ())
      {
      errcode = TIME_FORMAT;
      return false;
      }

   if (val < Min || val > Max)
      {
      errcode = TIME_OUT_OF_RANGE;
      return false;
      }

   return true;
   }



TGTimeEdit::TGTimeEdit (TWindow *p, uint resid, const char *fmt,
                        const TGTime &min, const TGTime &max, TModule *module)
          : TGDateTimeEdit (p, resid, fmt, module)
   {
   SetFilter (min, max, fmt);
   }


TGTimeEdit::TGTimeEdit (TWindow *p, uint resid, const string &fmt,
                        const TGTime &min, const TGTime &max, TModule *module)
          : TGDateTimeEdit (p, resid, fmt.c_str(), module)
   {
   SetFilter (min, max, fmt);
   }


TGTimeEdit::TGTimeEdit (TWindow *p, uint id, int x, int y, int w, int h,
                        const char *fmt, const TGTime &min, const TGTime &max,
                        TModule *module)
          : TGDateTimeEdit (p, id, x, y, w, h, fmt, module)
   {
   SetFilter (min, max, fmt);
   }


TGTimeEdit::TGTimeEdit (TWindow *p, uint id, int x, int y, int w, int h,
                        const string &fmt, const TGTime &min,
                        const TGTime &max, TModule *module)
          : TGDateTimeEdit (p, id, x, y, w, h, fmt.c_str(), module)
   {
   SetFilter (min, max, fmt);
   }


void TGTimeEdit::SetMin (const TGTime &d)
   {
   TGTimeValidator *dv = dynamic_cast<TGTimeValidator *>(Validator);
   if (dv)
      {
      dv->SetMin (d);
      if (dv->GetMax() < d)
         dv->SetMax (d);
      }
   }


void TGTimeEdit::SetMax (const TGTime &d)
   {
   TGTimeValidator *dv = dynamic_cast<TGTimeValidator *>(Validator);
   if (dv)
      {
      dv->SetMax (d);
      if (dv->GetMin() > d)
         dv->SetMin (d);
      }
   }


void TGTimeEdit::EvKillFocus (HWND h)
   {
   TGEdit::EvKillFocus (h);
   SetText (TGTime (*this).String(Format));
   }


TGTime TGTimeEdit::operator = (const TGTime &d)
   {
   TGTime time (d);
   SetText (time.String (Format));
   return time;
   }


TGTimeEdit::operator TGTime ()
   {
   int l = TextLen;
   if (l > 63)
      l = 63;
   char buffer[64];
   GetText (buffer, l+1);
   TGTime time;
   time.InterpretString (buffer, Format);
   return time;
   }


void TGTimeEdit::SetText (const char far *s)
   {
   TGTime time;
   time.InterpretString (s, Format);
   TGEdit::SetText (s);
   }


void TGTimeEdit::SetText (const string &s)
	{
   TGTime time;
	time.InterpretString (s, Format);
	TGEdit::SetText (s.c_str());
	}


void TGTimeEdit::SetFilter (const TGTime &min, const TGTime &max,
									 const string &fmt)
	{
	SetFilter (min, max, fmt.c_str());
	}


void TGTimeEdit::SetFilter (const TGTime &min, const TGTime &max,
                            const char *fmt)
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
         case 'H':
            {
            string token;
            while (*fmt == 'H')
               token.append (*fmt++);
            s += token;
            break;
            }

         case 'M':
            while (*fmt == 'M')
               *fmt++;
            s += "MM";
            break;

         case 'S':
            while (*fmt == 'S')
               *fmt++;
            s += "SS";
            break;

         case 's':
            {
            string token;
            while (*fmt == 's')
               token.append (*fmt++);
            if (token.length () <= 3)
               s += token;
            else
               s += "sss";
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

   Validator = new TGTimeValidator (min, max, this);
   }


void TGTimeEdit::DrawCell (TDC &dc, TRect &rc, const void *p, TGrid::Align align)
   {
   if (p)
      {
      string s = static_cast<TGTime *>(const_cast<void *>(p))->String (Format);
      TGEdit::DrawCell (dc, rc, s.c_str(), align);
      }
   else
      TGEdit::DrawCell (dc, rc, 0, align);
   }


uint TGTimeEdit::Transfer (void *buffer, TTransferDirection direction)
   {
   if (buffer)
      {
      switch (direction)
         {
         case tdSetData:
            *this = *static_cast<TGTime *>(buffer);
            break;

         case tdGetData:
            *static_cast<TGTime *>(buffer) = TGTime (*this);
            break;
         }
      }

   return sizeof(TGTime);
   }

// EOF

