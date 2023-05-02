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

#include <qconbrdg\clipbord.h>
#include <shape\genshape.h>
#include <material\genmat.h>
#include <stdio.h>

#define BUF_SIZE 130

//{{TQConBridgeClipboard Implementation}}
TQConBridgeClipboard::TQConBridgeClipboard (HWND hWnd):
    TClipboard(hWnd)
{
// The TClipboard constructor opens the clipboard, the TClipboard
// virtual destructor closes it.
    clipSpanFormat = RegisterClipboardFormat("QCB_Span");
    clipLoadFormat = RegisterClipboardFormat("QCB_Load");
}


TQConBridgeClipboard::~TQConBridgeClipboard ()
{
    // INSERT>> Your destructor code here.

}

void TQConBridgeClipboard::Put(LRFDSpan &span,Support &support,bool isRightSupport)
{
    TRACE("Entering TQConBridgeClipboard::Put for span and support");
    ostrstream os;

    os << ((&span) ? true : false) << " ";
    os << ((&support) ? true : false) << " ";
    os << isRightSupport << endl;

    if (&span)
       WriteSpan(os,&span);

    if (&support)
       WriteSupport(os,&support);

    os << ends;

    // Open and empty the clipboard
    EmptyClipboard();

    // Fill-out the clipboard data struct
    int size = sizeof(char)*strlen(os.str())+1;
    HGLOBAL hClipdata = GlobalAlloc(GHND,size);
    char *buffer = (char *)GlobalLock(hClipdata);
    strcpy(buffer,os.str());

    // Put the data on the clipboard
    SetClipboardData(clipSpanFormat,hClipdata);
    GlobalUnlock(hClipdata);

    delete[] os.str();

    TRACE("Leaving TQConBridgeClipboard::Put for span and support");
}

void TQConBridgeClipboard::Get(LRFDSpan **span,Support **support,bool &isRightSupport)
{
    // Get the data off the clipboard
    HANDLE hClipMemory = GetClipboardData(clipSpanFormat);
    char *buffer = (char *)GlobalLock(hClipMemory);
    istrstream is(buffer,strlen(buffer));
    char temp[BUF_SIZE];

    int isSpan;
    int isSupport;
    int rs;

    is.getline(temp,sizeof(temp));
    sscanf(temp,"%d %d %d",&isSpan,&isSupport,&rs);
    isRightSupport = rs ? true : false;
//    is >> isSpan >> isSupport >> isRightSupport;

    if (isSpan)
       *span = ReadSpan(is);

    if (isSupport)
       *support = ReadSupport(is);

    GlobalUnlock(hClipMemory);
}

void TQConBridgeClipboard::Put(BridgeLoad &load)
{
    TRACE("Entering TQConBridgeClipboard::Put for loads");
    ConcBridgeLoad *cload;
    UnifBridgeLoad *uload;

    // Open and empty the clipboard
    EmptyClipboard();

    // Fill-out the clipboard data struct
    HGLOBAL hClipdata = GlobalAlloc(GHND,sizeof(ClipboardLoadData));
    ClipboardLoadData *clipData = (ClipboardLoadData *)GlobalLock(hClipdata);

    clipData->Type = load.Type();
    clipData->Lcase = load.GetOwner()->GetID();

    if (load.Type() == blConc)
       {
       cload = TYPESAFE_DOWNCAST(&load,ConcBridgeLoad);

       clipData->Start = cload->StartLocation();
       clipData->End = 0.00;
       clipData->IsFract = cload->IsFract();
       clipData->Mag = cload->Magnitude();
       }
    else
       {
       uload = TYPESAFE_DOWNCAST(&load,UnifBridgeLoad);

       clipData->Start = uload->StartLocation();
       clipData->End = uload->EndLocation();
       clipData->IsFract = uload->IsFract();
       clipData->Mag = uload->Magnitude();
       }

    // Put the data on the clipboard
    SetClipboardData(clipLoadFormat,hClipdata);
    GlobalUnlock(hClipdata);
    TRACE("Leaving TQConBridgeClipboard::Put for loads");
}

void TQConBridgeClipboard::Get(BridgeLoad **load,int &lcase)
{
    // Get the data off the clipboard
    HANDLE hClipMemory = GetClipboardData(clipLoadFormat);
    HANDLE hMyMemory = GlobalAlloc(GHND,sizeof(ClipboardLoadData));
    ClipboardLoadData *clipData   = (ClipboardLoadData *)GlobalLock(hClipMemory);
    ClipboardLoadData *myClipData = (ClipboardLoadData *)GlobalLock(hMyMemory);
    memcpy(myClipData,clipData,sizeof(ClipboardLoadData));
    GlobalUnlock(hClipMemory);
    GlobalUnlock(hMyMemory);

    if (clipData->Type == blConc)
       {
       *load = new ConcBridgeLoad(NULL,clipData->Start,clipData->Mag,clipData->IsFract);
       }
    else
       {
       *load = new UnifBridgeLoad(NULL,clipData->Start,clipData->End,clipData->Mag,clipData->IsFract);
       }

    lcase = clipData->Lcase;

    GlobalFree(hMyMemory);
}

bool TQConBridgeClipboard::IsQCBSpanFormatAvailable() const
{
   return IsClipboardFormatAvailable(clipSpanFormat);
}

bool TQConBridgeClipboard::IsQCBLoadFormatAvailable() const
{
   return IsClipboardFormatAvailable(clipLoadFormat);
}


void TQConBridgeClipboard::WriteSpan(ostream &ofile,LRFDSpan *span)
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

void TQConBridgeClipboard::WriteSupport(ostream &ofile,Support *support)
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


LRFDSpan *TQConBridgeClipboard::ReadSpan(istream &ifile)
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

Support *TQConBridgeClipboard::ReadSupport(istream &ifile)
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

