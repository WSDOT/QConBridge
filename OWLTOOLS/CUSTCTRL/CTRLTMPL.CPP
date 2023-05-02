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

#include <owl\dialog.h>
#include <owl\control.h>
#include <owl\gdiobjec.h>
#include <owl\edit.h>
#include <owl\checkbox.h>
#include <custcntl.h>
#include "ctrltmpl.rh"

#if defined(__FLAT__)
#define CTLTITLE        94      /* max size of control text */
#endif

char szClassName[] = "Template";

//===================================================
// Custom control code
//===================================================

class _OWLCLASS TCustomControl : public TWindow
{
public:
   TCustomControl(HWND wnd,int AnId,CREATESTRUCT *lpCreate,TModule *Module = 0) :
   TWindow(wnd,Module)
{
   Attr.Id = AnId;
   Attr.Style = lpCreate->style;
   Attr.ExStyle = lpCreate->dwExStyle;
   Attr.W = lpCreate->cx;
   Attr.H = lpCreate->cy;
   Attr.X = lpCreate->y;
   Attr.Y = lpCreate->x;
   SetBkgndColor(RGB(127,127,127));
}

~TCustomControl() {}

virtual char * GetClassName()
{ return szClassName; }

protected:
UINT EvGetDlgCode(MSG far *)
{ return DLGC_WANTARROWS | DLGC_WANTCHARS; }
void EvLButtonDown(uint,TPoint &)
{ MessageBeep(-1); }
void EvPaint()
{ TPaintDC dc(*this);
  TRect &rect = *(TRect *)&dc.Ps.rcPaint;
  Paint(dc,dc.Ps.fErase,rect);
}
void Paint(TDC &dc,BOOL,TRect &);
void CleanupWindow()
{ TWindow::CleanupWindow();
  SetWindowLong(GWL_WNDPROC,(long)DefaultProc);
}

private:
DECLARE_RESPONSE_TABLE(TCustomControl);
};

DEFINE_RESPONSE_TABLE1(TCustomControl,TWindow)
  EV_WM_GETDLGCODE,
  EV_WM_LBUTTONDOWN,
  EV_WM_PAINT,
END_RESPONSE_TABLE;

void TCustomControl::Paint(TDC &dc,BOOL bRepaint,TRect &r)
{
   TWindow::Paint(dc,bRepaint,r);
   TBrush BackBrush(TWindow::BkgndColor);

   if (bRepaint)
      dc.FillRect(r,BackBrush);
}

//===================================================
// Dialog editor and DLL code
//===================================================

struct StyleDialogBuf
{
   char Name[CTLTITLE];
   char ID[CTLTITLE];
   WORD HScroll;
   WORD TabStop;
   WORD Disabled;
   WORD Visible;
   WORD VScroll;
   WORD Border;
   char Caption[CTLTITLE];
};

class _OWLCLASS TCustomControlDlg : public TDialog
{
public:
   TCustomControlDlg(TWindow *parent,
                     const char *name,
                     LPFNSTRTOID PStrToId,
                     LPFNIDTOSTR PIdToStr,
                     TModule *module);
protected:
   LPFNIDTOSTR lpfnIdToStr;
   LPFNSTRTOID lpfnStrToId;

   TEdit *EdName;

   bool CanClose();
};

TCustomControlDlg::TCustomControlDlg(TWindow *parent,
                                     const char *name,
                                     LPFNSTRTOID PStrToId,
                                     LPFNIDTOSTR PIdToStr,
                                     TModule *module) :
TDialog(parent,name,module),
lpfnStrToId(PStrToId),
lpfnIdToStr(PIdToStr)
{
   EdName = new TEdit(this,ID_EDNAME,CTLTITLE);
   TStatic *st = new TStatic(this,ID_STIDVALUE,CTLTITLE);
   st->EnableTransfer();
   new TCheckBox(this,ID_HSCROLL);
   new TCheckBox(this,ID_TABSTOP);
   new TCheckBox(this,IDC_EDITABLE);
   new TCheckBox(this,ID_CKVISIBLE);
   new TCheckBox(this,ID_VSCROLL);
   new TCheckBox(this,ID_CKBORDER);
   new TEdit(this,ID_EDCAPTION,CTLTITLE);
}

