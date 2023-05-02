///////////////////////////////////////////////////////////////////////
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

#include <fem2d\femmodel.h>
#include <checks.h>

#define LEVEL1 1
#define LEVEL2 2
DIAG_DECLARE_GROUP(Fem2D);

#if defined GEN_DEBUG_CODE
static ofstream logfile("fem.log");
#endif


void FemModel::StoreJntResults(int lcase)
{
   double force[3];
   double disp[3];
   JointIterator j(*joints);

   TRACEX(Fem2D,LEVEL2,"Loading " << lcase);

#if defined GEN_DEBUG_CODE
   logfile << "Loading " << lcase << endl;
#endif

   while(j)
      {
      Joint *jnt = j++;

      jnt->GetReactions(force);
      jnt->GetDisplacement(disp);

      TRACEX(Fem2D,LEVEL2,"Joint " << jnt->GetID());
      TRACEX(Fem2D,LEVEL2,"Fx = " << force[0] << " Fy = " << force[1] << " Mz = " << force[2]);
      TRACEX(Fem2D,LEVEL2,"Dx = " <<  disp[0] << " Dy = " <<  disp[1] << " Dz = " <<  disp[2]);
#if defined GEN_DEBUG_CODE
      logfile << "Joint " << jnt->GetID() << endl;
      logfile << "Fx = " << force[0] << " Fy = " << force[1] << " Mz = " << force[2] << endl;
      logfile << "Dx = " <<  disp[0] << " Dy = " <<  disp[1] << " Dz = " <<  disp[2] << endl;
#endif

      int idx;
      ResultArray* array;
      ResultArray find(jnt->GetID(),0,0,0);
      idx = jntResults->Find(&find);
      array = (idx == INT_MAX) ? 0 : (*jntResults)[idx];
      if (!array)
         {
         array = new ResultArray(jnt->GetID(),10,0,10);
         jntResults->Add(array);
         }

      Result *result = new Result(lcase,3,force,disp);
      array->Add(result);
      }
}

void FemModel::StoreMbrResults(int lcase)
{
   double force[6];
   double disp[6];

   TRACEX(Fem2D,LEVEL2,"Loading " << lcase);

#if defined GEN_DEBUG_CODE
   logfile << "Loading " << lcase << endl;
#endif

   ElementIterator i(*elements);
   while(i)
      {
      PlnFrameMbr *mbr = i++;
      mbr->GetJntForces(force);
      mbr->GetJntDisplacements(disp);

      TRACEX(Fem2D,LEVEL2,"Member " << mbr->GetID() << " Start End");
      TRACEX(Fem2D,LEVEL2,"Fx = " << force[0] << " Fy = " << force[1] << " Mz = " << force[2]);
      TRACEX(Fem2D,LEVEL2,"Dx = " <<  disp[0] << " Dy = " <<  disp[1] << " Dz = " <<  disp[2]);

      TRACEX(Fem2D,LEVEL2,"Member " << mbr->GetID() << " End End");
      TRACEX(Fem2D,LEVEL2,"Fx = " << force[3] << " Fy = " << force[4] << " Mz = " << force[5]);
      TRACEX(Fem2D,LEVEL2,"Dx = " <<  disp[3] << " Dy = " <<  disp[4] << " Dz = " <<  disp[5]);
#if defined GEN_DEBUG_CODE
      logfile << "Member " << mbr->GetID() << " Start End" << endl;
      logfile << "Fx = " << force[0] << " Fy = " << force[1] << " Mz = " << force[2] << endl;
      logfile << "Dx = " <<  disp[0] << " Dy = " <<  disp[1] << " Dz = " <<  disp[2] << endl;

      logfile << "Member " << mbr->GetID() << " End End" << endl;
      logfile << "Fx = " << force[3] << " Fy = " << force[4] << " Mz = " << force[5] << endl;
      logfile << "Dx = " <<  disp[3] << " Dy = " <<  disp[4] << " Dz = " <<  disp[5] << endl;
#endif

      int idx;
      ResultArray* array;
      ResultArray find(mbr->GetID(),0,0,0);
      idx = mbrResults->Find(&find);
      array = (idx == INT_MAX) ? 0 : (*mbrResults)[idx];
      if (!array)
         {
         array = new ResultArray(mbr->GetID(),10,0,10);
         mbrResults->Add(array);
         }

      Result *result = new Result(lcase,6,force,disp);
      array->Add(result);
      }
}

