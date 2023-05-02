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

#include <fem2d\beam.h>

//
//
//
TConcLdBeam::TConcLdBeam(double q,double la,LoadDir dir,double l,double ax,double iz,double e,MbrType type) :
TBeam(l,ax,iz,e,type)
{
   Q = q;
   Dir = dir;
   a = la;
   b = L - a;

   ComputeReactions();
   ComputeDisplacements();
}

void TConcLdBeam::GetLoad(double &ld)
{
   ld = Q;
}

void TConcLdBeam::GetLocation(double &loc)
{
   loc = a;
}

LoadDir TConcLdBeam::GetDirection()
{
   return Dir;
}

void TConcLdBeam::GetReactions(double &fxl,double &fyl,double &mzl,
                               double &fxr,double &fyr,double &mzr)
{
   fxl = Fxl;
   fyl = Fyl;
   mzl = Mzl;
   fxr = Fxr;
   fyr = Fyr;
   mzr = Mzr;
}

void TConcLdBeam::GetDisplacements(double &dxl,double &dyl,double &rzl,
                                   double &dxr,double &dyr,double &rzr)
{
   dxl = Dxl;
   dyl = Dyl;
   rzl = Rzl;
   dxr = Dxr;
   dyr = Dyr;
   rzr = Rzr;
}

void TConcLdBeam::GetForces(double x,double &fx,double &fy,double &mz)
{
   switch(Type)
      {
      case mtPinPin:
           ComputePPForces(x,fx,fy,mz);
           break;

      case mtPinFix:
           ComputePFForces(x,fx,fy,mz);
           break;

      case mtFixPin:
           ComputeFPForces(x,fx,fy,mz);
           break;

      case mtFixFix:
           ComputeFFForces(x,fx,fy,mz);
           break;
      }
}

void TConcLdBeam::GetDeflection(double x,double &dx,double &dy,double &rz)
{
   switch(Type)
      {
      case mtPinPin:
           ComputePPDeflection(x,dx,dy,rz);
           break;

      case mtPinFix:
           ComputePFDeflection(x,dx,dy,rz);
           break;

      case mtFixPin:
           ComputeFPDeflection(x,dx,dy,rz);
           break;

      case mtFixFix:
           ComputeFFDeflection(x,dx,dy,rz);
           break;
      }
}

void TConcLdBeam::ComputeReactions()
{
   switch(Type)
      {
      case mtPinPin:
           ComputePPReactions();
           break;

      case mtPinFix:
           ComputePFReactions();
           break;

      case mtFixPin:
           ComputeFPReactions();
           break;

      case mtFixFix:
           ComputeFFReactions();
           break;
      }
}

void TConcLdBeam::ComputeDisplacements()
{
   switch(Type)
      {
      case mtPinPin:
           ComputePPDisplacements();
           break;

      case mtPinFix:
           ComputePFDisplacements();
           break;

      case mtFixPin:
           ComputeFPDisplacements();
           break;

      case mtFixFix:
           ComputeFFDisplacements();
           break;
      }
}

void TConcLdBeam::ComputePPReactions()
{
   switch(Dir)
      {
      case ForceX:
           ComputePPReactionsFx();
           break;

      case ForceY:
           ComputePPReactionsFy();
           break;

      case MomentZ:
           ComputePPReactionsMz();
           break;
      }
}

void TConcLdBeam::ComputePFReactions()
{
   switch(Dir)
      {
      case ForceX:
           ComputePFReactionsFx();
           break;

      case ForceY:
           ComputePFReactionsFy();
           break;

      case MomentZ:
           ComputePFReactionsMz();
           break;
      }
}

void TConcLdBeam::ComputeFPReactions()
{
   switch(Dir)
      {
      case ForceX:
           ComputeFPReactionsFx();
           break;

      case ForceY:
           ComputeFPReactionsFy();
           break;

      case MomentZ:
           ComputeFPReactionsMz();
           break;
      }
}

void TConcLdBeam::ComputeFFReactions()
{
   switch(Dir)
      {
      case ForceX:
           ComputeFFReactionsFx();
           break;

      case ForceY:
           ComputeFFReactionsFy();
           break;

      case MomentZ:
           ComputeFFReactionsMz();
           break;
      }
}

