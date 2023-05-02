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

#if !defined RESDOMGR_H
#define RESDOMGR_H

#include <qconbrdg\bdomgr.h>
#include <qconbrdg\diagdobj.h>
#include <bridge\lrfdbrdg.h> // for DiagramCatagory

class TBridgeResultsView;

class TResultsDisplayObjectManager : public TBridgeDisplayObjectManager
{
public:
   TResultsDisplayObjectManager(TBridgeResultsView *view);
   ~TResultsDisplayObjectManager();

   void AddDiagramDisplayObject(TDiagramDisplayObject *ddo);
   void InvertDiagrams(bool invert);
   void Draw(TDC &dc,TRect &rect,bool bestFit);
   void Analyzed(bool analyzed);

   void DisplayGrid(bool display);
   bool DisplayGrid() const;

   bool DrawDiagram(DiagramCatagory dc);
   void DrawDiagram(DiagramCatagory dc,bool draw);

   void Clear();

protected:
   TIDiagramDisplayObjectContainer *diagramDisplayObjects;
   TIDiagramDisplayObjectContainer *activeDisplayObjects;
   bool activeDiagrams[NUM_LOADINGS];

   bool invertDiagrams;
   bool drawGrid;
   bool isAnalyzed;

   void SetupDisplayObjects();
   void SetupDisplay(TDC &dc,TRect &rect,bool bestFit);
   void SetupMapper(TDC &dc,bool bestFit);
   void SetupScales(TMapper &mapr);
   void SetupAxisRegion(TDC &dc,TRect &rect);
   void SetupLegend();
   void GetScaleString(string &scale);
   void DrawAxis(TDC &dc);
   void DrawLegend(TDC &dc);

   string axisFontTypeFace;
   int axisFontPointSize;

   TMapper diagramMapper;
   TBridgeResultsView *rview;
};

#endif