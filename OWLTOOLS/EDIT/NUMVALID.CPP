///////////////////////////////////////////////////////////////////////
// OWLTools - Extension Library for the Borland OWL Framework
// Copyright (C) 1999  Washington State Department of Transportation
//                     Bridge and Structures Office
//
// This library was developed as part of the Alternate Route Project
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the Alternate Route Library Open Source License as published by 
// the Washington State Department of Transportation, Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but is distributed 
// AS IS, WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
// or FITNESS FOR A PARTICULAR PURPOSE. See the Alternate Route Library Open Source 
// License for more details.
//
// You should have received a copy of the Alternate Route Library Open Source License 
// along with this program; if not, write to the Washington State Department of 
// Transportation, Bridge and Structures Office, 4500 3rd Ave SE - P.O. Box  47340, 
// Olympia, WA 98503, USA or e-mail Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Revision Log
// ============
// May 1997 - Created, Richard Brice
// Oct 1999 - Released as open source

#include <owltools\numvalid.h>

/******************************************************************************
TIncRangeValidator::TIncRangeValidator

Description:
   Inclusive range validator. The value in the edit control must be in the
   range [min,max]
******************************************************************************/
TIncRangeValidator::TIncRangeValidator(double min,double max) :
TPXPictureValidator(!(min < 0) ? "[* ][*#][.][*#][{{E,e}[{-,+}]*#}]" : "[* ][-][*#][.][*#][{{E,e}[{-,+}]*#}]")
{
   Min = min;
   Max = max;
}

void TIncRangeValidator::Error(TWindow* parent)
{
   TApplication *app = GetApplicationObject();
   ostrstream os;
   os << "Value must be in the range " << Min << " to " << Max << ends;
   if (parent)
      parent->MessageBox(os.str(),app->GetName(),MB_ICONEXCLAMATION | MB_OK);
   else
      ::MessageBox(0,os.str(),app->GetName(),MB_ICONEXCLAMATION | MB_OK | MB_TASKMODAL);

   delete[] os.str();
}


bool TIncRangeValidator::IsValid(const char far* s)
{
   if (TPXPictureValidator::IsValid(s))
      {
      double value;
      value = GetValue(s);
      if (Min <= value && value <= Max)
         return true;
      }

   return false;
}

uint TIncRangeValidator::Transfer(char far* s,void* buffer,TTransferDirection direction)
{
   if (Options & voTransfer)
      {
      if (direction == tdGetData)
         {
         *(double *)buffer = GetValue(s);
         }
      else if (direction == tdSetData)
         {
         ostrstream os;
         os << (*(double *)buffer) << ends;
         strcpy(s,os.str());
         delete[] os.str();
         }

      return sizeof (double);
      }
   else
      return 0;
}

double TIncRangeValidator::GetValue(const char far* s)
{
   double value;
   if (s[0] == 0)
      value = 0;
   else
      value = atof(s);
   return value;
   /*
   double value;
   char *text;

   text = new char[strlen(s) + 1];
   strcpy(text,s);
   if (text[0] == 0) // treat empty controls as zero
      value = 0;
   else
      {
      string valText( string(text).strip(string::Both,' ') );
      strcpy(text,valText.c_str());
      value = atof(text);
      }
   delete[] text;

   return value;
   */
}

/******************************************************************************
TExcRangeValidator::TExcRangeValidator

Description:
   Exclusive range validator. The value in the edit control must not be in the
   range [min,max]
******************************************************************************/
TExcRangeValidator::TExcRangeValidator(double min,double max) :
TPXPictureValidator("[* ][-][*#][.][*#][{{E,e}[{-,+}]*#}]")
{
   Min = min;
   Max = max;
}

void TExcRangeValidator::Error(TWindow* parent)
{
   TApplication *app = GetApplicationObject();
   ostrstream os;
   os << "Value must be less than " << Min << " or greater than " << Max << ends;
   if (parent)
      parent->MessageBox(os.str(),app->GetName(),MB_ICONEXCLAMATION | MB_OK);
   else
      ::MessageBox(0,os.str(),app->GetName(),MB_ICONEXCLAMATION | MB_OK | MB_TASKMODAL);

   delete[] os.str();
}


