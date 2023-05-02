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

#include <shape\genshape.h>

GeneralShape::GeneralShape(double _area,double xBar,double yBar,
                           double ixx,double iyy,double ixy,
                           double h,double w,
                           Material* material,
                           TFPoint& refPoint,TLocationPoint lp) :
TShape(material),
Area(_area),xBar(xBar),yBar(yBar),ixx(ixx),iyy(iyy),ixy(ixy),h(h),w(w)
{
   posRect = new TPosRect(h,w,0.00,refPoint,lp);
}

GeneralShape::~GeneralShape()
{
   delete posRect;
}

void
GeneralShape::ComputeLocalProperties()
{
   TFPoint origin = posRect->GetBottomLeft();

   area = Area;
   cg.Xbar = origin.x + xBar;
   cg.Ybar = origin.y + yBar;
   Ilocal.Ixx = ixx;
   Ilocal.Iyy = iyy;
   Ilocal.Ixy = ixy;
}

void
GeneralShape::ComputeExtents()
{
   extents = TFRect(0,0,w,h);
}

TShape*
GeneralShape::Clone() const
{
   return new GeneralShape(Area,xBar,yBar,ixx,iyy,ixy,h,w,material->Clone(),posRect->GetCenterCenter(),CenterCenter);
}


#if defined(_Windows)
void GeneralShape::Draw(TRealDC& /*dc*/)
{
}
#endif

