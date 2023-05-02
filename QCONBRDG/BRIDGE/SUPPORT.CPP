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

#include <bridge\support.h>
#include <bridge\span.h>
#include <bridge\bridge.h>
#include <fem2d\femmodel.h>
#include <math.h>

DIAG_DECLARE_GROUP(Bridge);

//
//
//
Support::Support(double h,double bo,TShape* s)
{
   ID = -1;
   height = h;
   baseOffset = bo;

   Init();

   AddSegment(new Segment(0.00,s));

   isPier = true;
   supportFixity = fxFixed;
}


Support::Support()
{
   ID = -1;
   height = 0;
   baseOffset = 0;

   Init();

   isPier = false;
   supportFixity = fxPinned;
}

Support::Support(int id)
{
   ID = id;
   height = 0;
   baseOffset = 0;

   Init();

   isPier = false;
   supportFixity = fxPinned;
}

Support::Support(Support &s)
{
   ID = -1;
   Init();

   // Copy segments
   int i, numSegments;
   numSegments = s.NumSegments();

   for (i = 0; i < numSegments; i++)
      {
      Segment *seg = s.GetSegment(i);
      Segment *newSeg = new Segment(*seg);
      AddSegment(newSeg);
      }

   // Copy Dimensions
   height = s.Height();
   baseOffset = s.BaseOffset();

   // Copy Boundry Conditions
   topContinuity = s.TopContinuity();
   supportFixity = s.SupportFixity();

   isPier = s.IsPier();

   leftSpan = NULL;
   rightSpan = NULL;
   nextSupport = NULL;
   prevSupport = NULL;
}

Support::~Support()
{
   // Get rid of the segments
   segments->Flush();
   delete segments;

   delete members;

   leftSpan = NULL;
   rightSpan = NULL;
   nextSupport = NULL;
   prevSupport = NULL;
}

double Support::BaseOffset() const
{
   return baseOffset;
}

double Support::Height() const
{
   return height;
}


void Support::SupportFixity(Fixity fixity)
{
   supportFixity = fixity;
}


Fixity Support::SupportFixity() const
{
   return supportFixity;
}


void Support::TopContinuity(Continuity continuity)
{
   topContinuity = continuity;
}


Continuity Support::TopContinuity() const
{
   return topContinuity;
}

bool Support::IsPier() const
{
   return isPier;
}

Span *Support::LeftSpan() const
{
   return leftSpan;
}

Span *Support::RightSpan() const
{
   return rightSpan;
}

Support *Support::NextSupport() const
{
   return nextSupport;
}

Support *Support::PrevSupport() const
{
   return prevSupport;
}

void Support::GetTopPoint(double &x,double &y) const
{
   if (LeftSpan())
      LeftSpan()->GetEndPoint(x,y);
   else
      {
      x = 0;
      y = 0;
      }
}

void Support::GetBottomPoint(double &x,double &y) const
{
   GetTopPoint(x,y);
   x += BaseOffset();
   y -= Height();
}

//
// Private
//
void Support::LeftSpan(Span *s)
{
   leftSpan = s;
}

void Support::RightSpan(Span *s)
{
   rightSpan = s;
}

void Support::NextSupport(Support *s)
{
   nextSupport = s;
}


void Support::PrevSupport(Support *s)
{
   prevSupport = s;
}

void Support::SetID(int id)
{
   ID = id;
}

void Support::AddSegment(Segment *segment)
{
   segments->Add(segment);
}

Segment *Support::GetSegment(int idx) const
{
   PRECONDITIONX(idx < segments->GetItemsInContainer(),"Support::GetSegment() - Invalid index");

   Segment *segment;
   segment = (*segments)[idx];

   CHECKX(segment,"Support::GetSegment() - Error getting segment");

   return segment;
}

void Support::ReplaceSegment(int idx,Segment *newSegment)
{
// update association with hinges
   RemoveSegment(idx);
   AddSegment(newSegment);
}

void Support::RemoveSegment(int idx)
{
   PRECONDITIONX(idx < segments->GetItemsInContainer(),"Support::RemoveSegment() - Invalid index");
   PRECONDITIONX(idx != 0,"Support::RemoveSegment() - Segment zero cannot be removed");

   segments->Detach(idx,TShouldDelete::Delete);
}


int Support::NumSegments() const
{
   return segments->GetItemsInContainer();
}

Segment *Support::FindSegment(double loc) const
{
   SegmentIterator i(*segments);
   Segment *segment,*prev;

   prev = NULL;
   while(i)
      {
      segment = i++;
      if (loc < segment->GetLocation())
         {
         segment = prev;
         break;
         }
      prev = segment;
      }

   return segment;
}


