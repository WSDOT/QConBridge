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

#if !defined(__bdoc_h)              // Sentry, use file only if it's not already included.
#define __bdoc_h


#include <owl\docview.h>
#include <bridge\lrfdbrdg.h>
#include <qconbrdg\prototyp.h>
#include <qconbrdg\mytypes.h>
#include <owltools\progdlg.h>

#include <owltools\symbol.h>   // for TSymbolType
#include <qconbrdg\concload.h> // for ConcLoadXferBuffer
#include <qconbrdg\unifload.h> // for UnifLoadXferBuffer
#include <qconbrdg\llgendlg.h> // for LLGenXferBuffer
#include <qconbrdg\loadfact.h> // for LoadFactorXferBuffer
#include <qconbrdg\stddload.h> // for StdDeadLoadXferBuffer

#include <dir.h> // for MAXPATH

enum TPlotLocation {plScreen,plPrinter};

class TTable;
class TLoadTable;
class TEnvelopeTable;
class TReactionTable;
class TReactionEnvelopeTable;

//{{TDocument = TBridgeDocument}}
class TBridgeDocument : public TDocument {
public:
    TBridgeDocument (TDocument* parent = 0);
    virtual ~TBridgeDocument ();

//{{TBridgeDocumentVIRTUAL_BEGIN}}
public:
    virtual bool Open (int mode, const char far* path=0);
    virtual bool Commit (bool force=false);
    virtual bool Revert (bool clear=false);
    virtual bool IsOpen ();
    virtual bool CanClose ();
    virtual bool Close ();
//{{TBridgeDocumentVIRTUAL_END}}

public:
   void CreateFromPrototype(BridgePrototype* prototype);
   void Clear();
   LRFDBridge &GetBridge();
   void SetAnalyzed(bool analyzed);
   bool IsAnalyzed() {return isAnalyzed;}
   void ResetToDefaults();
   void DisplayDimensions(bool display);
   void DisplayLabels(bool display);
   void DisplaySegmentTicks(bool display);
   void DisplayGrid(bool display);
   void InvertMomentDiagrams(bool invert);
   bool DisplayDimensions() const;
   bool DisplayLabels() const;
   bool DisplaySegmentTicks() const;
   bool DisplayGrid() const;
   bool InvertMomentDiagrams() const;
   void SetUnits(SystemOfUnits u);
   SystemOfUnits GetUnits() const;

   // Bridge Interface
   int  GetNumSpans() {return bridge->NumSpans();}
   int  GetNumSupports() {return bridge->NumSupports();}
   void AddSpanSupport(Span *newSpan,Support *newSupport,bool atEnd);
   void InsertSpanSupport(Span *refSpan,bool after,Span *newSpan,Support *newSupport,bool isRightSupport);
   void RemoveSpan(Span *oldSpan,bool removeRightSupport);
   void ReplaceSpan(Span *oldSpan,Span *newSpan);
   void ReplaceSupport(Support *oldSupport,Support *newSupport);
   void RemoveLoad(BridgeLoad *load);
   void ReplaceLoad(BridgeLoad *oldLoad,BridgeLoad *newLoad,int lcase);
   void ReplaceLoad(ConcBridgeLoad *oldLoad,ConcLoadXferBuffer *xfer);
   void ReplaceLoad(UnifBridgeLoad *oldLoad,UnifLoadXferBuffer *xfer);
   void InsertLoad(BridgeLoad *newLoad,int lcase);
   void AddLoad(Span* selSpan,ConcLoadXferBuffer *xfer);
   void AddLoad(Span* selSpan,UnifLoadXferBuffer *xfer);
   void SetLiveLoadGenParameters(LLGenXferBuffer &xfer);
   void GetLiveLoadGenParameters(LLGenXferBuffer &xfer);
   void SetLoadFactors(LoadFactorXferBuffer &xfer);
   void GetLoadFactors(LoadFactorXferBuffer &xfer);
   void Cantilever(bool removeSupport,bool isFirstSpan);
   void SetStdDeadLoads(StdDeadLoadXferBuffer &xfer);
   void GetStdDeadLoads(StdDeadLoadXferBuffer &xfer);
   void SetMinAnalysisPoints(int minAnalysisPoints);
   int  GetMinAnalysisPoints();
   void SetPedestrianLoad(double pLoad);
   double GetPedestrianLoad();
   void GetDynamicLoadAllowance(double &IMtruck,double &IMlane,double &IMfatigue);
   void SetDynamicLoadAllowance(double IMtruck,double IMlane,double IMfatigue);
   double GetLength();
   bool Analyze(TProgressDlg *dlg = NULL);
   double GetResult(int spanId,int poiOffset,MbrFace face,DiagramCatagory c,DiagramType t);
   double GetReaction(int pierId,DiagramCatagory c,DiagramType t);

   bool GenerateReport();
   const char *GetReportName() {return tmpFile;}

   void SetPlotColor(TPlotLocation loc,DiagramCatagory dc,TColor color);
   TColor GetPlotColor(DiagramCatagory dc);
   TColor GetPlotColor(TPlotLocation loc,DiagramCatagory dc);

   void SetPenStyle(TPlotLocation loc,DiagramCatagory dc,int ps);
   int GetPenStyle(DiagramCatagory dc);
   int GetPenStyle(TPlotLocation loc,DiagramCatagory dc);

   void SetPlotSymbolType(TPlotLocation loc,DiagramCatagory dc,TSymbolType symbol);
   TSymbolType GetPlotSymbolType(DiagramCatagory dc);
   TSymbolType GetPlotSymbolType(TPlotLocation loc,DiagramCatagory dc);
   TSymbol *GetPlotSymbol(DiagramCatagory dc);

   TTable *CreateTable(DiagramCatagory dc,bool bReactions=false);
   TLoadTable *CreateLoadTable(DiagramCatagory dc);
   TEnvelopeTable *CreateEnvelopeTable(DiagramCatagory dc);
   TReactionTable* CreateReactionTable(DiagramCatagory dc);
   TReactionEnvelopeTable* CreateReactionEnvelopeTable(DiagramCatagory dc);

protected:
   LRFDBridge *bridge;
   bool drawDimensions;
   bool drawLabels;
   bool drawSegmentTicks;
   bool drawGrid;
   bool invertMomentDiagrams;
   SystemOfUnits units;
   bool isAnalyzed;
   char tmpFile[MAXPATH];

   void SetupBridge();

   struct PlotStyleData
      {
      TColor color;
      int penStyle;
      TSymbolType symbol;
      } plotStyleData[2][NUM_LOADINGS];

   TSymbol *symbol[12];

private:
   void ReadINI();
   void WriteINI();
   void DeleteTempReportFile();
};    //{{TBridgeDocument}}


#endif                                      // __bdoc_h sentry.

