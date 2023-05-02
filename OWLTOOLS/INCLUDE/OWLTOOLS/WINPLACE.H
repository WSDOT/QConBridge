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

#if !defined (OWLTOOLS_WINPLACE_H_)
#define OWLTOOLS_WINPLACE_H_

#include <owltools\owltldll.h>
#include <owl\window.h>

enum TWindowPlacementFlag
{
   wpfRestoreToMaximized = WPF_RESTORETOMAXIMIZED,
   wpfSetMinPosition = WPF_SETMINPOSITION,
};

class _OWLTOOLSCLASS TWindowPlacement : public WINDOWPLACEMENT
{
public:
   TWindowPlacement();
   TWindowPlacement(WINDOWPLACEMENT& wp);
   virtual ~TWindowPlacement();

   TWindowPlacementFlag GetFlags() const;
   void SetFlags(TWindowPlacementFlag uFlags);

   uint GetShowState() const;
   void SetShowState(uint uShowCmd);

   void GetMinPosition(TPoint& ptMin) const;
   void SetMinPosition(const TPoint& ptMin);

   void GetMaxPosition(TPoint& ptMax) const;
   void SetMaxPosition(const TPoint& ptMax);

   void GetNormalPosition(TRect& rcNormal) const;
   void SetNormalPosition(const TRect& rcNormal);

   void SaveToRegistry(TWindow* window,const char* regKey);
   void RestoreFromRegistry(TWindow* window,const char* regKey);
};

#endif // OWLTOOLS_WINPLACE_H_