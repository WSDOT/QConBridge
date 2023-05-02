/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// numedit.h                                                               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#ifndef NUMEDIT_H
#define NUMEDIT_H

#if !defined(OWL_VALIDATE_H)
#include <owl\validate.h>
#endif

// real number //////////////////////////////////////////////////////////////
class _GRIDCLASS TGDoubleEdit;
class _GRIDCLASS TGDoubleValidator : public TFilterValidator
   {
   protected:
      double Min;
      double Max;
      TGDoubleEdit *owner;

   public:
      TGDoubleValidator (double min, double max, TGDoubleEdit *own);
      void Error (TWindow* parent);
      bool IsValid (const char * str);
      void SetMin (double min) { Min = min; }
      void SetMax (double max) { Max = max; }
      double GetMin () const { return Min; }
      double GetMax () const { return Max; }
   };

class _GRIDCLASS TGDoubleEdit : public TGEdit
   {
   friend TGDoubleValidator;

   protected:
      int Dec;
      double value;
      virtual void DrawCell (TDC &dc, TRect &rc, const void *p,
                             TGrid::Align align = TGrid::Right);
      virtual void EvKillFocus (HWND h);
      virtual void SetupWindow ();

   public:
      static bool blankNull;
      TGDoubleEdit (TWindow *p, int id, double val, int x, int y, int w, int h,
                  uint width, uint dec, double min, double max,
                  TModule *module = 0);
      TGDoubleEdit (TWindow *p, uint resid, uint width, uint dec,
                  double min, double max, TModule *module = 0);

      void SetMin (double d);
      void SetMax (double d);
      virtual uint Transfer (void *buffer, TTransferDirection direction);

      void operator = (double d);
      operator double ();

      void SetBlankNull (bool b);

      DECLARE_RESPONSE_TABLE (TGDoubleEdit);
   };


// long integer /////////////////////////////////////////////////////////////
class _GRIDCLASS TGLongEdit;
class _GRIDCLASS TGLongValidator : public TFilterValidator
   {
   protected:
      long Min;
      long Max;
      TGLongEdit *owner;

   public:
      TGLongValidator (long min, long max, TGLongEdit *own);
      void Error (TWindow* parent);
      bool IsValid (const char * str);
      void SetMin (long min) { Min = min; }
      void SetMax (long max) { Max = max; }
      long GetMin () const { return Min; }
      long GetMax () const { return Max; }
   };

class _GRIDCLASS TGLongEdit : public TGEdit
   {
   friend TGLongValidator;

   protected:
      long value;
      virtual void DrawCell (TDC &dc, TRect &rc, const void *p,
                             TGrid::Align align = TGrid::Right);
      virtual void SetupWindow ();

   public:
      static bool blankNull;
      TGLongEdit (TWindow *p, int id, long val,
                     int x, int y, int w, int h,
                     uint width, long min, long max,
                     TModule *module = 0);
      TGLongEdit (TWindow *p, uint resid, uint width,
                     long min, long max, TModule *module = 0);

      void SetMin (long d);
      void SetMax (long d);
      virtual uint Transfer (void *buffer, TTransferDirection direction);

      void operator = (long d);
      operator long ();

      void SetBlankNull (bool b);
   };

// short integer ////////////////////////////////////////////////////////////
class _GRIDCLASS TGShortEdit;
class _GRIDCLASS TGShortValidator : public TFilterValidator
   {
   protected:
      short Min;
      short Max;
      TGShortEdit *owner;

   public:
      TGShortValidator (short min, short max, TGShortEdit *own);
      void Error (TWindow* parent);
      bool IsValid (const char * str);
      void SetMin (short min) { Min = min; }
      void SetMax (short max) { Max = max; }
      short GetMin () const { return Min; }
      short GetMax () const { return Max; }
   };

class _GRIDCLASS TGShortEdit : public TGEdit
   {
   friend TGShortValidator;

   protected:
      short value;
      virtual void DrawCell (TDC &dc, TRect &rc, const void *p,
                             TGrid::Align align = TGrid::Right);
      virtual void SetupWindow ();

   public:
      static bool blankNull;
      TGShortEdit (TWindow *p, int id, short val,
                     int x, int y, int w, int h,
                     uint width, short min, short max,
                     TModule *module = 0);
      TGShortEdit (TWindow *p, uint resid, uint width,
                     short min, short max, TModule *module = 0);

      void SetMin (short d);
      void SetMax (short d);
      virtual uint Transfer (void *buffer, TTransferDirection direction);

      void operator = (short d);
      operator short ();

      void SetBlankNull (bool b);
   };

#endif

// EOF

