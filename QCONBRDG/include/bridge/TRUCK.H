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

#if !defined BRIDGE_TRUCK_H_
#define BRIDGE_TRUCK_H_

#include <bridge\brdgdll.h>
#include <bridge\inflline.h>
#include <classlib\arrays.h>

class _BRIDGECLASS Truck
{
public:
   Truck(int ID);
   Truck(Truck &truck);
   virtual ~Truck();
   void AddAxle(double space,double wgt);
   int GetNumAxles() const;
   double GetAxleWeight(int axle) const;
   double GetAxleSpacing(int axle) const;
   double GetTruckLength() const;
   int GetID() const {return id;}

   bool operator == (const Truck &trk) const {return GetID() == trk.GetID();}

protected:
   int id;
   typedef TArrayAsVector<double> TDoubleArray;
   typedef TArrayAsVectorIterator<double> TDoubleIterator;
   TDoubleArray *P;
   TDoubleArray *d;
};

typedef TIArrayAsVector<Truck>         TruckContainer;
typedef TIArrayAsVectorIterator<Truck> TruckIterator;

class _BRIDGECLASS TruckTrain
{
public:
   enum Direction {Forward,Backward};

   TruckTrain(int ID);
   virtual ~TruckTrain();
   void AddTruck(Truck &truck,double hw);
   int GetNumTrucks() const;
   Truck *GetTruck(int trk);
   double GetAxlePosition(int trk,int axle);
   double GetHeadwaySpacing(int count);
   double Evaluate(InfluenceLine &il,MbrFace face = mbrLeftFace);
   void SetLaneLoad(double _w)       {w = _w;}
   double GetLaneLoad() const        {return w;}
   void SetStepSize(double stepSize) {step = stepSize;}
   double GetStepSize() const        {return step;}
   void Move();
   void MoveTo(double newLocation);
   void SetDirection(Direction d);
   Direction GetDirection() const    {return dir;}
   int GetID() const                 {return id;}
   void SetTruckImpact(double im)    {IMtruck = im;}
   double GetTruckImpact() const     {return IMtruck;}
   void SetLaneImpact(double im)     {IMlane = im;}
   double GetLaneImpact() const      {return IMlane;}
   void SetPivot(int truck,int axle);
   void GetPivot(int &truck,int &axle);
   bool operator == (const TruckTrain &tt) const {return GetID() == tt.GetID();}

protected:
   int id;
   Direction dir;
   double step;
   double currentLocation;
   double pivotOffset;
   double w;
   double IMtruck;
   double IMlane;
   TruckContainer *trucks;
   int pivotTruck;
   int pivotAxle;
   typedef TArrayAsVector<double> TDoubleArray;
   typedef TArrayAsVectorIterator<double> TDoubleIterator;
   TDoubleArray *headway;

   double GetRelAxlePosition(int trk,int axle);
   int GetTruckPosition(Truck *truck) const;
};

typedef TIArrayAsVector<TruckTrain>         TruckTrainContainer;
typedef TIArrayAsVectorIterator<TruckTrain> TruckTrainIterator;

#endif // BRIDGE_TRUCK_H_