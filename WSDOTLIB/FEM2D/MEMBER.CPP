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

#include <fem2d\member.h>
#include <fem2d\joint.h>
#include <fem2d\beam.h>
#include <fem2d\matrix.h>
#include <material\material.h>

#include <math.h>

#if defined DUMP_KMATRIX
#include <iostream.h>
#include <fstream.h>
static ofstream kmatFile("kmatrix.log");
#endif

PlnFrameMbr::PlnFrameMbr(int id,Joint* start,Joint* end,TShape* sp) :
NumJoints(2),
TotalDOF(6)
{
   section = sp;

   Setup(id,start,end);
}

PlnFrameMbr::~PlnFrameMbr()
{
   Unlink(*(joint[0]));
   Unlink(*(joint[1]));

   delete[] joint;

   ClearLoads();
   delete loads;

   delete TransMatrix;
   delete Kglobal;
   delete Klocal;
   delete Fglobal;
   delete Dlocal;
   delete Rlocal;

   // The member does not own it's associated section
   section = NULL;
}

void PlnFrameMbr::Setup(int id,Joint *start,Joint *end)
{
   ID = id;

   // vector of 2 joints... abstract to n joints later
   joint = new Joint*[NumJoints];

   joint[0] = start;
   if (start)
      Link(*(joint[0]));

   joint[1] = end;
   if (end)
      Link(*(joint[1]));

   relStart = mbrRelNone;
   relEnd   = mbrRelNone;

   loads = new MbrLoadContainer(10,0,10);
   loads->OwnsElements(false); // does not own the loads that are applied, just keeps a reference to them

   TransMatrix = new Matrix(TotalDOF,TotalDOF);
   Kglobal     = new Matrix(TotalDOF,TotalDOF);
   Klocal      = new Matrix(TotalDOF,TotalDOF);
   Fglobal     = new Vector(TotalDOF);
   Dlocal      = new Vector(TotalDOF);
   Rlocal      = new Vector(TotalDOF);

   mbrDeadLoad = NULL;
}

int PlnFrameMbr::GetID() const
{
   return ID;
}

Joint *PlnFrameMbr::GetJoint(int idx) const
{
   return joint[idx];
}

double PlnFrameMbr::GetLength()
{
   // Assumes a 2d member...
   // Update for 3d
   double x1,y1,x2,y2;
   Joint *StartJnt, *EndJnt;
   StartJnt = joint[0];
   EndJnt   = joint[1];

   StartJnt->GetPosition(x1,y1);
   EndJnt->GetPosition(x2,y2);

   return sqrt(pow(x2-x1,2) + pow(y2-y1,2));
}

double PlnFrameMbr::GetAngle()
{
   // Update for 3d
   double x1,y1,x2,y2;
   Joint *StartJnt, *EndJnt;
   StartJnt = joint[0];
   EndJnt   = joint[1];

   StartJnt->GetPosition(x1,y1);
   EndJnt->GetPosition(x2,y2);

   return atan2(y2-y1,x2-x1);
}

void PlnFrameMbr::Init()
{
   BuildTransformationMatrix();
   BuildKlocal();
   BuildKglobal();

#if defined DUMP_KMATRIX
   kmatFile << "Member " << GetID() << endl;
   kmatFile << "Transformation Matrix" << endl;
   kmatFile << (*TransMatrix) << endl;
   kmatFile << "Local K Matrix" << endl;
   kmatFile << (*Klocal) << endl;
   kmatFile << "Global K Matrix" << endl;
   kmatFile << (*Kglobal) << endl;
   kmatFile << endl;
#endif
}

