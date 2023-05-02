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

#include <math.h>
#include <shape\ibeam.h>
#include <shape\plategdr.h>
#include <shape\genshape.h>
#include <material\genmat.h>

void CompIBeam(void);
void SteelPlateGirder();
void CompositeBlob();
void ReportProps(TShape &s);

void main(void)
{
//   CompIBeam();
//   SteelPlateGirder();
   CompositeBlob();
}

void CompIBeam(void)
{
   Material* matGirder = new GeneralMaterial(4933.,160.);
   Material* matSlab   = new GeneralMaterial(4028.,160.);
   TCompIBeam  ibeam(43.,2.875,2.625,2.,57.,3.,6.,73.5,25.,6.,6.,2.,16.5,9.5, // W74G
                    matGirder,
                    7.0, // slab thickness
                    60.,  // slab width
                    7.0, // slab offset (top of girder to top of slab
                    0.0, // wearing surface (used for weight, not properties)
                    matSlab,
                    TFPoint(0,0), // Coordinate of reference point
                    BottomCenter); // Location of reference point in box

   ibeam.ComputeProperties();
   ReportProps(ibeam);

   cout << "Qslab = " << ibeam.GetQslab() << endl;
   cout << endl;

   TIBeam& beam = ibeam.GetIBeam();
   ReportProps(beam);
}

void SteelPlateGirder()
{
   GeneralMaterial* matGirder = new GeneralMaterial(29000.,490.);
   GeneralMaterial* matSlab   = new GeneralMaterial(29000./8,160.);

   TCompPlateGirder compGirder(1.25,22,2.25,22,0.438,122,matGirder,
                            7.0,84.,7.5,0,matSlab);
   compGirder.ComputeProperties();
   ReportProps(compGirder);

   TPlateGirder& girder = compGirder.GetPlateGirder();
   ReportProps(girder);
   cout << "Qtf = " << girder.GetQtf() << endl;
   cout << "Qbf = " << girder.GetQbf() << endl;
}

void CompositeBlob()
{
   GeneralMaterial* material = new GeneralMaterial(1.,1.);

   TIBeam*  ibeam = new TIBeam(43.,2.875,2.625,2.,57.,3.,6.,73.5,25.,6.,6.,2.,16.5,9.5, // W74G
                    material,
                    TFPoint(0,0), // Coordinate of reference point
                    BottomCenter); // Location of reference point in box
   TCompSlab* slab = new TCompSlab(7.0,84.,7.0,28.,0.0,TFPoint(0,73.5),material->Clone());

   TSection compSection;
   compSection.AddShape(ibeam);
   compSection.AddShape(slab);
   compSection.ComputeProperties();

   cout << "Blob" << endl;
   ReportProps(*ibeam);
   cout << "Slab" << endl;
   ReportProps(*slab);
   cout << "Composite Section" << endl;
   ReportProps(compSection);

}

void ReportProps(TShape &s)
{
   cout << "Area = " << s.GetArea() << endl;

   TCentroid cg;
   cg = s.GetCentroid();

   cout << "Xbar = " << cg.Xbar << " Ybar = " << cg.Ybar << endl;

   cout << "Moments of inertia about the centroid" << endl;
   TInertia i;
   i = s.GetCentroidalInertia();
   cout << "Ixx = " << i.Ixx << " Iyy = " << i.Iyy << " Ixy = " << i.Ixy << endl;

   cout << "Moments of inertia about the global axes" << endl;
   TInertia ig;
   ig = s.GetGlobalInertia();
   cout << "I'xx = " << ig.Ixx << " I'yy = " << ig.Iyy << " I'xy = " << ig.Ixy << endl;

   cout << "Principle moments of inertia and direction" << endl;
   TPrincipleInertia ip;
   ip = s.GetPrincipleInertia();
   cout << "I11 = " << ip.I11 << " I22 = " << ip.I22 << " I12 = " << ip.I12 << " dir = " << ip.pDir << endl;
}
