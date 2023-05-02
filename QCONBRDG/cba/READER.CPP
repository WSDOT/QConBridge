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

#include <qconbrdg\reader.h>
#include <qconbrdg\mytypes.h>
#include <code\lrfdcode.h>
#include <shape\genshape.h>
#include <material\genmat.h>
#include <cstring.h>
#include <stdio.h>

#define BUF_SIZE 130

FileHeader::FileHeader(const char *appName,int appMajorVer,int appMinorVer,int formatId) :
AppName(appName), AppMajorVer(appMajorVer), AppMinorVer(appMinorVer), FormatID(formatId)
{
}

void FileHeader::ReadHeader(ifstream &ifile)
{
   char buffer[BUF_SIZE];

   ifile.getline(buffer,sizeof(buffer));
   appName = buffer;

   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%d %d",&appMajorVer,&appMinorVer);
//   istrstream(buffer) >> appMajorVer >> appMinorVer;

   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%d",&formatId);
//   istrstream(buffer) >> formatId;
}

void FileHeader::WriteHeader(ofstream &ofile)
{
   ofile << AppName << endl;
   ofile << AppMajorVer << " " << AppMinorVer << endl;
   ofile << FormatID << endl;
}

bool FileHeader::IsValid()
{
   bool name,version,format;

   name = (AppName == appName);
   version = (AppMajorVer == appMajorVer && AppMinorVer == appMinorVer);
   format = (FormatID == formatId);

   return (name && version && format);
}


//
//
//
QConBridgeFile::QConBridgeFile(TBridgeDocument &b) :
bdoc(b),
bridge(bdoc.GetBridge())
{
}

