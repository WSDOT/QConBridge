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

#include <math.h>
#include <shape\props.h>
#include <esl\mohr.h>

TPrincipleInertia::TPrincipleInertia(double I11,double I22,double I12,double pDir) :
I11(I11),I22(I22),I12(I12),pDir(pDir)
{
}

void TInertia::TransformInertia(double angle)
{
   MohrCircle2d mc(Ixx,Iyy,Ixy);
   mc.GetState(angle,Ixx,Iyy,Ixy);
}

TPrincipleInertia TInertia::ComputePrincipleValues()
{
   MohrCircle2d mc(Ixx,Iyy,Ixy);
   return TPrincipleInertia(mc.GetSmax(),mc.GetSmin(),mc.GetTmax(),mc.GetPrincDirection());
}

