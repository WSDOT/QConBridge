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

#if !defined FEM2D_MATRIX_H_
#define FEM2D_MATRIX_H_

#include <stdlib.h>
#include <iostream.h>
#include <classlib\arrays.h>

/********
This really should be part of a matrix math library
******/
#include <fem2d\femdll.h>

#define AB        0
#define ATB       1
#define ABT       2
#define ATBT      3

class _FEMCLASS Matrix
{
// Data Members
protected:
   int nRows;
   int nCols;
   double **matrix;

// Constructors/Destructor
public:
   Matrix(int nr,int nc);
   Matrix();
   virtual ~Matrix();

   void Resize(int nr,int nc);

// Member Functions
private:
   void Clear();

public:
   void   GetSize(int &nr,int &nc) const;
   void   SetElement(int r,int c,double val);
   double GetElement(int r,int c) const;
   void   Multiply(const Matrix *b,Matrix *c,int mode = AB);
   int    NumRows() const;
   int    NumColumns() const;

   void operator += (const Matrix &m);
   void operator -= (const Matrix &m);
   double operator()(int i,int j) const;
   double &operator()(int i,int j);

   friend ostream& operator << (ostream &os,const Matrix &m);
};

class Vector : public Matrix
{
public:
   Vector(int nr) : Matrix(nr,1) {};
   double operator()(int i) const;
   double &operator()(int i);
};

ostream& operator << (ostream &os,const Matrix &m);

typedef TIArrayAsVector<Matrix> MatrixContainer;

#endif // FEM2D_MATRIX_H_
