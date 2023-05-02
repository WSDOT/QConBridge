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

#include <qconbrdg\resdomgr.h>
#include <qconbrdg\qcbapp.rh>
#include <qconbrdg\rsltview.h>
#include <owltools\fonthelp.h>
#include <esl\engnotat.h>
#include <owl\docmanag.h>

TResultsDisplayObjectManager::TResultsDisplayObjectManager(TBridgeResultsView *view) :
TBridgeDisplayObjectManager(view)
{
   diagramDisplayObjects = new TIDiagramDisplayObjectContainer(10,0,10);
   activeDisplayObjects = new TIDiagramDisplayObjectContainer(10,0,10);
   activeDisplayObjects->OwnsElements(false);

   invertDiagrams = bview->bridgeDoc.InvertMomentDiagrams();
   DisplayGrid(bview->bridgeDoc.DisplayGrid());
   isAnalyzed = false;

   rview = view;

   axisFontTypeFace = "Arial";
   axisFontPointSize = -10;

   for (DiagramCatagory dc = dcDC; dc <= dcFatigue; dc++)
      {
      activeDiagrams[dc] = false;
      }
}

TResultsDisplayObjectManager::~TResultsDisplayObjectManager()
{
   delete diagramDisplayObjects;
   delete activeDisplayObjects;
}

void TResultsDisplayObjectManager::Clear()
{
   TBridgeDisplayObjectManager::Clear();
   diagramDisplayObjects->Flush();
   activeDisplayObjects->Flush();
}

bool TResultsDisplayObjectManager::DrawDiagram(DiagramCatagory dc)
{
   return activeDiagrams[dc];
}

void TResultsDisplayObjectManager::DrawDiagram(DiagramCatagory dc,bool draw)
{
   activeDiagrams[dc] = draw;
}

void TResultsDisplayObjectManager::AddDiagramDisplayObject(TDiagramDisplayObject *ddo)
{
   diagramDisplayObjects->Add(ddo);
}

void TResultsDisplayObjectManager::InvertDiagrams(bool invert)
{
   invertDiagrams = invert;
}

void TResultsDisplayObjectManager::Analyzed(bool analyzed)
{
   isAnalyzed = analyzed;
}

void TResultsDisplayObjectManager::DisplayGrid(bool display)
{
   drawGrid = display;
}

bool TResultsDisplayObjectManager::DisplayGrid() const
{
   return drawGrid;
}

void TResultsDisplayObjectManager::SetupDisplay(TDC &dc,TRect &rect,bool bestFit)
{
   TBridgeDisplayObjectManager::SetupDisplay(dc,rect,bestFit);
   SetupMapper(dc,bestFit);
   SetupDisplayObjects();
}

void TResultsDisplayObjectManager::SetupAxisRegion(TDC &dc,TRect &rect)
{
   TFont *axisFont;
   string testString("-999.999E+99");
   TSize size;

   axisFont = CreateSimpleFont(dc,axisFontTypeFace.c_str(),axisFontPointSize);
   dc.SelectObject(*axisFont);

   size = dc.GetTextExtent(testString.c_str(),testString.length());

   if (!isAnalyzed)
      axisRect = TRect(rect.TopLeft(),rect.BottomLeft());
   else
      axisRect = TRect(rect.TopLeft(),TPoint(rect.left+size.cx,rect.bottom));

   dc.RestoreFont();

   delete axisFont;
}