int  Support::GetID() const
{
   CHECKX(ID >= 0,"Support::GetID() - Invalid support ID");
   return ID;
}

//
// private
//
void Support::Init()
{
	connectionJntId = -1;
	reactionJntId = -1;

   segments = new SegmentContainer(5,0,5);

   leftSpan = NULL;
   rightSpan = NULL;
   nextSupport = NULL;
   prevSupport = NULL;

   supportFixity = fxFixed;
   topContinuity = ctContinuous;

   members = new TIntArray(5,0,5);
}

void Support::ComputeSegmentLengths()
{
   // if there are no segments, get the heck outta here
   if (segments->GetItemsInContainer() == 0)
      return;

   SegmentIterator i(*segments);
   Segment *prev = i++;
   while(i)
      {
      Segment *segment = i++;
      prev->SetLength(segment->GetLocation() - prev->GetLocation());
      prev = segment;
      }

   double Length = sqrt(baseOffset*baseOffset + height*height);
   prev->SetLength(Length - prev->GetLocation());

#if defined GEN_DEBUG_CODE
// The sum of the segment lengths must be equal to the component length
   i.Restart();
   double total = 0;
   while (i)
      {
      Segment *segment = i++;
      total += segment->GetLength();
      }
   CHECKX(zero_val(Length - total),"Support::ComputeSegmentLengths() - Error computing lengths");
#endif // GEN_DEBUG_CODE
}

void Support::GenerateFemComponents(int refJntId,Bridge *bridge)
{
   Joint *jnt;
   FemModel *model = bridge->GetModel();

   connectionJntId = refJntId;

   if (!IsPier())
      {
      // Simple support... Simply adjust the joint fixity
      jnt = model->GetJoint(refJntId);
      SupportJoint(jnt,SupportFixity());
      reactionJntId = refJntId;
      return;
      }

   ComputeSegmentLengths();

   SegmentIterator i(*segments);
   double supStartX,supStartY;
   double supEndX,supEndY;
   double mbrStartX,mbrStartY;
   double mbrEndX,mbrEndY;
   double angle;
   int startJnt,endJnt;
   int mbrId;
   PlnFrameMbr *mbr;

   GetTopPoint(supStartX,supStartY);
   GetBottomPoint(supEndX,supEndY);
   angle = atan2(supEndY-supStartY,supEndX-supStartX);

   startJnt = refJntId;

   while(i)
      {
      Segment *segment = i++;
      mbrStartX = supStartX + cos(angle)*segment->GetLocation();
      mbrStartY = supStartY + sin(angle)*segment->GetLocation();
      mbrEndX   = mbrStartX + cos(angle)*segment->GetLength();
      mbrEndY   = mbrStartY + sin(angle)*segment->GetLength();

      endJnt = bridge->NextAvailJnt();
      model->AddJoint(new Joint(endJnt,mbrEndX,mbrEndY));

      mbrId = bridge->NextAvailMbr();
      mbr = new PlnFrameMbr(mbrId,
                            model->GetJoint(startJnt),
                            model->GetJoint(endJnt),
                            segment->GetSection());

      model->AddMember(mbr);
      members->Add(mbrId);
      startJnt = endJnt;
      }

   mbr = model->GetMember((*members)[0]);
   mbr->Release(mbrStart,topContinuity == ctHinged ? mbrRelMz : mbrRelNone);

	int index = members->GetItemsInContainer()-1;
   if ( index < 0 )
	   index = 0;
   mbr = model->GetMember((*members)[index]);
   SupportJoint(mbr->GetJoint(1),supportFixity);
   reactionJntId = mbr->GetJoint(1)->GetID();
}

int  Support::StartJntId() const
{
	return connectionJntId;
}

int  Support::EndJntId() const
{
	return reactionJntId;
}

void Support::SupportJoint(Joint *jnt,Fixity fixity)
{
   jnt->Support();
   switch(fixity)
      {
      case fxPinned:
           jnt->Release(jntRelMz);
           break;

      case fxRoller:
           jnt->Release(jntRelFx | jntRelMz);
           break;

      case fxFixed:
           break;
      }
}

bool Support::OkToModel()
{
   // Make sure that each segment as a valid Ax, Iz, and E
   SegmentIterator i(*segments);
   while(i)
      {
      Segment *segment = i++;
      TShape* section = segment->GetSection();
      if (!(section->GetArea() > 0))
         return false;

      if (!(section->GetIxx() > 0))
         return false;

      if (!(section->GetModE() > 0))
         return false;
      }

   return true;
}
