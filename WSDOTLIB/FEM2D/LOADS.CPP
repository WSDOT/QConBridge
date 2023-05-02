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

#include <fem2d\loads.h>
#include <fem2d\beam.h>
#include <math.h>

//
//
//
JntLoad::JntLoad(int jnt,double fx,double fy,double mz)
{
   JntID = jnt;
   Fx = fx;
   Fy = fy;
   Mz = mz;
}

JntLoad::JntLoad(int jnt,LoadDir dir,double q)
{
   JntID = jnt;
   Fx = 0;
   Fy = 0;
   Mz = 0;

   switch(dir)
      {
      case ForceX:
           Fx = q;
           break;

      case ForceY:
           Fy = q;
           break;

      case MomentZ:
           Mz = q;
           break;
      }
}

int JntLoad::GetJoint() const
{
   return JntID;
}

void JntLoad::GetLoad(double &fx,double &fy,double &mz) const
{
   fx = Fx;
   fy = Fy;
   mz = Mz;
}

double JntLoad::GetLoad(LoadDir dir) const
{
   double ld;

   switch(dir)
      {
      case ForceX:
           ld = Fx;
           break;

      case ForceY:
           ld = Fy;
           break;

      case MomentZ:
           ld = Mz;
           break;
      }

   return ld;
}

bool JntLoad::operator == (const JntLoad &jntLd) const
{
   return (JntID == jntLd.GetJoint());
}

ostream &operator << (ostream &os,const JntLoad &jntLd)
{
   double fx,fy,mz;
   jntLd.GetLoad(fx,fy,mz);
   os << "JOINT " << jntLd.GetJoint() << " LOAD FORCE X " << fx
      << " FORCE Y " << fy << " MOMENT Z " << mz;

   return os;
}

// MbrLoad
//
// Abstract base class for all loads applied to members.
MbrLoad::MbrLoad(int mbrId,bool isFract,LoadDir dir,LoadOrientation orientation)
{
   MbrID = mbrId;
   IsFract = isFract;
   Dir = dir;
   Orientation = orientation;
}

int MbrLoad::GetMbr() const
{
   return MbrID;
}

LoadDir MbrLoad::GetLoadDirection() const
{
   return Dir;
}

LoadOrientation MbrLoad::GetLoadOrientation() const
{
   return Orientation;
}

bool MbrLoad::operator == (const MbrLoad &mbrLd) const
{
   return (MbrID == mbrLd.GetMbr());
}

ostream &operator << (ostream &os,const MbrLoad &mbrLd)
{
   mbrLd.Write(os);
   return os;
}

// UniformLoad
//
// This should be replaced with TLinearLoad when that loading is available
UniformLoad::UniformLoad(int mbrId,double la,double lb,double q,bool isFract,LoadDir dir,LoadOrientation orientation) :
MbrLoad(mbrId,isFract,dir,orientation)
{
   a = la;
   b = lb;
   Q = q;
}

double UniformLoad::GetLa() const
{
   return a;
}

double UniformLoad::GetLb() const
{
   return b;
}

double UniformLoad::GetQ() const
{
   return Q;
}