void TResultsDisplayObjectManager::DrawAxis(TDC &dc)
{
   TEXTMETRIC tm;
   int cyChar;
   int np;
   int i;
   int numPixels;
   int textAlign;
   int bkMode;
   TSize deviceExt;
   TFSize worldExt;
   double yRange;
   int exp;
   TFont *axisFont;
#if defined (__FLAT__)
   long x,y;
#else
   int x,y;
#endif

   // If the bridge is not analyzed or there are not current diagrams,
   // then there is nothing to display
   if (!isAnalyzed || activeDisplayObjects->GetItemsInContainer() == 0)
      return;

   //
   // Setup the font
   //
   axisFont = CreateSimpleFont(dc,axisFontTypeFace.c_str(),axisFontPointSize);
   dc.SelectObject(*axisFont);
   textAlign = dc.SetTextAlign(TA_TOP | TA_RIGHT);
   bkMode = dc.SetBkMode(TRANSPARENT);

   //
   // Get some information about the text we will be using
   //
   dc.GetTextMetrics(tm);
   cyChar = tm.tmHeight + tm.tmExternalLeading;

   //
   // Get some information about the world and the device
   //
   deviceExt = diagramMapper.GetDeviceExt();
   worldExt = diagramMapper.GetWorldExt();

   //
   // Compute yRange for the axis
   //
   yRange = worldExt.dy;

   if (yRange == 0)
      {
      // if range is zero, put 0.000e+00 at the x-axis, and get the
      // heck outta here
      diagramMapper.WPtoDP(0,0,x,y);

      ostrstream os;
      os << EngNotation(0.00,3) << ends;
      dc.TextOut(axisRect.right,y-cyChar/2,os.str());
      delete[] os.str();

      // this is bad code... this is repeated below... clean it up!!!
      dc.RestorePen();
      dc.RestoreFont();
      dc.SetTextAlign(textAlign);
      dc.SetBkMode(bkMode);
      delete axisFont;
      return;
      }

   rview->ConvertToDisplayUnits(yRange);
   exp = int(log10(fabs(yRange)));
   yRange *= pow(10,-exp);
   yRange = ceil(yRange);
   yRange *= pow(10,exp);

   // Get number of pixels needed to contain yRange
   // (this number of pixels might not be available, but we will make
   // provisions for this later)
   double temp =  1./yRange; // using inverse is same as converting back to internal units
   rview->ConvertToDisplayUnits(temp);
   temp = 1./temp; // invert it back
   diagramMapper.WPtoDP(0,temp,x,y);
   numPixels = diagramMapper.GetDeviceOrg().y - y;

   // Determine how many tags can fit in desired range
   if (21*cyChar < numPixels)
      np = 21;
   else if (9*cyChar < numPixels)
      np = 9;
   else if (5*cyChar < numPixels)
      np = 5;
   else
      np = 3;

   // Setup a non-dimensionalized mapper to help place the labels
   TMapper ndcMapper;
   ndcMapper.SetWorldExt(2,2);
   ndcMapper.SetWorldOrg(0,0);
   ndcMapper.SetDeviceExt(deviceExt.cx,numPixels);
   ndcMapper.SetDeviceOrg(diagramMapper.GetDeviceOrg());

   // Draw the labels
   for (i = 0; i < np; i++)
      {
      double f = ((double)(2*i)/(double)(np-1) - 1.);
      double tagValue = f*yRange/2;

      ndcMapper.WPtoDP(0,f,x,y);

      // Don't draw the label if it doesn't fit in the available pixel range
      if (y < bridgeRect.top || bridgeRect.bottom < y)
         continue;

      ostrstream os;
      os << EngNotation(tagValue,3) << ends;
      dc.TextOut(axisRect.right,y-cyChar/2,os.str());
      delete[] os.str();
      }

   // Draw grid/ticks at tenth points
   dc.MoveTo(axisRect.right,bridgeRect.top);
   dc.LineTo(axisRect.right,bridgeRect.bottom);
   TPen pen((drawGrid) ? TColor::Gray : TColor::Black,
             1,
            (drawGrid) ? PS_DOT : PS_SOLID);
   dc.SelectObject(pen);
   for (i = 0; i < 21; i++)
      {
      double f = ((2*i)/20. - 1.);
      ndcMapper.WPtoDP(0,f,x,y);
      dc.MoveTo(axisRect.right,y);

      // Don't draw the grid line/tick if it doesn't fit
      // in the available pixel range
      if (y < bridgeRect.top || bridgeRect.bottom < y)
         continue;

      if (drawGrid)
         dc.LineTo(bridgeRect.right,y);
      else
         dc.LineTo(axisRect.right+10,y); // draw a tick
      }

   dc.RestorePen();
   dc.RestoreFont();
   dc.SetTextAlign(textAlign);
   dc.SetBkMode(bkMode);

   delete axisFont;
}

