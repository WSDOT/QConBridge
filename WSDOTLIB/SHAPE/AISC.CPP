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

#include <string.h>
#include <math.h>
#include "aisc.hpp"

TRolledShape::TRolledShape(double Angle,
              TFPoint& rp,TLocationPoint lp,
              TColor Color,double e, double d) :
TShape(Color,e,d),
TPosRect(0,0,Angle,rp,lp)
{
}

void TRolledShape::ComputeProperties(void)
{
   TFPoint botLeft;

   area = AX;

   botLeft = GetBottomLeft();
   cg.Xbar = botLeft.x - ((ZD-ZC)*cos(GetRotationAngle()) + (YD-YC)*sin(GetRotationAngle()));
   cg.Ybar = botLeft.y + ((YD-YC)*cos(GetRotationAngle()) - (ZD-ZC)*sin(GetRotationAngle()));

   Ilocal.Ixx = IZ;
   Ilocal.Iyy = IY;
   Ilocal.Ixy = 0;
   Ilocal.TransformInertia(GetRotationAngle());

   TShape::ComputeProperties();
}

void TRolledShape::ComputeExtents(void)
{
   GetPosRectExtents(extents);
}

//
// TWShape
//
TWShape::TWShape(char *shape,double Angle,
         TFPoint& rp,TLocationPoint lp,
         TColor Color,double e,double d) :
TRolledShape(Angle,rp,lp,Color,e,d)
{
   ifstream ifile("wshapes9.tbl");

   // find shape
   while (ifile)
      {
      ifile >> *this;
      if (strncmp(shape,this->name,strlen(shape)) == 0)
         {
         break;
         }
      }

   SetPosRectSize(YD,ZD);
   ComputeExtents();
}

void TWShape::Draw(TOWLRealDC &dc)
{
   TFPoint fp[12];
   TFPoint center;
   TFPoint botLeft;

   botLeft = GetBottomLeft();

   fp[0].x = botLeft.x;
   fp[0].y = botLeft.y;
   fp[1] = fp[0] + TFPoint(0,FLTK);
   fp[2] = fp[1] + TFPoint((ZD-WBTK)/2,0);
   fp[3] = fp[2] + TFPoint(0,YD-2*FLTK);
   fp[4] = fp[3] - TFPoint((ZD-WBTK)/2,0);
   fp[5] = fp[4] + TFPoint(0,FLTK);
   fp[6] = fp[5] + TFPoint(ZD,0);
   fp[7] = fp[6] - TFPoint(0,FLTK);
   fp[8] = fp[7] - TFPoint((ZD-WBTK)/2,0);
   fp[9] = fp[8] - TFPoint(0,YD-2*FLTK);
   fp[10] = fp[9] + TFPoint((ZD-WBTK)/2,0);
   fp[11] = fp[10] - TFPoint(0,FLTK);

   center.x = refPoint.x;
   center.y = refPoint.y;
   dc.Rotate(center,GetRotationAngle(),fp,12);

   // Setup color
   dc.SelectObject(TBrush(color));
   dc.Polygon(&fp,12);
}

istream& operator >> (istream &s,TWShape &w)
{
   s >> w.name >> w.ND >> w.WEIGHT >> w.AX >> w.AY >> w.AZ >> w.YD >> w.WBTK
     >> w.ZD >> w.FLTK >> w.BF2TF >> w.DTW >> w.RT >> w.YDAFL >> w.IZ
     >> w.SZ >> w.SZS >> w.RZ >> w.IY >> w.SY >> w.SYS >> w.RY
     >> w.ZZ >> w.ZY >> w.IX >> w.CW >> w.INTYD >> w.YC >> w.ZC
     >> w.EY >> w.EZ >> w.GRPNUM >> w.SHAPE;
   return s;
}

//
// TChannel
//
TChannel::TChannel(char *shape,double Angle,
          TFPoint& rp,TLocationPoint lp,
          TColor Color,double e,double d) :
TRolledShape(Angle,rp,lp,Color,e,d)
{
   ifstream ifile("channel9.tbl");

   if (!ifile)
      cout << "Error opening channel9.tbl" << endl;

   // find shape
   while (ifile)
      {
      ifile >> *this;
      if (strncmp(shape,this->name,strlen(shape)) == 0)
         {
         break;
         }
      }

   SetPosRectSize(YD,ZD);
   ComputeExtents();
}