void UniformLoad::GetForceVector(MbrType type,double Length,double Angle,double *vector)
{
   double La,Lb;
   double Px,Py,Mz;
   double Fx1,Fy1,Mz1;
   double Fx2,Fy2,Mz2;
   TBeam *Pbeam,*Vbeam,*Mbeam;

   if (IsFract)
      {
      La = a*Length;
      Lb = b*Length;
      }
   else
      {
      La = a;
      Lb = b;
      }

   // Initialize the force vector
   vector[0] = 0;
   vector[1] = 0;
   vector[2] = 0;
   vector[3] = 0;
   vector[4] = 0;
   vector[5] = 0;

   GetLoadComponents(Angle,Px,Py,Mz);
   Pbeam = new	TUnifLdBeam(Px,La,Lb,ForceX,Length,1,1,1,type);
   Vbeam = new	TUnifLdBeam(Py,La,Lb,ForceY,Length,1,1,1,type);
   Mbeam = new	TUnifLdBeam(Mz,La,Lb,MomentZ,Length,1,1,1,type);

   Pbeam->GetReactions(Fx1,Fy1,Mz1,Fx2,Fy2,Mz2);
   vector[0] += Fx1;
   vector[1] += Fy1;
   vector[2] += Mz1;
   vector[3] += Fx2;
   vector[4] += Fy2;
   vector[5] += Mz2;

   Vbeam->GetReactions(Fx1,Fy1,Mz1,Fx2,Fy2,Mz2);
   vector[0] += Fx1;
   vector[1] += Fy1;
   vector[2] += Mz1;
   vector[3] += Fx2;
   vector[4] += Fy2;
   vector[5] += Mz2;

   Mbeam->GetReactions(Fx1,Fy1,Mz1,Fx2,Fy2,Mz2);
   vector[0] += Fx1;
   vector[1] += Fy1;
   vector[2] += Mz1;
   vector[3] += Fx2;
   vector[4] += Fy2;
   vector[5] += Mz2;

   delete Pbeam;
   delete Vbeam;
   delete Mbeam;
}

void UniformLoad::GetDispVector(MbrType type,double Length,double Angle,
                                double Ax,double Iz,double E,
                                double *vector)
{
   double La,Lb;
   double Px,Py,Mz;
   double Dx1,Dy1,Rz1;
   double Dx2,Dy2,Rz2;
   TBeam *Pbeam,*Vbeam,*Mbeam;

   if (IsFract)
      {
      La = a*Length;
      Lb = b*Length;
      }
   else
      {
      La = a;
      Lb = b;
      }

   // Initialize the force vector
   vector[0] = 0;
   vector[1] = 0;
   vector[2] = 0;
   vector[3] = 0;
   vector[4] = 0;
   vector[5] = 0;

   GetLoadComponents(Angle,Px,Py,Mz);
   Pbeam = new	TUnifLdBeam(Px,La,Lb,ForceX,Length,Ax,Iz,E,type);
   Vbeam = new	TUnifLdBeam(Py,La,Lb,ForceY,Length,Ax,Iz,E,type);
   Mbeam = new	TUnifLdBeam(Mz,La,Lb,MomentZ,Length,Ax,Iz,E,type);

   Pbeam->GetDisplacements(Dx1,Dy1,Rz1,Dx2,Dy2,Rz2);
   vector[0] += Dx1;
   vector[1] += Dy1;
   vector[2] += Rz1;
   vector[3] += Dx2;
   vector[4] += Dy2;
   vector[5] += Rz2;

   Vbeam->GetDisplacements(Dx1,Dy1,Rz1,Dx2,Dy2,Rz2);
   vector[0] += Dx1;
   vector[1] += Dy1;
   vector[2] += Rz1;
   vector[3] += Dx2;
   vector[4] += Dy2;
   vector[5] += Rz2;

   Mbeam->GetDisplacements(Dx1,Dy1,Rz1,Dx2,Dy2,Rz2);
   vector[0] += Dx1;
   vector[1] += Dy1;
   vector[2] += Rz1;
   vector[3] += Dx2;
   vector[4] += Dy2;
   vector[5] += Rz2;

   delete Pbeam;
   delete Vbeam;
   delete Mbeam;
}

void UniformLoad::GetInternalForces(double x,double Length,double Angle,MbrFace face,double &Fx,double &Fy,double &Mz)
{
   double La,Lb;
   double Px,Py,Mo; // force or moment per unit length

   if (IsFract)
      {
      La = a*Length;
      Lb = b*Length;
      }
   else
      {
      La = a;
      Lb = b;
      }

   if ( ((face == mbrLeftFace)  && (x <= La)) ||
        ((face == mbrRightFace) && (x >  Lb)) )
      {
      // Load has not contribution at this location
      Fx = 0;
      Fy = 0;
      Mz = 0;
      return;
      }

   GetLoadComponents(Angle,Px,Py,Mo);

   if ( (face == mbrLeftFace) && (Lb > x) )
      Lb = x;

   if ( (face == mbrRightFace) && (La < x) )
      La = x;

   Fx = Px*(Lb - La);
   Fy = Py*(Lb - La);
   Mz = Mo*(Lb - La) + Py*(Lb - La)*(Lb + La - 2.*x)/2.;
}

