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

#include <owltools\mapper.h>
#include <math.h>
#include <checks.h>

#define RoundOff(x) ( ((x) - int((x)) > 0.5) ? ceil((x)) : floor((x)) )

TMapper::TMapper() :
worldExt(0,0),
worldOrg(0,0),
origDeviceExt(0,0),
deviceExt(0,0),
deviceOrg(0,0),
mapMode(mmAnisotropic),
Xscales(6,0,3),
Yscales(6,0,3),
LogPixelsX(0),
LogPixelsY(0)
{
}

bool TMapper::SetWorldExt(double dx,double dy)
{
   return SetWorldExt(TFSize(dx,dy));
}

bool TMapper::SetWorldExt(TFSize &extents)
{
   worldExt = extents;
   return UpdateDeviceExtents();
}

TFSize TMapper::GetWorldExt() const
{
   return worldExt;
}

void TMapper::SetWorldOrg(double x,double y)
{
   SetWorldOrg(TFPoint(x,y));
}

void TMapper::SetWorldOrg(TFPoint &org)
{
   worldOrg = org;
}

TFPoint TMapper::GetWorldOrg() const
{
   return worldOrg;
}

bool TMapper::SetDeviceExt(int cx,int cy)
{
   return SetDeviceExt(TSize(cx,cy));
}

bool TMapper::SetDeviceExt(TSize &extents)
{
   origDeviceExt = extents;
   return UpdateDeviceExtents();
}

TSize TMapper::GetDeviceExt() const
{
   return deviceExt;
}

void TMapper::SetDeviceOrg(int x,int y)
{
   SetDeviceOrg(TPoint(x,y));
}

void TMapper::SetDeviceOrg(TPoint &org)
{
   deviceOrg = org;
}

TPoint TMapper::GetDeviceOrg() const
{
   return deviceOrg;
}


bool TMapper::SetMapMode(TMapMode mm)
{
#if (__DEBUG > 0) // PRECONDITION
   bool anisotropic = mm & mmAnisotropic;
   bool isotropic   = mm & mmIsotropic;

   // must be anisotropic or isotropic, but not both
   // cannot be bestFit without anisotropic or isotropic
   PRECONDITIONX(anisotropic ||  isotropic,"TMapper::SetMapMode() - invalid mapping mode ({mmAnisotropic,mmIsotropic},{mmBestFit})");
   PRECONDITIONX((anisotropic && !isotropic) || (!anisotropic && isotropic),"TMapper::SetMapMode() - invalid mapping mode ({mmAnisotropic,mmIsotropic},{mmBestFit})");
#endif

   mapMode = mm;
   return UpdateDeviceExtents();
}

TMapMode TMapper::GetMapMode() const
{
   return  mapMode;
}

#if defined (__WIN32__)
void TMapper::WPtoDP(double xWorld,double yWorld,LONG &xDevice,LONG &yDevice)
#else
void TMapper::WPtoDP(double xWorld,double yWorld,int &xDevice,int &yDevice)
#endif
{
   if (worldExt.dx == 0)
      xDevice = deviceOrg.x;
   else
      xDevice = RoundOff((xWorld - worldOrg.x)*((double)deviceExt.cx/worldExt.dx) + deviceOrg.x);

   if (worldExt.dy == 0)
      yDevice = deviceOrg.y;
   else
      yDevice = RoundOff((yWorld - worldOrg.y)*((double)deviceExt.cy/(-worldExt.dy)) + deviceOrg.y);
}

void TMapper::WPtoDP(TFPoint &world,TPoint &device)
{
   WPtoDP(world.x,world.y,device.x,device.y);
}

#if defined (__WIN32__)
void TMapper::DPtoWP(LONG xDevice,LONG yDevice,double &xWorld,double &yWorld)
#else
void TMapper::DPtoWP(int xDevice,int yDevice,double &xWorld,double &yWorld)
#endif
{
   if (deviceExt.cx == 0)
      xWorld = worldOrg.x;
   else
      xWorld = (xDevice - deviceOrg.x)*(worldExt.dx/(double)deviceExt.cx) + worldOrg.x;

   if (deviceExt.cy == 0)
      yWorld = worldOrg.y;
   else
      yWorld = (yDevice - deviceOrg.y)*((-worldExt.dy)/(double)deviceExt.cy) + worldOrg.y;
}

void TMapper::DPtoWP(TPoint &device,TFPoint &world)
{
   DPtoWP(device.x,device.y,world.x,world.y);
}

double TMapper::GetScaleX() const
{
   double scaleX;

   if (deviceExt.cx == 0)
      scaleX = 0.;
   else
      scaleX = fabs(worldExt.dx*LogPixelsX/25.4/deviceExt.cx);

   return scaleX;
}

double TMapper::GetScaleY() const
{
   double scaleY;

   if (deviceExt.cy == 0)
      scaleY = 0.;
   else
      scaleY = fabs(worldExt.dy*LogPixelsY/25.4/deviceExt.cy);

   return scaleY;
}

void TMapper::AddScaleX(double scale)
{
   PRECONDITIONX(0. < scale && scale < 1.,"TMapper::AddScaleX() - invalid range (0,1)");
   Xscales.Add(scale);
}

int TMapper::GetNumScaleX()
{
   return Xscales.GetItemsInContainer();
}

double TMapper::GetScaleX(int idx)
{
   return Xscales[idx];
}

void TMapper::ClearScaleX()
{
   Xscales.Flush();
}

void TMapper::AddScaleY(double scale)
{
   PRECONDITIONX(0. < scale && scale < 1.,"TMapper::AddScaleY() - invalid range (0,1)");
   Yscales.Add(scale);
}

int TMapper::GetNumScaleY()
{
   return Yscales.GetItemsInContainer();
}

