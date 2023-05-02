///////////////////////////////////////////////////////////////////////
// Bridge32 - Bridge Modeling Framework
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
// May 1997 - Created, Richard Brice, PE
// Oct 1999 - Released as Open Source

#if !defined BRGLDS_H_
#define BRGLDS_H_

// Bridge Load Types
enum blType {blConc,blUnif};
typedef blType blType;

// Load
//
// Represents a generic loading applied to a bridge structure
class Load
{
public:
   Load(const int Span,const double loc,const double Mag,const bool isFract);
   void Span(const int spanId);
   int Span() const;
   void IsFract(bool isFract);
   bool IsFract() const;
   void Start(double loc);
   double Start() const;
   virtual blType Type() const = 0;

private:
   int span;
   bool fract;
   double start;
   double mag;
};

// ConcLd
//
// Represents a concentrated load applied to a span
class ConcLd : public Load
{
public:
   ConcLd(const int Span,const double loc,const double Mag,const bool isFract);
   blType Type() const;
};

// UnifLd
//
// Represents a uniform load applied to a span
class UnifLd : public Load
{
public:
   UnifLd(const int Span,const double sloc,double eloc,const double Mag,const bool isFract);
   blType Type() const;
   void End(double loc);
   double End() const;

private:
   double end;
};

#endif // BRGLDS_H_

