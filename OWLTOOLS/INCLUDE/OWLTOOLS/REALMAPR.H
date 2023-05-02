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

#ifndef REALMAPR_H_
#define REALMAPR_H_

#include <owltools\owltldll.h>
#include <esl\geometry.h>
#include <winsys\geometry.h>

typedef TAnyPoint<double> TFPoint;
typedef TAnySize<double>  TFSize;
typedef TAnyRect<double>  TFRect;

class _OWLTOOLSCLASS TRealMapper
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
   double  devExtX;
   double  devExtY;
   double  devHeight;
   double  devWidth;

// Constructors/Destructor
public:
   TRealMapper();
   virtual ~TRealMapper(void);

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
   TRect   GetDeviceRect(void);

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

   void  NDC_AdjustExtents(void);

protected:
   // Debugging functions
   void ShowError(char *msg);
   void ShowWarning(char *msg);
};

#endif // REALMAPR_H_