void TConcLdBeam::ComputePPDisplacements()
{
   switch(Dir)
      {
      case ForceX:
           ComputePPDisplacementsFx();
           break;

      case ForceY:
           ComputePPDisplacementsFy();
           break;

      case MomentZ:
           ComputePPDisplacementsMz();
           break;
      }
}

void TConcLdBeam::ComputePFDisplacements()
{
   switch(Dir)
      {
      case ForceX:
           ComputePFDisplacementsFx();
           break;

      case ForceY:
           ComputePFDisplacementsFy();
           break;

      case MomentZ:
           ComputePFDisplacementsMz();
           break;
      }
}

void TConcLdBeam::ComputeFPDisplacements()
{
   switch(Dir)
      {
      case ForceX:
           ComputeFPDisplacementsFx();
           break;

      case ForceY:
           ComputeFPDisplacementsFy();
           break;

      case MomentZ:
           ComputeFPDisplacementsMz();
           break;
      }
}

void TConcLdBeam::ComputeFFDisplacements()
{
   switch(Dir)
      {
      case ForceX:
           ComputeFFDisplacementsFx();
           break;

      case ForceY:
           ComputeFFDisplacementsFy();
           break;

      case MomentZ:
           ComputeFFDisplacementsMz();
           break;
      }
}

void TConcLdBeam::ComputePPForces(double x,double &fx,double &fy,double &mz)
{
   switch(Dir)
      {
      case ForceX:
           ComputePPForcesFx(x,fx,fy,mz);
           break;

      case ForceY:
           ComputePPForcesFy(x,fx,fy,mz);
           break;

      case MomentZ:
           ComputePPForcesMz(x,fx,fy,mz);
           break;
      }
}

void TConcLdBeam::ComputePFForces(double x,double &fx,double &fy,double &mz)
{
   switch(Dir)
      {
      case ForceX:
           ComputePFForcesFx(x,fx,fy,mz);
           break;

      case ForceY:
           ComputePFForcesFy(x,fx,fy,mz);
           break;

      case MomentZ:
           ComputePFForcesMz(x,fx,fy,mz);
           break;
      }
}

void TConcLdBeam::ComputeFPForces(double x,double &fx,double &fy,double &mz)
{
   switch(Dir)
      {
      case ForceX:
           ComputeFPForcesFx(x,fx,fy,mz);
           break;

      case ForceY:
           ComputeFPForcesFy(x,fx,fy,mz);
           break;

      case MomentZ:
           ComputeFPForcesMz(x,fx,fy,mz);
           break;
      }
}

void TConcLdBeam::ComputeFFForces(double x,double &fx,double &fy,double &mz)
{
   switch(Dir)
      {
      case ForceX:
           ComputeFFForcesFx(x,fx,fy,mz);
           break;

      case ForceY:
           ComputeFFForcesFy(x,fx,fy,mz);
           break;

      case MomentZ:
           ComputeFFForcesMz(x,fx,fy,mz);
           break;
      }
}

void TConcLdBeam::ComputePPDeflection(double x,double &dx,double &dy,double &rz)
{
   switch(Dir)
      {
      case ForceX:
           ComputePPDeflectionFx(x,dx,dy,rz);
           break;

      case ForceY:
           ComputePPDeflectionFy(x,dx,dy,rz);
           break;

      case MomentZ:
           ComputePPDeflectionMz(x,dx,dy,rz);
           break;
      }
}

void TConcLdBeam::ComputePFDeflection(double x,double &dx,double &dy,double &rz)
{
   switch(Dir)
      {
      case ForceX:
           ComputePFDeflectionFx(x,dx,dy,rz);
           break;

      case ForceY:
           ComputePFDeflectionFy(x,dx,dy,rz);
           break;

      case MomentZ:
           ComputePFDeflectionMz(x,dx,dy,rz);
           break;
      }
}

void TConcLdBeam::ComputeFPDeflection(double x,double &dx,double &dy,double &rz)
{
   switch(Dir)
      {
      case ForceX:
           ComputeFPDeflectionFx(x,dx,dy,rz);
           break;

      case ForceY:
           ComputeFPDeflectionFy(x,dx,dy,rz);
           break;

      case MomentZ:
           ComputeFPDeflectionMz(x,dx,dy,rz);
           break;
      }
}

