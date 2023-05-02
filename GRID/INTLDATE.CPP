/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// intldate.cpp                                                            //
// General purpose date class                                              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "griddll.h"
#pragma hdrstop
#include <intldate.h>
#include <time.h>

// static variables /////////////////////////////////////////////////////////
string IntlDate::Picture;
short IntlDate::defaultCentury = 1900;
const short IntlDate::DaysBeforeMonth[13] =
           { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };

// SetDefCentury ////////////////////////////////////////////////////////////
void IntlDate::SetDefCentury (int century)
   {
   defaultCentury = short(century*100);
   }


// constructors /////////////////////////////////////////////////////////////
IntlDate::IntlDate ()
   {
   Zero ();
   }


IntlDate::IntlDate (DateConstant d)
   {
   Today ();
   if (d == idTomorrow)
      Inc (idDay);
   else if (d == idYesterday)
      Dec (idDay);
   }


IntlDate::IntlDate (const IntlDate &d)
   {
   day = d.day;
   month = d.month;
   year = d.year;
   dayNumber = d.dayNumber;
   }


IntlDate::IntlDate (int j, int m, int a)
   {
   Set (j, m, a);
   }


IntlDate::IntlDate (long n)
   {
   *this = n;
   }


IntlDate::IntlDate (const char *fmt, const char *picture, idLanguage l)
	{
	InterpretString (fmt, picture, l);
	}


IntlDate::IntlDate (const string &fmt, const char *picture, idLanguage l)
	{
	InterpretString (fmt, picture, l);
	}


IntlDate::IntlDate (const char *fmt, const string &picture, idLanguage l)
	{
	InterpretString (fmt, picture, l);
	}


IntlDate::IntlDate (const string &fmt, const string &picture, idLanguage l)
	{
	InterpretString (fmt, picture, l);
	}


IntlDate::IntlDate (const TDate &d)
   {
   *this = d;
   }


// operators ////////////////////////////////////////////////////////////////
IntlDate &IntlDate::operator += (long n)
   {
   return Inc (idDay, n);
   }


IntlDate &IntlDate::operator -= (long n)
   {
   return Dec (idDay, n);
   }


IntlDate &IntlDate::operator ++ ()
   {
   return Inc (idDay);
   }


IntlDate IntlDate::operator ++ (int)
   {
   IntlDate d (*this);
   Inc (idDay);
   return d;
   }


IntlDate &IntlDate::operator -- ()
   {
   return Dec (idDay);
   }


IntlDate IntlDate::operator -- (int)
   {
   IntlDate d (*this);
   Dec (idDay);
   return d;
   }


bool IntlDate::operator == (const IntlDate& b) const
   {
   return dayNumber == b.dayNumber;
   }


bool IntlDate::operator == (long n) const
   {
   return dayNumber == n;
   }


bool IntlDate::operator != (const IntlDate& b) const
   {
   return !(*this == b);
   }


bool IntlDate::operator != (long n) const
   {
   return !(*this == n);
   }


bool IntlDate::operator <= (const IntlDate& b) const
   {
   return dayNumber <= b.dayNumber;
   }


bool IntlDate::operator <= (long n) const
   {
   return dayNumber <= n;
   }


bool IntlDate::operator >  (const IntlDate& b) const
   {
   return !(*this <= b);
   }


bool IntlDate::operator >  (long n) const
   {
   return !(*this <= n);
   }


bool IntlDate::operator >= (const IntlDate& b) const
   {
   return dayNumber >= b.dayNumber;
   }


bool IntlDate::operator >= (long n) const
   {
   return dayNumber >= n;
   }


bool IntlDate::operator <  (const IntlDate& b) const
   {
   return !(*this >= b);
   }


bool IntlDate::operator <  (long n) const
   {
   return !(*this >= n);
   }


IntlDate &IntlDate::operator = (long n)
   {
   if (n <= 0)
      Zero ();
   else
      {
      dayNumber = n;
      CalcDMY ();
      }
   return *this;
   }


