///////////////////////////////////////////////////////////////////////
// OWLTools - Extension Library for the Borland OWL Framework
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
// May 1997 - Created, Richard Brice
// Oct 1999 - Released as open source

#include <math.h>
#include <string.h>
#include <owltools\realdc.h>

//
// TRealDC - A Real Device Context
//
TRealDC::TRealDC(HDC dc) :
TDC(dc)
{
   NDC_SetMapMode();
   ndcMapMode = MM_ANISOTROPIC;

   worldOrg = TFPoint(0.0,0.0);
   worldExt = TFSize(1.0,1.0);

   ndcOrg = TFPoint(0.0,0.0);
   ndcExt = TFSize(1.0,1.0);

   devRect.left = devRect.top = devRect.right = devRect.bottom = 0;
   devOrg.x = 0;
   devOrg.y = 0;
   devExtX = 0;
   devExtY = 0;
   devHeight = 0;
   devWidth  = 0;
}

TRealDC::~TRealDC(void)
{
   NDC_ResetMapMode();
}

//
// TRealDC Setup functions
//
int TRealDC::SetMapMode(int mm)
{
   if (mm != MM_ISOTROPIC && mm != MM_ANISOTROPIC)
      ShowError("Mode Mode must be MM_ISOTROPIC or MM_ANISOTROPIC");

   int temp = ndcMapMode;
   ndcMapMode = mm;
   return temp;
}

int TRealDC::GetMapMode(void)
{
   return ndcMapMode;
}

void TRealDC::SetDeviceRect(TRect &r)
{
   SetDeviceRect(r.left,r.top,r.right,r.bottom);
}

void TRealDC::SetDeviceRect(int left,int top,int right,int bottom)
{
   devRect.left   = left;
   devRect.top    = top;
   devRect.right  = right;
   devRect.bottom = bottom;

   devWidth  = devRect.right - devRect.left;
   devHeight = devRect.bottom - devRect.top;
   devExtX = devWidth;
   devExtY = -1*devHeight;
}

bool TRealDC::SetWorldOrg(double x,double y)
{
   return SetWorldOrg(TFPoint(x,y));
}

bool TRealDC::SetWorldOrg(TFPoint &point)
{
   // Maps (point.x,point.y) to (ViewOrg.x,ViewOrg.y)
   worldOrg = point;

   return TRUE;
}

bool TRealDC::SetWorldExt(double dx,double dy)
{
   return SetWorldExt(TFSize(dx,dy));
}

bool TRealDC::SetWorldExt(TFSize &extent)
{
   worldExt = extent;

   NDC_AdjustExtents();

   return TRUE;
}

bool TRealDC::SetNDCOrg(double x,double y)
{
   return SetNDCOrg(TFPoint(x,y));
}

bool TRealDC::SetNDCOrg(TFPoint &point)
{
   // Maps (point.x,point.y) to (WorldOrg.x,WorldOrg.y)
   ndcOrg = point;

   if (ndcOrg.x < 0 || ndcOrg.x > 1)
      ShowWarning("NDC X-Origin is out of bounds");
   if (ndcOrg.y < 0 || ndcOrg.y > 1)
      ShowWarning("NDC Y-Origin is out of bounds");

   return TRUE;
}

bool TRealDC::SetNDCExt(double dx,double dy)
{
   return SetNDCExt(TFSize(dx,dy));
}

bool TRealDC::SetNDCExt(TFSize &extent)
{
   ndcExt = extent;

   if (ndcExt.dx < 0 || ndcExt.dx > 1)
      ShowWarning("NDC X-Extent is out of bounds");
   if (ndcExt.dy < 0 || ndcExt.dy > 1)
      ShowWarning("NDC Y-Extent is out of bounds");

   NDC_AdjustExtents();

   return TRUE;
}

TFPoint TRealDC::GetWorldOrg(void)
{
   return worldOrg;
}

TFSize TRealDC::GetWorldExt(void)
{
   return worldExt;
}

TFPoint TRealDC::GetNDCOrg(void)
{
   return ndcOrg;
}

TFSize TRealDC::GetNDCExt(void)
{
   return ndcExt;
}

//
// TRealDC Drawing functions
//
#if defined (__WIN32__)
void TRealDC::WPtoDP(double worldX,double worldY,LONG &devX,LONG &devY)
#else
void TRealDC::WPtoDP(double worldX,double worldY,int &devX,int &devY)
#endif
{
   double ndcX, ndcY;

   WPtoNDC(worldX,worldY,ndcX,ndcY);
   NDCtoDP(ndcX,ndcY,devX,devY);
}

