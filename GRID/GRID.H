/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// grid.h                                                                  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#ifndef GRID_H
#define GRID_H

#define HRESIZE 500

#if !defined(OWL_SCROLLBAR_H)
#include <owl\scrollba.h>
#endif

#if !defined(OWL_DIALOG_H)
#include <owl\dialog.h>
#endif

#if !defined(OWL_COLOR_H)
#include <owl\color.h>
#endif

#if !defined(OWL_GDIOBJEC_H)
#include <owl\gdiobjec.h>
#endif
 
class _GRIDCLASS TGDialog : public TDialog
   {
   protected:
      TColor bkg;
      TBrush *brush;
      HBRUSH EvCtlColor(HDC hDC, HWND hWndChild, uint ctlType);
      virtual HBRUSH CtlColors (HDC hDC); 

   public:
      TGDialog (TWindow *p, TResId resId, TColor bkg, TModule *module = 0);
      TGDialog (TWindow *p, TResId resId, TModule *module = 0); 
      ~TGDialog ();
      virtual void   SetColor  (TColor bkg);
      virtual TColor BkgColor  () { return bkg; }
      virtual HBRUSH BkgBrush  () { return *brush; }

   DECLARE_RESPONSE_TABLE (TGDialog);
   };

class _GRIDCLASS TGCheckBox;
class _GRIDCLASS TGComboBox;
class _GRIDCLASS TGEdit;
class _GRIDCLASS TGStatic;
class _GRIDCLASS TGButton;

void _GRIDFUNCTION Frame3d (TDC &dc, TRect &rc, int w, TColor topleft, TColor bottomright);
void _GRIDFUNCTION Frame3d (TDC &dc, int x0, int y0, int x1, int y1, int w, TColor topleft, TColor bottomright);


#define defAttrib (gAllBorders  | \
                   gHideScroll  | \
                   gCenterTitle | \
                   gLineSwapOn  | \
                   gHorzShrink  | \
                   gVertShrink)
#define MK_TGRID 0x20