bool TCustomControlDlg::CanClose()
{
   StyleDialogBuf *temp = (StyleDialogBuf *)TransferBuffer;

   if ((*lpfnStrToId)(temp->Name))
      return true;

   return false;
}

//===================================================
// Style(), Flags(), and Info()
//===================================================
// extern "C"

BOOL FAR PASCAL _export
TemplateStyle(HWND HWindow,
              HANDLE hCtlStyle,
              LPFNSTRTOID StrToId,
              LPFNIDTOSTR IdToStr)
{
   LPRWCTLSTYLE PStyle = (LPRWCTLSTYLE)GlobalLock( hCtlStyle );
   BOOL ReturnValue = FALSE;
   TWindow RW_Window(HWindow);
   if (PStyle)
      {
      StyleDialogBuf TXBuffer;
      (*IdToStr)(PStyle->wId,TXBuffer.Name,CTLTITLE);
      strcpy(TXBuffer.Caption,PStyle->szTitle);
      itoa(PStyle->wId,TXBuffer.ID,10);
      TXBuffer.HScroll  = (PStyle->dwStyle & WS_HSCROLL ) ? BF_CHECKED : BF_UNCHECKED;
      TXBuffer.VScroll  = (PStyle->dwStyle & WS_VSCROLL ) ? BF_CHECKED : BF_UNCHECKED;
      TXBuffer.Border   = (PStyle->dwStyle & WS_BORDER  ) ? BF_CHECKED : BF_UNCHECKED;
      TXBuffer.Disabled = (PStyle->dwStyle & WS_DISABLED) ? BF_CHECKED : BF_UNCHECKED;
      TXBuffer.TabStop  = (PStyle->dwStyle & WS_TABSTOP ) ? BF_CHECKED : BF_UNCHECKED;
      TXBuffer.Visible  = (PStyle->dwStyle & WS_VISIBLE ) ? BF_CHECKED : BF_UNCHECKED;

      TCustomControlDlg PDlg(&RW_Window,
                             (LPSTR)MAKEINTRESOURCE(CTRL_DLG),
                             (LPFNSTRTOID)StrToId,
                             (LPFNIDTOSTR)IdToStr,
                             Module);
      PDlg.SetTransferBuffer(&TXBuffer);
      ReturnValue = ( IDOK == PDlg.Execute() );
      if (ReturnValue)
         {
         PStyle->dwStyle &= ~(WS_HSCROLL  |
                              WS_VSCROLL  |
                              WS_BORDER   |
                              WS_DISABLED |
                              WS_TABSTOP  |
                              WS_VISIBLE);
         if (TXBuffer.HScroll == BF_CHECKED)
            PStyle->dwStyle |= WS_HSCROLL;
         if (TXBuffer.VScroll == BF_CHECKED)
            PStyle->dwStyle |= WS_VSCROLL;
         if (TXBuffer.Border == BF_CHECKED)
            PStyle->dwStyle |= WS_BORDER;
         if (TXBuffer.Disabled == BF_CHECKED)
            PStyle->dwStyle |= WS_DISABLED;
         if (TXBuffer.TabStop == BF_CHECKED)
            PStyle->dwStyle |= WS_TABSTOP;
         if (TXBuffer.Visible == BF_CHECKED)
            PStyle->dwStyle |= WS_VISIBLE;
         PStyle->wId = HIWORD( (*StrToId)(TXBuffer.Name) );
         }
      GlobalUnlock(hCtlStyle);
      }
   return ReturnValue;
}

// extern "C"
WORD FAR PASCAL _export
TemplateFlags( DWORD, LPSTR szBuf, WORD )
{
   wsprintf(szBuf,(LPSTR)"0");
   return lstrlen(szBuf);
}

