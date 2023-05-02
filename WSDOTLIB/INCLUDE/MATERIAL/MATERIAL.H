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

#if !defined MATERIAL_MATERIAL_H_
#define MATERIAL_MATERIAL_H_

#if defined (BUILDMATLIB) && !defined(_MATLIB)
#define _MATCLASS __declspec(dllexport)
#elif defined(_MATLIB)
#define _MATCLASS
#else
#define _MATCLASS __declspec(dllimport)
#endif

class _MATCLASS Material
{
// Data Members
protected:
   double modE;
   double unitWgt;
   double cte;

// Constructors/Destructor
public:
   Material(double modE = 1.,double unitWgt = 1.,double cte = 1.);
   virtual ~Material();

// Member Functions
public:
   double GetModE()    const { return modE;    }
   double GetUnitWgt() const { return unitWgt; }
   double GetCoeffThermalExpansion() const {return cte;}
   virtual double ComputeStress(double strain) = 0;
   virtual double ComputeStrain(double stress) = 0;
   virtual Material* Clone() const = 0;

   bool operator == (const Material& m) const {return IsEqual(m);}

protected:
   virtual bool IsEqual(const Material& m) const = 0;

private:
   // Prevent accidental copying. Use Clone() to make copies
   Material(const Material& m);
   Material *operator = (const Material* m);
   Material &operator = (const Material& m);
};

#endif // MATERIAL_MATERIAL_H_
