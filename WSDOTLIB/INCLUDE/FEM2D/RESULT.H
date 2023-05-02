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

#if !defined FEM2D_RESULT_H_
#define FEM2D_RESULT_H_

#include <classlib\arrays.h>

// Manages results for a model component.
class Result
{
public:
   Result(int id,int ndof,double *force,double *disp);
   ~Result();

   int GetID() const;
   double GetForce(int dof) const;
   double GetDisplacement(int dof) const;
   int GetNumDOF() const;

   bool operator == (const Result &result) const;

private:
   int LoadingID;
   const int NumDOF;
   double *Force;
   double *Disp;
};

class ResultArray : public TIArrayAsVector<Result>
{
public:
   ResultArray(int id,int max,int min,int delta) : TIArrayAsVector<Result>(max,min,delta) { ID = id; };
   int GetID() const { return ID; };
   bool operator == (const ResultArray &array) const { return (ID == array.GetID()); };
private:
   int ID; // jnt, mbr, or poi id.
};

typedef TIArrayAsVector<ResultArray> ResultContainer;
typedef TIArrayAsVectorIterator<ResultArray>  ResultIterator;

#endif // FEM2D_RESULT_H_
