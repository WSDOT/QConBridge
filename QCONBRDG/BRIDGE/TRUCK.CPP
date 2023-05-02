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

#include <bridge\truck.h>
#include <bridge\debug.h>

Truck::Truck(int ID)
{
   id = ID;

   P = new TDoubleArray(3,0,3);
   d = new TDoubleArray(3,0,3);
}

Truck::Truck(Truck &truck)
{
   int axle;
   int numAxles;
   double wgt;
   double space;

   id = truck.GetID();

   P = new TDoubleArray(3,0,3);
   d = new TDoubleArray(3,0,3);

   numAxles = truck.GetNumAxles();

   for (axle = 0; axle < numAxles; axle++)
      {
      space = truck.GetAxleSpacing(axle);
      wgt = truck.GetAxleWeight(axle);
      AddAxle(space,wgt);
      }
}

Truck::~Truck()
{
   delete P;
   delete d;
}

void Truck::AddAxle(double space,double wgt)
{
   // if first axle, ignore spacing. first spacing must be zero.
   if (P->GetItemsInContainer() == 0)
      space = 0.;

   d->Add(space);
   P->Add(wgt);
}

int Truck::GetNumAxles() const
{
   return P->GetItemsInContainer();
}

double Truck::GetAxleWeight(int axle) const
{
   return (*P)[axle];
}

double Truck::GetAxleSpacing(int axle) const
{
   return (*d)[axle];
}

double Truck::GetTruckLength() const
{
   TDoubleIterator i(*d);
   double length = 0.;

   while (i)
      {
      double dx = i++;
      length += dx;
      }

   return length;
}

//
//
//
TruckTrain::TruckTrain(int ID)
{
   id = ID;
   dir = Forward;
   pivotTruck = 0;
   pivotAxle = 0;
   pivotOffset = 0;
   step = 0;
   currentLocation = 0;
   w = 0;

   IMtruck = 1.0;
   IMlane  = 1.0;

   trucks = new TruckContainer(5,0,5);
   headway = new TDoubleArray(5,0,5);
}

TruckTrain::~TruckTrain()
{
   delete trucks;
   delete headway;
}

void TruckTrain::AddTruck(Truck &truck,double hw)
{
   // if first truck, ignore headway. first headway must be zero.
   if (trucks->GetItemsInContainer() == 0)
      hw = 0;

   Truck *newTruck = new Truck(truck);

   trucks->Add(newTruck);
   headway->Add(hw);
}

int TruckTrain::GetNumTrucks() const
{
   return trucks->GetItemsInContainer();
}

Truck *TruckTrain::GetTruck(int trk)
{
   Truck *truck;
   truck = (*trucks)[trk];
   return truck;
}

double TruckTrain::GetHeadwaySpacing(int count)
{
   return (*headway)[count];
}

// Computes the position of an axle relative to the first axle
// on the first truck
double TruckTrain::GetRelAxlePosition(int trkPosition,int axle)
{
   double relPosition; // distance from the first axle on the first truck
                       // to the axle under consideration
   Truck *truck;       // pointer to the truck that has the axle under consideration
   int trk, i;         // counters

   // Get the truck
   truck = GetTruck(trkPosition);

   // Initialize
   relPosition = 0;

   // Add up length of all the trucks and headways in front of this truck
   for (trk = 0; trk < trkPosition; trk++)
      {
      relPosition += GetTruck(trk)->GetTruckLength();
      relPosition += (*headway)[trk];
      }

   // Add headway for this truck
   relPosition += (*headway)[trkPosition];

   // Add up axle spacings on this truck
   for (i = 0; i <= axle; i++)
      relPosition += truck->GetAxleSpacing(i);

   // Compute absolute position
   double sign;
   sign = (dir == Forward) ? -1 : 1;

   relPosition *= sign;

   return relPosition;
}

double TruckTrain::GetAxlePosition(int trk,int axle)
{
   double position;

   position = currentLocation + GetRelAxlePosition(trk,axle) - pivotOffset;

   return position;
}

int TruckTrain::GetTruckPosition(Truck *truck) const
{
   return trucks->Find(truck);
}

double TruckTrain::Evaluate(InfluenceLine &il,MbrFace face)
{
   double E = 0;
   double dE;
   int trk, numTrucks;
   int axle, numAxles;
   double axleWgt;
   double x;
   double iv;
   double area;
   Truck *truck;

   numTrucks = GetNumTrucks();

   #if defined GEN_DEBUG_CODE
   if (logTruck)
      {
      truckLog << "Evaluating useing influence line " << il.GetID() << endl;
      truckLog << "Face " << ((face == mbrLeftFace) ? "Left" : "Right") << endl;
      }
   #endif

   for (trk = 0; trk < numTrucks; trk++)
      {
      truck = GetTruck(trk);
      numAxles = truck->GetNumAxles();

      for (axle = 0; axle < numAxles; axle++)
         {
         axleWgt = truck->GetAxleWeight(axle);
         x = GetAxlePosition(trk,axle);
         iv = il.Evaluate(x,face);
         dE = iv * axleWgt * (1. + GetTruckImpact()/100.);
         E += dE;

#if defined GEN_DEBUG_CODE
         if (logTruck)
            {
            truckLog << "Truck " << truck->GetID() << " Axle " << axle
                     << " Wgt " << axleWgt << " X " << x
                     << " iv " << iv << " IM " << GetTruckImpact() << " dE " << dE << endl;
            }
#endif
         }
      }

   area = il.GetArea();
   dE = area * w * (1. + GetLaneImpact()/100.);
   E += dE;

#if defined GEN_DEBUG_CODE
   if (logTruck)
      {
      truckLog << "Area " << area << " w " << w << " IM " << GetLaneImpact() << " dE " << dE << endl;
      truckLog << "E = " << E << endl;
      }
#endif

   return E;
}

void TruckTrain::SetPivot(int truck,int axle)
{
   pivotTruck = truck;
   pivotAxle = axle;

   pivotOffset = GetRelAxlePosition(pivotTruck,pivotAxle);
}

void TruckTrain::GetPivot(int &truck,int &axle)
{
   truck = pivotTruck;
   axle = pivotAxle;
}

void TruckTrain::Move()
{
   currentLocation += step;
}

void TruckTrain::MoveTo(double newLocation)
{
   currentLocation = newLocation;
}

void TruckTrain::SetDirection(Direction d)
{
   dir = d;

   // Update the relative pivot axle position
   pivotOffset = GetRelAxlePosition(pivotTruck,pivotAxle);
}

