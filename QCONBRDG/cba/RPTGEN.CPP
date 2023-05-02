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


#include <qconbrdg\rptgen.h>
#include <qconbrdg\table.h>
#include <iomanip.h>
#include <esl\engnotat.h>
#include <esl\strutil.h>
#include <code\lrfdcode.h>

TReportGenerator::TReportGenerator(const char *filename,TBridgeDocument &bdoc) :
fileName(filename),
bdoc(bdoc)
{
}

bool TReportGenerator::GenerateReport()
{
   char buffer[50];

   // Delete the temp report file
   ofstream ofile(fileName.c_str());

   if (!ofile)
      return false;

   ofile << endl;

   GetCodeEditionString(buffer,sizeof(buffer));
   ofile << "Code: " << buffer << endl;
   ofile << endl;

   //
   //
   // Spans
   //
   //
   ofile << "Span Data" << endl;
   ofile << "---------" << endl;
   ofile << endl;
   for (Span *span = bdoc.GetBridge().GetFirstSpan(); span; span = span->NextSpan())
      {
      WriteSpan(ofile,span);
      ofile << endl;
      }

   //
   //
   // Supports
   //
   //
   ofile << "Support Data" << endl;
   ofile << "------------" << endl;
   ofile << endl;

   for (Support *support = bdoc.GetBridge().GetFirstSupport(); support; support = support->NextSupport())
      {
      WriteSupport(ofile,support);
      ofile << endl;
      }

   //
   //
   // Loads
   //
   //
   ofile << "Loading Data" << endl;
   ofile << "------------" << endl;

   WriteDCLoads(ofile);
   ofile << endl;

   WriteDWLoads(ofile);
   ofile << endl;

   //
   //
   // Live Load
   //
   //
   ofile << "Live Load Data" << endl;
   ofile << "--------------" << endl;

   WriteLiveLoadGenerationParameters(ofile);
   ofile << endl;

   WriteLiveLoadImpact(ofile);
   ofile << endl;

   WritePedestrianLoads(ofile);
   ofile << endl;

   //
   //
   // Load Factors
   //
   //
   ofile << "Load Factors" << endl;
   ofile << "------------" << endl;
   WriteLoadFactors(ofile);
   ofile << endl;

   //
   //
   // Results
   //
   //
   ofile << "Analysis Results" << endl;
   ofile << "----------------" << endl;

   WriteAnalysisResults(ofile,dcDC);
   ofile << endl;

   WriteAnalysisResults(ofile,dcDW);
   ofile << endl;

   WriteAnalysisResults(ofile,dcAll);
   ofile << endl;

   WriteAnalysisResults(ofile,dcDesignTandem);
   ofile << endl;

   WriteAnalysisResults(ofile,dcDesignTruck);
   ofile << endl;

   WriteAnalysisResults(ofile,dcDualTruckTrain);
   ofile << endl;

   WriteAnalysisResults(ofile,dcDualTandemTrain);
   ofile << endl;

   WriteAnalysisResults(ofile,dcFatigueTruck);
   ofile << endl;

   WriteAnalysisResults(ofile,dcStrengthI);
   ofile << endl;

   WriteAnalysisResults(ofile,dcServiceI);
   ofile << endl;

   WriteAnalysisResults(ofile,dcServiceII);
   ofile << endl;

   WriteAnalysisResults(ofile,dcServiceIII);
   ofile << endl;

   WriteAnalysisResults(ofile,dcFatigue);
   ofile << endl;

   ofile.close();

   return true;
}

//
// -------------------------------------------------------------------------
//

