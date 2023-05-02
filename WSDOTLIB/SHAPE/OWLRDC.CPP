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

#include "owlrdc.hpp"

TOWLRealDC::TOWLRealDC(HDC hdc) :
TRealDC(hdc),
TDC(hdc)
{
}
/*
TOWLRealDC::TOWLRealDC(TOWLRealDC &rdc) :
TRealDC(rdc.dc),
TDC(rdc.dc)
{
}

TOWLRealDC::TOWLRealDC(TRealDC &rdc) :
TRealDC(rdc.dc),
TDC(rdc.dc)
{
}
*/
bool TOWLRealDC::Polygon(const TFPointContainer &points)
{
   TFPoint *p2;
   int i;
   int numPnts;

   numPnts = points.GetItemsInContainer();
   p2 = new TFPoint[numPnts];

   for (i = 0; i < numPnts; i++)
      {
      p2[i] = *points[i];
      }

   bool retval;
   retval = TRealDC::Polygon(p2,numPnts);

   delete[] p2;

   return retval;
}

