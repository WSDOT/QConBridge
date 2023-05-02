/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// timeedit.h                                                              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#ifndef TIMEEDIT_H
#define TIMEEDIT_H

#ifndef TGTIME_H
#include "tgtime.h"
#endif

#ifndef DTEDIT_H
#include "dtedit.h"
#endif

typedef enum TimeError
   {
   TIME_OK, TIME_FORMAT, TIME_OUT_OF_RANGE
   } TimeError;

class _GRIDCLASS TGTimeEdit;
class _GRIDCLASS TGTimeValidator : public TGDateTimeValidator
   {
   friend TGTimeEdit;

   protected:
      TGTime Min;
      TGTime Max;
      TimeError errcode;

   public:
      TGTimeValidator (const TGTime &min, const TGTime &max, TGTimeEdit *owner);
      void Error (TWindow* parent);
      bool IsValid (const char * str);
      void SetMin (const TGTime &min) { Min = min; }
      void SetMax (const TGTime &max) { Max = max; }
      TGTime GetMin () const { return Min; }
      TGTime GetMax () const { return Max; }
   };

class _GRIDCLASS TGTimeEdit : public TGDateTimeEdit
   {
   friend TGTimeValidator;

   protected:
      virtual void DrawCell (TDC &dc, TRect &rc, const void *p, TGrid::Align align = TGrid::Left);
      virtual void EvKillFocus (HWND h);

   public:
      TGTimeEdit (TWindow *p, uint resid, const char *fmt,
                  const TGTime &min, const TGTime &max, TModule *module = 0);
		TGTimeEdit (TWindow *p, uint resid, const string &fmt,
						const TGTime &min, const TGTime &max, TModule *module = 0);
		TGTimeEdit (TWindow *p, uint id, int x, int y, int w, int h,
						const char *fmt, const TGTime &min,
                  const TGTime &max, TModule *module = 0);
		TGTimeEdit (TWindow *p, uint id, int x, int y, int w, int h,
						const string &fmt, const TGTime &min,
                  const TGTime &max, TModule *module = 0);

		void SetFilter (const TGTime &min, const TGTime &max, const char *edit);
		void SetFilter (const TGTime &min, const TGTime &max, const string &edit);
		void SetMin    (const TGTime &d);
      void SetMax    (const TGTime &d);
      virtual void SetText (const char far *s);
      virtual void SetText (const string &s);
      virtual uint Transfer (void *buffer, TTransferDirection direction);

      TGTime operator = (const TGTime &d);
      operator TGTime ();

      DECLARE_RESPONSE_TABLE (TGTimeEdit);
   };

#endif

// EOF
