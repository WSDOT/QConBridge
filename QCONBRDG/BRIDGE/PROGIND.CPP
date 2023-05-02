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

#include "progind.h"
#include <string.h>

ProgressIndicator::ProgressIndicator(CancelProc pfnCancelProc) :
pfnCancelProc(pfnCancelProc)
{
   message = 0;
   newMessage = false;
}

ProgressIndicator::~ProgressIndicator()
{
   delete[] message;
}

void ProgressIndicator::SetProgressMsg(const char *msg)
{
   delete[] message;
   message = new char[strlen(msg)+1];
   strcpy(message,msg);
   newMessage = true;
}

char *ProgressIndicator::GetProgressMsg(char *msg,int nChar)
{
   msg[0] = 0;
   strncat(msg,message,nChar);
   newMessage = false;
   return msg;
}

int ProgressIndicator::GetMessageLen()
{
   return strlen(message);
}

int ProgressIndicator::NewMessage()
{
   return newMessage;
}

int ProgressIndicator::ShouldContinue()
{
   return (*pfnCancelProc)();
}

