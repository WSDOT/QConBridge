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

#include <iostream.h>
#include <iomanip.h>
#include <fem2d\femmodel.h>
#include <material\genmat.h>
#include <shape\genshape.h>

void main(void)
{
   FemModel model;
   double Ax = 0.875369;
   double Iz = 0.2597;
   double E  = 3.876e15;
   double density = 3003.176*9.81;
   int lcase;
   int jnt;
   int mbr;
   int poi;
   double dx1,dy1,rz1,dx2,dy2,rz2;
   double fx1,fy1,mz1,fx2,fy2,mz2;
   int i,j;
   int nl = 20;
   int nMbrs = 1;
   Material *material;
   TShape* section;

   cout << "Fem2d Tester" << endl;

   model.SetEquilibriumCheckTolerance(1.e-10);

   /*
   model.AddJoint(new Joint(0,0,0,false));
   model.AddJoint(new Joint(1,30,0,false));
   model.AddJoint(new Joint(2,30,-10,false));
   model.AddJoint(new Joint(3,80,0,false));
   model.AddJoint(new Joint(4,80,-10,false));
   model.AddJoint(new Joint(5,110,0,false));

   model.GetJoint(0)->Support();
   model.GetJoint(2)->Support();
   model.GetJoint(4)->Support();
   model.GetJoint(5)->Support();

   // Roller
   model.GetJoint(0)->Release(jntRelFx | jntRelMz);
   model.GetJoint(5)->Release(jntRelFx | jntRelMz);

   // Setup members
   material = new Material(E,density);
   section = new Section(0,0,Ax,0,Iz,0);
   model.AddMember(new PlnFrameMbr(0,model.GetJoint(0),model.GetJoint(1),material,section));
   model.AddMember(new PlnFrameMbr(1,model.GetJoint(1),model.GetJoint(2),material,section));
   model.AddMember(new PlnFrameMbr(2,model.GetJoint(1),model.GetJoint(3),material,section));
   model.AddMember(new PlnFrameMbr(3,model.GetJoint(3),model.GetJoint(4),material,section));
   model.AddMember(new PlnFrameMbr(4,model.GetJoint(3),model.GetJoint(5),material,section));

   // Moment release at end of member 2
   model.GetMember(2)->Release(mtFixPin);
*/
   model.AddJoint(new Joint(0,0,0,false));
   model.AddJoint(new Joint(1,30.480,0,false));
   model.AddJoint(new Joint(2,60.960,0,false));
   model.GetJoint(0)->Support();
   model.GetJoint(1)->Support();
   model.GetJoint(2)->Support();
   model.GetJoint(0)->Release(jntRelFx | jntRelMz); // roller
   model.GetJoint(1)->Release(jntRelMz);            // pin
   model.GetJoint(2)->Release(jntRelFx | jntRelMz); // roller
   material = new GeneralMaterial(E,density);
   section = new GeneralShape(Ax,0,0,Iz,0,0,0,0,material);
   model.AddMember(new PlnFrameMbr(0,model.GetJoint(0),model.GetJoint(1),section->Clone()));
   model.AddMember(new PlnFrameMbr(1,model.GetJoint(1),model.GetJoint(2),section->Clone()));

//   model.AddLoading(new Loading(0));
   model.AddLoading(new DeadLoading(0)); // includes self weight
   model.GetLoading(0)->AddMbrLoad(new ConcentratedLoad(0,0.5,-44482.2,true,ForceY,Global));
   model.GetLoading(0)->AddMbrLoad(new ConcentratedLoad(1,0.5,-44482.2,true,ForceY,Global));

   for (mbr = 0; mbr < nMbrs; mbr++)
      {
      for (poi = 0; poi < 11; poi++)
         {
         double fract = poi/10.;
         model.DefinePOI(poi+11*mbr,mbr,fract,true);
         }
      }

   try
      {
      model.StiffnessAnalysis();
      }
   catch (xmsg xMsg)
      {
      cout << xMsg.why() << endl;
      }

   ofstream osm("moment.dat");
   ofstream osv("shear.dat");
   osm << "Moment - Left Face, Right Face" << endl;
   osv << "Shear  - Left Face, Right Face" << endl;
   for (poi = 0; poi < nMbrs*11; poi++)
      {
      osm << poi << " ";
      osv << poi << " ";
      lcase = 0;
      model.GetPOIForces(poi,lcase,mbrLeftFace,fx1,fy1,mz1);
      model.GetPOIForces(poi,lcase,mbrRightFace,fx2,fy2,mz2);
      osm << setiosflags(ios::showpoint | ios::floatfield) << setw(6) << setprecision(3) << mz1 << " " << setw(6) << setprecision(3) << mz2 << " ";
      osv << setiosflags(ios::showpoint | ios::floatfield) << setw(6) << setprecision(3) << fy1 << " " << setw(6) << setprecision(3) << fy2 << " ";
      osm << endl;
      osv << endl;
      }

   for (i = 0; i < nMbrs; i++)
      {
      model.GetMbrForces(i,0,fx1,fy1,mz1,fx2,fy2,mz2);

      osm << "Load Case " << 0 << " Member " << i << endl;
      osm << "Start Joint" << endl;
      osm << "Fx = " << fx1 << " Fy = " << fy1 << " Mz = " << mz1 << endl;
      osm << "End Joint" << endl;
      osm << "Fx = " << fx2 << " Fy = " << fy2 << " Mz = " << mz2 << endl;
      osm << endl;
      }

   osm.close();
   osv.close();
   model.GTStrudl("model.gtb");

   delete section;
}