void PlnFrameMbr::BuildTransformationMatrix()
{
   // Update for 3d
   double s;
   double c;
   double Angle;

   // 2d Transformation matrix
   // [ c  s  0  0  0  0]
   // [-s  c  0  0  0  0]
   // [ 0  0  1  0  0  0]
   // [ 0  0  0  c  s  0]
   // [ 0  0  0 -s  c  0]
   // [ 0  0  0  0  0  1]

   Angle = GetAngle();
   s = sin(Angle);
   c = cos(Angle);

   (*TransMatrix)(0,0) = c;
   (*TransMatrix)(0,1) = s;
   (*TransMatrix)(0,2) = 0;
   (*TransMatrix)(0,3) = 0;
   (*TransMatrix)(0,4) = 0;
   (*TransMatrix)(0,5) = 0;

   (*TransMatrix)(1,0) = -s;
   (*TransMatrix)(1,1) = c;
   (*TransMatrix)(1,2) = 0;
   (*TransMatrix)(1,3) = 0;
   (*TransMatrix)(1,4) = 0;
   (*TransMatrix)(1,5) = 0;

   (*TransMatrix)(2,0) = 0;
   (*TransMatrix)(2,1) = 0;
   (*TransMatrix)(2,2) = 1;
   (*TransMatrix)(2,3) = 0;
   (*TransMatrix)(2,4) = 0;
   (*TransMatrix)(2,5) = 0;

   (*TransMatrix)(3,0) = 0;
   (*TransMatrix)(3,1) = 0;
   (*TransMatrix)(3,2) = 0;
   (*TransMatrix)(3,3) = c;
   (*TransMatrix)(3,4) = s;
   (*TransMatrix)(3,5) = 0;

   (*TransMatrix)(4,0) = 0;
   (*TransMatrix)(4,1) = 0;
   (*TransMatrix)(4,2) = 0;
   (*TransMatrix)(4,3) = -s;
   (*TransMatrix)(4,4) = c;
   (*TransMatrix)(4,5) = 0;

   (*TransMatrix)(5,0) = 0;
   (*TransMatrix)(5,1) = 0;
   (*TransMatrix)(5,2) = 0;
   (*TransMatrix)(5,3) = 0;
   (*TransMatrix)(5,4) = 0;
   (*TransMatrix)(5,5) = 1;
}

void PlnFrameMbr::BuildKlocal()
{
   double Ax, Iz ,E;
   double Length;
   double A,D,G,H,J,K,L,N,O,P,S,T,U;
   double vi,vj,v;

   section->ComputeProperties();
   Ax = section->GetArea();
   Iz = section->GetCentroidalInertia().Ixx;
   E  = section->GetModE();
   Length = GetLength();

   vi = (relStart & mbrRelMz) ? 0 : 1;
   vj = (relEnd   & mbrRelMz) ? 0 : 1;
   v  = 4 - vi*vj;

   /* compute global stiffness matrix */

   A =  Ax*E/Length;
   D = -Ax*E/Length;
   G =  (12*E*Iz/pow(Length,3)) * (vi + vi*vj + vj)/v;
   H =   (6*E*Iz/pow(Length,2)) * (2*vi + vi*vj)/v;
   J = -(12*E*Iz/pow(Length,3)) * (vi + vi*vj + vj)/v;
   K =   (6*E*Iz/pow(Length,2)) * (2*vj + vi*vj)/v;
   L =   (4*E*Iz/Length)        * (3*vi)/v;
   N =  -(6*E*Iz/pow(Length,2)) * (2*vi + vi*vj)/v;
   O =   (2*E*Iz/Length)        * (3*vi*vj)/v;
   P =  Ax*E/Length;
   S =  (12*E*Iz/pow(Length,3)) * (vi + vi*vj + vj)/v;
   T =  -(6*E*Iz/pow(Length,2)) * (2*vj + vi*vj)/v;
   U =   (4*E*Iz/Length)        * (3*vj)/v;

   // K local

   (*Klocal)(0,0) = A;
   (*Klocal)(0,1) = 0;
   (*Klocal)(0,2) = 0;
   (*Klocal)(0,3) = D;
   (*Klocal)(0,4) = 0;
   (*Klocal)(0,5) = 0;

   (*Klocal)(1,0) = 0;
   (*Klocal)(1,1) = G;
   (*Klocal)(1,2) = H;
   (*Klocal)(1,3) = 0;
   (*Klocal)(1,4) = J;
   (*Klocal)(1,5) = K;

   (*Klocal)(2,0) = 0;
   (*Klocal)(2,1) = H;
   (*Klocal)(2,2) = L;
   (*Klocal)(2,3) = 0;
   (*Klocal)(2,4) = N;
   (*Klocal)(2,5) = O;

   (*Klocal)(3,0) = D;
   (*Klocal)(3,1) = 0;
   (*Klocal)(3,2) = 0;
   (*Klocal)(3,3) = P;
   (*Klocal)(3,4) = 0;
   (*Klocal)(3,5) = 0;

   (*Klocal)(4,0) = 0;
   (*Klocal)(4,1) = J;
   (*Klocal)(4,2) = N;
   (*Klocal)(4,3) = 0;
   (*Klocal)(4,4) = S;
   (*Klocal)(4,5) = T;

   (*Klocal)(5,0) = 0;
   (*Klocal)(5,1) = K;
   (*Klocal)(5,2) = O;
   (*Klocal)(5,3) = 0;
   (*Klocal)(5,4) = T;
   (*Klocal)(5,5) = U;
}

