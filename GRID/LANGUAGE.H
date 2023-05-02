/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// language.h                                                              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// resource holds 78 strings for each supported language;                  //
// for simplicity, each language is stored in a 100 strings table          //
// --------------------------------------------------------------+-------  //
// the strings are stored as follows:                            |Indexes  //
// --------------------------------------------------------------+-------  //
//  - 12 strings for months (long form)                          |   1-12  //
//  - 12 strings for months (short form)                         |  13-24  //
//  -  7 strings for week days (long form)                       |  25-31  //
//  -  7 strings for week days (short form)                      |  32-38  //
//  -  1 string for long date format (e.g. "Wwww, Mmmm d, YYYY") |     39  //
//  -  1 string for short date format (e.g. "MM/DD/YY")          |     40  //
//  - 31 strings for the suffix of the each day of the month     |  41-71  //
//     (st, nd, rd, th, ., er, º, etc)                           |         //
//  -  4 date error strings                                      |  72-75  //
//  -  3 strings for day, month, year                            |  76-78  //
//  -  3 time error strings                                      |  79-81  //
//  -  2 strings for out of range numbers                        |  98-99  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// supported languages: English, Dutch, French, German, Italian,           //
//                      Portuguese, Spanish                                //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Please report to me any error or inconsistency you may find in your     //
// language table.                                                         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// I'll be glad to add any other language, provided you send me your       //
// translation and instructions.                                           //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


#define DATE_LONG_MONTH_OFFSET    1
#define DATE_SHORT_MONTH_OFFSET  13
#define DATE_LONG_DAY_OFFSET     25
#define DATE_SHORT_DAY_OFFSET    32
#define DATE_LONG_FORM           39
#define DATE_SHORT_FORM          40
#define DATE_DAY_SUFFIX_OFFSET   41
#define DATE_TITLE               72
#define DATE_NODATE              73
#define DATE_OUTOFRANGE          74
#define DATE_INCORRECT           75
#define DATE_DATE                76
#define DATE_MONTH               77
#define DATE_YEAR                78

#define TIME_TITLE               79
#define TIME_OUTOFRANGE          80
#define TIME_INCORRECT           81

#define NUMBER_TITLE             98
#define NUMBER_OUTOFRANGE        99

#define LANGUAGE_NSTRINGS 100

#include "grid.rh"

typedef enum idLanguage
   {
   idUndefLanguage   = 0,
   idEnglish         = ENGLISH_STRINGS,
   idDutch           = DUTCH_STRINGS,
   idFrench          = FRENCH_STRINGS,
   idGerman          = GERMAN_STRINGS,
   idItalian         = ITALIAN_STRINGS,
   idPortuguese      = PORTUGUESE_STRINGS,
   idSpanish         = SPANISH_STRINGS
   } idLanguage;

class _GRIDCLASS Language
   {
   public:
      idLanguage currentLanguage;
      idLanguage systemLanguage;
      string shortDatePicture;
      string longDatePicture;
      Language ();

      void SetCurrentLanguage (idLanguage l = idEnglish);
      void GetSystemLanguageInfo ();
      string LoadString (int id, idLanguage l = idUndefLanguage) const;
      int SysLanguageId ();

      // date strings, long form --------------------------------------------
      string LongDate (idLanguage l = idUndefLanguage) const;
      string LongDay   (int w, idLanguage l = idUndefLanguage) const;
		string LongMonth (int m, idLanguage l = idUndefLanguage) const;

      // date strings, short form -------------------------------------------
      string DaySuffix (int day, idLanguage l = idUndefLanguage) const;
      string ShortDate (idLanguage l = idUndefLanguage) const;
      string ShortDay  (int w, idLanguage l = idUndefLanguage) const;
		string ShortMonth (int m, idLanguage l = idUndefLanguage) const;

      // date indexes, long form --------------------------------------------
		int LongDayIndex (const char *string, idLanguage l = idUndefLanguage) const;
      int LongDayIndex (const string &string, idLanguage l = idUndefLanguage) const;
		int LongMonthIndex (const char *string, idLanguage l = idUndefLanguage) const;
		int LongMonthIndex (const string &string, idLanguage l = idUndefLanguage) const;

      // date indexes, short form --------------------------------------------
		int ShortDayIndex (const char *string, idLanguage l = idUndefLanguage) const;
		int ShortDayIndex (const string &string, idLanguage l = idUndefLanguage) const;
		int ShortMonthIndex (const char *string, idLanguage l = idUndefLanguage) const;
		int ShortMonthIndex (const string &string, idLanguage l = idUndefLanguage) const;

      // date strings, system default ---------------------------------------
      string SysShortDate () const;
      string SysLongDate () const;
   };

extern Language language;
extern Language & _GRIDFUNCTION GetLanguage ();
extern void InitLanguage ();

#ifdef __WIN32__
#define Upper CharUpper
#define UpperBuff CharUpperBuff
#else
#define Upper AnsiUpper
#define UpperBuff AnsiUpperBuff
#endif

// EOF