void TRealDC::WPtoDP(TFPoint worldPoint,TPoint &devPoint)
{
   WPtoDP(worldPoint.x,worldPoint.y,devPoint.x,devPoint.y);
}

#if defined (__WIN32__)
void TRealDC::DPtoWP(LONG devX,LONG devY,double &worldX,double &worldY)
#else
void TRealDC::DPtoWP(int devX,int devY,double &worldX,double &worldY)
#endif
{
   double ndcX, ndcY;

   DPtoNDC(devX,devY,ndcX,ndcY);
   NDCtoWP(ndcX,ndcY,worldX,worldY);
}

void TRealDC::DPtoWP(TPoint devPoint,TFPoint &worldPoint)
{
   DPtoWP(devPoint.x,devPoint.y,worldPoint.x,worldPoint.y);
}

bool TRealDC::MoveTo(double worldX,double worldY)
{
   double ndcX,ndcY;

   WPtoNDC(worldX,worldY,ndcX,ndcY);

   return NDC_MoveTo(ndcX,ndcY);
}

bool TRealDC::MoveTo(TFPoint &worldPoint)
{
   return MoveTo(worldPoint.x,worldPoint.y);
}

bool TRealDC::MoveTo(TFPoint *worldPoint)
{
   return MoveTo(worldPoint->x,worldPoint->y);
}

bool TRealDC::MoveTo(int x,int y)
{
   return TDC::MoveTo(x,y);
}

bool TRealDC::MoveTo(const TPoint &point)
{
   return TDC::MoveTo(point);
}

bool TRealDC::MoveTo(const TPoint &point,TPoint &oldPoint)
{
   return TDC::MoveTo(point,oldPoint);
}

bool TRealDC::LineTo(double worldX,double worldY)
{
   double ndcX,ndcY;

   WPtoNDC(worldX,worldY,ndcX,ndcY);

   return NDC_LineTo(ndcX,ndcY);
}

bool TRealDC::LineTo(TFPoint &worldPoint)
{
   return LineTo(worldPoint.x,worldPoint.y);
}

bool TRealDC::LineTo(TFPoint *worldPoint)
{
   return LineTo(worldPoint->x,worldPoint->y);
}

bool TRealDC::LineTo(int x,int y)
{
   return TDC::LineTo(x,y);
}

bool TRealDC::LineTo(const TPoint &point)
{
   return TDC::LineTo(point);
}

bool TRealDC::Polygon(const TFPoint *worldPoints,int count)
{
   TFPoint *ndcPoints;

   ndcPoints = new TFPoint[count];
   WPtoNDC(worldPoints,ndcPoints,count);

   bool retval;
   retval = NDC_Polygon(ndcPoints,count);

   delete[] ndcPoints;

   return retval;
}

bool TRealDC::Polygon(const TPoint *points,int count)
{
   return TDC::Polygon(points,count);
}

bool TRealDC::Polyline(const double *worldX,const double *worldY,int count)
{
   TFPoint *worldPoints;
   int i;

   worldPoints = new TFPoint[count];

   for (i = 0; i < count; i++)
      {
      worldPoints[i].x = worldX[i];
      worldPoints[i].y = worldY[i];
      }

   bool retval;
   retval = Polyline(worldPoints,count);

   delete[] worldPoints;

   return retval;
}

bool TRealDC::Polyline(const TFPoint *worldPoints,int count)
{
   TFPoint *ndcPoints;
   ndcPoints = new TFPoint[count];

   WPtoNDC(worldPoints,ndcPoints,count);

   bool retval;
   retval = NDC_Polyline(ndcPoints,count);

   delete[] ndcPoints;

   return retval;
}

bool TRealDC::Polyline(const TPoint *points,int count)
{
   return TDC::Polyline(points,count);
}

bool TRealDC::Rectangle(double left,double top,double right,double bottom)
{
   double ndcLeft,ndcTop,ndcRight,ndcBottom;

   WPtoNDC(left,top,ndcLeft,ndcTop);
   WPtoNDC(right,bottom,ndcRight,ndcBottom);

   return NDC_Rectangle(ndcLeft,ndcTop,ndcRight,ndcBottom);
}

bool TRealDC::Rectangle(const TFPoint &p1,const TFPoint &p2)
{
   return Rectangle(p1.x,p1.y,p2.x,p2.y);
}

bool TRealDC::Rectangle(const TFPoint &point,const TFSize &size)
{
   return Rectangle(point.x,point.y,point.x + size.dx,point.y + size.dy);
}

