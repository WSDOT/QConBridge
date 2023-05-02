///////////////////////////////////////////////////////////////////////
// QConBridge - HL93 Live Load Analysis
// Copyright (C) 1999  Washington State Department of Transportation
//                     Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation, 
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but 
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source 
// License along with this program; if not, write to the Washington 
// State Department of Transportation, Bridge and Structures Office, 
// 4500 3rd AVE SE - P.O. Box  47340, Olympia, WA 98503, USA or e-mail 
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Revision Log
// ============
// May 1997 - Created, Richard Brice, PE
// Oct 1999 - Released as Open Source

// File:         BigeGbl.h
//
// Created:      Tue Apr 05 09:26:29 1994
//
// Description:  Class declarations
//
//               Global variable declarations & definitions
//
// Regenerated:  Tue Apr 05 09:26:29 1994
// Skeleton by:  Winpro/3, a product from Xian Corporation
//               bc4owl2.skl version 1.00 developed by:
//                 Terry Richards
//                 Terry Richards Software
//                 (908) 545-6533
//

#ifndef _BEDITCLI_H
#define _BEDITCLI_H

//32 bit apps cannot be built with vbx controls
//The next line prevents the VBX header being processed.
#define __OWL_VBXCTL_H

#include <dir.h>
#include <values.h>
//#include <owl\owlall.h>
#include <owl\window.h>
#include <owl\gdiobjec.h>
#include <owl\menu.h>
#include <owl\findrepl.h>
#include <classlib\stacks.h>
#include <classlib\arrays.h>

typedef TIArrayAsVector<string> StringArray;

class TBufferPos{

protected:

	int 			Line;
	int 			Col;

	StringArray* 	Buffer;
	BOOL         	LastUpDown;

	void         	Normalize();
	BOOL         	IsWholeWord(int Line,int Col,int Length,string BreakString);

public:

	TBufferPos(StringArray* ABuffer);

	int  		 	GetCol();
	int  		 	GetLine();
	void 		 	SetCol(int Val){Col = Val;};
	void         	SetLine(int Val){Line = Val;};

	void         	LineChange(int Delta);
	void         	EndUpDown();

	void         	Home(BOOL Ctrl);
	void 		 	End(BOOL Ctrl);
	BOOL 		 	AtEnd();

	void 		 	NextBreak(string BreakString);
	void 		 	PrevBreak(string BreakString);
	BOOL 		 	NextWord(string BreakString);
	BOOL 		 	PrevWord(string BreakString);
	BOOL 		 	FindNext(BOOL MatchCase,BOOL WholeWord,string BreakString,char *FindWhat);
	BOOL 		 	FindPrev(BOOL MatchCase,BOOL WholeWord,string BreakString,char *FindWhat);

	BOOL 		 	operator == (TBufferPos ABuf);
	BOOL 		 	operator != (TBufferPos ABuf);
	BOOL 		 	operator >  (TBufferPos ABuf);
	void 		 	operator =  (TBufferPos ABuf);


	void 		 	operator += (int Delta);
	void 		 	operator -= (int Delta);
	void 		 	operator ++ ();
	void 		 	operator -- ();
    BOOL			IncAndTest(int Delta);

};

class TEditClient; //Forward declaration

class JournalEntry{

protected:

	TBufferPos      Where;
	JournalEntry*   Addr;
	int             NumChars;

public:

	BOOL            IsLinked;

	JournalEntry(StringArray* pBuffer,TBufferPos Location,BOOL Linked);
	virtual ~JournalEntry(){};

	BOOL operator == (JournalEntry Other); //Required to be in a list

	virtual void Undo(TEditClient* Parent);

};

class JEInsert : public JournalEntry{

public:

	JEInsert(int HowMany,StringArray* pBuffer,TBufferPos Location,BOOL Linked);

	virtual void Undo(TEditClient* Parent);

};

class JEDelete : public JournalEntry{

protected:

	char * Deleted;

public:

	JEDelete(char * What,StringArray* pBuffer,TBufferPos Location,BOOL Linked);
	virtual ~JEDelete();

	virtual void Undo(TEditClient* Parent);

};

typedef TIStackAsList<JournalEntry> JournalStack;

class TEditClient : public TWindow{

friend JEInsert;
friend JEDelete;

protected:

