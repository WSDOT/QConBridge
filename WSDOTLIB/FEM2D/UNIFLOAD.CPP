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
TUnifLdBeam::TUnifLdBeam(double q,double la,double lb,LoadDir dir,double l,double ax,double iz,double e,MbrType type) :
TBeam(l,ax,iz,e,type)
{
   Dir = dir;
   a = la;
   b = lb;
   c = b - a;
   d = L - b/2 - a/2;

   Q = q*c;

   ComputeReactions();
   ComputeDisplacements();
}

void TUnifLdBeam::GetLoad(double &ld)
{
   ld = Q/c;
}

void TUnifLdBeam::GetLocation(double &la,double &lb)
{
   la = a;
   lb = b;
}

LoadDir TUnifLdBeam::GetDirection()
{
   return Dir;
}

void TUnifLdBeam::GetReactions(double &fxl,double &fyl,double &mzl,
                               double &fxr,double &fyr,double &mzr)
{
   fxl = Fxl;
   fyl = Fyl;
   mzl = Mzl;
   fxr = Fxr;
   fyr = Fyr;
   mzr = Mzr;
}

void TUnifLdBeam::GetDisplacements(double &dxl,double &dyl,double &rzl,
                                   double &dxr,double &dyr,double &rzr)
{
   dxl = Dxl;
   dyl = Dyl;
   rzl = Rzl;
   dxr = Dxr;
   dyr = Dyr;
   rzr = Rzr;
}

void TUnifLdBeam::GetForces(double x,double &fx,double &fy,double &mz)
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

void TUnifLdBeam::GetDeflection(double x,double &dx,double &dy,double &rz)
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

void TUnifLdBeam::ComputeReactions()
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

void TUnifLdBeam::ComputeDisplacements()
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

void TUnifLdBeam::ComputePPReactions()
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

void TUnifLdBeam::ComputePFReactions()
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

void TUnifLdBeam::ComputeFPReactions()
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

void TUnifLdBeam::ComputeFFReactions()
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

void TUnifLdBeam::ComputePPDisplacements()
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

void TUnifLdBeam::ComputePFDisplacements()
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

void TUnifLdBeam::ComputeFPDisplacements()
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

void TUnifLdBeam::ComputeFFDisplacements()
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

void TUnifLdBeam::ComputePPForces(double x,double &fx,double &fy,double &mz)
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

void TUnifLdBeam::ComputePFForces(double x,double &fx,double &fy,double &mz)
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

void TUnifLdBeam::ComputeFPForces(double x,double &fx,double &fy,double &mz)
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

void TUnifLdBeam::ComputeFFForces(double x,double &fx,double &fy,double &mz)
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

void TUnifLdBeam::ComputePPDeflection(double x,double &dx,double &dy,double &rz)
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

void TUnifLdBeam::ComputePFDeflection(double x,double &dx,double &dy,double &rz)
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

void TUnifLdBeam::ComputeFPDeflection(double x,double &dx,double &dy,double &rz)
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

void TUnifLdBeam::ComputeFFDeflection(double x,double &dx,double &dy,double &rz)
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

void TUnifLdBeam::ComputePPReactionsFx()
{
  Fxl = -Q*(2*L - a - b)/(2*L);
  Fyl = 0;
  Mzl = 0;

  Fxr = -Q*(a+b)/(2*L);
  Fyr = 0;
  Mzr = 0;
}

void TUnifLdBeam::ComputePPReactionsFy()
{
  Fxl = 0;
  Fyl = -Q*d/L;
  Mzl = 0;

  Fxr = 0;
  Fyr = -Q*(a + c/2)/L;
  Mzr = 0;
}

void TUnifLdBeam::ComputePPReactionsMz()
{
   // Not implemented yet
  Fxl = 0;
  Fyl = 0;
  Mzl = 0;

  Fxr = 0;
  Fyr = 0;
  Mzr = 0;
}

void TUnifLdBeam::ComputePFReactionsFx()
{
   ComputePPReactionsFx();
}

void TUnifLdBeam::ComputePFReactionsFy()
{
  Fxl = 0;
  Fyl = -Q*(8*L*L*L - 6*L*L*(a+b) + a*a*a + b*a*a + a*b*b + b*b*b)/(8*L*L*L);
  Mzl = 0;

  Fxr = 0;
  Fyr = -(Fyl + Q);
  Mzr = Fyl*L + Q*d;
}

