///////////////////////////////////////////////////////////////////////
// QConBridge - HL93 Live Load Analysis
// Copyright (C) 1999  Washington State Department of Transportation
//                     Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation, 
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but 
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source 
// License along with this program; if not, write to the Washington 
// State Department of Transportation, Bridge and Structures Office, 
// 4500 3rd AVE SE - P.O. Box  47340, Olympia, WA 98503, USA or e-mail 
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Revision Log
// ============
// May 1997 - Created, Richard Brice, PE
// Oct 1999 - Released as Open Source

#if !defined (__guniedit_h)
#define __guniedit_h


#include <owltools\nbredit.h>
#include <owl\static.h>
#include <owl\dc.h>
#include <winsys\geometry.h>
#include <iomanip.h>
#include <esl\units.h>
#include <griddll.h>

template <class T>
class _OWLCLASS TGUnitEdit : public TGDoubleEdit
{
protected:
   int width;
   int precision;

public:
TGUnitEdit(TWindow* parent, int resourceID,
           const T DisplayUnit,uint Width,
           uint Precision,
           int tagResId = 0,const char *tagText = 0,
           TModule* module = 0) :
TGDoubleEdit(parent,resourceID,Width,Precision,0.00,0.00,module),
displayUnit(DisplayUnit)
{
   width = Width;
   precision = Precision;

   // Don't force the user to use a range checking validator
   // this is an inherent problem with TGDoubleEdit
   // Only allow numeric patterns.
   SetValidator(new TPXPictureValidator("[* ][-][*#][.][*#][{{E,e}[{-,+}]*#}]"));

   if (tagResId > 0)
      Tag = new TTag(parent,tagResId,tagText,module);
   else
      Tag = 0;

// No need for tempValidator... TGDoubleEdit provides this functionality
//   tempValidator = 0;
}

~TGUnitEdit ()
{
   Destroy();
   delete Tag;
}

public:

uint Transfer (void* buffer, TTransferDirection direction)
{
   // This method completely bypasses the validators ability to transfer data
   uint result;

   if (direction == tdSetData)
      SetValue(*(double *)buffer);
   else if (direction == tdGetData)
      *(double *)buffer = GetValue();

   result = sizeof(double);

   return result;
}

void GetValueString(double value,string &text)
{
   ostrstream os;
   ostrstream os1;

   os << setiosflags(ios::left)
      << setiosflags(ios::fixed)
      << setiosflags(ios::showpoint)
      << setw(width)
      << setprecision(precision)
      << value << ends;
   text = string( string(os.str()).strip(string::Both,' ') );
   delete[] os.str();

   // If string is too long, try scientific notation
   if (text.length() > width)
      {
      os1 << setiosflags(ios::left | ios::scientific | ios::showpoint | ios::uppercase)
          << setw(9+precision)
          << setprecision(precision)
          << value << ends;
      text = string( string(os1.str()).strip(string::Both,' ') );
      }
}

virtual void SetValue(double val)
{
   // val is coming in as internal base units...
   // must convert to display units before converting to string
   // and displaying it.
   string text;
   T data(val);
   double value;

   // Convert to display units
   value = data.ConvertTo(displayUnit).value();

   GetValueString(value,text);
   SetText(text.c_str());
}

virtual double GetValue()
{
   double value;
   char *text;

   text = new char[GetTextLen() + 1];
   GetText(text,GetTextLen()+1);
   if (text[0] == 0) // treat empty controls as zero
      value = 0;
   else
      {
      string valText( string(text).strip(string::Both,' ') );
      strcpy(text,valText.c_str());
      value = atof(text);
      }
   delete[] text;

   // Value is in display units. Convert to base units before returning
   T data(value * displayUnit);
   return data.value();
}

bool EnableWindow(bool enable)
{
// Use TGDoubleEdit's methods to handle validator
   if (enable)
      RestoreValidator();
   else
      SuspendValidator();

   if (Tag)
      Tag->EnableWindow(enable);

   return TGDoubleEdit::EnableWindow(enable);
}

void SetUnits(T newDisplayUnit,const char *newTag)
{
   displayUnit = newDisplayUnit;
   Tag->SetTag(newTag);
}

void DrawCell (TDC &dc, TRect &rc, const void *p, TGrid::Align align = TGrid::Right)
{
   // Because of the way the cells are drawn, the cell value is passed to this
   // function in internal base units (Newtons and meters) and not in display
   // units. To work around this problem we will have to draw the cell
   // ourselves.

   // Get the value
   double value = *static_cast<const double *>(p);

   // Convert to display units
   T data(value);
   value = data.ConvertTo(displayUnit).value();

   // TGDoubleEdit does not provide for scientific notation so
   // setup the display string here and send it to TGEdit.
   // Notice that TGDoubleEdit::DrawCell() is complete bypassed.
   if (value == 0 && blankNull == true)
      TGEdit::DrawCell(dc,rc,0,align);
   else
      {
      string text;
      GetValueString(value,text);
      TGEdit::DrawCell(dc,rc,text.c_str(),align);
      }
}

protected:
   class TTag : public TStatic
      {
      public:
         TTag(TWindow *parent,int resourceID,const char *tagText,TModule *module = 0) :
         TStatic(parent,resourceID,strlen(tagText),module),
         TagText(tagText)
            {
            }

         void SetupWindow()
            {
            TStatic::SetupWindow();
            FitToText();
            SetText(TagText.c_str());
            }

         void SetTag(const char *newTag)
            {
            TagText = newTag;
            FitToText();
            SetText(TagText.c_str());
            }

         void FitToText()
            {
            TClientDC dc(HWindow);
            TSize size(dc.GetTextExtent(TagText.c_str(),TagText.length()));

            TPoint p(size.cx,size.cy);
            dc.LPtoDP(&p,1);

            SetWindowPos(0,TRect(TPoint(0,0),p),SWP_NOMOVE | SWP_NOZORDER);
            }

         protected:
         string TagText;
      };

   T displayUnit;
   TTag *Tag;
};

typedef TGUnitEdit<FundamentalUnits::Length>    TGLengthEdit;
typedef TGUnitEdit<DerivedUnits::Length2>       TGLength2Edit;
typedef TGUnitEdit<DerivedUnits::Length4>       TGLength4Edit;
typedef TGUnitEdit<DerivedUnits::UnitWeight>    TGUnitWeightEdit;
typedef TGUnitEdit<DerivedUnits::Density>       TGDensityEdit;
typedef TGUnitEdit<DerivedUnits::Pressure>      TGPressureEdit;
typedef TGUnitEdit<DerivedUnits::LinearForce>   TGLinearForceEdit;
typedef TGUnitEdit<DerivedUnits::Force>         TGForceEdit;
typedef TGUnitEdit<DerivedUnits::Moment>        TGMomentEdit;

#endif                                      // __unitedit_h sentry.