void TConcLdBeam::ComputeFFDeflection(double x,double &dx,double &dy,double &rz)
{
   switch(Dir)
      {
      case ForceX:
           ComputeFFDeflectionFx(x,dx,dy,rz);
           break;

      case ForceY:
           ComputeFFDeflectionFy(x,dx,dy,rz);
           break;

      case MomentZ:
           ComputeFFDeflectionMz(x,dx,dy,rz);
           break;
      }
}

void TConcLdBeam::ComputePPReactionsFx()
{
  Fxl = -Q*b/L;
  Fyl = 0;
  Mzl = 0;

  Fxr = -Q*a/L;
  Fyr = 0;
  Mzr = 0;
}

void TConcLdBeam::ComputePPReactionsFy()
{
  Fxl = 0;
  Fyl = -Q*b/L;
  Mzl = 0;

  Fxr = 0;
  Fyr = -Q*a/L;
  Mzr = 0;
}

void TConcLdBeam::ComputePPReactionsMz()
{
  Fxl = 0;
  Fyl = Q/L;
  Mzl = 0;

  Fxr = 0;
  Fyr = -Q/L;
  Mzr = 0;
}

void TConcLdBeam::ComputePFReactionsFx()
{
   ComputePPReactionsFx();
}

void TConcLdBeam::ComputePFReactionsFy()
{
  Fxl = 0;
  Fyl = Q*(pow(b,3) - 3*pow(b,2)*L)/(2*pow(L,3));
  Mzl = 0;

  Fxr = 0;
  Fyr = -(Fyl + Q);
  Mzr = Fyl*L + Q*b;
}

void TConcLdBeam::ComputePFReactionsMz()
{
  Fxl = 0;
  Fyl = 3*Q*(L*L - a*a)/(2*pow(L,3));
  Mzl = 0;

  Fxr = 0;
  Fyr = -3*Q*(L*L - a*a)/(2*pow(L,3));
  Mzr = Fyl*L - Q;
}

void TConcLdBeam::ComputeFPReactionsFx()
{
   ComputePPReactionsFx();
}

void TConcLdBeam::ComputeFPReactionsFy()
{
/*
  Fxr = 0;
  Fyr = Q*(b*b*b - 3*b*b*L)/(2*L*L*L);
  Mzr = 0;

  Fxl = 0;
  Fyl = -(Fyr + Q);
  Mzl = -(Fyr*L + Q*a);
*/
  Fxl = 0;
  Fyl = -Q*b*(3*L*L - b*b)/(2*L*L*L);
  Mzl = -Q*a*b*(b+L)/(2*L*L);

  Fxr = 0;
  Fyr = -Q*a*a*(b+2*L)/(2*L*L*L);
  Mzr = 0;
}

void TConcLdBeam::ComputeFPReactionsMz()
{
  Fxl = 0;
  Fyl = 3*Q*(L*L - b*b)/(2*pow(L,3));
  Mzl = Fyl*L - Q;

  Fxr = 0;
  Fyr = -3*Q*(L*L - b*b)/(2*pow(L,3));
  Mzr = 0;
}

void TConcLdBeam::ComputeFFReactionsFx()
{
   ComputePPReactionsFx();
}

void TConcLdBeam::ComputeFFReactionsFy()
{
   Fxl = 0;
   Fyl = -Q*pow(b,2)*(3*a + b)/pow(L,3);
   Mzl = -Q*a*pow(b,2)/pow(L,2);

   Fxr = 0;
   Fyr = -Q*pow(a,2)*(3*b + a)/pow(L,3);
   Mzr =  Q*pow(a,2)*b/pow(L,2);
}

void TConcLdBeam::ComputeFFReactionsMz()
{
   Fxl = 0;
   Fyl = 6*Q*(a*L - a*a)/pow(L,3);
   Mzl = Q*(4*L*a - 3*a*a - L*L)/pow(L,2);

   Fxr = 0;
   Fyr = -6*Q*(a*L - a*a)/pow(L,3);
   Mzr = Q*(2*L*a - 3*a*a)/pow(L,2);
}

void TConcLdBeam::ComputePPDisplacementsFx()
{
   Dxl = 0;
   Dyl = 0;
   Rzl = 0;

   Dxr = 0;
   Dyr = 0;
   Rzr = 0;
}