void TUnifLdBeam::ComputePFReactionsMz()
{
   // Not implemented yet
  Fxl = 0;
  Fyl = 0;
  Mzl = 0;

  Fxr = 0;
  Fyr = 0;
  Mzr = 0;
}

void TUnifLdBeam::ComputeFPReactionsFx()
{
   ComputePPReactionsFx();
}

void TUnifLdBeam::ComputeFPReactionsFy()
{
  Fxl = 0;
  Fyl = -Q*(a+b)*(6*L*L - a*a - b*b)/(8*L*L*L);
  Mzl = Fyl*L + Q*d;

  Fxr = 0;
  Fyr = -(Q + Fyl);
  Mzr = 0;
}

void TUnifLdBeam::ComputeFPReactionsMz()
{
   // Not implemented yet
  Fxl = 0;
  Fyl = 0;
  Mzl = 0;

  Fxr = 0;
  Fyr = 0;
  Mzr = 0;
}

void TUnifLdBeam::ComputeFFReactionsFx()
{
   ComputePPReactionsFx();
}

void TUnifLdBeam::ComputeFFReactionsFy()
{
   Fxl = 0;
   Fyl = -Q*(12*d*d - 8*d*d*d/L + 2*b*c*c/L - c*c*c/L - c*c)/(4*L*L);
   Mzl = Q*(24*d*d*d/L - 6*b*c*c/L + 3*c*c*c/L + 4*c*c - 24*d*d)/(24*L);

   Fxr = 0;
   Fyr = -(Fyl + Q);
   Mzr = Q*(24*d*d*d/L - 6*b*c*c/L + 3*c*c*c/L + 2*c*c - 48*d*d + 24*d*L)/(24*L);
}

void TUnifLdBeam::ComputeFFReactionsMz()
{
   // Not implemented yet
   Fxl = 0;
   Fyl = 0;
   Mzl = 0;

   Fxr = 0;
   Fyr = 0;
   Mzr = 0;
}

void TUnifLdBeam::ComputePPDisplacementsFx()
{
   Dxl = 0;
   Dyl = 0;
   Rzl = 0;

   Dxr = 0;
   Dyr = 0;
   Rzr = 0;
}

void TUnifLdBeam::ComputePPDisplacementsFy()
{
   Dxl = 0;
   Dyl = 0;
   Rzl = -8*Fyl*L*L
       - Q*(8*pow(d,3)/L - 2*b*c*c/L + pow(c,3)/L + 2*c*c);
   Rzl /= 48*E*Iz;

   Dxr = 0;
   Dyr = 0;
   Rzr = 16*Fyl*L*L
       + Q*(24*d*d - 8*pow(d,3)/L + 2*b*c*c/L - pow(c,3)/L);
   Rzr /= 48*E*Iz;
}

void TUnifLdBeam::ComputePPDisplacementsMz()
{
   // Not implemented yet
   Dxl = 0;
   Dyl = 0;
   Rzl = 0;

   Dxr = 0;
   Dyr = 0;
   Rzr = 0;
}

void TUnifLdBeam::ComputePFDisplacementsFx()
{
   ComputePPDisplacementsFx();
}

void TUnifLdBeam::ComputePFDisplacementsFy()
{
   Dxl = 0;
   Dyl = 0;
   Rzl = -(3*Fyl*L*L + Q*(a*a + b*b - 3*L*a - 3*L*b + b*a + 3*L*L))/(6*E*Iz);

   Dxr = 0;
   Dyr = 0;
   Rzr = 0;
}

void TUnifLdBeam::ComputePFDisplacementsMz()
{
   // Not implemented yet
   Dxl = 0;
   Dyl = 0;
   Rzl = 0;

   Dxr = 0;
   Dyr = 0;
   Rzr = 0;
}

void TUnifLdBeam::ComputeFPDisplacementsFx()
{
   ComputePPDisplacementsFx();
}

void TUnifLdBeam::ComputeFPDisplacementsFy()
{
   Dxl = 0;
   Dyl = 0;
   Rzl = 0;

   Dxr = 0;
   Dyr = 0;
   Rzr = 3*Fyl*L*L - 6*Mzl*L + Q*(a*a - L*a + L*L);
   Rzr /= 6*E*Iz;
}

