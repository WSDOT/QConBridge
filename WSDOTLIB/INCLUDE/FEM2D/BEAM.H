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

#if !defined FEM2D_BEAM_H_
#define FEM2D_BEAM_H_

#include <fem2d\femall.h>
#include <math.h>

class TBeam
{
// Public Implementation
public:
	TBeam(double l,double ax,double iz,double e,MbrType type = mtFixFix);
   double GetLength();
   MbrType GetType();
   virtual void GetLoad(double &ld) = 0;
   virtual LoadDir GetDirection() = 0;
	virtual void GetReactions(double &fxl,double &fyl,double &mzl,
                             double &fxr,double &fyr,double &mzr) = 0;
	virtual void GetDisplacements(double &dxl,double &dyl,double &rzl,
											double &dxr,double &dyr,double &rzr) = 0;
	virtual void GetForces(double x,double &fx,double &fy,double &mz) = 0;
	virtual void GetDeflection(double x,double &dx,double &dy,double &rz) = 0;

// Protected Implementation
protected:
	double L; // Length of the beam
	double Ax; // Cross sectional area
	double Iz; // Moment of inertia
   double E; // Mod. Elasticity
	MbrType Type; // Member type - boundary conditions
};

class TConcLdBeam : public TBeam
{
// Public Implementation
public:
	TConcLdBeam(double q,double la,LoadDir dir,double l,double ax,double iz,double e,MbrType type = mtFixFix);
   void GetLoad(double &ld);
   void GetLocation(double &loc);
   LoadDir GetDirection();
	void GetReactions(double &fxl,double &fyl,double &mzl,double &fxr,double &fyr,double &mzr);
	void GetDisplacements(double &dxl,double &dyl,double &rzl,
                         double &dxr,double &dyr,double &rzr);
	void GetForces(double x,double &fx,double &fy,double &mz);
	void GetDeflection(double x,double &dx,double &dy,double &rz);

// Private Implementation
private:
   double Q; // Generalized load
   double a,b;
   LoadDir Dir;
   double Fxl, Fxr;
   double Fyl, Fyr;
   double Mzl, Mzr;
   double Dxl, Dxr;
   double Dyl, Dyr;
   double Rzl, Rzr;

   void ComputeReactions();
   void ComputeDisplacements();

   void ComputePPReactions();
   void ComputePFReactions();
   void ComputeFPReactions();
   void ComputeFFReactions();

   void ComputePPDisplacements();
   void ComputePFDisplacements();
   void ComputeFPDisplacements();
   void ComputeFFDisplacements();

   void ComputePPForces(double x,double &fx,double &fy,double &mz);
   void ComputePFForces(double x,double &fx,double &fy,double &mz);
   void ComputeFPForces(double x,double &fx,double &fy,double &mz);
   void ComputeFFForces(double x,double &fx,double &fy,double &mz);

	void ComputePPDeflection(double x,double &dx,double &dy,double &rz);
	void ComputePFDeflection(double x,double &dx,double &dy,double &rz);
	void ComputeFPDeflection(double x,double &dx,double &dy,double &rz);
	void ComputeFFDeflection(double x,double &dx,double &dy,double &rz);

   void ComputePPReactionsFx();
   void ComputePPReactionsFy();
   void ComputePPReactionsMz();

   void ComputePFReactionsFx();
   void ComputePFReactionsFy();
   void ComputePFReactionsMz();

   void ComputeFPReactionsFx();
   void ComputeFPReactionsFy();
   void ComputeFPReactionsMz();

   void ComputeFFReactionsFx();
   void ComputeFFReactionsFy();
   void ComputeFFReactionsMz();

   void ComputePPDisplacementsFx();
   void ComputePPDisplacementsFy();
   void ComputePPDisplacementsMz();

   void ComputePFDisplacementsFx();
   void ComputePFDisplacementsFy();
   void ComputePFDisplacementsMz();

   void ComputeFPDisplacementsFx();
   void ComputeFPDisplacementsFy();
   void ComputeFPDisplacementsMz();

   void ComputeFFDisplacementsFx();
   void ComputeFFDisplacementsFy();
   void ComputeFFDisplacementsMz();

   void ComputePPForcesFx(double x,double &fx,double &fy,double &mz);
   void ComputePPForcesFy(double x,double &fx,double &fy,double &mz);
   void ComputePPForcesMz(double x,double &fx,double &fy,double &mz);

   void ComputePFForcesFx(double x,double &fx,double &fy,double &mz);
   void ComputePFForcesFy(double x,double &fx,double &fy,double &mz);
   void ComputePFForcesMz(double x,double &fx,double &fy,double &mz);

   void ComputeFPForcesFx(double x,double &fx,double &fy,double &mz);
   void ComputeFPForcesFy(double x,double &fx,double &fy,double &mz);
   void ComputeFPForcesMz(double x,double &fx,double &fy,double &mz);

   void ComputeFFForcesFx(double x,double &fx,double &fy,double &mz);
   void ComputeFFForcesFy(double x,double &fx,double &fy,double &mz);
   void ComputeFFForcesMz(double x,double &fx,double &fy,double &mz);

   void ComputePPDeflectionFx(double x,double &fx,double &fy,double &mz);
   void ComputePPDeflectionFy(double x,double &fx,double &fy,double &mz);
   void ComputePPDeflectionMz(double x,double &fx,double &fy,double &mz);

   void ComputePFDeflectionFx(double x,double &fx,double &fy,double &mz);
   void ComputePFDeflectionFy(double x,double &fx,double &fy,double &mz);
   void ComputePFDeflectionMz(double x,double &fx,double &fy,double &mz);

