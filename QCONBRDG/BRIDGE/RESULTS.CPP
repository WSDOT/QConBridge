///////////////////////////////////////////////////////////////////////
// Bridge32 - Bridge Modeling Framework
// Copyright (C) 1999  Washington State Department of Transportation
//                     Bridge and Structures Office
//
// This library was developed as part of the Alternate Route Project
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the Alternate Route Library Open Source License as published by 
// the Washington State Department of Transportation, Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but is distributed 
// AS IS, WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
// or FITNESS FOR A PARTICULAR PURPOSE. See the Alternate Route Library Open Source 
// License for more details.
//
// You should have received a copy of the Alternate Route Library Open Source License 
// along with this program; if not, write to the Washington State Department of 
// Transportation, Bridge and Structures Office, 4500 3rd Ave SE - P.O. Box  47340, 
// Olympia, WA 98503, USA or e-mail Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Revision Log
// ============
// May 1997 - Created, Richard Brice, PE
// Oct 1999 - Released as Open Source

#include <bridge\lrfdbrdg.h>
#include <bridge\lrfdspan.h>
#include <fem2d\femmodel.h>
#include <code\lrfdcode.h>
#include <float.h>

DIAG_DEFINE_GROUP(LRFDBridge_Results,DISABLED,LEVEL2);

double LRFDBridge::GetResult(int spanId,int spanPnt,MbrFace face,DiagramCatagory c,DiagramType t)
{
   int poiId;

   PRECONDITIONX(spanPnt < minAnalysisPoints,"LRFDBridge::GetResult() - Invalid spanPnt");

   poiId = SpanPntToPOI(spanId,spanPnt);
   return GetResult(poiId,face,c,t);
}

double LRFDBridge::GetResult(int poiId,MbrFace face,DiagramCatagory c,DiagramType t)
{
   AnalysisResults *analysisResults;
   double offset;
   double value;
   double lfValue;
   double rfValue;

   analysisResults = GetAnalysisResults(GetAnalysisResultsId(c,t));
   analysisResults->GetResult(poiId,offset,lfValue,rfValue);

   if (face == mbrLeftFace)
      value = lfValue;
   else
      value = rfValue;

   return value;
}

double LRFDBridge::GetReaction(int pierId,DiagramCatagory c,DiagramType t)
{
	DiagramType dt;
   switch (t)
   {
   case rtFx:
   case rtFy:
   case rtMz:
   	dt = dtReaction;
      break;

   case rtFxMin:
   case rtFyMin:
   case rtMzMin:
   	dt = dtMinReaction;
      break;

   case rtFxMax:
   case rtFyMax:
   case rtMzMax:
   	dt = dtMaxReaction;
      break;
   }

	AnalysisResults* analysisResults;
   double fx,fy,mz;
   analysisResults = GetAnalysisResults(GetAnalysisResultsId(c,dt));

   analysisResults->GetSupportResult(pierId,fx,fy,mz);

   double value;
   switch(t)
   {
   case rtFx:
   case rtFxMin:
   case rtFxMax:
   	 value = fx;
       break;

   case rtFy:
   case rtFyMin:
   case rtFyMax:
   	 value = fy;
       break;

   case rtMz:
   case rtMzMin:
   case rtMzMax:
   	 value = mz;
       break;

   default:
   	CHECK(false);
   }
	return value;
}

void LRFDBridge::StoreAnalysisResults(DiagramCatagory c,const char *desc,int loadId)
{
   int momentId;
   int shearId;
   int reactionId;

   momentId = NextAvailAnalysisResults();
   SetAnalysisResultsId(c,dtMoment,momentId);

   shearId = NextAvailAnalysisResults();
   SetAnalysisResultsId(c,dtShear,shearId);

   reactionId = NextAvailAnalysisResults();
   SetAnalysisResultsId(c,dtReaction,reactionId);

   StoreAnalysisResults(shearId,momentId,reactionId,desc,loadId);
}

