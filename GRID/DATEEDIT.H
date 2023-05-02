/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// dateedit.h                                                              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#ifndef DATEEDIT_H
#define DATEEDIT_H

#ifndef INTLDATE_H
#include "intldate.h"
#endif

#ifndef DTEDIT_H
#include "dtedit.h"
#endif

typedef enum DateError
   {
   DATE_OK, DATE_IS_NULL, DATE_FORMAT, DATE_OUT_OF_RANGE
   } DateError;

class _GRIDCLASS TGDateEdit;
class _GRIDCLASS TGDateValidator : public TGDateTimeValidator
   {
   friend TGDateEdit;

   protected:
      IntlDate Min;
      IntlDate Max;
      bool notNull;
      DateError errcode;

   public:
      TGDateValidator (const IntlDate &min, const IntlDate &max,
                       TGDateEdit *owner, bool notnull);
      void Error (TWindow* parent);
      bool IsValid (const char * str);
      void SetMin (const IntlDate &min) { Min = min; }
      void SetMax (const IntlDate &max) { Max = max; }
      IntlDate GetMin () const { return Min; }
      IntlDate GetMax () const { return Max; }
   };

class _GRIDCLASS TGDateEdit : public TGDateTimeEdit
   {
   friend TGDateValidator;

   protected:
      virtual void DrawCell (TDC &dc, TRect &rc, const void *p, TGrid::Align align = TGrid::Left);
      virtual void EvKillFocus (HWND h);
      virtual void EvKeyDown (uint vkey, uint repeat, uint flags);
      virtual void EvRButtonDblClk (uint keys, TPoint &where);

   public:
      TGDateEdit (TWindow *p, uint resid, const char *fmt,
                  const IntlDate &min, const IntlDate &max, bool notnull,
                  TModule *module = 0);
		TGDateEdit (TWindow *p, uint resid, const string &fmt,
						const IntlDate &min, const IntlDate &max, bool notnull,
                  TModule *module = 0);
		TGDateEdit (TWindow *p, uint id, int x, int y, int w, int h,
						const char *fmt, const IntlDate &min,
                  const IntlDate &max, bool notnull, TModule *module = 0);
		TGDateEdit (TWindow *p, uint id, int x, int y, int w, int h,
						const string &fmt, const IntlDate &min,
                  const IntlDate &max, bool notnull, TModule *module = 0);

		void SetFilter (const IntlDate &min, const IntlDate &max,
                      const char *edit, bool n);
		void SetFilter (const IntlDate &min, const IntlDate &max,
                      const string &edit, bool n);
		void SetMin    (const IntlDate &d);
      void SetMax    (const IntlDate &d);
      virtual void SetText (const char far *s);
      virtual void SetText (const string &s);
      virtual uint Transfer (void *buffer, TTransferDirection direction);

      IntlDate operator = (const IntlDate &d);
      operator IntlDate ();

      DECLARE_RESPONSE_TABLE (TGDateEdit);
   };

#endif

// EOF
