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

#include <shape\rebar.h>

TRebar::TRebar(double dBar,TFPoint &location) :
TShape(0)
{
   db = dBar;
   loc = location;
}

void TRebar::ComputeLocalProperties()
{
   area = M_PI*db*db/4; // This is only temporary... use the actual bar or strand areas
   cg.Xbar = loc.x;
   cg.Ybar = loc.y;
   Iglobal.Ixx = Iglobal.Iyy = Iglobal.Ixy = 0.;
   Ilocal.Ixx = Ilocal.Iyy = Ilocal.Ixy = 0.;
   Ip.I11 = Ip.I22 = Ip.I12 = Ip.pDir = 0.;
}

void TRebar::ComputeExtents()
{
   extents.minX = loc.x - db/2;
   extents.maxX = loc.x + db/2;
   extents.maxY = loc.y + db/2;
   extents.minY = loc.y - db/2;
}

TShape*
TRebar::Clone() const
{
   return new TRebar(db,loc);
}
//
//These methods where in the TSection class, but were removed because they
//really didn't belong there. There were put here so that they wouldn't be
//lost. Someday they may be of use.
//void TSection::GenerateRebar(TFPoint &start,TFPoint &end,int nSpaces)
//{
//   int     i;
//   int     nBars;
//   TFPoint step;
//   TFPoint lastPoint;
//   TRebar *rebar;
//
//   step = TFPoint((end.x - start.x)/nSpaces,(end.y - start.y)/nSpaces);
//   lastPoint = start;
//   nBars = nSpaces + 1;
//   for (i = 0; i < nBars; i++)
//      {
//      rebar = new TRebar(1.00,lastPoint);
//      AddShape(rebar);
//      lastPoint += step;
//      }
//}
//
//void TSection::GenerateRebar(TFPoint &center,double radius,int nSpaces)
//{
//   int i;
//   int nBars;
//   double step, angle;
//   TFPoint p;
//   TRebar *rebar;
//
//   step = 2*M_PI/nSpaces;
//   nBars = nSpaces;
//   p = center;
//   for (i = 0, angle = 0; i < nBars; i++, angle += step)
//      {
//      p.x = radius*cos(angle) + center.x;
//      p.y = radius*sin(angle) + center.y;
//      rebar = new TRebar(1.00,p);
//      AddShape(rebar);
//      }
//}
//