void TConcLdBeam::ComputePPDisplacementsFy()
{
   Dxl = 0;
   Dyl = 0;
   Rzl = Q*(b*L - pow(b,3)/L)/(6*E*Iz);

   Dxr = 0;
   Dyr = 0;
   Rzr = -Q*(2*b*L + pow(b,3)/L - 3*pow(b,2))/(6*E*Iz);
}

void TConcLdBeam::ComputePPDisplacementsMz()
{
   Dxl = 0;
   Dyl = 0;
   Rzl = Q*(2*L - 6*a + 3*a*a/L)/(6*E*Iz);

   Dxr = 0;
   Dyr = 0;
   Rzr = -Q*(L - 3*a*a/L)/(6*E*Iz);
}

void TConcLdBeam::ComputePFDisplacementsFx()
{
   ComputePPDisplacementsFx();
}

void TConcLdBeam::ComputePFDisplacementsFy()
{
   Dxl = 0;
   Dyl = 0;
   Rzl = Q*(a*a - pow(a,3)/L)/(4*E*Iz);

   Dxr = 0;
   Dyr = 0;
   Rzr = 0;
}

void TConcLdBeam::ComputePFDisplacementsMz()
{
   Dxl = 0;
   Dyl = 0;
   Rzl = (Q*(L-a) - Fyl*L*L/2)/(E*Iz);

   Dxr = 0;
   Dyr = 0;
   Rzr = 0;
}

void TConcLdBeam::ComputeFPDisplacementsFx()
{
   ComputePPDisplacementsFx();
}

void TConcLdBeam::ComputeFPDisplacementsFy()
{
   Dxl = 0;
   Dyl = 0;
   Rzl = 0;

   Dxr = 0;
   Dyr = 0;
   Rzr = (Q*(L*L - 2*L*a + a*a) + Fyl*L*L - 2*L*Mzl)/(2*E*Iz);
}

void TConcLdBeam::ComputeFPDisplacementsMz()
{
   Dxl = 0;
   Dyl = 0;
   Rzl = 0;

   Dxr = 0;
   Dyr = 0;
   Rzr = (Fyl*L*L - 2*Mzl*L - 2*Q*(L-a))/(2*E*Iz);
}

void TConcLdBeam::ComputeFFDisplacementsFx()
{
   ComputePPDisplacementsFx();
}

void TConcLdBeam::ComputeFFDisplacementsFy()
{
   Dxl = 0;
   Dyl = 0;
   Rzl = 0;

   Dxr = 0;
   Dyr = 0;
   Rzr = 0;
}

void TConcLdBeam::ComputeFFDisplacementsMz()
{
   Dxl = 0;
   Dyl = 0;
   Rzl = 0;

   Dxr = 0;
   Dyr = 0;
   Rzr = 0;
}

void TConcLdBeam::ComputePPForcesFx(double x,double &fx,double &fy,double &mz)
{
   fy = 0;
   mz = 0;
   if (x < a)
      {
      fx = -Fxl;
      }
   else
      {
      fx = -(Fxl + Q);
      }
}

void TConcLdBeam::ComputePPForcesFy(double x,double &fx,double &fy,double &mz)
{
   fx = 0;
   if (x < a)
      {
      fy = Fyl;
      mz = Fyl*x;
      }
   else
      {
      fy = Fyl + Q;
      mz = Fyl*x + Q*(x-a);
      }
}

void TConcLdBeam::ComputePPForcesMz(double x,double &fx,double &fy,double &mz)
{
   fx = 0;
   if (x < a)
      {
      fy = Fyl;
      mz = Fyl*x;
      }
   else
      {
      fy = Fyl;
      mz = Fyl*x - Q;
      }
}

void TConcLdBeam::ComputePFForcesFx(double x,double &fx,double &fy,double &mz)
{
   ComputePPForcesFx(x,fx,fy,mz);
}

void TConcLdBeam::ComputePFForcesFy(double x,double &fx,double &fy,double &mz)
{
   fx = 0;
   if (x < a)
      {
      fy = Fyl;
      mz = Fyl*x;
      }
   else
      {
      fy = Fyl + Q;
      mz = Fyl*x + Q*(x-a);
      }
}

void TConcLdBeam::ComputePFForcesMz(double x,double &fx,double &fy,double &mz)
{
   fx = 0;
   if (x < a)
      {
      fy = Fyl;
      mz = Fyl*x;
      }
   else
      {
      fy = Fyl;
      mz = Fyl*x - Q;
      }
}