void PlnFrameMbr::BuildKglobal()
{
   Matrix Temp(TotalDOF,TotalDOF);

   TransMatrix->Multiply(Klocal,&Temp,ATB);
   Temp.Multiply(TransMatrix,Kglobal);
}

// ClearLoads
//
// Clears any loads applied to this member
void PlnFrameMbr::ClearLoads()
{
   // This object does not own it's loads... Do not delete them.
   // Simply empty the container.
   loads->Flush(TShouldDelete::NoDelete);

   // Owns member deadload load, so it must be deleted
   if (mbrDeadLoad)
      {
      delete mbrDeadLoad;
      mbrDeadLoad = NULL;
      }
}

void PlnFrameMbr::ComputeResults()
{
   bool useClassic = true;
   int ndof;
   int dof;
   Joint *StartJnt, *EndJnt;
   StartJnt = joint[0];
   EndJnt   = joint[1];

   ndof = StartJnt->GetNumDOF();

   for (dof = 0; dof < ndof && useClassic; dof++)
      {
      if (StartJnt->GetCondensedDOF(dof) >= 0)
         useClassic = false;
      }

   ndof = EndJnt->GetNumDOF();

   for (dof = 0; dof < ndof && useClassic; dof++)
      {
      if (EndJnt->GetCondensedDOF(dof) >= 0)
         useClassic = false;
      }

   if (useClassic)
      ComputeClassicResults();
   else
      ComputeFemResults();
}

void PlnFrameMbr::ComputeFemResults()
{
   // compute mbr end forces (local)
   ComputeForces();

   // compute mbr end displacements (local)
   ComputeDisplacements();
}

void PlnFrameMbr::ComputeClassicResults()
{
   double dx1,dy1,rz1; // start displacements
   double dx2,dy2,rz2; // end displacements
   double Fx1,Fy1,Mz1; // start forces
   double Fx2,Fy2,Mz2; // end forces
   double *force;
   double *disp;

   dx1=dy1=rz1 = 0;
   dx2=dy2=rz2 = 0;
   Fx1=Fy1=Mz1 = 0;
   Fx2=Fy2=Mz2 = 0;

   force = new double[TotalDOF];
   disp  = new double[TotalDOF];

   // for every load in the current loadcase
   // compute member end displacements, rotations, and forces
   MbrLoadIterator ld(*loads);
   while(ld)
      {
      MbrLoad *mbrLd = (MbrLoad *)ld++;

      mbrLd->GetForceVector(GetMemberType(),GetLength(),GetAngle(),force);
      mbrLd->GetDispVector(GetMemberType(),GetLength(),GetAngle(),section->GetArea(),section->GetCentroidalInertia().Ixx,section->GetModE(),disp);

      // Rotations may need to be adjusted for global joint rotation...
      // ra = r + rb ???

      Fx1 += force[0];
      Fy1 += force[1];
      Mz1 += force[2];
      Fx2 += force[3];
      Fy2 += force[4];
      Mz2 += force[5];

      dx1 += disp[0];
      dy1 += disp[1];
      rz1 += disp[2];
      dx2 += disp[3];
      dy2 += disp[4];
      rz2 += disp[5];
      }

   // save displacements
   (*Dlocal)(0) = dx1;
   (*Dlocal)(1) = dy1;
   (*Dlocal)(2) = rz1;
   (*Dlocal)(3) = dx2;
   (*Dlocal)(4) = dy2;
   (*Dlocal)(5) = rz2;

   // save forces
   (*Rlocal)(0) = Fx1;
   (*Rlocal)(1) = Fy1;
   (*Rlocal)(2) = Mz1;
   (*Rlocal)(3) = Fx2;
   (*Rlocal)(4) = Fy2;
   (*Rlocal)(5) = Mz2;

   delete[] force;
   delete[] disp;
}

