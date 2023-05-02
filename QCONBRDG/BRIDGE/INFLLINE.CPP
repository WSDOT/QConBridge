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

#include <bridge\inflline.h>
#include <bridge\debug.h>

DIAG_DEFINE_GROUP(InfluenceLine,DISABLED,LEVEL3);

InfluenceLine::InfluenceLine(int id,const char *desc) :
Diagram(id,desc)
{
   mode = Normal;
   updateAreas = true;
   posArea = 0;
   negArea = 0;
   netArea = 0;
}

InfluenceLine::InfluenceLine(int id,InfluenceLine &il,const char *desc) :
Diagram(id,il,desc)
{
   mode = Normal;
   updateAreas = true;
   UpdateAreas();
}

InfluenceLine::InfluenceLine(int id,AnalysisResults &ar) :
Diagram(id,ar)
{
   mode = Normal;
   updateAreas = true;
   UpdateAreas();
}

double InfluenceLine::Evaluate(double x,MbrFace face)
{
   double y;

   // If x is beyond range of influence line, then there is no influence
   if (x < minX || x > maxX)
      return 0.0;

   y = Diagram::Evaluate(x,face);

   switch (mode)
      {
      case Normal:
           break;

      case ExcludePosValues:
           y = (y > 0) ? 0 : y;
           break;

      case ExcludeNegValues:
           y = (y < 0) ? 0 : y;
           break;
      }

   return y;
}

double InfluenceLine::GetArea() const
{
   double area;
   switch (mode)
      {
      case Normal:
           area = GetNetArea();
           break;

      case ExcludePosValues:
           area = GetNegArea();
           break;

      case ExcludeNegValues:
           area = GetPosArea();
           break;
      }

   return area;
}

void InfluenceLine::AddPoint(Diagram::Point *point)
{
   Diagram::AddPoint(point);

   if (updateAreas)
      UpdateAreas();
}

void InfluenceLine::EnableUpdate(bool enable,bool recalc)
{
   updateAreas = enable;
   if (updateAreas && recalc)
      UpdateAreas();
}

void InfluenceLine::UpdateAreas()
{
   TRACEX(InfluenceLine,LEVEL2,"*** Influence Line " << GetID());

   PointIterator i(*points);
   Diagram::Point *p1, *p2;
   double dA,dA1, dA2;
   double dx,dx1,dx2,dy;

   posArea = 0;
   negArea = 0;
   netArea = 0;

   p1 = i++;
   while(i)
      {
      p2 = i++;
      dx = p2->x - p1->x;
      dy = p2->y - p1->y;

      TRACEX(InfluenceLine,LEVEL3,"P1 x = " << p1->x << " y = " << p1->y);
      TRACEX(InfluenceLine,LEVEL3,"P2 x = " << p2->x << " y = " << p2->y);

      if ( (p1->y > 0 && p2->y < 0) || (p1->y < 0 && p2->y > 0) )
         {
         // Sign reversal occurs between these points
         dx1 = -p1->y*dx/dy;
         dx2 = dx - dx1;
         dA1 = 0.5*p1->y*dx1;
         dA2 = 0.5*p2->y*dx2;

         TRACEX(InfluenceLine,LEVEL3,"dx1 = " << dx1 << " p1->y = " << p1->y << " dA1 = " << dA1);
         TRACEX(InfluenceLine,LEVEL3,"dx2 = " << dx2 << " p2->y = " << p2->y << " dA2 = " << dA2);

         if (dA1 < 0)
            negArea += dA1;
         else
            posArea += dA1;

         if (dA2 < 0)
            negArea += dA2;
         else
            posArea += dA2;

         netArea += (dA1 + dA2);
         }
      else
         {
         // No sign reversal. Compute area of trapazoid
         dA = p1->y*dx + 0.5*dx*dy;

         TRACEX(InfluenceLine,LEVEL3,"dx = " << dx << " dy = " << dy << " dA = " << dA);

         if (dA > 0)
            posArea += dA;
         else
            negArea += dA;

         netArea += dA;
         }

      p1 = p2;
      }

   TRACEX(InfluenceLine,LEVEL2,"+Area = "       << posArea
                            << " -Area = "       << negArea
                            << " NetArea = "     << netArea);
}

#if defined GEN_DEBUG_CODE
void InfluenceLine::Write(ostream &os)
{
   Diagram::Write(os);

   os << "+Area = " << GetPosArea() << endl;
   os << "-Area = " << GetNegArea() << endl;
}
#endif

