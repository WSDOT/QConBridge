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

#if !defined ESL_ENGNOTAT_H_
#define ESL_ENGNOTAT_H_

#include <iostream.h>
#include <iomanip.h>
#include <stdlib.h>
#include <string.h>

struct EngNotationData
{
   double value;
   int nDigits;
   int sigDigits; // if 0, treat nDigits as the number of decimal points
                  // otherwise, treat nDigits as the number of significant
                  // digits
};

inline ostream& EngNotation(ostream &os,EngNotationData data)
{
   char buf[50];
   char *buffer;
   int decPos;
   int negative;
   int nDigits;
   int endOfBuf;

   nDigits = (data.sigDigits) ? data.nDigits : 4+data.nDigits;

   char *tempBuffer = ecvt(data.value,nDigits,&decPos,&negative);

   buffer = buf;

   if (negative)
      {
      *buffer++ = '-';
      }

   strcpy(buffer,tempBuffer);

   int shift = decPos % 3;

   int exponent = decPos - shift;
   if (shift == 0 || decPos < 0)
      {
      shift += 3;
      exponent -= 3;
      }

   memmove(&buffer[shift+1],&buffer[shift],strlen(&buffer[shift])+1);

   buffer[shift] = '.';

   endOfBuf = (data.sigDigits) ? strlen(buffer) : shift + data.nDigits + 1;

   buffer[endOfBuf++] = 'e';

   buffer[endOfBuf++] = (exponent < 0) ? '-' : '+';

   if (abs(exponent) < 10)
      buffer[endOfBuf++] = '0';

   itoa(abs(exponent),&buffer[endOfBuf],10);

   if (negative)
      buffer--;

   os << buffer;

   return os;
}

#if !defined (IOMANIPdeclare)
inline omanip<EngNotationData> EngNotation(double value,int nDigits,int sigDigits = 0)
   {
   EngNotationData data;
   data.value = value;
   data.nDigits = nDigits;
   data.sigDigits = sigDigits;
   return omanip<EngNotationData>(EngNotation,data);
   }
#else
inline OMANIP<EngNotationData> EngNotation(double value,int nDigits,int sigDigits = 0)
   {
   EngNotationData data;
   data.value = value;
   data.nDigits = nDigits;
   data.sigDigits = sigDigits;
   return OMANIP<EngNotationData>(EngNotation,data);
   }
#endif

#endif // ESL_ENGNOTAT_H_