void QConBridgeFile::Read(ifstream &ifile)
{
   int numSpans;
   int numSupports;
   int numLoads;
   int i,j;
   int isFirstSpanCantilevered;
   int isLastSpanCantilevered;
   LRFDSpan *span;
   Support *support;
   BridgeLoad *load;
   int loadType;
   BridgeLoading *dc;
   BridgeLoading *dw;
   bool enabled;
   double value;
   double truck,lane,fatigue;
   int numAxleInc;
   double dcMin,dcMax,dwMin,dwMax,ll;
   double pedestrian;
   double from,to;
   int inc;
   int minAnalysisPoints;
   int units;
   int code;
   char buffer[BUF_SIZE];

   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%d %d",&isFirstSpanCantilevered,&isLastSpanCantilevered);
//   istrstream(buffer) >> isFirstSpanCantilevered >> isLastSpanCantilevered;

   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%d %d %d",&numSpans,&numSupports,&numLoads);
//   istrstream(buffer) >> numSpans >> numSupports >> numLoads;

   dc = bridge.GetLoading(DC_LOAD);
   dw = bridge.GetLoading(DW_LOAD);

   // Generate a bridge with the correct number of spans and supports.
   for (i = 0; i < numSpans; i++)
      {
      span = new LRFDSpan(1,new GeneralShape(1,0,0,1,0,0,0,0,new GeneralMaterial(1,1)));
      support = new Support();
      bridge.AddSpanSupport(span,support,true);
      }

   if (isFirstSpanCantilevered)
      bridge.CantileverFirstSpan(true);

   if (isLastSpanCantilevered)
      bridge.CantileverLastSpan(true);

   for (i = 0; i < numSpans; i++)
      {
      span = ReadSpan(ifile);
      bridge.ReplaceSpan(i+1,span);
      }

   for (i = 0; i < numSupports; i++)
      {
      support = ReadSupport(ifile);
      bridge.ReplaceSupport(i+1,support);
      }

   for (i = 0; i < numLoads; i++)
      {
      ReadLoad(ifile,&load,loadType);
      if (loadType == DC_LOAD)
         dc->AddLoad(load);
      else
         dw->AddLoad(load);
      }

   // Selfweight
   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%d",&enabled);
//   istrstream(buffer) >> enabled;
   bridge.SelfWgtEnabled(enabled);

   // Traffic barrier
   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%d %lf",&enabled,&value);
//   istrstream(buffer) >> enabled >> value;
   bridge.TrafficBarrierEnabled(enabled);
   bridge.TrafficBarrierLd(value);

   // Wearing Surface
   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%d %lf",&enabled,&value);
//   istrstream(buffer) >> enabled >> value;
   bridge.WearSurfEnabled(enabled);
   bridge.WearSurfLd(value);

   // Utility
   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%d %lf",&enabled,&value);
//   istrstream(buffer) >> enabled >> value;
   bridge.UtilityEnabled(enabled);
   bridge.UtilityLd(value);

   // Impact Factors
   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%lf %lf %lf",&truck,&lane,&fatigue);
//   istrstream(buffer) >> truck >> lane >> fatigue;
   bridge.SetTruckImpact(truck);
   bridge.SetLaneImpact(lane);
   bridge.SetFatigueImpact(fatigue);

   // Design tandem
   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%d",&enabled);
//   istrstream(buffer) >> enabled;
   bridge.EnableDesignTandem(enabled);

   // Design truck
   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%d",&enabled);
//   istrstream(buffer) >> enabled;
   bridge.EnableDesignTruck(enabled);
   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%d",&numAxleInc);
//   istrstream(buffer) >> numAxleInc;
   bridge.SetNumAxleInc(numAxleInc);

   // Dual Truck Train
   for (j = DUALTRUCKS; j <= DUALTANDEMS; j++)
      {
      ifile.getline(buffer,sizeof(buffer));
      sscanf(buffer,"%d",&enabled);
//      istrstream(buffer) >> enabled;
      bridge.DualTruckLoadEnabled(j,enabled);
      for (i = 0; i < 3; i++)
         {
         ifile.getline(buffer,sizeof(buffer));
         sscanf(buffer,"%d %lf %lf %d",&enabled,&from,&to,&inc);
//         istrstream(buffer) >> enabled >> from >> to >> inc;
         bridge.DualTruckRangeEnabled(j,i,enabled);
         bridge.FromHeadwaySpacing(j,i,from);
         bridge.ToHeadwaySpacing(j,i,to);
         bridge.NumHeadwayInc(j,i,inc);
         }
      }

   // Fatigue truck
   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%d",&enabled);
//   istrstream(buffer) >> enabled;
   bridge.EnableFatigueTruck(enabled);

   // Load Factors
   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%lf %lf %lf %lf %lf",&dcMin,&dcMax,&dwMin,&dwMax,&ll);
//   istrstream(buffer) >> dcMin >> dcMax >> dwMin >> dwMax >> ll;
   bridge.SetStrengthILoadFactors(dcMin,dcMax,dwMin,dwMax,ll);

   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%lf %lf %lf",&dcMin,&dwMin,&ll);
//   istrstream(buffer) >> dcMin >> dwMin >> ll;
   bridge.SetServiceILoadFactors(dcMin,dwMin,ll);

   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%lf %lf %lf",&dcMin,&dwMin,&ll);
//   istrstream(buffer) >> dcMin >> dwMin >> ll;
   bridge.SetServiceIILoadFactors(dcMin,dwMin,ll);

   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%lf %lf %lf",&dcMin,&dwMin,&ll);
//   istrstream(buffer) >> dcMin >> dwMin >> ll;
   bridge.SetServiceIIILoadFactors(dcMin,dwMin,ll);

   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%lf %lf %lf",&dcMin,&dwMin,&ll);
//   istrstream(buffer) >> dcMin >> dwMin >> ll;
   bridge.SetFatigueLoadFactors(dcMin,dwMin,ll);

   // Pedestrian Live Load
   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%lf",&pedestrian);
//   istrstream(buffer) >> pedestrian;
   bridge.PedestrianLd(pedestrian);

   // Analysis Controls
   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%d",&minAnalysisPoints);
//   istrstream(buffer) >> minAnalysisPoints;
   bridge.SetMinAnalysisPoints(minAnalysisPoints);

   // Program Controls
   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%d",&units);
//   istrstream(buffer) >> units;
   bdoc.SetUnits((SystemOfUnits)units);

   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%d",&code);
//   istrstream(buffer) >> code;
   SetCodeEdition((codeEdition)code);
}

