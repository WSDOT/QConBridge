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

TGenericUnit::TGenericUnit()
{
   SIscale[a] = 1e-18;
   SIscale[f] = 1e-15;
   SIscale[p] = 1e-12;
   SIscale[n] = 1e-9;
   SIscale[u] = 1e-6;
   SIscale[_m] = 1e-3;
   SIscale[c] = 1e-2;
   SIscale[d] = 1e-1;
   SIscale[_base_] = 1;
   SIscale[da] = 1e1;
   SIscale[h] = 1e2;
   SIscale[k] = 1e3;
   SIscale[M] = 1e6;
   SIscale[G] = 1e9;
   SIscale[T] = 1e12;
}

double TGenericUnit::ScaleUS(double val,double scale1,double scale2,double exponent)
{
   return val*pow(1/scale1,exponent)*pow(scale2,exponent);
}

double TGenericUnit::ScaleSI(double val,double scale1,double scale2,double exponent)
{
   return val*pow(scale1,exponent)*pow(1/scale2,exponent);
}

double TGenericUnit::ConvertUStoSI(double val,double convFactor,double exponent)
{
   return val*pow(convFactor,exponent);
}

double TGenericUnit::ConvertSItoUS(double val,double convFactor,double exponent)
{
   return val/pow(convFactor,exponent);
}

