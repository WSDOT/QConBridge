/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// numedit.cpp                                                             //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "griddll.h"
#pragma hdrstop

bool TGDoubleEdit::blankNull = true;
bool TGLongEdit::blankNull   = true;
bool TGShortEdit::blankNull  = true;

// TGDoubleEdit /////////////////////////////////////////////////////////////
TGDoubleValidator::TGDoubleValidator (double min, double max, TGDoubleEdit *own)
                : TFilterValidator("0-9+-.")
   {
   Min = min;
   Max = max;
   owner = own;
   }


void TGDoubleValidator::Error(TWindow* parent)
   {
   string ti = language.LoadString (NUMBER_TITLE);
   const char *t = ti.c_str();
   string mess = language.LoadString (NUMBER_OUTOFRANGE);
   size_t pos = mess.find ("%s");
   if (pos != NPOS)
      {
      char s[64];
      sprintf (s, "%.*f", owner->Dec, Min);
      mess.replace (pos, 2, string(s));
      pos = mess.find ("%s");
      if (pos != NPOS)
         {
         sprintf (s, "%.*f", owner->Dec, Max);
         mess.replace (pos, 2, s);
         }
      }
   owner->SuspendValidator ();
   parent->MessageBox (mess.c_str(), t, MB_ICONEXCLAMATION|MB_OK|MB_TASKMODAL);
   owner->RestoreValidator ();
   }


bool TGDoubleValidator::IsValid (const char *s)
   {
   if (TFilterValidator::IsValid (s))
      {
      double val = atof (s);
      if (val >= Min && val <= Max)
         return true;
      }
   return false;
   }


DEFINE_RESPONSE_TABLE1 (TGDoubleEdit, TGEdit)
   EV_WM_KILLFOCUS,
END_RESPONSE_TABLE;

TGDoubleEdit::TGDoubleEdit (TWindow *p, int id, double val, int x, int y, int w, int h,
                        uint width, uint dec, double min,
                        double max, TModule *module)
           : TGEdit (p, id, "", x, y, w, h, width+1, false, module)
   {
   Dec = dec;
   Validator = new TGDoubleValidator (min, max, this);
   value = val;
   }


TGDoubleEdit::TGDoubleEdit (TWindow *p, uint resid, uint width, uint dec,
                        double min, double max, TModule *module)
           : TGEdit (p, resid, width+1, module)
   {
   Dec = dec;
   Validator = new TGDoubleValidator (min, max, this);
   value = 0;
   }


void TGDoubleEdit::SetupWindow ()
   {
   TGEdit::SetupWindow ();
   *this = value;
   }


void TGDoubleEdit::SetMin (double t)
   {
   TGDoubleValidator *dv = dynamic_cast<TGDoubleValidator *>(Validator);
   if (dv)
      {
      dv->SetMin (t);
      if (dv->GetMax() < t)
         dv->SetMax (t);
      }
   }


void TGDoubleEdit::SetMax (double t)
   {
   TGDoubleValidator *dv = dynamic_cast<TGDoubleValidator *>(Validator);
   if (dv)
      {
      dv->SetMax (t);
      if (dv->GetMin() > t)
         dv->SetMin (t);
      }
   }


void TGDoubleEdit::operator = (double t)
   {
   value = t;
   if (t != 0 || blankNull == false)
      {
      char buffer[64];
      sprintf (buffer, "%.*f", Dec, t);
      SetText (buffer);
      }
   else
      SetText ("");
   }


void TGDoubleEdit::EvKillFocus (HWND h)
   {
   TGEdit::EvKillFocus (h);
   char buffer[64];
   int l = TextLen;
   if (l > 63)
      l = 63;
   GetText (buffer, l+1);
   *this = atof (buffer);   // reformat
   }


TGDoubleEdit::operator double ()
   {
   int l = TextLen;
   if (l > 63)
      l = 63;
   char buffer[64];
   GetText (buffer, l+1);
   value = atof (buffer);
   return value;
   }


void TGDoubleEdit::DrawCell (TDC &dc, TRect &rc, const void *p, TGrid::Align align)
   {
   if (p)
      {
      char s[64];
      double d = *static_cast<const double *>(p);
      if (d != 0.0 || blankNull == false)
         sprintf (s, "%.*f", Dec, d);
      else
         *s = 0;
      TGEdit::DrawCell (dc, rc, s, align);
      }
   else
      TGEdit::DrawCell (dc, rc, 0, align);
   }



