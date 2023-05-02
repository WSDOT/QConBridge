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

#include <fem2d\matrix.h>
#include <iomanip.h>

Matrix::Matrix(int nr,int nc)
{
   nRows = 0;
   nCols = 0;
   matrix = NULL;
   Resize(nr,nc);
}

Matrix::Matrix()
{
   nRows = 0;
   nCols = 0;
   matrix = NULL;
}

Matrix::~Matrix()
{
   Clear();
}

void Matrix::Resize(int nr,int nc)
{
   int i;

   Clear();
   nRows = nr;
   nCols = nc;
   matrix = new double*[nRows];
   for (i = 0; i < nRows; i++)
      matrix[i] = new double[nCols];
}

void Matrix::Clear()
{
   int i;

   if (matrix)
      {
      for (i = 0; i < nRows; i++)
         delete[] matrix[i];

      delete[] matrix;
      }
}

void Matrix::GetSize(int &nr,int &nc) const
{
   nr = nRows;
   nc = nCols;
}

void Matrix::SetElement(int r,int c,double val)
{
   (*this)(r,c) = val;
}

double Matrix::GetElement(int r,int c) const
{
   return (*this)(r,c);
}

int Matrix::NumRows() const
{
   return nRows;
}

int Matrix::NumColumns() const
{
   return nCols;
}

void Matrix::Multiply(const Matrix *m,Matrix *r,int mode)
{
   int i,j,k;
   double v;
   int left, middle, right;

   // left and right are the dimension of r.
   // middle is the common dimension for both matrices.

   // Set up loop controls
   if ( (mode == AB) || (mode == ABT) )
      {
      // A (this) is not transposed
      left = nRows;
      middle = nCols;
      }
   else
      {
      // A (this) is transposed
      left = nCols;
      middle = nRows;
      }

   if ( (mode == AB) || (mode == ATB) )
      {
      // B is not transposed
      right = m->NumColumns();
      }
   else
      {
      // B is transposed
      right = m->NumRows();
      }

   for (i = 0; i < left; i++)
      {
      for (j = 0; j < right; j++)
         {
         v = 0;
         for (k = 0; k < middle; k++)
            {
            // Get matrix indices... Remember the matrix is not really
            // transposed. But the loop counters are setup as if it is.
            int r1,c1,r2,c2;
            if ( (mode == AB) || (mode == ABT) )
               {
               r1 = i;
               c1 = k;
               }
            else
               {
               r1 = k;
               c1 = i;
               }

            if ( (mode == AB) || (mode == ATB) )
               {
               r2 = k;
               c2 = j;
               }
            else
               {
               r2 = j;
               c2 = k;
               }

            v += matrix[r1][c1] * (*m)(r2,c2);
            }
         // Assign v to r
         (*r)(i,j) = v;
         }
      }
}

void Matrix::operator += (const Matrix &m)
{
   int i,j;

   for (i = 0; i < nRows; i++)
      {
      for (j = 0; j < nCols; j++)
         {
         matrix[i][j] += m(i,j);
         }
      }
}

void Matrix::operator -= (const Matrix &m)
{
   int i,j;

   for (i = 0; i < nRows; i++)
      {
      for (j = 0; j < nCols; j++)
         {
         matrix[i][j] -= m(i,j);
         }
      }
}

double Matrix::operator()(int i,int j) const
{
   return matrix[i][j];
}

double &Matrix::operator() (int i,int j)
{
   return matrix[i][j];
}

ostream& operator << (ostream &os,const Matrix &m)
{
   int i,j;

   for (i = 0; i < m.nRows; i++)
      {
      for (j = 0; j < m.nCols; j++)
         {
         os << setiosflags(ios::showpoint | ios::scientific) << setw(10) << setprecision(3) << m.matrix[i][j] << " ";
         }
      os << endl;
      }
   os << endl;

   return os;
}


double Vector::operator()(int i) const
{
   return matrix[i][0];
}

double &Vector::operator() (int i)
{
   return matrix[i][0];
}

