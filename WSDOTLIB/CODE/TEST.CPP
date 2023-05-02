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

#include "code\lrfdcode.h"
#include <iostream.h>
#include <iomanip.h>

void ReportDistFactors(ostream& os,LLDF& lldf);

void main(void)
{
//   LoadCombiner *loadCombiner = new LoadCombiner;
//   loadCombiner->CombineLoads(LoadCombiner::StrengthI);
   LoadCombiner loadCombiner;
   loadCombiner.CombineLoads(LoadCombiner::StrengthI);

   double fc = 27.5;
   double density = 2560.;
   cout << "Fc = " << fc << " MPa" << endl;
   cout << "Density = " << density << " kg/m^3" << endl;
   cout << "EConcrete = " << EConcrete(density,fc) << " MPa" << endl;

   double effLength = 100.;
   double spacing = 6.;
   double tSlab = 0.5833;
   double tWeb = 0.5;
   double wTopFlange = 6.;
   double overhang = 1.5;
   cout << "effLength  = " << effLength << " feet" << endl;
   cout << "spacing    = " << spacing << " feet" << endl;
   cout << "tSlab      = " << tSlab << " feet" << endl;
   cout << "tWeb       = " << tWeb << " feet" << endl;
   cout << "wTopFlange = " << wTopFlange << " feet" << endl;
   cout << "overhang   = " << overhang << " feet" << endl;
   cout << "Interior Girder - EffectiveSlabWidth = "
        << IBeam_EffectiveSlabWidth(effLength,spacing,tSlab,tWeb,wTopFlange,overhang,1) << endl;
   cout << "Exterior Girder - EffectiveSlabWidth = "
        << IBeam_EffectiveSlabWidth(effLength,spacing,tSlab,tWeb,wTopFlange,overhang,0) << endl;

   double nd = 0.95;
   double nr = 0.95;
   double ni = 1.05;
   cout << "nd = " << nd << endl;
   cout << "nr = " << nr << endl;
   cout << "ni = " << ni << endl;
   cout << "Limit State Factor - Max Load Factor " << LimitStateFactor(nd,nr,ni,true) << endl;
   cout << "Limit State Factor - Min Load Factor " << LimitStateFactor(nd,nr,ni,false) << endl;

   cout << "Multiple Presence Factor for 1 Loaded Lane " << GetMultiplePresenceFactor(1) << endl;
   cout << "Multiple Presence Factor for 2 Loaded Lane " << GetMultiplePresenceFactor(2) << endl;
   cout << "Multiple Presence Factor for 3 Loaded Lane " << GetMultiplePresenceFactor(3) << endl;
   cout << "Multiple Presence Factor for 4 Loaded Lane " << GetMultiplePresenceFactor(4) << endl;
   cout << "Multiple Presence Factor for 5 Loaded Lane " << GetMultiplePresenceFactor(5) << endl;

   cout << "Design Lane Width for 3500mm Curb To Curb Width " << GetStdDesignLaneWidth(3500.) << endl;
   cout << "Design Lane Width for 3700mm Curb To Curb Width " << GetStdDesignLaneWidth(3700.) << endl;
   cout << "Design Lane Width for 6200mm Curb To Curb Width " << GetStdDesignLaneWidth(6200.) << endl;
   cout << "Design Lane Width for 7500mm Curb To Curb Width " << GetStdDesignLaneWidth(7500.) << endl;


   LLDF_TypeA typeA90(1200,910,3600,90);
   cout << "Steel Grids on Steel Beams (Type A)" << endl;
   cout << "tg < 100mm" << endl;
   ReportDistFactors(cout,typeA90);

   LLDF_TypeA typeA110(1200,910,3600,110);
   cout << "Steel Grids on Steel Beams (Type A)" << endl;
   cout << "tg >= 100mm" << endl;
   ReportDistFactors(cout,typeA110);

   LLDF_TypeL typeL(1200,910,3600);
   cout << "Concrete Deck on Wood Beams (Type L)" << endl;
   ReportDistFactors(cout,typeL);

   LLDF_TypeAL_Wood typeAL_wood_plank(1200,910,3600,LLDF::WoodPlank);
   cout << "Wood Deck on Wood Beams (Wood Plank)" << endl;
   ReportDistFactors(cout,typeAL_wood_plank);

   LLDF_TypeAL_Wood typeAL_wood_stressLaminated(1200,910,3600,LLDF::StressLaminated);
   cout << "Wood Deck on Wood Beams (Stress Laminated)" << endl;
   ReportDistFactors(cout,typeAL_wood_stressLaminated);

   LLDF_TypeAL_Wood typeAL_wood_spikeLaminated(1200,910,3600,LLDF::SpikeLaminated);
   cout << "Wood Deck on Wood Beams (Spike Laminated)" << endl;
   ReportDistFactors(cout,typeAL_wood_spikeLaminated);

   LLDF_TypeAL_Wood typeAL_wood_glueLaminated(1200,910,3600,LLDF::GlueLaminated);
   cout << "Wood Deck on Wood Beams (Glue Laminated)" << endl;
   ReportDistFactors(cout,typeAL_wood_glueLaminated);

   LLDF_TypeAL_Steel typeAL_steel_stressLaminated(1200,910,3600,LLDF::StressLaminated);
   cout << "Wood Deck on Steel Beams (Stress Laminated)" << endl;
   ReportDistFactors(cout,typeAL_steel_stressLaminated);

   LLDF_TypeAL_Steel typeAL_steel_spikeLaminated(1200,910,3600,LLDF::SpikeLaminated);
   cout << "Wood Deck on Steel Beams (Spike Laminated)" << endl;
   ReportDistFactors(cout,typeAL_steel_spikeLaminated);

   LLDF_TypeAL_Steel typeAL_steel_glueLaminated(1200,910,3600,LLDF::GlueLaminated);
   cout << "Wood Deck on Steel Beams (Glue Laminated)" << endl;
   ReportDistFactors(cout,typeAL_steel_glueLaminated);

   LLDF_TypeAEK typeAEK(3660,910,3600,43000,240,5.1816e11,4,3);
   cout << "LRFD Class example" << endl;
   ReportDistFactors(cout,typeAEK);
}

