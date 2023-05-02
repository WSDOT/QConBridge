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


ostream& operator << (ostream &os,TLengthUnit &lu)
{
   switch(lu)
      {
      case am:    os << "am";   break;
      case fm:    os << "fm";   break;
      case pm:    os << "pm";   break;
      case nm:    os << "nm";   break;
      case um:    os << "um";   break;
      case mm:    os << "mm";   break;
      case cm:    os << "cm";   break;
      case m:     os << "m" ;   break;
      case dm:    os << "dm";   break;
      case dam:   os << "dam";  break;
      case hm:    os << "hm";   break;
      case km:    os << "km";   break;
      case Mm:    os << "Mm";   break;
      case Gm:    os << "Gm";   break;
      case Tm:    os << "Tm";   break;
      case inch:  os << "inch"; break;
      case feet:  os << "feet"; break;
      case yard:  os << "yard"; break;
      case mile:  os << "mile"; break;
      }

   return os;
}

ostream& operator << (ostream &os,TAreaUnit &au)
{
   switch(au)
      {
      case sqm:  os << "m^2";  break;
      case sqin: os << "in^2"; break;
      case sqft: os << "ft^2"; break;
      }

   return os;
}

ostream& operator << (ostream &os,TVolumeUnit &vu)
{
   switch(vu)
      {
      case cum:  os << "m^3";  break;
      case cuin: os << "in^3"; break;
      case cuft: os << "ft^3"; break;
      }

   return os;
}

TUnitLength::TUnitLength()
{
   exponent = 1;
   convFactor = 0.0254; // (m/inch)

   // multiply by scale to convert to inches.
   // divide by scale to convert from inches.
   lengthScale[inch-inch] = 1;
   lengthScale[feet-inch] = 1./12.;
   lengthScale[yard-inch] = 1./36.;
   lengthScale[mile-inch] = 1./63360.;
}


double TUnitLength::Convert(double val,TLengthUnit from,TLengthUnit to)
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

double TUnitLength::ConvertUS_US(double val,TLengthUnit from,TLengthUnit to)
{
   double scale1,scale2;

   scale1 = lengthScale[from-inch];
   scale2 = lengthScale[to-inch];

   return ScaleUS(val,scale1,scale2,exponent);
}

double TUnitLength::ConvertUS_SI(double val,TLengthUnit from,TLengthUnit to)
{
   double temp;

   temp = Convert(val,from,inch);
   temp = ConvertUStoSI(temp,convFactor,exponent);
   temp = Convert(temp,m,to);

   return temp;
}

double TUnitLength::ConvertSI_US(double val,TLengthUnit from,TLengthUnit to)
{
   double temp;

   temp = Convert(val,from,m);
   temp = ConvertSItoUS(temp,convFactor,exponent);
   temp = Convert(temp,inch,to);

   return temp;
}

double TUnitLength::ConvertSI_SI(double val,TLengthUnit from,TLengthUnit to)
{
   double scale1,scale2;

   scale1 = SIscale[from];
   scale2 = SIscale[to];

   return ScaleSI(val,scale1,scale2,exponent);
}

//
//
//
TUnitArea::TUnitArea() :
TUnitLength()
{
   exponent = 2;
}

double TUnitArea::Convert(double val,TAreaUnit from,TAreaUnit to)
{
   return TUnitLength::Convert(val,GetLengthUnit(from),GetLengthUnit(to));
}


TLengthUnit TUnitArea::GetLengthUnit(TAreaUnit au)
{
   TLengthUnit lu;
   switch(au)
      {
      case sqm:  lu = m;    break;
      case sqin: lu = inch; break;
      case sqft: lu = feet; break;
      }

   return lu;
}

TUnitVolume::TUnitVolume() :
TUnitLength()
{
   exponent = 3;
}

TLengthUnit TUnitVolume::GetLengthUnit(TVolumeUnit vu)
{
   TLengthUnit lu;
   switch(vu)
      {
      case cum:  lu = m;    break;
      case cuin: lu = inch; break;
      case cuft: lu = feet; break;
      }

   return lu;
}

double TUnitVolume::Convert(double val,TVolumeUnit from,TVolumeUnit to)
{
   return TUnitLength::Convert(val,GetLengthUnit(from),GetLengthUnit(to));
}