bool TExcRangeValidator::IsValid(const char far* s)
{
   if (TPXPictureValidator::IsValid(s))
      {
      double value;
      value = GetValue(s);
      if (value < Min || Max < value)
         return true;
      }

   return false;
}

uint TExcRangeValidator::Transfer(char far* s,void* buffer,TTransferDirection direction)
{
   if (Options & voTransfer)
      {
      if (direction == tdGetData)
         {
         *(double *)buffer = GetValue(s);
         }
      else if (direction == tdSetData)
         {
         ostrstream os;
         os << (*(double *)buffer) << ends;
         strcpy(s,os.str());
         delete[] os.str();
         }

      return sizeof (double);
      }
   else
      return 0;
}

double TExcRangeValidator::GetValue(const char far* s)
{
   double value;
   if (s[0] == 0)
      value = 0;
   else
      value = atof(s);
   return value;
/*
   double value;
   char *text;

   text = new char[strlen(s) + 1];
   strcpy(text,s);
   if (text[0] == 0) // treat empty controls as zero
      value = 0;
   else
      {
      string valText( string(text).strip(string::Both,' ') );
      strcpy(text,valText.c_str());
      value = atof(text);
      }
   delete[] text;

   return value;
*/
}


/******************************************************************************
TExtremeValueValidator::TExtremeValueValidator

Description:
   Extreme value validator. The value in the edit control must have the proper
   value with respect to the extreme value. The value may be <, <=, >, or
   >= the extreme value, depending on the LimitType specified.
******************************************************************************/
TExtremeValueValidator::TExtremeValueValidator(double extremeValue,LimitType lt) :
TPXPictureValidator((!(extremeValue < 0) && (lt == ltGT || lt == ltGE)) ? "[* ][*#][.][*#][{{E,e}[{-,+}]*#}]" : "[* ][-][*#][.][*#][{{E,e}[{-,+}]*#}]")
{
   ExtremeValue = extremeValue;
   lType = lt;
}

void TExtremeValueValidator::Error(TWindow* parent)
{
   TApplication *app = GetApplicationObject();
   ostrstream os;
   os << "Value must be ";
   switch (lType)
      {
      case ltLT:
           os << "less than ";
           break;

      case ltLE:
           os << "less than or equal to ";
           break;

      case ltGT:
           os << "greater than ";
           break;

      case ltGE:
           os << "greater than or equal to ";
           break;
      }
   os << ExtremeValue << ends;

   if (parent)
      parent->MessageBox(os.str(),app->GetName(),MB_ICONEXCLAMATION | MB_OK);
   else
      ::MessageBox(0,os.str(),app->GetName(),MB_ICONEXCLAMATION | MB_OK | MB_TASKMODAL);

   delete[] os.str();
}


bool TExtremeValueValidator::IsValid(const char far* s)
{
   bool testOK;

   if (TPXPictureValidator::IsValid(s))
      {
      double value;
      value = GetValue(s);
      switch (lType)
         {
         case ltLT:
              testOK = (value <  ExtremeValue) ? true : false;
              break;

         case ltLE:
              testOK = (value <= ExtremeValue) ? true : false;
              break;

         case ltGT:
              testOK = (value >  ExtremeValue) ? true : false;
              break;

         case ltGE:
              testOK = (value >= ExtremeValue) ? true : false;
              break;
         }

      if (testOK)
         return true;
      }

   return false;
}

uint TExtremeValueValidator::Transfer(char far* s,void* buffer,TTransferDirection direction)
{
   if (Options & voTransfer)
      {
      if (direction == tdGetData)
         {
         *(double *)buffer = GetValue(s);
         }
      else if (direction == tdSetData)
         {
         ostrstream os;
         os << (*(double *)buffer) << ends;
         strcpy(s,os.str());
         delete[] os.str();
         }

      return sizeof (double);
      }
   else
      return 0;
}

double TExtremeValueValidator::GetValue(const char far* s)
{
   double value;
   if (s[0] == 0)
      value = 0;
   else
      value = atof(s);
   return value;
/*
   double value;
   char *text;

   text = new char[strlen(s) + 1];
   strcpy(text,s);
   if (text[0] == 0) // treat empty controls as zero
      value = 0;
   else
      {
      string valText( string(text).strip(string::Both,' ') );
      strcpy(text,valText.c_str());
      value = atof(text);
      }
   delete[] text;

   return value;
*/
}