void UniformLoad::GetDisplacement(double x,MbrType type,double Length,double Angle,
                                  double Ax,double Iz,double E,
                                  double &dx,double &dy,double &rz)

{
   double La,Lb;
   double Px,Py,Mz;
   double Dx,Dy,Rz;
   TBeam *Pbeam,*Vbeam,*Mbeam;

   if (IsFract)
      {
      La = a*Length;
      Lb = b*Length;
      }
   else
      {
      La = a;
      Lb = b;
      }

   // Initialize the force vector
   dx = 0;
   dy = 0;
   rz = 0;

   GetLoadComponents(Angle,Px,Py,Mz);
   Pbeam = new	TUnifLdBeam(Px,La,Lb,ForceX,Length,Ax,Iz,E,type);
   Vbeam = new	TUnifLdBeam(Py,La,Lb,ForceY,Length,Ax,Iz,E,type);
   Mbeam = new	TUnifLdBeam(Mz,La,Lb,MomentZ,Length,Ax,Iz,E,type);

   Pbeam->GetDeflection(x,Dx,Dy,Rz);
   dx += Dx;
   dy += Dy;
   rz += Rz;

   Vbeam->GetDeflection(x,Dx,Dy,Rz);
   dx += Dx;
   dy += Dy;
   rz += Rz;

   Mbeam->GetDeflection(x,Dx,Dy,Rz);
   dx += Dx;
   dy += Dy;
   rz += Rz;

   delete Pbeam;
   delete Vbeam;
   delete Mbeam;
}

void UniformLoad::GetOriginForces(double Length,double Angle,double &Fx,double &Fy,double &Mz)
{
   double La,Lb;
   double Px,Py,Mo; // force or moment per unit length

   if (IsFract)
      {
      La = a*Length;
      Lb = b*Length;
      }
   else
      {
      La = a;
      Lb = b;
      }

   GetLoadComponents(Angle,Px,Py,Mo);

   Fx = Px*(Lb - La);
   Fy = Py*(Lb - La);
   Mz = Mo*(Lb - La) + Py*(Lb - La)*(Lb + La)/2.;
}

void UniformLoad::GetLoadComponents(double Angle,double &Px,double &Py,double &Mz)
{
   switch(Dir)
      {
      case ForceX:
           Mz = 0;
           if (Orientation == Global || Orientation == GlobalProjected)
              {
              Px = -Q*cos(Angle);
              Py =  Q*sin(Angle);
              }
           else
              {
              Px = Q;
              Py = 0;
              }
           break;

      case ForceY:
           Mz = 0;
           if (Orientation == Global || Orientation == GlobalProjected)
              {
              Px = Q*sin(Angle);
              Py = Q*cos(Angle);
              }
           else
              {
              Px = 0;
              Py = Q;
              }
           break;

      case MomentZ:
           Px = 0;
           Py = 0;
           Mz = Q;
           break;
      }
}

void UniformLoad::Write(ostream &os) const
{
   char *Direction[3] = {"FORCE X","FORCE Y","MOMENT Z"};
   char *Orientation[3] = {"LOCAL","GLOBAL","GLOBAL PROJECTED"};

   os << "MEMBER " << GetMbr() << " LOAD "
      << Direction[GetLoadDirection()] << " "
      << Orientation[GetLoadOrientation()] << " "
      << "UNIFORM ";

   if (IsFract)
      os << "FRA ";

   os << "W " << GetQ()   << " "
      << "LA " << GetLa() << " "
      << "LB " << GetLb();
}

// ConcentratedLoad
//
// Global Projected loads will be taken as Global Loads.
ConcentratedLoad::ConcentratedLoad(int mbrId,double la,double q,bool isFract,LoadDir dir,LoadOrientation orientation) :
MbrLoad(mbrId,isFract,dir,orientation)
{
   a = la;
   Q = q;
}