void TUnifLdBeam::ComputeFPDisplacementsMz()
{
   // Not implemented yet
   Dxl = 0;
   Dyl = 0;
   Rzl = 0;

   Dxr = 0;
   Dyr = 0;
   Rzr = 0;
}

void TUnifLdBeam::ComputeFFDisplacementsFx()
{
   ComputePPDisplacementsFx();
}

void TUnifLdBeam::ComputeFFDisplacementsFy()
{
   Dxl = 0;
   Dyl = 0;
   Rzl = 0;

   Dxr = 0;
   Dyr = 0;
   Rzr = 0;
}

void TUnifLdBeam::ComputeFFDisplacementsMz()
{
   // Not implemented yet
   Dxl = 0;
   Dyl = 0;
   Rzl = 0;

   Dxr = 0;
   Dyr = 0;
   Rzr = 0;
}

void TUnifLdBeam::ComputePPForcesFx(double x,double &fx,double &fy,double &mz)
{
   fy = 0;
   mz = 0;

   if (x < a)
      {
      fx = -Fxl;
      }
   else if (x < b)
      {
      fx = -Fxl - Q*(x-a)/(b-a);
      }
   else
      {
      fx = -Fxl - Q;
      }
}

void TUnifLdBeam::ComputePPForcesFy(double x,double &fx,double &fy,double &mz)
{
   fx = 0;
   if (x < a)
      {
      fy = Fyl;
      mz = Fyl*x;
      }
   else if (x < b)
      {
      fy = Fyl + Q*(x-a)/c;
      mz = Fyl*x + Q*(x-a)*(x-a)/(2*c);
      }
   else
      {
      fy = Fyl + Q;
      mz = Fyl*x + Q*(x - a/2 - b/2);
      }
}

void TUnifLdBeam::ComputePPForcesMz(double x,double &fx,double &fy,double &mz)
{
   // Not implemented yet
   fx = 0;
   fy = 0;
   mz = 0;
}

void TUnifLdBeam::ComputePFForcesFx(double x,double &fx,double &fy,double &mz)
{
   ComputePPForcesFx(x,fx,fy,mz);
}

void TUnifLdBeam::ComputePFForcesFy(double x,double &fx,double &fy,double &mz)
{
   fx = 0;

   if (x < a)
      {
      fy = Fyl;
      mz = Fyl*x;
      }
   else if (x < b)
      {
      fy = Fyl + Q*(x-a)/c;
      mz = Fyl*x + Q*pow(x-a,2)/(2*c);
      }
   else
      {
      fy = Fyl + Q;
      mz = Fyl*x + Q*(x - a/2 - b/2);
      }
}

void TUnifLdBeam::ComputePFForcesMz(double x,double &fx,double &fy,double &mz)
{
   // Not implemented yet
   fx = 0;
   fy = 0;
   mz = 0;
}

void TUnifLdBeam::ComputeFPForcesFx(double x,double &fx,double &fy,double &mz)
{
   ComputePPForcesFx(x,fx,fy,mz);
}

void TUnifLdBeam::ComputeFPForcesFy(double x,double &fx,double &fy,double &mz)
{
   fx = 0;
   if (x < a)
      {
      fy = Fyl;
      mz = Fyl*x - Mzl;
      }
   else if (x < b)
      {
      fy = Fyl + Q*(x-a)/(b-a);
      mz = Fyl*x - Mzl + Q*pow(x-a,2)/(2*(b-a));
      }
   else
      {
      fy = Fyl + Q;
      mz = Fyl*x - Mzl + Q*(x - a/2 - b/2);
      }
}

void TUnifLdBeam::ComputeFPForcesMz(double x,double &fx,double &fy,double &mz)
{
   // Not implemented yet
   fx = 0;
   fy = 0;
   mz = 0;
}

void TUnifLdBeam::ComputeFFForcesFx(double x,double &fx,double &fy,double &mz)
{
   ComputePPForcesFx(x,fx,fy,mz);
}