void PlnFrameMbr::Release(MbrType type)
{
   switch(type)
      {
      case mtPinPin:
           relStart = mbrRelMz;
           relEnd   = mbrRelMz;
           break;

      case mtPinFix:
           relStart = mbrRelMz;
           relEnd   = mbrRelNone;
           break;

      case mtFixPin:
           relStart = mbrRelNone;
           relEnd   = mbrRelMz;
           break;

      case mtFixFix:
           relStart = mbrRelNone;
           relEnd   = mbrRelNone;
           break;
      }
}

void PlnFrameMbr::Release(MbrEnd end,MbrRelease release)
{
   switch(end)
      {
      case mbrStart:
           relStart = release;
           break;

      case mbrEnd:
           relEnd = release;
           break;
      }
}

bool PlnFrameMbr::IsReleased(MbrEnd end,MbrRelease release)
{
   MbrRelease relTest;

   relTest = (end == mbrStart) ? relStart : relEnd;

   return (relTest & release);
}

MbrType PlnFrameMbr::GetMemberType()
{
   MbrType mbrType;

   if ((relStart & mbrRelMz) && (relEnd & mbrRelMz))
      mbrType = mtPinPin;
   else if ((relStart & mbrRelMz) && !(relEnd & mbrRelMz))
      mbrType = mtPinFix;
   else if (!(relStart & mbrRelMz) && (relEnd & mbrRelMz))
      mbrType = mtFixPin;
   else
      mbrType = mtFixFix;

   return mbrType;
}

// ComputeDisplacements
//
// Computes the member joint displacements in local coordinates,
// making adjustments for member releases.
//
// This function should be moved to a higher level of abstraction
// as TFemModel matures.
void PlnFrameMbr::ComputeDisplacements()
{
   double Dglobal[6];
   Vector Disp(TotalDOF);
   Joint *StartJnt, *EndJnt;
   StartJnt = joint[0];
   EndJnt   = joint[1];

   StartJnt->GetDisplacement(Dglobal);
   EndJnt->GetDisplacement(&Dglobal[3]);
   Disp(0) = Dglobal[0];
   Disp(1) = Dglobal[1];
   Disp(2) = Dglobal[2];
   Disp(3) = Dglobal[3];
   Disp(4) = Dglobal[4];
   Disp(5) = Dglobal[5];

   TransMatrix->Multiply(&Disp,Dlocal);

   // adjust member end rotation at released ends.
   double r1,r2;
   switch(GetMemberType())
      {
      case mtPinPin:
           // Joint rotations are incorrect for this member.
           // Adjust based on actual boundary condition.
           GetPinPinRotation(r1,r2);
           (*Dlocal)(2) = r1;
           (*Dlocal)(5) = r2;
           break;

      case mtPinFix:
           // Start joint rotation is incorrect for this member.
           // Adjust based on actual boundary condition.
           GetPinFixRotation(r1,r2);
           (*Dlocal)(2) = r1;
           break;

      case mtFixPin:
           // End joint rotation is incorrect for this member.
           // Adjust based on actual boundary condition.
           GetFixPinRotation(r1,r2);
           (*Dlocal)(5) = r2;
           break;
      }
}

// ComputeForces
//
// Computes the element forces at its joints.
// Assumes ComputeDisplacements have been called.
void PlnFrameMbr::ComputeForces()
{
   double Dglobal[6];
   Vector Disp(6);
   Vector Rglobal(6);
   Joint *StartJnt, *EndJnt;
   StartJnt = joint[0];
   EndJnt   = joint[1];

   StartJnt->GetDisplacement(Dglobal);
   EndJnt->GetDisplacement(&Dglobal[3]);
   Disp(0) = Dglobal[0];
   Disp(1) = Dglobal[1];
   Disp(2) = Dglobal[2];
   Disp(3) = Dglobal[3];
   Disp(4) = Dglobal[4];
   Disp(5) = Dglobal[5];

   Kglobal->Multiply(&Disp,&Rglobal);
   Rglobal -= (*Fglobal);
   TransMatrix->Multiply(&Rglobal,Rlocal);
}

void PlnFrameMbr::GetJntForces(double *force)
{
   int i;
   for (i = 0; i < 6; i++)
      force[i] = (*Rlocal)(i);
}

void PlnFrameMbr::GetGlobalJntForces(int jntId,double *force)
{
   int i,count,start,end;
   Vector Rglobal(6);

   TransMatrix->Multiply(Rlocal,&Rglobal,ATB);

   start = jntId*TotalDOF/NumJoints;
   end = start + TotalDOF/NumJoints;

   for (i = start, count = 0; i < end; i++, count++)
      {
      force[count] = Rglobal(i);
      }
}

