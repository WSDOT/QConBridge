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

#if !defined FEM2D_FEMMODEL_H_
#define FEM2D_FEMMODEL_H_

#include <checks.h>

#include <fem2d\femdll.h>
#include <fem2d\femall.h>
#include <material\material.h>
#include <shape\shape.h>
#include <fem2d\member.h>
#include <fem2d\joint.h>
#include <fem2d\loading.h>
#include <fem2d\loads.h>
#include <fem2d\poi.h>
#include <fem2d\result.h>
#include <fem2d\combo.h>
//#include <fem2d\envelope.h>
#include <fstream.h> // for GTStrudl


class _FEMCLASS FemModel
{
// Data Members
protected:
   JointContainer         *joints;
   ElementContainer       *elements;
   LoadingContainer       *loadings;
   POIContainer           *pntsOfInterest;
   CombinationContainer   *combos;
   ResultContainer        *jntResults;
   ResultContainer        *mbrResults;
   ResultContainer        *poiResults;
//   EnvelopeContainer      *envelopes;

   int bandWidth;
   int numGlobalDOF;
   int numCondensedDOF;
   double **K; // Global stiffness matrix
   double *F;  // Global Force Vector

#if defined GEN_DEBUG_CODE
   double **Korig;
   double *Forig;
#endif

   GravityDir gravityDir;

   double tolerance;

// Constructors/Destructor
public:
   FemModel();
  ~FemModel();

// Member Functions
public:
   // Input Functions
   void Clear();
   void SetGravityDir(GravityDir dir);

   // Joints
   void AddJoint(Joint *jnt);
   Joint *GetJoint(int id);
   void RemoveJoint(int id);

   // Members
   void AddMember(PlnFrameMbr *mbr);
   PlnFrameMbr *GetMember(int id);
   void RemoveMember(int id);

   // Independent Loadings
   void AddLoading(Loading *loading);
   Loading *GetLoading(int id);
   void RemoveLoading(int id);

   void DefinePOI(int id,int mbrId,double distFromStart,bool isFract = false);

   // Processing Functions
   void StiffnessAnalysis();

   // Loading combinations
   void AddLoadingCombination(LoadingCombo *combo);
   LoadingCombo *GetLoadingCombination(int id);
   void RemoveLoadingCombination(int id);
   void CombineLoads(int id = -1);

   // Envelopes
   /*
   void AddEnvelope(Envelope *envelope);
   Envelope *GetEnvelope(int id);
   void RemoveEnvelope(int id);
   void EnvelopeLoads(int id);
   void CreatePseudoLoading(int minId,int maxId,int envId);
   */

   // Output Functions
   void GetJntDisplacements(int jnt,int lc,double &dx,double &dy,double &rz);
   void GetReactions(int jnt,int lc,double &fx,double &fy,double &mz);
   void GetMbrForces(int mbr,int lc,double &FxStart,double &FyStart,double &MzStart,
                     double &FxEnd,double &FyEnd,double &MzEnd);
   void GetMbrDisplacements(int mbr,int lc,double &DxStart,double &DyStart,double &RzStart,
                            double &DxEnd,double &DyEnd,double &RzEnd);

   void GetPOIForces(int poi,int lc,MbrFace face,double &Fx,double &Fy,double &Mz);
   void GetPOIDeflection(int poi,int lc,double &Dx,double &Dy,double &Rz);

   void GTStrudl(const char *fname);

   // Misc Functions
   void SetEquilibriumCheckTolerance(double tol) {tolerance = tol;}
   double GetEquilibriumCheckTolerance() const {return tolerance;}

private:
   void InitModel();
   void ClearLoads();
   void FemAnalysis();
   void ClassicAnalysis();
   int  ComputeBandWidth();
   void BandSolve(int mode,int neq,int band,double **KBand,double *FBand);
   void AllocateKGlobal();
   void FreeKGlobal();
   void AllocateFGlobal();
   void FreeFGlobal();
   void AssembleGlobalStiffnessMatrix();
   void AssembleGlobalForceVector();
   void AssembleJointLoads();
   void AssembleElementLoads();
   void ApplyJntDisplacements();
   void ComputeMemberResults();
   void ComputeReactions();
   void CheckEquilibrium();
   void SolveDisplacementsClassical();

   void StoreJntResults(int lcase);
   void StoreMbrResults(int lcase);
   void StorePOIResults(int lcase);

   // GTStrudl
   void JointCoordinates(ofstream &ofile);
   void JointReleases(ofstream &ofile);
   void MemberIncidences(ofstream &ofile);
   void MemberReleases(ofstream &ofile);
   void Constants(ofstream &ofile);
   void MbrProperties(ofstream &ofile);
   void Loads(ofstream &ofile);
   void Combinations(ofstream &ofile);
   void Results(ofstream &ofile);

#if defined GEN_DEBUG_CODE
   void CheckSolution();
   double GetKij(int I,int J);
#endif

   // need ForEachElement and ForEachJoint methods
   friend Loading;
   friend DeadLoading;
};

#endif // FEM2D_FEMMODEL_H_
