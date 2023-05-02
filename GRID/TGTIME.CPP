/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// tgtime.h                                                                //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "griddll.h"
#pragma hdrstop
#include <time.h>
#include <math.h>

string TGTime::Picture;

TGTime::TGTime ()
   {
   Set (0,0);
   }


TGTime::TGTime (TimeConstant)
   {
   Now ();
   }


TGTime::TGTime (const TGTime &t)
   {
   hour = t.hour;
   minute = t.minute;
   second = t.second;
   millisec = t.millisec;
   sign = t.sign;
   value = t.value;
   }


TGTime::TGTime (int h, uint m, uint s, uint ms)
   {
   Set (h, m, s, ms);
   }


TGTime::TGTime (double n)
   {
   value = n;
   CalcHMS ();
   }


TGTime::TGTime (const string &s, const string &p)
   {
   InterpretString (s, p);
   }


TGTime::TGTime (const string &s, const char *p)
   {
   InterpretString (s, p);
   }


TGTime::TGTime (const char *s, const string &p)
   {
   InterpretString (s, p);
   }


TGTime::TGTime (const char *s, const char *p)
   {
   InterpretString (s, p);
   }


TGTime::TGTime (const TTime &t)
   {
   Set (t.Hour(), t.Minute(), t.Second());
   }


double TGTime::ToTime ()
   {
   value = hour*3600.0 + minute*60.0 + second + millisec/1000.0;
   if (sign < 0)
      value = -value;
   return value;
   }


TGTime &TGTime::CalcHMS ()
   {
   if (value < 0)
      {
      sign = -1;
      value = -value;
      }
   else
      {
      sign = 1;
      }
   value = floor (value*1000.0+0.5) / 1000.0; 
   long sec = floor (value);
   hour = ushort(sec / 3600);
   minute = (unsigned char)((sec/60) % 60);
   second = (unsigned char)(sec%60);
   millisec = (ushort)floor ((value-sec)*1000.0 + 0.5);
   return *this;
   }


TGTime &TGTime::operator += (double d)
   {
   value += d;
   return CalcHMS ();
   }


TGTime &TGTime::operator -= (double d)
   {
   value -= d;
   return CalcHMS ();
   }


TGTime &TGTime::operator /= (double d)
   {
   value /= d;
   return CalcHMS ();
   }



TGTime &TGTime::operator *= (double d)
   {
   value *= d;
   return CalcHMS ();
   }


bool TGTime::operator == (double d)
   {
   return value == d;
   }


bool TGTime::operator != (double d)
   {
   return value != d;
   }


bool TGTime::operator <= (double d)
   {
   return value <= d;
   }


bool TGTime::operator >= (double d)
   {
   return value >= d;
   }


bool TGTime::operator < (double d)
   {
   return value < d;
   }


bool TGTime::operator > (double d)
   {
   return value > d;
   }



bool TGTime::operator == (const TGTime &d)
   {
   return value == d.value;
   }


bool TGTime::operator != (const TGTime &d)
   {
   return value != d.value;
   }


bool TGTime::operator <= (const TGTime &d)
   {
   return value <= d.value;
   }


bool TGTime::operator >= (const TGTime &d)
   {
   return value >= d.value;
   }


bool TGTime::operator < (const TGTime &d)
   {
   return value < d.value;
   }


bool TGTime::operator > (const TGTime &d)
   {
   return value > d.value;
   }



TGTime &TGTime::operator = (const TTime &t)
   {
   return Set (t.Hour(), t.Minute(), t.Second());
   }


TGTime &TGTime::operator = (const TGTime &t)
   {
   hour = t.hour;
   minute = t.minute;
   second = t.second;
   millisec = t.millisec;
   sign = t.sign;
   value = t.value;
   return *this;
   }


TGTime &TGTime::operator = (double d)
   {
   value = d;
   return CalcHMS ();
   }


TGTime operator + (const TGTime &t, double n)
   {
   return TGTime (t.value+n);
   }


TGTime operator - (const TGTime &t, double n)
   {
   return TGTime (t.value-n);
   }


double operator - (const TGTime &t1, const TGTime &t2)
   {
   return TGTime (t1.value-t2.value);
   }


TGTime operator * (const TGTime &t, double n)
   {
   return TGTime (t.value*n);
   }


TGTime operator / (const TGTime &t, double n)
   {
   return TGTime (t.value/n);
   }


TGTime::operator double () const
   {
   return value;
   }


TGTime::operator TTime () const
   {
   return TTime (hour, minute, second);
   }


