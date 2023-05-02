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

#if !defined (MAPPER_H)
#define MAPPER_H

#include <owltools\owltldll.h>
#include <esl\geometry.h>
#include <owl/dc.h>
#include <classlib/arrays.h>

typedef TAnyPoint<double> TFPoint;
typedef TAnySize<double>  TFSize;
typedef TAnyRect<double>  TFRect;

typedef enum TMapMode {mmAnisotropic = 0x0001,
                       mmIsotropic   = 0x0002,
                       mmBestFitX    = 0x0004,
                       mmBestFitY    = 0x0008} TMapMode;

class _OWLTOOLSCLASS TMapper
{
public:
   TMapper();

   bool SetWorldExt(double dx,double dy);
   bool SetWorldExt(TFSize &extents);
   TFSize GetWorldExt() const;

   void SetWorldOrg(double x,double y);
   void SetWorldOrg(TFPoint &org);
   TFPoint GetWorldOrg() const;

   bool SetDeviceExt(int cx,int cy);
   bool SetDeviceExt(TSize &extents);
   TSize GetDeviceExt() const;

   void SetDeviceOrg(int x,int y);
   void SetDeviceOrg(TPoint &org);
   TPoint GetDeviceOrg() const;

   bool SetMapMode(TMapMode mm);
   TMapMode GetMapMode() const;

#if defined (__WIN32__)
   void WPtoDP(double worldX,double worldY,LONG &drawX,LONG &drawY);
#else
   void WPtoDP(double worldX,double worldY,int &drawX,int &drawY);
#endif
   void WPtoDP(TFPoint &world,TPoint &draw);

#if defined (__WIN32__)
   void DPtoWP(LONG drawX,LONG drawY,double &worldX,double &worldY);
#else
   void DPtoWP(int drawX,int drawY,double &worldX,double &worldY);
#endif
   void DPtoWP(TPoint &draw,TFPoint &world);

   void SetLogPixelsX(int lp) {LogPixelsX = lp;}
   int  GetLogPixelsX() const {return LogPixelsX;}
   void SetLogPixelsY(int lp) {LogPixelsY = lp;}
   int  GetLogPixelsY() const {return LogPixelsY;}

   double GetScaleX() const;
   double GetScaleY() const;

   void   AddScaleX(double scale);
   int    GetNumScaleX();
   double GetScaleX(int idx);
   void   ClearScaleX();

   void   AddScaleY(double scale);
   int    GetNumScaleY();
   double GetScaleY(int idx);
   void   ClearScaleY();

protected:
   TFSize worldExt;
   TFPoint worldOrg;
   TSize origDeviceExt;
   TSize deviceExt;
   TPoint deviceOrg;
   int LogPixelsX;
   int LogPixelsY;
   TMapMode mapMode;

   typedef TSArrayAsVector<double> ScaleArray;
   typedef TSArrayAsVectorIterator<double> ScaleIterator;

   ScaleArray Xscales;
   ScaleArray Yscales;

   bool ComputeBestFitScale(TMapMode fitMode,double scale,double &bfScale);
   bool UpdateDeviceExtents();
};

#endif // MAPPER_H