ostream& TReportGenerator::WriteSpan(ostream &os,Span *s)
{
   LRFDSpan *span = TYPESAFE_DOWNCAST(s,LRFDSpan);

   int i; // general purpose counter
   double value; // general floating point value
   DerivedUnits::Length lengthConv = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::meter : USConstants::feet );
   DerivedUnits::Length2   areaConv   = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::millimeter2 : USConstants::inch2 );
   DerivedUnits::Length4   izConv   = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::millimeter4 : USConstants::inch4 );
   DerivedUnits::Pressure pressureConv = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::Pascal : USConstants::pound_per_inch2 );
   DerivedUnits::UnitWeight unitWgtConv = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::newton_per_meter3 : USConstants::pound_per_feet3 );

   string lengthUnit = ( (bdoc.GetUnits() == unitsSI) ? "m"    : "ft" );

   //
   //
   // Basic span information
   //
   //

   os << "Span " << setw(2) << span->GetID();
   os << "  ";
   value = DerivedUnits::Length(span->Length()).ConvertTo(lengthConv).value();
   os << "Length: "
      << setiosflags(ios::floatfield | ios::showpoint)
      << setw(10) << setprecision(3) << value << " " << lengthUnit;

   if (span->IsCantilevered())
      {
      os << " ";
      os << "Cantilevered";
      }

   bool startHinged = (span->StartContinuity() == ctHinged);
   bool endHinged   = (span->EndContinuity()   == ctHinged);

   if (startHinged && endHinged)
      {
      os << " ";
      os << "Both ends hinged";
      }
   else if (startHinged)
      {
      os << " ";
      os << "Start of span hinged";
      }
   else if (endHinged)
   {
   os << " ";
   os << "End of span hinged";
   }

   os << endl;

   //
   //
   // Inspan hinges
   //
   //

   if (span->NumHinges() > 0)
      os << "In-span Hinges:";

   for (i = 0; i < span->NumHinges(); i++)
      {
      Hinge *hinge = span->GetHinge(i);
      os << " ";
      value = DerivedUnits::Length(hinge->GetLocation()).ConvertTo(lengthConv).value();
      os << setiosflags(ios::floatfield | ios::showpoint)
         << setw(10) << setprecision(3) << value << " " << lengthUnit;
      }

   os << endl;

   //
   //
   // Segment data
   //
   //
   if (span->NumSegments() > 0)
      {
      WriteSegmentHeading(os);

      for (i = 0; i < span->NumSegments(); i++)
         {
         Segment *segment = span->GetSegment(i);
         WriteSegment(os,segment);
         }
      }

   os << endl;

   //
   //
   // Live load distribution factors
   //
   //
   WriteLiveLoadDistFactors(os,span);

   os << endl;

   //
   //
   // Limit state factors
   //
   //
   WriteLimitStateFactors(os,span);
   os << endl;

   return os;
}

ostream& TReportGenerator::WriteSupport(ostream &os,Support *support)
{
   int i; // general purpose counter
   double value; // general floating point value
   DerivedUnits::Length lengthConv = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::meter : USConstants::feet );
   DerivedUnits::Length2   areaConv   = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::millimeter2 : USConstants::inch2 );
   DerivedUnits::Length4   izConv   = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::millimeter4 : USConstants::inch4 );
   DerivedUnits::Pressure pressureConv = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::Pascal : USConstants::pound_per_inch2 );
   DerivedUnits::UnitWeight unitWgtConv = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::newton_per_meter3 : USConstants::pound_per_feet3 );

   string lengthUnit = ( (bdoc.GetUnits() == unitsSI) ? "m"    : "ft" );

   if (support->IsPier())
      {
      os << "Support " << setw(2) << support->GetID() << endl;
      value = DerivedUnits::Length(support->Height()).ConvertTo(lengthConv).value();
      os << "Height " << setiosflags(ios::floatfield | ios::showpoint)
         << setw(10) << setprecision(3) << value << " " << lengthUnit;
      os << "  ";
      value = DerivedUnits::Length(support->BaseOffset()).ConvertTo(lengthConv).value();
      os << "Offset " << setiosflags(ios::floatfield | ios::showpoint)
         << setw(10) << setprecision(3) << value << " " << lengthUnit;
      os << "  ";
      os << "Top ";
      switch(support->TopContinuity())
         {
         case ctContinuous:
              os << "Continuous";
              break;

         case ctHinged:
              os << "Hinged";
              break;
         }
      os << "  ";
      os << "Bottom ";
      switch(support->SupportFixity())
         {
         case fxPinned:
              os << "Pinned" << endl;
              break;
         case fxRoller:
              os << "Roller" << endl;
              break;
         case fxFixed:
              os << "Fixed" << endl;
              break;
         }
      os << endl;

      //
      //
      // Segment data
      //
      //
      if (support->NumSegments() > 0)
         {
         WriteSegmentHeading(os);

         for (i = 0; i < support->NumSegments(); i++)
            {
            Segment *segment = support->GetSegment(i);
            WriteSegment(os,segment);
            }
         }

      }
   else
      {
      os << "Support " << setw(2) << support->GetID();
      switch(support->SupportFixity())
         {
         case fxPinned:
              os << "  " << "Pinned" << endl;
              break;

         case fxRoller:
              os << "  " << "Roller" << endl;
              break;

         case fxFixed:
              os << "  " << "Fixed" << endl;
              break;

         }
      os << endl;
      }

   return os;
}

