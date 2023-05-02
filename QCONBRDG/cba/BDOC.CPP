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


#include <qconbrdg\bdoc.h>
#include <qconbrdg\reader.h>
#include <qconbrdg\viewev.h>
#include <qconbrdg\qcbapp.h>
#include <qconbrdg\bdocmgr.h>
#include <code\lrfdcode.h>

#include <owl\docmanag.h>
#include <owl\profile.h>
#include <strstrea.h>
#include <iomanip.h>
#include <dir.h>
#include <cstring.h>

#define majorVersion   1
#define minorVersion   0
#define formatVersion 11

//{{TBridgeDocument Implementation}}


TBridgeDocument::TBridgeDocument (TDocument* parent):
    TDocument(parent)
{
    // Set the lrfd code version
    SetCodeEdition(lrfdFirstEdition94);

    bridge = NULL;
    SetupBridge();

    drawDimensions = false;
    drawLabels = false;
    drawSegmentTicks = false;
    units = unitsSI;
    isAnalyzed = false;

   for (DiagramCatagory dc = dcDC; dc <= dcFatigue; dc++)
      {
      plotStyleData[plScreen][dc].color = TColor::Black;
      plotStyleData[plScreen][dc].penStyle = PS_SOLID;
      plotStyleData[plScreen][dc].symbol = stNull;

      plotStyleData[plPrinter][dc].color = TColor::Black;
      plotStyleData[plPrinter][dc].penStyle = PS_SOLID;
      plotStyleData[plPrinter][dc].symbol = stNull;
      }

   symbol[stCircle]           = new TCircleSymbol();
   symbol[stSquare]           = new TSquareSymbol();
   symbol[stUpTriangle]       = new TUpTriangleSymbol();
   symbol[stDnTriangle]       = new TDnTriangleSymbol();
   symbol[stTickSquare]       = new TTickSquareSymbol();
   symbol[stDiamond]          = new TDiamondSymbol();
   symbol[stPlus]             = new TPlusSymbol();
   symbol[stCross]            = new TCrossSymbol();
   symbol[stAsterisk]         = new TAsteriskSymbol();
   symbol[stHourGlass]        = new THourGlassSymbol();
   symbol[stNull]             = new TNullSymbol();

   // clear this buffer...
   memset(tmpFile,0,MAXPATH);

   ReadINI();
}


TBridgeDocument::~TBridgeDocument ()
{
    WriteINI();

    delete bridge;

    for (TSymbolType st = stCircle; st <= stNull; st++)
       delete symbol[st];

    // Delete the temp report file
    DeleteTempReportFile();
}

//
//  The ini file itself should be an object... This will releive TBridgeDocument
//  from the responsibilities of managing the ini file!!!
//

