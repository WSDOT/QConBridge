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

#include "spandrel.hpp"

TGeneralSpandrel::TGeneralSpandrel(double A,double H,int N,int Quad,TShapePoint &rp,double e,double d) :
TShape(e,d)
{
   a = A;
   h = H;
   n = N;
   quad = Quad;
   refPnt = rp;
}

void TGeneralSpandrel::Draw(TRealDC &dc)
{
}

TZSSpandrel::TZSSpandrel(double A,double H,int N,int Quad,TShapePoint &rp,double e,double d) :
TGeneralSpandrel(A,H,N,Quad,rp,e,d)
{
}

void TZSSpandrel::ComputeProperties(void)
{
   double ybar, zbar;

   area = h*a/(n+1);

   if (IsNullShape())
      return;

   ybar = (n+1)*h/(4*n+2);
   zbar = (n+1)*a/(n+2);

   Ilocal.Iyy = a*a*a*h/((n+3)*(n+2)*(n+2));
   Ilocal.Izz = a*h*h*h*(7*n*n + 4*n + 1)/(12*(3*n + 1)*(2*n + 1)*(2*n + 1));
   Ilocal.Iyz = a*a*h*h*n/(4*(n+1)*(2*n + 1)*(n+2));

   switch (quad)
      {
      case QUAD_1:
           cg.Ybar = refPnt.y + ybar;
           cg.Zbar = refPnt.z + a - zbar;
           break;

      case QUAD_2:
           cg.Ybar = refPnt.y + ybar;
           cg.Zbar = refPnt.z - a + zbar;
           break;

      case QUAD_3:
           cg.Ybar = refPnt.y - ybar;
           cg.Zbar = refPnt.z - a + zbar;
           break;

      case QUAD_4:
           cg.Ybar = refPnt.y - ybar;
           cg.Zbar = refPnt.z + a - zbar;
           break;
      }

   TShape::ComputeProperties();

#ifdef DEBUG
   // use a TPolyShape object to approximate the sprandel. Compare results.
#endif
}


TISSpandrel::TISSpandrel(double A,double H,int N,int Quad,TShapePoint &rp,double e,double d) :
TGeneralSpandrel(A,H,N,Quad,rp,e,d)
{
}

void TISSpandrel::ComputeProperties(void)
{
   double ybar, zbar;

   area = a*h/(n+1);

   if (IsNullShape())
      return;

   ybar = (n+1)*a/(4*n+2);
   zbar = (n+1)*h/(n+2);

   Ilocal.Izz = h*h*h*a/((n+3)*(n+2)*(n+2));
   Ilocal.Iyy = h*a*a*a*(7*n*n + 4*n + 1)/(12*(3*n + 1)*(2*n + 1)*(2*n + 1));
   Ilocal.Iyz = h*h*a*a*n/(4*(n+1)*(2*n + 1)*(n+2));

   switch (quad)
      {
      case QUAD_1:
           cg.Ybar = refPnt.y + h - zbar;
           cg.Zbar = refPnt.z + ybar;
           break;

      case QUAD_2:
           cg.Ybar = refPnt.y + h - zbar;
           cg.Zbar = refPnt.z - ybar;
           break;

      case QUAD_3:
           cg.Ybar = refPnt.y - h + zbar;
           cg.Zbar = refPnt.z - ybar;
           break;

      case QUAD_4:
           cg.Ybar = refPnt.y - h + zbar;
           cg.Zbar = refPnt.z + ybar;
           break;
      }

   TShape::ComputeProperties();

#ifdef DEBUG
   // use a TPolyShape object to approximate the sprandel. Compare results.
#endif
}