ostream& TReportGenerator::WriteSegmentHeading(ostream &os)
{
   os << "Section Properties" << endl;
   os << "Location      Ax           Iz         Mod. E      ";
   os << ( (bdoc.GetUnits() == unitsSI) ? "Density" : "Unit Wgt" );
   os << endl;

   if (bdoc.GetUnits() == unitsSI)
     os << "  (m)       (mm^2)       (mm^4)        (Pa)       (kg/m^3)" << endl;
   else
     os << "  (ft)      (in^2)       (in^4)        (psi)       (pcf)"   << endl;

   return os;
}

ostream& TReportGenerator::WriteSegment(ostream &os,Segment *segment)
{
   char lineBuf[81];
   char tmpBuf[20];
   double value; // general floating point value
   DerivedUnits::Length lengthConv = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::meter : USConstants::feet );
   DerivedUnits::Length2   areaConv   = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::millimeter2 : USConstants::inch2 );
   DerivedUnits::Length4   izConv   = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::millimeter4 : USConstants::inch4 );
   DerivedUnits::Pressure pressureConv = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::Pascal : USConstants::pound_per_inch2 );
   DerivedUnits::UnitWeight unitWgtConv = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::newton_per_meter3 : USConstants::pound_per_feet3 );

   ostrstream ostmp(tmpBuf,sizeof(tmpBuf));
   initstg(lineBuf,' ',sizeof(lineBuf));

   // Write segment location
   //
   value = DerivedUnits::Length(segment->GetLocation()).ConvertTo(lengthConv).value();
   putf(lineBuf,value,1,8,3);

   // Write section properties
   //
   TShape *section = segment->GetSection();
   value = DerivedUnits::Length2(section->GetArea()).ConvertTo(areaConv).value();
   ostmp.seekp(0);
   ostmp << EngNotation(value,3) << ends;
   putstg(lineBuf,tmpBuf,11,strlen(tmpBuf));

   value = DerivedUnits::Length4(section->GetCentroidalInertia().Ixx).ConvertTo(izConv).value();
   ostmp.seekp(0);
   ostmp << EngNotation(value,3) << ends;
   putstg(lineBuf,tmpBuf,24,strlen(tmpBuf));

   // Write material properties
   //
   value = DerivedUnits::Pressure(section->GetModE()).ConvertTo(pressureConv).value();
   ostmp.seekp(0);
   ostmp << EngNotation(value,3) << ends;
   putstg(lineBuf,tmpBuf,37,strlen(tmpBuf));

   value = DerivedUnits::UnitWeight(section->GetUnitWeight()).ConvertTo(unitWgtConv).value();
   if (bdoc.GetUnits() == unitsSI)
      value /= SIConstants::g.value();
   ostmp.seekp(0);
   ostmp << EngNotation(value,3) << ends;
   putstg(lineBuf,tmpBuf,50,strlen(tmpBuf));

   os << lineBuf << endl;

   return os;
}

ostream& TReportGenerator::WriteLiveLoadDistFactors(ostream &os,LRFDSpan *span)
{
   int i;
   char lineBuf[81];
   LLDistFactors *lldf;
   double value;
   DerivedUnits::Length lengthConv = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::meter : USConstants::feet );
