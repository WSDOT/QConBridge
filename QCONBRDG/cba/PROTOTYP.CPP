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

#include <qconbrdg\prototyp.h>
#include <qconbrdg\bdoc.h>
#include <shape\genshape.h>
#include <material\genmat.h>

SingleSpanPrototype::SingleSpanPrototype(double length,Fixity leftSupport,Fixity rightSupport,TShape *s,
                                         double s_gMoment,double s_gShear,double f_gMoment,double f_gShear) :
BridgePrototype(),
length(length),
leftSupport(leftSupport),
rightSupport(rightSupport),
sect(s),
s_gMoment(s_gMoment),
s_gShear(s_gShear),
f_gMoment(f_gMoment),
f_gShear(f_gShear)
{
}

SingleSpanPrototype::~SingleSpanPrototype()
{
}

void SingleSpanPrototype::GenerateModel(TBridgeDocument *bridgeDoc)
{
   LRFDSpan *span;
   Support *support;

   // Replace default support
   support = new Support();
   support->SupportFixity(leftSupport);
   bridgeDoc->ReplaceSupport(bridgeDoc->GetBridge().GetSupport(1),support);

   // Add span and support
   TShape* sect1 = sect->Clone();
   sect1->ComputeProperties();
   span = new LRFDSpan(length,sect1);
   span->AddLLDistFactors(new LLDistFactors(0.00,s_gMoment,s_gMoment,s_gShear,f_gMoment,f_gMoment,f_gShear));
   support = new Support();
   support->SupportFixity(rightSupport);
   bridgeDoc->AddSpanSupport(span,support,true);
   bridgeDoc->SetMinAnalysisPoints(11);
}

//
//
//

MultiSpanBridgePrototype::MultiSpanBridgePrototype(MultiSpanData spanData) :
BridgePrototype(),
spanData(spanData)
{
}

MultiSpanBridgePrototype::~MultiSpanBridgePrototype()
{
}

void MultiSpanBridgePrototype::GenerateModel(TBridgeDocument *bridgeDoc)
{
   LRFDSpan *span;
   Support *support;
   int i;
   TShape *sect1,*sect2;
   LLDistFactors *df;

   // Replace the one and only support in the bridge with a roller support
   support = new Support();
   support->SupportFixity(spanData.leftSupport);
   bridgeDoc->ReplaceSupport(bridgeDoc->GetBridge().GetSupport(1),support);

   // Add first span and second support
   sect1 = new GeneralShape(spanData.Ax1,0,0,spanData.Iz1,0,0,0,0,new GeneralMaterial(spanData.ModE1,spanData.UnitWeight1));
   sect1->ComputeProperties();
   df = new LLDistFactors(0.00,spanData.s_gMoment1,spanData.s_gMoment1,spanData.s_gShear1,spanData.f_gMoment1,spanData.f_gMoment1,spanData.f_gShear1);

   span = new LRFDSpan(spanData.L1,sect1);
   span->AddLLDistFactors(df);

   if (spanData.simpleSpans)
      span->EndContinuity(ctHinged);

   if (spanData.hasColumn)
      {
      sect2 = new GeneralShape(spanData.AxCol,0,0,spanData.IzCol,0,0,0,0,new GeneralMaterial(spanData.ModECol,spanData.ColUnitWeight));
      sect2->ComputeProperties();
      support = new Support(spanData.hCol,0.00,sect2);
      support->TopContinuity( (spanData.isHinged && !spanData.simpleSpans) ? ctHinged : ctContinuous);
      support->SupportFixity(spanData.interiorSupport);
      }
   else
      {
      support = new Support();
      support->SupportFixity(spanData.interiorSupport);
      }
   bridgeDoc->AddSpanSupport(span,support,true);

   // Add all the interior spans and supports
   for (i = 1; i < spanData.numSpans-1; i++)
      {
      sect1 = new GeneralShape(spanData.Ax2,0,0,spanData.Iz2,0,0,0,0,new GeneralMaterial(spanData.ModE2,spanData.UnitWeight2));
      sect1->ComputeProperties();
      df = new LLDistFactors(0.00,spanData.s_gMoment2,spanData.s_gMoment2,spanData.s_gShear2,spanData.f_gMoment2,spanData.f_gMoment2,spanData.f_gShear2);
      span = new LRFDSpan(spanData.L2,sect1);
      span->AddLLDistFactors(df);
      if (spanData.simpleSpans)
         {
         span->StartContinuity(ctHinged);
         span->EndContinuity(ctHinged);
         }

      if (spanData.hasColumn)
         {
         sect2 = new GeneralShape(spanData.AxCol,0,0,spanData.IzCol,0,0,0,0,new GeneralMaterial(spanData.ModECol,spanData.ColUnitWeight));
         sect2->ComputeProperties();
         support = new Support(spanData.hCol,0.00,sect2);
         support->TopContinuity( (spanData.isHinged && !spanData.simpleSpans) ? ctHinged : ctContinuous);
         support->SupportFixity(spanData.interiorSupport);
         }
      else
         {
         support = new Support();
         support->SupportFixity(spanData.interiorSupport);
         }
      bridgeDoc->AddSpanSupport(span,support,true);
      }

   // Add last span and support
   sect1 = new GeneralShape(spanData.Ax1,0,0,spanData.Iz1,0,0,0,0,new GeneralMaterial(spanData.ModE1,spanData.UnitWeight1));
   sect1->ComputeProperties();
   df = new LLDistFactors(0.00,spanData.s_gMoment1,spanData.s_gMoment1,spanData.s_gShear1,spanData.f_gMoment1,spanData.f_gMoment1,spanData.f_gShear1);
   span = new LRFDSpan((spanData.numSpans == 2) ? spanData.L2 : spanData.L1,sect1);
   span->AddLLDistFactors(df);
   if (spanData.simpleSpans)
      span->StartContinuity(ctHinged);
   support = new Support();
   support->SupportFixity(spanData.rightSupport);
   bridgeDoc->AddSpanSupport(span,support,true);
   bridgeDoc->SetMinAnalysisPoints(11);
}

