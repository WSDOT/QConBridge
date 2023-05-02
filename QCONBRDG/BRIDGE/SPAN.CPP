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

#include <bridge\span.h>
#include <bridge\bridge.h>
#include <fem2d\femmodel.h>

#define InRange(x,a,b) (((a)<(x) || zero_val(x-a)) && ((x)<(b) || zero_val(x-b)))

//
//
//
Span::Span(double l,TShape *s)
{
   ID = -1;
   length = l;
   Init();

   // every span must have one segment that starts at 0.00
   AddSegment(new Segment(0.00,s));

   leftSupport = NULL;
   rightSupport = NULL;
   nextSpan = NULL;
   prevSpan = NULL;

   loads = new BridgeLoadContainer(10,0,10);
   loads->OwnsElements(false);
}

Span::Span(Span &span)
{
    PRECONDITIONX(&span != NULL,"Span::Span() - invalid span in copy c'tor");

    ID = -2;

    Init();

   // Copy segments
   int i, numSegments;
   numSegments = span.NumSegments();

   for (i = 0; i < numSegments; i++)
      {
      Segment *seg = span.GetSegment(i);
      Segment *newSeg = new Segment(*seg);
      AddSegment(newSeg);
      }

   // Copy hinges
   int j;
   for (j = 0; j < span.NumHinges(); j++)
      {
      Hinge *hinge = span.GetHinge(j);
      Hinge *newHinge = new Hinge(hinge->GetLocation());
      AddHinge(newHinge);
      }

   // Copy Dimensions
   length = span.Length();

   // Copy Continuity
   startContinuity = span.StartContinuity();
   endContinuity = span.EndContinuity();

   leftSupport = NULL;
   rightSupport = NULL;
   nextSpan = NULL;
   prevSpan = NULL;

   loads = new BridgeLoadContainer(10,0,10);
   loads->OwnsElements(false);

   // should the loads applied to the span be copied?
}

Span::~Span()
{
   // Get rid of the hinges
   delete hinge[0];
   delete hinge[1];

   // Get rid of the segments
   delete segments;

   // Since this span is being deleted, all loads that are applied to this
   // span must be deleted as well. If not, then there will be loads assigned
   // to spans that do not exist. That would be a good thing.
   BridgeLoadIterator i(*loads);
   while(i)
      {
      BridgeLoad *load = i++;
      load->GetOwner()->RemoveLoad(load);

      // The above statements alters the size of the container...
      // the iterator must be restarted. To prevent us from having
      // to go back over all the previously scanned objects, we will
      // just go back one object and start again...
      i.Restart(((i-1) < 0) ? 0 : i-1,loads->GetItemsInContainer());
      }
   delete loads;

   delete members;

   leftSupport = NULL;
   rightSupport = NULL;
   nextSpan = NULL;
   prevSpan = NULL;
}

void Span::SetID(int id)
{
   ID = id;
}

int Span::GetID() const
{
   CHECKX(ID >= 0,"Span::GetID() - invalid ID detected");
   return ID;
}

void Span::AddSegment(Segment *segment)
{
   segments->Add(segment);
}

Segment *Span::GetSegment(int idx) const
{
   PRECONDITIONX(idx < segments->GetItemsInContainer(),"Span::GetSegment() - Invalid index");

   Segment *segment;
   segment = (*segments)[idx];

   CHECKX(segment,"Span::GetSegment() - Error getting segment");

   return segment;
}

void Span::ReplaceSegment(int idx,Segment *newSegment)
{
   RemoveSegment(idx);
   AddSegment(newSegment);
}

void Span::RemoveSegment(int idx)
{
   PRECONDITIONX(idx < segments->GetItemsInContainer(),"Span::RemoveSegment() - Invalid index");
   PRECONDITIONX(idx != 0,"Span::RemoveSegment() - Segment zero cannot be removed");

   segments->Detach(idx,TShouldDelete::Delete);
}


int Span::NumSegments() const
{
   return segments->GetItemsInContainer();
}


void Span::StartContinuity(Continuity continuity)
{
   startContinuity = continuity;
}


Continuity Span::StartContinuity() const
{
   return startContinuity;
}