void TUnifLdBeam::ComputeFFForcesFy(double x,double &fx,double &fy,double &mz)
{
   fx = 0;

   if (x < a)
      {
      fy = Fyl;
      mz = Fyl*x - Mzl;
      }
   else if (x < b)
      {
      fy = Fyl + Q*(x-a)/c;
      mz = Fyl*x - Mzl + Q*pow(x-a,2)/(2*c);
      }
   else
      {
      fy = Fyl + Q;
      mz = Fyl*x - Mzl + Q*(x - L + d);
      }
}

void TUnifLdBeam::ComputeFFForcesMz(double x,double &fx,double &fy,double &mz)
{
   // Not implemented yet
   fx = 0;
   fy = 0;
   mz = 0;
}

void TUnifLdBeam::ComputePPDeflectionFx(double x,double &dx,double &dy,double &rz)
{
   dy = 0;
   rz = 0;

   if (x < a)
      {
      dx = -Fxl*x/(Ax*E);
      }
   else if (x < b)
      {
      dx = -Fxl*x - Q*(x*x - 2*x*a + a*a)/(2*(b-a));
      dx /= Ax*E;
      }
   else
      {
      dx = Fxl*(L-x) + Q*(L-x);
      dx /= Ax*E;
      }
}

void TUnifLdBeam::ComputePPDeflectionFy(double x,double &dx,double &dy,double &rz)
{
// rotations not done
   dx = 0;
   if (x < a)
      {
      dy = 8*Fyl*(pow(x,3) - L*L*x)
         - Q*x*(8*pow(d,3)/L - 2*b*c*c/L + pow(c,3)/L + 2*c*c);
      dy /= 48*E*Iz;
      rz = 8*Fyl*(3*x*x-L*L)
         - Q*(8*d*d*d/L - 2*b*c*c/L + c*c*c/L + 2*c*c);
      rz /= 48*E*Iz;
      }
   else if (x < b)
      {
      dy = 8*Fyl*(pow(x,3) - L*L*x)
         - Q*x*(8*pow(d,3)/L - 2*b*c*c/L + pow(c,3)/L + 2*c*c)
         + 2*Q*pow(x-a,4)/c;
      dy /= 48*E*Iz;
      rz = 8*Fyl*(3*x*x-L*L)
         - Q*(8*d*d*d/L - 2*b*c*c/L + c*c*c/L + 2*c*c)
         + 8*Q*pow(x-a,3)/c;
      rz /= 48*E*Iz;
      }
   else
      {
      dy = 8*Fyl*(pow(x,3) - L*L*x)
         - Q*x*(8*pow(d,3)/L - 2*b*c*c/L + pow(c,3)/L)
         + 8*Q*pow(x - a/2 - b/2,3)
         - Q*(2*b*c*c - c*c*c);
      dy /= 48*E*Iz;
      rz = 8*Fyl*(3*x*x-L*L)
         - Q*(8*d*d*d/L - 2*b*c*c/L + c*c*c/L)
         + 24*Q*pow(x - a/2 - b/2,2);
      rz /= 48*E*Iz;
      }
}

void TUnifLdBeam::ComputePPDeflectionMz(double x,double &dx,double &dy,double &rz)
{
   // Not implemented yet
   dx = 0;
   dy = 0;
   rz = 0;
}

void TUnifLdBeam::ComputePFDeflectionFx(double x,double &dx,double &dy,double &rz)
{
   ComputePPDeflectionFx(x,dx,dy,rz);
}

