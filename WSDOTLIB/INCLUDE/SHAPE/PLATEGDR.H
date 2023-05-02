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

#if !defined SHAPE_PLATEGDR_H_
#define SHAPE_PLATEGDR_H_

#include <shape\section.h>
#include <shape\compslab.h>
#include <shape\posrect.h>

class SHAPECLASS TPlateGirder : public TSection
{
// Data Members
protected:
   TPosRect *posRect;
   TRectangle *topFlange;
   TRectangle *botFlange;
   TRectangle *web;
   double tTopFlange;
   double wTopFlange;
   double tBotFlange;
   double wBotFlange;
   double tWeb;
   double hWeb;
   double Qtf;
   double Qbf;

// Constructors/Destructor
public:
   TPlateGirder(double tft,double tfw,double bft,double bfw,double tw,double hw,
                Material* material,
                TFPoint& rp = TFPoint(0,0),TLocationPoint lp = BottomCenter);
   ~TPlateGirder();

// Member Functions
public:
   double GetQbf();
   double GetQtf();

   double GetHeight() {return tTopFlange+hWeb+tBotFlange;}
   double GetTopFlangeWidth() {return wTopFlange;}
   double GetWebWidth() {return tWeb;}

protected:
   void ComputeLocalProperties();
   void GenerateSection(Material* material);

   friend class SHAPECLASS TCompPlateGirder;
};

class SHAPECLASS TCompPlateGirder : public TSection
{
// Data Members
protected:
   TCompSlab* slab;
   TPlateGirder* girder;
   double Qtf, Qbf;
   double Qslab;
   double Qtfslab; // top flange + slab

// Constructors/Destructor
public:
   TCompPlateGirder(double tft,double tfw,double bft,double bfw,double tw,double hw,
                    Material* matGirder,
                    double ts,double ws,double so,double twear,
                    Material* matSlab,
                    TFPoint& rp = TFPoint(0,0),TLocationPoint lp = BottomCenter);
   ~TCompPlateGirder();

// Member Functions
public:
   double GetQbf();
   double GetQtf();
   double GetQslab();
   double GetQtfslab();

   double GetTslab();
   double GetWslab();
   double GetSlabOffset();
   double GetTwearing();
   double GetEslab();
   double GetSlabUnitWeight();
   TPlateGirder& GetPlateGirder();

protected:
   void ComputeLocalProperties();
};

#endif // SHAPE_PLATEGDR_H_