void TResultsDisplayObjectManager::SetupMapper(TDC &dc,bool bestFit)
{
   double minx,miny,maxx,maxy;
   double MinX,MaxX,MinY,MaxY;
   double sign;

   // If analysis is not complete, then there are no diagrams to setup
   if (!isAnalyzed)
      return;

   SetupScales(diagramMapper);
   diagramMapper.SetLogPixelsX(dc.GetDeviceCaps(LOGPIXELSX));
   diagramMapper.SetLogPixelsY(dc.GetDeviceCaps(LOGPIXELSY));

   //
   // Get diagrams to be displayed
   //
   activeDisplayObjects->Flush();  // flush old ones
   for (DiagramCatagory dcat = dcDC; dcat <= dcFatigue; dcat++)
      {
      if (DrawDiagram(dcat))
         activeDisplayObjects->Add((*diagramDisplayObjects)[dcat]);
      }

   if (activeDisplayObjects->GetItemsInContainer() == 0)
      return; // nothing to set up...

   //
   // Find extents of diagrams to be displayed
   //
   MinX = 0;
   MaxX = 0;
   MinY = 0;
   MaxY = 0;

   TIDiagramDisplayObjectIterator i(*activeDisplayObjects);
   while(i)
      {
      TDiagramDisplayObject *dispObj = i++;
      dispObj->GetRange(minx,maxx,miny,maxy);
      MinX = min(minx,MinX);
      MaxX = max(maxx,MaxX);
      MinY = min(miny,MinY);
      MaxY = max(maxy,MaxY);
      }

   //
   // Setup Device
   //

   // use same origin and width as bridge and height of bridgeRect
   diagramMapper.SetDeviceOrg(bridgeMapper.GetDeviceOrg());
   diagramMapper.SetDeviceExt(bridgeRect.Size());

   //
   // Setup World
   //
   diagramMapper.SetWorldOrg((MaxX+MinX)/2,0);
   sign = (invertDiagrams) ? -1 : 1;
   diagramMapper.SetWorldExt(MaxX-MinX,sign*2*max(fabs(MinY),fabs(MaxY)));
   if (bestFit)
      diagramMapper.SetMapMode(mmAnisotropic | mmBestFitX | mmBestFitY);
   else
      diagramMapper.SetMapMode(mmAnisotropic);
}

void TResultsDisplayObjectManager::SetupScales(TMapper &mapr)
{
   mapr.ClearScaleX();
   mapr.ClearScaleY();
   //
   // Setup Best fit scales for bridge geometry
   //
   if (rview->bridgeDoc.GetUnits() == unitsSI)
      {
      mapr.AddScaleX(0.100);
      mapr.AddScaleX(0.125);
      mapr.AddScaleX(0.150);
      mapr.AddScaleX(0.200);
      mapr.AddScaleX(0.250);
      mapr.AddScaleX(0.500);

      mapr.AddScaleY(0.100);
      mapr.AddScaleY(0.125);
      mapr.AddScaleY(0.150);
      mapr.AddScaleY(0.200);
      mapr.AddScaleY(0.250);
      mapr.AddScaleY(0.500);
      }
   else
      {
      double f;
      if (strcmp(bview->GetViewName(),"Moment Diagram") == 0)
         f = 0.0533786593831; // lbs-ft/inch -> N-m/mm
      else
         f = 0.175126835246; // lbs/inch -> N/mm

      mapr.AddScaleX(0.1*0.012);
      mapr.AddScaleX(0.2*0.012);
      mapr.AddScaleX(0.3*0.012);
      mapr.AddScaleX(0.4*0.012);
      mapr.AddScaleX(0.5*0.012);
      mapr.AddScaleX(0.6*0.012);

      mapr.AddScaleY(0.1*f);
      mapr.AddScaleY(0.2*f);
      mapr.AddScaleY(0.3*f);
      mapr.AddScaleY(0.4*f);
      mapr.AddScaleY(0.5*f);
      mapr.AddScaleY(0.6*f);
      }
}

void TResultsDisplayObjectManager::GetScaleString(string &scale)
{
   if (!isAnalyzed || activeDisplayObjects->GetItemsInContainer() == 0)
      {
      TBridgeDisplayObjectManager::GetScaleString(scale);
      return;
      }

   char tag[10];
   double scaleX;
   double scaleY;

   scaleX = diagramMapper.GetScaleX();
   scaleY = diagramMapper.GetScaleY();

   if (strcmp(bview->GetViewName(),"Moment Diagram") == 0)
      strcpy(tag,(bview->bridgeDoc.GetUnits() == unitsSI ? "N-m/mm" : "ft-lbs/inch"));
   else
      strcpy(tag,(bview->bridgeDoc.GetUnits() == unitsSI ? "N/mm" : "lbs/inch"));

   ostrstream os;

   if (bview->bridgeDoc.GetUnits() == unitsSI)
      {
      os << setiosflags(ios::showpoint) << setprecision(2)
         << "HScale: " << scaleX << " m/mm - "
         << "VScale: " << EngNotation(scaleY,3) << " " << tag << ends;
      }
   else
      {
      // Hscale in m/mm. Convert to feet/inch
      scaleX *= 1000./12.;

      // Vscale in N-m/mm. Convert to k-ft/inch
   if (strcmp(bview->GetViewName(),"Moment Diagram") == 0)
      scaleY /= 0.0533786593831; // N-m/mm -> lbs-ft/inch
   else
      scaleY /= 0.175126835246; // N/mm -> lbs/inch

      os << setiosflags(ios::showpoint) << setprecision(2)
         << "HScale: " << scaleX << " ft/inch - "
         << "VScale: " << EngNotation(scaleY,3) << " " << tag << ends;
      }

   scale = os.str();
   delete[] os.str();
}