void FemModel::StorePOIResults(int lcase)
{
   TRACEX(Fem2D,LEVEL2,"Loading " << lcase);

#if defined GEN_DEBUG_CODE
   logfile << "Loading " << lcase << endl;
#endif

   POIIterator i(*pntsOfInterest);
   while(i)
      {
      POIRecord *poi = i++;

      double force[6];
      double disp[6];
      PlnFrameMbr *mbr;

      mbr = GetMember(poi->MbrID);
      mbr->GetInternalForces(poi->DistFromStart,mbrLeftFace,force);

      mbr->GetInternalForces(poi->DistFromStart,mbrRightFace,&force[3]);

      TRACEX(Fem2D,LEVEL2,"poi " << poi->ID << " Left Face");
      TRACEX(Fem2D,LEVEL2,"Fx = " << force[0] << " Fy = " << force[1] << " Mz = " << force[2]);
      TRACEX(Fem2D,LEVEL2,"poi " << poi->ID << " Right Face");
      TRACEX(Fem2D,LEVEL2,"Fx = " << force[3] << " Fy = " << force[4] << " Mz = " << force[5]);

#if defined GEN_DEBUG_CODE
      logfile << "poi " << poi->ID << " Left Face" << endl;
      logfile << "Fx = " << force[0] << " Fy = " << force[1] << " Mz = " << force[2] << endl;
      logfile << "poi " << poi->ID << " Right Face" << endl;
      logfile << "Fx = " << force[3] << " Fy = " << force[4] << " Mz = " << force[5] << endl;
#endif

      mbr->GetDeflection(poi->DistFromStart,disp);
      disp[3] = disp[0];
      disp[4] = disp[1];
      disp[5] = disp[2];

      Result *result = new Result(lcase,6,force,disp);

      int idx;
      ResultArray* array;
      ResultArray find(poi->GetID(),0,0,0);
      idx = poiResults->Find(&find);
      array = (idx == INT_MAX) ? 0 : (*poiResults)[idx];
      if (!array)
         {
         array = new ResultArray(poi->ID,10,0,10);
         poiResults->Add(array);
         }
      array->Add(result);
      }
}

void FemModel::CombineLoads(int id)
{
   CombinationIterator i(*combos);

   while (i)
      {
      LoadingCombo *combo = i++;
      if (id < 0 || id == combo->GetID())
         {
         combo->CombineResults(jntResults);
         combo->CombineResults(mbrResults);
         combo->CombineResults(poiResults);
         }
      }
}
/*
void FemModel::AddEnvelope(Envelope *envelope)
{
   envelopes->Add(envelope);
}

Envelope *FemModel::GetEnvelope(int id)
{
   return envelopes->Get(id);
}

void FemModel::RemoveEnvelope(int id)
{
   envelopes->Remove(id,TShouldDelete::Delete);
}

void FemModel::EnvelopeLoads(int id)
{
   Envelope *envelope;

   envelope = envelopes->Get(id);
   envelope->Create(jntResults,mbrResults,poiResults);
}

void FemModel::CreatePseudoLoading(int minId,int maxId,int envId)
{
   Envelope *env = envelopes->Get(envId);

   env->CreatePseudoLoading(minId,maxId,jntResults,mbrResults,poiResults);
}
*/
// GetJntDisplacements
//
// Retreives global joint displacements for a specific load case.
void FemModel::GetJntDisplacements(int jnt,int lc,double &dx,double &dy,double &rz)
{
   ResultArray *array;
   Result *jntResult;
   int idx;
   
   ResultArray findArray(jnt,0,0,0);
   idx = jntResults->Find(&findArray);
   CHECKX(idx != INT_MAX,"FemModel::GetJntDisplacements() - Joint not found");
   array = (*jntResults)[idx];

   Result findResult(lc,0,0,0);
   idx = array->Find(&findResult);
   CHECKX(idx != INT_MAX,"FemModel::GetJntDisplacements() - Load case not found");
   jntResult = (*array)[idx];

   dx = jntResult->GetDisplacement(0);
   dy = jntResult->GetDisplacement(1);
   rz = jntResult->GetDisplacement(2);
}

void FemModel::GetReactions(int jnt,int lc,double &fx,double &fy,double &mz)
{
   ResultArray *array;
   Result *jntResult;
   int idx;
   
   ResultArray findArray(jnt,0,0,0);
   idx = jntResults->Find(&findArray);
   CHECKX(idx != INT_MAX,"FemModel::GetReactions() - Joint not found");
   array = (*jntResults)[idx];

   Result findResult(lc,0,0,0);
   idx = array->Find(&findResult);
   CHECKX(idx != INT_MAX,"FemModel::GetReactions() - Load case not found");
   jntResult = (*array)[idx];

   fx = jntResult->GetForce(0);
   fy = jntResult->GetForce(1);
   mz = jntResult->GetForce(2);
}