IntlDate &IntlDate::operator = (const IntlDate& d)
   {
   day = d.day;
   month = d.month;
   year = d.year;
   dayNumber = d.dayNumber;
   return *this;
   }


IntlDate &IntlDate::operator = (const TDate &d)
   {
   day = char(d.DayOfMonth());
   month = char (d.Month());
   year = short(d.Year ());
   CalcDayNumber ();
   return *this;
   }

// friend operators /////////////////////////////////////////////////////////
IntlDate operator + (const IntlDate &d, long n)
   {
   return IntlDate (d.dayNumber + n);
   }


long operator - (const IntlDate &d, const IntlDate& b)
   {
   return d.dayNumber-b.dayNumber;
   }


IntlDate operator - (const IntlDate &d, long n)
   {
   return IntlDate (d.dayNumber-n);
   }


IntlDate::operator long () const
   {
   return dayNumber;
   }


IntlDate::operator TDate () const
   {
   return TDate (day, month, year);
   }


// CalcDMY //////////////////////////////////////////////////////////////////
#define ZERODATE (-1)   
IntlDate &IntlDate::CalcDMY ()
   {
   long n = dayNumber - ZERODATE;

   // there are 365.2425 days / year on average
   // use int math instead of dividing by 365.2425
   int y = (year = short (((n * 400L) / 146097L ) + 1)) - 1;
   int d = int ((n - (y*365L + (y>>2) - y/100L + y/400L)));

   if (d <= 0)
      {
      year--;
      y = year-1;
      d = (int) (n - (y*365L + (y>>2) - y/100L + y/400L));
      }

   int diy =  365 + IsLeap ();

   if (d > diy)
      {
      year++;
      d -= diy;
      }

   int isleap = (d <= 59 ? 0 : IsLeap ());
   for (int i = January; i <= December; i++)
      {
      if (d <= DaysBeforeMonth[i] + isleap)
         {
         month = char(i);
	      day = (char)(d - DaysBeforeMonth[--i] - (month <= 2 ? 0 : IsLeap ()));
         return *this;
         }
      }
   return Zero ();
   }


// CalcDayNumber () /////////////////////////////////////////////////////////
long IntlDate::CalcDayNumber ()
   {
   if (! IsValid ())
      *this = 0L;
   else
      {
      int y = year-1;
      dayNumber = y*365L + (y>>2) - y/100L + y/400L + DoY () + ZERODATE;
      }
   return dayNumber;
   }


// DaysInMonth () ///////////////////////////////////////////////////////////
char IntlDate::DaysInMonth () const
   {
   return (char)( DaysBeforeMonth[month]
          - DaysBeforeMonth[month-1]
          + (month != February ? 0 : IsLeap ()));
   }


// Dec (DateElement f, int n) ////////////////////////////////////////////
IntlDate &IntlDate::Dec (DateElement f, long n)
   {
   if (IsNull () || n == 0)
      return *this;

   if (n < 0)
      return Inc (f, -n);

   switch (f)
      {
      case idDay:
         dayNumber -= n;
         CalcDMY ();
         break;

      case idMonth:
         {
         if (n >= 12)
            {
            year -= short(n/12);
            n %= 12;
            }
         month -= char(n);   // n is guaranteed to be in the range 0..11
         if (month < January)
            {
            month += char(12);
            year--;
            }

         int dim = DaysInMonth ();
         if (day > dim)  // in case original day was Feb 29
            day = char(dim);
         CalcDayNumber();
         break;
         }

      case idYear:
         {
         year -= short(n);
         int dim = DaysInMonth ();
         if (day > dim)  // in case original day was Feb 29
            day = char(dim);
         CalcDayNumber();
         break;
         }
      }

   return *this;
   }


// DoW () ///////////////////////////////////////////////////////////////////
WeekDay IntlDate::DoW () const
   {
   return (WeekDay)(dayNumber % 7);
   }


// DoY () ///////////////////////////////////////////////////////////////////
int IntlDate::DoY () const
   {
   return (int)(DaysBeforeMonth[month-1] + day +
               (month <= February ? 0 : IsLeap ()));
   }