int PlnFrameMbr::GetJointId(Joint &j)
{
   int i;
   int jntId = -1;

   for (i = 0; i < NumJoints; i++)
      {
      if (&j == joint[i])
         {
         jntId = i;
         break;
         }
      }

   CHECKX(jntId >= 0,"PlnFrameMbr::GetJointId - Joint not found");

   return jntId;
}

void PlnFrameMbr::GetJntDisplacements(double *disp)
{
   int i;
   for (i = 0; i < 6; i++)
      disp[i] = (*Dlocal)(i);
}

// GetInternalForces
//
// Computes the internal member forces at the prescribed section.
// Assumes a stiffness analysis has been performed for the active
// loadcase. Also assumes the active loadcase is correct.
void PlnFrameMbr::GetInternalForces(double loc,MbrFace face,double *force)
{
   double fx, fy, mz; // Contribution of a single external load
   double Fx, Fy, Mz; // Contribution of all external loads
   double Rx, Ry, Rz; // Member End Forces

   // Initialize internal force resultants
   Fx = 0;
   Fy = 0;
   Mz = 0;

   // Iterate over all loads on this member, for the active loadcase
   MbrLoadIterator ld(*loads);
   while(ld)
      {
      // Member loads know how to compute their own internal force effects.
      MbrLoad *mbrLd = (MbrLoad *)ld++;
      mbrLd->GetInternalForces(loc,GetLength(),GetAngle(),face,fx,fy,mz);

      // Integrate the internal forces for this load into the resultant
      // internal force. The effect of the member end forces associated
      // with this load are not included.
      Fx += fx;
      Fy += fy;
      Mz += mz;
      }

   // Add effects of member end forces
   if (face == mbrRightFace)
      {
      // right face
      Rx = (*Rlocal)(3);
      Ry = (*Rlocal)(4);
      Rz = (*Rlocal)(5);
      force[0] = -Fx + -Rx;
      force[1] = -Fy + -Ry;
      force[2] = -Mz + -Ry*(GetLength() - loc) - Rz;
      }
   else
      {
      // left face
      Rx = (*Rlocal)(0);
      Ry = (*Rlocal)(1);
      Rz = (*Rlocal)(2);
      force[0] = -Fx + -Rx;
      force[1] = -Fy + -Ry;
      force[2] = -Mz +  Ry*loc + -Rz;
      }
}

// GetDeflection
//
// Gets the deflection normal to the member at the specified location,
// in local coordinates.
//
// For now, this function only gets the dy deflection. It must be
// updated to compute the dx and rz deflections as well.
void PlnFrameMbr::GetDeflection(double loc,double *disp)
{
   double dx, dy, rz;

   // Initialize internal force resultants
   disp[0] = 0;
   disp[1] = 0;
   disp[2] = 0;

   // Iterate over all loads on this member, for the active loadcase
   MbrLoadIterator ld(*loads);
   while(ld)
      {
      // Member loads know how to compute their own internal displacements.
      MbrLoad *mbrLd = (MbrLoad *)ld++;
      mbrLd->GetDisplacement(loc,mtPinPin,GetLength(),GetAngle(),
                             section->GetArea(),section->GetCentroidalInertia().Ixx,section->GetModE(),
                             dx,dy,rz);

      // Integrate the internal displacements for this load into the resultant
      // internal displacement. The effect of the member end disp associated
      // with this load are not included.
      disp[0] += dx;
      disp[1] += dy;
      disp[2] += rz;
      }

   // Compute displacements due to resultant moment at start of member
   TConcLdBeam Beam1((*Rlocal)(2),0,MomentZ,GetLength(),section->GetArea(),section->GetCentroidalInertia().Ixx,
                     section->GetModE(),mtPinPin);
   Beam1.GetDeflection(loc,dx,dy,rz);
   disp[0] += dx;
   disp[1] += dy;
   disp[2] += rz;

   // Compute displacements due to resultant moment at end of member
   TConcLdBeam Beam2((*Rlocal)(5),GetLength(),MomentZ,GetLength(),section->GetArea(),section->GetCentroidalInertia().Ixx,
                     section->GetModE(),mtPinPin);
   Beam2.GetDeflection(loc,dx,dy,rz);
   disp[0] += dx;
   disp[1] += dy;
   disp[2] += rz;

   // Compute effects of frame displacement
   disp[0] += (*Dlocal)(0) + ((*Dlocal)(3) - (*Dlocal)(0))*loc/GetLength();
   disp[1] += (*Dlocal)(1) + ((*Dlocal)(4) - (*Dlocal)(1))*loc/GetLength();
}

