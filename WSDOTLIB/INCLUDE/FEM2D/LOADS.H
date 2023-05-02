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

#if !defined FEM2D_LOADS_H_
#define FEM2D_LOADS_H_

#include <checks.h>
#include <iostream.h>
#include <classlib\arrays.h>
#include <fem2d\femdll.h>
#include <fem2d\femall.h>

class _FEMCLASS JntLoad
{
// Data Members
protected:
   int JntID;
   double Fx,Fy,Mz;

// Constructors/Destructor
public:
   JntLoad(int jnt,double fx,double fy,double mz);
   JntLoad(int jnt,LoadDir dir,double q);

// Member Functions
public:
   int GetJoint() const;
   void GetLoad(double &fx,double &fy,double &mz) const;
   double GetLoad(LoadDir dir) const;
   bool operator == (const JntLoad &jntLd) const;

   friend ostream &operator << (ostream &os,const JntLoad &jntLd);
};

ostream &operator << (ostream &os,const JntLoad &jntLd);

typedef TIArrayAsVector<JntLoad> JntLoadContainer;
typedef TIArrayAsVectorIterator<JntLoad> JntLoadIterator;

class _FEMCLASS MbrLoad
{
// Data Members
protected:
   int MbrID;
   LoadDir Dir;
   LoadOrientation Orientation;
   bool IsFract;

// Constructors/Destructor
public:
   MbrLoad(int mbrId,bool isFract,LoadDir type,LoadOrientation orientation);

// Member Functions
public:
   int GetMbr() const;
   LoadDir GetLoadDirection() const;
   LoadOrientation GetLoadOrientation() const;
   virtual void GetForceVector(MbrType type,double Length,double Angle,double *vector) = 0;
   virtual void GetInternalForces(double x,double Length,double Angle,MbrFace face,double &Fx,double &Fy,double &Mz) = 0;
   virtual void GetDispVector(MbrType type,double Length,double Angle,double Ax,double Iz,double E,double *vector) = 0;
   virtual void GetDisplacement(double x,MbrType type,double Length,double Angle,
                                double Ax,double Iz,double E,
                                double &dx,double &dy,double &rz) = 0;
   virtual void GetOriginForces(double Length,double Angle,double &Fx,double &Fy,double &Mz) = 0;
   bool operator == (const MbrLoad &mbrLd) const;
   virtual void Write(ostream &os) const = 0;

   friend ostream &operator << (ostream &os,const MbrLoad &mbrLd);
};

ostream &operator << (ostream &os,const MbrLoad &mbrLd);

typedef TIArrayAsVector<MbrLoad> MbrLoadContainer;
typedef TIArrayAsVectorIterator<MbrLoad> MbrLoadIterator;

class _FEMCLASS UniformLoad : public MbrLoad
{
// Data Members
private:
   double a, b, Q;

// Constructors/Destructor
public:
   UniformLoad(int mbrId,double la,double lb,double q,bool isFract,LoadDir dir,LoadOrientation orientation);

// Member Functions
public:
   double GetLa() const;
   double GetLb() const;
   double GetQ() const;
   void GetForceVector(MbrType type,double Length,double Angle,double *vector);
   void GetInternalForces(double x,double Length,double Angle,MbrFace face,double &Fx,double &Fy,double &Mz);
   void GetDispVector(MbrType type,double Length,double Angle,double Ax,double Iz,double E,double *vector);
   void GetDisplacement(double x,MbrType type,double Length,double Angle,
                        double Ax,double Iz,double E,
                        double &dx,double &dy,double &rz);
   void GetOriginForces(double Length,double Angle,double &Fx,double &Fy,double &Mz);
   void Write(ostream &os) const;

private:
   void GetLoadComponents(double Angle,double &Px,double &Py,double &Mz);
};

class _FEMCLASS ConcentratedLoad : public MbrLoad
{
// Data Members
private:
   double a, Q;

// Constructors/Destructor
public:
   ConcentratedLoad(int mbrId,double l,double q,bool isFract,LoadDir dir,LoadOrientation orientation);

// Member Functions
public:
   double GetL() const;
   double GetQ() const;
   void GetForceVector(MbrType type,double Length,double Angle,double *vector);
   void GetInternalForces(double x,double Length,double Angle,MbrFace face,double &Fx,double &Fy,double &Mz);
   void GetDispVector(MbrType type,double Length,double Angle,double Ax,double Iz,double E,double *vector);
   void GetDisplacement(double x,MbrType type,double Length,double Angle,
                        double Ax,double Iz,double E,
                        double &dx,double &dy,double &rz);
   void GetOriginForces(double Length,double Angle,double &Fx,double &Fy,double &Mz);
   void Write(ostream &os) const;

private:
   void GetLoadComponents(double Angle,double &Px,double &Py,double &Mz);
};

#endif // v