/*
Live Load Distribution Factors
Location  Str/Serv Limit States  Fatigue Limit State
  (m)          gM        gV          gM        gV
  (ft)         gM        gV          gM        gV
xxxx.xxx    xxxx.xxx  xxxx.xxx    xxxx.xxx  xxxx.xxx
*/
   os << "Live Load Distribution Factors" << endl;
   os << "Location  Str/Serv Limit States  Fatigue Limit State" << endl;

   if (bdoc.GetUnits() == unitsSI)
     os << "  (m)          gM        gV          gM        gV" << endl;
   else
     os << "  (ft)         gM        gV          gM        gV"   << endl;

   for (i = 0; i < span->NumLLDistFactors(); i++)
      {
      initstg(lineBuf,' ',sizeof(lineBuf));

      lldf = span->GetLLDistFactors(i);

      value = DerivedUnits::Length(lldf->location).ConvertTo(lengthConv).value();
      putf(lineBuf,value,1,8,3);

      putf(lineBuf,lldf->s_gPosMoment,13,8,3);
      putf(lineBuf,lldf->s_gShear,    23,8,3);
      putf(lineBuf,lldf->f_gPosMoment,35,8,3);
      putf(lineBuf,lldf->f_gShear,    45,8,3);

      os << lineBuf << endl;
      }


   return os;
}

ostream& TReportGenerator::WriteLimitStateFactors(ostream &os,LRFDSpan *span)
{
   char lineBuf[81];
   char *strengthTitle = "Strength Limit State Factors: ";
   char *serviceTitle =  "Service  Limit State Factors: ";
   char *importance = "Importance ";
   char *redundancy = "Redundancy ";
   char *ductility  = "Ductility ";

   initstg(lineBuf,' ',sizeof(lineBuf));
   putstg(lineBuf,strengthTitle,1,strlen(strengthTitle));
   putstg(lineBuf,ductility,31,strlen(ductility));
   putf(lineBuf,span->GetDuctilityFactor(lsStrength),41,4,2);
   putstg(lineBuf,redundancy,47,strlen(redundancy));
   putf(lineBuf,span->GetRedundancyFactor(lsStrength),58,4,2);
   putstg(lineBuf,importance,64,strlen(importance));
   putf(lineBuf,span->GetImportanceFactor(lsStrength),75,4,2);

   os << lineBuf << endl;

   initstg(lineBuf,' ',sizeof(lineBuf));
   putstg(lineBuf,serviceTitle,1,strlen(serviceTitle));
   putstg(lineBuf,ductility,31,strlen(ductility));
   putf(lineBuf,span->GetDuctilityFactor(lsService),41,4,2);
   putstg(lineBuf,redundancy,47,strlen(redundancy));
   putf(lineBuf,span->GetRedundancyFactor(lsService),58,4,2);
   putstg(lineBuf,importance,64,strlen(importance));
   putf(lineBuf,span->GetImportanceFactor(lsService),75,4,2);

   os << lineBuf << endl;

   return os;
}

ostream& TReportGenerator::WriteDCLoads(ostream &os)
{
   double value;
   string lfUnit = ( (bdoc.GetUnits() == unitsSI) ? "N/m"    : "plf" );
   DerivedUnits::LinearForce lfConv = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::newton_per_meter : USConstants::pound_per_feet );

   os << "DC Loads" << endl;
   os << "   Self Weight Generation " << (bdoc.GetBridge().SelfWgtEnabled() ? "Enabled" : "Disabled") << endl;

   os << "   Traffic Barrier Load ";
   value = DerivedUnits::LinearForce(bdoc.GetBridge().TrafficBarrierLd()).ConvertTo(lfConv).value();
   if (bdoc.GetBridge().TrafficBarrierEnabled())
      os << setw(11) << EngNotation(value,3) << " " << lfUnit;
   else
      os << "Disabled";
   os << endl;

   BridgeLoading *loading = bdoc.GetBridge().GetLoading(DC_LOAD);
   WriteBridgeLoading(os,loading);

   return os;
}

