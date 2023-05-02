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

#if !defined OWLTOOLS_PRPSHEET_H_
#define OWLTOOLS_PRPSHEET_H_

#include <owltools\owltldll.h>
#include <owl\propsht.h>

class _OWLTOOLSCLASS TMyPropertySheet;
class _OWLTOOLSCLASS TMyPropertyPage : public TPropertyPage
{
public:
   TMyPropertyPage(TPropertySheet* parent, TResId resid, const char far*title = 0,
                   TResId iconRes = 0, TModule* module = 0);
   ~TMyPropertyPage();

   int Apply(TNotify far& notifyInfo);
   bool KillActive(TNotify far& notifyInfo);
   TMyPropertySheet* GetParentSheet() const;
};
//
//-----------------------------------------------------------------------
//

class _OWLTOOLSCLASS TMyPropertySheet : public TPropertySheet
{
public:
   TMyPropertySheet(TWindow* parent, const char far* title, uint startPage = 0,
                    bool isWizard = false, uint32 flags = PSH_DEFAULT,
                    TModule* module = 0);
   ~TMyPropertySheet();

    // Helper routines to ask for info. about pages of sheet
    //
    TPropertyPage*  PageAtIndex(int index) const;
    int             IndexOfPage(const TPropertyPage* page) const;
    TPropertyPage*  GetActivePage() const;
    bool            SetActivePage(int index);
    int             TabIndexOfPage(const TPropertyPage* page) const;
    TPropertyPage*  PageAtTabIndex(int index) const;

protected:
    int             ActiveIndex;        // Index of active page

   void PositionPage(TPropertyPage* page);
};


#endif // OWLTOOLS_PRPSHEET_H_
