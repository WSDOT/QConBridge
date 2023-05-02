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

#include <code\lldist.h>
#include <code\lrfd_eqn.h>
#include <code\version.h>
#include <math.h>
#include <stdlib.h>
#include <checks.h>

// Threshold levels
#define ExecutionTrace   0
#define CalcTrace        1
DIAG_DEFINE_GROUP(LiveLoadDist,1,CalcTrace);

LLDF::LLDF()
{
}

LLDF::~LLDF()
{
}

double
LLDF::GetMomentDF(Location loc,NumLoadedLanes numLanes,LimitState ls)
{
   double g;

   switch(loc)
      {
      case IntGirder:
           switch(numLanes)
              {
              case OneLoadedLane:
                   switch(ls)
                      {
                      case Strength:
                      case Service:
                           g = GetMomentDF_Int_1_Strength();
                           break;

                      case Fatigue:
                           g = GetMomentDF_Int_Fatigue();
                           break;
                      }
                   break;

              case TwoOrMoreLoadedLanes:
                   switch(ls)
                      {
                      case Strength:
                      case Service:
                           g = GetMomentDF_Int_2_Strength();
                           break;

                      case Fatigue:
                           // throw an exception
                           break;
                      }
                   break;
              }
           break;

      case ExtGirder:
           switch(numLanes)
              {
              case OneLoadedLane:
                   switch(ls)
                      {
                      case Strength:
                      case Service:
                           g = GetMomentDF_Ext_1_Strength();
                           break;

                      case Fatigue:
                           g = GetMomentDF_Ext_Fatigue();
                           break;
                      }
                   break;

              case TwoOrMoreLoadedLanes:
                   switch(ls)
                      {
                      case Strength:
                      case Service:
                           g = GetMomentDF_Ext_2_Strength();
                           break;

                      case Fatigue:
                           // throw an exception
                           break;
                      }
                   break;
              }
           break;
      }

   return g;
}

double
LLDF::GetShearDF(Location loc,NumLoadedLanes numLanes,LimitState ls)
{
   double g;

   switch(loc)
      {
      case IntGirder:
           switch(numLanes)
              {
              case OneLoadedLane:
                   switch(ls)
                      {
                      case Strength:
                      case Service:
                           g = GetShearDF_Int_1_Strength();
                           break;

                      case Fatigue:
                           g = GetShearDF_Int_Fatigue();
                           break;
                      }
                   break;

              case TwoOrMoreLoadedLanes:
                   switch(ls)
                      {
                      case Strength:
                      case Service:
                           g = GetShearDF_Int_2_Strength();
                           break;

                      case Fatigue:
                           // throw an exception
                           break;
                      }
                   break;
              }
           break;

      case ExtGirder:
           switch(numLanes)
              {
              case OneLoadedLane:
                   switch(ls)
                      {
                      case Strength:
                      case Service:
                           g = GetShearDF_Ext_1_Strength();
                           break;

                      case Fatigue:
                           g = GetShearDF_Ext_Fatigue();
                           break;
                      }
                   break;

              case TwoOrMoreLoadedLanes:
                   switch(ls)
                      {
                      case Strength:
                      case Service:
                           g = GetShearDF_Ext_2_Strength();
                           break;

                      case Fatigue:
                           // throw an exception
                           break;
                      }
                   break;
              }
           break;
      }

   return g;
}

double
LLDF::DistributeByLeverRule(double S,double de,double wLane,int Nl)
{
   double cummd; // cummulative distance from first interior
                 // girder to axle under consideration
   int nAxles; // number of axles for Nl loaded lanes
   int axle;  // counter
   int beamIdx; // index of second beam from the left
   double g;  // distribution factor

   nAxles = Nl*2;
   beamIdx = 1; // second beam, zero-based index
   cummd = 0;
   for (axle = 0; axle < nAxles; axle++)
      {
      double d = GetDistanceToAxle(S,de,wLane,beamIdx,axle);
      cummd += (d > 0) ? d : 0;
      }

   g = ::GetMultiplePresenceFactor(Nl)*cummd/(2*S); // per lanes (includes multiple presence factor)

   return g;
}