bool TRealDC::Rectangle(const TFRect &rect)
{
   return Rectangle(rect.minX,rect.minY,rect.maxX,rect.maxY);
}

bool TRealDC::Rectangle(int x1,int y1,int x2,int y2)
{
   return TDC::Rectangle(x1,y1,x2,y2);
}

bool TRealDC::Rectangle(const TPoint &p1,const TPoint &p2)
{
   return TDC::Rectangle(p1,p2);
}

bool TRealDC::Rectangle(const TPoint &point,const TSize &s)
{
   return TDC::Rectangle(point,s);
}

bool TRealDC::Rectangle(const TRect &rect)
{
   return TDC::Rectangle(rect);
}

bool TRealDC::Ellipse(double left,double top,double right,double bottom)
{
   double ndcLeft,ndcTop,ndcRight,ndcBottom;

   WPtoNDC(left,top,ndcLeft,ndcTop);
   WPtoNDC(right,bottom,ndcRight,ndcBottom);

   return NDC_Ellipse(ndcLeft,ndcTop,ndcRight,ndcBottom);
}

bool TRealDC::Ellipse(const TFPoint &p1,const TFPoint &p2)
{
   return Ellipse(p1.x,p1.y,p2.x,p2.y);
}

bool TRealDC::Ellipse(const TFPoint &point,const TFSize &size)
{
   return Ellipse(point.x,point.y,point.x + size.dx,point.y + size.dy);
}

bool TRealDC::Ellipse(const TFRect &rect)
{
   return Ellipse(rect.minX,rect.minY,rect.maxX,rect.maxY);
}

bool TRealDC::Ellipse(int x1,int y1,int x2,int y2)
{
   return TDC::Ellipse(x1,y1,x2,y2);
}

bool TRealDC::Ellipse(const TPoint &p1,const TPoint &p2)
{
   return TDC::Ellipse(p1,p2);
}

bool TRealDC::Ellipse(const TPoint &point,const TSize &size)
{
   return TDC::Ellipse(point,size);
}

bool TRealDC::Ellipse(const TRect &rect)
{
   return TDC::Ellipse(rect);
}

bool TRealDC::Circle(const TPoint &center,int radius)
{
   TRect rect(center.x-radius,center.y+radius,center.x+radius,center.y-radius);
   return Ellipse(rect);
}

bool TRealDC::Circle(const TFPoint &center,double radius)
{
   TFRect rect(center.x-radius,center.y+radius,center.x+radius,center.y-radius);
   return Ellipse(rect);
}

bool TRealDC::Rotate(const TFPoint &center,const double angle,TFPoint *points,int count)
{
   TFPoint p1, p2;
   int i;

   if (count < 1)
      return FALSE;

   for (i = 0; i < count; i++)
      {
      p1 = points[i];
      p1 -= center;
      p2.x = p1.x*cos(angle) + p1.y*sin(angle);
      p2.y = p1.y*cos(angle) - p1.x*sin(angle);
      points[i] = p2 + center;
      }

   return TRUE;
}

//
// TRealDC Text functions
//

bool TRealDC::TextOut(const TFPoint& worldPoint,const char *string,int count)
{
   TFPoint ndcPoint;

   WPtoNDC(worldPoint,ndcPoint);
   return NDC_TextOut(ndcPoint.x,ndcPoint.y,string,count);
}

bool TRealDC::TextOut(const TPoint &point,const char *string,int count)
{
   return TDC::TextOut(point,string,count);
}

bool TRealDC::TextOut(int x,int y,const char *string,int count)
{
   return TDC::TextOut(x,y,string,count);
}

//
// TRealDC NDC functions
//
void TRealDC::WPtoNDC(double worldX,double worldY,double &ndcX,double &ndcY)
{
   ndcX = (worldX - worldOrg.x)*(ndcExt.dx/worldExt.dx) + ndcOrg.x;
   ndcY = (worldY - worldOrg.y)*(ndcExt.dy/worldExt.dy) + ndcOrg.y;

   if (ndcX < 0 || ndcX > 1)
      ShowError("Error in WPtoNDC: ndcX < 0 or ndcX > 1");
   if (ndcY < 0 || ndcY > 1)
      ShowError("Error in WPtoNDC: ndcY < 0 or ndcY > 1");
}

void TRealDC::WPtoNDC(const TFPoint &worldPoint,TFPoint &ndcPoint)
{
   WPtoNDC(worldPoint.x,worldPoint.y,ndcPoint.x,ndcPoint.y);
}

