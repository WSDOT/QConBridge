/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// test.cpp                                                                //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "griddll.h"
#pragma hdrstop
#include <owl\framewin.h>
#include <owl\validate.h>
#include "test.h"
#include "test.rh"
#include "typeinfo.h"

// OwlMain //////////////////////////////////////////////////////////////////
int OwlMain (int, char * [])
   {
   // grid initialization; this is unnecessary if you're using the library --
   // as a DLL - initialization will be done at DLL's startup ---------------
   #ifdef _GRIDLIB
   InitGridLib ();
   #endif

   return Application().Run ();
   }


// class Application ////////////////////////////////////////////////////////
void Application::InitMainWindow ()
   {
   EnableBWCC(true);
   EnableCtl3d (true);
   TFrameWindow *frame = new TFrameWindow (0, "TGrid 1.40 demo", new Test(0), true);
   SetMainWindow (frame);
   }


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Class Test                                                              //
//                                                                         //
//-------------------------------------------------------------------------//
//                                                                         //
// This class is derived from TGDialog                                     //
// It may be derived from TDialog if color support is not needed           //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
DEFINE_RESPONSE_TABLE1 (Test, TGDialog)
   EV_COMMAND (107, TestRefresh), 
   EV_COMMAND (108, TestHideShow),
   EV_COMMAND (109, TestDate),
   EV_COMMAND (115, TestHideCell),
   EV_COMMAND (117, TestAddColumn),
   EV_COMMAND (118, TestHideColumn),
   EV_WM_WINDOWPOSCHANGED,
END_RESPONSE_TABLE;


// constructor //////////////////////////////////////////////////////////////
Test::Test (TWindow *p) : TGDialog (p, DIALOG_TEST)
   {
   // initial state ---------------------------------------------------------
   visible = true;
   visiblecell = true;
   visiblecol = true;
   new_id = 1000;

   // show zero in TGDoubleEdit, TGLongEdit, TGShortEdit --------------------
   TGDoubleEdit::blankNull = false;
   TGLongEdit::blankNull   = false;
   TGShortEdit::blankNull  = false;

   // date information ------------------------------------------------------
	datefmt = GetLanguage().SysShortDate ();

	// create interface objects, just as if there were no grid. --------------
	ok   = new TButton (this, IDOK);
	esc  = new TButton (this, IDCANCEL);
	test = new TButton (this, 107);
	hide = new TButton (this, 108);
	date = new TButton (this, 109);
	cell = new TButton (this, 115);
  add  = new TButton (this, 117);
  HideCol  = new TButton (this, 118);

   new TGShortEdit (this, 111, 4, -999, 999);

   s2 = new TGStatic     (this, 102, 31);
   s1 = new TGEdit       (this, 101, 31);
   s3 = new TGCheckBox   (this, 103);        // s3->SetBwcc (true);
   s4 = new TGButton     (this, 104);
   s5 = new TGComboBox   (this, 105, 16);
   s6 = new TGDoubleEdit (this, 106, 6, 2, 0.0, 999.99);
   s7 = new TGDateEdit   (this, 110, datefmt, IntlDate (idToday), IntlDate (31,12,2049), false);
   s8 = new TGLongEdit   (this, 113, 6, -99999L, 99999L);
   s9 = new TGTimeEdit   (this, 116, "HH:MM:SS.sss", TGTime (), TGTime (23,59,59,999));

   tot = new TGDoubleEdit (this, 114, 9, 2, -999999.99, 999999.99);

   // set colors for some controls ------------------------------------------
   s2->SetColors (TColor(128,0,0), TColor::LtGray);   // Red on lightgray ---
   // next two controls have a light gray column background,
   // and a white background when the cell is active
   s7->SetColors (TColor(128,0,128), GetSysColor(COLOR_WINDOW), TColor::LtGray);//Magenta
   s6->SetColors (TColor(0,0,128), GetSysColor(COLOR_WINDOW), TColor::LtGray);  //Blue --

   // create the grid -------------------------------------------------------
   grid = new TestGrid (this);
   grid->datefmt = datefmt;

   // link controls to the grid. --------------------------------------------
   grid->Add (s1, "String");
   grid->Add (s2, "Static");
   grid->Add (s7, "Date");
   grid->Add (s9, "Time");
   grid->Add (s4, "Button");
   grid->Add (s5, "ComboBox");
   grid->Add (s6, "Double", TGrid::Right);
   grid->Add (s8, "Long", TGrid::Right);
   grid->Add (s3, "Check", TGrid::Center);

   // setup different fonts on 2 columns ------------------------------------
   // this is always done AFTER the relevant control has been Add'ed --------
   int logPixelsY = TScreenDC().GetDeviceCaps (LOGPIXELSY);
   // Arial, 9 points, normal -----------------------------------------------
   grid->SetFont (105, "Arial", -MulDiv(logPixelsY,9,72), FW_NORMAL);
   // Times New Roman, 10 points, bold --------------------------------------
	grid->SetFont (106, "Times New Roman", -MulDiv(logPixelsY,10,72));

   // uncomment next line to disable line swapping (ie CTRL+UP or DOWN ------
   // will move current line 1 line up or down) -----------------------------
   // grid->SetLineSwapping (false);  // line swapping is on by default -----
   }