double
LLDF::DistributeByStaticalMethod(double S,double de,double wLane,double Nb,int nl)
{
// See Section 4.6.2.2.2d

   double Xext; // horizontal distance from the center of gravity of the
                // pattern of girders to the exterior girder (mm)
   double x; // horizontal distance from the center of gravity of the
             // pattern of girders to each girder (mm)
   double e; // eccentricity of a lane from the center of gravity of the
             // pattern of girders (mm)
   double R; // Reaction on exterior beam in terms of lanes
   double cg; // location of center of gravity of the pattern of girders from
              // the left curb line
   double sumx2; // Summation of x^2
   int i; // a general purpose counter

   cg = (S*(Nb-1) + 2.0*de)/2.0;

   sumx2 = 0;
   for (i = 0; i < Nb; i++)
      {
      x = cg - de - i*S;
      sumx2 += pow(x,2);
      }

   Xext = cg - de;

   e = 0;
   for (i = 0; i < nl; i++)
      {
      e += cg - GetShyDistance() - 0.5*GetAxleSpacing() - i*wLane;
      }

   R = ((double)nl)/((double)Nb) + Xext*e/sumx2;
   R *= ::GetMultiplePresenceFactor(nl);

   return R;
}

double
LLDF::DistributeByStaticalMethod(double S,double de,double wLane,int Nb,int firstLoadedLane,int lastLoadedLane)
{
   int loadedLane;
   double g = 0;

   for (loadedLane = firstLoadedLane; loadedLane <= lastLoadedLane; loadedLane++)
      {
      g = max(g,DistributeByStaticalMethod(S,de,wLane,Nb,loadedLane));
      }

   return g;
}

double
LLDF::GetShyDistance() const
{
   return 600.; // mm - LRFD 3.6.1.3.1
}

double
LLDF::GetAxleSpacing() const
{
   return 1800.; // mm - LRFD Figure 3.6.1.2.2-1
}

double
LLDF::GetDistanceToAxle(double S,double de,double wLane,int nbeam,int naxle) const
{
   // nbeam and naxle are zero-based beam and axle indices, respectively,
   // counted from the left side of the cross-section, looking ahead on
   // station.
   double shy; // shy distance
   double axleSpacing; // axle spacing
   double d; // distance from nbeam to naxle

   shy = GetShyDistance();
   axleSpacing = GetAxleSpacing();

   d = nbeam*S + de - shy - naxle*axleSpacing - (naxle/2)*wLane/2;
   // NOTE: naxles/2 must return an int

   return d;
}

//
// --------------------------------------------------------------------------
//

LLDF_TypeA::LLDF_TypeA(double S,double de,double wLane,double tg) :
LLDF(),
S(S),de(de),wLane(wLane),tg(tg)
{
}

double
LLDF_TypeA::GetMomentDF_Int_1_Strength()
{
   double g;
   if (S > 1800.)
      g = DistributeByLeverRule(S,de,wLane,1);
   else
      g = (tg < 100.) ? S/2300. : S/3050.;

   return g;
}

double
LLDF_TypeA::GetMomentDF_Int_2_Strength()
{
   double g;
   if (S > 3200.)
      g = DistributeByLeverRule(S,de,wLane,2);
   else
      g = (tg < 100.) ? S/2400. : S/3050.;

   return g;
}

double
LLDF_TypeA::GetMomentDF_Int_Fatigue()
{
   return GetMomentDF_Int_1_Strength()/::GetMultiplePresenceFactor(1);
}

double
LLDF_TypeA::GetMomentDF_Ext_1_Strength()
{
   return DistributeByLeverRule(S,de,wLane,1);
}

double
LLDF_TypeA::GetMomentDF_Ext_2_Strength()
{
   return DistributeByLeverRule(S,de,wLane,2);
}

double
LLDF_TypeA::GetMomentDF_Ext_Fatigue()
{
   return GetMomentDF_Ext_1_Strength()/::GetMultiplePresenceFactor(1);
}

double
LLDF_TypeA::GetShearDF_Int_1_Strength()
{
   return DistributeByLeverRule(S,de,wLane,1);
}

double
LLDF_TypeA::GetShearDF_Int_2_Strength()
{
   return DistributeByLeverRule(S,de,wLane,2);
}

