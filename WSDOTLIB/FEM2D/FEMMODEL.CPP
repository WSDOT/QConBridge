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

#define ENABLED true
#define DISABLED false
#define LEVEL1 1
#define LEVEL2 2
DIAG_DEFINE_GROUP(Fem2D,ENABLED,LEVEL2);

// Definitions
//
// DOF - Degree Of Freedom
// Global DOF     - DOF of a with respect to the global reference frame
// Condensed DOF  - Resulting DOF when Kglobal is condensed
// ConstrainedDOF - A DOF that has been contrained by a boundary condition

//
// FemModel
//
FemModel::FemModel()
{
   joints         = new JointContainer(10,0,10);
   elements       = new ElementContainer(20,0,10);
   loadings       = new LoadingContainer(20,0,10);
   pntsOfInterest = new POIContainer(2,0,3);
   combos         = new CombinationContainer(20,0,10);
   jntResults     = new ResultContainer(20,0,10);
   mbrResults     = new ResultContainer(20,0,10);
   poiResults     = new ResultContainer(20,0,10);
//   envelopes      = new EnvelopeContainer(20,0,10);

   joints->OwnsElements(true);
   elements->OwnsElements(true);
   loadings->OwnsElements(true);
   pntsOfInterest->OwnsElements(true);
   combos->OwnsElements(true);
   jntResults->OwnsElements(true);
   mbrResults->OwnsElements(true);
   poiResults->OwnsElements(true);
//   envelopes->OwnsElements(true);

   K = NULL;
   F = NULL;

#if defined GEN_DEBUG_CODE
   Korig = NULL;
   Forig = NULL;
#endif

   // Valid values for these parameters are >= 0
   // They are set to < 0 there so they may be asserted
   // later if invalid
   bandWidth = -1;
   numCondensedDOF = -1;
   numGlobalDOF = -1;

   gravityDir = NegY;

   SetEquilibriumCheckTolerance(1.e-5);
}

FemModel::~FemModel()
{
   delete joints;
   delete elements;
   delete loadings;
   delete pntsOfInterest;
   delete combos;
   delete jntResults;
   delete mbrResults;
   delete poiResults;
//   delete envelopes;

   FreeKGlobal();
   FreeFGlobal();
}

void FemModel::Clear()
{
   // FemModel owns the contents of these containers.
   // FemModel is the only class that has the right to destroy the
   // contents of these containers. Other associated class may have
   // containers that hold references to objects in these containers,
   // but the other classes SHALL NOT destroy theses objects.

   joints->Flush(TShouldDelete::Delete);
   elements->Flush(TShouldDelete::Delete);
   loadings->Flush(TShouldDelete::Delete);
   pntsOfInterest->Flush(TShouldDelete::Delete);
   combos->Flush(TShouldDelete::Delete);
   jntResults->Flush(TShouldDelete::Delete);
   mbrResults->Flush(TShouldDelete::Delete);
   poiResults->Flush(TShouldDelete::Delete);
//   envelopes->Flush(TShouldDelete::Delete);

   FreeKGlobal();
   FreeFGlobal();

   bandWidth = -1;
   numCondensedDOF = -1;
   numGlobalDOF = -1;

   gravityDir = NegY;
}

// SetGravityDir
//
// Sets the direction of gravity for self-weight calculations
void FemModel::SetGravityDir(GravityDir dir)
{
   gravityDir = dir;
}

// AddJoint
//
// Adds a joint to the fem model. Default status is Enabled.
void FemModel::AddJoint(Joint *jnt)
{
   PRECONDITIONX(jnt != NULL,"FemModel::AddJoint() - jnt is NULL");
   joints->Add(jnt);
}

// GetJoint
//
// Retreives a joint from the fem model for manipulation.
Joint *FemModel::GetJoint(int id)
{
   int idx;
   Joint find(id,0,0);
   idx = joints->Find(&find);
   CHECKX(idx != INT_MAX,"FemModel::GetJoint() - joint not found");
   return (idx == INT_MAX) ? 0 :(*joints)[idx];
}

// RemoveJoint
//
// Deletes a joint from the fem model
void FemModel::RemoveJoint(int id)
{
   Joint *jnt = GetJoint(id);
   if (jnt)
      joints->Destroy(jnt);
}

// AddMember
//
// Adds a Plane Frame element to the fem model. Default status is
// Enabled.
void FemModel::AddMember(PlnFrameMbr *mbr)
{
#if defined GEN_DEBUG_CODE
   int i;
   for (i = 0; i < mbr->GetNumJoints(); i++)
      {
      // GetJoint will throw an exception if the joint is not found
      GetJoint(mbr->GetJoint(i)->GetID());
      }
#endif
   elements->Add(mbr);
}

PlnFrameMbr *FemModel::GetMember(int id)
{
   PlnFrameMbr find(id,0,0,0);
   int idx = elements->Find(&find);
   CHECKX(idx != INT_MAX,"FemModel::GetMember() - member not found");
   return (idx == INT_MAX) ? 0 : (*elements)[idx];
}

