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

#include <math.h>
#include "tube.hpp"
#include "polygon.hpp"
#include "ellipse.hpp"

//
// TRectTube
//
// Class representing a generalized rectangular tube with rounded corners.
//
TRectTube::TRectTube(double H,double B,double T1,double T2,double Ri,double Ro,
                     int color,double e,double d) :
TSection(color,e,d)
{
   h = H;
   b = B;
   t1 = T1;
   t2 = T2;
   ri = Ri;
   ro = Ro;

   GenerateSection();
}

void TRectTube::GenerateSection(void)
{
   TRectangle *rectOuter, *rectInner;
   TCircleComplement *co1, *co2, *co3, *co4;
   TCircleComplement *ci1, *ci2, *ci3, *ci4;
   double angle = 0.;

   rectOuter = new TRectangle(h,b,angle,TShapePoint(0,0),CenterCenter,Color,baseE,baseDensity);

   rectInner = new TRectangle(h-2*t2,b-2*t1,angle,TShapePoint(0,0),CenterCenter,Color,baseE,baseDensity);
   rectInner->IsSolid(FALSE);

   co1 = new TCircleComplement(ro,3*M_PI/2,TShapePoint(h/2,-b/2),CenterCenter,Color,baseE,baseDensity);
   co1->IsSolid(FALSE);
   co2 = new TCircleComplement(ro,M_PI,TShapePoint(h/2,b/2),CenterCenter,Color,baseE,baseDensity);
   co2->IsSolid(FALSE);
   co3 = new TCircleComplement(ro,M_PI/2,TShapePoint(-h/2,b/2),CenterCenter,Color,baseE,baseDensity);
   co3->IsSolid(FALSE);
   co4 = new TCircleComplement(ro,0.,TShapePoint(-h/2,-b/2),CenterCenter,Color,baseE,baseDensity);
   co4->IsSolid(FALSE);

   ci1 = new TCircleComplement(ri,3*M_PI/2,TShapePoint(h/2-t2,t1-b/2),CenterCenter,Color,baseE,baseDensity);
   ci2 = new TCircleComplement(ri,M_PI,TShapePoint(h/2-t2,b/2-t1),CenterCenter,Color,baseE,baseDensity);
   ci3 = new TCircleComplement(ri,M_PI/2,TShapePoint(t2-h/2,b/2-t1),CenterCenter,Color,baseE,baseDensity);
   ci4 = new TCircleComplement(ri,0.,TShapePoint(t2-h/2,t1-b/2),CenterCenter,Color,baseE,baseDensity);

   AddGeometricObject(rectOuter);
   AddGeometricObject(rectInner);
   AddGeometricObject(co1);
   AddGeometricObject(co2);
   AddGeometricObject(co3);
   AddGeometricObject(co4);
   AddGeometricObject(ci1);
   AddGeometricObject(ci2);
   AddGeometricObject(ci3);
   AddGeometricObject(ci4);
}

//
// TEllipticalTube
//
TEllipticalTube::TEllipticalTube(double B,double H,double T,
                 int color,double e,double d) :
TSection(color,e,d)
{
   b = B;
   h = H;
   t = T;

   GenerateSection();
}

void TEllipticalTube::GenerateSection(void)
{
   TEllipse *ellipse1, *ellipse2;

   ellipse1 = new TEllipse(b,h,0.,TShapePoint(0,0),CenterCenter,Color,baseE,baseDensity);
   ellipse2 = new TEllipse(b-2*t,h-2*t,0.,TShapePoint(0,0),CenterCenter,Color,baseE,baseDensity);
   ellipse2->IsSolid(FALSE);

   AddGeometricObject(ellipse1);
   AddGeometricObject(ellipse2);
}

//
// TCircularTube
//
TCircularTube::TCircularTube(double D,double T,
               int color,double e,double d) :
TEllipticalTube(D,D,T,color,e,d)
{
}

