///////////////////////////////////////////////////////////////////////
// OWLTools - Extension Library for the Borland OWL Framework
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
// May 1997 - Created, Richard Brice
// Oct 1999 - Released as open source

#ifndef UNITS_H_
#define UNITS_H_

#include "owltools.h"
#include <math.h>
#include <iostream.h>

typedef _OWLDATA enum TSIPrefix{a,f,p,n,u,_m,c,_base_,d,da,h,k,M,G,T};
typedef _OWLDATA enum TMassUnit {ag,fg,pg,ng,ug,mg,cg,g,dg,dag,hg,kg,Mg,Gg,Tg,slug,lbm};
typedef _OWLDATA enum TLengthUnit {am,fm,pm,nm,um,mm,cm,m,dm,dam,hm,km,Mm,Gm,Tm,inch,feet,yard,mile};
typedef _OWLDATA enum TForceUnit {aN,fN,pN,nN,uN,mN,cN,N,dN,daN,hN,kN,MN,GN,TN,lbs,kips};
typedef _OWLDATA enum TPressureUnit {aPa,fPa,pPa,nPa,uPa,mPa,cPa,Pa,dPa,daPa,hPa,kPa,MPa,GPa,TPa,psi,psf,ksi,ksf};
typedef _OWLDATA enum TForceDensityUnit {Ncm = 8,kNcm = 12,pci,pcf,kci,kcf};
typedef _OWLDATA enum TMomentUnit {Nm = 8,kNm = 12,inlbs,ftlbs,inkips,ftkips};
typedef _OWLDATA enum TAreaUnit {sqm = 8,sqin,sqft};
typedef _OWLDATA enum TVolumeUnit {cum = 8,cuin,cuft};
typedef _OWLDATA enum TTimeUnit {second,minute,hour};

ostream& _OWLFUNC operator << (ostream &os,TLengthUnit &lu);
ostream& _OWLFUNC operator << (ostream &os,TForceUnit &fu);
ostream& _OWLFUNC operator << (ostream &os,TPressureUnit &pu);
ostream& _OWLFUNC operator << (ostream &os,TMomentUnit &mu);
ostream& _OWLFUNC operator << (ostream &os,TAreaUnit &au);
ostream& _OWLFUNC operator << (ostream &os,TVolumeUnit &vu);

class _OWLCLASS TGenericUnit
{
protected:
   double SIscale[15];

public:
   TGenericUnit();

protected:
   double ScaleUS(double val,double scale1,double scale2,double exponent);
   double ScaleSI(double val,double scale1,double scale2,double exponent);
   double ConvertUStoSI(double val,double convFactor,double exponent);
   double ConvertSItoUS(double val,double convFactor,double exponent);
};

class _OWLCLASS TUnitMass : public TGenericUnit
{
// Data Members
protected:
   double convFactor;
   double massScale[3];
   double exponent;

// Constructor/Destructor
public:
   TUnitMass();

// Methods
public:
   double Convert(double val,TMassUnit from,TMassUnit to);

private:
   double ConvertUS_US(double val,TMassUnit from,TMassUnit to);
   double ConvertUS_SI(double val,TMassUnit from,TMassUnit to);
   double ConvertSI_US(double val,TMassUnit from,TMassUnit to);
   double ConvertSI_SI(double val,TMassUnit from,TMassUnit to);
};

class _OWLCLASS TUnitTime : public TGenericUnit
{
// Data Members
protected:
   double timeScale[3];
   double exponent;

// Constructors/Destructor
   TUnitTime();

// Methods
   double Convert(double vale,TTimeUnit from,TTimeUnit to);
};

class _OWLCLASS TUnitLength : public TGenericUnit
{
// Data Members
protected:
   double convFactor;
   double lengthScale[4];
   double exponent;

// Constructor/Destructor
public:
   TUnitLength();

// Methods
public:
   double Convert(double val,TLengthUnit from,TLengthUnit to);

private:
   double ConvertUS_US(double val,TLengthUnit from,TLengthUnit to);
   double ConvertUS_SI(double val,TLengthUnit from,TLengthUnit to);
   double ConvertSI_US(double val,TLengthUnit from,TLengthUnit to);
   double ConvertSI_SI(double val,TLengthUnit from,TLengthUnit to);
};

class _OWLCLASS TUnitArea : protected TUnitLength
{
// Constructor/Destructor
public:
   TUnitArea();
   double Convert(double val,TAreaUnit from,TAreaUnit to);
   TLengthUnit GetLengthUnit(TAreaUnit au);
};

class _OWLCLASS TUnitVolume : public TUnitLength
{
// Constructor/Destructor
public:
   TUnitVolume();
   TLengthUnit GetLengthUnit(TVolumeUnit vu);
   double Convert(double val,TVolumeUnit from,TVolumeUnit to);
};

class _OWLCLASS TUnitForce : public TUnitMass,public TUnitLength,public TUnitTime
{
// Data Members
protected:
   double forceScale[2];
   double exponent;

// Constructor/Destructor
public:
   TUnitForce();

// Methods
public:
   double Convert(double val,TForceUnit from,TForceUnit to);

private:
   double ConvertUS_US(double val,TForceUnit from,TForceUnit to);
   double ConvertUS_SI(double val,TForceUnit from,TForceUnit to);
   double ConvertSI_US(double val,TForceUnit from,TForceUnit to);
   double ConvertSI_SI(double val,TForceUnit from,TForceUnit to);
};

class _OWLCLASS TUnitPressure : public TUnitForce,public TUnitArea
{
// Constructor/Destructor
public:
   TUnitPressure();

// Methods
public:
   double Convert(double val,TPressureUnit from,TPressureUnit to);
   void DecomposeUnit(TPressureUnit pu,TForceUnit &fu,TAreaUnit &au);
};

class _OWLCLASS TUnitForceDensity : public TUnitForce, public TUnitVolume
{
// Constructor/Destructor
public:
   TUnitForceDensity();

// Methods
public:
   double Convert(double val,TForceDensityUnit from,TForceDensityUnit to);
   void DecomposeUnit(TForceDensityUnit pu,TForceUnit &fu,TVolumeUnit &vu);
};

class _OWLCLASS TUnitMoment : public TUnitForce
{
// TUnitLength is already an ansestor of TUnitForce. Not need to derive
// from this class again.
// Constructor/Destructor
public:
   TUnitMoment();

// Methods
public:
   double Convert(double val,TMomentUnit from,TMomentUnit to);

private:
   void DecomposeUnit(TMomentUnit pu,TForceUnit &fu,TLengthUnit &lu);
};

#endif // UNITS_H_