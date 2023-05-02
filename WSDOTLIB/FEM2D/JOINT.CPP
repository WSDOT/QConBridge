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

#include <fem2d\joint.h>
#include <fem2d\member.h>
#include <iomanip.h>
#include <checks.h>

//
// Joint
//
Joint::Joint(int id,double x,double y,bool fixed)
{
   ID = id;
   X = x;
   Y = y;
   Setup(fixed);
}

Joint::Joint(int id,double dx,double dy,Joint &refJnt,bool fixed)
{
   double x,y;

   ID = id;

   refJnt.GetPosition(x,y);
   X = x + dx;
   Y = y + dy;

   Setup(fixed);
}

Joint::~Joint()
{
   Unlink();
   delete attachedElements;
}

// Init
//
// Common joint initialization needed by all constructors
void Joint::Setup(bool fixed)
{
   int i;
   for (i = 0; i < 3; i++)
      {
      Constrained[i]  = fixed;
      CondensedDOF[i] = -1; // constrained dof - condensed out of Kglobal
      GlobalDOF[i]    = -1; // Assertion check value...
      }

   Reaction[0] = 0;
   Reaction[1] = 0;
   Reaction[2] = 0;

   jntLoad[0] = 0;
   jntLoad[1] = 0;
   jntLoad[2] = 0;

   attachedElements = new ElementContainer(10,0,10);
   attachedElements->OwnsElements(false); // This container does not own the members.
}

// Init
//
// Initialize joint for analysis
void Joint::Init(int nGDOF,int nCDOF,int &nGDOFused,int &nCDOFused)
{
   int count = 0;
   for (int i = 0; i < 3; i++)
      {
      GlobalDOF[i] = nGDOF + i;
      if (!Constrained[i]) // This dof is free
         {
         CondensedDOF[i] = nCDOF;
         nCDOF++;
         count++;
         }
      }

   nGDOFused = 3;
   nCDOFused = count;
}

// GetCondensedDOF
//
// Retreives the condensed dof number corrosponding to one of the
// joints local dof's. 0 = dx, 1 = dy, and 2 = rz.
int Joint::GetCondensedDOF(int dof) const
{
   // Update for 3d
   PRECONDITIONX(dof >= 0 && dof < 3,"Joint::GetCondensedDOF() : Invalid DOF");
   return CondensedDOF[dof];
}

// GetGlobalDOF
//
// Retreives the global dof number corrosponding to one of the
// joints local dof's. 0 = dx, 1 = dy, and 2 = rz.
int Joint::GetGlobalDOF(int dof) const
{
   // Update for 3d
   PRECONDITIONX(dof >= 0 && dof < 3,"Joint::GetGlobalDOF() : Invalid DOF");
   CHECKX(GlobalDOF[dof] >= 0,"Joint::GetGlobalDOF() : An error has occured with the global dof index");

   return GlobalDOF[dof];
}

// GetID
//
// Retreives the joint id
int Joint::GetID() const
{
   return ID;
}

// GetPosition
//
// Retreives the joint's position
void Joint::GetPosition(double &x,double &y) const
{
   // Update for 3d
   x = X;
   y = Y;
}

// Move
//
// Modifies a joint's position
void Joint::Move(double x,double y)
{
   X = x;
   Y = y;
}

// Support
//
// Supports, or constrains, all of the joint's dof's.
void Joint::Support()
{
   int i;
   for (i = 0; i < 3; i++) // Update for 3d
      Constrained[i] = true;
}

// Free
//
// Frees all of the joint's dof's.
void Joint::Free()
{
   int i;
   for (i = 0; i < 3; i++) // Update for 3d
      Constrained[i] = false;
}

// Release
//
// Releases a specific dof
void Joint::Release(int release)
{
   // Update for 3d
   if ((release & jntRelFx) == jntRelFx)
      Constrained[0] = false;

   if ((release & jntRelFy) == jntRelFy)
      Constrained[1] = false;

   if ((release & jntRelMz) == jntRelMz)
      Constrained[2] = false;
}

// IsReleased
//
// Returns true if a specific dof is free
bool Joint::IsReleased(int release) const
{
   bool retval;

   // Update for 3d
   if ((release & jntRelFx) == jntRelFx)
      retval = !Constrained[0];
   else if ((release & jntRelFy) == jntRelFy)
      retval = !Constrained[1];
   else if ((release & jntRelMz) == jntRelMz)
      retval = !Constrained[2];
   else
      retval = false;

   return retval;
}

// IsSupport
//
// Returns true if this joint is a support (i.e. has a constrained dof)
bool Joint::IsSupport() const
{
   bool isSupport = false;

   int i;
   for (i = 0; i < 3 && !isSupport; i++) // Update for 3d
      {
      if (Constrained[i])
         isSupport = true;
      }

   return isSupport;
}

