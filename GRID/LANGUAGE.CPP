/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// language.cpp                                                            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "griddll.h"
#pragma hdrstop

struct winlangtable
   {
   char code[4];
   idLanguage langid;
   };
static winlangtable wlt[] =
   {
   { "DEU", idGerman },
   { "ENG", idEnglish },
   { "ENU", idEnglish },
   { "ESN", idSpanish },
   { "ESP", idSpanish },
   { "FRA", idFrench },
   { "FRB", idFrench },
   { "FRC", idFrench },
   { "ITA", idItalian },
   { "NLD", idDutch },
   { "PTG", idPortuguese }
   };
static const int nwlt = sizeof(wlt) / sizeof(winlangtable);


Language language;

Language &GetLanguage ()
   {
   return language;
   }

void InitLanguage ()
   {
   language.GetSystemLanguageInfo ();
   }


Language::Language ()
   {
   currentLanguage = systemLanguage = idEnglish;
   }


void Language::SetCurrentLanguage (idLanguage lan)
   {
   currentLanguage = lan;
   }


void Language::GetSystemLanguageInfo ()
   {
   char str[80];
   GetProfileString ("intl", "sShortDate", "MM/dd/yyyy", str, 80);
   shortDatePicture = str;
   GetProfileString ("intl", "sLongDate", "dddd, MM/dd/yyyy", str, 80);
   longDatePicture = str;

   GetProfileString ("intl", "sLanguage", "Eng", str, 4);
   string lang = str;
   lang.to_upper ();
   int i;
   for (i = 0; i < nwlt; i++)
      {
      if (lang == wlt[i].code)
         {
         currentLanguage = systemLanguage = wlt[i].langid;
         break;
         }
      }

   // if unsupported language ...
   if (i == nwlt)
      currentLanguage = systemLanguage = idEnglish;
   }


string Language::LoadString (int resid, idLanguage l) const
   {
   #ifndef _GRIDLIB
      #define module GridModule
   #else
      #define module ::Module
   #endif

   if (l == idUndefLanguage)
      l = currentLanguage;

   return string (module->GetInstance(), resid + l);
   }


int Language::SysLanguageId ()
   {
   switch (systemLanguage)
      {
      default:
         return LANG_ENGLISH;
      case idDutch:
         return LANG_DUTCH;
      case idFrench:
         return LANG_FRENCH;
      case idGerman:
         return LANG_GERMAN;
      case idItalian:
         return LANG_ITALIAN;
      case idPortuguese:
         return LANG_PORTUGUESE;
      case idSpanish:
         return LANG_SPANISH;
      }
   }

// read resource strings  ///////////////////////////////////////////////////
string Language::LongMonth (int m, idLanguage l) const
   {
   if (m < 1 || m > 12)
      return "";
   else
      return LoadString (m+DATE_LONG_MONTH_OFFSET-1, l);
   }


string Language::ShortMonth (int m, idLanguage l) const
   {
   if (m < 1 || m > 12)
      return "";
   else
      return LoadString (m+DATE_SHORT_MONTH_OFFSET-1, l);
   }


string Language::LongDay (int w, idLanguage l) const
   {
   if (w < Monday || w > Sunday)
      return "";
   else
      return LoadString (int(w)+DATE_LONG_DAY_OFFSET, l);
   }



string Language::ShortDay (int w, idLanguage l) const
   {
   if (w < Monday || w > Sunday)
      return "";
   else
      return LoadString (int(w)+DATE_SHORT_DAY_OFFSET, l);
   }


string Language::LongDate (idLanguage l) const
   {
   return LoadString (DATE_LONG_FORM, l);
   }


string Language::ShortDate (idLanguage l) const
   {
   return LoadString (DATE_SHORT_FORM, l);
   }


string Language::SysLongDate () const
   {
   return longDatePicture;
   }


string Language::SysShortDate () const
   {
   return shortDatePicture;
   }


string Language::DaySuffix (int day, idLanguage l) const
   {
   return LoadString (DATE_DAY_SUFFIX_OFFSET+day-1, l);
   }


int Language::ShortDayIndex (const string &string, idLanguage l) const
	{
	return ShortDayIndex (string.c_str(), l);
	}


int Language::ShortDayIndex (const char *string, idLanguage l) const
   {
   for (int w = Monday; w <= Sunday; w ++)
      {
      if (lstrcmpi (ShortDay (int(w), l).c_str(), string) == 0)  // International compare
         return int(w);
      }

   return InvalidDay;
   }


int Language::LongDayIndex (const string &string, idLanguage l) const
	{
   return LongDayIndex (string.c_str(), l);
	}


int Language::LongDayIndex (const char *string, idLanguage l) const
   {
   for (int w = Monday; w <= Sunday; w ++)
      {
      if (lstrcmpi (LongDay (int(w), l).c_str(), string) == 0)  // International compare
         return int(w);
      }

   return InvalidDay;
   }


int Language::ShortMonthIndex (const string &string, idLanguage l) const
	{
	return ShortMonthIndex (string.c_str(), l);
	}

int Language::ShortMonthIndex (const char *string, idLanguage l) const
   {
   for (int m = 1; m <= 12; m++)
      {
      if (lstrcmpi (ShortMonth (m, l).c_str(), string) == 0)  // International compare
         return m;
      }

   return 0;
   }


int Language::LongMonthIndex (const string &string, idLanguage l) const
	{
	return LongMonthIndex (string.c_str(), l);
	}


int Language::LongMonthIndex (const char *string, idLanguage l) const
   {
   for (int m = 1; m <= 12; m++)
      {
      if (lstrcmpi (LongMonth (m, l).c_str(), string) == 0)  // International compare
         return m;
      }

   return 0;
   }

// EOF