void TRealDC::WPtoNDC(const TFPoint *worldPoint,TFPoint *ndcPoint,int count)
{
   int i;

   for (i = 0; i < count; i++)
      WPtoNDC(worldPoint[i],ndcPoint[i]);
}

void TRealDC::NDCtoWP(double ndcX,double ndcY,double &worldX,double &worldY)
{
   worldX = (ndcX - ndcOrg.x)*(worldExt.dx/ndcExt.dx) + worldOrg.x;
   worldY = (ndcY - ndcOrg.y)*(worldExt.dy/ndcExt.dy) + worldOrg.y;
}

void TRealDC::NDCtoWP(TFPoint ndcPoint,TFPoint &worldPoint)
{
   NDCtoWP(ndcPoint.x,ndcPoint.y,worldPoint.x,worldPoint.y);
}

#if defined (__WIN32__)
void TRealDC::NDCtoDP(double ndcX,double ndcY,LONG &devX,LONG &devY)
#else
void TRealDC::NDCtoDP(double ndcX,double ndcY,int &devX,int &devY)
#endif
{
   TFPoint devOrg;
   devOrg.x = ndcOrg.x*devExtX + devRect.left + ((devRect.right - devRect.left) - devExtX)/2;
   devOrg.y = ndcOrg.y*(-devExtY) + devRect.top + ((devRect.bottom - devRect.top) + devExtY)/2;
   devX = (ndcX - ndcOrg.x)*devExtX/ndcExt.dx + devOrg.x;
   devY = (ndcY - ndcOrg.y)*devExtY/ndcExt.dy + devOrg.y;
}

void TRealDC::NDCtoDP(TFPoint ndcPoint,TPoint &devPoint)
{
   NDCtoDP(ndcPoint.x,ndcPoint.y,devPoint.x,devPoint.y);
}

void TRealDC::NDCtoDP(const TFPoint *ndcPoint,TPoint *devPoint,int count)
{
   int i;

   for (i = 0; i < count; i++)
      NDCtoDP(ndcPoint[i],devPoint[i]);
}

#if defined (__WIN32__)
void TRealDC::DPtoNDC(LONG devX,LONG devY,double &ndcX,double &ndcY)
#else
void TRealDC::DPtoNDC(int devX,int devY,double &ndcX,double &ndcY)
#endif
{
   TFPoint devOrg;
   devOrg.x = ndcOrg.x*devExtX + devRect.left + ((devRect.right - devRect.left) - devExtX)/2;
   devOrg.y = ndcOrg.y*(-devExtY) + devRect.top + ((devRect.bottom - devRect.top) + devExtY)/2;
   ndcX = (devX - devOrg.x)*ndcExt.dx/devExtX + ndcOrg.x;
   ndcY = (devY - devOrg.y)*ndcExt.dy/devExtY + ndcOrg.y;

   if (ndcX < 0 || ndcX > 1)
      ShowError("Error in DPtoNDC: ndcX < 0 or ndcX > 1");
   if (ndcY < 0 || ndcY > 1)
      ShowError("Error in DPtoNDC: ndcY < 0 or ndcY > 1");
}

void TRealDC::DPtoNDC(TPoint devPoint,TFPoint &ndcPoint)
{
   DPtoNDC(devPoint.x,devPoint.y,ndcPoint.x,ndcPoint.y);
}

void TRealDC::NDC_SetMapMode(void)
{
   deviceMapMode = TDC::SetMapMode(MM_TEXT);
}

void TRealDC::NDC_ResetMapMode(void)
{
   TDC::SetMapMode(deviceMapMode);
}

void TRealDC::NDC_AdjustExtents(void)
{
   double ndcX, ndcY;
   double xSign, ySign;

   // the available device extents is the size of the device reduced
   // by the ndc extent fraction. (i.e. if ndcExt = (0.9,0.9) then
   // only 90% of the device width and height are avaiable for the extents
   devExtX = devWidth * ndcExt.dx;
   devExtY = -1*devHeight * ndcExt.dy;

   if (ndcMapMode == MM_ANISOTROPIC)
      return;

   xSign = ndcExt.dx/fabs(ndcExt.dx);
   ySign = ndcExt.dy/fabs(ndcExt.dy);

   ndcX = fabs(worldExt.dx*ndcExt.dy/worldExt.dy);
   ndcY = fabs(worldExt.dy*ndcExt.dx/worldExt.dx);

   if (ndcX > fabs(ndcExt.dx))
      ndcExt.dy = ySign * fabs(ndcY);
   else
      ndcExt.dx = xSign * fabs(ndcX);

   double extX, extY;
   double Xsign, Ysign;

   // Scale down the device extents to the same proportions as the NDC extents
   Xsign = devExtX/abs(devExtX);
   Ysign = devExtY/abs(devExtY);

   extX = floor(fabs(devExtY * ndcExt.dx/ndcExt.dy));
   extY = floor(fabs(devExtX * ndcExt.dy/ndcExt.dx));

   if (extX > abs(devExtX))
      devExtY = Ysign * abs(extY);
   else
      devExtX = Xsign * abs(extX);
}