// GetPinPinRotation
//
// Computes the member end rotations for a member with Pin-Pin
// internal boundary conditions. Uses the current element loads.
void PlnFrameMbr::GetPinPinRotation(double &rz1,double &rz2)
{
   double vector[6];
   MbrLoadIterator i(*loads);

   rz1 = 0;
   rz2 = 0;

   while(i)
      {
      MbrLoad *load = i++;
      load->GetDispVector(mtPinPin,GetLength(),GetAngle(),
                          section->GetArea(),section->GetCentroidalInertia().Ixx,section->GetModE(),
                          vector);
      rz1 += vector[2];
      rz2 += vector[5];
      }
}

// GetPinFixRotation
//
// Computes the member end rotations for a member with Pin-Fix
// internal boundary conditions. Uses the current element loads.
//
// Rotation at the start end is equal to the rotation at the
// end end less the area under the M/EI diagram
void PlnFrameMbr::GetPinFixRotation(double &rz1,double &rz2)
{
   double vector[6];
   MbrLoadIterator i(*loads);

   rz1 = 0;
   rz2 = 0;

   while(i)
      {
      MbrLoad *load = i++;
      load->GetDispVector(mtPinFix,GetLength(),GetAngle(),
                          section->GetArea(),section->GetCentroidalInertia().Ixx,section->GetModE(),
                          vector);
      rz1 += vector[2];
      rz2 += vector[5];
      }

   rz1 += GetLength()*(*Rlocal)(5)/(2 * section->GetCentroidalInertia().Ixx * section->GetModE());
   rz1 = (*Dlocal)(5) - rz1;
}

// GetFixPinRotation
//
// Computes the member end rotations for a member with Fix-Pin
// internal boundary conditions. Uses the current element loads.
//
// The rotation at the end end is the rotation at the start end
// plus the area under the M/EI diagram.
void PlnFrameMbr::GetFixPinRotation(double &rz1,double &rz2)
{
   double vector[6];
   MbrLoadIterator i(*loads);

   rz1 = 0;
   rz2 = 0;

   while(i)
      {
      MbrLoad *load = i++;
      load->GetDispVector(mtFixPin,GetLength(),GetAngle(),
                          section->GetArea(),section->GetCentroidalInertia().Ixx,section->GetModE(),
                          vector);
      rz1 += vector[2];
      rz2 += vector[5];
      }

   rz2 += -(*Rlocal)(2)*GetLength()/(2 * section->GetCentroidalInertia().Ixx * section->GetModE());
   rz2 =   (*Dlocal)(2) + rz2;
}

void PlnFrameMbr::AssembleF()
{
   // Assembles the local and global force vectors for the applied loads.
   // Loads should have been applied from TFemModel for the active load case.
   Vector Flocal(TotalDOF);

   // Initialize the local force vector
   Flocal(0) = 0;
   Flocal(1) = 0;
   Flocal(2) = 0;
   Flocal(3) = 0;
   Flocal(4) = 0;
   Flocal(5) = 0;

   // Iterate over all loads on this member
   MbrLoadIterator ld(*loads);
   double *vector;
   vector = new double[TotalDOF];
   while(ld)
      {
      MbrLoad *mbrLd = ld++;
      mbrLd->GetForceVector(GetMemberType(),GetLength(),GetAngle(),vector);

      // Integrate the force vector into this member's local force vector
      for (int i = 0; i < TotalDOF; i++)
         Flocal(i) -= vector[i];
      }
   delete[] vector;

   // Compute Global Force Vector
   TransMatrix->Multiply(&Flocal,Fglobal,ATB);
}

int PlnFrameMbr::GetNumDOF() const
{
   return TotalDOF;
}

int PlnFrameMbr::GetNumJoints() const
{
   return NumJoints;
}

int PlnFrameMbr::GetCondensedDOF(int dof)
{
   int retval;
   Joint *StartJnt, *EndJnt;
   StartJnt = joint[0];
   EndJnt   = joint[1];

   if (dof < 3)
      retval = StartJnt->GetCondensedDOF(dof);
   else
      retval = EndJnt->GetCondensedDOF(dof - 3);

   return retval;
}

