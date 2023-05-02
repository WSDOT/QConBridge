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

#if !defined (INFLLINE_H_)
#define INFLLINE_H_

#include <bridge\brdgdll.h>
#include <bridge\diagram.h>

class _BRIDGECLASS InfluenceLine : public Diagram
{
public:
   InfluenceLine(int id,const char *desc = 0);
   InfluenceLine(int id,InfluenceLine &il,const char *desc = 0);
   InfluenceLine(int id,AnalysisResults &ar);

   enum TMode {Normal,ExcludeNegValues,ExcludePosValues};

   double GetPosArea() const {return posArea;}
   double GetNegArea() const {return negArea;}
   double GetNetArea() const {return netArea;}
   double GetArea() const;
   void   SetMode(TMode newMode) {mode = newMode;}
   double Evaluate(double x,MbrFace face = mbrLeftFace);
   void   AddPoint(Diagram::Point *point);
   void   EnableUpdate(bool enable,bool recalc = true);

#if defined GEN_DEBUG_CODE
   void Write(ostream &os);
#endif

protected:
   TMode  mode;
   bool   updateAreas;
   double posArea;
   double negArea;
   double netArea;

   void UpdateAreas();
};

typedef TIArrayAsVector<InfluenceLine>         InfluenceLineContainer;
typedef TIArrayAsVectorIterator<InfluenceLine> InfluenceLineIterator;

#endif // INFLLINE_H_