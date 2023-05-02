/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// intldate.h                                                              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#ifndef INTLDATE_H
#define INTLDATE_H

#ifndef CLASSLIB__DATE_H
#include <classlib/date.h>
#endif


typedef enum DateConstant { idYesterday, idToday, idTomorrow } DateConstant;
typedef enum DateElement  { idDay, idMonth, idYear } DateElement;
typedef enum WeekDay  { Monday, Tuesday, Wednesday, Thursday,
                        Friday, Saturday, Sunday, InvalidDay } WeekDay;
typedef enum MonthType { January = 1, February, March, April, May, June,
                         July, August, September, October, November, December } MonthType;


class _GRIDCLASS IntlDate
   {
   protected:
      static string Picture;
      static short defaultCentury;
      static const short DaysBeforeMonth[13];

      char day;
      char month;
      short year;
      long  dayNumber;

      long CalcDayNumber ();
      IntlDate &CalcDMY ();

   public:
      IntlDate ();
      IntlDate (DateConstant org);
      IntlDate (const IntlDate &d);
      IntlDate (int j, int m, int a);
      IntlDate (long n);
      IntlDate (const char *str, const char *picture = 0, idLanguage l = idUndefLanguage);
      IntlDate (const string &str, const char *picture = 0, idLanguage l = idUndefLanguage);
      IntlDate (const char *str, const string &picture, idLanguage l = idUndefLanguage);
      IntlDate (const string &str, const string &picture, idLanguage l = idUndefLanguage);
      IntlDate (const TDate &d);

      IntlDate &operator += (long n);
      IntlDate &operator -= (long n);

      IntlDate &operator ++ ();
      IntlDate operator ++ (int);

      IntlDate &operator -- ();
      IntlDate operator -- (int);

      bool operator == (const IntlDate& b) const;
      bool operator == (long n) const;

      bool operator != (const IntlDate& b) const;
      bool operator != (long n) const;

      bool operator <= (const IntlDate& b) const;
      bool operator <= (long n) const;

      bool operator >= (const IntlDate& b) const;
      bool operator >= (long n) const;

      bool operator <  (const IntlDate& b) const;
      bool operator <  (long n) const;

      bool operator >  (const IntlDate& b) const;
      bool operator >  (long n) const;

      IntlDate &operator = (const IntlDate& d);
      IntlDate &operator = (const TDate &d);
      IntlDate &operator = (long n);

      friend _GRIDFUNCTION IntlDate operator + (const IntlDate &d, long n);

      friend _GRIDFUNCTION long operator - (const IntlDate &d, const IntlDate& b);
      friend _GRIDFUNCTION IntlDate operator - (const IntlDate &d, long n);

      operator long () const;
      operator TDate () const;

      // string operators use the default picture format held in Picture
      IntlDate &operator = (const string &s) { InterpretString (s); return *this; }
      IntlDate &operator = (const char *s) { InterpretString (s); return *this; }
      operator string () const { return String(); }
      operator const char * () const { return String().c_str(); }

      char DaysInMonth () const;
      IntlDate &Dec    (DateElement f = idDay, long n = 1);
      short DosDate    () const;
      WeekDay DoW      () const;
      int  DoY         () const;
      int  GetDay      () const { return day; }
      int  GetMonth    () const { return month; }
      int  GetYear     () const { return year; }
      IntlDate &Inc    (DateElement f = idDay, long n = 1);
      IntlDate &InterpretString  (const char *s, const char *strPict = 0, idLanguage l = idUndefLanguage);
		IntlDate &InterpretString  (const string &s, const char *strPict = 0, idLanguage l = idUndefLanguage);
		IntlDate &InterpretString  (const char *s, const string &strPict, idLanguage l = idUndefLanguage);
      IntlDate &InterpretString  (const string &s, const string &strPict, idLanguage l = idUndefLanguage);
		bool IsNull      () const;
      bool IsValid     () const;
      bool IsLeap      () const { return IsLeap (year); }
      bool IsLeap      (int y) const;
      bool IsValidMonth (int m) const;
      bool IsValidMonthDay (int d) const;
      bool IsValidWeekDay (int d) const;
		IntlDate &Set       (int n, DateElement f = idDay);
		IntlDate &Set       (int j, int m, int a = 0);
		string String (const char *picture = 0, idLanguage l = idUndefLanguage) const;
		string String (const string &picture, idLanguage l = idUndefLanguage) const;
		IntlDate &Today     ();
		IntlDate &Tomorrow  ();
		IntlDate &Yesterday ();
		int  WoY         (int firstWeekDay = int(Monday)) const;
      IntlDate &Zero      ();

      // defaults
      static string &GetPicture () { return Picture; }
		static void SetDefCentury (int century);
		static void SetPicture  (const char *p);
		static void SetPicture  (const string &p);
	};


ostream & _GRIDFUNCTION operator << (ostream &s, IntlDate &date);
istream & _GRIDFUNCTION operator >> (istream &s, IntlDate &date);

#endif

// EOF