double
LLDF_TypeA::GetShearDF_Int_Fatigue()
{
   return GetShearDF_Int_1_Strength()/::GetMultiplePresenceFactor(1);
}

double
LLDF_TypeA::GetShearDF_Ext_1_Strength()
{
   return DistributeByLeverRule(S,de,wLane,1);
}

double
LLDF_TypeA::GetShearDF_Ext_2_Strength()
{
   return DistributeByLeverRule(S,de,wLane,2);
}

double
LLDF_TypeA::GetShearDF_Ext_Fatigue()
{
   return GetShearDF_Ext_1_Strength()/::GetMultiplePresenceFactor(1);
}

//
// --------------------------------------------------------------------------
//

LLDF_TypeL::LLDF_TypeL(double S,double de,double wLane) :
LLDF(),
S(S),de(de),wLane(wLane)
{
}

double
LLDF_TypeL::GetMomentDF_Int_1_Strength()
{
   double g;

   if (S > 1800.)
      g = DistributeByLeverRule(S,de,wLane,1);
   else
      g = S/3700.;

   return g;
}

double
LLDF_TypeL::GetMomentDF_Int_2_Strength()
{
   double g;

   if (S > 1800.)
      g = DistributeByLeverRule(S,de,wLane,1);
   else
      g = S/3000.;

   return g;
}

double
LLDF_TypeL::GetMomentDF_Int_Fatigue()
{
   return GetMomentDF_Int_1_Strength()/::GetMultiplePresenceFactor(1);
}

double
LLDF_TypeL::GetMomentDF_Ext_1_Strength()
{
   return DistributeByLeverRule(S,de,wLane,1);
}

double
LLDF_TypeL::GetMomentDF_Ext_2_Strength()
{
   return DistributeByLeverRule(S,de,wLane,2);
}

double
LLDF_TypeL::GetMomentDF_Ext_Fatigue()
{
   return GetMomentDF_Ext_1_Strength()/::GetMultiplePresenceFactor(1);
}

double
LLDF_TypeL::GetShearDF_Int_1_Strength()
{
   return DistributeByLeverRule(S,de,wLane,1);
}

double
LLDF_TypeL::GetShearDF_Int_2_Strength()
{
   return DistributeByLeverRule(S,de,wLane,2);
}

double
LLDF_TypeL::GetShearDF_Int_Fatigue()
{
   return GetShearDF_Int_1_Strength()/::GetMultiplePresenceFactor(1);
}

double
LLDF_TypeL::GetShearDF_Ext_1_Strength()
{
   return DistributeByLeverRule(S,de,wLane,1);
}

double
LLDF_TypeL::GetShearDF_Ext_2_Strength()
{
   return DistributeByLeverRule(S,de,wLane,2);
}

double
LLDF_TypeL::GetShearDF_Ext_Fatigue()
{
   return GetShearDF_Ext_1_Strength()/::GetMultiplePresenceFactor(1);
}

//
// --------------------------------------------------------------------------
//

LLDF_TypeAL_Wood::LLDF_TypeAL_Wood(double S,double de,double wLane,DeckType deckType) :
LLDF(),
S(S),de(de),wLane(wLane),deckType(deckType)
{
}

double
LLDF_TypeAL_Wood::GetMomentDF_Int_1_Strength()
{
   double N; // N, as in S/N
   double Smax; // If S exceeds this value, use the lever rule
   double g;

   switch(deckType)
      {
      case WoodPlank:         N = 2000.;  Smax = 1500.;  break;
      case StressLaminated:   N = 2800.;  Smax = 1800.;  break;
      case SpikeLaminated:    N = 2500.;  Smax = 1800.;  break;
      case GlueLaminated:     N = 3000.;  Smax = 1800.;  break;
      }

   if (S > Smax)
      g = DistributeByLeverRule(S,de,wLane,1);
   else
      g = S/N;

   return g;
}

double
LLDF_TypeAL_Wood::GetMomentDF_Int_2_Strength()
{
   double N; // N, as in S/N
   double Smax; // If S exceeds this value, use the lever rule
   double g;

   switch(deckType)
      {
      case WoodPlank:         N = 2300.;  Smax = 1500.;  break;
      case StressLaminated:   N = 2700.;  Smax = 1800.;  break;
      case SpikeLaminated:    N = 2600.;  Smax = 1800.;  break;
      case GlueLaminated:     N = 3000.;  Smax = 1800.;  break;
      }

   if (S > Smax)
      g = DistributeByLeverRule(S,de,wLane,2);
   else
      g = S/N;

   return g;
}

