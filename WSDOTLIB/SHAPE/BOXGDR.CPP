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

#include <shape\boxgdr.h>
#include <material\genmat.h>

TBoxGirder::TBoxGirder(double bsw,double bst,double bh,EXTWEB &lWeb,EXTWEB &rWeb,
                       TOPSLAB &ts,Material* material) :
TSection()
{
   botSlabWidth    = bsw;
   botSlabThick    = bst;
   boxHeight       = bh;
   lextWeb         = lWeb;
   rextWeb         = rWeb;
   topSlab         = ts;

   GenerateSection(material);
}

void TBoxGirder::GenerateSection(Material* material)
{
   double lewThick, rewThick; // thickness measured horizontal
   double lAngle, rAngle;
   TFPoint *p;

   p = new TFPoint[26];

   lAngle = atan2(boxHeight + topSlab.thick - topSlab.leftExtThick, lextWeb.offset);
   rAngle = atan2(boxHeight + topSlab.thick - topSlab.rightExtThick,rextWeb.offset);

   if (lextWeb.horizontal)
      lewThick = lextWeb.thick;
   else
      lewThick = lextWeb.thick/sin(lAngle);

   if (rextWeb.horizontal)
      rewThick = rextWeb.thick;
   else
      rewThick = rextWeb.thick/sin(rAngle);

   // Create Points

   p[0] = TFPoint(topSlab.leftOverhang+lextWeb.offset,0);
   p[1] = TFPoint(topSlab.leftOverhang+lextWeb.offset-botSlabThick/tan(lAngle),botSlabThick);
   p[2] = TFPoint(topSlab.leftOverhang+lextWeb.offset+botSlabWidth+botSlabThick/tan(rAngle),botSlabThick);
   p[3] = TFPoint(topSlab.leftOverhang+lextWeb.offset+botSlabWidth,0);
   p[4] = TFPoint(topSlab.leftOverhang+lextWeb.offset-boxHeight/tan(lAngle),boxHeight);
   p[5] = TFPoint(p[4].x+lewThick+lextWeb.taper,boxHeight);
   p[6] = TFPoint(p[1].x+lewThick,p[1].y);

   double d,dx;
   d  = boxHeight - topSlab.thick;
   dx = p[6].x - p[5].x;

   p[7] = TFPoint(p[5].x + dx*lextWeb.bTopFillet/d,boxHeight-lextWeb.hTopFillet);
   p[8] = TFPoint(p[7].x + lextWeb.bTopFillet,p[7].y + lextWeb.hTopFillet);
   p[9] = TFPoint(p[6].x - dx*lextWeb.bTopFillet/d,p[6].y + lextWeb.hBotFillet);
   p[10] = TFPoint(p[9].x + lextWeb.bBotFillet,p[9].y - lextWeb.hBotFillet);
   p[11] = TFPoint(p[3].x + boxHeight/tan(rAngle),boxHeight);
   p[12] = TFPoint(p[11].x-rewThick-rextWeb.taper,boxHeight);
   p[13] = TFPoint(p[2].x-rewThick,p[2].y);

   dx = p[12].x - p[13].x;
   p[14] = TFPoint(p[12].x - dx*rextWeb.bTopFillet/d,p[12].y - rextWeb.hTopFillet);
   p[15] = TFPoint(p[14].x - rextWeb.bTopFillet,p[14].y + rextWeb.hTopFillet);
   p[16] = TFPoint(p[13].x + dx*rextWeb.bTopFillet/d,p[13].y + rextWeb.hBotFillet);
   p[17] = TFPoint(p[16].x - rextWeb.bBotFillet,p[16].y - rextWeb.hBotFillet);

   p[18] = TFPoint(topSlab.leftOverhang,boxHeight+topSlab.thick-topSlab.leftIntThick);
   p[19] = TFPoint(0,boxHeight+topSlab.thick-topSlab.leftExtThick);
   p[20] = TFPoint(0,boxHeight+topSlab.thick-topSlab.tWearing);
   p[21] = TFPoint(topSlab.leftOverhang+lextWeb.offset+botSlabWidth+rextWeb.offset+topSlab.rightOverhang,
                   boxHeight+topSlab.thick-topSlab.tWearing);
   p[22] = TFPoint(topSlab.leftOverhang+lextWeb.offset+botSlabWidth+rextWeb.offset+topSlab.rightOverhang,
                   boxHeight+topSlab.thick-topSlab.rightExtThick);
   p[23] = TFPoint(topSlab.leftOverhang+lextWeb.offset+botSlabWidth+rextWeb.offset,
                   boxHeight+topSlab.thick-topSlab.rightIntThick);
   p[24] = TFPoint(0,boxHeight + topSlab.thick);
   p[25] = TFPoint(topSlab.leftOverhang+lextWeb.offset+botSlabWidth+rextWeb.offset,
                   boxHeight + topSlab.thick);

/*
   p[0].x = -botSlabWidth/2;
   p[0].y = 0;

   p[1].x = -botSlabWidth/2 - topSlab.thick*(lextWeb.offset/boxHeight);
   p[1].y = botSlabThick;

   p[2].x = botSlabWidth/2 + topSlab.thick*(rextWeb.offset/boxHeight);
   p[2].y = botSlabThick;

   p[3].x = botSlabWidth/2;
   p[3].y = 0;

   p[4].x = -botSlabWidth/2 - lextWeb.offset;
   p[4].y = boxHeight;

   p[5].x = -botSlabWidth/2 - lextWeb.offset + lewThick + lextWeb.taper;
   p[5].y = boxHeight;

   p[6].x = p[1].x + lewThick;
   p[6].y = botSlabThick;

   dx = p[5].x - p[6].x;
   d1 = boxHeight - botSlabThick;

   p[7].x = p[5].x - dx*lextWeb.hTopFillet/d1;
   p[7].y = p[5].y - lextWeb.hTopFillet;

   p[8].x = p[7].x + lextWeb.bTopFillet;
   p[8].y = p[7].y + lextWeb.hTopFillet;

   p[9].x = p[6].x + dx*lextWeb.hBotFillet/d1;
   p[9].y = p[6].y + lextWeb.hBotFillet;

   p[10].x = p[9].x + lextWeb.bBotFillet;
   p[10].y = p[9].y - lextWeb.hBotFillet;

   p[11].x = botSlabWidth/2 + rextWeb.offset;
   p[11].y = boxHeight;

   p[12].x = botSlabWidth/2 + rextWeb.offset - rewThick - rextWeb.taper;
   p[12].y = boxHeight;

   p[13].x = p[2].x - rewThick;
   p[13].y = botSlabThick;

   dx = p[12].x - p[13].x;

   p[14].x = p[12].x - dx*rextWeb.hTopFillet/d1;
   p[14].y = p[12].y - rextWeb.hTopFillet;

   p[15].x = p[14].x - rextWeb.bTopFillet;
   p[15].y = p[14].y + rextWeb.hTopFillet;

   p[16].x = p[13].x + dx*rextWeb.hBotFillet/d1;
   p[16].y = p[13].y + rextWeb.hBotFillet;

   p[17].x = p[16].x - rextWeb.bBotFillet;
   p[17].y = p[16].y - rextWeb.hBotFillet;

   // Top Slab
   p[18].x = p[4].x + (topSlab.leftIntThick - topSlab.thick)*(lextWeb.offset/boxHeight);
   p[18].y = p[4].y - (topSlab.leftIntThick - topSlab.thick);

   p[19].x = -botSlabWidth/2 - topSlab.leftOverhang;
   p[19].y = boxHeight + topSlab.thick - topSlab.leftExtThick;

   p[20].x = -botSlabWidth/2 - topSlab.leftOverhang;
   p[20].y = boxHeight + topSlab.thick - topSlab.tWearing;

   p[21].x = botSlabWidth/2 + topSlab.rightOverhang;
   p[21].y = boxHeight + topSlab.thick - topSlab.tWearing;

   p[22].x = botSlabWidth/2 + topSlab.rightOverhang;
   p[22].y = boxHeight + topSlab.thick - topSlab.rightExtThick;

   p[23].x = p[11].x - (topSlab.rightIntThick - topSlab.thick)*(rextWeb.offset/boxHeight);
   p[23].y = p[11].y - (topSlab.rightIntThick - topSlab.thick);

   // Wearing Surface
   p[24].x = -botSlabWidth/2 - topSlab.leftOverhang;
   p[24].y = boxHeight + topSlab.thick;

   p[25].x = botSlabWidth/2 + topSlab.rightOverhang;
   p[25].y = boxHeight + topSlab.thick;
*/
   // Assemble Shapes

   // Bottom Slab
   botSlabShape = new TPolyShape(material,TFPoint(0,0),0);
   botSlabShape->AddPoint(p[0]);
   botSlabShape->AddPoint(p[1]);
   botSlabShape->AddPoint(p[2]);
   botSlabShape->AddPoint(p[3]);
   AddShape(botSlabShape);

   // general shape pointer
   TPolyShape *shape;

   // Left exterior web
   shape = new TPolyShape(material->Clone(),TFPoint(0,0),0);
   shape->AddPoint(p[1]);
   shape->AddPoint(p[4]);
   shape->AddPoint(p[5]);
   shape->AddPoint(p[6]);
   AddShape(shape);

   // Left exterior web top fillet
   shape = new TPolyShape(material->Clone(),TFPoint(0,0),0);
   shape->AddPoint(p[5]);
   shape->AddPoint(p[8]);
   shape->AddPoint(p[7]);
   AddShape(shape);

   // Left exterior web bottom fillet
   shape = new TPolyShape(material->Clone(),TFPoint(0,0),0);
   shape->AddPoint(p[6]);
   shape->AddPoint(p[9]);
   shape->AddPoint(p[10]);
   AddShape(shape);

   // Right exterior web
   shape = new TPolyShape(material->Clone(),TFPoint(0,0),0);
   shape->AddPoint(p[2]);
   shape->AddPoint(p[11]);
   shape->AddPoint(p[12]);
   shape->AddPoint(p[13]);
   AddShape(shape);

   // Right exterior web top fillet
   shape = new TPolyShape(material->Clone(),TFPoint(0,0),0);
   shape->AddPoint(p[12]);
   shape->AddPoint(p[15]);
   shape->AddPoint(p[14]);
   AddShape(shape);

   // Right exterior web bottom fillet
   shape = new TPolyShape(material->Clone(),TFPoint(0,0),0);
   shape->AddPoint(p[13]);
   shape->AddPoint(p[16]);
   shape->AddPoint(p[17]);
   AddShape(shape);

   // Top Slab
   topSlabShape = new TPolyShape(new GeneralMaterial(topSlab.modE,topSlab.density),TFPoint(0,0));
   topSlabShape->AddPoint(p[4]);
   topSlabShape->AddPoint(p[18]);
   topSlabShape->AddPoint(p[19]);
   topSlabShape->AddPoint(p[20]);
   topSlabShape->AddPoint(p[21]);
   topSlabShape->AddPoint(p[22]);
   topSlabShape->AddPoint(p[23]);
   topSlabShape->AddPoint(p[11]);
   AddShape(topSlabShape);

   // Wearing Surface
   wearingSurface = new TPolyShape(material->Clone(),TFPoint(0,0),0);
   wearingSurface->IsStructural(FALSE);
   wearingSurface->AddPoint(p[20]);
   wearingSurface->AddPoint(p[24]);
   wearingSurface->AddPoint(p[25]);
   wearingSurface->AddPoint(p[21]);
   AddShape(wearingSurface);
}

