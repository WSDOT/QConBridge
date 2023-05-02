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

#include <shape\plategdr.h>

TPlateGirder::TPlateGirder(double tft,double tfw,double bft,double bfw,double tw,double hw,
              Material* material,
              TFPoint& rp,TLocationPoint lp) :
TSection()
{
   tTopFlange = tft;
   wTopFlange = tfw;
   tBotFlange = bft;
   wBotFlange = bfw;
   tWeb       = tw;
   hWeb       = hw;

   posRect = new TPosRect(hw+tft+bft,max(tfw,bfw),0.,rp,lp);

   GenerateSection(material);
}

TPlateGirder::~TPlateGirder()
{
// These are added to the sContainer, don't delete them explicitly
//   delete botFlange;
//   delete topFlange;
//   delete web;

   delete posRect;
}

void TPlateGirder::GenerateSection(Material* material)
{
   double angle;
   TFPoint origin;

   angle = posRect->GetRotationAngle();
   origin = posRect->GetBottomCenter();

   botFlange = new TRectangle(tBotFlange,wBotFlange,material,angle,origin,BottomCenter);
   AddShape(botFlange);

   topFlange = new TRectangle(tTopFlange,wTopFlange,material->Clone(),angle,origin+TFPoint(0,tBotFlange+hWeb),BottomCenter);
   AddShape(topFlange);

   web = new TRectangle(hWeb,tWeb,material->Clone(),angle,origin+TFPoint(0,tBotFlange),BottomCenter);
   AddShape(web);
}

void TPlateGirder::ComputeLocalProperties()
{
   TCentroid CG;

   TSection::ComputeLocalProperties();

   double Abf = botFlange->GetArea();
   CG = botFlange->GetCentroid();
   Qbf = Abf*(cg.Ybar - CG.Ybar);

   double Atf = topFlange->GetArea();
   CG = topFlange->GetCentroid();
   Qtf = Atf*(CG.Ybar - cg.Ybar);
}

double TPlateGirder::GetQbf()
{
   return Qbf;
}

double TPlateGirder::GetQtf()
{
   return Qtf;
}

//
// TCompPlateGirder
//
TCompPlateGirder::TCompPlateGirder(double tft,double tfw,double bft,double bfw,double tw,double hw,
                                   Material* matGirder,
                                   double ts,double ws,double so,double twear,
                                   Material* matSlab,
                                   TFPoint& rp,TLocationPoint lp) :
TSection()
{
   TFPoint wp;
   TPosRect* posRect;

   girder = new TPlateGirder(tft,tfw,bft,bfw,tw,hw,matGirder,rp,lp);
   AddShape(girder);

   posRect = girder->posRect;
   wp = posRect->GetTopCenter();
   slab = new TCompSlab(ts,ws,so,tfw,twear,wp,matSlab);
   AddShape(slab);
}

TCompPlateGirder::~TCompPlateGirder()
{
// These are added to the sContainer, don't delete them explicitly
//   delete slab;
//   delete girder;
}

void TCompPlateGirder::ComputeLocalProperties()
{
   TCentroid CG;

   TSection::ComputeLocalProperties();

   double Abf = girder->botFlange->GetArea();
   CG = girder->botFlange->GetCentroid();
   Qbf = Abf*(cg.Ybar - CG.Ybar);

   double Atf = girder->topFlange->GetArea();
   CG = girder->topFlange->GetCentroid();
   Qtf = Atf*(CG.Ybar - cg.Ybar);

   double n = slab->GetModE()/baseShape->GetModE();
   if (RoundToNearestN())
      RoundN(n);

   CG = slab->GetCentroid();
   Qslab = n*slab->GetArea()*(CG.Ybar - cg.Ybar);

   Qtfslab = Qslab + Qtf;
}

double TCompPlateGirder::GetQbf()
{
   return Qbf;
}

double TCompPlateGirder::GetQtf()
{
   return Qtf;
}

double TCompPlateGirder::GetQslab()
{
   return Qslab;
}

double TCompPlateGirder::GetQtfslab()
{
   return Qtfslab;
}

double TCompPlateGirder::GetTslab()
{
   return slab->GetTslab();
}

double TCompPlateGirder::GetWslab()
{
   return slab->GetWslab();
}

double TCompPlateGirder::GetSlabOffset()
{
   return slab->GetSlabOffset();
}

double TCompPlateGirder::GetTwearing()
{
   return slab->GetTwearing();
}

double TCompPlateGirder::GetEslab()
{
   return slab->GetModE();
}

double TCompPlateGirder::GetSlabUnitWeight()
{
   return slab->GetUnitWeight();
}

TPlateGirder& TCompPlateGirder::GetPlateGirder()
{
   return *girder;
}