ostream& TReportGenerator::WriteDWLoads(ostream &os)
{
   double value;
   string lfUnit = ( (bdoc.GetUnits() == unitsSI) ? "N/m"    : "plf" );
   DerivedUnits::LinearForce lfConv = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::newton_per_meter : USConstants::pound_per_feet );

   os << "DW Loads" << endl;

   os << "   Utility Load ";
   value = DerivedUnits::LinearForce(bdoc.GetBridge().UtilityLd()).ConvertTo(lfConv).value();
   if (bdoc.GetBridge().UtilityEnabled())
      os << setw(11) << EngNotation(value,3) << " " << lfUnit;
   else
      os << "Disabled";
   os << endl;

   os << "   Wearing Surface Load ";
   value = DerivedUnits::LinearForce(bdoc.GetBridge().WearSurfLd()).ConvertTo(lfConv).value();
   if (bdoc.GetBridge().WearSurfEnabled())
      os << setw(11) << EngNotation(value,3) << " " << lfUnit;
   else
      os << "Disabled";
   os << endl;

   BridgeLoading *loading = bdoc.GetBridge().GetLoading(DW_LOAD);
   WriteBridgeLoading(os,loading);

   return os;
}

ostream& TReportGenerator::WriteBridgeLoading(ostream &os,BridgeLoading *loading)
{
   int i;

   for (i = 0; i < loading->NumLoads(); i++)
      {
      BridgeLoad *load = loading->GetLoad(i);

      ConcBridgeLoad *cload;
      UnifBridgeLoad *uload;

      switch(load->Type())
         {
         case blConc:
              cload = TYPESAFE_DOWNCAST(load,ConcBridgeLoad);
              WriteConcBridgeLoad(os,cload);
              break;

         case blUnif:
              uload = TYPESAFE_DOWNCAST(load,UnifBridgeLoad);
              WriteUnifBridgeLoad(os,uload);
              break;
         }
      }

   return os;
}

ostream& TReportGenerator::WriteConcBridgeLoad(ostream &os,ConcBridgeLoad *cload)
{
   char lineBuf[81];
   char tmpBuf[20];
   double value;
   string forceUnit = ( (bdoc.GetUnits() == unitsSI) ? "N" : "lbs" );
   string locUnit = ( (bdoc.GetUnits() == unitsSI) ? "m"  : "ft" );
   DerivedUnits::Length lengthConv = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::meter : USConstants::feet );
   DerivedUnits::Force forceConv = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::newton : USConstants::pound );

   initstg(lineBuf,' ',sizeof(lineBuf));
   initstg(tmpBuf, ' ',sizeof(tmpBuf));

/*
   Span xx  P xxx.xxxexx N   at xxx.xxxL
   Span xx  P xxx.xxxexx lbs at xxx.xxxL
   Span xx  P xxx.xxxexx N   at xxx.xxx m
   Span xx  P xxx.xxxexx lbs at xxx.xxx ft
*/

   putstg(lineBuf,"Span",4,4);
   puti(lineBuf,cload->GetSpan()->GetID(),9,2);

   // write magnitude of load
   putchr(lineBuf,'P',13);
   ostrstream ostmp(tmpBuf,sizeof(tmpBuf));
   value = DerivedUnits::Force(cload->Magnitude()).ConvertTo(forceConv).value();
   ostmp << setw(11) << EngNotation(value,3) << " " << forceUnit << " at " << ends;
   putstg(lineBuf,tmpBuf,15,strlen(tmpBuf));

   // write location of load
   if (cload->IsFract())
      {
      // fractional specification
      putf(lineBuf,cload->StartLocation(),33,7,3);
      putchr(lineBuf,'L',40);
      }
   else
      {
      // absolute location
      value = DerivedUnits::Length(cload->StartLocation()).ConvertTo(lengthConv).value();
      putf(lineBuf,value,33,7,3);
      putstg(lineBuf,locUnit.c_str(),41,2);
      }

   os << lineBuf << endl;

   return os;
}

