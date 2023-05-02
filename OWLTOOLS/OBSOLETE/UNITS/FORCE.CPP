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
#include <cstring.h>

ostream& operator << (ostream &os,TForceUnit &fu)
{
   switch(fu)
      {
      case aN:   os << "aN";   break;
      case fN:   os << "fN";   break;
      case pN:   os << "pN";   break;
      case nN:   os << "nN";   break;
      case uN:   os << "uN";   break;
      case mN:   os << "mN";   break;
      case cN:   os << "cN";   break;
      case N:    os << "N";    break;
      case dN:   os << "dN";   break;
      case daN:  os << "daN";  break;
      case hN:   os << "hN";   break;
      case kN:   os << "kN";   break;
      case MN:   os << "MN";   break;
      case GN:   os << "GN";   break;
      case TN:   os << "TN";   break;
      case lbs:  os << "lbs";  break;
      case kips: os << "kips"; break;
      }

   return os;
}

TUnitForce::TUnitForce() :
TUnitMass(),TUnitLength(),TUnitTime()
{
   exponent = 1;
   TUnitTime::exponent = -2;
   forceScale[lbs-lbs] = 1.;
   forceScale[kips-lbs] = 1./1000.;
}

double TUnitForce::Convert(double val,TForceUnit from,TForceUnit to)
{
   double temp;

   if (from < 15)
      {
      temp = (to < 15) ? ConvertSI_SI(val,from,to) : ConvertSI_US(val,from,to);
      }
   else
      {
      temp = (to < 15) ? ConvertUS_SI(val,from,to) : ConvertUS_US(val,from,to);
      }

   return temp;
}

double TUnitForce::ConvertUS_US(double val,TForceUnit from,TForceUnit to)
{
   // use TUnitMass::ScaleUS to avoid ambiguity with other base classes
   // (could have used any of the ancestor classes, I chose TUnitMass
   //  because it is the first ancestor)
   return TUnitMass::ScaleUS(val,forceScale[from-lbs],forceScale[to-lbs],exponent);
}

double TUnitForce::ConvertUS_SI(double val,TForceUnit from,TForceUnit to)
{
   double temp;

   temp = Convert(val,from,lbs);
   temp = TUnitMass::Convert(temp,slug,kg);
   temp = TUnitLength::Convert(temp,feet,m);
   temp = TUnitTime::Convert(temp,second,second);
   temp = Convert(temp,N,to);

   return temp;
}

double TUnitForce::ConvertSI_US(double val,TForceUnit from,TForceUnit to)
{
   double temp;

   temp = Convert(val,from,N);
   temp = TUnitMass::Convert(temp,kg,slug);
   temp = TUnitLength::Convert(temp,m,feet);
   temp = TUnitTime::Convert(temp,second,second);
   temp = Convert(temp,lbs,to);

   return temp;
}

double TUnitForce::ConvertSI_SI(double val,TForceUnit from,TForceUnit to)
{
   // use TUnitMass::ScaleUS to avoid ambiguity with other base classes
   return TUnitMass::ScaleSI(val,TUnitMass::SIscale[from],TUnitMass::SIscale[to],exponent);
}

