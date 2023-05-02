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

#if !defined FEM2D_ENVELOPE_H_
#define FEM2D_ENVELOPE_H_

#include <fem2d\femdll.h>
#include <fem2d\femall.h>
#include <fem2d\result.h>

class _FEMCLASS FemModel;

class _FEMCLASS Envelope
{
public:
   Envelope(int id);
   ~Envelope();

   void AddLoading(int id);
   int GetID() const;

   int GetMinJntForce(int jnt,int dof);
   int GetMaxJntForce(int jnt,int dof);

   int GetMinJntDisp(int jnt,int dof);
   int GetMaxJntDisp(int jnt,int dof);

   int GetMinMbrForce(int mbr,int dof);
   int GetMaxMbrForce(int mbr,int dof);

   int GetMinMbrDisp(int mbr,int dof);
   int GetMaxMbrDisp(int mbr,int dof);

   int GetMinPOIForce(int poi,int dof);
   int GetMaxPOIForce(int poi,int dof);

   int GetMinPOIDisp(int poi,int dof);
   int GetMaxPOIDisp(int poi,int dof);


   bool operator == (const Envelope &env) const;

protected:
   int ID; // envelope id

   struct TInteger {
     int i;
     TInteger() {}
     TInteger(int _i) {i = _i;}
     operator int() const {return i;}
   };
   typedef TArrayAsVector<TInteger> TIntArray;
   typedef TArrayAsVectorIterator<TInteger> TIntIterator;

   TIntArray *Loads; // container of loading ids

   void Create(ResultContainer *jntResults,ResultContainer *mbrResults,ResultContainer *poiResults);
   void CreatePseudoLoading(int minId,int maxId,
                            ResultContainer *jntResults,
                            ResultContainer *mbrResults,
                            ResultContainer *poiResults);
   void EnvelopeJnt(ResultContainer *jntResults);
   void EnvelopeMbr(ResultContainer *mbrResults);
   void EnvelopePOI(ResultContainer *poiResults);
   void CreatePseudoJntLoading(int minId,int maxId,ResultContainer *jntResults);
   void CreatePseudoMbrLoading(int minId,int maxId,ResultContainer *mbrResults);
   void CreatePseudoPOILoading(int minId,int maxId,ResultContainer *poiResults);

   // Nested class
   class _FEMCLASS EnvPoint
   {
   public:
      EnvPoint(int id,int ndof);
      ~EnvPoint();

      int GetID() const;
      int GetNumDOF() const;

      void SetMinForce(int dof,int min);
      int  GetMinForce(int dof) const;

      void SetMaxForce(int dof,int max);
      int  GetMaxForce(int dof) const;

      void SetMinDisp(int dof,int min);
      int  GetMinDisp(int dof) const;

      void SetMaxDisp(int dof,int max);
      int  GetMaxDisp(int dof) const;

      bool operator == (const EnvPoint &ep) const;

   private:
      int ID; // jnt, mbr, or poi id
      int NumDOF;
      int *MinForceID;
      int *MaxForceID;
      int *MinDispID;
      int *MaxDispID;
   };
   typedef TIDomainContainer<EnvPoint> EnvPointContainer;
   typedef TIDomainIterator<EnvPoint>  EnvPointIterator;

   EnvPointContainer *jntEnvelope;
   EnvPointContainer *mbrEnvelope;
   EnvPointContainer *poiEnvelope;

   friend FemModel;
};

typedef TIDomainContainer<Envelope> EnvelopeContainer;
typedef TIDomainIterator<Envelope>  EnvelopeIterator;

#endif // FEM2D_ENVELOPE_H_