double TMapper::GetScaleY(int idx)
{
   return Yscales[idx];
}

void TMapper::ClearScaleY()
{
   Yscales.Flush();
}

bool TMapper::ComputeBestFitScale(TMapMode fitMode,double scale,double &bfScale)
{
   int exp;
   double normScale;
   double bfNormScale;
   double maxScale;

   if (scale == 0)
      {
      bfScale = scale;
      return true;
      }

   exp = int(log10(scale));
   if (scale >= 1)
      exp += 1;

   normScale = scale * pow(10,-exp);
   int numScales = (fitMode == mmBestFitX) ? GetNumScaleX() : GetNumScaleY();
   if (fitMode == mmBestFitX)
      maxScale = Xscales[numScales-1];
   else
      maxScale = Yscales[numScales-1];

   while (normScale > maxScale)
      {
      normScale/=10;
      exp++;
      }

   // find best fit scale from available scales
   // assumes scales are sorted least to most.
   // Find first scale that is larger that or equal to normScale
   int i;
   bool found;
   for (i = 0,found = false; i < numScales && !found; i++)
      {
      bfNormScale = (fitMode == mmBestFitX) ? GetScaleX(i) : GetScaleY(i);
      if (normScale <= bfNormScale)
         found = true;
      }
   // Not found... best fit cannot be made with available scales
   if (!found)
      return false;

   bfScale = bfNormScale * pow(10,exp);

   return true;
}

bool TMapper::UpdateDeviceExtents()
{
   bool anisotropic = mapMode & mmAnisotropic;
   bool isotropic   = mapMode & mmIsotropic;
   bool bestFitX    = mapMode & mmBestFitX;
   bool bestFitY    = mapMode & mmBestFitY;
   TPoint topLeft;
   TFSize nWorldExt;   // no-sign world extents
   TSize  nDeviceExt;  // no-sign device extents
   double scaleX;
   double scaleY;
   double scale;

   //
   // Copy world and device extents, without sign.
   // Device ratio-ing must be done without signs
   //
   nWorldExt.dx  = fabs(worldExt.dx);
   nWorldExt.dy  = fabs(worldExt.dy);
   nDeviceExt.cx = abs(origDeviceExt.cx);
   nDeviceExt.cy = abs(origDeviceExt.cy);

   //
   // Adjust size for mapping mode
   //
   if (anisotropic)
      {
      if (bestFitX)
         {
         scaleX = nWorldExt.dx*LogPixelsX/25.4/nDeviceExt.cx;

         if (!ComputeBestFitScale(mmBestFitX,scaleX,scaleX))
            return false;

         if (scaleX == 0)
            nDeviceExt.cx = 0;
         else
            nDeviceExt.cx = RoundOff(nWorldExt.dx*LogPixelsX/25.4/scaleX);
         }

      if (bestFitY)
         {
         scaleY = nWorldExt.dy*LogPixelsY/25.4/nDeviceExt.cy;

         if (!ComputeBestFitScale(mmBestFitY,scaleY,scaleY))
            return false;

         if (scaleY == 0)
            nDeviceExt.cy = 0;
         else
            nDeviceExt.cy = RoundOff(nWorldExt.dy*LogPixelsX/25.4/scaleY);
         }
      }
   else if (isotropic)
      {
      double s1,s2;
      s1 = fabs(nWorldExt.dx*nDeviceExt.cy);
      s2 = fabs(nWorldExt.dy*nDeviceExt.cx);

      if (s1 > s2)
         {
         // worldExt.dx will not equal zero
         CHECKX(nWorldExt.dx != 0,"TMapper::UpdateDrawingRect() - nWorldExt.dx == 0 - possible logic flaw");
         nDeviceExt.cy = RoundOff(((double)nDeviceExt.cx*nWorldExt.dy)/nWorldExt.dx);
         }
      else if (s1 < s2)
         {
         // worldExt.dy will not equal zero
         CHECKX(nWorldExt.dy != 0,"TMapper::UpdateDrawingRect() - nWorldExt.dy == 0 - possible logic flaw");
         nDeviceExt.cx = RoundOff(((double)nDeviceExt.cy*nWorldExt.dx)/nWorldExt.dy);
         }
      else
         {
         // do nothing
         }

      if (bestFitX || bestFitY)
         {
         // Both scales will be the same except in the special case when
         // one or both of the scales are zero
         if (nDeviceExt.cx != 0)
            scale = nWorldExt.dx*LogPixelsX/25.4/nDeviceExt.cx;
         else if (nDeviceExt.cy != 0)
            scale = nWorldExt.dy*LogPixelsY/25.4/nDeviceExt.cy;
         else
            {
            scale = 0;
            }

         if (!ComputeBestFitScale((bestFitX) ? mmBestFitX : mmBestFitY,scale,scale))
            return false;

         if (scale == 0)
            {
            nDeviceExt.cx = 0;
            nDeviceExt.cy = 0;
            }
         else
            {
            nDeviceExt.cx = RoundOff(nWorldExt.dx*LogPixelsX/25.4/scale);
            nDeviceExt.cy = RoundOff(nWorldExt.dy*LogPixelsY/25.4/scale);
            }
         }
      }
   else
      {
      #pragma warn -ccc
      CHECKX(0,"Invalid mapping mode detected");
      #pragma warn .ccc
      }

   // Copy new device extents to deviceExt and recall original signs
   deviceExt.cx = nDeviceExt.cx;
   deviceExt.cx *= (origDeviceExt.cx < 0) ? -1 : 1;
   deviceExt.cy = nDeviceExt.cy;
   deviceExt.cy *= (origDeviceExt.cy < 0) ? -1 : 1;

   return true;
}