bool TRealDC::NDC_MoveTo(double ndcX,double ndcY)
{
#if defined (__WIN32__)
   LONG devX,devY;
#else
   int devX,devY;
#endif

   NDCtoDP(ndcX,ndcY,devX,devY);

   return TDC::MoveTo(devX,devY);
}

bool TRealDC::NDC_MoveTo(TFPoint ndcPoint)
{
   return NDC_MoveTo(ndcPoint.x,ndcPoint.y);
}

bool TRealDC::NDC_LineTo(double ndcX,double ndcY)
{
#if defined (__WIN32__)
   LONG devX,devY;
#else
   int devX,devY;
#endif

   NDCtoDP(ndcX,ndcY,devX,devY);
   return TDC::LineTo(devX,devY);
}

bool TRealDC::NDC_LineTo(TFPoint ndcPoint)
{
   return NDC_LineTo(ndcPoint.x,ndcPoint.y);
}

bool TRealDC::NDC_Polygon(const TFPoint *ndcPoints,int count)
{
   TPoint *devPoints;

   devPoints = new TPoint[count];
   NDCtoDP(ndcPoints,devPoints,count);

   bool retval;
   retval = TDC::Polygon(devPoints,count);

   delete[] devPoints;

   return retval;
}

bool TRealDC::NDC_Polyline(const TFPoint *ndcPoints,int count)
{
   TPoint *devPoints;

   devPoints = new TPoint[count];
   NDCtoDP(ndcPoints,devPoints,count);

   bool retval;

   retval = TDC::Polyline(devPoints,count);

   delete[] devPoints;

   return retval;
}

bool TRealDC::NDC_Rectangle(double ndcLeft,double ndcTop,double ndcRight,double ndcBottom)
{
#if defined (__WIN32__)
   LONG devLeft,devTop,devRight,devBottom;
#else
   int devLeft,devTop,devRight,devBottom;
#endif

   NDCtoDP(ndcLeft,ndcTop,devLeft,devTop);
   NDCtoDP(ndcRight,ndcBottom,devRight,devBottom);

   return TDC::Rectangle(devLeft,devTop,devRight,devBottom);
}

bool TRealDC::NDC_Rectangle(TFRect ndcRect)
{
   return NDC_Rectangle(ndcRect.minX,ndcRect.minY,ndcRect.maxX,ndcRect.maxY);
}

bool TRealDC::NDC_Ellipse(double ndcLeft,double ndcTop,double ndcRight,double ndcBottom)
{
#if defined (__WIN32__)
   LONG devLeft,devTop,devRight,devBottom;
#else
   int devLeft,devTop,devRight,devBottom;
#endif

   NDCtoDP(ndcLeft,ndcTop,devLeft,devTop);
   NDCtoDP(ndcRight,ndcBottom,devRight,devBottom);

   return TDC::Ellipse(devLeft,devTop,devRight,devBottom);
}

bool TRealDC::NDC_Ellipse(TFRect ndcRect)
{
   return NDC_Ellipse(ndcRect.minX,ndcRect.minY,ndcRect.maxX,ndcRect.maxY);
}

bool TRealDC::NDC_TextOut(double ndcX,double ndcY,const char *string,int count)
{
#if defined (__WIN32__)
   LONG devX,devY;
#else
   int devX,devY;
#endif

   NDCtoDP(ndcX,ndcY,devX,devY);

   return TDC::TextOut(devX,devY,string,(count >= 0) ? count : strlen(string));
}

//
// TRealDC Debug functions
//
void TRealDC::ShowError(char *msg)
{
   MessageBox(0,msg,"Fatal Error",MB_OK | MB_ICONSTOP);
//   ExitProcess(0);
}

void TRealDC::ShowWarning(char *msg)
{
   MessageBox(0,msg,"Warning",MB_OK | MB_ICONEXCLAMATION);
}
