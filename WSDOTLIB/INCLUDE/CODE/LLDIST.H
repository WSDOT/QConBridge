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

#if !defined LLDIST_H_
#define LLDIST_H_

#include <code\lrfddll.h>

#include <classlib\defs.h> // for bool

#include <except.h>
#include <cstring.h>

class _CODECLASS LLDF
{
public:
   LLDF();
   virtual ~LLDF();

   enum Location {IntGirder,ExtGirder};
   enum NumLoadedLanes {OneLoadedLane,TwoOrMoreLoadedLanes};
   enum LimitState {Service,Strength,Fatigue};
   enum DeckType {Concrete,SteelGrid,FilledGrid,WoodPlank,
                  StressLaminated,SpikeLaminated,GlueLaminated};

   double GetMomentDF(Location loc,NumLoadedLanes numLanes,LimitState ls);
   double GetShearDF(Location loc,NumLoadedLanes numLanes,LimitState ls);

protected:
   double DistributeByLeverRule(double S,double de,double wLane,int Nl);
   double DistributeByStaticalMethod(double S,double de,double wLane,double Nb,int nl);
   double DistributeByStaticalMethod(double S,double de,double wLane,int Nb,int firstLoadedLane,int lastLoadedLane);

   double GetShyDistance() const;
   double GetAxleSpacing() const;
   double GetDistanceToAxle(double S,double de,double wLane,int nbeam,int naxle) const;

   // Override for specific supporting component and deck type
   virtual double GetMomentDF_Int_1_Strength() = 0;
   virtual double GetMomentDF_Int_2_Strength() = 0;
   virtual double GetMomentDF_Int_Fatigue() = 0;
   virtual double GetMomentDF_Ext_1_Strength() = 0;
   virtual double GetMomentDF_Ext_2_Strength() = 0;
   virtual double GetMomentDF_Ext_Fatigue() = 0;

   virtual double GetShearDF_Int_1_Strength() = 0;
   virtual double GetShearDF_Int_2_Strength() = 0;
   virtual double GetShearDF_Int_Fatigue() = 0;
   virtual double GetShearDF_Ext_1_Strength() = 0;
   virtual double GetShearDF_Ext_2_Strength() = 0;
   virtual double GetShearDF_Ext_Fatigue() = 0;
};

// Steel Grids on Steel Beams
class _CODECLASS LLDF_TypeA : public LLDF
{
public:
   LLDF_TypeA(double S,double de,double wLane,double tg);
   ~LLDF_TypeA() {}

protected:
   double S; // spacing of beams or webs (mm)
   double de; // distance between the center of exterior beam and the interior
              // edge of curb or traffic (mm)
   double wLane; // width of design lane (mm)
   double tg; // depth of steel grid or corrugated steel plank (mm)

   double GetMomentDF_Int_1_Strength();
   double GetMomentDF_Int_2_Strength();
   double GetMomentDF_Int_Fatigue();
   double GetMomentDF_Ext_1_Strength();
   double GetMomentDF_Ext_2_Strength();
   double GetMomentDF_Ext_Fatigue();

   double GetShearDF_Int_1_Strength();
   double GetShearDF_Int_2_Strength();
   double GetShearDF_Int_Fatigue();
   double GetShearDF_Ext_1_Strength();
   double GetShearDF_Ext_2_Strength();
   double GetShearDF_Ext_Fatigue();
};

// Concrete Deck on Wood Beams
class _CODECLASS LLDF_TypeL : public LLDF
{
public:
   LLDF_TypeL(double S,double de,double wLane);
   ~LLDF_TypeL() {}

protected:
   double S; // spacing of beams or webs (mm)
   double de; // distance between the center of exterior beam and the interior
              // edge of curb or traffic (mm)
   double wLane; // width of design lane (mm)

   double GetMomentDF_Int_1_Strength();
   double GetMomentDF_Int_2_Strength();
   double GetMomentDF_Int_Fatigue();
   double GetMomentDF_Ext_1_Strength();
   double GetMomentDF_Ext_2_Strength();
   double GetMomentDF_Ext_Fatigue();

   double GetShearDF_Int_1_Strength();
   double GetShearDF_Int_2_Strength();
   double GetShearDF_Int_Fatigue();
   double GetShearDF_Ext_1_Strength();
   double GetShearDF_Ext_2_Strength();
   double GetShearDF_Ext_Fatigue();
};

// Wood Deck on Wood Beams
class _CODECLASS LLDF_TypeAL_Wood : public LLDF
{
public:
   LLDF_TypeAL_Wood(double S,double de,double wLane,DeckType deckType);
   ~LLDF_TypeAL_Wood() {}

protected:
   double S; // spacing of beams or webs (mm)
   double de; // distance between the center of exterior beam and the interior
              // edge of curb or traffic (mm)
   double wLane; // width of design lane (mm)
   DeckType deckType; // deck type

