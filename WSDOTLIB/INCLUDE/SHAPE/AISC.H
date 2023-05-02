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

#ifndef AISC_HPP_
#define AISC_HPP_

#include <iostream.h>
#include <fstream.h>
#include "shape.hpp"
#include "posrect.hpp"

class TRolledShape : public TShape, public TPosRect
{
// Data Members
protected:
   char name[16];
   double ND,WEIGHT,AX,AY,AZ,YD,WBTK,
          ZD,FLTK,BF2TF,DTW,RT,YDAFL,IZ,
          SZ,SZS,RZ,IY,SY,SYS,RY,
          ZZ,ZY,IX,CW,INTYD,YC,ZC,
          EY,EZ,GRPNUM,SHAPE;

// Constructors/Destructor
public:
   TRolledShape(double Angle = 0.,
                TFPoint& rp = TFPoint(0,0),TLocationPoint lp = CenterCenter,
                TColor Color=RGB(0,0,0),double e = 1.,double d = 1.);

// Member Functions
public:
   void ComputeProperties(void);
   void ComputeExtents(void);
};

class TWShape : public TRolledShape
{
// Data Members

// Constructors/Destructor
public:
   TWShape(char *shape,double Angle = 0.,
           TFPoint& rp = TFPoint(0,0),TLocationPoint lp = CenterCenter,
           TColor Color=RGB(0,0,0),double e = 1.,double d = 1.);

// Member Functions
public:
   virtual void Draw(TOWLRealDC &dc);

protected:
   void SetPosRectDimensions(void);
   friend istream& operator >> (istream &s,TWShape &w);
};

class TChannel : public TRolledShape
{
// Data Members

// Constructors/Destructor
public:
   TChannel(char *shape,double Angle = 0.,
            TFPoint& rp = TFPoint(0,0),TLocationPoint lp = CenterCenter,
            TColor Color=RGB(0,0,0),double e = 1.,double d = 1.);

// Member Functions
public:
   virtual void Draw(TRealDC &dc);

protected:
   friend istream& operator >> (istream &s,TChannel &c);
};


class TTee : public TRolledShape
{
// Data Members

// Constructors/Destructor
public:
   TTee(char *shape,double Angle = 0.,
        TFPoint& rp = TFPoint(0,0),TLocationPoint lp = CenterCenter,
        TColor Color=RGB(0,0,0),double e = 1.,double d = 1.);

// Member Functions
public:
   virtual void Draw(TRealDC &dc);

protected:
   friend istream& operator >> (istream &s,TTee &t);
};

#endif // AISC_HPP_
