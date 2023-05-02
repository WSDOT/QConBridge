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

#include <qconbrdg\gt0valid.h>

TGTZeroValidator::TGTZeroValidator(const char *label,const char *tag) :
TExtremeValueValidator(0,ltGT),
Label(label),
Tag(tag)
{
}

void TGTZeroValidator::Error(TWindow* parent)
{
   TApplication *app = GetApplicationObject();
   ostrstream os;
   os << Label << " must be greater than " << ExtremeValue << " " << Tag << ends;
   if (parent)
      parent->MessageBox(os.str(),app->GetName(),MB_ICONEXCLAMATION | MB_OK);
   else
      ::MessageBox(0,os.str(),app->GetName(),MB_ICONEXCLAMATION | MB_OK | MB_TASKMODAL);

   delete[] os.str();
}

//
//
//
TGEZeroValidator::TGEZeroValidator(const char *label,const char *tag) :
TExtremeValueValidator(0,ltGE),
Label(label),
Tag(tag)
{
}

void TGEZeroValidator::Error(TWindow* parent)
{
   TApplication *app = GetApplicationObject();
   ostrstream os;
   os << Label << " must be greater than or equal to " << ExtremeValue << " " << Tag << ends;
   if (parent)
      parent->MessageBox(os.str(),app->GetName(),MB_ICONEXCLAMATION | MB_OK);
   else
      ::MessageBox(0,os.str(),app->GetName(),MB_ICONEXCLAMATION | MB_OK | MB_TASKMODAL);

   delete[] os.str();
}

//
//
//

TTagIncRangeValidator::TTagIncRangeValidator(const char *label,const char *tag,double min,double max) :
TIncRangeValidator(min,max),
Label(label),
Tag(tag)
{
}

void TTagIncRangeValidator::Error(TWindow* parent)
{
   TApplication *app = GetApplicationObject();
   ostrstream os;
   os << Label << " must be in the range " << Min << " " << Tag << " to " << Max << " " << Tag << ends;
   if (parent)
      parent->MessageBox(os.str(),app->GetName(),MB_ICONEXCLAMATION | MB_OK);
   else
      ::MessageBox(0,os.str(),app->GetName(),MB_ICONEXCLAMATION | MB_OK | MB_TASKMODAL);

   delete[] os.str();
}