// SetupWindow //////////////////////////////////////////////////////////////
void Test::SetupWindow ()
   {
   TGDialog::SetupWindow ();

   // grid's size is now available ------------------------------------------
   ofstream out ("dim.txt");
   TRect dim = grid->GetWindowRect ();
   // this line shows the grid's overall size (including borders) -----------
   out << "grid's width: "  << grid->GetTotalWidth()   << "; "
       << "grid's height: " << grid->GetTotalHeight () << "\n";
   // these 2 lines show the grid's size, excluding borders -----------------
   // both should yield the same result                                     -
   out << "TGrid::Attr.W: " << grid->Attr.W  << "; "
       << "TGrid::Attr.H: " << grid->Attr.H  << "\n";
   out << "Window width: "  << dim.Width()   << "; "
       << "Window height: " << dim.Height () << "\n";
   // Note: only when all controls have been created can the grid's size ----
   //       safely be queried; it is thus advisable not to query the size   -
   //       from TGrid's own SetupWindow, but rather from it's parent's     -
   //       SetupWindow, where one can assume all controls have been created.

   // stuffing data into TGComboBox -----------------------------------------
   int max = MAXLINE;
   if (max < 1000)
      max = 1000;
   else if (max >= 10000)
      max = 9999;
   for (int i = 0; i <= max; i++)
      {
      char string[5];
      sprintf (string, "%04d", i);
      s5->AddString (string);
      }
   }


