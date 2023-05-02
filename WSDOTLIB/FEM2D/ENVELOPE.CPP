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

#include "envelope.h"

#if !defined(__VALUES_H)
#include <values.h>
#endif

//
//
//
Envelope::Envelope(int id)
{
   ID = id;
   Loads = new TIntArray(20,0,10);

   jntEnvelope = new EnvPointContainer(20,0,10);
   mbrEnvelope = new EnvPointContainer(20,0,10);
   poiEnvelope = new EnvPointContainer(20,0,10);
}

Envelope::~Envelope()
{
   delete Loads;

   delete jntEnvelope;
   delete mbrEnvelope;
   delete poiEnvelope;
}

int Envelope::GetID() const
{
   return ID;
}

void Envelope::AddLoading(int id)
{
   Loads->Add(id);
}

void Envelope::Create(ResultContainer *jntResults,ResultContainer *mbrResults,ResultContainer *poiResults)
{
   EnvelopeJnt(jntResults);
   EnvelopeMbr(mbrResults);
   EnvelopePOI(poiResults);
}

int Envelope::GetMinJntForce(int jnt,int dof)
{
   EnvPoint *ep;
   ep = jntEnvelope->Get(jnt);
   return ep->GetMinForce(dof);
}

int Envelope::GetMaxJntForce(int jnt,int dof)
{
   EnvPoint *ep;
   ep = jntEnvelope->Get(jnt);
   return ep->GetMaxForce(dof);
}

int Envelope::GetMinJntDisp(int jnt,int dof)
{
   EnvPoint *ep;
   ep = jntEnvelope->Get(jnt);
   return ep->GetMinDisp(dof);
}

int Envelope::GetMaxJntDisp(int jnt,int dof)
{
   EnvPoint *ep;
   ep = jntEnvelope->Get(jnt);
   return ep->GetMaxDisp(dof);
}

int Envelope::GetMinMbrForce(int mbr,int dof)
{
   EnvPoint *ep;
   ep = mbrEnvelope->Get(mbr);
   return ep->GetMinForce(dof);
}

int Envelope::GetMaxMbrForce(int mbr,int dof)
{
   EnvPoint *ep;
   ep = mbrEnvelope->Get(mbr);
   return ep->GetMaxForce(dof);
}

int Envelope::GetMinMbrDisp(int mbr,int dof)
{
   EnvPoint *ep;
   ep = mbrEnvelope->Get(mbr);
   return ep->GetMinDisp(dof);
}

int Envelope::GetMaxMbrDisp(int mbr,int dof)
{
   EnvPoint *ep;
   ep = mbrEnvelope->Get(mbr);
   return ep->GetMaxDisp(dof);
}

int Envelope::GetMinPOIForce(int poi,int dof)
{
   EnvPoint *ep;
   ep = poiEnvelope->Get(poi);
   return ep->GetMinForce(dof);
}

int Envelope::GetMaxPOIForce(int poi,int dof)
{
   EnvPoint *ep;
   ep = poiEnvelope->Get(poi);
   return ep->GetMaxForce(dof);
}

int Envelope::GetMinPOIDisp(int poi,int dof)
{
   EnvPoint *ep;
   ep = poiEnvelope->Get(poi);
   return ep->GetMinDisp(dof);
}

int Envelope::GetMaxPOIDisp(int poi,int dof)
{
   EnvPoint *ep;
   ep = poiEnvelope->Get(poi);
   return ep->GetMaxDisp(dof);
}

bool Envelope::operator == (const Envelope &env) const
{
   return (ID == env.GetID());
}

void Envelope::EnvelopeJnt(ResultContainer *jntResults)
{
   ResultIterator i(*jntResults);
   int ndof;
   double maxForce[6];
   double minForce[6];
   double maxDisp[6];
   double minDisp[6];

   while(i)
      {
      ResultArray *array = i++;
      EnvPoint *envPnt = new EnvPoint(array->GetID(),3);

      for (int k = 0; k < 6; k++)
         {
         maxForce[k] = -MAXDOUBLE;
         minForce[k] =  MAXDOUBLE;
         maxDisp[k]  = -MAXDOUBLE;
         minDisp[k]  =  MAXDOUBLE;
         }

      TIntIterator l(*Loads);
      while(l)
         {
         int j = l++;
         Result *result = array->Get(j);
         int dof;
         ndof = result->GetNumDOF();
         for (dof = 0; dof < ndof; dof++)
            {
            double f,d;

            f = result->GetForce(dof);
            d = result->GetDisplacement(dof);

            if (f >= maxForce[dof])
               {
               maxForce[dof] = f;
               envPnt->SetMaxForce(dof,j);
               }

            if (f <= minForce[dof])
               {
               minForce[dof] = f;
               envPnt->SetMinForce(dof,j);
               }

            if (d >= maxDisp[dof])
               {
               maxDisp[dof] = d;
               envPnt->SetMaxDisp(dof,j);
               }

            if (d <= minDisp[dof])
               {
               minDisp[dof] = d;
               envPnt->SetMinDisp(dof,j);
               }
            }
         }

      jntEnvelope->Add(envPnt);
      }
}

