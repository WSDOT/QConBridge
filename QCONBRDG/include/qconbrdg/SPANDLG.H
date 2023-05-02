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

#if !defined(__spandlg_h)              // Sentry, use file only if it's not already included.
#define __spandlg_h


#include <owl\listbox.h>
#include <owltools\prpsheet.h>
#include <owltools\radiogrp.h>
#include <owltools\nbredit.h>
#include <owltools\unitedit.h>
#include <owltools\matedit.h>
#include <bridge\bridge.h>
#include <bridge\lrfdspan.h>
#include <qconbrdg\spantbl.h>
#include <qconbrdg\mytypes.h>
#include <qconbrdg\editview.h>
#include <qconbrdg\qcbapp.rh>            // Definition of all resources.

enum AddSpanLocation {aslAtStart,aslAtEnd,aslAtCurrent};
typedef AddSpanLocation AddSpanLocation;

struct TEditSpanDlgXferBuffer
{
	TEditSpanDlgXferBuffer(SystemOfUnits units);
	TEditSpanDlgXferBuffer(const LRFDSpan *span);
	LRFDSpan *GetSpan();
	int NumHinges() const;

	struct Properties
		{
		double length;
		SpanTableData table;
		} properties;

	struct BoundryConditions
		{
		int leftBC;
		int rightBC;
		} bc;

	struct Hinges
		{
		int numHinges;
		double hinge1Loc;
		double hinge2Loc;
		} hinges;

	struct LimStateFactors
		{
		double nd_strength;
		double nr_strength;
		double ni_strength;
		double nd_service;
		double nr_service;
		double ni_service;
		} limitStateFactors;

	bool IsLeftEndContinuous() const
		{ return (bc.leftBC == BF_CHECKED); }
	bool IsRightEndContinuous() const
		{ return (bc.rightBC == BF_CHECKED); }
};

struct TAddSpanDlgXferBuffer : public TEditSpanDlgXferBuffer
{
	TAddSpanDlgXferBuffer(bool sel,SystemOfUnits units);
	TAddSpanDlgXferBuffer(const LRFDSpan*span);

	struct SupportData
		{
		int    simple;
		double height;
		double offset;
		double ax;
		double iz;
		double modE;
		double unitWeight;
		int    fixity;
		int    hinge;
		} supportData;

	struct InsertionPoint
		{
		int point;
		} insertionPoint;

	AddSpanLocation GetLocation() const;
	Support *GetSupport();
};

class TEditSpanDlg : public TMyPropertySheet
{
public:
   TEditSpanDlg(TWindow* parent,SystemOfUnits units,TModule* module = 0);
	 virtual ~TEditSpanDlg ();
    virtual void SetupTransferBuffer(TEditSpanDlgXferBuffer *buffer);

protected:
   TPropertyPage* page1;
   TPropertyPage* page2;
   TPropertyPage* page3;
   TPropertyPage* page4;
};

class TAddSpanDlg : public TEditSpanDlg
{
public:
   TAddSpanDlg(TBridgeEditorView* parent,SystemOfUnits units,TModule* module = 0);
    virtual ~TAddSpanDlg ();
    void SetupTransferBuffer(TEditSpanDlgXferBuffer *buffer);

    TBridgeEditorView *editorView;

protected:
    TPropertyPage* page5;
    TPropertyPage* page6;
};

#endif                                      // __spandlg_h sentry.