// Respond to window resizing, passing the message to TGrid. ////////////////
void Test::EvWindowPosChanged (WINDOWPOS far &wpos)
   {
   TGDialog::EvWindowPosChanged (wpos);
   
   // change other controls' position ---------------------------------------
   int h = HIWORD(GetDialogBaseUnits());
   int y = wpos.cy - 3*h;
   test->SetWindowPos (0, test->Attr.X, y-test->Attr.H/2-2, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
   hide->SetWindowPos (0, hide->Attr.X, y+hide->Attr.H/2+2, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
   date->SetWindowPos (0, date->Attr.X, y-test->Attr.H/2-2, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
   cell->SetWindowPos (0, cell->Attr.X, y+cell->Attr.H/2+2, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
   add ->SetWindowPos (0, add ->Attr.X, y-test->Attr.H/2-2, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
   HideCol->SetWindowPos(0, HideCol->Attr.X, y+HideCol->Attr.H/2+2, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
   ok->SetWindowPos (0, ok->Attr.X, y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
   esc->SetWindowPos (0, esc->Attr.X, y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);

   // change grid size ------------------------------------------------------
   // new coordinates are relative to borders -------------------------------
   int y0 = 40;
   grid->SetWindowPos (0, 10, y0, wpos.cx-20, y-2*h-y0, SWP_NOZORDER);
   }


// test the RefreshItem and ChangeTitle methods here ////////////////////////
void Test::TestRefresh ()
   {
   // let's change some column to test RefreshItem --------------------------
   for (int i = 0; i < MAXLINE; i++)
      {
      int val = atoi (grid->data[i].str+8);
      val += i;
      sprintf (grid->data[i].str, "EDIT1 - %04d", val);

      grid->RefreshItem (i, 101);
      }

   // test ChangeTitle too --------------------------------------------------
   const char *s = grid->GetTitle (101);
   int val = strlen(s) <= 9 ? 0 : atoi (s+9);
   char buffer[30];
   sprintf (buffer, "Edit 1 - %d", val+1);
   grid->ChangeTitle (101, buffer);
   }


// test the EvShowWindow response ///////////////////////////////////////////
void Test::TestHideShow ()
   {
   if (visible)
      {
      grid->Show (SW_HIDE);
      hide->SetCaption ("S&how");
      }
   else
      {
      grid->Show (SW_SHOW);
		hide->SetCaption ("&Hide");
      }
   visible ^= 1;
   }


void Test::TestHideCell ()
   {
   if (visiblecell)
      cell->SetCaption ("Show &cells");
   else
		cell->SetCaption ("Hide &cells");
   visiblecell ^= 1;
   grid->Refresh ();
   }

void Test::TestHideColumn ()
   {
   if (visiblecol)
   {
      grid->HideColumn(102);
      HideCol->SetCaption ("Show Colu&mn");
   }
   else
   {
      grid->UnHideColumn(102);
		  HideCol->SetCaption ("Hide Colu&mn");
   }
   visiblecol ^= 1;
//   grid->Refresh ();
   }


void Test::TestAddColumn ()
   {
   TGEdit *someedit = new TGEdit (this, new_id++, "", 0,0, s1->Width(), s1->Height(), 11, false);
   someedit->Create ();
   grid->Add (someedit, "Added");
   }

// show a calculated total //////////////////////////////////////////////////
void Test::CalcTotal (Data *d, int n)
   {
   total = 0;
   for (int i = 0; i < n; i++, d++)
      total += d->real * d->integer;

   *tot = total;
   }

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Class TestGrid                                                          //
//                                                                         //
//-------------------------------------------------------------------------//
//                                                                         //
// This class is derived from TGrid                                        //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

// constructor //////////////////////////////////////////////////////////////
TestGrid::TestGrid (TWindow *p) :
   TGrid (p, 100, TGrid::NextLine, TGrid::TabNextColumn/*AutoNextLine*/,
          TGrid::ReverseColor, MAXLINE,
          gAllBorders|
          gHideScroll|
          gCenterTitle|
          gSizeColumn|
          gLineSwapOn|
          gHorzShrink|
//          gAutoSelection|
          gMultipleSelection|
          gDefCursor),   // other option: gArrowCursor
   green (TColor::LtGreen),
   red   (TColor::LtRed),
   blue  (TColor::LtBlue),
   owner (dynamic_cast<Test *>(p))
   {
   // initialize some test data
   IntlDate date (idToday);
   TGTime time;
   for (int i = 0; i < MAXLINE; i++, date.Inc (idMonth), time.Inc (tHour))
      {
      char edstring[16];
      char ststring[16];
      char cbstring[5];
      sprintf (edstring,  "EDIT1 - %04d", i+1);
      sprintf (ststring,  "STATIC - %04d", i+1);
      sprintf (cbstring,  "%04d", i+1);
      data[i].Init (edstring, i*10, (MAXLINE-i) -MAXLINE/2, ststring, (i&1) == 1?true:false, cbstring, date, time);
      }
   // Change fonts: grid hass Arial, 10 points, bold. -----------------------
   //               title has Times New Roman, 12 points, normal, italic. ---
	int logPixelsY = TScreenDC().GetDeviceCaps (LOGPIXELSY);
   SetFont ("Arial", -MulDiv(logPixelsY,9,72));
   SetTitleFont ("Times New Roman", -MulDiv(logPixelsY,11,72), FW_NORMAL, true);

   // log cell changes to log.txt file. -------------------------------------
   log = fopen ("log.txt", "w");
   }


// destructor ///////////////////////////////////////////////////////////////
TestGrid::~TestGrid ()
   {
   if (log)
      fclose (log);

   // dump data for verification --------------------------------------------
   FILE *fd = fopen ("dump.txt", "w");
   fprintf (fd, " %-20s ! %6s ! %6s ! %-10s ! %-12s ! %-13s ! %-5s ! %-5s\n", "String", "Real", "Long", "Date", "Time", "Static", "Combo", "Check");
   fprintf (fd, "----------------------+--------+--------+------------+--------------+---------------+-------+-------\n");
   int i;
   for (i = 0; i < MAXLINE; i++)
      {
      fprintf (fd, " %-20.20s ! %6.2f ! %6ld ! %10s ! %12s ! %-13s ! %-5s ! %-5d\n",
            data[i].str,
            data[i].real,
            data[i].integer,
            data[i].date.String().c_str(),
            data[i].time.String("HH:MM:SS.sss").c_str(),
            data[i].stat,
            data[i].combo,
            data[i].chk);
      }

   int nsel = CountSelectedLines ();
   if (nsel > 0)
      {
      fprintf (fd, "%d line(s) were selected: ", nsel);
      int *lines = new int[nsel];
      GetSelectedLines (lines);
      for (i = 0; i < nsel; i++)
         {
         if (i)
            fprintf (fd, ", ");
         fprintf (fd, "%d", lines[i]);
         }
      fprintf (fd, "\n");
      delete [] lines;
      }
   else
      fprintf (fd, "no line was selected");

   fclose (fd);
   }


// CalcPos //////////////////////////////////////////////////////////////////
// change current line before we start editing the grid                    //
bool TestGrid::CalcPos ()
	{
   if (TGrid::CalcPos ())
      {
      CalcTotal ();
      // uncomment next line to change initial active line
      // Go (20);
      return true;
      }
   return false;
   }


// AppendNewLine ////////////////////////////////////////////////////////////
// Overloaded TGrid::AppendNewLine; this is only necessary if:             //
//  1) maxLines == -1                                                      //
//  and                                                                    //
//  2) the maximum number of lines has to be controlled                    //
bool TestGrid::AppendNewLine (int &newline)
   {
   if (newline >= MAXLINE)
      return false;
   else
      return true;
   }


// TransferItem /////////////////////////////////////////////////////////////
// const void *TransferItem : overloaded virtual function                  //
// this function returns the relevant data for line l and column id        //
// (data is transferred from memory to grid)                               //
const void *TestGrid::TransferItem (int line, int id)
   {
   if (line >= MAXLINE)
      return 0;

   switch (id)
      {
      case 101:
         return data[line].str;

      case 102:
         return data[line].stat;

      case 103:
         return &data[line].chk;

      case 104:
         return "Button";

      case 105:
         return data[line].combo;

      case 106:
         return &data[line].real;

      case 110:
         return &data[line].date;

      case 113:
         return &data[line].integer;

      case 116:
         return &data[line].time;
      }

   return 0;
   }


// TransferItem /////////////////////////////////////////////////////////////
// void  TransferItem : overloaded virtual function                        //
// this function retrieves data from the grid and stores it in memory      //
// (data is transferred from grid to memory)                               //
void TestGrid::TransferItem (int line, int id, const void *item)
   {
   if (line >= MAXLINE || item == 0)
      return;

   switch (id)
      {
      case 101:
         strcpy (data[line].str, XFR_STRING(item));
         break;
      case 102:
         strcpy (data[line].stat, XFR_STRING(item));
         break;
      case 103:
			data[line].chk = XFR_bool(item);
			break;
		case 105:
			strcpy (data[line].combo, XFR_STRING(item));
			break;
		case 106:
			data[line].real = XFR_DOUBLE(item);
			break;
		case 110:
			data[line].date = XFR_DATE(item);
         break;
		case 113:
			data[line].integer = XFR_LONG(item);
			break;
		case 116:
			data[line].time = XFR_TIME(item);
         break;
      }
   }


// compute the sum of columns
void TestGrid::CalcTotal ()
   {
   owner->CalcTotal (data, MAXLINE);
   if (log)
      fprintf (log, "Total: %g\n", owner->total);
   }

// InsertLine ///////////////////////////////////////////////////////////////
// bool InsertLine : overloaded virtual function                           //
// We have to move data on our behalf, then clear the current line         //
// return true to allow tell TGrid line insert is allowed                  //
bool TestGrid::InsertLine (int line)
   {
   movmem (data+line, data+line+1, sizeof(Data)*(MAXLINE-line-1));
   memset (data+line, 0, sizeof(Data));

   char string[16];
   sprintf (string, "INSERTED - %04d", line+1);
   data[line].Init (string, line+1, (line+1), string, true, "0000", IntlDate(idToday), TGTime(tNow));

   return true;
   }


// DeleteLine ///////////////////////////////////////////////////////////////
// bool InsertLine : overloaded virtual function                           //
// We have to move data on our behalf, then clear the last line            //
// return true to tell TGrid line delete is allowed                        //
bool TestGrid::DeleteLine (int line)
   {
   movmem (data+line+1, data+line, sizeof(Data)*(MAXLINE-line-1));
   memset (data+MAXLINE-1, 0, sizeof(Data));

   char string[16];
   sprintf (string, "DELETED - %04d", line+1);
   data[MAXLINE-1].Init (string, line+1, (line+1), string, false, "1000", IntlDate(idToday), TGTime(tNow));

   return true;
   }


// GetTitleColor ////////////////////////////////////////////////////////////
// TColor GetTitleColor: overloaded virtual function                       //
// Will return the desired color for the specified part of the title       //
// (text, background, Frame or 3D highlight/shadow)                        //
TColor TestGrid::GetTitleColor (TGrid::TitleColor part) const
   {
   switch (part)
      {
      case TGrid::TitleText:
         return TColor(0,0,255); // blue
      case TGrid::TitleBackground:
      case TGrid::TitleHilite:
      case TGrid::TitleShadow:
      case TGrid::TitleFrame:
         break;
      }

   return TGrid::GetTitleColor (part);
   }


// DataChanging /////////////////////////////////////////////////////////////
// bool DataChanging: overloaded virtual function                          //
// Called when a cell is loosing focus and its data has changed; returns   //
// true if it's OK to transfer data                                        //
bool TestGrid::DataChanging (int line, int id, const void *)
   {
   if (log)
      fprintf (log, "Data changing: line %d, item %d\n", line, id);
   return true;
   }


// DataChanged //////////////////////////////////////////////////////////////
// void DataChanged: overloaded virtual function                           //
// Called after a cell has written back its data                           //
void TestGrid::DataChanged (int line, int id, const void *)
   {
   if (id == 106 || id == 113)
      CalcTotal ();
   else if (id == 103)
      {
      RefreshItem (line, 102);
      RefreshItem (line, 105);
      }

   if (log)
      fprintf (log, "Data changed: line %d, item %d\n", line, id);
   }


// ScrollData ///////////////////////////////////////////////////////////////
// void ScrollData: overloaded virtual function                            //
// Called whenever the grid scrolls horizontally (also called first time   //
// the grid is painted)                                                    //
void TestGrid::ScrollData (int firstline, int lastline)
   {
   if (log)
      fprintf (log, "Buffer data from line %d to line %d\n", firstline, lastline);
   }


// NewRowSelected ///////////////////////////////////////////////////////////
// void NewRowSelected: overloaded virtual function                        //
// Called whenever focus changes from one row to another                   //
void TestGrid::NewRowSelected (int newrow, int oldrow)
   {
   if (log)
      fprintf (log, "Line changed from %d to %d\n", oldrow, newrow);
   }


// CellColor ////////////////////////////////////////////////////////////////
// bool CellColor: returns custom colors to be used on a specific cell     //
// Change colors for real and integer columns according to cell value      //
bool TestGrid::CellColor (int line, int id, bool active,
                          TColor &bg, TColor &fg, HBRUSH &brush)
   {
   if (!active)
      {
      switch (id)
         {
         case 106:
            if (data[line].real >= 100.0)
               {
               bg = TColor::LtGreen;
               fg = TColor::Black;
               brush = green;
               return true;
               }
            break;

         case 113:
            if (data[line].integer == 0)
               {
               fg = TColor::LtYellow;
               bg = TColor::LtBlue;
               brush = blue;
               return true;
               }
            else if (data[line].integer < 0)
               {
               fg = TColor::White;
               bg = TColor::LtRed;
               brush = red;
               return true;
               }
            break;
         }
      }

   return false;
   }


// VisibleCell //////////////////////////////////////////////////////////////
// TGrid::Visible VisibleCell: returns a constant telling TGrid if the     //
// specified cell is visible                                               //
TGrid::Visible TestGrid::VisibleCell (int line, int id)
   {
   if (dynamic_cast<Test *>(Parent)->visiblecell == false)
      {
      switch (id)
         {
         case 103:
            if (line == 0)
               return TGrid::gEmpty;
            break;

         case 104:
            if (line > 0)
               return TGrid::gLocked;
            break;

         case 105:
         case 102:
            if (line > 0 && data[line].chk == false)
               return TGrid::gEmpty;
            else if (id == 105)
               return TGrid::gFlat;
            break;
         }
      }

   return TGrid::VisibleCell (line, id);
   }

// EOF