// GetMbrForces
//
// Retreives Plane Frame member end forces in the local member coodinate
// system. This will need to be generalized as FemModel matures.
void FemModel::GetMbrForces(int mbr,int lc,double &FxStart,double &FyStart,double &MzStart,
                             double &FxEnd,double &FyEnd,double &MzEnd)
{
   ResultArray *array;
   Result *mbrResult;
   int idx;

   ResultArray findArray(mbr,0,0,0);
   idx = mbrResults->Find(&findArray);
   CHECKX(idx != INT_MAX,"FemModel::GetMbrForces() - Member not found");
   array = (*mbrResults)[idx];

   Result findResult(lc,0,0,0);
   idx = array->Find(&findResult);
   CHECKX(idx != INT_MAX,"FemModel::GetMbrForces() - Load case not found");
   mbrResult = (*array)[idx];

   FxStart = mbrResult->GetForce(0);
   FyStart = mbrResult->GetForce(1);
   MzStart = mbrResult->GetForce(2);
   FxEnd   = mbrResult->GetForce(3);
   FyEnd   = mbrResult->GetForce(4);
   MzEnd   = mbrResult->GetForce(5);
}

// GetMbrDisplacements
//
// Retreives Plane Frame member displacements in the local member coodinate
// system. This will need to be generalized as FemModel matures.
void FemModel::GetMbrDisplacements(int mbr,int lc,double &DxStart,double &DyStart,double &RzStart,
                                    double &DxEnd,double &DyEnd,double &RzEnd)
{
   ResultArray *array;
   Result *mbrResult;
   int idx;

   ResultArray findArray(mbr,0,0,0);
   idx = mbrResults->Find(&findArray);
   CHECKX(idx != INT_MAX,"FemModel::GetMbrDisplacements() - Member not found");
   array = (*mbrResults)[idx];

   Result findResult(lc,0,0,0);
   idx = array->Find(&findResult);
   CHECKX(idx != INT_MAX,"FemModel::GetMbrDisplacements() - Load case not found");
   mbrResult = (*array)[idx];

   DxStart = mbrResult->GetDisplacement(0);
   DyStart = mbrResult->GetDisplacement(1);
   RzStart = mbrResult->GetDisplacement(2);
   DxEnd   = mbrResult->GetDisplacement(3);
   DyEnd   = mbrResult->GetDisplacement(4);
   RzEnd   = mbrResult->GetDisplacement(5);
}

void FemModel::GetPOIForces(int poi,int lc,MbrFace face,double &Fx,double &Fy,double &Mz)
{
   ResultArray *array;
   Result *poiResult;
   int idx;
   int offset;

   ResultArray findArray(poi,0,0,0);
   idx = poiResults->Find(&findArray);
   CHECKX(idx != INT_MAX,"FemModel::GetPOIForces() - POI not found");
   array = (*poiResults)[idx];

   Result findResult(lc,0,0,0);
   idx = array->Find(&findResult);
   CHECKX(idx != INT_MAX,"FemModel::GetPOIForces() - Load case not found");
   poiResult = (*array)[idx];

   if (face == mbrLeftFace)
      offset = 0;
   else
      offset = 3;

   Fx = poiResult->GetForce(0+offset);
   Fy = poiResult->GetForce(1+offset);
   Mz = poiResult->GetForce(2+offset);
}

void FemModel::GetPOIDeflection(int poi,int lc,double &Dx,double &Dy,double &Rz)
{
   ResultArray *array;
   Result *poiResult;
   int idx;

   ResultArray findArray(poi,0,0,0);
   idx = poiResults->Find(&findArray);
   CHECKX(idx != INT_MAX,"FemModel::GetPOIDeflection() - POI not found");
   array = (*poiResults)[idx];

   Result findResult(lc,0,0,0);
   idx = array->Find(&findResult);
   CHECKX(idx != INT_MAX,"FemModel::GetPOIDeflection() - Load case not found");
   poiResult = (*array)[idx];

   Dx = poiResult->GetDisplacement(0);
   Dy = poiResult->GetDisplacement(1);
   Rz = poiResult->GetDisplacement(2);
}