// Inc (DateElement f, long n) ///////////////////////////////////////////
IntlDate &IntlDate::Inc (DateElement f, long n)
   {
   if (IsNull () || n == 0)
      return *this;

   if (n < 0)
      return Dec (f, -n);

   switch (f)
      {
      case idDay:
         dayNumber += n;
         CalcDMY ();
         break;

      case idMonth:
         {
         if (n >= 12)
            {
            year += short(n/12);
            n %= 12;
            }
         month += (char)n;   // n is guaranteed to be in the range 0..11
         if (month > December)
            {
            year ++;
            month -= (char)12;
            }
         int dim = DaysInMonth ();
         if (day > dim)  // in case original day was Feb 29
            day = char(dim);
         CalcDayNumber();
         break;
         }

      case idYear:
         {
         year += short(n);
         int dim = DaysInMonth ();
         if (day > dim)  // in case original day was Feb 29
            day = char(dim);
         CalcDayNumber();
         break;
         }
      }

   return *this;
   }


/////////////////////////////////////////////////////////////////////////////
// InterpretString (const char *s, const char *fmt, idLanguage lan)          //
// InterpretString (const string &s, const char *fmt, idLanguage lan)        //
// InterpretString (const char *s, const string &fmt, idLanguage lan)        //
// InterpretString (const string &s, const string &fmt, idLanguage lan)      //
// s holds the string to interpret                                         //
// fmt holds the string format                                             //
// lan is the language to use                                              //
/////////////////////////////////////////////////////////////////////////////
IntlDate &IntlDate::InterpretString (const string &s, const char *fmt, idLanguage lan)
	{
	return InterpretString (s.c_str(), fmt, lan);
	}


IntlDate &IntlDate::InterpretString (const char *s, const string &fmt, idLanguage lan)
	{
	return InterpretString (s, fmt.c_str(), lan);
	}


IntlDate &IntlDate::InterpretString (const string &s, const string &fmt, idLanguage lan)
	{
	return InterpretString (s.c_str(), fmt.c_str(), lan);
	}


