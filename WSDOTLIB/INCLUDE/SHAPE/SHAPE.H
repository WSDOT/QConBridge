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

#if !defined SHAPE_SHAPE_H_
#define SHAPE_SHAPE_H_

#include <shape\shapedll.h>
#include <shape\props.h>
#include <material\material.h>
#include <checks.h>
#include <shape\points.h>

#if defined (_Windows)
#include <owl\color.h>
#include <owltools\realdc.h>
#endif

class SHAPECLASS TShape
{
// Data Members
protected:
   // Status/States
   bool   isSolid;
   bool   isStructural;

   // Section Properties
   Material* material;
   double area;
   TCentroid cg;
   TInertia  Iglobal, Ilocal;
   TPrincipleInertia Ip;

   // Graphical Properties
   TFRect extents;
#if defined(_Windows)
   TColor color;
#endif

// Constructors/Destructor
public:
   TShape(Material* material);
   virtual ~TShape();

   bool operator == (const TShape&) const {return false;} // no two shapes are the same

// Member Functions
public: // Status/States
   bool IsSolid();
   bool IsSolid(bool);
   bool IsStructural();
   bool IsStructural(bool);

public: // Section Properties
   virtual void ComputeProperties();
   double GetModE();
   double GetUnitWeight();

   // Virtual ???
   virtual double GetArea();
   virtual double GetIxx();
   virtual double GetIyy();
   virtual TCentroid GetCentroid();
   virtual double GetXbar();
   virtual double GetYbar();
   virtual TInertia GetGlobalInertia();
   virtual TInertia GetCentroidalInertia();
   virtual TPrincipleInertia GetPrincipleInertia();


public: // Graphical Properties
   virtual void InitProperties();
   void GetExtents(double *xmin,double *xmax,double *ymin,double *ymax);
   void GetExtents(TFRect &r);
   void GetExtents(TFPoint &p,TFSize &size);
   virtual void ComputeExtents() = 0;

#if defined(_Windows)
   void SetColor(TColor Color);
   void SetColor(COLORREF Color);
   virtual void Draw(TRealDC &dc) = 0;
#endif

   virtual TShape* Clone() const = 0;

protected:
   virtual bool IsNullShape();

   virtual void ComputeLocalProperties() = 0;

private:
   void ComputeGlobalInertia();
   void ComputePrincipleInertia();
   void VerifySign();
};


#endif // SHAPE_SHAPE_H_