double ConcentratedLoad::GetL() const
{
   return a;
}

double ConcentratedLoad::GetQ() const
{
   return Q;
}

void ConcentratedLoad::GetForceVector(MbrType type,double Length,double Angle,double *vector)
{
   double La;
   double Px,Py,Mz;
   double Fx1,Fy1,Mz1;
   double Fx2,Fy2,Mz2;
   TBeam *Pbeam,*Vbeam,*Mbeam;

   if (IsFract)
      La = a*Length;
   else
      La = a;

   // Initialize the force vector
   vector[0] = 0;
   vector[1] = 0;
   vector[2] = 0;
   vector[3] = 0;
   vector[4] = 0;
   vector[5] = 0;

   GetLoadComponents(Angle,Px,Py,Mz);
   Pbeam = new	TConcLdBeam(Px,La,ForceX,Length,1,1,1,type);
   Vbeam = new	TConcLdBeam(Py,La,ForceY,Length,1,1,1,type);
   Mbeam = new	TConcLdBeam(Mz,La,MomentZ,Length,1,1,1,type);

   Pbeam->GetReactions(Fx1,Fy1,Mz1,Fx2,Fy2,Mz2);
   vector[0] += Fx1;
   vector[1] += Fy1;
   vector[2] += Mz1;
   vector[3] += Fx2;
   vector[4] += Fy2;
   vector[5] += Mz2;

   Vbeam->GetReactions(Fx1,Fy1,Mz1,Fx2,Fy2,Mz2);
   vector[0] += Fx1;
   vector[1] += Fy1;
   vector[2] += Mz1;
   vector[3] += Fx2;
   vector[4] += Fy2;
   vector[5] += Mz2;

   Mbeam->GetReactions(Fx1,Fy1,Mz1,Fx2,Fy2,Mz2);
   vector[0] += Fx1;
   vector[1] += Fy1;
   vector[2] += Mz1;
   vector[3] += Fx2;
   vector[4] += Fy2;
   vector[5] += Mz2;

   delete Pbeam;
   delete Vbeam;
   delete Mbeam;
}

void ConcentratedLoad::GetDispVector(MbrType type,double Length,double Angle,
                                     double Ax,double Iz,double E,
                                     double *vector)
{
   double La;
   double Px,Py,Mz; // force or moment
   double Dx1,Dy1,Rz1;
   double Dx2,Dy2,Rz2;
   TBeam *Pbeam,*Vbeam,*Mbeam;

   if (IsFract)
      La = a*Length;
   else
      La = a;

   // Initialize the force vector
   vector[0] = 0;
   vector[1] = 0;
   vector[2] = 0;
   vector[3] = 0;
   vector[4] = 0;
   vector[5] = 0;

   GetLoadComponents(Angle,Px,Py,Mz);
   Pbeam = new	TConcLdBeam(Px,La,ForceX,Length,Ax,Iz,E,type);
   Vbeam = new	TConcLdBeam(Py,La,ForceY,Length,Ax,Iz,E,type);
   Mbeam = new	TConcLdBeam(Mz,La,MomentZ,Length,Ax,Iz,E,type);

   Pbeam->GetDisplacements(Dx1,Dy1,Rz1,Dx2,Dy2,Rz2);
   vector[0] += Dx1;
   vector[1] += Dy1;
   vector[2] += Rz1;
   vector[3] += Dx2;
   vector[4] += Dy2;
   vector[5] += Rz2;

   Vbeam->GetDisplacements(Dx1,Dy1,Rz1,Dx2,Dy2,Rz2);
   vector[0] += Dx1;
   vector[1] += Dy1;
   vector[2] += Rz1;
   vector[3] += Dx2;
   vector[4] += Dy2;
   vector[5] += Rz2;

   Mbeam->GetDisplacements(Dx1,Dy1,Rz1,Dx2,Dy2,Rz2);
   vector[0] += Dx1;
   vector[1] += Dy1;
   vector[2] += Rz1;
   vector[3] += Dx2;
   vector[4] += Dy2;
   vector[5] += Rz2;

   delete Pbeam;
   delete Vbeam;
   delete Mbeam;
}

