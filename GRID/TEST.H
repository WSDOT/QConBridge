/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// test.h                                                                  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


#ifndef __OWL_PRINTER_H
#include <owl/printer.h>
#endif

// class Application                                                        *
class Application : public TApplication
   {
   public:
      Application () : TApplication () {}
      void InitMainWindow ();
   };


#define MAXLINE 20
class Data
   {
   public:
      char str[31];
      double real;
      long integer;
      char stat[31];
      char combo[16];
      bool chk;
      IntlDate date;
      TGTime time;

      Data () : chk(false), real(0), integer(0) { *str = 0; *stat = 0; *combo = 0; }
      void Init (char *s, double s2, long s3, char *st, bool b, char *c, IntlDate &da, TGTime &ti)
         {
         strcpy (str, s);
         real = s2;
         integer = s3;
         strcpy (stat, st);
         chk = b;
         strcpy (combo, c);
         date = da;
         time = ti;
         }
   };


class TestGrid : public TGrid
	{
	friend class Test;

   protected:
      FILE *log;
      string datefmt;
      Test *owner;
      TBrush green;
      TBrush red;
      TBrush blue;

      void CalcTotal ();

      virtual bool AppendNewLine (int &newline);
      virtual bool CalcPos ();
      virtual bool CellColor        (int line, int id, bool active, TColor &bg, TColor &fg, HBRUSH &brush);
      virtual void DataChanged      (int line, int id, const void *data);
      virtual bool DataChanging     (int line, int id, const void *data);
      virtual bool DeleteLine       (int line);
      virtual bool InsertLine       (int line);
      virtual TColor GetTitleColor  (TGrid::TitleColor part) const;
      virtual void NewRowSelected   (int newrow, int oldrow);
      virtual void ScrollData       (int firstline, int lastline);
      virtual const void *TransferItem (int line, int id);
      virtual void TransferItem     (int line, int id, const void *data);
      virtual Visible VisibleCell   (int line, int id);

   public:
      Data data[MAXLINE];
      TestGrid (TWindow *p);
     ~TestGrid ();
   };



class Test : public TGDialog
   {
   protected:
      TestGrid *grid;
      TGEdit        *s1;
      TGStatic      *s2;
      TGCheckBox    *s3;
      TGButton      *s4;
      TGComboBox    *s5;
      TGDoubleEdit  *s6;
      TGDateEdit    *s7;
      TGLongEdit    *s8;
      TGTimeEdit    *s9;
      TGDoubleEdit *tot;
      TButton *ok, *esc, *test, *hide, *cell, *date, *add, *HideCol;
      string datefmt;
      int new_id;
      void EvWindowPosChanged (WINDOWPOS far &wpos);
      void SetupWindow ();
      void TestRefresh ();
      void TestHideShow ();
      void TestHideCell ();
      void TestDate ();
      void TestAddColumn ();
      void TestHideColumn ();

   public:
      Test (TWindow *p);
      void CalcTotal (Data *d, int n);
      double total;
		bool visible, visiblecell, visiblecol;

      DECLARE_RESPONSE_TABLE (Test);
   };

// EOF