double
LLDF_TypeAL_Wood::GetMomentDF_Int_Fatigue()
{
   return GetMomentDF_Int_1_Strength()/::GetMultiplePresenceFactor(1);
}

double
LLDF_TypeAL_Wood::GetMomentDF_Ext_1_Strength()
{
   return DistributeByLeverRule(S,de,wLane,1);
}

double
LLDF_TypeAL_Wood::GetMomentDF_Ext_2_Strength()
{
   return DistributeByLeverRule(S,de,wLane,2);
}

double
LLDF_TypeAL_Wood::GetMomentDF_Ext_Fatigue()
{
   return GetMomentDF_Ext_1_Strength()/::GetMultiplePresenceFactor(1);
}

double
LLDF_TypeAL_Wood::GetShearDF_Int_1_Strength()
{
   double N; // N, as in S/N
   double Smax; // If S exceeds this value, use the lever rule
   double g;

   switch(deckType)
      {
      case WoodPlank:         N = 2000.;  Smax = 1500.;  break;
      case StressLaminated:   N = 2800.;  Smax = 1800.;  break;
      case SpikeLaminated:    N = 2500.;  Smax = 1800.;  break;
      case GlueLaminated:     N = 3000.;  Smax = 1800.;  break;
      }

   if (S > Smax)
      g = DistributeByLeverRule(S,de,wLane,1);
   else
      g = S/N;

   return g;
}

double
LLDF_TypeAL_Wood::GetShearDF_Int_2_Strength()
{
   double N; // N, as in S/N
   double Smax; // If S exceeds this value, use the lever rule
   double g;

   switch(deckType)
      {
      case WoodPlank:         N = 2300.;  Smax = 1500.;  break;
      case StressLaminated:   N = 2700.;  Smax = 1800.;  break;
      case SpikeLaminated:    N = 2600.;  Smax = 1800.;  break;
      case GlueLaminated:     N = 3000.;  Smax = 1800.;  break;
      }

   if (S > Smax)
      g = DistributeByLeverRule(S,de,wLane,2);
   else
      g = S/N;

   return g;
}

double
LLDF_TypeAL_Wood::GetShearDF_Int_Fatigue()
{
   return GetShearDF_Int_1_Strength()/::GetMultiplePresenceFactor(1);
}

double
LLDF_TypeAL_Wood::GetShearDF_Ext_1_Strength()
{
   return DistributeByLeverRule(S,de,wLane,1);
}

double
LLDF_TypeAL_Wood::GetShearDF_Ext_2_Strength()
{
   return DistributeByLeverRule(S,de,wLane,2);
}

double
LLDF_TypeAL_Wood::GetShearDF_Ext_Fatigue()
{
   return GetShearDF_Ext_1_Strength()/::GetMultiplePresenceFactor(1);
}

//
// --------------------------------------------------------------------------
//

LLDF_TypeAL_Steel::LLDF_TypeAL_Steel(double S,double de,double wLane,DeckType deckType) :
LLDF(),
S(S),de(de),wLane(wLane),deckType(deckType)
{
}

double
LLDF_TypeAL_Steel::GetMomentDF_Int_1_Strength()
{
   double N; // N, as in S/N
   double Smax; // If S exceeds this value, use the lever rule
   double g;

   switch(deckType)
      {
      case StressLaminated:   N = 2800.;  Smax = 1800.;  break;
      case SpikeLaminated:    N = 2500.;  Smax = 1800.;  break;
      case GlueLaminated:     N = 2670.;  Smax = 1800.;  break;
      }

   if (S > Smax)
      g = DistributeByLeverRule(S,de,wLane,1);
   else
      g = S/N;

   return g;
}