void QConBridgeFile::Write(ofstream &ofile)
{
   int numSpans;
   int numSupports;
   int numLoads;
   int i,j;
   double dcMin,dcMax,dwMin,dwMax,ll;

   ofile << (bridge.GetFirstSpan()->IsCantilevered()) << " " <<
            (bridge.GetLastSpan()->IsCantilevered())  << endl;

   numSpans = bridge.NumSpans();
   numSupports = bridge.NumSupports();

   BridgeLoading *dc, *dw;
   numLoads = 0;
   dc = bridge.GetLoading(DC_LOAD);
   numLoads += dc->NumLoads();
   dw = bridge.GetLoading(DW_LOAD);
   numLoads += dw->NumLoads();

   ofile << numSpans << " " << numSupports << " " << numLoads << endl;

   for (i = 0; i < numSpans; i++)
      {
      WriteSpan(ofile,TYPESAFE_DOWNCAST(bridge.GetSpan(i+1),LRFDSpan));
      }

   for (i = 0; i < numSupports; i++)
      WriteSupport(ofile,bridge.GetSupport(i+1));

   BridgeLoadIterator *iter;

   iter = dw->GetIterator();
   while (*iter)
      {
      BridgeLoad *load = (*iter)++;
      WriteLoad(ofile,load);
      }
   delete iter;

   iter = dc->GetIterator();
   while (*iter)
      {
      BridgeLoad *load = (*iter)++;
      WriteLoad(ofile,load);
      }
   delete iter;

   // SelfWeight
   ofile << bridge.SelfWgtEnabled() << endl;
   ofile << bridge.TrafficBarrierEnabled() << " "
         << bridge.TrafficBarrierLd() << endl;
   ofile << bridge.WearSurfEnabled() << " "
         << bridge.WearSurfLd() << endl;
   ofile << bridge.UtilityEnabled() << " "
         << bridge.UtilityLd() << endl;

   // Impact Factors
   ofile << bridge.GetTruckImpact()   << " "
         << bridge.GetLaneImpact()    << " "
         << bridge.GetFatigueImpact() << endl;

   // Design Tandem
   ofile << bridge.EnableDesignTandem() << endl;

   // Design Truck
   ofile << bridge.EnableDesignTruck() << endl;
   ofile << bridge.GetNumAxleInc() << endl;

   // Dual Truck Train
   for (j = DUALTRUCKS; j <= DUALTANDEMS; j++)
      {
      ofile << bridge.DualTruckLoadEnabled(j) << endl;

      for (i = 0; i < 3; i++)
         {
         ofile << bridge.DualTruckRangeEnabled(j,i) << " "
               << bridge.FromHeadwaySpacing(j,i)    << " "
               << bridge.ToHeadwaySpacing(j,i)      << " "
               << bridge.NumHeadwayInc(j,i)         << endl;
         }
      }

   // Fatigue Truck
   ofile << bridge.EnableFatigueTruck() << endl;

   // Load Factors
   bridge.GetStrengthILoadFactors(dcMin,dcMax,dwMin,dwMax,ll);
   ofile << dcMin << " " << dcMax << " " << dwMin << " " << dwMax << " " << ll << endl;

   bridge.GetServiceILoadFactors(dcMin,dwMin,ll);
   ofile << dcMin << " " << dwMin << " " << ll<< endl;

   bridge.GetServiceIILoadFactors(dcMin,dwMin,ll);
   ofile << dcMin << " " << dwMin << " " << ll<< endl;

   bridge.GetServiceIIILoadFactors(dcMin,dwMin,ll);
   ofile << dcMin << " " << dwMin << " " << ll<< endl;

   bridge.GetFatigueLoadFactors(dcMin,dwMin,ll);
   ofile << dcMin << " " << dwMin << " " << ll<< endl;

   // Pedestrian Live Load
   ofile << bridge.PedestrianLd() << endl;

   // Analysis Controls
   ofile << bridge.GetMinAnalysisPoints() << endl;

   // Program Controls
   ofile << bdoc.GetUnits() << endl;
   ofile << GetCodeEdition() << endl;
}