class _GRIDCLASS TGrid : public TControl
   {
   public:
      // enumerations ----------------------------------------------------------
      typedef enum Align        { Left =DT_LEFT, Center =DT_CENTER, Right =DT_RIGHT } Align;
      typedef enum Attributes   { gNoBorder         = 0x0000,
                                  gVScroll          = 0x0001,
                                  gHScroll          = 0x0002,
                                  gTitle            = 0x0004,
                                  gVBar             = 0x0008,
                                  gNoScroll         = 0x000c,
                                  gAllBorders       = 0x000f,
                                  gHideHScroll      = 0x0010,
                                  gHideVScroll      = 0x0020,
                                  gHideScroll       = 0x0030,
                                  gDisableHScroll   = 0x0040,
                                  gDisableVScroll   = 0x0080,
                                  gDisableScroll    = 0x00c0,
                                  gCenterTitle      = 0x0104,
                                  gLineSwapOn       = 0x0200,
                                  gSizeColumn       = 0x0400,
                                  gHorzShrink       = 0x0800,
                                  gSingleSelection  = 0x1000,
                                  gMultipleSelection= 0x2000, 
                                  gAutoSelection    = 0x3000,
                                  gArrowCursor      = 0x4000,
                                  gDefCursor        = 0x0000,
                                  gVertShrink       = 0x8000
                                } Attributes;
      typedef enum CursorType   { DefCursor, ArrowCursor } CursorType;
      typedef enum EnterKey     { EnterNextColumn, NextLine, EndDialog, DoNothing } EnterKey;
      typedef enum LineMarker   { ArrowOnly, ArrowAndText, ReverseColor } LineMarker;
      typedef enum LineSelect   { None, Single, Multiple, SingleAutoSelect } LineSelect;
      typedef enum TabKey       { TabNextColumn, AutoNextLine, NextControl } TabKey;
      typedef enum TitleColor   { TitleText, TitleBackground, TitleHilite, TitleShadow, TitleFrame } TitleColor;
      typedef enum Type         { gEdit, gStatic, gCombo, gButton, gCheck, gDateEdit, gTimeEdit, gDoubleEdit, gLongEdit, gShortEdit } Type;
      typedef enum Visible      { gControl, gFlat, gLocked, gEmpty } Visible;

   public:
      class _GRIDCLASS Client : public TControl
         {
         protected:
            TGrid *grid;
            void Paint (TDC &dc, bool erase, TRect &clip);
            HBRUSH EvCtlColor (HDC hDC, HWND hWndChild, uint ctlType);
            bool EvEraseBkgnd    (HDC hwnd) { return true; }
            void EvKillFocus     (HWND hWndGetFocus);
            void EvLButtonDown   (uint modKeys, TPoint &point);
            void EvLButtonUp     (uint modKeys, TPoint &point);
            void EvLButtonDblClk (uint modKeys, TPoint &point);
            void EvRButtonDown   (uint modKeys, TPoint &point);
            void EvRButtonUp     (uint modKeys, TPoint &point);
            void EvRButtonDblClk (uint modKeys, TPoint &point);
            bool EvSetCursor     (HWND hwnd, uint hitTest, uint mouseMsg);
            void EvSetFocus      (HWND hwndLooseFocus);
            LRESULT WindowProc   (uint msg, WPARAM wParam, LPARAM lParam);

         public:
            char * GetClassName ();
            Client (TGrid *g);
            bool Create ();

         DECLARE_RESPONSE_TABLE (TGrid::Client);
         };


      class _GRIDCLASS Column
         {
         public:
            TGrid *grid;
            TControl *control;
            Column *next;
            Column *previous;
            int left;
            int width;
            int Hidden;                  // Charles R. Cordell
            TGrid::Align align;
            TGrid::Type  type;
            string title;
            TFont *font;

            Column (TControl *c, Column *prev, TGrid *grid, const char *t);
           ~Column ();
            bool CanKillFocus ();
            int CtrlWidth () const;
            int CtrlHeight () const;
            void HideControl ();
            bool SetFocus ();
            void SetFont ();
            void SetFont (const char *facename, int size, int weight, bool italic);
            void SetTitle (const char *newtitle);
            void ShowControl ();
            char * GetClassName ();
         };

      class _GRIDCLASS VScroll : public TScrollBar
         {
         protected:
            void EvKeyDown (uint key, uint repeat, uint flags);
            uint minRange;
            uint maxRange;
            int factor;

         public:
            TGrid *grid;

            VScroll (TGrid *g);

            void SetLRange (uint min, uint max, bool draw);
            void SetLPosition (uint pos);

            void SBBottom ();
            void SBLineDown ();
            void SBLineUp ();
            void SBPageDown ();
            void SBPageUp ();
            void SBThumbPosition (int pos);
            void SBThumbTrack (int pos);
            void SBTop ();

            DECLARE_RESPONSE_TABLE(TGrid::VScroll);
         };

      class _GRIDCLASS HScroll : public TScrollBar
         {
         public:
            TGrid *grid;

            HScroll (TGrid *g);

            void SBBottom ();
            void SBLineDown ();
            void SBLineUp ();
            void SBPageDown ();
            void SBPageUp ();
            void SBThumbPosition (int pos);
            void SBThumbTrack (int pos);
            void SBTop ();
         };

      class _GRIDCLASS Title : public TControl
         {
         friend TGrid;

         private:
            bool capture;
            bool showBack;
            bool visible;
            int xorg;
            bool MoveMouseCursor ();
            Column *column;

         protected:
            TGrid *grid;

            bool  EvSetCursor (HWND hwnd, uint hitTest, uint mouseMsg);
            void  EvLButtonDblClk (uint modKeys, TPoint &p) { grid->TitleLButtonDblClk (grid->FindColumn (p)); }
            void  EvLButtonDown (uint modKeys, TPoint &p);
            void  EvLButtonUp (uint modKeys, TPoint &p);
            void  EvRButtonDblClk (uint modKeys, TPoint &p) { grid->TitleRButtonDblClk (grid->FindColumn (p)); }
            void  EvRButtonDown (uint modKeys, TPoint &p)   { grid->TitleRButtonDown   (grid->FindColumn (p)); }
            void  EvRButtonUp (uint modKeys, TPoint &p)     { grid->TitleRButtonUp     (grid->FindColumn (p)); }
            void  EvMouseMove (uint modKeys, TPoint &p);
            void  DrawCell (TDC &dc, int x, int w, const char *p, TGrid::Align a);
            char *GetClassName ();
            void  GetWindowClass (WNDCLASS &wc);
            int   CalcHeight ();
            void  Paint (TDC &dc, bool erase, TRect &);
            LRESULT WindowProc   (uint msg, WPARAM wParam, LPARAM lParam);

         public:
            Title (TGrid *g, bool visible);
            bool Create ();

            DECLARE_RESPONSE_TABLE (TGrid::Title);
         };

      class _GRIDCLASS VerticalBar : public TControl
         {
         friend TGrid;

         protected:
            TGrid *grid;
            bool lineNumber;
            int charWidth;

            int CalcWidth ();                                  
            void DrawCell (TDC &dc, TRect &rc, int line);
            void EvLButtonDblClk (uint modKeys, TPoint &point);
            void EvLButtonDown (uint modKeys, TPoint &point);
            void EvLButtonUp (uint modKeys, TPoint &point);
            void EvRButtonDblClk (uint modKeys, TPoint &point);
            void EvRButtonDown (uint modKeys, TPoint &point);
            void EvRButtonUp (uint modKeys, TPoint &point);
            TRect FillRect (TDC &dc, int line, bool erase) const;
            const char *Info (int line) const;
            void Paint (TDC &dc, bool erase, TRect &);
            void ShowMarker (int line) const;
            void ShowText (int line) const;

         public:
            VerticalBar (TGrid *g);
            void SetUserText (int cwidth);
            bool Create ();
            char * GetClassName ();

            DECLARE_RESPONSE_TABLE (TGrid::VerticalBar);
         };


   public:
      // constructors and destructor -------------------------------------------
      TGrid (TWindow *p, int id, int x, int y, int w, int h, EnterKey ek =NextLine, TabKey =TabNextColumn,
             LineMarker lm =ReverseColor, int max =-1, int attrib =defAttrib, TModule *module = 0);
      TGrid (TWindow *p, int id, EnterKey ek =NextLine, TabKey =TabNextColumn, LineMarker lm =ReverseColor, int max =-1,
             int attrib =defAttrib, TModule *module = 0);
     ~TGrid ();

      // public methods --------------------------------------------------------
      Column *FindColumn (HWND hwnd)   const;
      Column *FindColumn (int id)      const;
      Column *FindColumn (TControl *c) const;
      Column *FindColumn (TPoint &p)   const;

      int        CalcVBarWidth ()     const { return vbar->CalcWidth(); }
      int        CountSelectedLines() const;
      bool       Create ();
      int        GetActiveLine ()     const { return activeLine; }
      int        GetClientHeight ()   const { return clientHeight; }
      int        GetClientWidth ()    const { return clientWidth; }
      int        GetColumnWidth (int id) const;
      int        GetCurrentLine ()    const { return firstLine+activeLine; }
      TColor     GetEmptyGridColor () const { return emptyGrid; }
      EnterKey   GetEnterKey ()       const { return enterKey; }
      int        GetFirstLine ()      const { return firstLine;  }
      TColor     GetHiliteColor ()    const { return hilite; }
      TColor     GetHiliteTextColor ()const { return hilitetext; }
      TColor     GetLineColor ()      const { return lineColor; }
      LineMarker GetLineMarker ()     const { return lineMarker; }
      bool       GetLineSwapping ()   const { return lineSwapping; }
      int        GetMaxLines ()       const { return maxLines; }
      int        GetNLines ()         const { return nLines; }
      TabKey     GetTabKey ()         const { return tabKey; }
      int        GetLogicalWidth ()   const { return logicalWidth; }
      int        GetSelectedLine ()   const;
      int        GetSelectedLines (int *lines) const;
      int        GetSelectedLines (TBitField &bf) const;
      int        GetTitleHeight ()    const { return titleHeight; }
      int        GetTotalWidth  ()    const { return Attr.W; }
      int        GetTotalHeight ()    const { return Attr.H; }
      int        GetVBarWidth ()      const { return vbarWidth; }
      int        GetVisibleLines ()   const { return visibleLines; }
      int        GetX ()              const { return Attr.X; }
      int        GetY ()              const { return Attr.Y; }
      void       HideColumn(int id);           // Charles R. Cordell
      void       UnHideColumn(int id);         // Charles R. Cordell

      bool IsModified         () const { return modified; }
      void SetModify          (bool flag) { modified = flag; }
      bool LineSelected       (int line) const;
      void SetColumnWidth     (int id, int width);
      void SetEmptyGridColor  (TColor c)     { emptyGrid = c; }
      void SetEnterKey        (EnterKey ek)  { enterKey = ek; }
      void SetHiliteColor     (TColor c)     { hilite = c; }
      void SetHiliteTextColor (TColor c)     { hilitetext = c; }
      void SetLineColor       (TColor c)     { lineColor = c; }
      void SetLineMarker      (LineMarker lm);
      void SetLineSwapping    (bool t)       { lineSwapping = t; }
      void SetTabKey          (TabKey tk)    { tabKey = tk; }
      void UnselectAllLines   ();

      // virtual public methods ------------------------------------------------
      virtual void   Add            (TControl *p, const char *title, Align a = Left, int HIDE=false);  // Charles R. Cordell
      virtual bool   CellColor      (int line, int id, bool active, TColor &bg, TColor &fg, HBRUSH &brush) { return false; }
      virtual void   ChangeColWidth (Column *e, int delta, int minw = 8);
      virtual void   ChangeTitle    (int id, const char *newTitle);
      virtual int    GetDataLine    (int y);
      virtual TColor GetTitleColor  (TitleColor part) const;
      virtual const char *GetTitle  (int id) const;
      virtual void   Go             (int pos, bool showControl = true);
      virtual void   GotoFirstColumn ();
      virtual void   GotoLastColumn ();
      virtual bool   GotoNextColumn ();
      virtual bool   GotoPreviousColumn ();
      virtual void   HorzShrink     ();
      virtual void   Move           (int delta, bool showControl = true);
      virtual void   Refresh        ();
      virtual void   RefreshItem    (int line, int id);
      virtual void   RefreshLine    (int line);
      virtual void   ResetPosition  ();
      virtual void   Remove         (int id, bool destroy = true);
      virtual void   Remove         (Column *e, bool destroy = true);
      virtual bool   SelectLine     (int line, uint modKeys = 0);    // changed by Dean Crawford
      virtual void   SetFont        (const char *facename, int height=0, int weight=FW_BOLD, bool italic=false);
      virtual void   SetFont        (int id, const char *facename, int height=0, int weight=FW_BOLD, bool italic=false);
      virtual void   SetLineHeight  (int h);
      virtual void   SetMaxLines    (int m);
      virtual void   SetNLines      (int n);
      virtual void   SetTitle       (int id, const char *text);
      virtual void   SetVisibleCells(Visible v);
      virtual void   SwapLines      (int l1, int l2);
      // The next 6 methods were suggested by Dean Crawford
      virtual void   TitleLButtonDblClk (Column *column) {}
      virtual void   TitleLButtonDown   (Column *column) {}
      virtual void   TitleLButtonUp     (Column *column) {}
      virtual void   TitleRButtonDblClk (Column *column) {}
      virtual void   TitleRButtonDown   (Column *column) {}
      virtual void   TitleRButtonUp     (Column *column) {}

      // virtual public methods which will usually be overloaded -------------
      virtual void DataChanged      (int line, int id, const void *data) {}
      virtual bool DataChanging     (int line, int id, const void *data) { return true; }
      virtual bool DeleteLine       (int line)           { return false; }
      virtual bool InsertLine       (int line)           { return false; }
      virtual bool IsValidLine      (int line)           { return true; }
      virtual void LineSelected     (int line, bool selected) {}
      virtual void NewRowSelected   (int newrow, int oldrow)      {}
      virtual void ScrollData       (int firstline, int lastline) {}

      virtual const void *TransferItem (int line, int id) = 0;
      virtual void TransferItem        (int line, int id, const void *data) = 0;

      DECLARE_RESPONSE_TABLE (TGrid);

   protected:
      friend Client;
      friend Column;
      friend VScroll;
      friend HScroll;
      friend Title;
      friend VerticalBar;
      friend TGCheckBox;
      friend TGComboBox;
      friend TGEdit;
      friend TGStatic;
      friend TGButton;
      Title   *title;
      Column *firstColumn;
      Column *lastColumn;
      Column *current;
      bool calcPos;
      bool lineSwapping;
      bool centerTitle;
      bool disableHScroll;
      bool disableVScroll;
      bool hideHScroll;
      bool hideVScroll;
      bool vscrollHidden;
      bool hscrollHidden;
      bool sizeColumn;
      bool tempNoHMove;
      bool horzShrink;
      bool vertShrink;
      bool staticOnly;
      bool modified;

      int lineHeight;            // lineHeight in pixels                                           /
      int firstLine;             // offset of first visible line                                   /
      int activeLine;            // offset of visible active line                                  /
      int visibleLines;          // number of visible lines                                        /
      int selectRow;             // last current line used by TGrid::Go() (added by Dean Crawford) /
      int maxLines;              // maximum number of lines (-1 = no limite)                       /
      int nLines;                // actual number of lines, if maxLines == -1                      /
      int nControls;             // number of columns with a control                               /
      int hpos;                  // current horizontal offset (0 <= hpos < lastColumn->left)       /
      int selectedLine;          // index of line currently selected (single selection)            /
      int nsel;                  // actual number of lines in selectedLines                        /
      int logicalWidth;          // total width for all columns                                    /
      int clientWidth;           // client width                                                   /
      int clientHeight;          // client height                                                  /
      int titleHeight;           // height of title bar                                            /
      int vbarWidth;             // width of vertical bar                                          /
      int cxvscroll;             // width of vertical scroller                                     /
      int cyhscroll;             // height of horizontal scroller                                  /

      TBitField *selectedLines;  // indexes of lines currently selected        /
      CursorType cursorType;
      VScroll *vscroll;
      HScroll *hscroll;
      VerticalBar *vbar;
      Client *client;
      TColor lineColor;
      TColor emptyGrid;
      TColor hilite;
      TColor hilitetext;
      EnterKey enterKey;
      TabKey   tabKey;
      LineMarker lineMarker;
      Visible visible;
      LineSelect lineSelection;
      TFont *gridFont;
      TFont *titleFont;
      TBitmap *ComboBmp;
      TBitmap *CheckBoxesBmp;
      TBitmap *BwccCheckBmp;
      TBitmap *BwccUncheckBmp;
      TBitmap *Ctl3dCheckBmp;
      LOGFONT *gridLogFont;
      LOGFONT *titleLogFont;
      TBrush *buttonFace;

      // virtual protected methods which are usually not overriden -------------
      virtual void   AppendColumn ();
      virtual bool   CalcPos ();
      virtual bool   CanKillFocus (Column *e);
      virtual void   ChangeWindowPos (int &w,int &h, uint flags);
      virtual void   CheckHScroll ();
      virtual void   ClearModify (TControl *c);
      virtual void   DrawCorner (TDC &dc, TRect &rc);
      virtual void   FindNearestUnlockedCell (Column *e);
      virtual TFont *GetGridFont (int id) const;
      virtual void   Init (int max, EnterKey ek, TabKey tk, LineMarker lm, int attrib);
      virtual bool   IsHScrollbarNeeded ();
      virtual bool   IsVScrollbarNeeded ();
      virtual bool   KeyDown (uint key, int repeat);
      virtual void   KillFocusFromChild (TControl *child, HWND getFocus, bool okToChange);
      virtual void   MoveHPos (int delta);
      virtual void   ProcessMouse (uint modKeys, TPoint &point,uint message, bool sendToControl = true);
      virtual bool   SetCursorShape (TPoint &p); 
      virtual void   SetFocusOnChild (TControl *child, HWND hWndLostFocus);
      virtual void   SetFont ();
      virtual void   SetHPos (int newpos);
      virtual void   SetHScrollRange ();
      virtual void   SetTitleFont ();
      virtual void   SetTitleFont (const char *facename, int height =0, int weight =FW_BOLD, bool italic =false);
      virtual void   SetupWindow ();
      virtual void   SetVBarText (int cwidth = 4);
      virtual void   SetVisibleLines (int nl);
      virtual bool   ValidateLine (int line);

      // message processing and TControl's overloaded methods ------------------
      bool CanClose            ();
      HBRUSH CtlColor          (HDC hDC, HWND hWndChild, uint ctlType);
      bool EvEraseBkgnd        (HDC hwnd);
      void EvKillFocus         (HWND hWndGetFocus);
      void EvLButtonDown       (uint modKeys, TPoint &point);
      void EvLButtonUp         (uint modKeys, TPoint &point);
      void EvLButtonDblClk     (uint modKeys, TPoint &point);
      void EvNCPaint           ();
      void EvPaint             ();
      void EvRButtonDown       (uint modKeys, TPoint &point);
      void EvRButtonUp         (uint modKeys, TPoint &point);
      void EvRButtonDblClk     (uint modKeys, TPoint &point);
      void EvSetFocus          (HWND hWndLooseFocus);
      void EvShowWindow        (bool show, uint status);
      void EvWindowPosChanging (WINDOWPOS far &wpos);
      char *GetClassName       ();
      void GetWindowClass      (WNDCLASS &wc);
      void Paint               (TDC &dc, bool erase, TRect &rc);
      void PaintClient         (TDC &dc, bool erase, TRect &rc);
      void PerformCreate       (int menuOrId);

      // virtual protected methods which may be overriden ----------------------
      virtual bool AppendNewLine  (int &newline) { return true; }
      virtual Visible VisibleCell (int line, int id) { return visible; }

      // keyboard handling -----------------------------------------------------
      virtual bool VkDelete (bool ctrl, bool shift, int repeat);
      virtual bool VkDown   (bool ctrl, bool shift, int repeat);
      virtual bool VkEnd    (bool ctrl, bool shift, int repeat);
      virtual bool VkEscape (bool ctrl, bool shift, int repeat);
      virtual bool VkHome   (bool ctrl, bool shift, int repeat);
      virtual bool VkInsert (bool ctrl, bool shift, int repeat);
      virtual bool VkLeft   (bool ctrl, bool shift, int repeat);
      virtual bool VkNext   (bool ctrl, bool shift, int repeat);
      virtual bool VkPrior  (bool ctrl, bool shift, int repeat);
      virtual bool VkReturn (bool ctrl, bool shift, int repeat);
      virtual bool VkRight  (bool ctrl, bool shift, int repeat);
      virtual bool VkTab    (bool ctrl, bool shift, int repeat);
      virtual bool VkUp     (bool ctrl, bool shift, int repeat);
   };

