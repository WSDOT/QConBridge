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

#include <owltools/prpsheet.h>


TMyPropertyPage::TMyPropertyPage(TPropertySheet* parent, TResId resid, const char far*title,
                   TResId iconRes, TModule* module) :
      TPropertyPage(parent,resid,title,iconRes,module)
{
}

TMyPropertyPage::~TMyPropertyPage()
{
}

int TMyPropertyPage::Apply(TNotify far& notifyInfo)
{
   // This method is called when the OK or Apply buttons are pressed.
   // Since TPropertySheet is derived from TWindow, its calls to
   // CloseWindow() and CanClose() do not call TransferData, and hence the
   // data never gets transfered from the controls. We want to transfer the
   // data when the OK or Apply buttons are pressed. This method does exactly
   // that.

   int result = TPropertyPage::Apply(notifyInfo);

   // This is now done by OWL (introduced in version 5.01)
//   if (TransferBuffer)
//      TransferData(tdGetData);

   return result;
}

bool TMyPropertyPage::KillActive(TNotify far& notifyInfo)
{
   // This method is called when a page is about to loose activation
   // either because the user pressed the OK button or another page is
   // becoming activated. Returning true prevents the page from loosing
   // activation.  This is our chance to validate the controls.
   // TDialog::CanClose() (inherited from TWindow) calls CanClose() for
   // each of its child windows.
   int result = TPropertyPage::KillActive(notifyInfo);
   return result || !CanClose();
}


TMyPropertySheet* TMyPropertyPage::GetParentSheet() const
{
   return TYPESAFE_DOWNCAST(GetSheet(),TMyPropertySheet);
}


//
//
//

//
// Used internally [within this module] to match pages and indices
//
struct TPageInfo {
  const TPropertyPage* Page;      // Pointer to PropertyPage within sheet
  int                  Index;     // Index of the page [zero-based]
};


TMyPropertySheet::TMyPropertySheet(TWindow* parent, const char far* title, uint startPage,
                    bool isWizard, uint32 flags,
                    TModule* module) :
      TPropertySheet(parent, title, startPage, isWizard, flags, module)
{
   ActiveIndex = startPage;
}

TMyPropertySheet::~TMyPropertySheet()
{
}


//
// Internal routine used to identify the page at a particular index
//
static bool
matchPageIndex(TPropertyPage* /*pPage*/, int* pArray)
{
  if (pArray[0] == pArray[1])
    return true;
  else
    pArray[0]++;

  return false;
}

//
// Returns a pointer to the object representing the page at the
// specified index. Returns zero if no page is found.
//
TPropertyPage*
TMyPropertySheet::PageAtIndex(int index) const
{
  PRECONDITION(index < GetPageCount());
  int counts[]={0, index};
  TPropertyPage* page = CONST_CAST(TMyPropertySheet*,
                this)->FirstPageThat(TCondPageFunc(matchPageIndex), counts);
  return page;
}

//
// Internal callback used to find the index of a particular page
// within a property sheet.
//
static bool
matchPagePtr(TPropertyPage* pPage, TPageInfo* pageInfo)
{
  if (pPage == pageInfo->Page)
    return true;
  else {
    pageInfo->Index++;
    return false;
  }
}

//
// Returns the index of the specified page or '-1' if the
// specified page could not be found in the child list of
// the sheet object
//
int
TMyPropertySheet::IndexOfPage(const TPropertyPage* page) const
{
  PRECONDITION(page);
  TPageInfo pi = {page, 0};
  int index = CONST_CAST(TMyPropertySheet*,
     this)->FirstPageThat(TCondPageFunc(matchPagePtr), &pi) ? pi.Index : -1;
  return index;
}

//
// Returns the TProperPage* of the active page
//
TPropertyPage*
TMyPropertySheet::GetActivePage() const
{
  return (ActiveIndex < 0) ? 0 : PageAtIndex(ActiveIndex);
}

//
// Activates the page at the specified index
//
bool
TMyPropertySheet::SetActivePage(int index)
{
  PRECONDITION(index < GetPageCount());
  PRECONDITION(GetHandle());

  bool retval = SelectPage(index);
  if (retval)
     ActiveIndex = index;

  return retval;

/*
  // Retrieve pointer to page
  //
  TPropertyPage* page = PageAtIndex(index);
  CHECK(page);

  // Create the page/dialog if necessary
  //
  if (!page->GetHandle()) {
    if (!page->GetParentO())
      page->SetParent(this);
    page->Create();
  }

  // If it's the very first page, skip the positioning - we're in a
  // catch-22 [The sheet/tab can only be positioned after the first
  // page is created since we need a page to invoke MapDialogRect, and
  // the page can only be positioned after the sheet/tab has been
  // positioned.]
  //
  if (ActiveIndex == -1) {
    ActiveIndex = index;
    return true;
  }

  // Position page if it's not current active and hide current
  //
  TPropertyPage* curActive = GetActivePage();
  if (curActive != page) {
    PositionPage(page);
    if (curActive)
      curActive->ShowWindow(SW_HIDE);
  }

  // Update 'ActiveIndex'
  //
  ActiveIndex = index;
  return true;
*/
}

