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
#include <fstream.h>
#include <string.h>
#include "aisc.hpp"

class TWSHAPE
   {
   public:
   char name[8];
   double ND,WEIGHT,AX,AY,AZ,YD,WBTK,
          ZD,FLTK,BF2TF,DTW,RT,YDAFL,IZ,
          SZ,SZS,RZ,IY,SY,SYS,RY,
          ZZ,ZY,IX,CW,INTYD,YC,ZC,
          EY,EZ,GRPNUM,SHAPE;

   friend istream& operator >> (istream &s,TWSHAPE &w);
   };

istream& operator >> (istream &s,TWSHAPE &w)
{
   s >> w.name >> w.ND >> w.WEIGHT >> w.AX >> w.AY >> w.AZ >> w.YD >> w.WBTK
     >> w.ZD >> w.FLTK >> w.BF2TF >> w.DTW >> w.RT >> w.YDAFL >> w.IZ
     >> w.SZ >> w.SZS >> w.RZ >> w.IY >> w.SY >> w.SYS >> w.RY
     >> w.ZZ >> w.ZY >> w.IX >> w.CW >> w.INTYD >> w.YC >> w.ZC
     >> w.EY >> w.EZ >> w.GRPNUM >> w.SHAPE;
   return s;
}


void main(void)
{
   ifstream ifile("wshapes9.tbl");
   TWSHAPE w;

   while (!ifile.eof())
      {
      ifile >> w;
      if (strncmp("W44X248",w.name,7) == 0)
         {
         cout << "W44X248 found" << endl;
         break;
         }
      }
}