void ReportDistFactors(ostream& os,LLDF& lldf)
{
   os << "*** Strength/Service Limit States ***" << endl;
   os << "Interior Girders" << endl;
   os << "gMoment - 1  Loaded Lane " << lldf.GetMomentDF(LLDF::IntGirder,LLDF::OneLoadedLane,LLDF::Strength) << endl;
   os << "gMoment - 2+ Loaded Lane " << lldf.GetMomentDF(LLDF::IntGirder,LLDF::TwoOrMoreLoadedLanes,LLDF::Strength) << endl;
   os << "gShear  - 1  Loaded Lane " << lldf.GetShearDF(LLDF::IntGirder,LLDF::OneLoadedLane,LLDF::Strength) << endl;
   os << "gShear  - 2+ Loaded Lane " << lldf.GetShearDF(LLDF::IntGirder,LLDF::TwoOrMoreLoadedLanes,LLDF::Strength) << endl;
   os << endl << "Exterior Girders" << endl;
   os << "gMoment - 1  Loaded Lane " << lldf.GetMomentDF(LLDF::ExtGirder,LLDF::OneLoadedLane,LLDF::Strength) << endl;
   os << "gMoment - 2+ Loaded Lane " << lldf.GetMomentDF(LLDF::ExtGirder,LLDF::TwoOrMoreLoadedLanes,LLDF::Strength) << endl;
   os << "gShear  - 1  Loaded Lane " << lldf.GetShearDF(LLDF::ExtGirder,LLDF::OneLoadedLane,LLDF::Strength) << endl;
   os << "gShear  - 2+ Loaded Lane " << lldf.GetShearDF(LLDF::ExtGirder,LLDF::TwoOrMoreLoadedLanes,LLDF::Strength) << endl;

   os << endl;
   os << "*** Fatigue Limit States ***" << endl;
   os << "Interior Girders" << endl;
   os << "gMoment                  " << lldf.GetMomentDF(LLDF::IntGirder,LLDF::OneLoadedLane,LLDF::Fatigue) << endl;
   os << "gShear                   " << lldf.GetShearDF(LLDF::IntGirder,LLDF::OneLoadedLane,LLDF::Fatigue) << endl;
   os << endl << "Exterior Girders" << endl;
   os << "gMoment                  " << lldf.GetMomentDF(LLDF::ExtGirder,LLDF::OneLoadedLane,LLDF::Fatigue) << endl;
   os << "gShear                   " << lldf.GetShearDF(LLDF::ExtGirder,LLDF::OneLoadedLane,LLDF::Fatigue) << endl;
}
