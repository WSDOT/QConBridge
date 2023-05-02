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

#if !defined FEM2D_LOADING_H_
#define FEM2D_LOADING_H_

#include <checks.h>
#include <fem2d\femdll.h>
#include <fem2d\femall.h>
#include <fem2d\loads.h>
#include <fem2d\result.h>
#include <fem2d\member.h>
#include <fem2d\joint.h>
#include <classlib\arrays.h>

class _FEMCLASS FemModel;

class _FEMCLASS Loading
{
// Data Members
protected:
   int ID;
   JntLoadContainer *jntLoads;
   MbrLoadContainer *mbrLoads;

// Constructors/Destructor
public:
   Loading(int id);
   virtual ~Loading();

// Member Functions
public:
   int  GetID() const;
   virtual void AddJntLoad(JntLoad *load);
   virtual void AddMbrLoad(MbrLoad *load);
   int operator == (const Loading &l) const;
   virtual void Write(ostream &os) const;

protected:
   virtual void ApplyLoads(FemModel *model);

   friend ostream &operator << (ostream &os,const Loading &l);
   friend FemModel;
};

typedef TIArrayAsVector<Loading> LoadingContainer;
typedef TIArrayAsVectorIterator<Loading>  LoadingIterator;

class _FEMCLASS DeadLoading : public Loading
{
// Data Members
protected:
   GravityDir gravityDir;
   DeadLoadingType loadingType;
   double scale;

// Constructors/Destructor
public:
   DeadLoading(int id,GravityDir dir = NegY,double v = 1.0,DeadLoadingType type = dltMember);

// Member Functions
public:
   void Write(ostream &os) const;

protected:
   void ApplyLoads(FemModel *model);

   friend FemModel;
};

ostream &operator << (ostream &os,const Loading &l);

#endif // FEM2D_LOADING_H_
