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

#define  STRICT
#include <windows.h>
#pragma hdrstop

// Turn off warning: Parameter '' is never used
#pragma argsused

// Every DLL has an entry point LibMain || DllEntryPoint
// and an exit point WEP.
#if defined(__FLAT__)
BOOL WINAPI DllEntryPoint( HINSTANCE hinstDll,
                           DWORD fdwRreason,
                           LPVOID plvReserved)
#else /* not flat model  */
int FAR PASCAL LibMain( HINSTANCE hInstance,
                        WORD wDataSegment,
                        WORD wHeapSize,
                        LPSTR lpszCmdLine )
#endif /* __FLAT */
{
#ifndef  __FLAT__

// The startup code for the DLL initializes the local heap(if there is one)
// with a call to LocalInit which locks the data segment.

    if ( wHeapSize != 0 )
        UnlockData( 0 );
#endif

    return 1;   // Indicate that the DLL was initialized successfully.
}

// Turn off warning: Parameter '' is never used
#pragma argsused

int FAR PASCAL WEP ( int bSystemExit )
{
    return 1;
}