void Span::EndContinuity(Continuity continuity)
{
   endContinuity = continuity;
}


Continuity Span::EndContinuity() const
{
   return endContinuity;
}


Segment *Span::FindSegment(double loc) const
{
   SegmentIterator i(*segments);
   Segment *segment,*prev;

   PRECONDITIONX(0 <= loc && loc <= Length(),"Span::FindSegment() - loc not on this span");

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

void Span::AddHinge(Hinge *aHinge)
{
   hinge[numHinges] = aHinge;
   numHinges++;
}


void Span::RemoveHinge(int n)
{
   delete hinge[n];
   hinge[n] = NULL;

   if (n == 0)
      {
      hinge[0] = hinge[1];
      hinge[1] = NULL;
      }

   numHinges--;
}


Hinge *Span::GetHinge(int n) const
{
   CHECKX(n == 0 || n == 1,"Span::GetHinge() - Hinge id must be 0 or 1");

   return hinge[n];
}

int Span::NumHinges() const
{
   return numHinges;
}

void Span::LeftSupport(Support *ls)
{
   leftSupport = ls;
}


Support *Span::LeftSupport() const
{
   return leftSupport;
}


void Span::RightSupport(Support *rs)
{
   rightSupport = rs;
}


Support *Span::RightSupport() const
{
   return rightSupport;
}

void Span::NextSpan(Span *next)
{
   nextSpan = next;
}

Span *Span::NextSpan() const
{
   return nextSpan;
}

void Span::PrevSpan(Span *prev)
{
   prevSpan = prev;
}

Span *Span::PrevSpan() const
{
   return prevSpan;
}


double Span::Length() const
{
   return length;
}

bool Span::IsCantilevered() const
{
   return (!LeftSupport() || !RightSupport());
}

void Span::GetStartPoint(double &x,double &y) const
{
   Span *current;

   x = 0;
   y = 0;

   current = PrevSpan();
   while(current)
      {
      x += current->Length();
      current = current->PrevSpan();
      }
}

void Span::GetEndPoint(double &x,double &y) const
{
   Span *current;

   x = Length();
   y = 0;

   current = PrevSpan();
   while(current)
      {
      x += current->Length();
      current = current->PrevSpan();
      }
}

void Span::Mirror()
{
   Continuity tempContinuity;
   Hinge *tempHinge;
   SegmentContainer *tempSegments;
   int i, numSegments;
   double loc;

   //
   // Swap end continuities
   //
   tempContinuity = startContinuity;
   startContinuity = endContinuity;
   endContinuity = tempContinuity;

   //
   // Swap hinges
   //
   if (numHinges > 0)
      {
      // Modify hinge location
      if (hinge[0])
         hinge[0]->SetLocation(Length() - hinge[0]->GetLocation());
      if (hinge[1])
         hinge[1]->SetLocation(Length() - hinge[1]->GetLocation());

      if (numHinges > 1)
         {
         // Swap hinge pointers
         tempHinge = hinge[0];
         hinge[0] = hinge[1];
         hinge[1] = tempHinge;
         }
      }

   //
   // Mirror Segments
   //

   // Get number of segmenst that need to be mirrored
   numSegments = segments->GetItemsInContainer();

   // Get a pointer to the current segment container
   tempSegments = segments;

   // Create a new segment container
   segments = new SegmentContainer(numSegments,0,5);
   segments->OwnsElements(0); // yes

   loc = 0.0;
   for (i = numSegments-1; i >= 0; i--)
      {
      // Get the segment data
      Segment *segment = (*tempSegments)[i];
      TShape *sect = segment->GetSection();

      // Create a new segment
      segments->Add(new Segment(loc,sect->Clone()));

      // Compute starting location of the next segment
      loc = Length() - segment->GetLocation();
      }

   // Wipe out the old segment container
   tempSegments->Flush(TShouldDelete::Delete);
   delete tempSegments;
}

void Span::AddLoad(BridgeLoad *load)
{
   loads->Add(load);

   CHECKX(load->TestLink(this),"Span::AddLoad() - bad link detected");
}

void Span::RemoveLoad(BridgeLoad *load)
{
   CHECKX(load->TestLink(this),"Span::RemoveLoad() - bad link detected");

   loads->Detach(load,TShouldDelete::NoDelete);
}

BridgeLoad *Span::GetLoad(int id)
{
   BridgeLoadIterator i(*loads);
   while(i)
      {
      BridgeLoad *load = i++;

      CHECKX(load->TestLink(this),"Span::GetLoad() - bad link detected");

      if (load->GetID() == id)
         return load;
      }

   return NULL;
}

void Span::Init()
{
   members = new TIntArray(5,0,5);
   segments = new SegmentContainer(5,0,5);

   startContinuity = ctContinuous;
   endContinuity   = ctContinuous;

   hinge[0] = NULL;
   hinge[1] = NULL;
   numHinges = 0;
}

void Span::ComputeSegmentLengths()
{
   PRECONDITIONX(segments->GetItemsInContainer() > 0,"Span::ComputeSegmentLengths() - There must be at least one segment");

   SegmentIterator i(*segments);
   Segment *prev = i++;
   while(i)
      {
      Segment *segment = i++;
      prev->SetLength(segment->GetLocation() - prev->GetLocation());
      prev = segment;
      }

   double Length = length;
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

int Span::GenerateFemComponents(int refJntId,Bridge *bridge)
{
   double spanStartX,spanStartY;
   double spanEndX,  spanEndY;
   double mbrEndX,mbrEndY;
   double lastMbrEndX, lastMbrEndY;
   double angle;
   int j;
   int startJnt,endJnt;
   FemModel *model = bridge->GetModel();
   SegmentIterator i(*segments);
   int mbrId;
   PlnFrameMbr *mbr;

   ComputeSegmentLengths();

   GetStartPoint(spanStartX,spanStartY);
   GetEndPoint(spanEndX,spanEndY);
   angle = atan2(spanEndY-spanStartY,spanEndX-spanStartX);

   startJnt = refJntId;

   while(i)
      {
      Segment *segment = i++;
      double segStartX = spanStartX + cos(angle)*segment->GetLocation();
      double segStartY = spanStartY + sin(angle)*segment->GetLocation();
      double segEndX   = segStartX  + cos(angle)*segment->GetLength();
      double segEndY   = segStartY  + sin(angle)*segment->GetLength();

      // Initialize last used point to be start of segment
      lastMbrEndX = segStartX;
      lastMbrEndY = segStartY;

      // Generate members for the hinges

      for (j = 0; j < NumHinges(); j++)
         {
         Hinge *aHinge = hinge[j];

         if (segment->GetLocation() < aHinge->GetLocation() &&
              aHinge->GetLocation() <= (segment->GetLocation() + segment->GetLength()) ||
              zero_val(aHinge->GetLocation()))
            {
            // Hinge is in this segment

            // Member ends at hinge location
            mbrEndX = segStartX + cos(angle)*(aHinge->GetLocation() - segment->GetLocation());
            mbrEndY = segStartY + sin(angle)*(aHinge->GetLocation() - segment->GetLocation());

            endJnt = bridge->NextAvailJnt();
            model->AddJoint(new Joint(endJnt,mbrEndX,mbrEndY));

            mbrId = bridge->NextAvailMbr();
            mbr = new PlnFrameMbr(mbrId,
                                  model->GetJoint(startJnt),
                                  model->GetJoint(endJnt),
                                  segment->GetSection());
            model->AddMember(mbr);
            members->Add(mbrId);

            if (zero_val(aHinge->GetLocation()))
               mbr->Release(mtPinFix); // Hinge at start of span
            else
               mbr->Release(mtFixPin);

            startJnt = endJnt;

            lastMbrEndX = mbrEndX;
            lastMbrEndY = mbrEndY;
            }
         }

      // Compute the distance from the last point and the
      // end of the segment. If it is not zero, then the last hinge
      // did not occur at the end of the segment. Generate a member from
      // the last hinge to the end of the segment.
      if (!zero_val(sqrt(pow(lastMbrEndX-segEndX,2) + pow(lastMbrEndY-segEndY,2))))
         {
         endJnt = bridge->NextAvailJnt();
         model->AddJoint(new Joint(endJnt,segEndX,segEndY));

         mbrId = bridge->NextAvailMbr();
         mbr = new PlnFrameMbr(mbrId,
                               model->GetJoint(startJnt),
                               model->GetJoint(endJnt),
                               segment->GetSection());
         model->AddMember(mbr);
         members->Add(mbrId);
         startJnt = endJnt;
         }
      }

   // Get the first member in the span and set its boundary condition
   mbr = model->GetMember((*members)[0]);
   mbr->Release(mbrStart,startContinuity == ctHinged ? mbrRelMz : mbrRelNone);

   // Get the last member in the span and set its boundary condition
   mbr = model->GetMember((*members)[members->GetItemsInContainer()-1]);
   mbr->Release(mbrEnd,endContinuity == ctHinged ? mbrRelMz : mbrRelNone);

   // Return the last joint id used (mbr is still the last member in the span)
   return mbr->GetJoint(1)->GetID();
}

void Span::ApplyLoad(int ld,BridgeLoad *load,Bridge *bridge)
{
   PRECONDITIONX(load->TestLink(this),"Span::ApplyLoad() - bad link detected");

   switch(load->Type())
      {
      case blConc:
           ApplyConcLoad(ld,TYPESAFE_DOWNCAST(load,ConcBridgeLoad),bridge);
           break;

      case blUnif:
           ApplyUnifLoad(ld,TYPESAFE_DOWNCAST(load,UnifBridgeLoad),bridge);
           break;
      }
}

void Span::ApplyConcLoad(int ld,ConcBridgeLoad *cload,Bridge *bridge)
{
   FemModel *model = bridge->GetModel();
   Loading *loading = model->GetLoading(ld);
   int mbrId;
   double l;
   double q;

   l = cload->StartLocation();
   q = -cload->Magnitude();
   if (cload->IsFract())
      l *= Length();
   FindMbr(model,l,mbrId,l);
   loading->AddMbrLoad(new ConcentratedLoad(mbrId,l,q,true,ForceY,Global));
}

void Span::ApplyUnifLoad(int ld,UnifBridgeLoad *uload,Bridge *bridge)
{
   FemModel *model = bridge->GetModel();
   Loading *loading = model->GetLoading(ld);
   double l1,l2;
   double q;
   int mbr1Id,mbr2Id;
   int i;

   l1 = uload->StartLocation();
   l2 = uload->EndLocation();
   q  = -uload->Magnitude();
   if (uload->IsFract())
      {
      l1 *= Length();
      l2 *= Length();
      }
   FindMbr(model,l1,mbr1Id,l1);
   FindMbr(model,l2,mbr2Id,l2);

   if (mbr1Id == mbr2Id)
      loading->AddMbrLoad(new UniformLoad(mbr1Id,l1,l2,q,true,ForceY,Global));
   else
      {
      for (i = mbr1Id; i <= mbr2Id; i++)
         {
         if (i == mbr1Id)
            loading->AddMbrLoad(new UniformLoad(i,l1,1.0,q,true,ForceY,Global));
         else if (i == mbr2Id)
            loading->AddMbrLoad(new UniformLoad(i,0,l2,q,true,ForceY,Global));
         else
            loading->AddMbrLoad(new UniformLoad(i,0.0,1.0,q,true,ForceY,Global));
         }
      }
}

void Span::FindMbr(FemModel *femModel,double loc,int &mbrId,double &dist)
{
   TIntIterator i(*members);

#if (__DEBUG==2) // CHECKX (See below)
   mbrId = -1;
#endif

   while(i)
      {
      PlnFrameMbr *mbr = femModel->GetMember(i++);
      Joint *start, *end;
      double x1,y1,x2,y2,x3,y3;

      start = mbr->GetJoint(0);
      start->GetPosition(x1,y1);

      end   = mbr->GetJoint(1);
      end->GetPosition(x2,y2);

      GetStartPoint(x3,y3);

      if (InRange(x3+loc,x1,x2))
         {
         mbrId = mbr->GetID();
         dist = (x3+loc) - x1;
         dist /= mbr->GetLength(); // fractional length of member
         }
      }

   CHECKX(mbrId >= 0,"Span::FindMbr() - Member was not found");
}

bool Span::OkToModel()
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
