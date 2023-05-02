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

#include <fem2d\result.h>

//
//
//
Result::Result(int id,int ndof,double *force,double *disp) :
NumDOF(ndof)
{
   int i;

   LoadingID = id;
   Force = new double[NumDOF];
   Disp = new double[NumDOF];

   for (i = 0; i < NumDOF; i++)
      {
      Force[i] = force[i];
      Disp[i] = disp[i];
      }
}

Result::~Result()
{
   delete[] Force;
   delete[] Disp;
}

int Result::GetID() const
{
   return LoadingID;
}

double Result::GetForce(int dof) const
{
   return Force[dof];
}

double Result::GetDisplacement(int dof) const
{
   return Disp[dof];
}

int Result::GetNumDOF() const
{
   return NumDOF;
}

bool Result::operator == (const Result &result) const
{
   return (LoadingID == result.GetID());
}