void TConcLdBeam::ComputeFPForcesFx(double x,double &fx,double &fy,double &mz)
{
   ComputePPForcesFx(x,fx,fy,mz);
}

void TConcLdBeam::ComputeFPForcesFy(double x,double &fx,double &fy,double &mz)
{
   fx = 0;
   if (x < a)
      {
      fy = Fyl;
      mz = Fyl*x - Mzl;
      }
   else
      {
      fy = Fyl + Q;
      mz = Fyl*x + Q*(x-a) - Mzl;
      }
}

void TConcLdBeam::ComputeFPForcesMz(double x,double &fx,double &fy,double &mz)
{
   fx = 0;
   if (x < a)
      {
      fy = Fyl;
      mz = Fyl*x - Mzl;
      }
   else
      {
      fy = Fyl;
      mz = Fyl*x - Q - Mzl;
      }
}

void TConcLdBeam::ComputeFFForcesFx(double x,double &fx,double &fy,double &mz)
{
   ComputePPForcesFx(x,fx,fy,mz);
}

void TConcLdBeam::ComputeFFForcesFy(double x,double &fx,double &fy,double &mz)
{
   fx = 0;
   if (x < a)
      {
      fy = Fyl;
      mz = Fyl*x - Mzl;
      }
   else
      {
      fy = Fyl + Q;
      mz = Fyl*x - Mzl + Q*(x-a);
      }
}

void TConcLdBeam::ComputeFFForcesMz(double x,double &fx,double &fy,double &mz)
{
   fx = 0;
   if (x < a)
      {
      fy = Fyl;
      mz = Fyl*x - Mzl;
      }
   else
      {
      fy = Fyl;
      mz = Fyl*x - Mzl - Q;
      }
}

void TConcLdBeam::ComputePPDeflectionFx(double x,double &dx,double &dy,double &rz)
{
   dy = 0;
   rz = 0;

   if (x < a)
      {
      dx = Q*b*x/(Ax*E*L);
      }
   else
      {
      dx = Q*a*(L-x)/(Ax*E*L);
      }
}

void TConcLdBeam::ComputePPDeflectionFy(double x,double &dx,double &dy,double &rz)
{
   dx = 0;
   if (x < a)
      {
      dy = Q*b*x*(2*L*(L-x) - b*b - pow((L-x),2))/(6*E*Iz*L);
      rz = -Q*b*(3*x*x + b*b - L*L)/(6*E*Iz*L);
      }
   else
      {
      dy = Q*a*(L-x)*(2*L*b - b*b - pow((L-x),2))/(6*E*Iz*L);
      rz = Q*a*(3*x*x + b*b + 3*L*L - 2*L*b - 6*L*x)/(6*E*Iz*L);
      }
}

void TConcLdBeam::ComputePPDeflectionMz(double x,double &dx,double &dy,double &rz)
{
   dx = 0;
   if (x < a)
      {
      dy = Q*(pow(x,3)/L - (6*a - 3*a*a/L - 2*L)*x)/(6*E*Iz);
      rz = Q*(3*x*x/L - 6*a + 3*a*a/L + 2*L)/(6*E*Iz);
      }
   else
      {
      dy = Q*(pow(x,3)/L + (3*a*a/L + 2*L)*x - 3*a*a - 3*x*x)/(6*E*Iz);
      rz = Q*(3*x*x/L + 2*L + 3*a*a/L -6*x)/(6*E*Iz);
      }
}

void TConcLdBeam::ComputePFDeflectionFx(double x,double &dx,double &dy,double &rz)
{
   ComputePPDeflectionFx(x,dx,dy,rz);
}

void TConcLdBeam::ComputePFDeflectionFy(double x,double &dx,double &dy,double &rz)
{
   dx = 0;
   if (x < a)
      {
      dy = (Fyl*(x*x*x - 3*L*L*x) + Q*x*(6*a*L - 3*a*a - 3*L*L))/(6*E*Iz);
      rz = (Q*(2*a*L - a*a - L*L) - Fyl*(L*L - x*x))/(2*E*Iz);
      }
   else
      {
      dy = (Fyl*(x*x*x - 3*L*L*x + 2*L*L*L) + Q*(pow(x,3) - 3*a*x*x + (6*a*L - 3*L*L)*x + 2*pow(L,3) - 3*a*L*L))/(6*E*Iz);
      rz = (Q*(x*x - 2*a*x - L*L + 2*a*L) - Fyl*(L*L - x*x))/(2*E*Iz);
      }
}