void ConcentratedLoad::GetInternalForces(double x,double Length,double Angle,MbrFace face,double &Fx,double &Fy,double &Mz)
{
   double La;
   double Px,Py,Mo; // force or moment

   if (IsFract)
      La = a*Length;
   else
      La = a;

   if ( ((face == mbrLeftFace)  && (x <= La)) ||
        ((face == mbrRightFace) && (x >= La)) )
      {
      // Load has not contribution at this location.
      //
      // if (x == La) only consider it if (face == mbrLeftFace)
      Fx = 0;
      Fy = 0;
      Mz = 0;
      return;
      }

   GetLoadComponents(Angle,Px,Py,Mo);

   Fx = Px;
   Fy = Py;
   Mz = Mo + Py*(La-x);
}

void ConcentratedLoad::GetDisplacement(double x,MbrType type,double Length,double Angle,
                                       double Ax,double Iz,double E,
                                       double &dx,double &dy,double &rz)

{
   double La;
   double Px,Py,Mz;
   double Dx,Dy,Rz;
   TBeam *Pbeam,*Vbeam,*Mbeam;

   if (IsFract)
      La = a*Length;
   else
      La = a;

   // Initialize the force vector
   dx = 0;
   dy = 0;
   rz = 0;

   GetLoadComponents(Angle,Px,Py,Mz);
   Pbeam = new	TConcLdBeam(Px,La,ForceX,Length,Ax,Iz,E,type);
   Vbeam = new	TConcLdBeam(Py,La,ForceY,Length,Ax,Iz,E,type);
   Mbeam = new	TConcLdBeam(Mz,La,MomentZ,Length,Ax,Iz,E,type);

   Pbeam->GetDeflection(x,Dx,Dy,Rz);
   dx += Dx;
   dy += Dy;
   rz += Rz;

   Vbeam->GetDeflection(x,Dx,Dy,Rz);
   dx += Dx;
   dy += Dy;
   rz += Rz;

   Mbeam->GetDeflection(x,Dx,Dy,Rz);
   dx += Dx;
   dy += Dy;
   rz += Rz;

   delete Pbeam;
   delete Vbeam;
   delete Mbeam;
}

void ConcentratedLoad::GetOriginForces(double Length,double Angle,double &Fx,double &Fy,double &Mz)
{
   double La;
   double Px,Py,Mo; // force or moment per unit length

   if (IsFract)
      {
      La = a*Length;
      }
   else
      {
      La = a;
      }

   GetLoadComponents(Angle,Px,Py,Mo);

   Fx = Px;
   Fy = Py;
   Mz = Mo + Py*La;
}

void ConcentratedLoad::GetLoadComponents(double Angle,double &Px,double &Py,double &Mz)
{
   switch(Dir)
      {
      case ForceX:
           Mz = 0;
           if (Orientation == Global || Orientation == GlobalProjected)
              {
              Px = -Q*cos(Angle);
              Py =  Q*sin(Angle);
              }
           else
              {
              Px = Q;
              Py = 0;
              }
           break;

      case ForceY:
           Mz = 0;
           if (Orientation == Global || Orientation == GlobalProjected)
              {
              Px = Q*sin(Angle);
              Py = Q*cos(Angle);
              }
           else
              {
              Px = 0;
              Py = Q;
              }
           break;

      case MomentZ:
           Px = 0;
           Py = 0;
           Mz = Q;
           break;
      }
}

void ConcentratedLoad::Write(ostream &os) const
{
   char *Direction[3] = {"FORCE X","FORCE Y","MOMENT Z"};
   char *Orientation[3] = {"LOCAL","GLOBAL","GLOBAL PROJECTED"};

   os << "MEMBER " << GetMbr() << " LOAD "
      << Direction[GetLoadDirection()] << " "
      << Orientation[GetLoadOrientation()] << " "
      << "CONCENTRATED ";

   if (IsFract)
      os << "FRA ";

   os << "P " << GetQ() << " "
      << "L " << GetL();
}