ostream& TReportGenerator::WriteUnifBridgeLoad(ostream &os,UnifBridgeLoad *uload)
{
   char lineBuf[81];
   char tmpBuf[30];
   double value;
   string forceUnit = ( (bdoc.GetUnits() == unitsSI) ? "N/m" : "plf" );
   string locUnit = ( (bdoc.GetUnits() == unitsSI) ? "m"  : "ft" );
   DerivedUnits::Length lengthConv = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::meter : USConstants::feet );
   DerivedUnits::LinearForce lfConv = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::newton_per_meter : USConstants::pound_per_feet );

   initstg(lineBuf,' ',sizeof(lineBuf));
   initstg(tmpBuf, ' ',sizeof(tmpBuf));

/*
   Span xx  W xxx.xxxexx N   from xxx.xxxL   to xxx.xxxL
   Span xx  W xxx.xxxexx lbs from xxx.xxxL   to xxx.xxxL
   Span xx  W xxx.xxxexx N   from xxx.xxx m  to xxx.xxx m
   Span xx  W xxx.xxxexx lbs from xxx.xxx ft to xxx.xxx ft
*/

   putstg(lineBuf,"Span",4,4);
   puti(lineBuf,uload->GetSpan()->GetID(),9,2);

   // write magnitude of load
   putchr(lineBuf,'W',13);
   ostrstream ostmp(tmpBuf,sizeof(tmpBuf));
   value = DerivedUnits::LinearForce(uload->Magnitude()).ConvertTo(lfConv).value();
   ostmp << setw(11) << EngNotation(value,3) << " " << forceUnit << " from " << ends;
   putstg(lineBuf,tmpBuf,15,strlen(tmpBuf));

   // write location of load
   if (uload->IsFract())
      {
      // fractional specification
      putf(lineBuf,uload->StartLocation(),35,7,3);
      putchr(lineBuf,'L',42);
      putstg(lineBuf,"to",46,2);
      putf(lineBuf,uload->EndLocation(),49,7,3);
      putchr(lineBuf,'L',56);
      }
   else
      {
      // absolute location
      value = DerivedUnits::Length(uload->StartLocation()).ConvertTo(lengthConv).value();
      putf(lineBuf,value,35,7,3);
      putstg(lineBuf,locUnit.c_str(),43,2);
      putstg(lineBuf,"to",46,2);
      value = DerivedUnits::Length(uload->EndLocation()).ConvertTo(lengthConv).value();
      putf(lineBuf,value,49,7,3);
      putstg(lineBuf,locUnit.c_str(),57,2);
      }

   os << lineBuf << endl;

   return os;
}

ostream& TReportGenerator::WritePedestrianLoads(ostream &os)
{
   double value;
   string lfUnit = ( (bdoc.GetUnits() == unitsSI) ? "N/m"    : "plf" );
   DerivedUnits::LinearForce lfConv = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::newton_per_meter : USConstants::pound_per_feet );

   os << "   Pedestrian Live Load";
   value = DerivedUnits::LinearForce(bdoc.GetBridge().PedestrianLd()).ConvertTo(lfConv).value();
   os << setw(13) << EngNotation(value,3) << " " << lfUnit << endl;

   return os;
}