double PlnFrameMbr::GetKglobal(int DOFi,int DOFj)
{
   return (*Kglobal)(DOFi,DOFj);
}

bool PlnFrameMbr::operator == (const PlnFrameMbr &mbr) const
{
   return (ID == mbr.GetID());
}

void PlnFrameMbr::ApplyLoad(MbrLoad *load)
{
   loads->Add(load);
}

void PlnFrameMbr::GetFglobal(double *f) const
{
   int i;

   for (i = 0; i < TotalDOF; i++)
      f[i] = (*Fglobal)(i);
}

// GenDeadload
//
// Generates dead load.
// Allowing a member to generate it's own deadload and adding the
// load components to a loadcase, releives TFemModel from this
// responsibility. The primary benifit of this is that new
// member and element types can be added, and TFemModel doesn't care.
// As TFemModel library matures, this should become a pure virtual
// member function for all member and element types.
void PlnFrameMbr::GenDeadload(GravityDir dir,double scale,DeadLoadingType type)
{
   LoadDir ldDir;
   double sign;
   double q;

   switch(dir)
      {
      case NegX:
           ldDir = ForceX;
           sign = -1;
           break;

      case NegY:
           ldDir = ForceY;
           sign = -1;
           break;

      case PosX:
           ldDir = ForceX;
           sign = 1;
           break;

      case PosY:
           ldDir = ForceY;
           sign = 1;
           break;
      }

   if (type == dltMember)
      {
      q = scale*sign*section->GetArea()*section->GetUnitWeight();
      mbrDeadLoad = new UniformLoad(ID,0,GetLength(),q,false,ldDir,GlobalProjected);
      ApplyLoad(mbrDeadLoad);
      }
   else
      {
      q = scale*sign*section->GetArea()*section->GetUnitWeight()*GetLength()/2;
      JntLoad j0(joint[0]->GetID(),ldDir,q);
      JntLoad j1(joint[1]->GetID(),ldDir,q);
      joint[0]->ApplyLoad(&j0);
      joint[1]->ApplyLoad(&j1);
      }
}

TShape* PlnFrameMbr::GetSection()  const
{
   return section;
}

void PlnFrameMbr::Link(Joint &jnt)
{
   PRECONDITIONX(&jnt,"PlnFrameMbr::Unlink() - bad joint");
   jnt.attachedElements->Add(this);
}

void PlnFrameMbr::Unlink(Joint &jnt)
{
   if (&jnt)
      jnt.attachedElements->Detach(this,TShouldDelete::NoDelete);
}

bool PlnFrameMbr::IsEquilibriumSatisfied(double tolerance)
{
   double _fx, _fy, _mz; // Contribution of a single external load
   double fx, fy, mz; // Contribution of all external loads on the member
   double Fx, Fy, Mz; // Resultant forces at right end of member
   double fx1,fy1,mz1; // Member end forces at start of member
   double fx2,fy2,mz2; // Member end forces at end of member
   double length;   // Member length
   double angle; // Member orientation

   // Initialize internal force resultants
   fx = 0;
   fy = 0;
   mz = 0;

   length = GetLength();
   angle = GetAngle();

   // Iterate over all loads on this member, for the active loadcase
   // Get force effects at start of member from the applied loads.
   MbrLoadIterator iter(*loads);
   while(iter)
      {
      // Member loads know how to compute their own internal force effects.
      MbrLoad *mbrLd = iter++;
      mbrLd->GetOriginForces(length,angle,_fx,_fy,_mz);
      fx += _fx;
      fy += _fy;
      mz += _mz;
      }

   fx1 = (*Rlocal)(0);
   fy1 = (*Rlocal)(1);
   mz1 = (*Rlocal)(2);
   fx2 = (*Rlocal)(3);
   fy2 = (*Rlocal)(4);
   mz2 = (*Rlocal)(5);
   Fx = fx + fx1 + fx2;
   Fy = fy + fy1 + fy2;
   Mz = mz + mz1 + mz2 + length*fy2;


   if (!((-tolerance <= Fx) && (Fx <= tolerance)))
      return false;
   if (!((-tolerance <= Fy) && (Fy <= tolerance)))
      return false;
   if (!((-tolerance <= Mz) && (Mz <= tolerance)))
      return false;

   return true;
}