TGTime &TGTime::Dec (TimeElement t, int n)
   {
   switch (t)
      {
      case tHour:
         value -= n*3600.0;
         break;

      case tMinute:
         value -= n*60.0;
         break;

      case tSecond:
         value -= n;
         break;

      case tMilliSec:
         value -= n/1000.0;
         break;
      }

   return CalcHMS ();
   }


TGTime &TGTime::Inc (TimeElement t, int n)
   {
   switch (t)
      {
      case tHour:
         value += n*3600.0;
         break;

      case tMinute:
         value += n*60.0;
         break;

      case tSecond:
         value += n;
         break;

      case tMilliSec:
         value += n/1000.0;
         break;
      }

   return CalcHMS ();
   }


short TGTime::DosTime () const
   {
   return static_cast<short>( (second>>1) | (minute<<5) | (hour<<11) );
   }


TGTime &TGTime::InterpretString (const char *s, const string &fmt)
   {
   return InterpretString (s, fmt.c_str());
   }


TGTime &TGTime::InterpretString (const string &s, const string &fmt)
   {
   return InterpretString (s.c_str(), fmt.c_str());
   }


TGTime &TGTime::InterpretString (const string &s, const char *fmt)
   {
   return InterpretString (s.c_str(), fmt);
   }


TGTime &TGTime::InterpretString (const char *s, const char *fmt)
   {
   Set (0,0);

   if (fmt == 0)
      {
      Picture = "HH:MM:SS";
      fmt = Picture.c_str ();
      }

   sign = 1;
   if (*s == '-')
      {
      sign = -1;
      s++;
      }

   while (*fmt)
      {
      switch (*fmt)
         {
         case 'H':
            {
            int len = 0;
            while (*fmt == 'H')
               len++, fmt++;
            hour = (ushort)atoi (string (s, 0, len).c_str());
            while (*s && len-- && isdigit (*s))
               s++;
            break;
            }

         case 'h':
            {
            int len = 0;
            while (*fmt == 'h')
               len++, fmt++;
            string tmp (s, 0, len);
            while (tmp.length() < len)
               tmp += "0";
            long double val = atol (tmp.c_str());
            val /= pow10l (len);
            val *= 60.0;
            minute = floorl (val);
            val -= minute;
            val *= 60.0;
            second = floorl (val);
            val -= second;
            val *= 1000.0;
            millisec = floorl (val + 0.5);
            while (*s && len-- && isdigit (*s))
               s++;
            break;
            }

         case 'M':
            {
            int len = 0;
            while (*fmt == 'M')
               len++, fmt++;
            minute = (unsigned char)atoi (string (s, 0, len).c_str());
            if (minute > 60)
               {
               hour += ushort(minute / 60);
               minute %= ushort(60);
               }
            while (*s && len-- && isdigit (*s))
               s++;
            break;
            }

         case 'm':
            {
            int len = 0;
            while (*fmt == 'm')
               len++, fmt++;
            string tmp (s, 0, len);
            while (tmp.length() < len)
               tmp += "0";
            long double val = atol (tmp.c_str());
            val /= pow10l (len);
            val *= 60.0;
            second = floorl (val);
            val -= second;
            val *= 1000.0;
            millisec = floorl (val + 0.5);
            while (*s && len-- && isdigit (*s))
               s++;
            break;
            }

         case 'S':
            {
            int len = 0;
            while (*fmt == 'S')
               len++, fmt++;
            second = (unsigned char)atoi (string (s, 0, len).c_str());
            if (second >= 60)
               {
               minute += ushort(second / 60);
               second %= ushort(60);
               if (minute >= 60)
                  {
                  hour += ushort(minute / 60);
                  minute %= ushort(60);
                  }
               }
            while (*s && len-- && isdigit (*s))
               s++;
            break;
            }

         case 's':
            {
            int len = 0;
            while (*fmt == 's' && len < 3)
               len++, fmt++;
            string tmp (s, 0, len);
            while (tmp.length() < len)
               tmp += "0";
            millisec = ushort(atoi (tmp.c_str()));
            while (*s && len-- && isdigit (*s))
               s++;
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
               s++;
               fmt++;
               }
            if (*fmt == delim)
               fmt++;
            break;
            }

         default:
            if (*fmt == '\\')
               fmt++;
            s++;
            fmt++;
            break;
         }
      }

   if (IsValid())
      ToTime ();

   return *this;
   }


bool TGTime::IsValid () const
   {
   if (minute > 59 || second > 59 || millisec > 999)
      return false;
   else
      return true;
   }


