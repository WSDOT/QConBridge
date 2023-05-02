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

#include <shape\ibeam.h>
#include <shape\polygon.h>
#include <string.h>
#include <iostream.h>
#include <fstream.h>

TIBeam::TIBeam(double a,double b,double c,double d,double e,double f,double g,
               double h,double j,double v,double w,double x,double y,double z,
               Material* material,
               TFPoint& rp,TLocationPoint lp) :
TSection()
{
   A = a;
   B = b;
   C = c;
   D = d;
   E = e;
   F = f;
   G = g;
   H = h;
   J = j;
   V = v;
   W = w;
   X = x;
   Y = y;
   Z = z;

   posRect = new TPosRect(H,max(A,J),0.,rp,lp);
   GenerateSection(material);
}

TIBeam::~TIBeam()
{
   TRACE("TIBeam::~TIBeam()");
   delete posRect;
}

double TIBeam::GetA()
{
   return A;
}

double TIBeam::GetB()
{
   return B;
}

double TIBeam::GetC()
{
   return C;
}

double TIBeam::GetD()
{
   return D;
}

double TIBeam::GetE()
{
   return E;
}

double TIBeam::GetF()
{
   return F;
}

double TIBeam::GetG()
{
   return G;
}

double TIBeam::GetH()
{
   return H;
}

double TIBeam::GetJ()
{
   return J;
}

double TIBeam::GetV()
{
   return V;
}

double TIBeam::GetW()
{
   return W;
}

double TIBeam::GetX()
{
   return X;
}

double TIBeam::GetY()
{
   return Y;
}

double TIBeam::GetZ()
{
   return Z;
}

void TIBeam::GenerateSection(Material* material)
{
   ibeam = new TPolyShape(material,
                          posRect->GetBottomCenter(),
                          posRect->GetRotationAngle());

   ibeam->AddPoint(-J/2,0);
   ibeam->AddPoint(-J/2,G);
   ibeam->AddPoint(-W/2,G+F);
   ibeam->AddPoint(-V/2,G+F+E);
   ibeam->AddPoint(-V/2 - X,G+F+E+D);
   ibeam->AddPoint(-A/2,G+F+E+D+C);
   ibeam->AddPoint(-A/2,H);
   ibeam->AddPoint(A/2,H);
   ibeam->AddPoint(A/2,G+F+E+D+C);
   ibeam->AddPoint(V/2 + X,G+F+E+D);
   ibeam->AddPoint(V/2,G+F+E);
   ibeam->AddPoint(W/2,G+F);
   ibeam->AddPoint(J/2,G);
   ibeam->AddPoint(J/2,0);

   AddShape(ibeam);
}

//
// TCompIBeam
//
TCompIBeam::TCompIBeam(double a,double b,double c,double d,double e,double f,double g,
                       double h,double j,double v,double w,double x,double y,double z,
                       Material* matGirder,
                       double ts,double ws,double so,double tw,
                       Material* matSlab,
                       TFPoint& rp,TLocationPoint lp) :
TSection()
{
   TFPoint wp;
   TPosRect* posRect;

   ibeam = new TIBeam(a,b,c,d,e,f,g,h,j,v,w,x,y,z,matGirder,rp,lp);
   AddShape(ibeam);

   posRect = ibeam->posRect;
   wp = posRect->GetTopCenter();
   slab = new TCompSlab(ts,ws,so,a,tw,wp,matSlab);
   AddShape(slab);
}

void TCompIBeam::ComputeLocalProperties()
{
   TCentroid slabCG;

   TSection::ComputeLocalProperties();

   double n = slab->GetModE()/baseShape->GetModE();
   if (RoundToNearestN())
      RoundN(n);

   slabCG = slab->GetCentroid();
   Qslab = n*slab->GetArea()*(slabCG.Ybar - cg.Ybar);
}

double TCompIBeam::GetQslab()
{
   return Qslab;
}

double TCompIBeam::GetTslab()
{
   return slab->GetTslab();
}

double TCompIBeam::GetWslab()
{
   return slab->GetWslab();
}

double TCompIBeam::GetSlabOffset()
{
   return slab->GetSlabOffset();
}

double TCompIBeam::GetTwearing()
{
   return slab->GetTwearing();
}

double TCompIBeam::GetEslab()
{
   return slab->GetModE();
}

double TCompIBeam::GetSlabUnitWeight()
{
   return slab->GetUnitWeight();
}

#if defined (_Windows)
void TCompIBeam::SetColor(TColor beamColor,TColor slabColor)
{
   ibeam->SetColor(beamColor);
   slab->SetColor(slabColor);
}
#endif

