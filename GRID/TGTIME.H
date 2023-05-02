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


#ifndef TGTIME_H
#define TGTIME_H

#ifndef CLASSLIB_TIME_H
#include <classlib/time.h>
#endif

typedef enum TimeConstant { tNow } TimeConstant;
typedef enum TimeElement { tHour, tMinute, tSecond, tMilliSec } TimeElement;

class _GRIDCLASS TGTime
   {
   protected:
      static string Picture;

      ushort hour;
      ushort minute;
      ushort second;
      ushort millisec;
      short sign;
      double value;

      double ToTime ();
      TGTime &CalcHMS ();

   public:
      TGTime ();
      TGTime (TimeConstant);
      TGTime (const TGTime &t);
      TGTime (int h, uint m, uint s = 0, uint ms = 0);
      TGTime (double n);
      TGTime (const char   *f, const char   *picture = 0);
      TGTime (const string &f, const char   *picture = 0);
      TGTime (const char   *f, const string &picture);
      TGTime (const string &f, const string &picture);
      TGTime (const TTime &t);

      TGTime &operator += (double d);
      TGTime &operator -= (double d);
      TGTime &operator /= (double d);
      TGTime &operator *= (double d);

      bool operator == (double d);
      bool operator != (double d);
      bool operator <= (double d);
      bool operator >= (double d);
      bool operator <  (double d);
      bool operator >  (double d);

      bool operator == (const TGTime &d);
      bool operator != (const TGTime &d);
      bool operator <= (const TGTime &d);
      bool operator >= (const TGTime &d);
      bool operator <  (const TGTime &d);
      bool operator >  (const TGTime &d);

      TGTime &operator = (const TTime &t);
      TGTime &operator = (double d);
      TGTime &operator = (const TGTime &t);

      friend _GRIDFUNCTION TGTime operator + (const TGTime &t, double n);
      friend _GRIDFUNCTION TGTime operator - (const TGTime &t, double n);
      friend _GRIDFUNCTION double operator - (const TGTime &t1, const TGTime &t2);
      friend _GRIDFUNCTION TGTime operator * (const TGTime &t, double n);
      friend _GRIDFUNCTION TGTime operator / (const TGTime &t, double n);

      operator double () const;
      operator TTime () const;

      // string operators use the default picture format held in Picture
      TGTime &operator = (const string &s) { InterpretString (s); return *this; }
      TGTime &operator = (const char *s) { InterpretString (s); return *this; }
      operator string () const { return String(); }
      operator const char * () const { return String().c_str(); }

      TGTime &Dec             (TimeElement t, int n = 1);
      short DosTime           () const;
      ushort  GetHour         () { return hour; }
      ushort  GetMillisec     () { return millisec; }
      unsigned char GetMinute () { return minute; }
      unsigned char GetSecond () { return second; }

      TGTime &Inc             (TimeElement t, int n = 1);
      TGTime &InterpretString (const char *string,   const char *fmt = 0);
      TGTime &InterpretString (const string &string, const char *fmt = 0);
      TGTime &InterpretString (const char *string,   const string &fmt);
      TGTime &InterpretString (const string &string, const string &fmt);
      bool    IsValid         () const;
      TGTime &Now             ();
      TGTime &Set             (uint n, TimeElement f);
      TGTime &Set             (int h, uint m, uint s = 0, uint ms = 0);
      string  String          (const char *fmt = 0) const;
      string  String          (const string &fmt) const;

      static string &GetPicture () { return Picture; }
      static void   SetPicture  (const char *p);
      static void   SetPicture  (const string &p);
   };

ostream & _GRIDFUNCTION operator << (ostream &s, TGTime &time);
istream & _GRIDFUNCTION operator >> (istream &s, TGTime &time);

#endif

// EOF
