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

#include "units.h"

ostream& operator << (ostream &os,TPressureUnit &pu)
{
   switch(pu)
      {
      case aPa:  os << "aPa";   break;
      case fPa:  os << "fPa";   break;
      case pPa:  os << "pPa";   break;
      case nPa:  os << "nPa";   break;
      case uPa:  os << "uPa";   break;
      case mPa:  os << "mPa";   break;
      case cPa:  os << "cPa";   break;
      case Pa:   os << "Pa";    break;
      case dPa:  os << "dPa";   break;
      case daPa: os << "daPa";  break;
      case hPa:  os << "hPa";   break;
      case kPa:  os << "kPa";   break;
      case MPa:  os << "MPa";   break;
      case GPa:  os << "GPa";   break;
      case TPa:  os << "TPa";   break;
      case psi:  os << "psi";   break;
      case psf:  os << "psf";   break;
      case ksi:  os << "ksi";   break;
      case ksf:  os << "ksf";   break;
      }

   return os;
}

TUnitPressure::TUnitPressure():
TUnitForce(),TUnitArea()
{
   TUnitForce::exponent = 1;
   TUnitArea::exponent = -2;
}

double TUnitPressure::Convert(double val,TPressureUnit from,TPressureUnit to)
{
   TForceUnit fromForce, toForce;
   TAreaUnit  fromArea, toArea;
   double temp;

   DecomposeUnit(from,fromForce,fromArea);
   DecomposeUnit(to,toForce,toArea);
   temp = TUnitForce::Convert(val,fromForce,toForce);
   temp = TUnitArea::Convert(temp,fromArea,toArea);

   return temp;
}

void TUnitPressure::DecomposeUnit(TPressureUnit pu,TForceUnit &fu,TAreaUnit &au)
{
   switch (pu)
      {
      case psi:
           fu = lbs;
           au = sqin;
           break;

      case psf:
           fu = lbs;
           au = sqft;
           break;

      case ksi:
           fu = kips;
           au = sqin;
           break;

      case ksf:
           fu = kips;
           au = sqft;
           break;

      case aPa:
           fu = aN;
           au = sqm;
           break;

      case fPa:
           fu = fN;
           au = sqm;
           break;

      case pPa:
           fu = pN;
           au = sqm;
           break;

      case nPa:
           fu = nN;
           au = sqm;
           break;

      case uPa:
           fu = uN;
           au = sqm;
           break;

      case mPa:
           fu = mN;
           au = sqm;
           break;

      case cPa:
           fu = cN;
           au = sqm;
           break;

      case Pa:
           fu = N;
           au = sqm;
           break;

      case dPa:
           fu = dN;
           au = sqm;
           break;

      case daPa:
           fu = daN;
           au = sqm;
           break;

      case hPa:
           fu = hN;
           au = sqm;
           break;

      case kPa:
           fu = kN;
           au = sqm;
           break;

      case MPa:
           fu = MN;
           au = sqm;
           break;

      case GPa:
           fu = GN;
           au = sqm;
           break;

      case TPa:
           fu = TN;
           au = sqm;
           break;
      }
}

