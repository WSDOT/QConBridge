///////////////////////////////////////////////////////////////////////
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

#if !defined ESL_STRUTIL_H_
#define ESL_STRUTIL_H_

#include <esl\esldll.h>

#if defined _MSC_VER
#define bool unsigned int
#define false 0
#define true  1
#endif

int    ESLFUNC geti  ( char* line, int col, int width);
double ESLFUNC getf  ( char* line, int col, int width);
void   ESLFUNC getstg( char* line, int col, int width, char* stg, bool leftJustify = false);
char   ESLFUNC getltr(char* line,int col);
void   ESLFUNC puti  ( char* line, int num, int start, int width);
void   ESLFUNC putf  ( char* line, double num, int start, int width, int dec);
void   ESLFUNC putchr( char* line, char c,int col);
void   ESLFUNC putstg( char* line, const char stg[], int col, int len);
void   ESLFUNC initstg(char* line, char c, int numChar);
bool   ESLFUNC IsBlankStg(char* line,int numChar);

#define DT_METER 1
#define DT_INCH  2
#define DT_FEET  3

int    ESLFUNC GetFeet(double dim,int dimType);
double ESLFUNC GetInch(double dim,int dimType);

#endif // ESL_STRUTIL_H_
