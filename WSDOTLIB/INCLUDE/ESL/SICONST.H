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

#if !defined ESL_SICONST_H_
#define ESL_SICONST_H_

/*
Base Units
Mass           kilogram
Length         meter
Time           second
Charge         ampere
Intensity      candela
Angle          radians
*/

const SIConstants::Mass         SIConstants::kilogram = 1.0;
const SIConstants::Force        SIConstants::newton = 1.0;
const SIConstants::Time         SIConstants::second = 1.0;
const SIConstants::Acceleration SIConstants::meter_per_sec2 = 1.0;
const SIConstants::Acceleration SIConstants::g = 9.80665;
const SIConstants::Density      SIConstants::kilogram_per_meter3 = 1.0;

const SIConstants::Length SIConstants::kilometer  = 1000.;
const SIConstants::Length SIConstants::meter      = 1.0;
const SIConstants::Length SIConstants::centimeter = 0.01;
const SIConstants::Length SIConstants::millimeter = 0.001;

const SIConstants::Length2     SIConstants::meter2 = 1.00;
const SIConstants::Length2     SIConstants::millimeter2 = 1.0e-6;
const SIConstants::Pressure SIConstants::Pascal = 1.00;
const SIConstants::Pressure SIConstants::MPa    = 1.0e6;

const SIConstants::UnitWeight  SIConstants::newton_per_meter3 = 1.0;
const SIConstants::Length4 SIConstants::meter4 = 1.0;
const SIConstants::Length4 SIConstants::millimeter4 = 1.0e-12;

const SIConstants::LinearForce SIConstants::newton_per_meter = 1.0;

const SIConstants::Moment SIConstants::newton_meter = 1.0;

const USConstants::Length USConstants::inch = 0.0254;
const USConstants::Length USConstants::feet = 0.3048;
const USConstants::Force  USConstants::pound = 4.44822161526;
const USConstants::Force  USConstants::kip = 4448.22161526;

const USConstants::Length2   USConstants::inch2 = 0.00064516;
const USConstants::Length2   USConstants::feet2 = 0.09290304;
const USConstants::Pressure USConstants::kip_per_inch2 = 6894757.29317;
const USConstants::Pressure USConstants::pound_per_inch2 = 6894.75729317;
const USConstants::Pressure USConstants::kip_per_feet2 = 47880.2589803;
const USConstants::Pressure USConstants::pound_per_feet2 = 47.8802589803;
const USConstants::UnitWeight USConstants::pound_per_feet3 = 157.087463846;
const USConstants::UnitWeight USConstants::kip_per_feet3 = 157087.463846;
const USConstants::UnitWeight USConstants::kip_per_inch3 = 271447137.526;
const USConstants::UnitWeight USConstants::pound_per_inch3 = 271447.137526;
const USConstants::Length4 USConstants::inch4 = 4.162314256e-7;

const USConstants::LinearForce USConstants::pound_per_feet = 14.5939029372;
const USConstants::LinearForce USConstants::pound_per_inch = 175.126835246;
const USConstants::LinearForce USConstants::kip_per_feet = 14593.9029372;
const USConstants::LinearForce USConstants::kip_per_inch = 175126.835246;

const USConstants::Moment USConstants::kip_feet = 1355.81794833;
const USConstants::Moment USConstants::pound_feet = 1.35581794833;

#endif // ESL_SICONST_H_