   void ComputeFPDeflectionFx(double x,double &fx,double &fy,double &mz);
   void ComputeFPDeflectionFy(double x,double &fx,double &fy,double &mz);
   void ComputeFPDeflectionMz(double x,double &fx,double &fy,double &mz);

   void ComputeFFDeflectionFx(double x,double &fx,double &fy,double &mz);
   void ComputeFFDeflectionFy(double x,double &fx,double &fy,double &mz);
   void ComputeFFDeflectionMz(double x,double &fx,double &fy,double &mz);
};

class TUnifLdBeam : public TBeam
{
// Public Implementation
public:
	TUnifLdBeam(double q,double la,double lb,LoadDir dir,double l,double ax,double iz,double e,MbrType type = mtFixFix);
   void GetLoad(double &ld);
   void GetLocation(double &la,double &lb);
   LoadDir GetDirection();
	void GetReactions(double &fxl,double &fyl,double &mzl,double &fxr,double &fyr,double &mzr);
	void GetDisplacements(double &dxl,double &dyl,double &rzl,
                         double &dxr,double &dyr,double &rzr);
	void GetForces(double x,double &fx,double &fy,double &mz);
	void GetDeflection(double x,double &dx,double &dy,double &rz);

// Private Implementation
private:
   double Q; // Generalized load
   double a,b,c,d;
   LoadDir Dir;
   double Fxl, Fxr;
   double Fyl, Fyr;
   double Mzl, Mzr;
   double Dxl, Dxr;
   double Dyl, Dyr;
   double Rzl, Rzr;

   void ComputeReactions();
   void ComputeDisplacements();

   void ComputePPReactions();
   void ComputePFReactions();
   void ComputeFPReactions();
   void ComputeFFReactions();

   void ComputePPDisplacements();
   void ComputePFDisplacements();
   void ComputeFPDisplacements();
   void ComputeFFDisplacements();

   void ComputePPForces(double x,double &fx,double &fy,double &mz);
   void ComputePFForces(double x,double &fx,double &fy,double &mz);
   void ComputeFPForces(double x,double &fx,double &fy,double &mz);
   void ComputeFFForces(double x,double &fx,double &fy,double &mz);

	void ComputePPDeflection(double x,double &dx,double &dy,double &rz);
	void ComputePFDeflection(double x,double &dx,double &dy,double &rz);
	void ComputeFPDeflection(double x,double &dx,double &dy,double &rz);
	void ComputeFFDeflection(double x,double &dx,double &dy,double &rz);

   void ComputePPReactionsFx();
   void ComputePPReactionsFy();
   void ComputePPReactionsMz();

   void ComputePFReactionsFx();
   void ComputePFReactionsFy();
   void ComputePFReactionsMz();

   void ComputeFPReactionsFx();
   void ComputeFPReactionsFy();
   void ComputeFPReactionsMz();

   void ComputeFFReactionsFx();
   void ComputeFFReactionsFy();
   void ComputeFFReactionsMz();

   void ComputePPDisplacementsFx();
   void ComputePPDisplacementsFy();
   void ComputePPDisplacementsMz();

   void ComputePFDisplacementsFx();
   void ComputePFDisplacementsFy();
   void ComputePFDisplacementsMz();

   void ComputeFPDisplacementsFx();
   void ComputeFPDisplacementsFy();
   void ComputeFPDisplacementsMz();

   void ComputeFFDisplacementsFx();
   void ComputeFFDisplacementsFy();
   void ComputeFFDisplacementsMz();

   void ComputePPForcesFx(double x,double &fx,double &fy,double &mz);
   void ComputePPForcesFy(double x,double &fx,double &fy,double &mz);
   void ComputePPForcesMz(double x,double &fx,double &fy,double &mz);

   void ComputePFForcesFx(double x,double &fx,double &fy,double &mz);
   void ComputePFForcesFy(double x,double &fx,double &fy,double &mz);
   void ComputePFForcesMz(double x,double &fx,double &fy,double &mz);

   void ComputeFPForcesFx(double x,double &fx,double &fy,double &mz);
   void ComputeFPForcesFy(double x,double &fx,double &fy,double &mz);
   void ComputeFPForcesMz(double x,double &fx,double &fy,double &mz);

   void ComputeFFForcesFx(double x,double &fx,double &fy,double &mz);
   void ComputeFFForcesFy(double x,double &fx,double &fy,double &mz);
   void ComputeFFForcesMz(double x,double &fx,double &fy,double &mz);

   void ComputePPDeflectionFx(double x,double &fx,double &fy,double &mz);
   void ComputePPDeflectionFy(double x,double &fx,double &fy,double &mz);
   void ComputePPDeflectionMz(double x,double &fx,double &fy,double &mz);

   void ComputePFDeflectionFx(double x,double &fx,double &fy,double &mz);
   void ComputePFDeflectionFy(double x,double &fx,double &fy,double &mz);
   void ComputePFDeflectionMz(double x,double &fx,double &fy,double &mz);

   void ComputeFPDeflectionFx(double x,double &fx,double &fy,double &mz);
   void ComputeFPDeflectionFy(double x,double &fx,double &fy,double &mz);
   void ComputeFPDeflectionMz(double x,double &fx,double &fy,double &mz);

   void ComputeFFDeflectionFx(double x,double &fx,double &fy,double &mz);
   void ComputeFFDeflectionFy(double x,double &fx,double &fy,double &mz);
   void ComputeFFDeflectionMz(double x,double &fx,double &fy,double &mz);
};

#endif // FEM2D_BEAM_H_