   double GetMomentDF_Int_1_Strength();
   double GetMomentDF_Int_2_Strength();
   double GetMomentDF_Int_Fatigue();
   double GetMomentDF_Ext_1_Strength();
   double GetMomentDF_Ext_2_Strength();
   double GetMomentDF_Ext_Fatigue();

   double GetShearDF_Int_1_Strength();
   double GetShearDF_Int_2_Strength();
   double GetShearDF_Int_Fatigue();
   double GetShearDF_Ext_1_Strength();
   double GetShearDF_Ext_2_Strength();
   double GetShearDF_Ext_Fatigue();
};

// Wood Deck on Steel Beams
class _CODECLASS LLDF_TypeAL_Steel : public LLDF
{
public:
   LLDF_TypeAL_Steel(double S,double de,double wLane,DeckType deckType);
   ~LLDF_TypeAL_Steel() {}

protected:
   double S; // spacing of beams or webs (mm)
   double de; // distance between the center of exterior beam and the interior
              // edge of curb or traffic (mm)
   double wLane; // width of design lane (mm)
   DeckType deckType; // deck type

   double GetMomentDF_Int_1_Strength();
   double GetMomentDF_Int_2_Strength();
   double GetMomentDF_Int_Fatigue();
   double GetMomentDF_Ext_1_Strength();
   double GetMomentDF_Ext_2_Strength();
   double GetMomentDF_Ext_Fatigue();

   double GetShearDF_Int_1_Strength();
   double GetShearDF_Int_2_Strength();
   double GetShearDF_Int_Fatigue();
   double GetShearDF_Ext_1_Strength();
   double GetShearDF_Ext_2_Strength();
   double GetShearDF_Ext_Fatigue();
};

// Concrete deck on Steel or Concrete Beams
class _CODECLASS LLDF_TypeAEKIJ : public LLDF
{
public:
   LLDF_TypeAEKIJ(double S,double de,double wLane,double L,double ts,double Kg,int Nb,
                  double skewAngle = 0,bool skewMoment = false,bool skewShear = false);
   ~LLDF_TypeAEKIJ() {}

   static double ComputeKg(double n,double I,double A,double eg);

protected:
   double S; // spacing of beams or webs (mm)
   double de; // distance between the center of exterior beam and the interior
              // edge of curb or traffic (mm)
   double wLane; // width of design lane (mm)
   double L; // span length (mm)
   double ts; // thickness of slab
   double Kg; // longitudinal stiffness parameter (mm^4)
   int Nb; // number of beams
   double skewAngle; // skew angle
   bool skewMoment; // if true, corrects moment factors for skew
   bool skewShear; // if true, corrects shear factors for skew

   double GetMomentDF_Int_1_Strength();
   double GetMomentDF_Int_2_Strength();
   double GetMomentDF_Int_Fatigue();
   double GetMomentDF_Ext_1_Strength();
   double GetMomentDF_Ext_2_Strength();
   double GetMomentDF_Ext_Fatigue();

   double GetShearDF_Int_1_Strength();
   double GetShearDF_Int_2_Strength();
   double GetShearDF_Int_Fatigue();
   double GetShearDF_Ext_1_Strength();
   double GetShearDF_Ext_2_Strength();
   double GetShearDF_Ext_Fatigue();

   double GetMomentSkewCorrectionFactor();
   double GetShearSkewCorrectionFactor();
};

// Concrete deck on Steel or Concrete Beams with XFrames or Diaphragms
class _CODECLASS LLDF_TypeAEK : public LLDF_TypeAEKIJ
{
public:
   LLDF_TypeAEK(double S,double de,double wLane,double L,double ts,double Kg,int Nb,int Nl,
                bool xFrames = true,
                double skewAngle = 0,bool skewMoment = false,bool skewShear = false);
   ~LLDF_TypeAEK() {}

protected:
   int Nl; // number of design lanes
   bool xFrames; // if true, statical method is used

   double GetMomentDF_Ext_1_Strength();
   double GetMomentDF_Ext_2_Strength();

   double GetShearDF_Ext_1_Strength();
   double GetShearDF_Ext_2_Strength();
};