void TConcLdBeam::ComputePFDeflectionMz(double x,double &dx,double &dy,double &rz)
{
   dx = 0;
   if (x < a)
      {
      dy = (Q*x*(L-a) + Fyl*(x*x*x/6 - L*L*x/2))/(E*Iz);
      rz = (Q*(L-a) - Fyl*(L*L/2 - x*x/2))/(E*Iz);
      }
   else
      {
      dy = (Q*(L*x - x*x/2 - L*L/2) + Fyl*(x*x*x/6 - L*L*x/2 + L*L*L/3))/(E*Iz);
      rz = (Q*(L-x) - Fyl*(L*L/2 - x*x/2))/(E*Iz);
      }
}

void TConcLdBeam::ComputeFPDeflectionFx(double x,double &dx,double &dy,double &rz)
{
   ComputePPDeflectionFx(x,dx,dy,rz);
}

void TConcLdBeam::ComputeFPDeflectionFy(double x,double &dx,double &dy,double &rz)
{
   dx = 0;
   if (x < a)
      {
      dy = (Fyl*x*x*x - 3*Mzl*x*x)/(6*E*Iz);
      rz = (Fyl*x*x - 2*Mzl*x)/(2*E*Iz);
      }
   else
      {
      dy = (x-L)*(Q*(L*x + x*x - 3*x*a + L*L + 3*a*a - 3*L*a) - 3*(L+x)*Mzl + (x*x + L*x + L*L)*Fyl)/(6*E*Iz);
      rz = (Q*(x*x - 2*a*x + a*a) + Fyl*x*x - 2*Mzl*x)/(2*E*Iz);
      }
}

void TConcLdBeam::ComputeFPDeflectionMz(double x,double &dx,double &dy,double &rz)
{
   dx = 0;
   if (x < a)
      {
      dy = (Fyl*x*x*x - 3*Mzl*x*x)/(6*E*Iz);
      rz = (Fyl*x*x - 2*Mzl*x)/(2*E*Iz);
      }
   else
      {
      dy = (L-x)*(3*(L+x)*Mzl - 3*Q*(2*a - x - L) - Fyl*(L*x + x*x + L*L))/(6*E*Iz);
      rz = (Fyl*x*x - 2*Mzl*x - 2*Q*(x-a))/(2*E*Iz);
      }
}

void TConcLdBeam::ComputeFFDeflectionFx(double x,double &dx,double &dy,double &rz)
{
   ComputePPDeflectionFx(x,dx,dy,rz);
}

void TConcLdBeam::ComputeFFDeflectionFy(double x,double &dx,double &dy,double &rz)
{
   dx = 0;
   if (x < a)
      {
      dy = Q*b*b*x*x*(3*a*L - 3*a*x - b*x)/(6*E*Iz*pow(L,3));
      rz = Q*b*b*x*(2*a*L - 3*a*x - b*x)/(2*E*Iz*pow(L,3));
      }
   else
      {
      dy = Q*a*a*pow(L-x,2)*(3*b*L - (3*b + a)*(L-x))/(6*E*Iz*pow(L,3));
      rz = Q*a*a*(L-x)*(a*L + b*L - 3*b*x - a*x)/(2*E*Iz*pow(L,3));
      }
}

void TConcLdBeam::ComputeFFDeflectionMz(double x,double &dx,double &dy,double &rz)
{
   dx = 0;
   if (x < a)
      {
      dy = (Fyl*pow(x,3) - 3*Mzl*x*x)/(6*E*Iz);
      rz = (3*Fyl*x*x - 6*Mzl*x)/(6*E*Iz);
      }
   else
      {
      dy = (Fyl*(x*x*x + 2*L*L*L - 3*L*L*x) + (Q+Mzl)*(6*L*x - 3*x*x - 3*L*L))/(6*E*Iz);
      rz = (6*(Q+Mzl)*(L-x) - 3*Fyl*(L*L - x*x))/(6*E*Iz);
      }
}
