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

#if !defined OWLTOOLS_OWLTLDLL_H_
#define OWLTOOLS_OWLTLDLL_H_

#if defined (BUILDOWLTOOLS) && !defined(_OWLTOOLSLIB)
#define _OWLTOOLSCLASS __declspec(dllexport)
#define _OWLTOOLSFUNC  __declspec(dllexport)
#elif defined(_OWLTOOLSLIB)
#define _OWLTOOLSCLASS
#define _OWLTOOLSFUNC
#else
#define _OWLTOOLSCLASS __declspec(dllimport)
#define _OWLTOOLSFUNC
#endif

#endif