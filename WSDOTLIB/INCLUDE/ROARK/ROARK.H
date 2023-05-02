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

#if !defined ROARD_ROARK_H_
#define ROARK_ROARK_H_

/************************************************************************
   Refenence:
   Formulas For Stress and Strain 4th Edition R.J. Roark
   Table III pp 104-113

   Naming Convension:
      Class Names -
         Class Names are of the form [C|P|F][C|P|F]{loadtype}
         where :
         [C|P|F] = Cantilever, Pinned, or Fixed end support
         the first one is for the left support and the second for the right
         support.
         {loadtype} can be EndLoad, IntermediateLoad,UniformLoad,PartUniformLoad,
         LeftTriangle,PartLeftTriangle,RightTriangle,PartRightTriangle,
         EndCouple,IntermediateCouple,CenterLoad, or CenterCouple

         Example:  PFRightTriangle - Pinned-Fixed beam with a right triangle
                   load. See Loading 25.
*************************************************************************/

#if defined (BUILDROARK) && !defined(ROARKLIB)
#define _ROARKCLASS __declspec(dllexport)
#elif defined(ROARKLIB)
#define _ROARKCLASS
#else
#define _ROARKCLASS __declspec(dllimport)
#endif

#include <math.h>

class _ROARKCLASS RoarkBeam
{
// Data Members
protected:
   double L,EI;

// Constructors/Destructor
public:
   RoarkBeam(double length,double e,double i);

// Member Functions
public:
   virtual void GetReactions(double &Ra,double &Rb) = 0;
   virtual void GetMoments(double &Ma,double &Mb) = 0;
   virtual void GetRotations(double &ra,double &rb) = 0;
   virtual void GetDeflections(double &Ya,double &Yb) = 0;

   virtual double ComputeShear(double x) = 0;
   virtual double ComputeMoment(double x) = 0;
   virtual double ComputeDeflection(double x) = 0;
};

/*
class _ROARKCLASS CFEndLoad : public RoarkBeam // Loading 1
{
// Data Members
// Constructors/Destructor
// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};
*/