void TBridgeDocument::WriteINI()
{
   string path = "qconbrdg.ini";

   TProfile plotStyleScreenColor("PlotStyle.Screen.Color",    path.c_str());
   TProfile plotStyleScreenPen("PlotStyle.Screen.Pen",        path.c_str());
   TProfile plotStyleScreenSymbol("PlotStyle.Screen.Symbol",  path.c_str());
   TProfile plotStylePrinterColor("PlotStyle.Printer.Color",  path.c_str());
   TProfile plotStylePrinterPen("PlotStyle.Printer.Pen",      path.c_str());
   TProfile plotStylePrinterSymbol("PlotStyle.Printer.Symbol",path.c_str());
   TProfile decorations("Decorations",                        path.c_str());
   TProfile sysUnits("Units",                                 path.c_str());
   int red;
   int green;
   int blue;
   TColor color;
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


   for (DiagramCatagory dc = dcDC; dc <= dcFatigue; dc++)
      {
      char buffer[15];
      ostrstream os(buffer,sizeof(buffer));
      string item;

      item = GetDocManager().GetApplication()->LoadString(key[dc]);

      color = GetPlotColor(plScreen,dc);
      red = color.Red();
      green = color.Green();
      blue = color.Blue();

      os << setw(3) << red << " " << green << " " << blue << ends;
      plotStyleScreenColor.WriteString(item.c_str(),buffer);

      color = GetPlotColor(plPrinter,dc);
      red = color.Red();
      green = color.Green();
      blue = color.Blue();

      os.seekp(0);
      os << setw(3) << red << " " << green << " " << blue << ends;
      plotStylePrinterColor.WriteString(item.c_str(),buffer);

      plotStyleScreenPen.WriteInt(item.c_str(),GetPenStyle(plScreen,dc));
      plotStylePrinterPen.WriteInt(item.c_str(),GetPenStyle(plPrinter,dc));

      plotStyleScreenSymbol.WriteInt(item.c_str(),GetPlotSymbolType(plScreen,dc));
      plotStylePrinterSymbol.WriteInt(item.c_str(),GetPlotSymbolType(plPrinter,dc));
      }

   decorations.WriteInt("Label",DisplayLabels());
   decorations.WriteInt("Tick",DisplaySegmentTicks());
   decorations.WriteInt("Dimensions",DisplayDimensions());
   decorations.WriteInt("Grid",DisplayGrid());
   decorations.WriteInt("Invert",InvertMomentDiagrams());

   sysUnits.WriteInt("Units",GetUnits());
}

