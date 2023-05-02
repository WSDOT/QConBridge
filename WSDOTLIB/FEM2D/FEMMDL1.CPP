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

// StiffnessAnalysis
//
// Performs a stiffness analysis.
void FemModel::StiffnessAnalysis()
{
   InitModel();

   if (numCondensedDOF > 0)
      FemAnalysis();
   else
      ClassicAnalysis();
}

void FemModel::FemAnalysis()
{
   bandWidth = ComputeBandWidth();

   AssembleGlobalStiffnessMatrix();

   // Triangularize Global Stiffness Matrix
   BandSolve(1,numCondensedDOF,bandWidth,K,NULL);

   LoadingIterator ldIter(*loadings);

   while(ldIter)
      {
      ClearLoads(); // clear any previously applied loadings

      Loading *loading = ldIter++;
      loading->ApplyLoads(this);

      AssembleGlobalForceVector();
      BandSolve(2,numCondensedDOF,bandWidth,K,F);

#if defined GEN_DEBUG_CODE
      CheckSolution();
#endif

      ApplyJntDisplacements();
      ComputeMemberResults();
      ComputeReactions();
      CheckEquilibrium();
      StoreJntResults(loading->GetID());
      StoreMbrResults(loading->GetID());
      StorePOIResults(loading->GetID());
      }
}

void FemModel::ClassicAnalysis()
{
   LoadingIterator ldIter(*loadings);

   while(ldIter)
      {
      ClearLoads(); // clear any previously applied loadings

      Loading *loading = ldIter++;
      loading->ApplyLoads(this);

      SolveDisplacementsClassical();
      ComputeMemberResults();
      ComputeReactions();
      CheckEquilibrium();
      StoreJntResults(loading->GetID());
      StoreMbrResults(loading->GetID());
      StorePOIResults(loading->GetID());
      }
}

// AssembleGlobalForceVector
//
// Assembles the global force vector.
void FemModel::AssembleGlobalForceVector()
{
   FreeFGlobal();
   AllocateFGlobal();

   AssembleJointLoads();
   AssembleElementLoads();
}

// AssembleJointLoads
//
// Assemble joint loads into the global force vector
void FemModel::AssembleJointLoads()
{
   double *f;
   int ndof;
   JointIterator j(*joints);

   while(j)
      {
      Joint *jnt = j++;
      ndof = jnt->GetNumDOF();

      f = new double[ndof];

      jnt->GetFglobal(f);

      for (int i = 0; i < ndof; i++)
         {
         int dof;
         dof = jnt->GetCondensedDOF(i);
         if (dof >= 0)
            F[dof] += f[i];
         }

      delete [] f;
      }
}

// AssembleElementLoads
//
// Assembles element loads into the global force vector.
void FemModel::AssembleElementLoads()
{
   ElementIterator e(*elements);

   while(e)
      {
      PlnFrameMbr *mbr = e++;

      // Each element has to assemble its force vector in global coordinates
      // before it can be integrated into the global force vector.
      mbr->AssembleF();

      int dof, cdof,ndof;
      double *f;
      ndof = mbr->GetNumDOF();
      f = new double[ndof];
      mbr->GetFglobal(f);
      for (dof = 0; dof < ndof; dof++)
         {
         cdof = mbr->GetCondensedDOF(dof);
         if (cdof >= 0)
            {
            F[cdof] += f[dof];
#if defined GEN_DEBUG_CODE
            Forig[cdof] += f[dof];
#endif
            }
         }
      delete[] f;
      }
}

// AssembleGlobalStiffnessMatrix
//
// Assembles the global stiffness matrix by adding the stiffness
// contribution of each enabled element in the model.
void FemModel::AssembleGlobalStiffnessMatrix()
{
   FreeKGlobal();
   AllocateKGlobal();

   ElementIterator iter(*elements);

   while(iter)
      {
      int i,j;
      int dof;
      PlnFrameMbr *mbr = iter++;

      dof = mbr->GetNumDOF();

      for (i = 0; i < dof; i++)
         {
         for (j = 0; j < dof; j++)
            {
            int CDOFi,CDOFj;
            int I,J;
            double k;

            CDOFi = mbr->GetCondensedDOF(i);
            CDOFj = mbr->GetCondensedDOF(j);

            if (CDOFi >= 0 && CDOFj >= 0)
               {
               I = CDOFi;
               J = CDOFj - CDOFi;

               if (J >= 0)
                  {
                  k = mbr->GetKglobal(i,j);
                  K[I][J] += k;
#if defined GEN_DEBUG_CODE
                  Korig[I][J] += k;
#endif
                  }
               }
            }  // End j loop
         } // End i loop
      } // End of while
}

