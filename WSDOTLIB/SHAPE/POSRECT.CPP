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

#include <shape\posrect.h>

//
// TPosTrapezoid
//
TPosTrapezoid::TPosTrapezoid(double H,double B1,double B2,double C,double Angle,TFPoint& rp,TLocationPoint lp)
{
   h = H;
   b = B1;
   bTop = B2;
   c = C;

   rotAngle = Angle;
   refPoint = rp;
   locPoint = lp;

   ComputeCenterCenter();
}

TPosTrapezoid::~TPosTrapezoid()
{
}

void TPosTrapezoid::ComputeCenterCenter()
{
   TFPoint givenPnt;

   switch(locPoint)
      {
      case CenterCenter:
           givenPnt.x = 0;
           givenPnt.y = 0;
           break;

      case TopLeft:
           givenPnt.x = -(b+bTop)/4 + c/2;
           givenPnt.y = h/2;
           break;

      case TopCenter:
           givenPnt.x = -(b+bTop)/4 + c/2 + bTop/2;
           givenPnt.y = h/2;
           break;

      case TopRight:
           givenPnt.x = -(b+bTop)/4 + c/2 + bTop;
           givenPnt.y = h/2;
           break;

      case LeftCenter:
           givenPnt.x = (b+bTop)/4;
           givenPnt.y = 0;
           break;

      case RightCenter:
           givenPnt.x = -(b+bTop)/4;
           givenPnt.y = 0;
           break;

      case BottomLeft:
           givenPnt.x = c/2 + (b+bTop)/4;
           givenPnt.y = -h/2;
           break;

      case BottomCenter:
           givenPnt.x = -b/2 + c/2 + (b+bTop)/4;
           givenPnt.y = -h/2;
           break;

      case BottomRight:
           givenPnt.x = -(b+bTop)/4 - c/2 + b;
           givenPnt.y = -h/2;
           break;
      }

   centerPnt = refPoint;
   centerPnt -= ToGlobalCoordinates(givenPnt);
}


TFPoint TPosTrapezoid::GetTopLeft()
{
   TFPoint pnt;

   pnt = centerPnt + ToGlobalCoordinates(TFPoint(-(b+bTop)/4 + c/2,h/2));

   return pnt;
}

TFPoint TPosTrapezoid::GetTopCenter()
{
   TFPoint pnt;

   pnt = centerPnt + ToGlobalCoordinates(TFPoint(-(b+bTop)/4 + c/2 + bTop/2,h/2));

   return pnt;
}

TFPoint TPosTrapezoid::GetTopRight()
{
   TFPoint pnt;

   pnt = centerPnt + ToGlobalCoordinates(TFPoint(-(b+bTop)/4 + c/2 + bTop,h/2));

   return pnt;
}

TFPoint TPosTrapezoid::GetLeftCenter()
{
   TFPoint pnt;

   pnt = centerPnt + ToGlobalCoordinates(TFPoint((b+bTop)/4,0));

   return pnt;
}

TFPoint TPosTrapezoid::GetCenterCenter()
{
   return centerPnt;
}

TFPoint TPosTrapezoid::GetRightCenter()
{
   TFPoint pnt;

   pnt = centerPnt + ToGlobalCoordinates(TFPoint(-(b+bTop)/4,0));

   return pnt;
}

TFPoint TPosTrapezoid::GetBottomLeft()
{
   TFPoint pnt;

   pnt = centerPnt + ToGlobalCoordinates(TFPoint(-(b+bTop)/4 - c/2,-h/2));

   return pnt;
}

TFPoint TPosTrapezoid::GetBottomCenter()
{
   TFPoint pnt;

   pnt = centerPnt + ToGlobalCoordinates(TFPoint(-b/2 + c/2 + (b+bTop)/4,-h/2));

   return pnt;
}

TFPoint TPosTrapezoid::GetBottomRight()
{
   TFPoint pnt;

   pnt = centerPnt + ToGlobalCoordinates(TFPoint(-(b+bTop)/4 - c/2 + b,-h/2));

   return pnt;
}

TFPoint TPosTrapezoid::GetPosPoint(TLocationPoint lp)
{
   TFPoint pnt;

   switch(lp)
      {
      case CenterCenter:
           pnt = GetCenterCenter();
           break;

      case TopLeft:
           pnt = GetTopLeft();
           break;

      case TopCenter:
           pnt = GetTopCenter();
           break;

      case TopRight:
           pnt = GetTopRight();
           break;

      case LeftCenter:
           pnt = GetLeftCenter();
           break;

      case RightCenter:
           pnt = GetRightCenter();
           break;

      case BottomLeft:
           pnt = GetBottomLeft();
           break;

      case BottomCenter:
           pnt = GetBottomCenter();
           break;

      case BottomRight:
           pnt = GetBottomRight();
           break;
      }

   return pnt;
}

double TPosTrapezoid::GetRotationAngle()
{
   return rotAngle;
}

TFPoint TPosTrapezoid::ToGlobalCoordinates(TFPoint &lp)
{
   TFPoint gp;

   gp.x = lp.x * cos(rotAngle) - lp.y * sin(rotAngle);
   gp.y = lp.y * cos(rotAngle) + lp.x * sin(rotAngle);

   return gp;
}

//
//
//
TPosRect::TPosRect(double H,double B,double Angle,TFPoint& rp,TLocationPoint lp) :
TPosTrapezoid(H,B,B,0,Angle,rp,lp)
{
}

TPosRect::~TPosRect()
{
}