TGTime &TGTime::Now ()
   {
   #ifdef __WIN32__
   if ((GetVersion() & 0x80000000L) == 0)
      {
      SYSTEMTIME st;
      GetLocalTime (&st);
      Set (st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
      }
   else
   #endif
      {
      long t;
      time (&t);
      struct tm *ut = localtime (&t);
      Set (ut->tm_hour, ut->tm_min, ut->tm_sec);
      }
   return *this;
   }


TGTime &TGTime::Set (uint n, TimeElement f)
   {
   switch (f)
      {
      case tHour:
         hour = (ushort)n;
         break;

      case tMinute:
         if (n < 60)
            minute = (unsigned char)n;
         break;

      case tSecond:
         if (n < 60)
            second = (unsigned char)n;
         break;

      case tMilliSec:
         if (n < 1000)
            millisec = (ushort)n;
         break;
      }
   ToTime ();
   return *this;
   }


TGTime &TGTime::Set (int h, uint m, uint s, uint ms)
   {
   if (h < 0)
      {
      sign = -1;
      h = -h;
      }
   else
      sign = 1;
   hour = (ushort)h;
   minute = (unsigned char)m;
   second = (unsigned char)s;
   millisec = (ushort)ms;
   ToTime ();
   return *this;
   }


string TGTime::String (const string &fmt) const
   {
   return String (fmt.c_str());
   }


string TGTime::String (const char *fmt) const
   {
   if (fmt == 0)
      {
      Picture = "HH:MM:SS";
      fmt = Picture.c_str();
      }

   if (! IsValid ())
      {
      return string();
      }

   char buffer[128];
   *buffer = 0;
   const char *p = fmt;
   char *b = buffer;

   while (*p)
      {
      switch (*p)
         {
         case 'H':
            {
            int len = 0;
            while (*p == 'H')
               len++, p++;
            if (len == 1)
               sprintf (b, "%d", hour);
            else
               sprintf (b, "%0*d", len, hour);
            b += strlen(b);
            break;
            }

         case 'h':
            {
            int len = 0;
            while (*p == 'h')
               len++, p++;
            double val = (minute / 60.0 + second/3600.0 + millisec / 3600.0 / 1000.0);
            val *= pow10(len);
            long digits = floor (val+0.5);
            sprintf (b, "%0*ld", len, digits);
            b[len] = 0;
            b += len;
            break;
            }

         case 'M':
            {
            int len = 0;
            while (*p == 'M')
               len++, p++;
            if (len == 1)
               sprintf (b, "%d", minute);
            else if (len == 2)
               sprintf (b, "%02d", minute);
            else
               {
               long n = hour*60+minute;
               sprintf (b, "%0*ld", len, n);
               }
            b += strlen(b);
            break;
            }

         case 'm':
            {
            int len = 0;
            while (*p == 'm')
               len++, p++;
            double val = (second/60.0 + millisec / 60.0 / 1000.0);
            val *= pow10(len);
            long digits = floor (val+0.5);
            sprintf (b, "%0*ld", len, digits);
            b[len] = 0;
            b += len;
            break;
            }

         case 'S':
            {
            int len = 0;
            while (*p == 'S')
               len++, p++;
            if (len == 1)
               sprintf (b, "%d", second);
            if (len == 2)
               sprintf (b, "%02d", second);
            else
               {
               long n = hour*3600L + minute*60L + second;
               sprintf (b, "%0*ld", len, n);
               }
            b += strlen(b);
            break;
            }

         case 's':
            {
            int len = 0;
            while (*p == 's')
               len++, p++;
            sprintf (b, "%03d", millisec);
            b[len] = 0;
            b += len;
            break;
            }

         // literals --------------------------------------------------------
         case '\'':
         case '\"':
            {
            char delim = *p++;
            while (*p && *p != delim)
               {
               if (*p == '\\') // take next char literally
                  p++;
               *b++ = *p++;
               }
            if (*p == delim)
               p++;
            break;
            }

         default:
            if (*p == '\\') // take next char literally
               p++;
            *b++ = *p++;
            break;
         }
      }

   *b = 0;

   return string(buffer);
   }


void TGTime::SetPicture (const string &p)
   {
	Picture = p;
	}


void TGTime::SetPicture (const char *p)
	{
	if (p)
		Picture = p;
	else
		Picture = "";
	}


// stream support (uses default formats) ////////////////////////////////////
ostream &operator << (ostream &s, TGTime &time)
   {
   s << time.String ();
   return s;
   }


istream &operator >> (istream &s, TGTime &time)
   {
   string str;
   s >> str;
   time = str;
   return s;
   }

// EOF