void TResultsDisplayObjectManager::Draw(TDC &dc,TRect &rect,bool bestFit)
{
   TBridgeDisplayObjectManager::Draw(dc,rect,bestFit);

   if (!isAnalyzed || activeDisplayObjects->GetItemsInContainer() == 0)
      {
      TColor oldColor;
      int oldMode;
      oldColor = dc.SetTextColor(TColor::LtRed);
      oldMode = dc.SetBkMode(TRANSPARENT);
      dc.SetTextAlign(TA_CENTER);
      dc.SelectObject(TFont("Arial",-18));
      dc.TextOut(bview->GetClientRect().Width()/2,
                 bview->GetClientRect().Height()/2,
                 (!isAnalyzed) ? "Results Not Available - Use Bridge | Analyze" : "Use View menu to select loadings");
      dc.SetTextColor(oldColor);
      dc.SetBkMode(oldMode);
      dc.RestoreFont();
      }
   else
      {
      TIDiagramDisplayObjectIterator i(*activeDisplayObjects);

      while(i)
         {
         TDiagramDisplayObject *dispObj = i++;
         dispObj->SetColor(bview->bridgeDoc.GetPlotColor(dispObj->GetDiagramCatagory()));
         dispObj->SetPenStyle(bview->bridgeDoc.GetPenStyle(dispObj->GetDiagramCatagory()));
         dispObj->SetSymbol(bview->bridgeDoc.GetPlotSymbol(dispObj->GetDiagramCatagory()));
         dispObj->Draw(dc);
         }
      }
}

void TResultsDisplayObjectManager::SetupDisplayObjects()
{
   TIDiagramDisplayObjectIterator i(*activeDisplayObjects);
   while(i)
      {
      TDiagramDisplayObject *dispObj = i++;
      dispObj->Setup(diagramMapper);
      }
}

void TResultsDisplayObjectManager::DrawLegend(TDC &dc)
{
   if (isAnalyzed && activeDisplayObjects->GetItemsInContainer() != 0)
      TBridgeDisplayObjectManager::DrawLegend(dc);
}

void TResultsDisplayObjectManager::SetupLegend()
{
   int key[NUM_LOADINGS] =
                 {IDS_DC,
                  IDS_DW,
                  IDS_LL_ENV,
                  IDS_TANDEM_ENV,
                  IDS_TRUCK_ENV,
                  IDS_TRKTRAIN_ENV,
                  IDS_TANDEMTRAIN_ENV,
                  IDS_FATIGUETRK_ENV,
                  IDS_PEDESTRIAN_ENV,
                  IDS_STRENGTH_I_ENV,
                  IDS_SERVICE_I_ENV,
                  IDS_SERVICE_II_ENV,
                  IDS_SERVICE_III_ENV,
                  IDS_FATIGUE_ENV};

   delete legend;
   legend = new TLegend;

   //
   // Setup the cells
   //
   for (DiagramCatagory dc = dcDC; dc <= dcFatigue; dc++)
      {
      if (DrawDiagram(dc))
         {
         TSymbol *symbol;
         int penStyle;
         TColor color;
         string item;

         item = rview->GetDocument().GetDocManager().GetApplication()->LoadString(key[dc]);

         color = bview->bridgeDoc.GetPlotColor(dc);
         penStyle = bview->bridgeDoc.GetPenStyle(dc);
         symbol = bview->bridgeDoc.GetPlotSymbol(dc);

         legend->AddCell(new TLinestyleCell(item.c_str(),color,penStyle,symbol));
         }
      }
}
