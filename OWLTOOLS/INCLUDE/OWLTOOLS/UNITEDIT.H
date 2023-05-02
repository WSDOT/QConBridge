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

#if !defined(__unitedit_h)              // Sentry, use file only if it's not already included.
#define __unitedit_h

#include <owltools\owltldll.h>
#include <owltools\nbredit.h>
#include <owl\static.h>
#include <owl\dc.h>
#include <winsys\geometry.h>
#include <esl\units.h>

template <class T>
class _OWLCLASS TUnitEdit : public TNumberEdit
{
public:
TUnitEdit(TWindow* parent, int resourceID,
          const T DisplayUnit,uint Width,
          uint Precision,
          int tagResId = 0,const char *tagText = 0,
          TModule* module = 0) :
TNumberEdit(parent,resourceID,Width,Precision,module),
displayUnit(DisplayUnit)
{
   if (tagResId > 0)
      Tag = new TTag(parent,tagResId,tagText,module);
   else
      Tag = 0;

   tempValidator = 0;
}

~TUnitEdit ()
{
   Destroy();
   delete Tag;
}

public:

uint Transfer (void* buffer, TTransferDirection direction)
{
   // Note that this method bypasses the validators ability to transfer data
   uint result;

   if (direction == tdSetData)
      SetValue(*(double *)buffer);
   else if (direction == tdGetData)
      *(double *)buffer = GetValue();

   result = sizeof(double);

   return result;
}

void SetValue(double val)
{
   T data(val);
   TNumberEdit::SetValue(data.ConvertTo(displayUnit).value());
}

void SetValueEx(double val)
{
   TNumberEdit::SetValue(val);
}

double GetValue()
{
   T data(TNumberEdit::GetValue() * displayUnit);
   return data.value();
}

double GetValueEx()
{
   return TNumberEdit::GetValue();
}

bool EnableWindow(bool enable)
{
   bool retval;

   if (enable)
      {
      // If the window is being enabled, restore the validator
      Validator = tempValidator;
      tempValidator = 0;
      }
   else
      {
      // The window is being disabled. Hide the validator so that
      // the contents of the edit control will not be validated
      tempValidator = Validator;
      Validator = 0;
      }



   retval = TNumberEdit::EnableWindow(enable);
   if (Tag)
      Tag->EnableWindow(enable);

   return retval;
}

void SetUnits(T newDisplayUnit,const char *newTag)
{
   displayUnit = newDisplayUnit;
   if (Tag)
      Tag->SetTag(newTag);
}

void ShowTag(int cmdShow)
{
   if (Tag)
      Tag->Show(cmdShow);
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
   TValidator *tempValidator;
};

typedef TUnitEdit<FundamentalUnits::Length>    TLengthEdit;
typedef TUnitEdit<DerivedUnits::Length2>       TLength2Edit;
typedef TUnitEdit<DerivedUnits::Length4>       TLength4Edit;
typedef TUnitEdit<DerivedUnits::UnitWeight>    TUnitWeightEdit;
typedef TUnitEdit<DerivedUnits::Pressure>      TPressureEdit;
typedef TUnitEdit<DerivedUnits::LinearForce>   TLinearForceEdit;
typedef TUnitEdit<DerivedUnits::Force>         TForceEdit;
typedef TUnitEdit<DerivedUnits::Moment>        TMomentEdit;
typedef TUnitEdit<DerivedUnits::Density>       TDensityEdit;

#endif                                      // __unitedit_h sentry.

