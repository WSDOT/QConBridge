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

#if !defined ESL_UNITS_H_
#define ESL_UNITS_H_


enum SystemOfUnits {unitsSI = 0,unitsEnglish = 1};
typedef SystemOfUnits SystemOfUnits;


template <int m,int l,int t,int q,int k,int i,int a>
class Physical
{
public:
   Physical(const double v) : val(v) {}
   Physical() : val(0) {}
   double &value() { return val; }
   const double &value() const { return val; }

private:
   typedef Physical<m,l,t,q,k,i,a> Phys; // shorthand

public:
   Phys ConvertTo(const Phys& dest_unit) const { return val/dest_unit.val; }

   friend Phys& operator +  (const Phys& lhs,const Phys& rhs)
                                       { Phys temp(lhs); return temp += rhs; }
   friend Phys& operator -  (const Phys& lhs,const Phys& rhs)
                                       { Phys temp(lhs); return temp -= rhs; }
   friend Phys& operator *  (const Phys& lhs,const double val)
                                       { Phys temp(lhs); return temp *= val; }
   friend Phys& operator /  (const Phys& lhs,const double val)
                                       { Phys temp(lhs); return temp /= val; }
   friend Phys& operator *  (const double val,const Phys& rhs)
                                       { Phys temp(rhs); return temp *= val; }
   Phys& operator += (const Phys& rhs) { val += rhs.val; return *this; }
   Phys& operator -= (const Phys& rhs) { val -= rhs.val; return *this; }
   Phys& setToZero()                   { val = 0.00;     return *this; }
   Phys& operator *= (double s)        { val *= s;       return *this; }
   Phys& operator /= (double s)        { val /= s;       return *this; }

private:
   double val;

   // If this operator were to exist, it would circumvent this
   // class' ability to be type safe.
   operator double() const { return val; }
};

class FundamentalUnits
{
public:
   typedef Physical<1,0,0,0,0,0,0> Mass;
   typedef Physical<0,1,0,0,0,0,0> Length;
   typedef Physical<0,0,1,0,0,0,0> Time;
   typedef Physical<0,0,0,1,0,0,0> Charge;
   typedef Physical<0,0,0,0,0,1,0> Intensity;
   typedef Physical<0,0,0,0,0,0,1> Angle;
};

class DerivedUnits : public FundamentalUnits
{
public:
   typedef Physical<0, 1,-1,0,0,0,0> Velocity;
   typedef Physical<0, 1,-2,0,0,0,0> Acceleration;
   typedef Physical<1, 1,-2,0,0,0,0> Force;
   typedef Physical<1, 2,-2,0,0,0,0> Energy;
   typedef Physical<0, 2, 0,0,0,0,0> Length2;
   typedef Physical<0, 4, 0,0,0,0,0> Length4;
   typedef Physical<1, 3,-2,0,0,0,0> Pressure;
   typedef Physical<1,-2,-2,0,0,0,0> UnitWeight;
   typedef Physical<1, 0,-2,0,0,0,0> LinearForce;
   typedef Physical<1 ,2,-2,0,0,0,0> Moment;
   typedef Physical<1,-3, 0,0,0,0,0> Density;
};

class SIConstants : public DerivedUnits
{
public:
   static const Mass kilogram;
   static const Length meter;
   static const Length kilometer;
   static const Length centimeter;
   static const Length millimeter;
   static const Length2 meter2;
   static const Length2 millimeter2;
   static const Length4 meter4;
   static const Length4 millimeter4;
   static const Force newton;
   static const Time second;
   static const Acceleration meter_per_sec2;
   static const Acceleration g;
   static const Pressure Pascal;
   static const Pressure MPa;
   static const UnitWeight newton_per_meter3;
   static const LinearForce newton_per_meter;
   static const Moment newton_meter;
   static const Density kilogram_per_meter3;
};

class USConstants : public SIConstants
{
public:
   static const Length inch;
   static const Length feet;
   static const Length2   inch2;
   static const Length2   feet2;
   static const Length4 inch4;
   static const Force  pound;
   static const Force  kip;
   static const Pressure pound_per_inch2;
   static const Pressure pound_per_feet2;
   static const Pressure kip_per_inch2;
   static const Pressure kip_per_feet2;
   static const UnitWeight pound_per_feet3;
   static const UnitWeight pound_per_inch3;
   static const UnitWeight kip_per_feet3;
   static const UnitWeight kip_per_inch3;
   static const LinearForce pound_per_feet;
   static const LinearForce pound_per_inch;
   static const LinearForce kip_per_feet;
   static const LinearForce kip_per_inch;
   static const Moment kip_feet;
   static const Moment pound_feet;
};

#endif // ESL_UNITS_H_