ostream& TReportGenerator::WriteLiveLoadGenerationParameters(ostream &os)
{
   int i;
   double from, to;
   bool first;
   DerivedUnits::Length lengthConv = ( (bdoc.GetUnits() == unitsSI) ? SIConstants::meter : USConstants::feet );
   string lengthUnit = ( (bdoc.GetUnits() == unitsSI) ? "m"    : "ft" );

   os << "Live Load Generation Parameters" << endl;
   os << "   Design Tandem    : " << (bdoc.GetBridge().EnableDesignTandem() ? "Enabled" : "Disabled") << endl;
   os << "   Design Truck     : ";
   if (bdoc.GetBridge().EnableDesignTruck())
      {
      os << bdoc.GetBridge().GetNumAxleInc() << " rear axle spacing increments" << endl;
      }
   else
      {
      os << "Disabled" << endl;
      }

   os << "   Dual Truck Train :";
   if (!bdoc.GetBridge().DualTruckLoadEnabled(DUALTRUCKS))
      os << " Disabled" << endl;
   else
      {
      first = true;
      for (i = 0; i < 3; i++)
         {
         if (bdoc.GetBridge().DualTruckRangeEnabled(DUALTRUCKS,i))
            {
            from = DerivedUnits::Length(bdoc.GetBridge().FromHeadwaySpacing(DUALTRUCKS,i)).ConvertTo(lengthConv).value();
            to   = DerivedUnits::Length(bdoc.GetBridge().ToHeadwaySpacing(DUALTRUCKS,i)).ConvertTo(lengthConv).value();
            if (first)
               os << " ";
            else
               os << "                      ";
            first = false;
            os << "Headway Spacing varies from "
               << setw(6) << setiosflags(ios::floatfield | ios::showpoint | ios::right)
               << from << " " << lengthUnit << " to "
               << setw(6) << setiosflags(ios::floatfield | ios::showpoint | ios::right)
               << to << " " << lengthUnit
               << " using "
               << bdoc.GetBridge().NumHeadwayInc(DUALTRUCKS,i) << " increments" << endl;
            }
         }
      }

   os << "   Dual Tandem Train:";
   if (!bdoc.GetBridge().DualTruckLoadEnabled(DUALTANDEMS))
      os << " Disabled" << endl;
   else
      {
      for (i = 0; i < 3; i++)
         {
         first = true;
         if (bdoc.GetBridge().DualTruckRangeEnabled(DUALTANDEMS,i))
            {
            from = DerivedUnits::Length(bdoc.GetBridge().FromHeadwaySpacing(DUALTANDEMS,i)).ConvertTo(lengthConv).value();
            to   = DerivedUnits::Length(bdoc.GetBridge().ToHeadwaySpacing(DUALTANDEMS,i)).ConvertTo(lengthConv).value();
            if (first)
               os << " ";
            else
               os << "                      ";
            first = false;
            os << "Headway Spacing varies from "
               << setw(6) << setiosflags(ios::floatfield | ios::showpoint | ios::right)
               << from << " " << lengthUnit << " to "
               << setw(6) << setiosflags(ios::floatfield | ios::showpoint | ios::right)
               << to << " " << lengthUnit
               << " using "
               << bdoc.GetBridge().NumHeadwayInc(DUALTANDEMS,i) << " increments" << endl;
            }
         }
      }

   os << "   Fatigue Truck    : " << (bdoc.GetBridge().EnableFatigueTruck() ? "Enabled" : "Disabled") << endl;

   return os;
}

ostream& TReportGenerator::WriteLiveLoadImpact(ostream &os)
{
   os << "Live Load Impact" << endl;
   os << "   Truck Loads   "
      << setw(6) << setiosflags(ios::floatfield | ios::showpoint | ios::right)
      << bdoc.GetBridge().GetTruckImpact() << "%" << endl;
   os << "   Lane  Loads   "
      << setw(6) << setiosflags(ios::floatfield | ios::showpoint | ios::right)
      << bdoc.GetBridge().GetLaneImpact()  << "%" << endl;
   os << "   Fatigue Truck "
      << setw(6) << setiosflags(ios::floatfield | ios::showpoint | ios::right)
      << bdoc.GetBridge().GetFatigueImpact() << "%" << endl;
   return os;
}