LRFDSpan *QConBridgeFile::ReadSpan(ifstream &ifile)
{
   double length;
   char leftContinuous;
   char rightContinuous;
   int seg, numSegments;
   int hinge, numHinges;
   int lldf, numLLDF;
   double loc;
   double ax, iz;
   double mode;
   double unitWeight;
   LRFDSpan *aSpan;
   TShape* shape;
   double s_gPM;
   double s_gNM;
   double s_gV;
   double f_gPM;
   double f_gNM;
   double f_gV;
   double strength_nd;
   double strength_nr;
   double strength_ni;
   double service_nd;
   double service_nr;
   double service_ni;
   char buffer[BUF_SIZE];

   // Read basic span record
   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%lf %c %c %d %d %d",&length,&leftContinuous,&rightContinuous,&numSegments,&numHinges,&numLLDF);
//   istrstream(buffer) >> length >> leftContinuous >> rightContinuous >> numSegments >> numHinges >> numLLDF;

   // Read first segment (which must always exist
   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%lf %lf %lf %lf %lf",&loc,&ax,&iz,&mode,&unitWeight);
//   istrstream(buffer) >> loc >> ax >> iz >> mode >> unitWeight;

   // Create a new span
   shape = new GeneralShape(ax,0,0,iz,0,0,0,0,new GeneralMaterial(mode,unitWeight));
   shape->ComputeProperties();
   aSpan = new LRFDSpan(length,shape);

   // Read remaining segments
   for (seg = 1; seg < numSegments; seg++)
      {
      ifile.getline(buffer,sizeof(buffer));
      sscanf(buffer,"%lf %lf %lf %lf %lf",&loc,&ax,&iz,&mode,&unitWeight);
//      istrstream(buffer) >> loc >> ax >> iz >> mode >> unitWeight;
      shape = new GeneralShape(ax,0,0,iz,0,0,0,0,new GeneralMaterial(mode,unitWeight));
      shape->ComputeProperties();
      aSpan->AddSegment(new Segment(loc,shape));
      }

   // Read in-span hinges
   for (hinge = 0; hinge < numHinges; hinge++)
      {
      ifile.getline(buffer,sizeof(buffer));
      sscanf(buffer,"%lf",&loc);
//      istrstream(buffer) >> loc;
      aSpan->AddHinge(new Hinge(loc));
      }

   // Modify Boundary Conditions
   if (leftContinuous == 'H')
      aSpan->StartContinuity(ctHinged);

   if (rightContinuous == 'H')
      aSpan->EndContinuity(ctHinged);

   // Live Load Distribution Factors
   for (lldf = 0; lldf < numLLDF; lldf++)
      {
      ifile.getline(buffer,sizeof(buffer));
      sscanf(buffer,"%lf %lf %lf %lf %lf %lf %lf",&loc,&s_gPM,&s_gNM,&s_gV,&f_gPM,&f_gNM,&f_gV);
//      istrstream(buffer) >> loc >> s_gPM >> s_gNM >> s_gV >> f_gPM >> f_gNM >> f_gV;
      aSpan->AddLLDistFactors(new LLDistFactors(loc,s_gPM,s_gNM,s_gV,f_gPM,f_gNM,f_gV));
      }

   // Limit State Factors
   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%lf %lf %lf %lf %lf %lf",&strength_nd,&strength_nr,&strength_ni,&service_nd,&service_nr,&service_ni);
//   istrstream(buffer) >> strength_nd >> strength_nr >> strength_ni
//                      >> service_nd  >> service_nr  >> service_ni;
   aSpan->SetDuctilityFactor(strength_nd,lsStrength);
   aSpan->SetRedundancyFactor(strength_nr,lsStrength);
   aSpan->SetImportanceFactor(strength_ni,lsStrength);
   aSpan->SetDuctilityFactor(service_nd,lsService);
   aSpan->SetRedundancyFactor(service_nr,lsService);
   aSpan->SetImportanceFactor(service_ni,lsService);

   return aSpan;
}

