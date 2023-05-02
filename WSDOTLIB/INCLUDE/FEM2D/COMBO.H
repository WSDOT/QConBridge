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

#if !defined FEM2D_COMBO_H_
#define FEM2D_COMBO_H_

#include <fem2d\femdll.h>
#include <fem2d\femall.h>
#include <fem2d\result.h>

class _FEMCLASS FemModel;

class _FEMCLASS LoadingCombo
{
// Data Members
private:
   struct ComboData
      {
      int loadId;
      double factor;
      ComboData(int id,double f) {loadId = id; factor = f;};
      bool operator == (const ComboData &c) const { return true; };
      };
   typedef TIArrayAsVector<ComboData> ComboDataContainer;
   typedef TIArrayAsVectorIterator<ComboData> ComboDataIterator;

   int     ID;
   double  Scale;
   ComboDataContainer *comboData;

// Constructors/Destructor
public:
   LoadingCombo(int id);
   ~LoadingCombo();

// Member Functions
public:
   void AddLoadingData(int ld,double factor);
   void SetScale(double s);
   double GetScale() const;

   int GetID() const;
   int GetNumLoads() const;
   int GetLoadID(int idx) const;
   double GetFactor(int idx) const;

protected:
   void CombineResults(ResultContainer *results);

// Operators
public:
   bool operator == (const LoadingCombo &combo) const;

   friend ostream &operator << (ostream &os,const LoadingCombo &c);

   friend FemModel;
};

typedef TIArrayAsVector<LoadingCombo> CombinationContainer;
typedef TIArrayAsVectorIterator<LoadingCombo>  CombinationIterator;

ostream &operator << (ostream &os,const LoadingCombo &c);

#endif // FEM2D_COMBO_H_
