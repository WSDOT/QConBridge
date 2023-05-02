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

#ifndef REALDC_HPP_
#define REALDC_HPP_

#include <owltools\owltldll.h>
#include <esl\geometry.h>
#include <winsys\geometry.h>
#include <owl\gdiobjec.h>

typedef TAnyPoint<double> TFPoint;
typedef TAnySize<double>  TFSize;
typedef TAnyRect<double>  TFRect;

class _OWLTOOLSCLASS TRealDC : public TDC
{
// Data Members
private:
   int     deviceMapMode;
   int     ndcMapMode;
   TFPoint worldOrg;
   TFSize  worldExt;
   TFPoint ndcOrg;
   TFSize  ndcExt;
   TFPoint devOrg;
   TRect   devRect;
#if defined (__WIN32__)
   LONG    devExtX;
   LONG    devExtY;
   LONG    devHeight;
   LONG    devWidth;
#else
   double  devExtX;
   double  devExtY;
   double  devHeight;
   double  devWidth;
#endif

// Constructors/Destructor
public:
   TRealDC(HDC dc);
   ~TRealDC(void);

// Methods
public:
   // Setup functions
   int  SetMapMode(int mm);
   void SetDeviceRect(int left,int top,int right,int bottom);
   void SetDeviceRect(TRect &rect);
   bool SetWorldOrg(double x,double y);
   bool SetWorldOrg(TFPoint &point);
   bool SetWorldExt(double dx,double dy);
   bool SetWorldExt(TFSize &extent);
   bool SetNDCOrg(double x,double y);
   bool SetNDCOrg(TFPoint &point);
   bool SetNDCExt(double dx,double dy);
   bool SetNDCExt(TFSize &extent);

   int     GetMapMode(void);
   TFPoint GetWorldOrg(void);
   TFSize  GetWorldExt(void);
   TFPoint GetNDCOrg(void);
   TFSize  GetNDCExt(void);

   // Drawing functions
#if defined (__WIN32__)
   void WPtoDP(double worldX,double worldY,LONG &devX,LONG &devY);
#else
   void WPtoDP(double worldX,double worldY,int &devX,int &devY);
#endif

   void WPtoDP(TFPoint worldPoint,TPoint &devPoint);

#if defined (__WIN32__)
   void DPtoWP(LONG devX,LONG devY,double &worldX,double &worldY);
#else
   void DPtoWP(int devX,int devY,double &worldX,double &worldY);
#endif

   void DPtoWP(TPoint devPoint,TFPoint &worldPoint);
   bool MoveTo(double worldX,double worldY);
   bool MoveTo(TFPoint &worldPoint);
   bool MoveTo(TFPoint *worldPoint);
   bool MoveTo(int x,int y);
   bool MoveTo(const TPoint &point);
   bool MoveTo(const TPoint &point,TPoint &oldPoint);
   bool LineTo(double worldX,double worldY);
   bool LineTo(TFPoint &worldPoint);
   bool LineTo(TFPoint *worldPoint);
   bool LineTo(int x,int y);
   bool LineTo(const TPoint &point);
   bool Polygon(const TFPoint *worldPoints,int count);
   bool Polygon(const TPoint *points,int count);
   bool Polyline(const double *worldX,const double *worldY,int count);
   bool Polyline(const TFPoint *worldPoints,int count);
   bool Polyline(const TPoint *points,int count);
   bool Rectangle(double left,double top,double right,double bottom);
   bool Rectangle(const TFPoint &p1,const TFPoint &p2);
   bool Rectangle(const TFPoint &point,const TFSize &size);
   bool Rectangle(const TFRect &rect);
   bool Rectangle(int x1,int y1,int x2,int y2);
   bool Rectangle(const TPoint &p1,const TPoint &p2);
   bool Rectangle(const TPoint &point,const TSize &s);
   bool Rectangle(const TRect &rect);
   bool Ellipse(double left,double top,double right,double bottom);
   bool Ellipse(const TFPoint &p1,const TFPoint &p2);
   bool Ellipse(const TFPoint &point,const TFSize &size);
   bool Ellipse(const TFRect &rect);
   bool Ellipse(int x1,int y1,int x2,int y2);
   bool Ellipse(const TPoint &p1,const TPoint &p2);
   bool Ellipse(const TPoint &point,const TSize &size);
   bool Ellipse(const TRect &rect);
   bool Circle(const TPoint &center,int radius);
   bool Circle(const TFPoint &center,double radius);
   bool Rotate(const TFPoint &center,const double angle,TFPoint *points,int count);

   // Text functions
   bool TextOut(const TFPoint& worldPoint,const char *string,int count = -1);
   bool TextOut(const TPoint &point,const char *string,int count = -1);
   bool TextOut(int x,int y,const char *string,int count = -1);

   // NDC functions
   void  WPtoNDC(double worldX,double worldY,double &ndcX,double &ndcY);
   void  WPtoNDC(const TFPoint &worldPoint,TFPoint &ndcPoint);
   void  WPtoNDC(const TFPoint *worldPoint,TFPoint *ndcPoint,int count = 1);

   void  NDCtoWP(double ndcX,double ndcY,double &worldX,double &worldY);
   void  NDCtoWP(TFPoint ndcPoint,TFPoint &worldPoint);

#if defined (__WIN32__)
   void  NDCtoDP(double ndcX,double ndcY,LONG &devX,LONG &devY);
#else
   void  NDCtoDP(double ndcX,double ndcY,int &devX,int &devY);
#endif
   void  NDCtoDP(TFPoint ndcPoint,TPoint &devPoint);
   void  NDCtoDP(const TFPoint *ndcPoint,TPoint *devPoint,int count = 1);

#if defined (__WIN32__)
   void  DPtoNDC(LONG devX,LONG devY,double &ndcX,double &ndcY);
#else
   void  DPtoNDC(int devX,int devY,double &ndcX,double &ndcY);
#endif

   void  DPtoNDC(TPoint devPoint,TFPoint &ndcPoint);

   void   NDC_SetMapMode(void);
   void   NDC_ResetMapMode(void);
   void   NDC_AdjustExtents(void);
   bool   NDC_MoveTo(double ndcX,double ndcY);
   bool   NDC_MoveTo(TFPoint ndcPoint);
   bool   NDC_LineTo(double ndcX,double ndcY);
   bool   NDC_LineTo(TFPoint ndcPoint);
   bool   NDC_Polygon(const TFPoint *ndcPoints,int count);
   bool   NDC_Polyline(const TFPoint *ndcPoints,int count);
   bool   NDC_Rectangle(double ndcLeft,double ndcTop,double ndcRight,double ndcBottom);
   bool   NDC_Rectangle(TFRect ndcRect);
   bool   NDC_Ellipse(double ndcLeft,double ndcTop,double ndcRight,double ndcBottom);
   bool   NDC_Ellipse(TFRect ndcRect);
   bool   NDC_TextOut(double ndcX,double ndcY,const char *string,int count = -1);

protected:
   // Debugging functions
   void ShowError(char *msg);
   void ShowWarning(char *msg);
};

#endif // REALDC_HPP_