IntlDate &IntlDate::InterpretString (const char *s, const char *fmt, idLanguage lan)
	{
   Zero();

   if (fmt == 0)
      {
      language.GetSystemLanguageInfo ();
      Picture = language.shortDatePicture;
      fmt = Picture.c_str ();
      }

   bool hasDay = false;
   while (*fmt)
      {
      switch (*fmt)
         {
         case 'D': case 'd':
            {
            int len = 0;
            while (*fmt == 'd' || *fmt == 'D')
               len++, fmt++;
            if (len <= 2) // if len > 2, we have a week day (eg Monday)
               {
               day = (char)atoi (string (s, 0, 2).c_str());
               hasDay = true;
               }
            while (*s && len-- && isdigit (*s))
               s++;
            break;
            }

         case 'M': case 'm':
            {
            int len = 0;
            while (*fmt == 'm' || *fmt == 'M')
               len++, fmt++;
            switch (len)
               {
               case 1:
               case 2:
                  {
                  string s1 (s, 0, 2);
                  month = (char)atoi (s1.c_str());
                  len = s1.length ();
                  break;
                  }
               case 3:
                  {
                  string s1 (s, 0, 3);
                  month = (char)language.ShortMonthIndex (s1, lan);
                  len = s1.length ();
                  break;
                  }
               default:
                  {
                  const char *p = s;
                  int n = 0;
                  while (p && *p && IsCharAlpha (*p))
                     n++;
                  string s1 (s, 0, n);
                  month = (char)language.LongMonthIndex (s1, lan);
                  len = s1.length ();
                  }
               }
            while (*s && len-- && isdigit (*s))
               s++;
            break;
            }

         case 'Y': case 'y':
            {
            int len = 0;
            while (*fmt == 'y' || *fmt == 'Y')
               len++, fmt++;
            if (len > 2)
               {
               string s1 (s, 0, 4);
               year = short(atoi (s1.c_str()));
               len = 4;
               }
            else
               {
               string s1 (s, 0, 2);
               year = short(atoi (s1.c_str()));
               len = 2;
               }
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

   // if format string does not contain day, set a default day = 1
   if (IsValidMonth (month) && !hasDay)
      day = 1;

   // defaults: current month and current year
   if (day && hasDay)
      {
      IntlDate Today (idToday);
      if (month == 0)
         month = Today.month;
      if (year == 0)
         year = Today.year;
      if (year < 100)
         year += defaultCentury;
      }

   if (IsValid())
      CalcDayNumber();
   else if (IsNull ())
      Zero ();

   return *this;
   }


// IsNull () ////////////////////////////////////////////////////////////////
bool IntlDate::IsNull () const
   {
   return (year == 0 && month == 0 && day == 0) ? true : false;
   }


// IsValid... () ////////////////////////////////////////////////////////////
bool IntlDate::IsValid () const
   {
   return (year <= 0 || !IsValidMonth(month) || !IsValidMonthDay (day)) ?
          false : true;
   }

bool IntlDate::IsValidMonth (int m) const
   {
   return (m >= January && m <= December) ? true : false;
   }

bool IntlDate::IsValidMonthDay (int d) const
   {
   return (d >= 1 && d <= DaysInMonth()) ? true : false;
   }

bool IntlDate::IsValidWeekDay (int d) const
   {
   return (d >= Monday && d <= Sunday) ? true : false;
   }

// IsLeap (int y) ///////////////////////////////////////////////////////////
bool IntlDate::IsLeap (int y) const
   {
   return (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)) ?  true : false;
   }


// Set (int n, DateElement f) ////////////////////////////////////////////
IntlDate &IntlDate::Set (int n, DateElement f)
   {
   switch (f)
      {
      case idDay:
         if (IsValidMonthDay (n))
            day = (char)n;
         break;

      case idMonth:
         if (IsValidMonth (n))
            month = (char)n;
         break;

      case idYear:
         year = short(n);
         break;
      }

   if (day > DaysInMonth ())
      day = DaysInMonth ();
   CalcDayNumber();
   return *this;
   }


// Set (int j, int m, int a) ////////////////////////////////////////////////
IntlDate &IntlDate::Set (int j, int m, int a)
   {
   if (a == 0)
      Today ();
   else if (a < 100)
      a += defaultCentury;
   day = char(j);
   month = char(m);
   year = short(a);
   CalcDayNumber();
   return *this;
   }


// String (char *fmt) ///////////////////////////////////////////////////////
// creates a date string
string IntlDate::String (const string &picture, idLanguage l) const
	{
   return String (picture.c_str(), l);
	}


string IntlDate::String (const char *picture, idLanguage l) const
   {
   if (picture == 0)
      {
      language.GetSystemLanguageInfo ();
      Picture = language.shortDatePicture;
      picture = Picture.c_str();
      }

   if (! IsValid ())
      {
      string s = "";
      return s;
      }

   char buffer[128];
   char *b = buffer;
   *b = 0;
   const char *p = picture;

   while (*p)
      {
      switch (*p)
         {
         case 'W': case 'w':    // week of year
            {
            int len = 0;
            while (*p == 'w' || *p == 'W')
               len++, p++;
            switch (len)
               {
               case 1:
                  sprintf (b, "%d", WoY());
                  break;
               default:
                  sprintf (b, "%02d", WoY());
                  break;
               }
            b += strlen(b);
            break;
            }

         case 'Q': case 'q':    // quarter
            {
            while (*p == 'q' || *p == 'Q')
               p++;
            sprintf (b, "%d", (month+2)/3);
            b += strlen(b);
            break;
            }

         case 'D': case 'd':    // day of month
            {
            // get token
            string token;
            int len = 0;
            while (*p == 'd' || *p == 'D')
               token.append (*p++), len++;
            switch (len)
               {
               case 1:
                  sprintf (b, "%d", day);
                  break;
               case 2:
                  sprintf (b, "%02d", day);
                  break;
               case 3:
                  strcpy (b, language.ShortDay(DoW(), l).c_str());
                  break;
               default:
                  strcpy (b, language.LongDay(DoW(), l).c_str());
                  break;
               }
            if (len >= 3)
               {
               if (token[1] == 'D')
                  Upper (b);
               else if (token[0] == 'D')
                  UpperBuff (b, 1);
               }
            b += strlen(b);
            break;
            }

         case 's': case 'S':    // day of month suffix
            {
            string suffix = language.DaySuffix (day, l);
            if (!suffix.is_null())
               {
               strcat (b, suffix.c_str());
               b += strlen (b);
               }
            while (*p == 's' || *p == 'S')
               p++;
            break;
            }

         case 'M': case 'm':   // month
            {
            // get token
            string token;
            int len = 0;
            while (*p == 'm' || *p == 'M')
               token.append (*p++), len++;
            switch (len)
               {
               case 1:
                  sprintf (b, "%d", month);
                  break;
               case 2:
                  sprintf (b, "%02d", month);
                  break;
               case 3:
                  strcpy (b, language.ShortMonth (month, l).c_str());
                  break;
               default:
                  strcpy (b, language.LongMonth (month, l).c_str());
                  break;
               }
            if (len > 1 && token[1] == 'M')
               Upper (b);
            else if (len > 0 && token[0] == 'M')
               UpperBuff (b, 1);
            b += strlen(b);
            break;
            }

         case 'Y': case 'y':    // year
            {
            int len = 0;
            while (*p == 'y' || *p == 'Y')
               p++, len++;
            if (len <= 2)
               sprintf (b, "%2d", year % 100);
            else
               sprintf (b, "%4d", year);
            b += strlen(b);
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


// Today () /////////////////////////////////////////////////////////////////
IntlDate &IntlDate::Today ()
   {
   #ifdef __WIN32__
   if ((GetVersion() & 0x80000000L) == 0)
      {
      SYSTEMTIME st;
      GetLocalTime (&st);
      Set (st.wDay, st.wMonth, st.wYear);
      }
   else
   #endif
      {
      long t = time(0);
      struct tm *ut = localtime (&t);
      Set (ut->tm_mday, ut->tm_mon + 1, ut->tm_year + defaultCentury);
      }
   return *this;
   }


// Tomorrow  ////////////////////////////////////////////////////////////////
IntlDate &IntlDate::Tomorrow ()
   {
   Today ();
   return Inc (idDay);
   }


// Yesterday ////////////////////////////////////////////////////////////////
IntlDate &IntlDate::Yesterday ()
   {
   Today ();
   return Dec (idDay);
   }


// DosDate   ////////////////////////////////////////////////////////////////
short IntlDate::DosDate () const
   {
   return short (day | (month << 5) | ((year - 1980) <<  9));
   }


// WoY () ///////////////////////////////////////////////////////////////////
int IntlDate::WoY (int firstWeekDay) const
   {
   IntlDate start (1, January, year);
   while (start.DoW() != firstWeekDay)
      start++;

   long w = dayNumber - start.dayNumber;
   if (w < 0)
      return IntlDate (31, December, year-1).WoY();

   w /= 7;
   return (int)w + 1;
   }


// Zero date ////////////////////////////////////////////////////////////////
IntlDate &IntlDate::Zero ()
   {
   dayNumber = 0;
   day = 0;
   month = 0;
   year = 0;
   return *this;
   }


// default string format  ///////////////////////////////////////////////////
void IntlDate::SetPicture (const string &p)
   {
	Picture = p;
	}


void IntlDate::SetPicture (const char *p)
	{
	if (p)
		Picture = p;
	else
		Picture = "";
	}

// stream support (uses default formats) ////////////////////////////////////
ostream &operator << (ostream &s, IntlDate &date)
   {
   s << date.String ();
   return s;
   }


istream &operator >> (istream &s, IntlDate &date)
   {
   string str;
   s >> str;
   date = str;
   return s;
   }

// EOF
