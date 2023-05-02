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

#include <fem2d\combo.h>
#include <mem.h>

//
//
//
LoadingCombo::LoadingCombo(int id)
{
   ID = id;
   Scale = 1.0;

   comboData = new ComboDataContainer(10,0,10);
   comboData->OwnsElements(0); // Yes
}

LoadingCombo::~LoadingCombo()
{
   comboData->Flush(TShouldDelete::Delete);
   delete comboData;
}

int LoadingCombo::GetID() const
{
   return ID;
}

void LoadingCombo::AddLoadingData(int ld,double factor)
{
   comboData->Add(new ComboData(ld,factor));
}

void LoadingCombo::SetScale(double s)
{
   Scale = s;
}
/*
int LoadingCombo::GetNumLoads() const
{
   return numLoads;
}

int LoadingCombo::GetLoadID(int idx) const
{
   return Loads[idx];
}

double LoadingCombo::GetFactor(int idx) const
{
   return Factors[idx];
}
*/
double LoadingCombo::GetScale() const
{
   return Scale;
}

void LoadingCombo::CombineResults(ResultContainer *results)
{
   ResultIterator i(*results);
   int ndof;
   double force[6];
   double disp[6];

   while(i)
      {
      ResultArray *array = i++;
      memset(force,0,6*sizeof(double));
      memset(disp,0,6*sizeof(double));

      ComboDataIterator j(*comboData);

      while(j)
         {
         ComboData *data = j++;

         Result find(data->loadId,0,0,0);
         int idx = array->Find(&find);
         CHECKX(idx != INT_MAX,"LoadingCombo::CombineResults() - Results not found");
         Result* result = (*array)[idx];

         int dof;
         ndof = result->GetNumDOF();
         for (dof = 0; dof < ndof; dof++)
            {
            force[dof] += Scale * data->factor * result->GetForce(dof);
            disp[dof]  += Scale * data->factor * result->GetDisplacement(dof);
            }
         }

      Result *newResult = new Result(ID,ndof,force,disp);
      array->Add(newResult);
      }
}

bool LoadingCombo::operator == (const LoadingCombo &combo) const
{
   return (ID == combo.GetID());
}

//
//
//
ostream &operator << (ostream &os,const LoadingCombo &c)
{
   os << "LOADING COMBINATION " << c.GetID() << " COMBINE";

   LoadingCombo::ComboDataIterator i(*(c.comboData));

   while(i)
      {
      LoadingCombo::ComboData *data = i++;
      os << " -" << endl << data->loadId << " " << data->factor;
      }
   os << endl;
   os << "COMBINE " << c.GetID() << endl;

   return os;
}

