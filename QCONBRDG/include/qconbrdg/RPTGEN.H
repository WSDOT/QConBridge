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

#if !defined RPTGEN_H
#define RPTGEN_H

#include <qconbrdg\bdoc.h>

class TReportGenerator
{
public:
   TReportGenerator(const char *filename,TBridgeDocument &bdoc);

   bool GenerateReport();

protected:
   string fileName;
   TBridgeDocument &bdoc;

   ostream& WriteSpan(ostream &os,Span *s);
   ostream& WriteSupport(ostream &os,Support *support);
   ostream& WriteSegmentHeading(ostream &os);
   ostream& WriteSegment(ostream &os,Segment *segment);
   ostream& WriteLiveLoadDistFactors(ostream &os,LRFDSpan *span);
   ostream& WriteLimitStateFactors(ostream &os,LRFDSpan *span);
   ostream& WriteDCLoads(ostream &os);
   ostream& WriteDWLoads(ostream &os);
   ostream& WriteBridgeLoading(ostream &os,BridgeLoading *loading);
   ostream& WriteConcBridgeLoad(ostream &os,ConcBridgeLoad *cload);
   ostream& WriteUnifBridgeLoad(ostream &os,UnifBridgeLoad *cload);
   ostream& WritePedestrianLoads(ostream &os);
   ostream& WriteLiveLoadGenerationParameters(ostream &os);
   ostream& WriteLiveLoadImpact(ostream &os);
   ostream& WriteLoadFactors(ostream &os);
   ostream& WriteAnalysisResults(ostream &os,DiagramCatagory dc);
};

#endif // RPTGEN_H