void FemModel::RemoveMember(int id)
{
   PlnFrameMbr* element = GetMember(id);
   if (element)
      elements->Destroy(element);
}

//
// Independent Loadings
//
void FemModel::AddLoading(Loading *loading)
{
   loadings->Add(loading);
}

Loading *FemModel::GetLoading(int id)
{
   Loading find(id);
   int idx = loadings->Find(&find);
   CHECKX(idx != INT_MAX,"FemModel::GetLoading() - loading not found");
   return (idx == INT_MAX) ? 0 : (*loadings)[idx];
}

void FemModel::RemoveLoading(int id)
{
   Loading* loading = GetLoading(id);
   if (loading)
      loadings->Destroy(loading);
}

//
// Dependent Loadings
//
void FemModel::AddLoadingCombination(LoadingCombo *combo)
{
#if defined GEN_DEBUG_CODE
   // Cannot use the same id as a loading
   Loading *loading = GetLoading(combo->GetID());
   PRECONDITIONX(loading == NULL,"FemModel::AddLoadingCombination() ~ ID already used");
#endif
   combos->Add(combo);
}

LoadingCombo *FemModel::GetLoadingCombination(int id)
{
   LoadingCombo find(id);
   int idx = combos->Find(&find);
   CHECKX(idx != INT_MAX,"FemModel::GetLoadingCombinations() - combination not found");
   return (idx == INT_MAX) ? 0 : (*combos)[idx];
}

void FemModel::RemoveLoadingCombination(int id)
{
   LoadingCombo* combo = GetLoadingCombination(id);
   if (combo)
      combos->Destroy(combo);
}

//
//
//
void FemModel::DefinePOI(int id,int mbrId,double distFromStart,bool isFract)
{
   PlnFrameMbr *mbr;
   POIRecord *poi;

   if (isFract)
      {
      // distance from start of member is fractional
      // assert of < 0 or > 1
      mbr = GetMember(mbrId);
      distFromStart *= mbr->GetLength();
      }

   poi = new POIRecord(id,mbrId,distFromStart);
   pntsOfInterest->Add(poi);
}

// FreeKGlobal
//
// Frees the memory allocation for the global stiffness matrix
void FemModel::FreeKGlobal()
{
   if (K != NULL)
      {
      for (int i = 0; i < numCondensedDOF; i++)
         delete[] K[i];
      delete[] K;
      }

   K = NULL;

#if defined GEN_DEBUG_CODE
   if (Korig != NULL)
      {
      for (int i = 0; i < numCondensedDOF; i++)
         delete[] Korig[i];
      delete[] Korig;
      }

   Korig = NULL;
#endif
}

// FreeFGlobal
//
// Frees the memory allocation for the global force vector
void FemModel::FreeFGlobal()
{
   if (F != NULL)
      delete[] F;
   F = NULL;

#if defined GEN_DEBUG_CODE
   if (Forig != NULL)
      delete[] Forig;
   Forig = NULL;
#endif
}

// AllocateKGlobal
//
// Allocates memory for the global stiffness matrix. All entries are
// initialized to 0.0
void FemModel::AllocateKGlobal()
{
   PRECONDITIONX(numCondensedDOF >= 0,"FemModel::AllocateKGlobal() : NumCondensedDOF < 0");
   PRECONDITIONX(bandWidth       >= 0,"FemModel::AllocateKGlobal() : BandWidth < 0");

   int i,j;

   // if one is currently allocated, free that allocation before
   // proceeding
   if (K != NULL)
      FreeKGlobal();

   K = new double*[numCondensedDOF];
   for (i = 0; i < numCondensedDOF; i++)
      {
      K[i] = new double[bandWidth];

      // initialize this row
      for (j = 0; j < bandWidth; j++)
         K[i][j] = 0.0;
      }

#if defined GEN_DEBUG_CODE
   Korig = new double*[numCondensedDOF];
   for (i = 0; i < numCondensedDOF; i++)
      {
      Korig[i] = new double[bandWidth];

      // initialize this row
      for (j = 0; j < bandWidth; j++)
         Korig[i][j] = 0.0;
      }
#endif
}

// AllocateFGlobal
//
// Allocates memory for the global force vector. All entries are
// initialized to 0.0
void FemModel::AllocateFGlobal()
{
   PRECONDITIONX(numCondensedDOF >= 0,"FemModel::AllocateFGlobal() : NumCondensedDOF < 0");

   int i;

   // if one is currently allocated, free that allocation before
   // proceeding
   if (F != NULL)
      FreeFGlobal();

   F = new double[numCondensedDOF];

   // initialize the vector
   for (i = 0; i < numCondensedDOF; i++)
      F[i] = 0.0;

#if defined GEN_DEBUG_CODE
   Forig = new double[numCondensedDOF];

   // initialize the vector
   for (i = 0; i < numCondensedDOF; i++)
      Forig[i] = 0.0;
#endif
}

