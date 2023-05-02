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

#include <code\lrfd_eqn.h>
#include <code\version.h>

#include <math.h>
#include <stdlib.h>
#include <checks.h>

// EConcrete
//
// Description:
// Computes the modulus of elasticity of concrete
//
// Code Reference: 1st Edition Article 5.2.4.2
//
// Units: As Noted
double EConcrete(double density /* kg/m^3 */,
                 double fc      /* MPa */)
{
   double E;

   E = 0.043*pow(density,1.5)*sqrt(fc);

   return E; // MPa
}

// IBeam_EffectiveSlabWidth
//
// Description:
// Computes the effective slab width of a slab on IBeam section.
//
// Code Reference: 1st Edition Article 4.6.2.6
//
// Units: Consistant Length Units
double IBeam_EffectiveSlabWidth(double effLength,
                                double spacing,
                                double tSlab,
                                double tWeb,
                                double wTopFlange,
                                double overHang,
                                bool   isInterior)
{
   double w1,w2,w3;
   double wEff;

   //
   // Interior Girder
   //
   w1 = 0.25*effLength;
   w2 = 12.0*tSlab + max(tWeb,0.5*wTopFlange);
   w3 = spacing;

   wEff = min(w1,min(w2,w3));

   if (!isInterior)
      {
      //
      // Exterior Girder
      //
      w1 = 0.125*effLength;
      w2 = 6.0*tSlab + max(0.5*tWeb,0.25*wTopFlange);
      w3 = overHang;

      wEff = min(w1,min(w2,w3)) + wEff/2;
      }

   return wEff;
}

// LimitStateFactor
//
// Description:
// Computes the limit state factor
//
// Code Reference: 1st Edition Article 1.3.2
//                 1st Edition with 1996 Interim Article 1.3.2
//
// Units: None
double LimitStateFactor(double nd, // ductility
                        double nr, // redundancy
                        double ni, // importance
                        bool   maxFactor // for use with maximum load factor
                        )
{
   double n;

   n = nd * ni * nr;

   switch (GetCodeEdition())
      {
      case lrfdFirstEdition94:
           n = (n < 0.95) ? 0.95 : n;
           break;

      case lrfdFirstEdition94_96Interim:
           if (maxFactor)
              {
              n = (n < 0.95) ? 0.95 : n;
              }
           else
              {
              n = 1.0/n;
              n = (n > 1.0) ? 1.0 : n;
              }
           break;

      default:
           #pragma warn -ccc
           CHECKX(0,"LimitStateFactor() - Bad Code Version");
           #pragma warn .ccc
      }

   return n;
}

// GetMultiplePresenceFactor
//
// Description:
// Returns the multiple presence factor for the number
// of loaded lanes provided
//
// Code Reference: 3.6.1.1.2, Table 3.6.1.1.2-1
double GetMultiplePresenceFactor(int numLoadedLanes)
{
   double m;
   switch(numLoadedLanes)
      {
      case 1:    m = 1.20; break;
      case 2:    m = 1.00; break;
      case 3:    m = 0.85; break;
      default:   m = 0.65; break; // 4 or more loaded lanes
      }

   return m;
}

// GetStdDesignLaneWidth
//
// Description:
// Returns the design lane width. Typically this is a 3600mm design
// lane, but special provisions prevail if the curb to curb with
// does not accomodate 2 or more 3600mm lanes.
//
// Code Reference: 3.6.1.1.1
double GetStdDesignLaneWidth(double wCurbToCurb)
{
   const double wStdLane = 3600.; // 3600mm std design lane width
   int nDesignLanes; // number of 3600mm design lanes between curbs
   double wDesignLane; // computed design lane width;

   nDesignLanes = (int)floor(wCurbToCurb/wStdLane);

   if (6000. <= wCurbToCurb && wCurbToCurb <= 7200.)
      wDesignLane = wCurbToCurb/2.;
   else if (nDesignLanes == 0)
      wDesignLane = wCurbToCurb;
   else
      wDesignLane = wStdLane;

   CHECKX(wDesignLane >= 1800.,"GetStdDesignLaneWidth() - Lane with is less than axle spacing");

   return wDesignLane;
}