ostream& TReportGenerator::WriteLoadFactors(ostream &os)
{
   double dcMin,dcMax,dwMin,dwMax,ll;

   bdoc.GetBridge().GetStrengthILoadFactors(dcMin,dcMax,dwMin,dwMax,ll);
   os << "   Strength I " << "  "
      << "DC min " << setw(5) << setiosflags(ios::floatfield | ios::showpoint | ios::right) << dcMin << " "
      << "DC max " << setw(5) << setiosflags(ios::floatfield | ios::showpoint | ios::right) << dcMax << " "
      << "DW min " << setw(5) << setiosflags(ios::floatfield | ios::showpoint | ios::right) << dwMin << " "
      << "DW max " << setw(5) << setiosflags(ios::floatfield | ios::showpoint | ios::right) << dwMax << " "
      << "LL     " << setw(5) << setiosflags(ios::floatfield | ios::showpoint | ios::right) << ll << endl;

   bdoc.GetBridge().GetServiceILoadFactors(dcMax,dwMax,ll);
   os << "   Service I  " << "  "
      << "DC     " << setw(5) << setiosflags(ios::floatfield | ios::showpoint | ios::right) << dcMax << " "
      << "DW     " << setw(5) << setiosflags(ios::floatfield | ios::showpoint | ios::right) << dwMax << " "
      << "LL     " << setw(5) << setiosflags(ios::floatfield | ios::showpoint | ios::right) << ll << endl;

   bdoc.GetBridge().GetServiceIILoadFactors(dcMax,dwMax,ll);
   os << "   Service II " << "  "
      << "DC     " << setw(5) << setiosflags(ios::floatfield | ios::showpoint | ios::right) << dcMax << " "
      << "DW     " << setw(5) << setiosflags(ios::floatfield | ios::showpoint | ios::right) << dwMax << " "
      << "LL     " << setw(5) << setiosflags(ios::floatfield | ios::showpoint | ios::right) << ll << endl;

   bdoc.GetBridge().GetServiceIIILoadFactors(dcMax,dwMax,ll);
   os << "   Service III" << "  "
      << "DC     " << setw(5) << setiosflags(ios::floatfield | ios::showpoint | ios::right) << dcMax << " "
      << "DW     " << setw(5) << setiosflags(ios::floatfield | ios::showpoint | ios::right) << dwMax << " "
      << "LL     " << setw(5) << setiosflags(ios::floatfield | ios::showpoint | ios::right) << ll << endl;

   bdoc.GetBridge().GetFatigueLoadFactors(dcMax,dwMax,ll);
   os << "   Fatigue    " << "  "
      << "DC     " << setw(5) << setiosflags(ios::floatfield | ios::showpoint | ios::right) << dcMax << " "
      << "DW     " << setw(5) << setiosflags(ios::floatfield | ios::showpoint | ios::right) << dwMax << " "
      << "LL     " << setw(5) << setiosflags(ios::floatfield | ios::showpoint | ios::right) << ll << endl;

   return os;
}

ostream& TReportGenerator::WriteAnalysisResults(ostream &os,DiagramCatagory dc)
{
   char buffer[80];
   int i,j;
   TTable *tbl = bdoc.CreateTable(dc);

   // Write table title
   os << tbl->GetTitle(buffer,sizeof(buffer)) << endl;

   // Write Table header
   for (i = 0; i < tbl->GetNumFields(); i++)
      {
      os << setw(tbl->GetFieldWidth(i)) << tbl->GetFieldName(i,buffer,sizeof(buffer));
      }
   os << endl;

   // Write table
   for (i = 0; i < tbl->GetNumRecords(); i++)
      {
      for (j = 0; j < tbl->GetNumFields(); j++)
         {
         os << setw(tbl->GetFieldWidth(j)) << setiosflags(ios::right)
            << tbl->GetEntry(i,j,buffer,sizeof(buffer));
         }
      os << endl;
      }

   delete tbl;

   os << endl << endl;

   tbl = bdoc.CreateTable(dc,true);
   os << tbl->GetTitle(buffer,sizeof(buffer)) << endl;
   for ( i = 0; i < tbl->GetNumFields(); i++ )
   {
      os << setw(tbl->GetFieldWidth(i)) << tbl->GetFieldName(i,buffer,sizeof(buffer));
   }
   os << endl;
   for (i = 0; i < tbl->GetNumRecords(); i++)
      {
      for (j = 0; j < tbl->GetNumFields(); j++)
         {
         os << setw(tbl->GetFieldWidth(j)) << setiosflags(ios::right)
            << tbl->GetEntry(i,j,buffer,sizeof(buffer));
         }
      os << endl;
      }

   delete tbl;

   return os;
}