Support *QConBridgeFile::ReadSupport(ifstream &ifile)
{
   int segment,numSegments;
   char baseFixity;
   char topContinuity;
   int typeSimple;
   double bo,h;
   double loc,ax,iz,e,d;
   char buffer[BUF_SIZE];

   Support *s;
   TShape* shape;

   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%d %c",&typeSimple,&baseFixity);
//   istrstream(buffer) >> typeSimple >> baseFixity;

   if (typeSimple)
      {
      // Simple Support
      s = new Support();
      s->SupportFixity((baseFixity == 'R') ? fxRoller : fxPinned);
      }
   else
      {
      // Pier
      ifile.getline(buffer,sizeof(buffer));
      sscanf(buffer,"%lf %lf %c %d",&h,&bo,&topContinuity,&numSegments);
//      istrstream(buffer) >> h >> bo >> topContinuity >> numSegments;

      ifile.getline(buffer,sizeof(buffer));
      sscanf(buffer,"%lf %lf %lf %lf %lf",&loc,&ax,&iz,&e,&d);
//      istrstream(buffer) >> loc >> ax >> iz >> e >> d;
      shape = new GeneralShape(ax,0,0,iz,0,0,0,0,new GeneralMaterial(e,d));
      shape->ComputeProperties();
      s = new Support(h,bo,shape);
      s->TopContinuity( (topContinuity == 'H') ? ctHinged : ctContinuous );
      s->SupportFixity((baseFixity == 'R') ? fxRoller : (baseFixity == 'P') ? fxPinned : fxFixed);

      for (segment = 1; segment < numSegments; segment++)
         {
         ifile.getline(buffer,sizeof(buffer));
         sscanf(buffer,"%lf %lf %lf %lf %lf",&loc,&ax,&iz,&e,&d);
//         istrstream(buffer) >> loc >> ax >> iz >> e >> d;
         shape = new GeneralShape(ax,0,0,iz,0,0,0,0,new GeneralMaterial(e,d));
         shape->ComputeProperties();
         s->AddSegment(new Segment(loc,shape));
         }
      }

   return s;
}

void QConBridgeFile::ReadLoad(ifstream &ifile,BridgeLoad **load,int &loadType)
{
   int type;
   int span;
   double start,end;
   bool isFract;
   double mag;
   Span *aSpan;
   char buffer[BUF_SIZE];

   ifile.getline(buffer,sizeof(buffer));
   sscanf(buffer,"%d %d %lf %lf %d %lf %d",&type,&span,&start,&end,&isFract,&mag,&loadType);
//   istrstream(buffer) >> type >> span >> start >> end >> isFract >> mag >> loadType;

   aSpan = bridge.GetSpan(span);

   if (type == blConc)
      {
      *load = new ConcBridgeLoad(aSpan,start,mag,isFract);
      }
   else
      {
      *load = new UnifBridgeLoad(aSpan,start,end,mag,isFract);
      }
}

