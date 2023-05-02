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

#if !defined ESL_MOHR_H_
#define ESL_MOHR_H_

#include <esl\esldll.h>

class ESLCLASS MohrCircle2d
{
private:
   double sMin;
   double sMax;
   double tMax;
   double theta;
   double radius;
   double center;


public:
   MohrCircle2d(double sii,double sjj,double sij);
   MohrCircle2d(const MohrCircle2d &mc);

   double GetSmax() const { return sMax; }
   double GetSmin() const { return sMin; }
   double GetTmax() const { return tMax; }
   double GetPrincDirection() const { return theta; }
   void   GetState(double angle,double &sii,double &sjj,double &sij) const;

   MohrCircle2d &operator = (const MohrCircle2d &mc) {return *this;}

private:
   void Init(double sii,double sjj,double sij);
};

class ESLCLASS MohrCircle3d
{
private:
   double s0; // Max principle value
   double s1; // Middle principle value
   double s2; // Min priciple value
   double v[3][3]; // Direction of principle axes
   double t0; // Principle stress/strain 0
   double t1; // Principle stress/strain 1
   double t2; // Principle stress/strain 2
   double sn0; // Normal stress corrosponding to principle stress/strain 0
   double sn1; // Normal stress corrosponding to principle stress/strain 1
   double sn2; // Normal stress corrosponding to principle stress/strain 2

public:
   MohrCircle3d(double sii,double sjj,double skk,double sij,double sjk,double sik);
   double GetS0()  const { return s0; }
   double GetS1()  const { return s1; }
   double GetS2()  const { return s2; }
   void   GetPrincDirection(int n,double &i,double &j,double &k) const { i = v[n][0]; j = v[n][1]; k = v[n][2]; }
   double GetT0()  const { return t0;  }
   double GetT1()  const { return t1;  }
   double GetT2()  const { return t2;  }
   double GetSn0() const { return sn0; }
   double GetSn1() const { return sn1; }
   double GetSn2() const { return sn2; }
   double GetSoct() const;
   double GetToct() const;

private:
   // Hide these to prevent unwanted copying!!!
   MohrCircle3d(const MohrCircle3d &mc) {}
   MohrCircle3d &operator = (const MohrCircle3d &mc) {return *this;}
};

#endif // ESL_MOHR_H_