/*
class _CODECLASS LLDF
{
protected:
   double A; // area of beam (mm2)
   double b; // width of beam (mm)
   double C; // stiffness parameter
   double d; // depth of beam (mm)
   double de; // distance between the center of exterior beam and the interior
              // edge of curb or traffic (mm)
   double D; // width of distribution per lane (mm)
   double eg; // distance between the centers of gravity of the basic beam
              // and deck (mm)
   double I; // moment of interia of beam (mm)
   double Ip; // polar moment of inertia (mm4)
   double J; // St. Venant's torsional constant (mm4)
   double K; // const for different types of distribution
   double Kg; // longitudinal stiffness parameter (mm4)
   double L; // span of beam (mm)
   double n; // modular ratio between deck and beam
   int Nb; // number of beams
   int Nc; // number of cells in a concrete box girder
   int Nl; // number of design lanes
   double S; // spacing of beams or webs (mm)
   double tg; // depth of steel grid or corrugated steel plank (mm)
   double to; // depth of structural overlay (mm)
   double ts; // depth of concrete slab (mm)
   double W; // edge-to-edge width of bridge (mm)
   double We; // half the web spacing, plus the total overhang (mm)
   double theta; // skew angle (degrees)
   double mu; // Poisson's ratio
   int skewCorrectShear;  // true if shear is to be corrected for skew
   int skewCorrectMoment; // true if moment is to be corrected for skew

public:
   LLDF(
   double _A,
   double _b,
   double _C,
   double _d,
   double _de,
   double _D,
   double _eg,
   double _I,
   double _Ip,
   double _J,
   double _K,
   double _L,
   double _n,
   int _Nb,
   int _Nc,
   int _Nl,
   double _S,
   double _tg,
   double _to,
   double _ts,
   double _W,
   double _We,
   double _theta,
   double _mu,
   int correctShear,
   int correctMoment
   );

   virtual ~LLDF();

   enum Location {IntGirder,ExtGirder};
   enum NumLoadedLanes {OneLoadedLane,TwoOrMoreLoadedLanes};
   enum LimitState {Strength,Fatigue};
   enum DeckType {Concrete,SteelGrid,FilledGrid,WoodPlank,
                  StressLaminated,SpikeLaminated,GlueLaminated};

   double GetKg() const {return Kg;}
   virtual double GetMomentDF(Location loc,NumLoadedLanes numLanes,LimitState ls) = 0;
   virtual double GetShearDF(Location loc,NumLoadedLanes numLanes,LimitState ls)  = 0;
   virtual void CheckApplicablity() = 0;

protected:
   virtual double DistributeByLeverRule() = 0;
   virtual double GetMomentSkewCorrectionFactor() = 0;
   virtual double GetShearSkewCorrectionFactor()  = 0;

   double GetShyDistance() const;
   double GetAxleSpacing() const;
   double GetLaneWidth() const;
};

// Live load distribution factors for cross section Type a,e,k,i,j
class _CODECLASS LLDF_aekij : public LLDF
{
public:
   LLDF_aekij(
   double A,
   double de,
   double eg,
   double I,
   double L,
   double n,
   int Nb,
   int Nl,
   double S,
   double ts,
   double theta,
   int correctShear,
   int correctMoment
   );

   void CheckApplicablity();
   double GetMomentDF(Location loc,NumLoadedLanes numLanes,LimitState ls);
   double GetShearDF(Location loc,NumLoadedLanes numLanes,LimitState ls);

protected:
   double DistributeByLeverRule();
   double GetMomentSkewCorrectionFactor();
   double GetShearSkewCorrectionFactor();

   virtual double GetMomentDF_Int_1_Strength();
   virtual double GetMomentDF_Int_1_Fatigue();
   virtual double GetMomentDF_Int_2_Strength();
   virtual double GetMomentDF_Ext_1_Strength();
   virtual double GetMomentDF_Ext_1_Fatigue();
   virtual double GetMomentDF_Ext_2_Strength();

   virtual double GetShearDF_Int_1_Strength();
   virtual double GetShearDF_Int_1_Fatigue();
   virtual double GetShearDF_Int_2_Strength();
   virtual double GetShearDF_Ext_1_Strength();
   virtual double GetShearDF_Ext_1_Fatigue();
   virtual double GetShearDF_Ext_2_Strength();
};

class _CODECLASS LLDF_aek : public LLDF_aekij
{
protected:
   int isXFrames;

public:
   LLDF_aek(
   double A,
   double de,
   double eg,
   double I,
   double L,
   double n,
   int Nb,
   int Nl,
   double S,
   double ts,
   double theta,
   int correctShear,
   int correctMoment,
   int IsXFrames
   );

protected:
   double GetMomentDF_Ext_1_Strength();
   double GetMomentDF_Ext_1_Fatigue();
   double GetMomentDF_Ext_2_Strength();

   double GetShearDF_Ext_1_Fatigue();
   double GetShearDF_Ext_2_Strength();

   double DistributeByStaticalMethod(int nl);
};

// Live load distribution factors for Precast Concrete I or
// Bulb-Tee Sections (Type k)
class _CODECLASS LLDF_PCI : public LLDF_aek
{
public:
   LLDF_PCI(
   double A,
   double de,
   double eg,
   double I,
   double L,
   double n,
   int Nb,
   int Nl,
   double S,
   double ts,
   double theta,
   int correctShear,
   int correctMoment,
   int IsXFrames
   );
};
*/
#endif // LLDIST_H_