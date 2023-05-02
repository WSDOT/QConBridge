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

#include <code\version.h>
#include <string.h>

// To change the default edition at compile-time, choose
// Edit Local Options | Compiler | Defines from the project
// node speed menu. Change the EDITION define to one of the
// #define's found in version.h. Recompile
codeEdition Edition = EDITION;

codeEdition GetCodeEdition()
{
   return Edition;
}

codeEdition SetCodeEdition(codeEdition edition)
{
   codeEdition prevEdition = Edition;
   Edition = edition;
   return prevEdition;
}

void GetCodeEditionString(char *buffer,int maxChar)
{
   char *editionName[2] = {"LRFD First Edition 1994",
                           "LRFD First Edition 1994 with 1996 Interims"};

   buffer[0] = 0;
   strncat(buffer,editionName[GetCodeEdition()],maxChar);
}