// COLOR_CODE //////////////////////////////////////////////////////////////////
#define COLOR_CODE(type)                                                   \
HBRUSH type::CtlColors (HDC hDC)                                           \
   {                                                                       \
   TDC dc(hDC);                                                            \
   dc.SetTextColor (text);                                                 \
   dc.SetBkColor (bkg);                                                    \
   return *brush;                                                          \
   }                                                                       \
void type::SetColors (TColor t, TColor b)                                  \
   {                                                                       \
   text = t;                                                               \
   bkg = b;                                                                \
   delete brush;                                                           \
   brush = new TBrush (bkg);                                               \
   }

// COLOR_HEADER ////////////////////////////////////////////////////////////////
#define COLOR_HEADER                                                       \
   protected:                                                              \
      TColor text;                                                         \
      TColor bkg;                                                          \
      TBrush *brush;                                                       \
   public:                                                                 \
      virtual HBRUSH CtlColors (HDC hDC);                                  \
      virtual void   SetColors (TColor text,                               \
                                TColor bkg = GetSysColor(COLOR_WINDOW));   \
      virtual TColor BkgColor ()  { return bkg; }                          \
      virtual TColor TextColor () { return text; }                         \
      virtual HBRUSH BkgBrush  () { return *brush; }

// GRID_EV_GETDLGCODE //////////////////////////////////////////////////////////
#define GRID_EV_GETDLGCODE(obj)                                             \
   uint TG##obj::EvGetDlgCode (MSG far *msg)                                \
      {                                                                     \
      if (grid)                                                             \
         return T##obj::EvGetDlgCode (msg) | DLGC_WANTALLKEYS;              \
      else                                                                  \
         return T##obj::EvGetDlgCode (msg);                                 \
      }

