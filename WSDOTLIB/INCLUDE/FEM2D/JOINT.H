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

#if !defined FEM2D_JOINT_H_
#define FEM2D_JOINT_H_

#include <checks.h>

#include <fem2d\femdll.h>
#include <fem2d\femall.h>
#include <fem2d\loads.h>
#include <fem2d\member.h>
#include <classlib\arrays.h>

class _FEMCLASS FemModel;
class _FEMCLASS Loading;

// Joint
//
// Representation of the most basic geometrical property of a fem model.
// It is not envisioned that any classes will be derived from this, except
// for possibly a graphical version.
class _FEMCLASS Joint
{
// Data Members
private:
   int   ID;                       // unique identifier
   double X,Y;                     // initial position
   bool Constrained[3];            // DOF constrait states
   int  CondensedDOF[3];           // Condensed DOF numbers
   int  GlobalDOF[3];              // Global DOF numbers
   double jntLoad[3];              // Joint loads
   double Disp[3];                 // displacements
   double Reaction[3];             // reactions
   ElementContainer *attachedElements;  // reference to all elements attached to this
                                        // joint

// Constructors/Destructor
public:
   Joint(int id,double x,double y,bool fixed = false);
   Joint(int id,double dx,double dy,Joint &refJnt,bool fixed = false);
  ~Joint();

// Member Functions
public:
   int  GetID() const;
   void GetPosition(double &x,double &y) const;
   void Move(double x,double y);
   void Support();
   void Free();
   void Release(int release);
   bool IsSupport() const;
   bool IsReleased(int release) const;


   int operator == (const Joint &j) const;
   friend ostream &operator << (ostream &os,Joint *jnt);

protected:
   void Setup(bool fixed);
   void Init(int nGDOF,int nCDOF,int &nGDOFused,int &nCDOFused);
   int  GetNumDOF() const;
   int  GetGlobalDOF(int dof) const;
   int  GetCondensedDOF(int dof) const;

   void ClearLoads();
   void ApplyLoad(JntLoad *ld);
   void GetFglobal(double *v) const;

   void SetDisplacement(double *disp);
   void GetDisplacement(double *disp) const;

   void ComputeReactions();
   void GetReactions(double *react) const;

   bool IsEquilibriumSatisfied(double tolerance);

   void Unlink();

   friend FemModel;
   friend PlnFrameMbr;
   friend Loading;
};

ostream &operator << (ostream &os,Joint *jnt);

typedef TIArrayAsVector<Joint> JointContainer;
typedef TIArrayAsVectorIterator<Joint>  JointIterator;

#endif // FEM2D_JOINT_H_