void Envelope::EnvelopeMbr(ResultContainer *mbrResults)
{
   ResultIterator i(*mbrResults);
   int ndof;
   double maxForce[6];
   double minForce[6];
   double maxDisp[6];
   double minDisp[6];

   while(i)
      {
      ResultArray *array = i++;
      EnvPoint *envPnt = new EnvPoint(array->GetID(),6);

      for (int k = 0; k < 6; k++)
         {
         maxForce[k] = -MAXDOUBLE;
         minForce[k] =  MAXDOUBLE;
         maxDisp[k]  = -MAXDOUBLE;
         minDisp[k]  =  MAXDOUBLE;
         }

      TIntIterator l(*Loads);
      while(l)
         {
         int j = l++;
         Result *result = array->Get(j);
         int dof;
         ndof = result->GetNumDOF();
         for (dof = 0; dof < ndof; dof++)
            {
            double f,d;

            f = result->GetForce(dof);
            d = result->GetDisplacement(dof);

            if (f >= maxForce[dof])
               {
               maxForce[dof] = f;
               envPnt->SetMaxForce(dof,j);
               }

            if (f <= minForce[dof])
               {
               minForce[dof] = f;
               envPnt->SetMinForce(dof,j);
               }

            if (d >= maxDisp[dof])
               {
               maxDisp[dof] = d;
               envPnt->SetMaxDisp(dof,j);
               }

            if (d <= minDisp[dof])
               {
               minDisp[dof] = d;
               envPnt->SetMinDisp(dof,j);
               }
            }
         }

      mbrEnvelope->Add(envPnt);
      }
}

void Envelope::EnvelopePOI(ResultContainer *poiResults)
{
   ResultIterator i(*poiResults);
   int ndof;
   double maxForce[6];
   double minForce[6];
   double maxDisp[6];
   double minDisp[6];

   while(i)
      {
      ResultArray *array = i++;
      EnvPoint *envPnt = new EnvPoint(array->GetID(),6);

      for (int k = 0; k < 6; k++)
         {
         maxForce[k] = -MAXDOUBLE;
         minForce[k] =  MAXDOUBLE;
         maxDisp[k]  = -MAXDOUBLE;
         minDisp[k]  =  MAXDOUBLE;
         }

      TIntIterator l(*Loads);
      while(l)
         {
         int j = l++;
         Result *result = array->Get(j);
         int dof;
         ndof = result->GetNumDOF();
         for (dof = 0; dof < ndof; dof++)
            {
            double f,d;

            f = result->GetForce(dof);
            d = result->GetDisplacement(dof);

            if (f >= maxForce[dof])
               {
               maxForce[dof] = f;
               envPnt->SetMaxForce(dof,j);
               }

            if (f <= minForce[dof])
               {
               minForce[dof] = f;
               envPnt->SetMinForce(dof,j);
               }

            if (d >= maxDisp[dof])
               {
               maxDisp[dof] = d;
               envPnt->SetMaxDisp(dof,j);
               }

            if (d <= minDisp[dof])
               {
               minDisp[dof] = d;
               envPnt->SetMinDisp(dof,j);
               }
            }
         }

      poiEnvelope->Add(envPnt);
      }
}

void Envelope::CreatePseudoLoading(int minId,int maxId,
                                    ResultContainer *jntResults,
                                    ResultContainer *mbrResults,
                                    ResultContainer *poiResults)
{
   CreatePseudoJntLoading(minId,maxId,jntResults);
   CreatePseudoMbrLoading(minId,maxId,mbrResults);
   CreatePseudoPOILoading(minId,maxId,poiResults);
}

void Envelope::CreatePseudoJntLoading(int minId,int maxId,ResultContainer *jntResults)
{
   EnvPointIterator i(*jntEnvelope);
   double minForce[3];
   double maxForce[3];
   double minDisp[3];
   double maxDisp[3];

   while(i)
      {
      EnvPoint *envPnt = i++;
      ResultArray *array = jntResults->Get(envPnt->GetID());

      int dof;
      for (dof = 0; dof < 3; dof++)
         {
         int minF,maxF,minD,maxD;

         minF = envPnt->GetMinForce(dof);
         maxF = envPnt->GetMaxForce(dof);

         minD = envPnt->GetMinDisp(dof);
         maxD = envPnt->GetMaxDisp(dof);

         Result *result;

         result = array->Get(minF);
         minForce[dof] = result->GetForce(dof);

         result = array->Get(maxF);
         maxForce[dof] = result->GetForce(dof);

         result = array->Get(minD);
         minDisp[dof] = result->GetDisplacement(dof);

         result = array->Get(maxD);
         maxDisp[dof] = result->GetDisplacement(dof);
         }

      Result *minResult = new Result(minId,3,minForce,minDisp);
      Result *maxResult = new Result(maxId,3,maxForce,maxDisp);
      array->Add(minResult);
      array->Add(maxResult);
      }
}

