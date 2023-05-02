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

#if !defined (NUMVALID_H_)
#define NUMVALID_H_

#include <owltools\owltldll.h>
#include <owl\applicat.h>
#include <owl\framewin.h>
#include <owl\validate.h>

class _OWLTOOLSCLASS TIncRangeValidator : public TPXPictureValidator
{
public:
   TIncRangeValidator(double min,double max);
   void Error(TWindow* parent);
   bool IsValid(const char far* s);
   uint Transfer(char far* s,void* buffer,TTransferDirection direction);

protected:
   double GetValue(const char far* s);
   double Min;
   double Max;
};


class _OWLTOOLSCLASS TExcRangeValidator : public TPXPictureValidator
{
public:
   TExcRangeValidator(double min,double max);
   void Error(TWindow* parent);
   bool IsValid(const char far* s);
   uint Transfer(char far* s,void* buffer,TTransferDirection direction);

protected:
   double GetValue(const char far* s);
   double Min;
   double Max;
};

class _OWLTOOLSCLASS TExtremeValueValidator : public TPXPictureValidator
{
public:
   enum LimitType {ltLT,ltLE,ltGT,ltGE};

   TExtremeValueValidator(double extremeValue,LimitType lt);
   void Error(TWindow* parent);
   bool IsValid(const char far* s);
   uint Transfer(char far* s,void* buffer,TTransferDirection direction);

protected:
   double GetValue(const char far* s);
   double ExtremeValue;
   LimitType lType;
};

#endif // NUMVALID_H_