void LRFDBridge::StoreAnalysisResults(int shearId,int momentId,int reactionId,const char *desc,int loadId)
{
   AnalysisResults *moment,*shear, *reaction;
   double lfFx,lfFy,lfMz;
   double rfFx,rfFy,rfMz;
   double Fx,Fy,Mz; // reactions
   double offset;
   string descMoment = desc;
   string descShear  = desc;
   string descReaction = desc;

   descMoment   += " - Moment";
   descShear    += " - Shear";
   descReaction += " - Reactions";

   shear = new AnalysisResults(shearId,descShear.c_str());
   AddAnalysisResults(shear);

   moment = new AnalysisResults(momentId,descMoment.c_str());
   AddAnalysisResults(moment);

   PointOfInterestIterator j(*pntsOfInterest);
   while(j)
      {
      PointOfInterest *poi = j++;

      femModel->GetPOIForces(poi->id,loadId,mbrLeftFace,lfFx,lfFy,lfMz);
      femModel->GetPOIForces(poi->id,loadId,mbrRightFace,rfFx,rfFy,rfMz);

      offset = GetSuperstructureOffset(poi->spanId,poi->offset);

      moment->AddResult(poi->id,offset,lfMz,-rfMz);
      shear->AddResult(poi->id,offset,-lfFy,rfFy);
      }

   reaction = new AnalysisResults(reactionId,descReaction.c_str());
   AddAnalysisResults(reaction);
   Support* s;
   for ( s = firstSupport; s != NULL; s = s->NextSupport() )
   {
   	int pierId = s->GetID();
   	int jntId = s->EndJntId();
   	femModel->GetReactions(jntId,loadId,Fx,Fy,Mz);
      reaction->AddSupportResult(pierId,Fx,Fy,Mz);
   }
}

void LRFDBridge::SetAnalysisResultsId(DiagramCatagory c,DiagramType t,int id)
{
   AnalysisResultsId searchId;
   AnalysisResultsId *ID;
   int idx;

   searchId.c = c;
   searchId.t = t;
   idx = analysisResultsIds->Find(&searchId);
   if (idx == INT_MAX)
      {
      // Identifier was not found... Create a new one.
      ID = new AnalysisResultsId;
      ID->c = c;
      ID->t = t;
      ID->id = id;
      analysisResultsIds->Add(ID);
      }
   else
      {
      // Identifier was found... Change the id.
      ID = (*analysisResultsIds)[idx];
      ID->id = id;
      }
}

int LRFDBridge::GetAnalysisResultsId(DiagramCatagory c,DiagramType t)
{
   AnalysisResultsId searchId;
   AnalysisResultsId *ID;
   int idx;

   searchId.c = c;
   searchId.t = t;
   idx = analysisResultsIds->Find(&searchId);

   if (idx == INT_MAX)
      return -1;

   ID = (*analysisResultsIds)[idx];
   return ID->id;
}

void LRFDBridge::StoreAnalysisResultsAsDiagrams(DiagramCatagory dc,DiagramType dt)
{
   int id;

   TRACEX(LRFDBridge_Results,LEVEL2,"LRFDBridge::StoreAnalysisResultsAsDiagrams()");

   id = NextAvailDiagram();
   SetDiagramId(dc,dt,id);
   AddDiagram(CreateDiagram(dc,dt,id));
}


void LRFDBridge::SetDiagramId(DiagramCatagory c,DiagramType t,int id)
{
   DiagramId searchId;
   DiagramId *ID;
   int idx;

   TRACEX(LRFDBridge_Results,LEVEL2,"LRFDBridge::SetDiagramId()");

   searchId.c = c;
   searchId.t = t;
   idx = diagramIds->Find(&searchId);
   if (idx == INT_MAX)
      {
      // Identifier was not found... Create a new one.
      ID = new DiagramId;
      ID->c = c;
      ID->t = t;
      ID->id = id;
      diagramIds->Add(ID);
      }
   else
      {
      // Identifier was found... Change the id.
      ID = (*diagramIds)[idx];
      ID->id = id;
      }
}

int LRFDBridge::GetDiagramId(DiagramCatagory c,DiagramType t)
{
   DiagramId searchId;
   DiagramId *ID;
   int idx;

   searchId.c = c;
   searchId.t = t;
   idx = diagramIds->Find(&searchId);

   if (idx == INT_MAX)
      return -1;

   ID = (*diagramIds)[idx];
   return ID->id;
}


Diagram *LRFDBridge::CreateDiagram(DiagramCatagory dc,DiagramType dt,int id)
{
   Diagram *diagram;
   AnalysisResults *analysisResults;

   TRACEX(LRFDBridge_Results,LEVEL2,"LRFDBridge::CreateDiagram()");

   analysisResults = GetAnalysisResults(GetAnalysisResultsId(dc,dt));
   diagram = new Diagram(id,*analysisResults);
   return diagram;
}

