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

void FemModel::BandSolve(int mode,int neq,int band,double **KBand,double *FBand)
{
/*------------------------------------------------------------------
  symmetric band matrix equation solver
  gauss-doolittle method
  solves equations [KBand]{x}={FBand}
  Original Fortran source code by Walter Haisler of Texas A&M University
  converted to C by Richard Brice.

  KBand - symmetric banded coefficient matrix stored in compacted
			 form
  FBand - right hand side of equations being solved
  neq   - number of equations being solved
  band  - semi-bandwidth of the equations being solved
  mode = 1 triangularizes the symmetric, banded matrix KBand and
			overwrites it on to KBand (hence, KBand is destroyed and
			is replaced by its triangularized form). Note that
			FBand is not needed.
  mode = 2 obtains solution to [KBand]{x}={FBand} for a particular
			right hand side FBand (assumes that triangularized form
			of KBand is stored in KBand).
  mode = 3 preforms both forward elimination and back substitution
			at the same time.
  note----for solution of several sets of equations with same left side
		[KBand] but different right sides {FBand}, the first solution should
		be obtained with mode=3 (or mode=1 and mode=2). subsequent
		solutions with new right-hand sides requires only calling
		bansol with mode=2 (triangularized KBand and new FBand needed).
  warning--this program assumes that KBand is positive definite and
		 diagonally dominate. no pivoting or checking for zero
		 diagonal elements is preformed.
*/

	int nrs, nr, n, mr, m, i, j, k, l;
	double pivot, cp;

   PRECONDITIONX(mode >= 1 && mode <= 3,"FemModel::BandSolve() : Invalid Mode");

	nrs = neq - 1;
	nr = neq;

	/**************************************/
	/* perform triangularization of KBand */
	/**************************************/

	if (mode == 1 || mode == 3)
	{
	 for (n = 1; n <= nrs; n++)
		{
		 m = n - 1;
		 mr = min(band,nr-m);
		 pivot = KBand[n-1][0];
		 for (l = 2; l <= mr; l++)
			{
			 cp = KBand[n-1][l-1]/pivot;
			 i = m + l;
			 j = 0;
			 for (k = l;k <= mr;k++)
				 {
				 j++;
				 KBand[i-1][j-1] -= cp*KBand[n-1][k-1];
				 }
			 KBand[n-1][l-1] = cp;
			}
		}
	}

	if (mode == 2 || mode == 3)

	/*******************************/
	/* foward elimination of FBand */
	/*******************************/

	{
	 for (n = 1;n <= nrs;n++)
		{
		 m = n - 1;
		 mr = min(band,nr-m);
		 cp = FBand[n-1];
		 FBand[n-1] = cp/KBand[n-1][0];
		 for (l = 2;l <= mr;l++)
			{
			 i = m + l;
			 FBand[i-1] -= KBand[n-1][l-1]*cp;
			}
	  }

	  /********************************************/
	  /* backward substitution to obtain solution */
	  /********************************************/

	  FBand[nr-1] /= KBand[nr-1][0];
	  for (i = 1;i <= nrs;i++)
		 {
		  n = nr - i;
		  m = n - 1;
		  mr = min(band,nr-m);
		  for (k = 2;k <= mr;k++)
			 {
			 l = m + k;
			 FBand[n-1] -= KBand[n-1][k-1]*FBand[l-1];
			 }
		 }
	}
	return;

}


// ComputeBandWidth
//
// Computes the semi-bandwidth of the structure.
// The semi-band with it the maximum difference between global dof's on
// a single element + 1.
//
// This function could be improved by allowing FemModel to re-order or
// re-index the joints to produce a smaller bandwidth. A smaller bandwidth
// would reduce memory requirements and allow for faster solutions.
int FemModel::ComputeBandWidth()
{
   int bw = 1;
   int j,k;
   int dof1, dof2;

   ElementIterator i(*elements);
   while(i)
      {
      int nDOF;
      PlnFrameMbr *mbr = i++;

      nDOF = mbr->GetNumDOF();
      for (j = 0; j < nDOF; j++)
         {
         for (k = 0; k < nDOF; k++)
            {
            dof1 = mbr->GetCondensedDOF(j);
            dof2 = mbr->GetCondensedDOF(k);
            if (dof1 >= 0 && dof2 >= 0)
               bw = max(bw,abs(dof1-dof2) + 1);
            }
         }
      }

   return bw;
}