double
LLDF_TypeAL_Steel::GetMomentDF_Int_2_Strength()
{
   double N; // N, as in S/N
   double Smax; // If S exceeds this value, use the lever rule
   double g;

   switch(deckType)
      {
      case StressLaminated:   N = 2700.;  Smax = 1800.;  break;
      case SpikeLaminated:    N = 2600.;  Smax = 1800.;  break;
      case GlueLaminated:     N = 2700.;  Smax = 1800.;  break;
      }

   if (S > Smax)
      g = DistributeByLeverRule(S,de,wLane,2);
   else
      g = S/N;

   return g;
}

double
LLDF_TypeAL_Steel::GetMomentDF_Int_Fatigue()
{
   return GetMomentDF_Int_1_Strength()/::GetMultiplePresenceFactor(1);
}

double
LLDF_TypeAL_Steel::GetMomentDF_Ext_1_Strength()
{
   return DistributeByLeverRule(S,de,wLane,1);
}

double
LLDF_TypeAL_Steel::GetMomentDF_Ext_2_Strength()
{
   return DistributeByLeverRule(S,de,wLane,2);
}

double
LLDF_TypeAL_Steel::GetMomentDF_Ext_Fatigue()
{
   return GetMomentDF_Ext_1_Strength()/::GetMultiplePresenceFactor(1);
}

double
LLDF_TypeAL_Steel::GetShearDF_Int_1_Strength()
{
   double N; // N, as in S/N
   double Smax; // If S exceeds this value, use the lever rule
   double g;

   switch(deckType)
      {
      case StressLaminated:   N = 2800.;  Smax = 1800.;  break;
      case SpikeLaminated:    N = 2500.;  Smax = 1800.;  break;
      case GlueLaminated:     N = 2670.;  Smax = 1800.;  break;
      }

   if (S > Smax)
      g = DistributeByLeverRule(S,de,wLane,1);
   else
      g = S/N;

   return g;
}

double
LLDF_TypeAL_Steel::GetShearDF_Int_2_Strength()
{
   double N; // N, as in S/N
   double Smax; // If S exceeds this value, use the lever rule
   double g;

   switch(deckType)
      {
      case StressLaminated:   N = 2700.;  Smax = 1800.;  break;
      case SpikeLaminated:    N = 2600.;  Smax = 1800.;  break;
      case GlueLaminated:     N = 2700.;  Smax = 1800.;  break;
      }

   if (S > Smax)
      g = DistributeByLeverRule(S,de,wLane,2);
   else
      g = S/N;

   return g;
}

double
LLDF_TypeAL_Steel::GetShearDF_Int_Fatigue()
{
   return GetShearDF_Int_1_Strength()/::GetMultiplePresenceFactor(1);
}

double
LLDF_TypeAL_Steel::GetShearDF_Ext_1_Strength()
{
   return DistributeByLeverRule(S,de,wLane,1);
}

double
LLDF_TypeAL_Steel::GetShearDF_Ext_2_Strength()
{
   return DistributeByLeverRule(S,de,wLane,2);
}

double
LLDF_TypeAL_Steel::GetShearDF_Ext_Fatigue()
{
   return GetShearDF_Ext_1_Strength()/::GetMultiplePresenceFactor(1);
}

//
// --------------------------------------------------------------------------
//

LLDF_TypeAEKIJ::LLDF_TypeAEKIJ(double S,double de,double wLane,double L,double ts,double Kg,int Nb,
                               double skewAngle,bool skewMoment,bool skewShear) :
S(S),de(de),wLane(wLane),L(L),ts(ts),Kg(Kg),Nb(Nb),
skewAngle(skewAngle),skewMoment(skewMoment),skewShear(skewShear)
{
}

double
LLDF_TypeAEKIJ::ComputeKg(double n,double I,double A,double eg)
{
   return n*(I + A*eg*eg);
}

double
LLDF_TypeAEKIJ::GetMomentDF_Int_1_Strength()
{
   double skew;
   double g;

   skew = GetMomentSkewCorrectionFactor();

   if (S > 4900.)
      g = DistributeByLeverRule(S,de,wLane,1);
   else
      g = 0.06 + pow(S/4300.,0.4)*pow(S/L,0.3)*pow(Kg/(L*pow(ts,3)),0.1);

   g *= skew;

   return g;
}

