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
#include <shape\shape.h>

//
// TShape
//

TShape::TShape(Material* material) :
material(material)
{
   isSolid      = true;
   isStructural = true;

   InitProperties();
   
#if defined(_Windows)
   color = TColor::Black;
#endif

}

TShape::~TShape()
{
   TRACE("TShape::~TShape()");
   delete material;
}

void TShape::InitProperties()
{
   area = 0.;
   cg.Ybar = 0.;
   cg.Xbar = 0.;
   Ilocal.Iyy = 0.;
   Ilocal.Ixx = 0.;
   Ilocal.Ixy = 0.;
   Iglobal.Iyy = 0.;
   Iglobal.Ixx = 0.;
   Iglobal.Ixy = 0.;
}

bool TShape::IsSolid()
{
   return isSolid;
}

bool TShape::IsSolid(bool s)
{
   bool retval;

   retval = isSolid;
   isSolid = s;

   return retval;
}

bool TShape::IsStructural()
{
   return isStructural;
}

bool TShape::IsStructural(bool a)
{
   bool retval;

   retval = isStructural;
   isStructural = a;

   return retval;
}

void TShape::ComputeProperties()
{
   InitProperties();
   ComputeLocalProperties();
   VerifySign();
   ComputeGlobalInertia();
   ComputePrincipleInertia();
   ComputeExtents();
}

void TShape::VerifySign()
{
   // Make sure sign is correct
   double sign;
   sign = (IsSolid()) ? 1. : -1.;

   area = sign*fabs(area);

   Ilocal.Ixx = sign*fabs(Ilocal.Ixx);
   Ilocal.Iyy = sign*fabs(Ilocal.Iyy);
   Ilocal.Ixy = sign*Ilocal.Ixy;

   Iglobal.Ixx = sign*fabs(Iglobal.Ixx);
   Iglobal.Iyy = sign*fabs(Iglobal.Iyy);
   Iglobal.Ixy = sign*Iglobal.Ixy;
}

void TShape::ComputeGlobalInertia()
{
   Iglobal.Iyy = Ilocal.Iyy + area*cg.Xbar*cg.Xbar;
   Iglobal.Ixx = Ilocal.Ixx + area*cg.Ybar*cg.Ybar;
   Iglobal.Ixy = Ilocal.Ixy + area*cg.Ybar*cg.Xbar;
}

void TShape::ComputePrincipleInertia()
{
   Ip = Ilocal.ComputePrincipleValues();
}

bool TShape::IsNullShape()
{
   bool retval = false;

   if (zero_val(area))
      {
      InitProperties();
      retval = true;
      }

   return retval;
}

double TShape::GetModE()
{
   return (material) ? material->GetModE() : 0;
}

double TShape::GetUnitWeight()
{
   return (material) ? material->GetUnitWgt() : 0;
}

void TShape::GetExtents(double *xmin,double *xmax,double *ymin,double *ymax)
{
   *ymin = extents.minY;
   *ymax = extents.maxY;
   *xmin = extents.minX;
   *xmax = extents.maxX;
}

void TShape::GetExtents(TFRect &r)
{
   r = extents;
}

void TShape::GetExtents(TFPoint &p,TFSize &size)
{
   p = extents.BottomLeft();
   size = extents.Size();
}

double TShape::GetArea()
{
   return area;
}

double TShape::GetIxx()
{
   return Ilocal.Ixx;
}

double TShape::GetIyy()
{
   return Ilocal.Iyy;
}

TCentroid TShape::GetCentroid()
{
   return cg;
}

double TShape::GetXbar()
{
   return cg.Xbar;
}

double TShape::GetYbar()
{
   return cg.Ybar;
}

TInertia TShape::GetGlobalInertia()
{
   return Iglobal;
}

TInertia TShape::GetCentroidalInertia()
{
   return Ilocal;
}

TPrincipleInertia TShape::GetPrincipleInertia()
{
   return Ip;
}

#if defined(_Windows)

void TShape::SetColor(TColor Color)
{
   color = Color;
}

void TShape::SetColor(COLORREF Color)
{
   SetColor(TColor(Color));
}

#endif