	TMenuDescr         MenuDescr;
	char               File[MAXPATH];
	StringArray        pLines;
	JournalStack       UndoStack;
	int                LineHeight,CharAveWidth,CaretWidth;
	int                TabArray[50],NumTabs;
	TBufferPos         CaretPos,HiStart,HiEnd;
	BOOL               IsDirty;
	BOOL               MouseDown;
	BOOL               ReplaceDialog;
	BOOL               OverType;
	BOOL               PartFile;

	// Structures used by Find and Replace dialogs to pass information
	TFindReplaceDialog *        SearchDialog;
	TFindReplaceDialog::TData   FindData;

	virtual TPoint     BufferToClient(TBufferPos pt);
	virtual TBufferPos ClientToBuffer(TPoint pt);
	virtual void       LoadFile();
	virtual void       ClearUndoStack();
	virtual void       ScrollToCaret();
	virtual void       DoDelete(BOOL Log=TRUE);
	virtual void       DeleteOneChar();
	virtual void       DoInsert(char Key,BOOL Log=TRUE);
	virtual void       DoInsert(char *KeyString,BOOL Log=TRUE);
	virtual BOOL       DoCopy();
	virtual void       InvalidateToEnd(TBufferPos& Where);
	virtual void       ScrollerSetRange();
	virtual void       DoSearch();
	virtual BOOL       DoFind(BOOL Down,BOOL MatchCase,BOOL WholeWord,char * FindWhat);
	virtual void       DoReplace(char * ReplaceWith);
	virtual void       SetupWindow();

	//These are functions you might want to overload

	virtual void       SetFont(TDC& DC);
	virtual void       SetColor(TDC& DC,int Mode);
	virtual void       SetTabs();
	virtual void       SetBreakChars(string& BreakString);
	virtual void       SizeChanged(BOOL JustFocus=FALSE);
	virtual void       PosChanged(BOOL JustFocus=FALSE);

public:

//	TEditClient(TMDIChild* pParent,const char * FileName);
	TEditClient(TWindow* pParent,const char * FileName);
	virtual ~TEditClient();

	 virtual char far * GetClassName();

	// We need to override Paint so we can draw & write in the window
	virtual void Paint(TDC& HDC,BOOL erase,TRect& rc);

	// This is a good place to see if the file needs saving...
//	virtual BOOL CanClose();

	// Handle messages from the Find / Replace common dialog.
   /*
	LRESULT WMFindMsg(WPARAM wp,LPARAM lp);

	void CM_FILESAVEMethod();
	void CM_FILESAVEASMethod();
	void CM_FILECLOSEMethod();
	void CM_EDITUNDOMethod();
	void CM_EDITCUTMethod();
	void CM_EDITCOPYMethod();
	void CM_EDITPASTEMethod();
	void CM_EDITDELETEMethod();
	void CM_EDITCLEARMethod();
	void CM_EDITFINDMethod();
	void CM_EDITFINDNEXTMethod();
	void CM_EDITREPLACEMethod();

	void CM_FILESAVEEnable(TCommandEnabler& commandHandler);
	void CM_EDITUNDOEnable(TCommandEnabler& commandHandler);
	void CM_EDITCUTCOPYEnable(TCommandEnabler& commandHandler);
	void CM_EDITPASTEEnable(TCommandEnabler& commandHandler);
	void CM_EDITFINDEnable(TCommandEnabler& commandHandler);
	void CM_EDITFINDNEXTEnable(TCommandEnabler& commandHandler);
	void CM_EDITREPLACEEnable(TCommandEnabler& commandHandler);
   */

	virtual void Open(char * FileName=NULL);

	void EvSetFocus(HWND);
	void EvKillFocus(HWND);
	void EvLButtonDown(UINT modKeys, TPoint& point);
	void EvMouseMove(UINT modKeys, TPoint& point);
	void EvLButtonUp(UINT modKeys, TPoint& point);
	void EvLButtonDblClk(UINT modKeys, TPoint& point);
	void EvHScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl);
	void EvVScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl);
	void EvKeyDown(UINT key, UINT repeatCount, UINT flags);
	void EvChar(UINT key, UINT repeatCount, UINT flags);
	void EvSetFont(HFONT font, bool redraw);

	DECLARE_RESPONSE_TABLE(TEditClient);

};

#endif

//   E N D   O F   F I L E   BigeGbl.h