void Envelope::CreatePseudoMbrLoading(int minId,int maxId,ResultContainer *mbrResults)
{
   EnvPointIterator i(*mbrEnvelope);
   double minForce[6];
   double maxForce[6];
   double minDisp[6];
   double maxDisp[6];

   while(i)
      {
      EnvPoint *envPnt = i++;
      ResultArray *array = mbrResults->Get(envPnt->GetID());

      int dof;
      for (dof = 0; dof < 6; dof++)
         {
         int minF,maxF,minD,maxD;

         minF = envPnt->GetMinForce(dof);
         maxF = envPnt->GetMaxForce(dof);

         minD = envPnt->GetMinDisp(dof);
         maxD = envPnt->GetMaxDisp(dof);

         Result *result;

         result = array->Get(minF);
         minForce[dof] = result->GetForce(dof);

         result = array->Get(maxF);
         maxForce[dof] = result->GetForce(dof);

         result = array->Get(minD);
         minDisp[dof] = result->GetDisplacement(dof);

         result = array->Get(maxD);
         maxDisp[dof] = result->GetDisplacement(dof);
         }

      Result *minResult = new Result(minId,6,minForce,minDisp);
      Result *maxResult = new Result(maxId,6,maxForce,maxDisp);
      array->Add(minResult);
      array->Add(maxResult);
      }
}

void Envelope::CreatePseudoPOILoading(int minId,int maxId,ResultContainer *poiResults)
{
   EnvPointIterator i(*poiEnvelope);
   double minForce[6];
   double maxForce[6];
   double minDisp[6];
   double maxDisp[6];

   while(i)
      {
      EnvPoint *envPnt = i++;
      ResultArray *array = poiResults->Get(envPnt->GetID());

      int dof;
      for (dof = 0; dof < 6; dof++)
         {
         int minF,maxF,minD,maxD;

         minF = envPnt->GetMinForce(dof);
         maxF = envPnt->GetMaxForce(dof);

         minD = envPnt->GetMinDisp(dof);
         maxD = envPnt->GetMaxDisp(dof);

         Result *result;

         result = array->Get(minF);
         minForce[dof] = result->GetForce(dof);

         result = array->Get(maxF);
         maxForce[dof] = result->GetForce(dof);

         result = array->Get(minD);
         minDisp[dof] = result->GetDisplacement(dof);

         result = array->Get(maxD);
         maxDisp[dof] = result->GetDisplacement(dof);
         }

      Result *minResult = new Result(minId,6,minForce,minDisp);
      Result *maxResult = new Result(maxId,6,maxForce,maxDisp);
      array->Add(minResult);
      array->Add(maxResult);
      }
}

//

Envelope::EnvPoint::EnvPoint(int id,int ndof)
{
   ID = id;
   NumDOF = ndof;

   MinForceID = new int[NumDOF];
   MaxForceID = new int[NumDOF];
   MinDispID  = new int[NumDOF];
   MaxDispID  = new int[NumDOF];
}

Envelope::EnvPoint::~EnvPoint()
{
   delete[] MinForceID;
   delete[] MaxForceID;
   delete[] MinDispID;
   delete[] MaxDispID;
}

int Envelope::EnvPoint::GetID() const
{
   return ID;
}

int Envelope::EnvPoint::GetNumDOF() const
{
   return NumDOF;
}

void Envelope::EnvPoint::SetMinForce(int dof,int min)
{
   MinForceID[dof] = min;
}

int Envelope::EnvPoint::GetMinForce(int dof) const
{
   return MinForceID[dof];
}

void Envelope::EnvPoint::SetMaxForce(int dof,int max)
{
   MaxForceID[dof] = max;
}

int Envelope::EnvPoint::GetMaxForce(int dof) const
{
   return MaxForceID[dof];
}

void Envelope::EnvPoint::SetMinDisp(int dof,int min)
{
   MinDispID[dof] = min;
}

int Envelope::EnvPoint::GetMinDisp(int dof) const
{
   return MinDispID[dof];
}

void Envelope::EnvPoint::SetMaxDisp(int dof,int max)
{
   MaxDispID[dof] = max;
}

int Envelope::EnvPoint::GetMaxDisp(int dof) const
{
   return MaxDispID[dof];
}

bool Envelope::EnvPoint::operator == (const EnvPoint &ep) const
{
   return (ID == ep.GetID());
}

