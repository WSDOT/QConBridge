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

#include <fem2d\loading.h>
#include <fem2d\femmodel.h>

Loading::Loading(int id)
{
   ID = id;
   jntLoads = new JntLoadContainer(10,0,10);
   jntLoads->OwnsElements(false);

   mbrLoads = new MbrLoadContainer(10,0,10);
   mbrLoads->OwnsElements(false);
}

Loading::~Loading()
{
   jntLoads->Flush(TShouldDelete::Delete);
   delete jntLoads;

   mbrLoads->Flush(TShouldDelete::Delete);
   delete mbrLoads;
}

int Loading::GetID() const
{
   return ID;
}

void Loading::AddJntLoad(JntLoad *load)
{
   jntLoads->Add(load);
}

void Loading::AddMbrLoad(MbrLoad *load)
{
   mbrLoads->Add(load);
}

void Loading::ApplyLoads(FemModel *model)
{
   JntLoadIterator jntLdIter = JntLoadIterator(*jntLoads);

   while (jntLdIter)
      {
      JntLoad *jntLd = jntLdIter++;
      Joint *jnt;
      jnt = model->GetJoint(jntLd->GetJoint());
      jnt->ApplyLoad(jntLd);
      }

   MbrLoadIterator mbrLdIter = MbrLoadIterator(*mbrLoads);

   while(mbrLdIter)
      {
      MbrLoad *mbrLd = mbrLdIter++;
      PlnFrameMbr *ele;
      ele = model->GetMember(mbrLd->GetMbr());
      ele->ApplyLoad(mbrLd);
      }
}

int Loading::operator == (const Loading &l) const
{
   return (ID == l.GetID());
}

void Loading::Write(ostream &os) const
{
   // If there is nothing to write, get the heck outta here.
   if (jntLoads->GetItemsInContainer() == 0 &&
       mbrLoads->GetItemsInContainer() == 0)
      return;


   os << "LOADING " << GetID() << endl;

   JntLoadIterator j = JntLoadIterator(*jntLoads);
   while (j)
      {
      JntLoad *jntLd = j++;
      os << *jntLd << endl;
      }

   MbrLoadIterator m = MbrLoadIterator(*mbrLoads);
   while (m)
      {
      MbrLoad *mbrLd = m++;
      os << (*mbrLd) << endl;
      }

   os << "$" << endl;
}

//
//
//
DeadLoading::DeadLoading(int id,GravityDir dir,double v,DeadLoadingType type) :
Loading(id)
{
   gravityDir = dir;
   loadingType = type;
   scale = v;
}

void DeadLoading::ApplyLoads(FemModel *model)
{
   Loading::ApplyLoads(model);

   ElementIterator e(*model->elements);
   while (e)
      {
      PlnFrameMbr *ele = e++;
      ele->GenDeadload(gravityDir,scale,loadingType);
      }
}

void DeadLoading::Write(ostream &os) const
{
   os << "DEAD LOADING " << ID << " DIRECTION ";
   switch(gravityDir)
      {
      case PosX:
           os << "X";
           break;

      case NegX:
           os << "-X";
           break;

      case PosY:
           os << "Y";
           break;

      case NegY:
           os << "-Y";
           break;
      }
   os << " FACTOR " << scale;
   os << " ALL MEMBERS" << endl;

   // Write additional loads using in this deadloading.
   JntLoadIterator j = JntLoadIterator(*jntLoads);
   while (j)
      {
      JntLoad *jntLd = j++;
      os << *jntLd << endl;
      }

   MbrLoadIterator m = MbrLoadIterator(*mbrLoads);
   while (m)
      {
      MbrLoad *mbrLd = m++;
      os << (*mbrLd) << endl;
      }

   os << "$" << endl;
}


ostream &operator << (ostream &os,const Loading &l)
{
   l.Write(os);
   return os;
}