// GRID_EV_KEYUP ///////////////////////////////////////////////////////////////
#define GRID_EV_KEYUP(obj)                                                  \
   void TG##obj::EvKeyUp (uint key, uint repeat, uint flags)                \
      {                                                                     \
      if (grid)                                                             \
         {                                                                  \
         switch (key)                                                       \
            {                                                               \
            case VK_INSERT:                                                 \
            case VK_DELETE:                                                 \
            case VK_HOME:                                                   \
            case VK_END:                                                    \
            case VK_ESCAPE:                                                 \
            case VK_TAB:                                                    \
            case VK_RETURN:                                                 \
            case VK_UP:                                                     \
            case VK_DOWN:                                                   \
            case VK_PRIOR:                                                  \
            case VK_NEXT:                                                   \
            case VK_LEFT:                                                   \
            case VK_RIGHT:                                                  \
               return;                                                      \
            }                                                               \
         }                                                                  \
      T##obj::EvKeyUp (key, repeat, flags);                                 \
      }


// GRID_EV_CHAR ////////////////////////////////////////////////////////////////
#define GRID_EV_CHAR(obj)                                                   \
   void TG##obj::EvChar (uint key, uint repeat, uint flags)                 \
      {                                                                     \
      if (grid)                                                             \
         {                                                                  \
         switch (key)                                                       \
            {                                                               \
            case VK_ESCAPE:                                                 \
            case VK_TAB:                                                    \
            case VK_RETURN:                                                 \
               return;                                                      \
            }                                                               \
         }                                                                  \
      T##obj::EvChar (key, repeat, flags);                                  \
      }


