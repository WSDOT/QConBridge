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

#if !defined (PROTOTYPE_H_)
#define PROTOTYPE_H_

#include <bridge\lrfdspan.h>

class TBridgeDocument;

class BridgePrototype
{
public:
   BridgePrototype() {}
   virtual ~BridgePrototype() {}
   virtual void GenerateModel(TBridgeDocument *bridgeDoc) = 0;
};

class SingleSpanPrototype : public BridgePrototype
{
public:
   SingleSpanPrototype(double length,Fixity leftSupport,Fixity rightSupport,TShape *s,
                       double s_gMoment,double s_gShear,double f_gMoment,double f_gShear);
   ~SingleSpanPrototype();
   void GenerateModel(TBridgeDocument *bridgeDoc);

protected:
   double length;
   Fixity leftSupport;
   Fixity rightSupport;
   TShape *sect;
   double s_gMoment;
   double s_gShear;
   double f_gMoment;
   double f_gShear;
};

struct MultiSpanData
{
   int numSpans;
   double L1;
   double L2;
   bool simpleSpans;
   bool hasColumn;
   double hCol;
   bool isHinged;
   Fixity leftSupport;
   Fixity interiorSupport;
   Fixity rightSupport;
   double Ax1;
   double Iz1;
   double Ax2;
   double Iz2;
   double AxCol;
   double IzCol;
   double ModE1;
   double UnitWeight1;
   double ModE2;
   double UnitWeight2;
   double ModECol;
   double ColUnitWeight;
   double s_gMoment1;
   double s_gShear1;
   double f_gMoment1;
   double f_gShear1;
   double s_gMoment2;
   double s_gShear2;
   double f_gMoment2;
   double f_gShear2;
};

class MultiSpanBridgePrototype : public BridgePrototype
{
public:
   MultiSpanBridgePrototype(MultiSpanData spanData);
   ~MultiSpanBridgePrototype();
   void GenerateModel(TBridgeDocument *bridgeDoc);

protected:
   MultiSpanData spanData;
};

#endif // PROTOTYPE_H_
