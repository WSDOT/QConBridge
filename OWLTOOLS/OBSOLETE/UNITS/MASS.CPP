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

TUnitMass::TUnitMass()
{
   massScale[slug-slug] = 1;
   massScale[lbm-slug] = 1./32.2;
   convFactor = 14593.9029372; // (g/slug)
   exponent = 1;
}

double TUnitMass::Convert(double val,TMassUnit from,TMassUnit to)
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

double TUnitMass::ConvertUS_US(double val,TMassUnit from,TMassUnit to)
{
   return ScaleUS(val,massScale[from-slug],massScale[to-slug],exponent);
}

double TUnitMass::ConvertUS_SI(double val,TMassUnit from,TMassUnit to)
{
   double temp;

   temp = Convert(val,from,slug);
   temp = ConvertUStoSI(val,convFactor,exponent);
   temp = Convert(temp,g,to);

   return temp;
}

double TUnitMass::ConvertSI_US(double val,TMassUnit from,TMassUnit to)
{
   double temp;

   temp = Convert(val,from,g);
   temp = ConvertSItoUS(temp,convFactor,exponent);
   temp = Convert(temp,slug,to);

   return temp;
}

double TUnitMass::ConvertSI_SI(double val,TMassUnit from,TMassUnit to)
{
   return ScaleSI(val,SIscale[from],SIscale[to],exponent);
}