void QConBridgeFile::WriteSpan(ofstream &ofile,LRFDSpan *span)
{
   Continuity continuity;
   int seg, numSegments;
   int hinge, numHinges;
   int lldf, numLLDF;

   ofile << span->Length() << " ";

   continuity = span->StartContinuity();
   switch(continuity)
      {
      case ctHinged:
           ofile << "H ";
           break;

      case ctContinuous:
           ofile << "C ";
           break;
      }

   continuity = span->EndContinuity();
   switch(continuity)
      {
      case ctHinged:
           ofile << "H ";
           break;

      case ctContinuous:
           ofile << "C ";
           break;
      }

   numSegments = span->NumSegments();
   numHinges = span->NumHinges();
   numLLDF = span->NumLLDistFactors();
   ofile << numSegments << " " << numHinges << " " << numLLDF << endl;

   for (seg = 0; seg < numSegments; seg++)
      {
      Segment *segment = span->GetSegment(seg);
      TShape *s = segment->GetSection();

      ofile << segment->GetLocation() << " "
            << s->GetArea() << " " << s->GetCentroidalInertia().Ixx << " "
            << s->GetModE() << " " << s->GetUnitWeight() << endl;
      }

   for (hinge = 0; hinge < numHinges; hinge++)
      {
      Hinge *h = span->GetHinge(hinge);
      ofile << h->GetLocation() << endl;
      }

   //
   // Live Load Distribution Factors
   //

   for (lldf = 0; lldf < numLLDF; lldf++)
      {
      LLDistFactors *llDistFact = span->GetLLDistFactors(lldf);
      ofile << llDistFact->location     << " "
            << llDistFact->s_gPosMoment << " "
            << llDistFact->s_gNegMoment << " "
            << llDistFact->s_gShear     << " "
            << llDistFact->f_gPosMoment << " "
            << llDistFact->f_gNegMoment << " "
            << llDistFact->f_gShear     << endl;
      }

   // Limit State Factors
   ofile << span->GetDuctilityFactor(lsStrength)  << " "
         << span->GetRedundancyFactor(lsStrength) << " "
         << span->GetImportanceFactor(lsStrength) << " "
         << span->GetDuctilityFactor(lsService)   << " "
         << span->GetRedundancyFactor(lsService)  << " "
         << span->GetImportanceFactor(lsService)  << endl;
}

void QConBridgeFile::WriteSupport(ofstream &ofile,Support *support)
{
   int seg, numSegments;

   if (support->IsPier())
      {
      ofile << "0 ";
      switch(support->SupportFixity())
         {
         case fxRoller:
              ofile << "R";
              break;

         case fxPinned:
              ofile << "P";
              break;

         case fxFixed:
              ofile << "F";
              break;
         }
      ofile << endl;

      ofile << support->Height() << " " << support->BaseOffset() << " ";
      if (support->TopContinuity() == ctHinged)
         ofile << "H ";
      else
         ofile << "C ";

      numSegments = support->NumSegments();
      ofile << numSegments << endl;

      for (seg = 0; seg < numSegments; seg++)
         {
         Segment *segment = support->GetSegment(seg);
         TShape *s = segment->GetSection();

         ofile << segment->GetLocation() << " "
               << s->GetArea() << " " << s->GetCentroidalInertia().Ixx << " "
               << s->GetModE() << " " << s->GetUnitWeight() << endl;
         }
      }
   else
      {
      ofile << "1 ";
      switch(support->SupportFixity())
         {
         case fxRoller:
              ofile << "R";
              break;

         case fxPinned:
              ofile << "P";
              break;
         }
      ofile << endl;
      }
}

void QConBridgeFile::WriteLoad(ofstream &ofile,BridgeLoad *load)
{
   if (load->Type() == blConc)
      {
      ConcBridgeLoad *cload = TYPESAFE_DOWNCAST(load,ConcBridgeLoad);
      ofile << cload->Type()             << " "
            << cload->GetSpan()->GetID() << " "
            << cload->StartLocation()    << " "
            << "0.00"                    << " "
            << cload->IsFract()          << " "
            << cload->Magnitude()        << " "
            << cload->GetOwner()->GetID() << endl;
      }
   else
      {
      UnifBridgeLoad *uload = TYPESAFE_DOWNCAST(load,UnifBridgeLoad);
      ofile << uload->Type()             << " "
            << uload->GetSpan()->GetID() << " "
            << uload->StartLocation()    << " "
            << uload->EndLocation()      << " "
            << uload->IsFract()          << " "
            << uload->Magnitude()        << " "
            << uload->GetOwner()->GetID() << endl;
      }
}

