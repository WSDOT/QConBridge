///////////////////////////////////////////////////////////////////////
// QConBridge - HL93 Live Load Analysis
// Copyright (C) 1999  Washington State Department of Transportation
//                     Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation, 
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but 
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source 
// License along with this program; if not, write to the Washington 
// State Department of Transportation, Bridge and Structures Office, 
// 4500 3rd AVE SE - P.O. Box  47340, Olympia, WA 98503, USA or e-mail 
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Revision Log
// ============
// May 1997 - Created, Richard Brice, PE
// Oct 1999 - Released as Open Source

#if !defined READER_H_
#define READER_H_

/**************************************************************
   Bridge Analyzer Version 1.0

   Copyright (c) 1995 by WSDOT
   All Rights Reserved.

   Written By: Richard Brice, PE
**************************************************************/

#include <iostream.h>
#include <fstream.h>
#include <owl\docview.h>
#include <qconbrdg\bdoc.h>


class FileHeader
{
public:
   FileHeader(const char *appName,int appMajorVer,int appMinorVer,int formatId);
   void ReadHeader(ifstream &ifile);
   void WriteHeader(ofstream &ofile);
   virtual bool IsValid();

protected:
   // Required
   string AppName;
   int    AppMajorVer;
   int    AppMinorVer;
   int    FormatID;

   // Read from file
   string appName;
   int    appMajorVer;
   int    appMinorVer;
   int    formatId;
};

class QConBridgeFile
{
public:
   QConBridgeFile(TBridgeDocument &b);
   virtual void Read(ifstream &ifile);
   virtual void Write(ofstream &ofile);

protected:
   TBridgeDocument &bdoc;
   LRFDBridge &bridge;

   LRFDSpan *ReadSpan(ifstream &ifile);
   Support *ReadSupport(ifstream &ifile);
   void ReadLoad(ifstream &ifile,BridgeLoad **load,int &loadType);
   void WriteSpan(ofstream &ofile,LRFDSpan *span);
   void WriteSupport(ofstream &ofile,Support *support);
   void WriteLoad(ofstream &ofile,BridgeLoad *load);
};


#endif // READER_H_