class _ROARKCLASS CFEndLoad : public RoarkBeam // Loading 1
{
// Data Members
protected:
   double W;

// Constructors/Destructor
public:
   CFEndLoad(double w,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

class _ROARKCLASS CFIntermediateLoad : public RoarkBeam // Loading 2
{
// Data Members
protected:
   double a,b,W;

// Constructors/Destructor
public:
   CFIntermediateLoad(double w,double Lb,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

class _ROARKCLASS PPIntermediateLoad : public RoarkBeam // Loading 12
{
// Data Members
protected:
   double a,b,W;

// Constructors/Destructor
public:
   PPIntermediateLoad(double w,double La,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

class _ROARKCLASS PPPartUniformLoad : public RoarkBeam // Loading 14
{
// Data Members
protected:
   double a,b,c,d,W;

// Constructors/Destructor
public:
   PPPartUniformLoad(double La,double Lb,double w,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

class _ROARKCLASS PPPartRightTriangleLoad : public RoarkBeam // Loading 16
{
// Data Members
protected:
   double a,b,c,d,W;

// Constructors/Destructor
public:
   PPPartRightTriangleLoad(double La,double Lb,double w,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

class _ROARKCLASS PPPartLeftTriangleLoad : public PPPartRightTriangleLoad // Loading 16 - reversed
{
// Data Members
protected:
   double a,b,c,d,W;

// Constructors/Destructor
public:
   PPPartLeftTriangleLoad(double La,double Lb,double w,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

class _ROARKCLASS PPIntermediateCouple : public RoarkBeam // Loading 20
{
// Data Members
protected:
   double a, Mo;

// Constructors/Destructor
public:
   PPIntermediateCouple(double M,double La,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

class _ROARKCLASS PFIntermediateLoad : public RoarkBeam // Loading 22
{
// Data Members
protected:
   double a,W;

// Constructors/Destructor
public:
   PFIntermediateLoad(double w,double Lb,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

class _ROARKCLASS FPIntermediateLoad : public PFIntermediateLoad // Loading 22 - reversed
{
// Data Members

// Constructors/Destructor
public:
   FPIntermediateLoad(double w,double Lb,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

class _ROARKCLASS PFPartUniformLoad : public RoarkBeam // Loading 24
{
// Notes:
// La and Lb are measure from the left hand suppport (the pinned end).
// Roark measures a and b from the right support (the fixed end).
// I have choosen to reference the left support for consistancy.

// Data Members
protected:
   double a,b,c,d,W;

// Constructors/Destructor
public:
   PFPartUniformLoad(double w,double La,double Lb,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

class _ROARKCLASS FPPartUniformLoad : public PFPartUniformLoad // Loading 24 - reversed
{
// Notes:
// La and Lb are measure from the left hand suppport (the fixed end).
// This case is an exact mirror image of the case shown for reference number 24.

// Data Members
protected:

// Constructors/Destructor
public:
   FPPartUniformLoad(double w,double La,double Lb,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

class _ROARKCLASS PFPartRightTriangleLoad : public RoarkBeam // Loading 26
{
// Notes:
// La and Lb are measure from the left hand suppport (the pinned end).
// Roark measures a and b from the right support (the fixed end).
// I have choosen to reference the left support for consistancy.

// Data Members
protected:
   double a,b,c,d,W;

// Constructors/Destructor
public:
   PFPartRightTriangleLoad(double w,double La,double Lb,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

class _ROARKCLASS FPPartLeftTriangleLoad : public PFPartRightTriangleLoad // Loading 26 - reversed
{
// Notes:
// La and Lb are measure from the left hand suppport (the fixed end).
// This case is an exact mirror image of the case shown for reference number 26.

// Data Members

// Constructors/Destructor
public:
   FPPartLeftTriangleLoad(double w,double La,double Lb,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

class _ROARKCLASS PFPartLeftTriangleLoad : public RoarkBeam // Loading 28
{
// Notes:
// La and Lb are measure from the left hand suppport (the pinned end).
// Roark measures a and b from the right support (the fixed end).
// I have choosen to reference the left support for consistancy.

// Data Members
protected:
   double a,b,c,d,W;

// Constructors/Destructor
public:
   PFPartLeftTriangleLoad(double w,double La,double Lb,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

class _ROARKCLASS FPPartRightTriangleLoad : public PFPartLeftTriangleLoad // Loading 28 - reversed
{
// Notes:
// La and Lb are measure from the left hand suppport (the fixed end).
// This case is an exact mirror image of the case shown for reference number 28.

// Data Members

// Constructors/Destructor
public:
   FPPartRightTriangleLoad(double w,double La,double Lb,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

class _ROARKCLASS PFIntermediateCouple : public RoarkBeam // Loading 30
{
// Data Members
protected:
   double a, Mo;

// Constructors/Destructor
public:
   PFIntermediateCouple(double M,double La,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

class _ROARKCLASS FPIntermediateCouple : public PFIntermediateCouple // Loading 30 - reversed
{
// Data Members

// Constructors/Destructor
public:
   FPIntermediateCouple(double M,double La,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

class _ROARKCLASS FFIntermediateLoad : public RoarkBeam // Loading 32
{
// Data Members
protected:
   double a,b,c,d,W;

// Constructors/Destructor
public:
   FFIntermediateLoad(double w,double La,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

class _ROARKCLASS FFPartUniformLoad : public RoarkBeam // Loading 34
{
// Data Members
protected:
   double a,b,c,d,W;

// Constructors/Destructor
public:
   FFPartUniformLoad(double La,double Lb,double w,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

class _ROARKCLASS FFPartRightTriangleLoad : public RoarkBeam // Loading 36
{
// Data Members
protected:
   double a,b,c,d,W;

// Constructors/Destructor
public:
   FFPartRightTriangleLoad(double La,double Lb,double w,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

class _ROARKCLASS FFPartLeftTriangleLoad : public FFPartRightTriangleLoad // Loading 36 - reversed
{
// Data Members

// Constructors/Destructor
public:
   FFPartLeftTriangleLoad(double La,double Lb,double w,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

class _ROARKCLASS FFIntermediateCouple : public RoarkBeam // Loading 37
{
// Data Members
protected:
   double a, Mo;

// Constructors/Destructor
public:
   FFIntermediateCouple(double M,double La,double l,double e,double i);

// Member Functions
public:
   void GetReactions(double &Ra,double &Rb);
   void GetMoments(double &Ma,double &Mb);
   void GetRotations(double &ra,double &rb);
   void GetDeflections(double &Ya,double &Yb);

   double ComputeShear(double x);
   double ComputeMoment(double x);
   double ComputeDeflection(double x);
};

#endif // ROARK_ROARK_H_
