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

#if !defined LRFD_EQN_H
#define LRFD_EQN_H

#include <code\lrfddll.h>


#include <classlib\defs.h> // for bool

extern "C" { // Prevent name-mangling
double _CODEFUNC EConcrete(double density,double fc);
double _CODEFUNC IBeam_EffectiveSlabWidth(double effLength,
                                          double spacing,
                                          double tSlab,
                                          double tWeb,
                                          double wTopFlange,
                                          double overHang,
                                          bool   isInterior);
double _CODEFUNC LimitStateFactor(double nd,double nr,double ni,bool maxFactor = true);
double _CODEFUNC GetMultiplePresenceFactor(int numLoadedLanes);
double _CODEFUNC GetStdDesignLaneWidth(double wCurbToCurb);
} // end of extern "C"

#endif // LRFD_EQN_H