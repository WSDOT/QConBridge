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
#include <shape\ellipse.h>

//
// TEllipse
//
// Reference:  Vector Mechanics for Engineers - Statics 4th Edition
//             Beer & Johnston
TEllipse::TEllipse(double H,double B,Material* material,
                   TFPoint& rp,TLocationPoint lp) :
TShape(material)
{
   h = H;
   b = B;
   posRect = new TPosRect(H,B,0,rp,lp);

   ComputeExtents();
}

TEllipse::~TEllipse()
{
   if (posRect)
      delete posRect;
}

void TEllipse::ComputeExtents()
{
   TFPoint upLeft;
   TFPoint loRight;

   upLeft = posRect->GetTopLeft();
   loRight = posRect->GetBottomRight();

   extents = TFRect(upLeft,loRight);
}

void TEllipse::ComputeLocalProperties()
{
   TFPoint centerPnt;
   double B, H;

   B = b/2;
   H = h/2;

   area = M_PI*B*H;

   if (IsNullShape())
      return;

   centerPnt = posRect->GetCenterCenter();
   cg.Xbar = centerPnt.x;
   cg.Ybar = centerPnt.y;

   Ilocal.Ixx = M_PI*H*B*B*B/4;
   Ilocal.Iyy = M_PI*H*H*H*B/4;
   Ilocal.Ixy = 0;
   Ilocal.TransformInertia(posRect->GetRotationAngle());
}

TShape*
TEllipse::Clone() const
{
   return new TEllipse(h,b,material->Clone(),posRect->GetCenterCenter(),CenterCenter);
}

#if defined(_Windows)
void
TEllipse::Draw(TRealDC& dc)
{
   TFRect extents;
   GetExtents(extents);
   dc.Ellipse(extents);
}
#endif

//
// TCircle
//
TCircle::TCircle(double D,Material* material,
                 TFPoint& rp,TLocationPoint lp) :
TEllipse(D,D,material,rp,lp)
{
}

/*******************************************************
//
// TQuarterEllipse
//
// Reference: AISC Steel Design Manual 9th Edition
TQuarterEllipse::TQuarterEllipse(double B,double H,double Angle,
                 TFPoint& rp,TLocationPoint lp,
                 int color,double e,double d) :
TEllipse(B,H,Angle,rp,lp,color,e,d)
{
}

void TQuarterEllipse::ComputeProperties()
{
   double ybar, zbar;
   double ysign, zsign;

   area = M_PI*h*b/4;

   if (IsNullShape())
      return;

   ybar = 4*h/(3*M_PI);
   zbar = 4*b/(3*M_PI);

   switch(quad)
      {
      case QUAD_1: ysign =  1.; zsign =  1.; break;
      case QUAD_2: ysign =  1.; zsign = -1.; break;
      case QUAD_3: ysign = -1.; zsign = -1.; break;
      case QUAD_4: ysign = -1.; zsign =  1.; break;
      }

   cg.Ybar = startPnt.y + ysign*ybar;
   cg.Zbar = startPnt.z + zsign*zbar;

   Ilocal.Iyy = h*h*h*b*(M_PI/16 - 4/(9*M_PI));
   Ilocal.Izz = h*b*b*b*(M_PI/16 - 4/(9*M_PI));
   Ilocal.Iyz = 0; // ????

   TShape::ComputeProperties();
}

void TQuarterEllipse::Draw(TRealDC &dc)
{
}

//
// TQuarterCircle
//
// Reference: AISC Steel Design Manual 9th Edition
TQuarterCircle::TQuarterCircle(double R,double Angle,
                TFPoint& rp,TLocationPoint lp,
                int color,double e,double d) :
TQuarterEllipse(R,R,Angle,rp,lp,color,e,d)
{
}

//
// TEllipticComplement
//
TEllipticComplement::TEllipticComplement(double B,double H,double Angle,
                     TFPoint &rp,TLocationPoint lp,
                     int color,double e,double d) :
TQuarterEllipse(B,H,Angle,rp,lp,color,e,d)
{
}

void TEllipticComplement::ComputeProperties()
{
   double ybar, zbar;
   double ysign, zsign;

   area = b*h*(1 - M_PI/4);

   if (IsNullShape())
      return;

   ybar = h - h/(6*(1 - M_PI/4));
   zbar = b - b/(6*(1 - M_PI/4));

   switch(quad)
      {
      case QUAD_1: ysign =  1.; zsign =  1.; break;
      case QUAD_2: ysign =  1.; zsign = -1.; break;
      case QUAD_3: ysign = -1.; zsign = -1.; break;
      case QUAD_4: ysign = -1.; zsign =  1.; break;
      };

   cg.Ybar = startPnt.y + ysign*ybar;
   cg.Zbar = startPnt.z + zsign*zbar;

   Ilocal.Iyy = h*h*h*b*(1./3. - M_PI/36 - 1/(36*(1 - M_PI/4)));
   Ilocal.Izz = h*b*b*b*(1./3. - M_PI/36 - 1/(36*(1 - M_PI/4)));
   Ilocal.Iyz = 0.; // ???

   TShape::ComputeProperties();
}

//
// TCircleComplement
//
TCircleComplement::TCircleComplement(double R,double Angle,
                   TFPoint& rp,TLocationPoint lp,
                   int color,double e,double d) :
TEllipticComplement(R,R,Angle,rp,lp,color,e,d)
{
}
************************************************************/
