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

#if !defined SHAPE_SECTION_H_
#define SHAPE_SECTION_H_

#include <classlib/arrays.h>
#include <shape\shape.h>

typedef TIArrayAsVector<TShape>         TShapeContainer;
typedef TIArrayAsVectorIterator<TShape> TShapeContainerIterator;

class SHAPECLASS TSection : public TShape
{
// Data Members
private:
   TShapeContainer *sContainer;  // shapes
   bool   roundN;
   double grossArea;

protected:
   TShape* baseShape;

// Constructors/Destructor
public:
   TSection(bool roundN = false);
   ~TSection();

// Member Functions
public:
   bool RoundToNearestN(bool);
   bool RoundToNearestN();
   void RoundN(double &n);
   bool IsSolid(bool);
   bool IsStructural(bool);
   void AddShape(TShape* s);
   TShape* GetShape(int i);
   double GetGrossArea();
   void ComputeExtents();

   TShape* Clone() const;

#if defined(_Windows)
   virtual void Draw(TRealDC &dc);
   void SetColor(TColor Color);
#endif

protected:
   bool IsNullShape();
   void ComputeLocalProperties();
   void UpdateExtents(TShape* s);
   void InitProperties();
};

#endif // SHAPE_SECTION_H_
