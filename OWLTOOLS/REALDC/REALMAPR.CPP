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
#include <owltools\realmapr.h>

//
// TRealMapper - Real number mapper
//
TRealMapper::TRealMapper()
{
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

TRealMapper::~TRealMapper(void)
{
}

//
// TRealMapper Setup functions
//
int TRealMapper::SetMapMode(int mm)
{
   if (mm != MM_ISOTROPIC && mm != MM_ANISOTROPIC)
      ShowError("Mode Mode must be MM_ISOTROPIC or MM_ANISOTROPIC");

   int temp = ndcMapMode;
   ndcMapMode = mm;
   return temp;
}

int TRealMapper::GetMapMode(void)
{
   return ndcMapMode;
}

void TRealMapper::SetDeviceRect(TRect &r)
{
   SetDeviceRect(r.left,r.top,r.right,r.bottom);
}

void TRealMapper::SetDeviceRect(int left,int top,int right,int bottom)
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

bool TRealMapper::SetWorldOrg(double x,double y)
{
   return SetWorldOrg(TFPoint(x,y));
}

bool TRealMapper::SetWorldOrg(TFPoint &point)
{
   // Maps (point.x,point.y) to (ViewOrg.x,ViewOrg.y)
   worldOrg = point;

   return TRUE;
}

bool TRealMapper::SetWorldExt(double dx,double dy)
{
   return SetWorldExt(TFSize(dx,dy));
}

bool TRealMapper::SetWorldExt(TFSize &extent)
{
   worldExt = extent;

   NDC_AdjustExtents();

   return TRUE;
}

bool TRealMapper::SetNDCOrg(double x,double y)
{
   return SetNDCOrg(TFPoint(x,y));
}

bool TRealMapper::SetNDCOrg(TFPoint &point)
{
   // Maps (point.x,point.y) to (WorldOrg.x,WorldOrg.y)
   ndcOrg = point;

   if (ndcOrg.x < 0 || ndcOrg.x > 1)
      ShowWarning("NDC X-Origin is out of bounds");
   if (ndcOrg.y < 0 || ndcOrg.y > 1)
      ShowWarning("NDC Y-Origin is out of bounds");

   return TRUE;
}

bool TRealMapper::SetNDCExt(double dx,double dy)
{
   return SetNDCExt(TFSize(dx,dy));
}

bool TRealMapper::SetNDCExt(TFSize &extent)
{
   ndcExt = extent;

   if (ndcExt.dx < 0 || ndcExt.dx > 1)
      ShowWarning("NDC X-Extent is out of bounds");
   if (ndcExt.dy < 0 || ndcExt.dy > 1)
      ShowWarning("NDC Y-Extent is out of bounds");

   NDC_AdjustExtents();

   return TRUE;
}

TFPoint TRealMapper::GetWorldOrg(void)
{
   return worldOrg;
}

TFSize TRealMapper::GetWorldExt(void)
{
   return worldExt;
}

TFPoint TRealMapper::GetNDCOrg(void)
{
   return ndcOrg;
}

TFSize TRealMapper::GetNDCExt(void)
{
   return ndcExt;
}

TRect TRealMapper::GetDeviceRect(void)
{
   return devRect;
}

//
// TRealMapper Drawing functions
//
#if defined (__WIN32__)
void TRealMapper::WPtoDP(double worldX,double worldY,LONG &devX,LONG &devY)
#else
void TRealMapper::WPtoDP(double worldX,double worldY,int &devX,int &devY)
#endif
{
   double ndcX, ndcY;

   WPtoNDC(worldX,worldY,ndcX,ndcY);
   NDCtoDP(ndcX,ndcY,devX,devY);
}

void TRealMapper::WPtoDP(TFPoint worldPoint,TPoint &devPoint)
{
   WPtoDP(worldPoint.x,worldPoint.y,devPoint.x,devPoint.y);
}

#if defined (__WIN32__)
void TRealMapper::DPtoWP(LONG devX,LONG devY,double &worldX,double &worldY)
#else
void TRealMapper::DPtoWP(int devX,int devY,double &worldX,double &worldY)
#endif
{
   double ndcX, ndcY;

   DPtoNDC(devX,devY,ndcX,ndcY);
   NDCtoWP(ndcX,ndcY,worldX,worldY);
}

void TRealMapper::DPtoWP(TPoint devPoint,TFPoint &worldPoint)
{
   DPtoWP(devPoint.x,devPoint.y,worldPoint.x,worldPoint.y);
}

//
// TRealMapper NDC functions
//
void TRealMapper::WPtoNDC(double worldX,double worldY,double &ndcX,double &ndcY)
{
   ndcX = (worldX - worldOrg.x)*(ndcExt.dx/worldExt.dx) + ndcOrg.x;
   ndcY = (worldY - worldOrg.y)*(ndcExt.dy/worldExt.dy) + ndcOrg.y;

   if (ndcX < 0 || ndcX > 1)
      ShowError("Error in WPtoNDC: ndcX < 0 or ndcX > 1");
   if (ndcY < 0 || ndcY > 1)
      ShowError("Error in WPtoNDC: ndcY < 0 or ndcY > 1");
}

void TRealMapper::WPtoNDC(const TFPoint &worldPoint,TFPoint &ndcPoint)
{
   WPtoNDC(worldPoint.x,worldPoint.y,ndcPoint.x,ndcPoint.y);
}

void TRealMapper::WPtoNDC(const TFPoint *worldPoint,TFPoint *ndcPoint,int count)
{
   int i;

   for (i = 0; i < count; i++)
      WPtoNDC(worldPoint[i],ndcPoint[i]);
}

void TRealMapper::NDCtoWP(double ndcX,double ndcY,double &worldX,double &worldY)
{
   worldX = (ndcX - ndcOrg.x)*(worldExt.dx/ndcExt.dx) + worldOrg.x;
   worldY = (ndcY - ndcOrg.y)*(worldExt.dy/ndcExt.dy) + worldOrg.y;
}

void TRealMapper::NDCtoWP(TFPoint ndcPoint,TFPoint &worldPoint)
{
   NDCtoWP(ndcPoint.x,ndcPoint.y,worldPoint.x,worldPoint.y);
}

#if defined (__WIN32__)
void TRealMapper::NDCtoDP(double ndcX,double ndcY,LONG &devX,LONG &devY)
#else
void TRealMapper::NDCtoDP(double ndcX,double ndcY,int &devX,int &devY)
#endif
{
   TFPoint devOrg;
   devOrg.x = ndcOrg.x*devExtX + devRect.left + ((devRect.right - devRect.left) - devExtX)/2;
   devOrg.y = ndcOrg.y*(-devExtY) + devRect.top + ((devRect.bottom - devRect.top) + devExtY)/2;
   devX = (ndcX - ndcOrg.x)*devExtX/ndcExt.dx + devOrg.x;
   devY = (ndcY - ndcOrg.y)*devExtY/ndcExt.dy + devOrg.y;
}

void TRealMapper::NDCtoDP(TFPoint ndcPoint,TPoint &devPoint)
{
   NDCtoDP(ndcPoint.x,ndcPoint.y,devPoint.x,devPoint.y);
}

void TRealMapper::NDCtoDP(const TFPoint *ndcPoint,TPoint *devPoint,int count)
{
   int i;

   for (i = 0; i < count; i++)
      NDCtoDP(ndcPoint[i],devPoint[i]);
}

#if defined (__WIN32__)
void TRealMapper::DPtoNDC(LONG devX,LONG devY,double &ndcX,double &ndcY)
#else
void TRealMapper::DPtoNDC(int devX,int devY,double &ndcX,double &ndcY)
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

void TRealMapper::DPtoNDC(TPoint devPoint,TFPoint &ndcPoint)
{
   DPtoNDC(devPoint.x,devPoint.y,ndcPoint.x,ndcPoint.y);
}

void TRealMapper::NDC_AdjustExtents(void)
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

   ndcX = fabs(worldExt.dx*(ndcExt.dy/worldExt.dy));
   ndcY = fabs(worldExt.dy*(ndcExt.dx/worldExt.dx));

   if (ndcX > fabs(ndcExt.dx))
      ndcExt.dy = ySign * fabs(ndcY);
   else
      ndcExt.dx = xSign * fabs(ndcX);

   double extX, extY;
   double Xsign, Ysign;

   // Scale down the device extents to the same proportions as the NDC extents
   Xsign = devExtX/abs(devExtX);
   Ysign = devExtY/abs(devExtY);

   extX = floor(fabs(devExtY * (ndcExt.dx/ndcExt.dy)));
   extY = floor(fabs(devExtX * (ndcExt.dy/ndcExt.dx)));

   if (extX > abs(devExtX))
      devExtY = Ysign * abs(extY);
   else
      devExtX = Xsign * abs(extX);
}

//
// TRealMapper Debug functions
//
void TRealMapper::ShowError(char *msg)
{
   ::MessageBox(0,msg,"Fatal Error",MB_OK | MB_ICONSTOP);
}

void TRealMapper::ShowWarning(char *msg)
{
   ::MessageBox(0,msg,"Warning",MB_OK | MB_ICONEXCLAMATION);
}