// ClearLoads
//
// Clears any loads applied to this joint
void Joint::ClearLoads()
{
   jntLoad[0] = 0;
   jntLoad[1] = 0;
   jntLoad[2] = 0;

   Reaction[0] = 0;
   Reaction[1] = 0;
   Reaction[2] = 0;
}

// ApplyLoad
//
// Applys a load to this joint
void Joint::ApplyLoad(JntLoad *ld)
{
   double fx,fy,mz;
   ld->GetLoad(fx,fy,mz);
   jntLoad[0] += fx;
   jntLoad[1] += fy;
   jntLoad[2] += mz;
}

void Joint::GetFglobal(double *v) const
{
   v[0] = jntLoad[0];
   v[1] = jntLoad[1];
   v[2] = jntLoad[2];
}

// SetDisplacement
//
// Sets the joint displacement. To be called by TFemModel after a stiffness
// analysis.
void Joint::SetDisplacement(double *disp)
{
   Disp[0] = disp[0];
   Disp[1] = disp[1];
   Disp[2] = disp[2];
}

// GetDisplacement
//
// Retreives the joint displacements.
void Joint::GetDisplacement(double *disp) const
{
   for (int i = 0; i < GetNumDOF(); i++)
      disp[i] = Disp[i];
}

void Joint::ComputeReactions()
{
   // Reactions = unbalanced element forces + loads applied directly
   //    to this joint
   int jntId;
   double react[3];
   Reaction[0] = 0;
   Reaction[1] = 0;
   Reaction[2] = 0;

   ElementIterator e(*attachedElements);
   while(e)
      {
      PlnFrameMbr *ele = e++;

      jntId = ele->GetJointId(*this);
      ele->GetGlobalJntForces(jntId,react);
      Reaction[0] += react[0];
      Reaction[1] += react[1];
      Reaction[2] += react[2];
      }
   Reaction[0] -= jntLoad[0];
   Reaction[1] -= jntLoad[1];
   Reaction[2] -= jntLoad[2];
}

// GetReactions
//
// Retreives the joint reactions.
void Joint::GetReactions(double *react) const
{
   for (int i = 0; i < 3; i++)
      react[i] = Reaction[i] - jntLoad[i];
}

int Joint::GetNumDOF() const
{
   return 3;
}

// operator ==
//
// Equality operator, used by TIJointContainer
int Joint::operator == (const Joint &j) const
{
   return (ID == j.GetID());
}

// operator <<
//
// output stream operator
ostream &operator << (ostream &os,Joint *jnt)
{
   os << jnt->ID << " "
      << setw(8) << setprecision(4)
      << setiosflags(ios::fixed | ios::showpoint | ios::right)
      << jnt->X << setw(1) << " " << setw(8)
      << jnt->Y;

   if (jnt->IsSupport())
      os << " S";

   return os;
}

void Joint::Unlink()
{
   ElementIterator i(*attachedElements);

   // Disolve the link with all the incident members.
   while(i)
      {
      PlnFrameMbr *mbr = i++;

      int count, numJnts;
      numJnts = mbr->GetNumJoints();

      // Test memory addresses for equality
      for (count = 0; count < numJnts; count++)
         {
         if (mbr->joint[count] == this)
            {
            mbr->joint[count] = NULL;
            break;
            }
         }

      CHECKX(count != numJnts,"Joint::Unlink() - Bad link");
      }

   // empty the container
   attachedElements->Flush(TShouldDelete::NoDelete);
}


bool Joint::IsEquilibriumSatisfied(double tolerance)
{
   double Fx, Fy, Mz; // Forces from attached elements
   double fx, fy, mz; // Externally applied loads and reactions

   // External forces
   fx = Reaction[0] + jntLoad[0];
   fy = Reaction[1] + jntLoad[1];
   mz = Reaction[2] + jntLoad[2];

   // Internal forces
   Fx = 0;
   Fy = 0;
   Mz = 0;
   ElementIterator e(*attachedElements);
   while(e)
      {
      PlnFrameMbr *ele = e++;
      int jntId = ele->GetJointId(*this);
      double eleEndForce[3];
      ele->GetGlobalJntForces(jntId,eleEndForce);
      Fx += eleEndForce[0];
      Fy += eleEndForce[1];
      Mz += eleEndForce[2];
      }

   Fx -= fx;
   Fy -= fy;
   Mz -= mz;

   // Check equilibrium
   if (!((-tolerance <= Fx) && (Fx <= tolerance)))
      return false;
   if (!((-tolerance <= Fy) && (Fy <= tolerance)))
      return false;
   if (!((-tolerance <= Mz) && (Mz <= tolerance)))
      return false;

   return true;
}

#undef  EPS
#define EPS  1.0e-5
#undef   zero_val
#define  zero_val( x )     (((-EPS)<=(x)) && ((x)<=(EPS)))