double
LLDF_TypeAEKIJ::GetMomentDF_Int_2_Strength()
{
   double skew;
   double g;

   skew = GetMomentSkewCorrectionFactor();
   if (S > 4900.)
      g = DistributeByLeverRule(S,de,wLane,2);
   else
      g = 0.075 + pow(S/2900.,0.6)*pow(S/L,0.2)*pow(Kg/(L*pow(ts,3)),0.1);

   g *= skew;

   return g;
}

double
LLDF_TypeAEKIJ::GetMomentDF_Int_Fatigue()
{
   return GetMomentDF_Int_1_Strength()/::GetMultiplePresenceFactor(1);
}

double
LLDF_TypeAEKIJ::GetMomentDF_Ext_1_Strength()
{
   double skew;
   double g;

   g = DistributeByLeverRule(S,de,wLane,1);
   skew = GetMomentSkewCorrectionFactor();
   g *= skew;

   return g;
}

double
LLDF_TypeAEKIJ::GetMomentDF_Ext_2_Strength()
{
   double e;
   double g;

   e = max(0.77 + de/2800.,1.0);

   // Skew correction factor is included int GetMomentDF_Int_2_Strength
   g = e*LLDF_TypeAEKIJ::GetMomentDF_Int_2_Strength();

   return g;
}

double
LLDF_TypeAEKIJ::GetMomentDF_Ext_Fatigue()
{
   return GetMomentDF_Ext_1_Strength()/::GetMultiplePresenceFactor(1);
}

double
LLDF_TypeAEKIJ::GetShearDF_Int_1_Strength()
{
   double skew;
   double g;

   skew = GetShearSkewCorrectionFactor();

   if (S > 4900.)
      g = DistributeByLeverRule(S,de,wLane,1);
   else
      g = 0.36 + S/7600.;

   g *= skew;

   return g;
}

double
LLDF_TypeAEKIJ::GetShearDF_Int_2_Strength()
{
   double skew;
   double g;

   skew = GetShearSkewCorrectionFactor();

   if (S > 4900.)
      g = DistributeByLeverRule(S,de,wLane,2);
   else
      g = 0.2 + S/3600. - pow(S/10700.,2.0);

   g *= skew;

   return g;
}

double
LLDF_TypeAEKIJ::GetShearDF_Int_Fatigue()
{
   return GetShearDF_Int_1_Strength()/::GetMultiplePresenceFactor(1);
}

double
LLDF_TypeAEKIJ::GetShearDF_Ext_1_Strength()
{
   double skew;
   double g;

   g = DistributeByLeverRule(S,de,wLane,1);
   skew = GetShearSkewCorrectionFactor();
   g *= skew;

   return g;
}

double
LLDF_TypeAEKIJ::GetShearDF_Ext_2_Strength()
{
   double e;
   double g;

   e = 0.6 + de/3000.;

   // Skew correction factor is included in GetShearDF_Int_2_Strength
   g = e*LLDF_TypeAEKIJ::GetShearDF_Int_2_Strength();

   return g;
}

double
LLDF_TypeAEKIJ::GetShearDF_Ext_Fatigue()
{
   return GetShearDF_Ext_1_Strength()/::GetMultiplePresenceFactor(1);
}

double
LLDF_TypeAEKIJ::GetMomentSkewCorrectionFactor()
{
   double c1;
   double skew;

   if (!skewMoment)
      return 1.0;

   if (skewAngle < 30)
      c1 = 0;
   else
      c1 = 0.25*pow(Kg/(L*pow(ts,3)),0.25)*pow((S/L),0.5);

   double theSkewAngle;
   if ( skewAngle > 60 )
      theSkewAngle = 60;
   else
      theSkewAngle = skewAngle;

   skew = 1. - c1*pow(tan(theSkewAngle*M_PI/180.),1.5);

   return skew;
}

double
LLDF_TypeAEKIJ::GetShearSkewCorrectionFactor()
{
   double skew;

   if (!skewShear)
      return 1.0;

   double theSkewAngle;
   if ( skewAngle > 60 )
      theSkewAngle = 60;
   else
      theSkewAngle = skewAngle;

   skew = 1.0 + 0.20*pow(L*pow(ts,3)/Kg,0.3)*tan(theSkewAngle*M_PI/180.);

   return skew;
}

//
// --------------------------------------------------------------------------
//

