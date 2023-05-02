/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// dtedit.h                                                                //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#ifndef DTEDIT_H
#define DTEDIT_H

class _GRIDCLASS TGDateTimeEdit;
class _GRIDCLASS TGDateEdit;
class _GRIDCLASS TGTimeEdit;

class _GRIDCLASS TGDateTimeValidator : public TValidator
   {
   friend TGDateTimeEdit;
   friend TGDateEdit;
   friend TGTimeEdit;

   protected:
      int IncCaret;
      const char *validChar;
      TGDateTimeEdit *owner;

   public:
      TGDateTimeValidator (const char *s) { IncCaret = 0; validChar = s; }
      bool IsValidInput (char far* str, bool suppressFill);
   };


class _GRIDCLASS TGDateValidator;
class _GRIDCLASS TGTimeValidator;
class _GRIDCLASS TGDateTimeEdit : public TGEdit
   {
   friend TGDateTimeValidator;
   friend TGDateValidator;
   friend TGTimeValidator;

   protected:
      string Format;
      virtual void EvChar (uint key, uint repeatCount, uint flags);

   public:
      TGDateTimeEdit (TWindow *p, uint resid, const char *fmt,
                      TModule *module = 0);
      TGDateTimeEdit (TWindow *p, uint id, int x, int y, int w, int h,
                      const char *fmt, TModule *module = 0);
      string GetFormat () const { return Format; }
      DECLARE_RESPONSE_TABLE (TGDateTimeEdit);
   };

#endif

// EOF