// GRID_EV_KEYDOWN /////////////////////////////////////////////////////////////
#define GRID_EV_KEYDOWN(obj)                                                \
   void TG##obj::EvKeyDown (uint key, uint repeat, uint flags)              \
      {                                                                     \
      if (grid)                                                             \
         {                                                                  \
         switch (key)                                                       \
            {                                                               \
            case VK_HOME:                                                   \
            case VK_END:                                                    \
            case VK_ESCAPE:                                                 \
            case VK_INSERT:                                                 \
            case VK_DELETE:                                                 \
            case VK_TAB:                                                    \
            case VK_RETURN:                                                 \
            case VK_UP:                                                     \
            case VK_DOWN:                                                   \
            case VK_PRIOR:                                                  \
            case VK_NEXT:                                                   \
            case VK_LEFT:                                                   \
            case VK_RIGHT:                                                  \
               if (grid->KeyDown (key, repeat))                             \
                  return;                                                   \
               break;                                                       \
            }                                                               \
         }                                                                  \
      T##obj::EvKeyDown (key, repeat, flags);                               \
      }


// MOUSE_HANDLER_CODE //////////////////////////////////////////////////////////
#define MOUSE_HANDLER_CODE(obj)                                             \
   void TG##obj::EvLButtonDown (uint modKeys, TPoint &point)                \
      {                                                                     \
      if (grid)                                                             \
         {                                                                  \
         MapWindowPoints (*grid->client, &point, 1);                        \
         grid->ProcessMouse (modKeys, point, WM_LBUTTONDOWN, false);        \
         }                                                                  \
      T##obj::EvLButtonDown (modKeys, point);                               \
      }                                                                     \
   void TG##obj::EvLButtonUp (uint modKeys, TPoint &point)                  \
      {                                                                     \
      if (grid)                                                             \
         {                                                                  \
         MapWindowPoints (*grid->client, &point, 1);                        \
         grid->ProcessMouse (modKeys, point, WM_LBUTTONUP, false);          \
         }                                                                  \
      T##obj::EvLButtonUp (modKeys, point);                                 \
      }                                                                     \
   void TG##obj::EvLButtonDblClk (uint modKeys, TPoint &point)              \
      {                                                                     \
      if (grid)                                                             \
         {                                                                  \
         MapWindowPoints (*grid->client, &point, 1);                        \
         grid->ProcessMouse (modKeys, point, WM_LBUTTONDBLCLK, false);      \
         }                                                                  \
      T##obj::EvLButtonDblClk (modKeys, point);                             \
      }                                                                     \
   void TG##obj::EvRButtonDown (uint modKeys, TPoint &point)                \
      {                                                                     \
      if (grid)                                                             \
         {                                                                  \
         MapWindowPoints (*grid->client, &point, 1);                        \
         grid->ProcessMouse (modKeys, point, WM_RBUTTONDOWN, false);        \
         }                                                                  \
      T##obj::EvRButtonDown (modKeys, point);                               \
      }                                                                     \
   void TG##obj::EvRButtonUp (uint modKeys, TPoint &point)                  \
      {                                                                     \
      if (grid)                                                             \
         {                                                                  \
         MapWindowPoints (*grid->client, &point, 1);                        \
         grid->ProcessMouse (modKeys, point, WM_RBUTTONUP, false);          \
         }                                                                  \
      T##obj::EvRButtonUp (modKeys, point);                                 \
      }                                                                     \
   void TG##obj::EvRButtonDblClk (uint modKeys, TPoint &point)              \
      {                                                                     \
      if (grid)                                                             \
         {                                                                  \
         MapWindowPoints (*grid->client, &point, 1);                        \
         grid->ProcessMouse (modKeys, point, WM_RBUTTONDBLCLK, false);      \
         }                                                                  \
      T##obj::EvRButtonDblClk (modKeys, point);                             \
      }


// MOUSE_HANDLER_HEADER ////////////////////////////////////////////////////////
#define MOUSE_HANDLER_HEADER                                                \
   protected:                                                               \
      void EvLButtonDown (uint modKeys, TPoint &point);                     \
      void EvLButtonUp (uint modKeys, TPoint &point);                       \
      void EvLButtonDblClk (uint modKeys, TPoint &point);                   \
      void EvRButtonDown (uint modKeys, TPoint &point);                     \
      void EvRButtonUp (uint modKeys, TPoint &point);                       \
      void EvRButtonDblClk (uint modKeys, TPoint &point);

#endif

// These macros make transfering data from a grid easier ///////////////////////
#define XFR_STRING(x)  static_cast<const char *>(x)
#define XFR_bool(x)   *static_cast<const bool *>(x)
#define XFR_DATE(x)   *static_cast<const IntlDate *>(x)
#define XFR_TIME(x)   *static_cast<const TGTime *>(x)
#define XFR_DOUBLE(x) *static_cast<const double *>(x)
#define XFR_LONG(x)   *static_cast<const long *>(x)
#define XFR_SHORT(x)  *static_cast<const short *>(x)


// EOF