LLDF_TypeAEK::LLDF_TypeAEK(double S,double de,double wLane,double L,double ts,double Kg,int Nb,int Nl,
                           bool xFrames,
                           double skewAngle,bool skewMoment,bool skewShear) :
LLDF_TypeAEKIJ(S,de,wLane,L,ts,Kg,Nb,skewAngle,skewMoment,skewShear),
Nl(Nl),
xFrames(xFrames)
{
}

double
LLDF_TypeAEK::GetMomentDF_Ext_1_Strength()
{
   double skew;
   double g1 = 0;
   double g2 = 0;

   g1 = LLDF_TypeAEKIJ::GetMomentDF_Ext_1_Strength(); // accounts for skew

   if (xFrames)
      {
      // but not less than that which would be obtained by assuming that the
      // cross-section deflects and rotates as a rigid cross-section. 4.6.2.2.2d
      skew = GetMomentSkewCorrectionFactor();
      g2 = skew*DistributeByStaticalMethod(S,de,wLane,Nb,1,1);
      }

   return max(g1,g2);
}

double
LLDF_TypeAEK::GetMomentDF_Ext_2_Strength()
{
   double g1 = 0;
   double g2 = 0;
   double skew;

   g1 = LLDF_TypeAEKIJ::GetMomentDF_Ext_2_Strength();

   if (xFrames)
      {
      // but not less than that which would be obtained by assuming that the
      // cross-section deflects and rotates as a rigid cross-section. 4.6.2.2.2d
      skew = GetMomentSkewCorrectionFactor();
      g2 = skew*DistributeByStaticalMethod(S,de,wLane,Nb,2,Nl);
      }

   return max(g1,g2);
}

double
LLDF_TypeAEK::GetShearDF_Ext_1_Strength()
{
   double skew;
   double g1 = 0;
   double g2 = 0;

   g1 = LLDF_TypeAEKIJ::GetShearDF_Ext_1_Strength(); // accounts for skew

   if (xFrames)
      {
      // but not less than that which would be obtained by assuming that the
      // cross-section deflects and rotates as a rigid cross-section. 4.6.2.2.2d
      skew = GetMomentSkewCorrectionFactor();
      g2 = skew*DistributeByStaticalMethod(S,de,wLane,Nb,1,1);
      }

   return max(g1,g2);
}

double
LLDF_TypeAEK::GetShearDF_Ext_2_Strength()
{
   double g1 = 0;
   double g2 = 0;
   double skew;

   g1 = LLDF_TypeAEKIJ::GetShearDF_Ext_2_Strength();

   if (xFrames)
      {
      // but not less than that which would be obtained by assuming that the
      // cross-section deflects and rotates as a rigid cross-section. 4.6.2.2.2d
      skew = GetShearSkewCorrectionFactor();
      g2 = skew*DistributeByStaticalMethod(S,de,wLane,Nb,2,Nl);
      }

   return max(g1,g2);
}

//
// --------------------------------------------------------------------------
//

//Cut and paste for new types. These are the basic methods required

/*
LLDF_TypeL::LLDF_TypeL(double S,double de,double wLane) :
LLDF(),
S(S),de(de),wLane(wLane)
{
}

double
LLDF_TypeL::GetMomentDF_Int_1_Strength()
{
}

double
LLDF_TypeL::GetMomentDF_Int_2_Strength()
{
}

double
LLDF_TypeL::GetMomentDF_Int_Fatigue()
{
}

double
LLDF_TypeL::GetMomentDF_Ext_1_Strength()
{
}

double
LLDF_TypeL::GetMomentDF_Ext_2_Strength()
{
}

double
LLDF_TypeL::GetMomentDF_Ext_Fatigue()
{
}

double
LLDF_TypeL::GetShearDF_Int_1_Strength()
{
}

double
LLDF_TypeL::GetShearDF_Int_2_Strength()
{
}

double
LLDF_TypeL::GetShearDF_Int_Fatigue()
{
}

double
LLDF_TypeL::GetShearDF_Ext_1_Strength()
{
}

double
LLDF_TypeL::GetShearDF_Ext_2_Strength()
{
}

double
LLDF_TypeL::GetShearDF_Ext_Fatigue()
{
}
*/
//
// --------------------------------------------------------------------------
//