void TChannel::Draw(TOWLRealDC &dc)
{
   TFPoint fp[8];
   TFPoint center;
   TFPoint botLeft;

   botLeft = GetBottomLeft();

   fp[0].x = botLeft.x;
   fp[0].y = botLeft.y;
   fp[1] = fp[0] + TFPoint(0,YD);
   fp[2] = fp[1] + TFPoint(ZD,0);
   fp[3] = fp[2] - TFPoint(0,FLTK);
   fp[4] = fp[3] - TFPoint(ZD-WBTK,0);
   fp[5] = fp[4] - TFPoint(0,YD-2*FLTK);
   fp[6] = fp[5] + TFPoint(ZD-WBTK,0);
   fp[7] = fp[6] - TFPoint(0,FLTK);

   center.x = refPoint.x;
   center.y = refPoint.y;
   dc.Rotate(center,GetRotationAngle(),fp,8);

   // Setup color
   dc.SelectObject(TBrush(color));
   dc.Polygon(fp,8);
}

istream& operator >> (istream &s,TChannel &c)
{
   s >> c.name >> c.ND >> c.WEIGHT >> c.AX >> c.AY >> c.AZ >> c.YD >> c.WBTK
     >> c.ZD >> c.FLTK >> c.ZC >> c.EZ >> c.BF2TF >> c.DTW >> c.YDAFL
     >> c.IZ >> c.ZZ >> c.SZ >> c.SZS >> c.RZ >> c.IY >> c.ZY
     >> c.SY >> c.SYS >> c.RY >> c.IX >> c.CW >> c.INTYD >> c.YC
     >> c.EY >> c.GRPNUM >> c.SHAPE;

   return s;
}

//
// TTee
//
TTee::TTee(char *shape,double Angle,
      TFPoint& rp,TLocationPoint lp,
      TColor Color,double e,double d) :
TRolledShape(Angle,rp,lp,Color,e,d)
{
   ifstream ifile("tees9.tbl");

   if (!ifile)
      cout << "Error opening tees9.tbl" << endl;

   // find shape
   while (ifile)
      {
      ifile >> *this;
      if (strncmp(shape,this->name,strlen(shape)) == 0)
         {
         break;
         }
      }

   SetPosRectSize(YD,ZD);
   ComputeExtents();
}

void TTee::Draw(TOWLRealDC &dc)
{
   TFPoint fp[8];
   TFPoint center;
   TFPoint topLeft;

   topLeft = GetTopLeft();

   fp[0].x = topLeft.x;
   fp[0].y = topLeft.y;
   fp[1] = fp[0] + TFPoint(ZD,0);
   fp[2] = fp[1] - TFPoint(0,FLTK);
   fp[3] = fp[2] - TFPoint((ZD-WBTK)/2,0);
   fp[4] = fp[3] - TFPoint(0,YD-FLTK);
   fp[5] = fp[4] - TFPoint(WBTK,0);
   fp[6] = fp[5] + TFPoint(0,YD-FLTK);
   fp[7] = fp[6] - TFPoint((ZD-WBTK)/2,0);

   center.x = refPoint.x;
   center.y = refPoint.y;
   dc.Rotate(center,GetRotationAngle(),fp,8);

   // Setup color
   dc.SelectObject(TBrush(color));
   dc.Polygon(fp,8);
}

istream& operator >> (istream &s,TTee &t)
{
   s >> t.name >> t.ND >> t.WEIGHT >> t.AX >> t.AY >> t.AZ >> t.YD >> t.WBTK
     >> t.ZD >> t.FLTK >> t.BF2TF >> t.DTW >> t.YDAFL >> t.IZ >> t.SZ
     >> t.SZS >> t.RZ >> t.IY >> t.SY >> t.SYS >> t.RY >> t.IX
     >> t.ZZ >> t.ZY >> t.CW >> t.INTYD >> t.YC >> t.ZC >> t.EY
     >> t.EZ >> t.RT >> t.GRPNUM >> t.SHAPE;

   return s;
}

