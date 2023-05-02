///////////////////////////////////////////////////////////////////////
// Bridge32 - Bridge Modeling Framework
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

#if !defined FEM_H_
#define FEM_H_

#if defined (BUILDBRIDGE) && defined(_BRIDGEDLL)
#define _BRIDGECLASS _export
#elif defined(_BRIDGEDLL)
#define _BRIDGECLASS _import
#else
#define _BRIDGECLASS
#endif

#include "femmodel.h"

class _BRIDGECLASS Bridge;
class _BRIDGECLASS Component;

// FemBuilder
//
// Generates a fem model of a bridge
class FemBuilder
{
public:
   FemBuilder(Bridge &aBridge,FemModel &aModel);
   void Build();

private:
   Bridge &bridge;
   FemModel &model;
   int naj;
   int nam;

   void GenerateJnts();
   void GenerateMbrs();
   void GenerateSpanPnts();
   int NextAvailJnt();
   int NextAvailMbr();
   void GenMajorJnts();
   void GenMinorJnts(Component *component);
   void GenMembers(Component *component);
};

// FemLoader
//
// Applies bridge loads to a fem model
class FemLoader
{
public:
   FemLoader(Bridge &aBridge,FemModel &aModel);
   void GenSelfWgt();
   void GenTrafficBarrier();
   void GenWearSurf();
   void GenUtility();
   void GenMiscDC();
   void GenMiscDW();

private:
   Bridge &bridge;
   FemModel &model;
   int nal;

   int NextAvailLoad();
   void ApplyUnifLd(int loadId,const double w);
//   void ApplyLoad(const Load &ld);
};

#endif // FEM_H_