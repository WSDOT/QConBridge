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

#if !defined FEM2D_FEMALL_H_
#define FEM2D_FEMALL_H_

// This include file has stuff that is needed by all fem files

#undef  EPS
#define EPS  1.0e-5
#undef   zero_val
#define  zero_val( x )     (((-EPS)<=(x)) && ((x)<=(EPS)))

enum MbrType    {mtPinPin,mtPinFix,mtFixPin,mtFixFix};
typedef MbrType MbrType;

enum MbrFace    {mbrLeftFace,mbrRightFace};
typedef MbrFace MbrFace;

enum MbrEnd     {mbrStart,mbrEnd};
typedef MbrEnd MbrEnd;

enum MbrRelease {mbrRelNone,mbrRelMz};
typedef MbrRelease MbrRelease;

enum LoadDir {ForceX,ForceY,MomentZ};
typedef LoadDir LoadDir;

enum LoadOrientation {Local,Global,GlobalProjected};
typedef LoadOrientation LoadOrientation;

enum GravityDir {PosX,NegX,PosY,NegY};
typedef GravityDir GravityDir;

enum DeadLoadingType {dltJoint,dltMember};
typedef DeadLoadingType DeadLoadingType;

// Joint Releases
// These may be ored together to form a compond release
enum JntRelease {jntRelNone = 0,jntRelFx = 0x0001,jntRelFy = 0x0002,jntRelMz = 0x0004,jntRelAll = jntRelFx|jntRelFy|jntRelMz};
typedef JntRelease JntRelease;

#endif // FEM2D_FEMALL_H_