void TGDoubleEdit::SetBlankNull (bool b)
   {
   if (blankNull != b)
      {
      blankNull = b;
      if (grid && grid->IsWindow ())
         {
         int id = GetId ();
         for (int i = 0; i < grid->GetVisibleLines(); i++)
            grid->RefreshItem (i, id);
         }
      if (IsWindow ())
         Invalidate ();
      }
   }


uint TGDoubleEdit::Transfer (void *buffer, TTransferDirection direction)
   {
   if (buffer)
      {
      switch (direction)
         {
         case tdSetData:
            *this = *static_cast<double *>(buffer);
            break;

         case tdGetData:
            *static_cast<double *>(buffer) = *this;
            break;
         }
      }

   return sizeof(double);
   }

// TGLongEdit ////////////////////////////////////////////////////////////
TGLongValidator::TGLongValidator (long min, long max, TGLongEdit *own)
                : TFilterValidator("0-9+-")
   {
   owner = own;
   Min = min;
   Max = max;
   }


void TGLongValidator::Error(TWindow* parent)
   {
   string ti = language.LoadString (NUMBER_TITLE);
   const char *t = ti.c_str();
   string mess = language.LoadString (NUMBER_OUTOFRANGE);
   size_t pos = mess.find ("%s");
   if (pos != NPOS)
      {
      char s[64];
      sprintf (s, "%ld", Min);
      mess.replace (pos, 2, string(s));
      pos = mess.find ("%s");
      if (pos != NPOS)
         {
         sprintf (s, "%ld", Max);
         mess.replace (pos, 2, s);
         }
      }
   owner->SuspendValidator ();
   parent->MessageBox (mess.c_str(), t, MB_ICONEXCLAMATION|MB_OK|MB_TASKMODAL);
   owner->RestoreValidator ();
   }


bool TGLongValidator::IsValid (const char *s)
   {
   if (TFilterValidator::IsValid (s))
      {
      long val = atol (s);
      if (val >= Min && val <= Max)
         return true;
      }
   return false;
   }


TGLongEdit::TGLongEdit (TWindow *p, int id, long val,
                              int x, int y, int w, int h, uint width,
                              long min, long max, TModule *module)
             : TGEdit (p, id, "", x, y, w, h, width+1, false, module)
   {
   Validator = new TGLongValidator (min, max, this);
   value = val;
   }


TGLongEdit::TGLongEdit (TWindow *p, uint resid, uint width,
                        long min, long max, TModule *module)
           : TGEdit (p, resid, width+1, module)
   {
   Validator = new TGLongValidator (min, max, this);
   value = 0;
   }



void TGLongEdit::SetupWindow ()
   {
   TGEdit::SetupWindow ();
   *this = value;
   }


void TGLongEdit::SetMin (long t)
   {
   TGLongValidator *dv = dynamic_cast<TGLongValidator *>(Validator);
   if (dv)
      {
      dv->SetMin (t);
      if (dv->GetMax() < t)
         dv->SetMax (t);
      }
   }


void TGLongEdit::SetMax (long t)
   {
   TGLongValidator *dv = dynamic_cast<TGLongValidator *>(Validator);
   if (dv)
      {
      dv->SetMax (t);
      if (dv->GetMin() > t)
         dv->SetMin (t);
      }
   }


void TGLongEdit::operator = (long t)
   {
   value = t;
   if (t || blankNull == false)
      {
      char buffer[64];
      sprintf (buffer, "%ld", t);
      SetText (buffer);
      }
   else
      SetText ("");
   }


TGLongEdit::operator long ()
   {
   int l = TextLen;
   if (l > 63)
      l = 63;
   char buffer[64];
   GetText (buffer, l+1);
   value =  atol (buffer);
   return value;
   }


void TGLongEdit::DrawCell (TDC &dc, TRect &rc, const void *p, TGrid::Align align)
   {
   if (p)
      {
      char s[64];
      long d = *static_cast<const long *>(p);
      if (d != 0 || blankNull == false)
         sprintf (s, "%ld", d);
      else
         *s = 0;
      TGEdit::DrawCell (dc, rc, s, align);
      }
   else
      TGEdit::DrawCell (dc, rc, 0, align);
   }


void TGLongEdit::SetBlankNull (bool b)
   {
   if (blankNull != b)
      {
      blankNull = b;
      if (grid && grid->IsWindow ())
         {
         int id = GetId ();
         for (int i = 0; i < grid->GetVisibleLines(); i++)
            grid->RefreshItem (i, id);
         }
      if (IsWindow ())
         Invalidate ();
      }
   }


