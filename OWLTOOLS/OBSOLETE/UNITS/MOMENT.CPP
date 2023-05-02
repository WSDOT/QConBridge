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

ostream& operator << (ostream &os,TMomentUnit &mu)
{
   switch(mu)
      {
      case Nm:      os << "N-m";     break;
      case kNm:     os << "kN-m";    break;
      case inlbs:   os << "in-lbs";  break;
      case ftlbs:   os << "ft-lbs";  break;
      case inkips:  os << "in-kips"; break;
      case ftkips:  os << "ft-kips"; break;
      }

   return os;
}

TUnitMoment::TUnitMoment():
TUnitForce()
{
}

double TUnitMoment::Convert(double val,TMomentUnit from,TMomentUnit to)
{
   TForceUnit fromForce, toForce;
   TLengthUnit fromLength, toLength;
   double temp;

   DecomposeUnit(from,fromForce,fromLength);
   DecomposeUnit(to,toForce,toLength);
   temp = TUnitForce::Convert(val,fromForce,toForce);
   temp = TUnitLength::Convert(temp,fromLength,toLength);

   return temp;
}

void TUnitMoment::DecomposeUnit(TMomentUnit mu,TForceUnit &fu,TLengthUnit &lu)
{
   switch (mu)
      {
      case inlbs:
           fu = lbs;
           lu = inch;
           break;

      case ftlbs:
           fu = lbs;
           lu = feet;
           break;

      case inkips:
           fu = kips;
           lu = inch;
           break;

      case ftkips:
           fu = kips;
           lu = feet;
           break;

      case Nm:
           fu = N;
           lu = m;
           break;

      case kNm:
           fu = kN;
           lu = m;
           break;
      }
}