void FemModel::InitModel()
{
   // Initialize joints by assigning global and condensed dof numbers
   numCondensedDOF = 0;
   numGlobalDOF    = 0;

   JointIterator j(*joints);
   while(j)
      {
      int nGDOFused,nCDOFused;
      Joint *jnt = j++;
      jnt->Init(numGlobalDOF,numCondensedDOF,nGDOFused,nCDOFused);
      numGlobalDOF    += nGDOFused;
      numCondensedDOF += nCDOFused;
      }

   // Allow all the elements a chance to initialize themselves
   ElementIterator m(*elements);
   while(m)
      {
      PlnFrameMbr *mbr = m++;

      mbr->Init();
      }
}

void FemModel::ClearLoads()
{
   JointIterator i(*joints);
   while(i)
      {
      Joint *jnt = i++;
      jnt->ClearLoads();
      }

   ElementIterator j(*elements);
   while(j)
      {
      PlnFrameMbr *mbr = j++;
      mbr->ClearLoads();
      }
}

// ApplyJntDisplacements
//
// Applies the global joint displacements to the structure
void FemModel::ApplyJntDisplacements()
{
   JointIterator j(*joints);

   while(j)
      {
      int cdof, dof, ndof;
      double *disp;
      Joint *jnt = j++;

      ndof = jnt->GetNumDOF();
      disp = new double[ndof];
      for (dof = 0; dof < ndof; dof++)
         {
         cdof = jnt->GetCondensedDOF(dof);
         disp[dof] = (cdof < 0) ? 0.0 : F[cdof];
         }

      // Apply the displacement to the joint for use in further
      // results processing.
      //
      // This implies a shorting coming in TJoint
      jnt->SetDisplacement(disp);

      delete[] disp;
      }
}

void FemModel::ComputeMemberResults()
{
   ElementIterator i(*elements);
   while(i)
      {
      PlnFrameMbr *mbr = i++;
      mbr->ComputeResults();
      }
}

void FemModel::ComputeReactions()
{
   JointIterator j(*joints);

   while (j)
      {
      Joint *jnt = j++;
      jnt->ComputeReactions();
      }
}

// SolveDisplacementsClassical
//
// Solves global displacements using classical Roark beam formulas.
void FemModel::SolveDisplacementsClassical()
{
   // Since all global dof's are constrained, all global displacements
   // are zero. This function simply assigns 0.0 to all joint displacements.
   // Individual elements shall modify member end displacements based upon
   // member end boundary conditions

   JointIterator j(*joints);

   while(j)
      {
      double disp[3];
      Joint *jnt = j++;
      disp[0] = 0;
      disp[1] = 0;
      disp[2] = 0;
      jnt->SetDisplacement(disp);
      }
}

void FemModel::CheckEquilibrium()
{
   char buffer[256];
   ostrstream os(buffer,sizeof(buffer));

   JointIterator j(*joints);
   while(j)
      {
      Joint *jnt = j++;
      if (!jnt->IsEquilibriumSatisfied(GetEquilibriumCheckTolerance()))
         {
         os << "Equilibrium Not Satisfied for joint " << jnt->GetID() << ends;
         throw(xmsg(buffer));
         }
      }

   ElementIterator i(*elements);
   while(i)
      {
      PlnFrameMbr *ele = i++;
      if (!ele->IsEquilibriumSatisfied(GetEquilibriumCheckTolerance()))
         {
         os << "Equilibrium Not Satisfied for Element " << ele->GetID() << ends;
         throw(xmsg(buffer));
         }
      }
}

#if defined GEN_DEBUG_CODE
void FemModel::CheckSolution()
{
   double  *Error;
   Error = new double[numCondensedDOF];
   int l,m;

   for (l = 0; l < numCondensedDOF; l++)
      {
      Error[l] = 0.;
      for (m = 0; m < numCondensedDOF; m++)
         {
         Error[l] += GetKij(l,m) * F[m];
         }
      Error[l] -= Forig[l];

      TRACEX(Fem2D,LEVEL2,"Error[" << l << "] = " << Error[l]);
      CHECKX(zero_val(Error[l]),"Error solving global stiffness equations");
      }

   delete[] Error;
}

double FemModel::GetKij(int I,int J)
{
   int temp;
   int i,j;

   if (J < I)
      {
      // Requesting entry below main diagonal
      // Transposed entry is the same
      temp = I;
      I = J;
      J = temp;
      }

   i = I;
   j = J - I;

   if (j >= bandWidth)
      return 0.;

   return Korig[i][j];
}

#endif