// extern "C"
HANDLE FAR PASCAL _export
TemplateInfo(void)
{
   HANDLE hInfo = GlobalAlloc(GHND,sizeof(RWCTLINFO));
   if (hInfo)
      {
      LPRWCTLINFO Info = (LPRWCTLINFO)GlobalLock(hInfo);
      Info->wVersion = 0x0100;
      Info->wCtlTypes = 1;
      lstrcpy(Info->szClass,szClassName);
      strcpy(Info->szTitle,szClassName);
      Info->Type[0].wType = 0;
      Info->Type[0].wWidth = 32;
      Info->Type[0].wHeight = 32;
      lstrcpy(Info->Type[0].szDescr,szClassName);
      Info->Type[0].dwStyle = WS_VISIBLE | WS_CHILD | WS_TABSTOP;
      Info->Type[0].hToolBit = LoadBitmap(Module->GetInstance(),MAKEINTRESOURCE(CTRL_BMP));
      Info->Type[0].hDropCurs = LoadCursor(Module->GetInstance(),MAKEINTRESOURCE(IDC_CROSS));
      GlobalUnlock(hInfo);
      }
   return hInfo;
}

//===================================================
// ListClasses()
//===================================================
extern "C"
HANDLE PASCAL _export
ListClasses(LPSTR, //szAppName,
            WORD, // wVersion,
            LPFNLOADRES, // fnLoad,
            LPFNEDITRES) //fnEdit)
{
   HANDLE hClasses = GlobalAlloc(GHND,sizeof(CTLCLASSLIST));
   if (hClasses)
      {
      LPCTLCLASSLIST Classes = (LPCTLCLASSLIST)GlobalLock(hClasses);
      Classes->nClasses = 1;
      Classes->Classes[0].fnRWInfo = TemplateInfo;
      Classes->Classes[0].fnRWStyle = TemplateStyle;
      Classes->Classes[0].fnFlags = (LPFNFLAGS)TemplateFlags;
      lstrcpy(Classes->Classes[0].szClass,szClassName);
      GlobalUnlock(hClasses);
      }
   return hClasses;
}

//===================================================
// Window procedure for the control
//===================================================
LONG FAR PASCAL _export
TemplateWndProc(HWND HWindow,WORD wMsg,WORD wParam,LONG lParam)
{
   CREATESTRUCT *lpCreate = (CREATESTRUCT*)lParam;
   if (wMsg == WM_CREATE)
      {
      int id = GetWindowWord(HWindow,GWW_ID);

      TWindow *Ctrl = new TCustomControl(HWindow,id,lpCreate,::Module);
      SetWindowLong(HWindow,0,(long)Ctrl);
      return 0;
      }
   else if (wMsg == WM_NCDESTROY)
      {
      TWindow *Ctrl = (TWindow *)GetWindowLong(HWindow,0);
      delete Ctrl;
      return 0;
      }
   else
      return DefWindowProc(HWindow,wMsg,wParam,lParam);
}

static WNDCLASS wc =
{
   CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_GLOBALCLASS,
   (WNDPROC)TemplateWndProc,
   0,
   4, // Extra bytes for OWL pointer and RW flag
   0,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL
};

//===================================================
// Static TModule and LibMain
//===================================================
#if defined OWLTLSCG
static TModule module("OWLTLSCG.DLL",FALSE);
#elif defined OWLTLS16
static TModule module("OWLTLS16.DLL",FALSE);
#else  // OWLTLS32
static TModule module("OWLTLS32.DLL",FALSE);
#endif

#if defined(__FLAT__)
BOOL WINAPI DllEntryPoint( HINSTANCE hInstance,
                           DWORD fdwRreason,
                           LPVOID plvReserved)
#else /* not flat model  */
int FAR PASCAL LibMain(HINSTANCE hInstance,
                       WORD,
                       WORD wHeapSize,
                       LPSTR lpszCmdLine)
#endif
{

#if !defined (__FLAT__)
   if (wHeapSize > 0)
      UnlockData(0);
#endif

#if defined (__FLAT__)
   module.InitModule(hInstance,0);
#else
   module.InitModule(hInstance,lpszCmdLine);
#endif
   ::Module = &module;

   wc.hCursor = LoadCursor(NULL,IDC_ARROW);
   wc.hInstance = hInstance;
   wc.lpszClassName = szClassName;
   return RegisterClass(&wc) ? TRUE : FALSE;
}
