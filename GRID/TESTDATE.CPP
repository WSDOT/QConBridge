/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// testdate.cpp                                                            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "griddll.h"
#pragma hdrstop
#include "test.h"


void Test::TestDate ()
   {
   unlink ("date.txt");
   ofstream out ("date.txt");

   GetLanguage().SetCurrentLanguage (idEnglish);
   IntlDate Today (idToday);
   IntlDate Tomorrow (idTomorrow);
   IntlDate Yesterday (idYesterday);
   string str, longdate, shortdate;

   // default language
   longdate = GetLanguage().LongDate();
   shortdate = GetLanguage().ShortDate();

   // default long form
   out << "Language: idEnglish\n"
       << "Default long form\n"
       << "Today is "
		 << Today.String (longdate)
		 << "\n"
		 << "Tomorrow will be "
		 << Tomorrow.String (longdate)
		 << "\n"
		 << "Yesterday was "
       << Yesterday.String (longdate)
       << "\n";

   // default short form
   out << "Default short form\n"
       << "Today is "
       << Today.String (shortdate)
       << "\n";

   // language: idFrench
   GetLanguage().SetCurrentLanguage (idFrench);
   shortdate = GetLanguage().ShortDate();  // refresh short date
   longdate = GetLanguage().LongDate();    // refresh long date

   // default long form
   out << "Langue: Français\n"
       << "Forme longue par défaut\n"
       << "Nous sommes le "
       << Today.String (longdate)
       << "\n"
       << "Demain, nous serons le "
       << Tomorrow.String (longdate)
       << "\n"
       << "Hier, nous étions le "
       << Yesterday.String (longdate)
       << "\n";

   // default short form
   out << "Forme courte par défaut"
       << "\n"
       << "Nous sommes le "
       << Today.String (shortdate)
       << "\n";

   // revert to idEnglish
   GetLanguage().SetCurrentLanguage (idEnglish);
   shortdate = GetLanguage().ShortDate();  // refresh short date
   longdate = GetLanguage().LongDate();    // refresh long date

   // check operators
   IntlDate test = Today + 1L;
   out << "Language: idEnglish\n"
       << "test = "
       << test.String (shortdate)
       << "\n"

       << (test == Tomorrow ? "test == Tomorrow: Ok\n" : "test == Tomorrow: Error\n")
       << (test == Today ? "test == Today: Error\n" : "test == Today: Ok\n")

       << (test != Tomorrow ? "test != Tomorrow: Error\n" : "test != Tomorrow: Ok\n")
       << (test != Today ? "test != Today: Ok\n" : "test != Today: Error\n")

       << (test < Tomorrow ? "test < Tomorrow: Error\n" : "test < Tomorrow: Ok\n")
       << (test < Today ? "test < Today: Error\n" : "test < Today: Ok\n")

       << (test <= Tomorrow ? "test <= Tomorrow: Ok\n" : "test <= Tomorrow: Error\n")
       << (test <= Today ? "test <= Today: Error\n" : "test <= Today: Ok\n")

       << (test > Tomorrow ? "test > Tomorrow: Error\n" : "test > Tomorrow: Ok\n")
       << (test > Today ? "test > Today: Ok\n" : "test > Today: Error\n")

       << (test >= Tomorrow ? "test >= Tomorrow: Ok\n" : "test >= Tomorrow: Error\n")
       << (test >= Today ? "test >= Today: Ok\n" : "test >= Today: Error\n");

   test.Dec (idMonth);
   out << "test.Dec(idMonth): "
       << test.String (shortdate)
       << "\n";

   test.Dec (idYear, 10);
   out << "test.Dec(idYear, 10): "
       << test.String (shortdate)
       << "\n"

       << (test == Tomorrow ? "test == Tomorrow: Error\n" : "test == Tomorrow: Ok\n")
       << (test == Today ? "test == Today: Error\n" : "test == Today: Ok\n")

       << (test != Tomorrow ? "test != Tomorrow: Ok\n" : "test != Tomorrow: Error\n")
       << (test != Today ? "test != Today: Ok\n" : "test != Today: Error\n")

       << (test < Tomorrow ? "test < Tomorrow: Ok\n" : "test < Tomorrow: Error\n")
       << (test < Today ? "test < Today: Ok\n" : "test < Today: Error\n")

       << (test <= Tomorrow ? "test <= Tomorrow: Ok\n" : "test <= Tomorrow: Error\n")
       << (test <= Today ? "test <= Today: Ok\n" : "test <= Today: Error\n")

       << (test > Tomorrow ? "test > Tomorrow: Error\n" : "test > Tomorrow: Ok\n")
       << (test > Today ? "test > Today: Error\n" : "test > Today: Ok\n")

       << (test >= Tomorrow ? "test >= Tomorrow: Error\n" : "test >= Tomorrow: Ok\n")
       << (test >= Today ? "test >= Today: Error\n" : "test >= Today: Ok\n");

   test = Today;
   out << "test = Today: "
       << test.String (shortdate)
       << "\n";

   test.Inc (idMonth);
   out << "test.Inc(idMonth): "
		 << test.String (shortdate)
		 << "\n";

   test.Inc (idYear, 10);
   out << "test.Inc(idYear, 10): "
       << test.String (shortdate)
       << "\n"

       << (test == Tomorrow ? "test == Tomorrow: Error\n" : "test == Tomorrow: Ok\n")
       << (test == Today ? "test == Today: Error\n" : "test == Today: Ok\n")

       << (test != Tomorrow ? "test != Tomorrow: Ok\n" : "test != Tomorrow: Error\n")
       << (test != Today ? "test != Today: Ok\n" : "test != Today: Error\n")

       << (test < Tomorrow ? "test < Tomorrow: Error\n" : "test < Tomorrow: Ok\n")
       << (test < Today ? "test < Today: Error\n" : "test < Today: Ok\n")

       << (test <= Tomorrow ? "test <= Tomorrow: Error\n" : "test <= Tomorrow: Ok\n")
       << (test <= Today ? "test <= Today: Error\n" : "test <= Today: Ok\n")

       << (test > Tomorrow ? "test > Tomorrow: Ok\n" : "test > Tomorrow: Error\n")
       << (test > Today ? "test > Today: Ok\n" : "test > Today: Error\n")

       << (test >= Tomorrow ? "test >= Tomorrow: Ok\n" : "test >= Tomorrow: Error\n")
       << (test >= Today ? "test >= Today: Ok\n" : "test >= Today: Error\n");

   // test constructors
   long t = test;
   IntlDate d1;
   IntlDate d2 (10, 1, 1957);
   IntlDate d3 (t);
   IntlDate d4 ("1957/01/10", "YYYY/MM/DD");
   IntlDate d5 ("570110", "YYMMDD");
   IntlDate d6 (10, 1, 57);
   out << "t = "
       << t
       << "\n"
       << "d1 = "
       << d1.String ()
       << "\n"
       << "d2 = "
       << d2.String ()
       << "\n"
       << "d3 = "
       << d3.String ()
       << "\n"
       << "d4 = "
       << d4.String ()
       << "\n"
       << "d5 = "
       << d5.String ()
       << "\n"
       << "d6 = "
       << d6.String ()
       << "\n";

   // test operator - and operator +
   out << "d4 - d5 = "
       << (d4-d5)
       << ((d4-d5) == 0 ? "Ok" : "Error")
       << "\n"
       << "Today-d4 = "
       << (Today-d4)
       << "\n"
       << "Today-Tomorrow = "
       << (Today-Tomorrow)
       << "\n"
       << "Today-Yesterday = "
       << (Today-Yesterday)
       << "\n"
       << "Today - 7L = "
       << (Today-7L).String()
       << "\n"
       << "Today + 7L = "
       << (Today+7L).String()
       << "\n\n";

   IntlDate d7 (30, 12, 1990);
   for (int i = 0; i < 10; i++)
      {
      IntlDate d8 = d7;
      d8.Inc (idYear, i);
      for (int j = 0; j < 5; j++, d8++)
         {
         out << d8.String ("ddd dd-mmm-yyyy", idEnglish)
             << "\n";
         }
      out << "\n";
      }


   TGTime t0 (4.0);
   TGTime t1 ("09.4777", "HH.hhhh");
   out << t0.String ("HH.MM.SS.sss\n")
       << t1.String ("HH.hh")
       << " - "
       << t1.String ("HH:MM.mm")
       << " - "
       << t1.String ("HH:MM:SS.sss")
       << "\n\n";

   MessageBox ("Test recorded in date.txt file", "Test date");
   }

// EOF