void TBridgeDocument::ReadINI()
{
   string path = "qconbrdg.ini";

   TProfile plotStyleScreenColor("PlotStyle.Screen.Color",    path.c_str());
   TProfile plotStyleScreenPen("PlotStyle.Screen.Pen",        path.c_str());
   TProfile plotStyleScreenSymbol("PlotStyle.Screen.Symbol",  path.c_str());
   TProfile plotStylePrinterColor("PlotStyle.Printer.Color",  path.c_str());
   TProfile plotStylePrinterPen("PlotStyle.Printer.Pen",      path.c_str());
   TProfile plotStylePrinterSymbol("PlotStyle.Printer.Symbol",path.c_str());
   TProfile decorations("Decorations",                        path.c_str());
   TProfile sysUnits("Units",                                 path.c_str());
   int red;
   int green;
   int blue;
   int key[NUM_LOADINGS] = {IDS_DC,
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


   for (DiagramCatagory dc = dcDC; dc <= dcFatigue; dc++)
      {
      string item;

      item = GetDocManager().GetApplication()->LoadString(key[dc]);

      char buffer[15];
      plotStyleScreenColor.GetString(item.c_str(),buffer,sizeof(buffer),"0 0 0");
      istrstream is1(buffer);
      is1 >> red >> green >> blue;
      SetPlotColor(plScreen,dc,TColor(RGB(red,green,blue)));

      SetPenStyle(plScreen,dc,plotStyleScreenPen.GetInt(item.c_str(),PS_SOLID));
      SetPlotSymbolType(plScreen,dc,(TSymbolType)plotStyleScreenSymbol.GetInt(item.c_str(),stNull));

      plotStylePrinterColor.GetString(item.c_str(),buffer,sizeof(buffer),"0 0 0");
      istrstream is2(buffer);
      is2 >> red >> green >> blue;
      SetPlotColor(plPrinter,dc,TColor(RGB(red,green,blue)));

      SetPenStyle(plPrinter,dc,plotStylePrinterPen.GetInt(item.c_str(),PS_SOLID));
      SetPlotSymbolType(plPrinter,dc,(TSymbolType)plotStylePrinterSymbol.GetInt(item.c_str(),stNull));
      }

   DisplayLabels(decorations.GetInt("Label",false));
   DisplaySegmentTicks(decorations.GetInt("Tick",false));
   DisplayDimensions(decorations.GetInt("Dimensions",false));
   DisplayGrid(decorations.GetInt("Grid",false));
   InvertMomentDiagrams(decorations.GetInt("Invert",false));

   SetUnits((SystemOfUnits)sysUnits.GetInt("Units",unitsSI));
}

void TBridgeDocument::SetAnalyzed(bool analyzed)
{
   bool notify = (isAnalyzed != analyzed);

   if (!analyzed)
      DeleteTempReportFile();

   isAnalyzed = analyzed;

   if (notify)
      NotifyViews(vnAnalyzed,analyzed);
}

void TBridgeDocument::DeleteTempReportFile()
{
   OFSTRUCT ofStruct;
   OpenFile(tmpFile,&ofStruct,OF_DELETE);
   memset(tmpFile,0,MAXPATH);
}

void TBridgeDocument::Clear()
{
   NotifyViews(vnClearAll,0);
   bridge->Clear();
   SetupBridge();
   ResetToDefaults();
   SetDirty(true);
   isAnalyzed = false;
}

LRFDBridge &TBridgeDocument::GetBridge()
{
   return *bridge;
}

void TBridgeDocument::CreateFromPrototype(BridgePrototype* prototype)
{
   Clear(); // calls SetupBridge

   prototype->GenerateModel(this);
   ResetToDefaults();
   SetDirty(true);
   SetAnalyzed(false);
}


bool TBridgeDocument::Open (int /*mode*/, const char far* path)
{
    if (path)
       SetDocPath(path);

    if ( GetDocPath() )
       {
       SetupBridge();
       ifstream ifile(GetDocPath());

       if (!ifile)
          return false;

       QConBridgeFile reader(*this);
       FileHeader header(GetDocManager().GetApplication()->GetName(),majorVersion,minorVersion,formatVersion);
       header.ReadHeader(ifile);
       if (!header.IsValid())
          {
          ostrstream msg;
          msg << "Error Reading " << GetDocPath() << endl;
          msg << "Invalid File Format" << ends;
          GetDocManager().GetApplication()->GetMainWindow()->MessageBox(msg.str(),"Error",MB_OK | MB_ICONEXCLAMATION);
          delete[] msg.str();
          return false;
          }

       reader.Read(ifile);
       }

    SetDirty(false); // new file, it is not dirty

    return true;
}


bool TBridgeDocument::Commit (bool force)
{
   if (!IsDirty() && !force)
      return true;

   ofstream ofile(GetDocPath());

   if (!ofile && ofile.bad())
      return false;

   QConBridgeFile writer(*this);
   FileHeader header(GetDocManager().GetApplication()->GetName(),majorVersion,minorVersion,formatVersion);
   header.WriteHeader(ofile);
   writer.Write(ofile);

   SetDirty(false);

   return true;
}


bool TBridgeDocument::Revert (bool /*clear*/)
{
   // WARNING: This disables checking of child documents!!!
   Clear();
   return Open(0,GetDocPath());
}

void TBridgeDocument::SetupBridge()
{
   if (bridge)
      delete bridge;

   bridge = new LRFDBridge();
   ResetToDefaults();

   // The first support of the original bridge has been deleted.
   // A new first support has been create. The views must be notified
   // that they must rebuild the display objects. This will work for now.
   NotifyViews(vnRebuildDisplay,0);
}

bool TBridgeDocument::IsOpen ()
{
   return (bridge != NULL);
}


bool TBridgeDocument::CanClose ()
{
    bool result;

    result = TDocument::CanClose();

    // INSERT>> Your code here.

    return result;
}


bool TBridgeDocument::Close ()
{
    // If this is the parent doc, update the recent file list
    // (don't update if this is an embedded document)
    if (GetParentDoc() == 0)
       {
       TQConBridgeApp* app = TYPESAFE_DOWNCAST(GetDocManager().GetApplication(),TQConBridgeApp);
       const char* path = GetDocPath();
       if (path && app)
          app->SaveMenuChoice(path);
       }

    return TDocument::Close();
}