uint TGLongEdit::Transfer (void *buffer, TTransferDirection direction)
   {
   if (buffer)
      {
      switch (direction)
         {
         case tdSetData:
            *this = *static_cast<long *>(buffer);
            break;

         case tdGetData:
            *static_cast<long *>(buffer) = *this;
            break;
         }
      }

   return sizeof(long);
   }

// TGShortEdit ////////////////////////////////////////////////////////////
TGShortValidator::TGShortValidator (short min, short max, TGShortEdit *own)
                : TFilterValidator("0-9+-")
   {
   owner = own;
   Min = min;
   Max = max;
   }


void TGShortValidator::Error(TWindow* parent)
   {
   string ti = language.LoadString (NUMBER_TITLE);
   const char *t = ti.c_str();
   string mess = language.LoadString (NUMBER_OUTOFRANGE);
   size_t pos = mess.find ("%s");
   if (pos != NPOS)
      {
      char s[64];
      sprintf (s, "%d", Min);
      mess.replace (pos, 2, string(s));
      pos = mess.find ("%s");
      if (pos != NPOS)
         {
         sprintf (s, "%d", Max);
         mess.replace (pos, 2, s);
         }
      }
   owner->SuspendValidator ();
   parent->MessageBox (mess.c_str(), t, MB_ICONEXCLAMATION|MB_OK|MB_TASKMODAL);
   owner->RestoreValidator ();
   }


bool TGShortValidator::IsValid (const char *s)
   {
   if (TFilterValidator::IsValid (s))
      {
      short val = short(atoi (s));
      if (val >= Min && val <= Max)
         return true;
      }
   return false;
   }


TGShortEdit::TGShortEdit (TWindow *p, int id, short val,
                              int x, int y, int w, int h, uint width,
                              short min, short max, TModule *module)
             : TGEdit (p, id, "", x, y, w, h, width+1, false, module)
   {
   Validator = new TGShortValidator (min, max, this);
   value = val;
   }


TGShortEdit::TGShortEdit (TWindow *p, uint resid, uint width,
                        short min, short max, TModule *module)
           : TGEdit (p, resid, width+1, module)
   {
   Validator = new TGShortValidator (min, max, this);
   value = 0;
   }


void TGShortEdit::SetupWindow ()
   {
   TGEdit::SetupWindow ();
   *this = value;
   }


void TGShortEdit::SetMin (short t)
   {
   TGShortValidator *dv = dynamic_cast<TGShortValidator *>(Validator);
   if (dv)
      {
      dv->SetMin (t);
      if (dv->GetMax() < t)
         dv->SetMax (t);
      }
   }


void TGShortEdit::SetMax (short t)
   {
   TGShortValidator *dv = dynamic_cast<TGShortValidator *>(Validator);
   if (dv)
      {
      dv->SetMax (t);
      if (dv->GetMin() > t)
         dv->SetMin (t);
      }
   }


void TGShortEdit::operator = (short t)
   {
   value = 0;
   if (t || blankNull == false)
      {
      char buffer[64];
      sprintf (buffer, "%d", t);
      SetText (buffer);
      }
   else
      SetText ("");
   }


TGShortEdit::operator short ()
   {
   int l = TextLen;
   if (l > 63)
      l = 63;
   char buffer[64];
   GetText (buffer, l+1);
   value =  short(atoi (buffer));
   return value;
   }


void TGShortEdit::DrawCell (TDC &dc, TRect &rc, const void *p, TGrid::Align align)
   {
   if (p)
      {
      char s[64];
      short d = *static_cast<const short *>(p);
      if (d != 0 || blankNull == false)
         sprintf (s, "%d", d);
      else
         *s = 0;
      TGEdit::DrawCell (dc, rc, s, align);
      }
   else
      TGEdit::DrawCell (dc, rc, 0, align);
   }


void TGShortEdit::SetBlankNull (bool b)
   {
   if (blankNull != b)
      {
      blankNull = b;
      if (grid && grid->IsWindow ())
         {
         int id = GetId ();
         for (int i = 0; i < grid->GetVisibleLines(); i++)
            grid->RefreshItem (i, id);
         }
      if (IsWindow ())
         Invalidate ();
      }
   }


uint TGShortEdit::Transfer (void *buffer, TTransferDirection direction)
   {
   if (buffer)
      {
      switch (direction)
         {
         case tdSetData:
            *this = *static_cast<short *>(buffer);
            break;

         case tdGetData:
            *static_cast<short *>(buffer) = *this;
            break;
         }
      }

   return sizeof(short);
   }

// EOF