void TBoxGirder::AddInteriorWeb(double loc,double space,int nwebs,INTWEB &web,Material* material)
{
   int w;

   for (w = 0; w < nwebs; w++)
      {
      AddInteriorWeb(loc + w*space,web,(w==0) ? material : material->Clone());
      }
}

void TBoxGirder::AddInteriorWeb(double loc,INTWEB &web,Material* material)
{
   // loc = location from bottom left point on bottom slab (p[0])
   TFPoint rp(loc+topSlab.leftOverhang+lextWeb.offset,botSlabThick);
   TFPoint *p;

   p = new TFPoint[12];

   p[0] = rp + TFPoint(-web.thick/2,0);
   p[1] = rp + TFPoint(-web.thick/2-web.lTaper,boxHeight-botSlabThick);
   p[2] = rp + TFPoint(web.thick/2+web.rTaper,boxHeight-botSlabThick);
   p[3] = rp + TFPoint(web.thick/2,0);
   p[4] = p[1] + TFPoint(web.hTopLeftFillet*web.lTaper/(boxHeight-botSlabThick),-web.hTopLeftFillet);
   p[5] = p[4] + TFPoint(-web.bTopLeftFillet,web.hTopLeftFillet);
   p[6] = p[0] + TFPoint(-web.hBotLeftFillet*web.lTaper/(boxHeight-botSlabThick),web.hBotLeftFillet);
   p[7] = p[6] + TFPoint(-web.bBotLeftFillet,-web.hBotLeftFillet);
   p[8] = p[2] + TFPoint(-web.hTopRightFillet*web.rTaper/(boxHeight-botSlabThick),-web.hTopRightFillet);
   p[9] = p[8] + TFPoint(web.bTopRightFillet,web.hTopRightFillet);
   p[10] = p[3] + TFPoint(web.hBotRightFillet*web.rTaper/(boxHeight-botSlabThick),web.hBotRightFillet);
   p[11] = p[10] + TFPoint(web.bBotRightFillet,-web.hBotRightFillet);

   TPolyShape *shape;
   // main web
   shape = new TPolyShape(material,TFPoint(0,0),0);
   shape->AddPoint(p[0]);
   shape->AddPoint(p[1]);
   shape->AddPoint(p[2]);
   shape->AddPoint(p[3]);
   AddShape(shape);

   // top left fillet
   shape = new TPolyShape(material->Clone(),TFPoint(0,0),0);
   shape->AddPoint(p[4]);
   shape->AddPoint(p[1]);
   shape->AddPoint(p[5]);
   AddShape(shape);

   // bottom left fillet
   shape = new TPolyShape(material->Clone(),TFPoint(0,0),0);
   shape->AddPoint(p[0]);
   shape->AddPoint(p[6]);
   shape->AddPoint(p[7]);
   AddShape(shape);

   // top right fillet
   shape = new TPolyShape(material->Clone(),TFPoint(0,0),0);
   shape->AddPoint(p[8]);
   shape->AddPoint(p[2]);
   shape->AddPoint(p[9]);
   AddShape(shape);

   // bottom right fillet
   shape = new TPolyShape(material->Clone(),TFPoint(0,0),0);
   shape->AddPoint(p[3]);
   shape->AddPoint(p[10]);
   shape->AddPoint(p[11]);
   AddShape(shape);
}

void TBoxGirder::ComputeLocalProperties()
{
   TSection::ComputeLocalProperties();

   double A;
   TCentroid CG;

   double n = topSlabShape->GetModE()/baseShape->GetModE();
   if (RoundToNearestN())
      RoundN(n);

   A = topSlabShape->GetArea();
   CG = topSlabShape->GetCentroid();
   Qtop = n*A*(CG.Ybar - cg.Ybar);

   A = botSlabShape->GetArea();
   CG = botSlabShape->GetCentroid();
   Qbot = A*(CG.Ybar - cg.Ybar);
}

double TBoxGirder::GetQts()
{
   return Qtop;
}

double TBoxGirder::GetQbs()
{
   return Qbot;
}