void TUnifLdBeam::ComputePFDeflectionFy(double x,double &dx,double &dy,double &rz)
{
   dx = 0;
   if (x < a)
      {
      dy = Fyl*(x*x*x/6 - L*L*x/2 + L*L*L/3)
         + Q*((-a*a/6 + L*a/2 - b*a/6 - L*L/2 + b*L/2 - b*b/6)*x
              + b*a*a/24 - L*L*a/4 + a*b*b/24 + a*a*a/24
              + L*L*L/3 - L*L*b/4 + b*b*b/24);
      dy /= E*Iz;
      rz = 3*Fyl*(L*L - x*x)
         + Q*(a*a + b*b + b*a - 3*L*a - 3*L*b + 3*L*L);
      rz /= -6*E*Iz;
      }
   else if (x < b)
      {
      dy = Fyl*(x*x*x/6 - L*L*x/2 + L*L*L/3)
         + Q*(pow(x-a,4)/(24*(b-a)) - x*a*a/6 + L*a*x/2 - b*a*x/6 - L*L*x/2
              + b*L*x/2 - x*b*b/6 + a*a*a/24 + b*a*a/24 - L*L*a/4 + a*b*b/24
              + L*L*L/3 - L*L*b/4 + b*b*b/24);
      dy /= E*Iz;
      rz = 3*Fyl*(L*L - x*x)
         + Q*(a*a + b*a +b*b - 3*L*a + 3*L*L - 3*b*L - pow(x-a,3)/(b-a));
      rz /= -6*E*Iz;
      }
   else
      {
      dy = Fyl*(2*x + 4*L)*pow(L-x,2)/12
         + Q*(4*L + 2*x - 3*a - 3*b)*pow(L-x,2)/12;
      dy /= E*Iz;
      rz = 3*Fyl*(L+x)*(L-x)
         + 3*Q*(L-x)*(L - a - b + x);
      rz /= -6*E*Iz;
      }
}

void TUnifLdBeam::ComputePFDeflectionMz(double x,double &dx,double &dy,double &rz)
{
   // Not implemented yet
   dx = 0;
   dy = 0;
   rz = 0;
}

void TUnifLdBeam::ComputeFPDeflectionFx(double x,double &dx,double &dy,double &rz)
{
   ComputePPDeflectionFx(x,dx,dy,rz);
}

void TUnifLdBeam::ComputeFPDeflectionFy(double x,double &dx,double &dy,double &rz)
{
   dx = 0;
   if (x < a)
      {
      dy = Fyl*x*x*x - 3*Mzl*x*x;
      dy /= 6*E*Iz;
      rz = 3*Fyl*x*x - 6*Mzl*x;
      rz /= 6*E*Iz;
      }
   else if (x < b)
      {
      dy = 4*Fyl*x*x*x - 12*Mzl*x*x + Q*pow(x-a,4)/(b-a);
      dy /= 24*E*Iz;
      rz = 3*Fyl*x*x - 6*Mzl*x + Q*pow(x-a,3)/(b-a);
      rz /= 6*E*Iz;
      }
   else
      {
      dy = 4*Fyl*x*x*x - 12*Mzl*x*x - Q*(a - 2*x + b)*(a*a - 2*x*a - 2*b*x + 2*x*x + b*b);
      dy /= 24*E*Iz;
      rz = 3*Fyl*x*x - 6*Mzl*x + Q*(a*a + b*a + b*b + 3*x*x - 3*a*x - 3*b*x);
      rz /= 6*E*Iz;
      }
}

void TUnifLdBeam::ComputeFPDeflectionMz(double x,double &dx,double &dy,double &rz)
{
   // Not implemented yet
   dx = 0;
   dy = 0;
   rz = 0;
}

void TUnifLdBeam::ComputeFFDeflectionFx(double x,double &dx,double &dy,double &rz)
{
   ComputePPDeflectionFx(x,dx,dy,rz);
}

void TUnifLdBeam::ComputeFFDeflectionFy(double x,double &dx,double &dy,double &rz)
{
   dx = 0;

   if (x < a)
      {
      dy = Fyl*x*x*x - 3*Mzl*x*x;
      dy /= 6*E*Iz;
      rz = 3*Fyl*x*x - 6*Mzl*x;
      rz /= 6*E*Iz;
      }
   else if (x < b)
      {
      dy = Fyl*x*x*x - 3*Mzl*x*x + Q*pow(x-a,4)/(4*c);
      dy /= 6*E*Iz;
      rz = 3*Fyl*x*x - 6*Mzl*x + Q*pow(x-a,3)/c;
      rz /= 6*E*Iz;
      }
   else
      {
      dy = Fyr*pow(L-x,3) + 3*Mzr*pow(L-x,2);
      dy /= 6*E*Iz;
      rz = -3*Fyr*pow(L-x,2) - 6*Mzr*(L-x);
      rz /= 6*E*Iz;
      }
}

void TUnifLdBeam::ComputeFFDeflectionMz(double x,double &dx,double &dy,double &rz)
{
   // Not implemented yet
   dx = 0;
   dy = 0;
   rz = 0;
}