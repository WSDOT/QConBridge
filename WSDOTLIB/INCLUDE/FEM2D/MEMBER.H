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

#if !defined FEM2D_MEMBER_H_
#define FEM2D_MEMBER_H_

#include <checks.h>

#include <fem2d\femdll.h>
#include <fem2d\femall.h>
#include <shape\shape.h>
#include <fem2d\loads.h> // only include because container is template based

// Placeholder definitions
class _FEMCLASS Joint;
class _FEMCLASS FemModel;
class _FEMCLASS Loading;
class _FEMCLASS DeadLoading;
class _FEMCLASS Matrix;
class _FEMCLASS Vector;

// PlnFrameMbr
//
// Implementation of a Plane Frame beam member
//
class _FEMCLASS PlnFrameMbr
{
// Data Members
protected:
   int    ID;
   Joint** joint;
   TShape* section;
   MbrRelease relStart;
   MbrRelease relEnd;

   Matrix* Kglobal;
   Matrix* Klocal;
   Vector* Fglobal;
   Matrix* TransMatrix;
   const int TotalDOF;
   const int NumJoints;

   MbrLoadContainer *loads;
   UniformLoad *mbrDeadLoad;

   Vector *Dlocal; // local displacements
   Vector *Rlocal; // local resultant forces

// Constructors/Destructor
public:
   PlnFrameMbr(int id,Joint* start,Joint* end,TShape* sp);
   ~PlnFrameMbr();

// Member Functions
public:

   int GetID() const;
   Joint* GetJoint(int idx) const;
   void Release(MbrType type);
   void Release(MbrEnd end,MbrRelease release);
   bool IsReleased(MbrEnd end,MbrRelease release);
   TShape* GetSection() const;
   double GetLength();

protected:
   void Setup(int id,Joint *start,Joint *end);
   void Init();
   void ClearLoads();
   void AssembleF();
   int  GetJointId(Joint &j);
   void GetGlobalJntForces(int jntId,double *force);
   void ComputeResults();
   MbrType GetMemberType();
   int GetNumDOF() const;
   int GetNumJoints() const;
   int GetCondensedDOF(int dof);
   double GetKglobal(int DOFi,int DOFj);
   void GetFglobal(double *f) const;

   void ApplyLoad(MbrLoad *load);
   void GenDeadload(GravityDir dir,double scale,DeadLoadingType type);

   void ComputeDisplacements();
   void ComputeForces();

   void GetJntForces(double *force);
   void GetJntDisplacements(double *disp);

   void GetInternalForces(double loc,MbrFace face,double *force);
   void GetDeflection(double loc,double *disp);

   bool IsEquilibriumSatisfied(double tolerance);

private:
   double GetAngle();
   void BuildTransformationMatrix();
   void BuildKlocal();
   void BuildKglobal();
   void ComputeFemResults();
   void ComputeClassicResults();
   void GetPinPinRotation(double &rz1,double &rz2);
   void GetPinFixRotation(double &rz1,double &rz2);
   void GetFixPinRotation(double &rz1,double &rz2);
   void Link(Joint &jnt);
   void Unlink(Joint &jnt);

// Operators
public:
   bool operator == (const PlnFrameMbr &mbr) const;

   friend Joint;
   friend FemModel;
   friend Loading;
   friend DeadLoading;
};

typedef TIArrayAsVector<PlnFrameMbr>          ElementContainer;
typedef TIArrayAsVectorIterator<PlnFrameMbr>  ElementIterator;

#endif // FEM2D_MEMBER_H_
