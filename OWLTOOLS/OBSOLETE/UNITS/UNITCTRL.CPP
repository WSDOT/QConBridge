///////////////////////////////////////////////////////////////////////
// OWLTools - Extension Library for the Borland OWL Framework
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
// May 1997 - Created, Richard Brice
// Oct 1999 - Released as open source

#include <strstrea.h>
#include <iomanip.h>
#include "unitctrl.h"

#define BUFSIZE 30

#define CM_LBS    101
#define CM_KIPS   102
#define CM_N      103
#define CM_KN     104

#define CM_INCH 101
#define CM_FEET 102
#define CM_MILLIMETER 103
#define CM_CENTIMETER 104
#define CM_METER 105

#define CM_PSI 101
#define CM_PSF 102
#define CM_KSI 103
#define CM_KSF 104
#define CM_KPA 105
#define CM_PA  106
#define CM_MPA 107

#define CM_PCI 101
#define CM_PCF 102
#define CM_KCI 103
#define CM_KCF 104
#define CM_NCM 105
#define CM_KNCM 106

#define CM_INLBS 101
#define CM_INKIP 102
#define CM_FTLBS 103
#define CM_FTKIP 104
#define CM_NM    105
#define CM_KNM   106

DEFINE_RESPONSE_TABLE1(TForceEdit,TUnitEdit)
    EV_WM_RBUTTONDOWN,
    EV_COMMAND_AND_ID(CM_LBS,CmUnit),
    EV_COMMAND_AND_ID(CM_KIPS,CmUnit),
    EV_COMMAND_AND_ID(CM_N,CmUnit),
    EV_COMMAND_AND_ID(CM_KN,CmUnit),
END_RESPONSE_TABLE;

DEFINE_RESPONSE_TABLE1(TLengthEdit,TUnitEdit)
    EV_WM_RBUTTONDOWN,
    EV_COMMAND_AND_ID(CM_INCH,CmUnit),
    EV_COMMAND_AND_ID(CM_FEET,CmUnit),
    EV_COMMAND_AND_ID(CM_MILLIMETER,CmUnit),
    EV_COMMAND_AND_ID(CM_CENTIMETER,CmUnit),
    EV_COMMAND_AND_ID(CM_METER,CmUnit),
END_RESPONSE_TABLE;

DEFINE_RESPONSE_TABLE1(TPressureEdit,TUnitEdit)
    EV_WM_RBUTTONDOWN,
    EV_COMMAND_AND_ID(CM_PSI,CmUnit),
    EV_COMMAND_AND_ID(CM_PSF,CmUnit),
    EV_COMMAND_AND_ID(CM_KSI,CmUnit),
    EV_COMMAND_AND_ID(CM_KSF,CmUnit),
    EV_COMMAND_AND_ID(CM_KPA,CmUnit),
    EV_COMMAND_AND_ID(CM_PA,CmUnit),
    EV_COMMAND_AND_ID(CM_MPA,CmUnit),
END_RESPONSE_TABLE;

DEFINE_RESPONSE_TABLE1(TDensityEdit,TUnitEdit)
    EV_WM_RBUTTONDOWN,
    EV_COMMAND_AND_ID(CM_PCI,CmUnit),
    EV_COMMAND_AND_ID(CM_PCF,CmUnit),
    EV_COMMAND_AND_ID(CM_KCI,CmUnit),
    EV_COMMAND_AND_ID(CM_KCF,CmUnit),
    EV_COMMAND_AND_ID(CM_NCM,CmUnit),
    EV_COMMAND_AND_ID(CM_KNCM,CmUnit),
END_RESPONSE_TABLE;

DEFINE_RESPONSE_TABLE1(TMomentEdit,TUnitEdit)
    EV_WM_RBUTTONDOWN,
    EV_COMMAND_AND_ID(CM_INLBS,CmUnit),
    EV_COMMAND_AND_ID(CM_INKIP,CmUnit),
    EV_COMMAND_AND_ID(CM_FTLBS,CmUnit),
    EV_COMMAND_AND_ID(CM_FTKIP,CmUnit),
    EV_COMMAND_AND_ID(CM_NM,CmUnit),
    EV_COMMAND_AND_ID(CM_KNM,CmUnit),
END_RESPONSE_TABLE;

TStaticUnit::TStaticUnit(TWindow *parent,int resourceId) :
TStatic(parent,resourceId,BUFSIZE)
{
   EnableTransfer();
}

void TStaticUnit::SetupWindow()
{
   TStatic::SetupWindow();
}

//
//
//
TStaticForceUnit::TStaticForceUnit(TWindow *parent,int resourceId,TForceUnit fu) :
TStaticUnit(parent,resourceId)
{
   unit = fu;
}

void TStaticForceUnit::SetupWindow()
{
   TStaticUnit::SetupWindow();
   SetUnit(unit);
}

void TStaticForceUnit::SetUnit(TForceUnit fu)
{
   unit = fu;
   switch (fu)
      {
      case lbs:
           SetText("lbs");
           break;
      case kips:
           SetText("kips");
           break;
      case N:
           SetText("N");
           break;
      case kN:
           SetText("kN");
           break;
      };
}

uint TStaticForceUnit::Transfer (void* buffer, TTransferDirection direction)
{
   uint result;
   result = TStaticUnit::Transfer(buffer,direction);

   TForceUnit fu;
   if (direction == tdSetData)
      {
      fu = (*(TForceUnit *)buffer);
      SetUnit(fu);
      }
   else if (direction == tdGetData)
      {
      *(TForceUnit *)buffer = unit;
      }

   result = sizeof(TForceUnit);

   return result;
}

//
//
//
TStaticLengthUnit::TStaticLengthUnit(TWindow *parent,int resourceId,TLengthUnit lu) :
TStaticUnit(parent,resourceId)
{
   unit = lu;
}

void TStaticLengthUnit::SetupWindow()
{
   TStaticUnit::SetupWindow();
   SetUnit(unit);
}

void TStaticLengthUnit::SetUnit(TLengthUnit lu)
{
   unit = lu;
   switch (lu)
      {
      case inch:
           SetText("inch");
           break;
      case feet:
           SetText("feet");
           break;
      case mm:
           SetText("mm");
           break;
      case cm:
           SetText("cm");
           break;
      case m:
           SetText("m");
           break;
      };
}

uint TStaticLengthUnit::Transfer (void* buffer, TTransferDirection direction)
{
   uint result;
   result = TStaticUnit::Transfer(buffer,direction);

   TLengthUnit lu;
   if (direction == tdSetData)
      {
      lu = (*(TLengthUnit *)buffer);
      SetUnit(lu);
      }
   else if (direction == tdGetData)
      {
      *(TLengthUnit *)buffer = unit;
      }

   result = sizeof(TLengthUnit);

   return result;
}

//
//
//
TStaticPressureUnit::TStaticPressureUnit(TWindow *parent,int resourceId,TPressureUnit pu) :
TStaticUnit(parent,resourceId)
{
   unit = pu;
}

void TStaticPressureUnit::SetupWindow()
{
   TStaticUnit::SetupWindow();
   SetUnit(unit);
}

void TStaticPressureUnit::SetUnit(TPressureUnit pu)
{
   unit = pu;
   switch (pu)
      {
      case psi:
           SetText("psi");
           break;
      case psf:
           SetText("psf");
           break;
      case ksi:
           SetText("ksi");
           break;
      case ksf:
           SetText("ksf");
           break;
      case Pa:
           SetText("Pa");
           break;
      case kPa:
           SetText("kPa");
           break;
      case MPa:
           SetText("MPa");
           break;
      };
}

uint TStaticPressureUnit::Transfer (void* buffer, TTransferDirection direction)
{
   uint result;
   result = TStaticUnit::Transfer(buffer,direction);

   TPressureUnit pu;
   if (direction == tdSetData)
      {
      pu = (*(TPressureUnit *)buffer);
      SetUnit(pu);
      }
   else if (direction == tdGetData)
      {
      *(TPressureUnit *)buffer = unit;
      }

   result = sizeof(TPressureUnit);

   return result;
}

//
//
//
TStaticDensityUnit::TStaticDensityUnit(TWindow *parent,int resourceId,TForceDensityUnit du) :
TStaticUnit(parent,resourceId)
{
   unit = du;
}

void TStaticDensityUnit::SetupWindow()
{
   TStaticUnit::SetupWindow();
   SetUnit(unit);
}

void TStaticDensityUnit::SetUnit(TForceDensityUnit du)
{
   unit = du;
   switch (du)
      {
      case pci:
           SetText("pci");
           break;
      case pcf:
           SetText("pcf");
           break;
      case kci:
           SetText("kci");
           break;
      case kcf:
           SetText("kcf");
           break;
      case Ncm:
           SetText("N/m3");
           break;
      case kNcm:
           SetText("kN/m3");
           break;
      };
}

uint TStaticDensityUnit::Transfer (void* buffer, TTransferDirection direction)
{
   uint result;
   result = TStaticUnit::Transfer(buffer,direction);

   TForceDensityUnit du;
   if (direction == tdSetData)
      {
      du = (*(TForceDensityUnit *)buffer);
      SetUnit(du);
      }
   else if (direction == tdGetData)
      {
      *(TForceDensityUnit *)buffer = unit;
      }

   result = sizeof(TForceDensityUnit);

   return result;
}

//
//
//
TStaticMomentUnit::TStaticMomentUnit(TWindow *parent,int resourceId,TMomentUnit mu) :
TStaticUnit(parent,resourceId)
{
   unit = mu;
}

void TStaticMomentUnit::SetupWindow()
{
   TStaticUnit::SetupWindow();
   SetUnit(unit);
}

void TStaticMomentUnit::SetUnit(TMomentUnit mu)
{
   unit = mu;
   switch (mu)
      {
      case inlbs:
           SetText("in-lbs");
           break;
      case ftlbs:
           SetText("ft-lbs");
           break;
      case inkips:
           SetText("in-kips");
           break;
      case ftkips:
           SetText("ft-kips");
           break;
      case Nm:
           SetText("N-m");
           break;
      case kNm:
           SetText("kN-m");
           break;
      };
}

uint TStaticMomentUnit::Transfer (void* buffer, TTransferDirection direction)
{
   uint result;
   result = TStaticUnit::Transfer(buffer,direction);

   TMomentUnit mu;
   if (direction == tdSetData)
      {
      mu = (*(TMomentUnit *)buffer);
      SetUnit(mu);
      }
   else if (direction == tdGetData)
      {
      *(TMomentUnit *)buffer = unit;
      }

   result = sizeof(TMomentUnit);

   return result;
}

//
//
//
TUnitEdit::TUnitEdit (TWindow* parent, int resourceID,uint Width,uint Precision,TModule* module) :
TNumberEdit (parent,resourceID,Width,Precision,module)
{
}

TUnitEdit::~TUnitEdit()
{
}

//
//
//
TForceEdit::TForceEdit (TWindow* parent, int resourceID,uint Width,uint Precision,TForceUnit fu, TModule* module):
TUnitEdit (parent,resourceID,Width,Precision,module)
{
   staticUnit = new TStaticForceUnit(parent,resourceID+1,fu);

   forceMenu.AppendMenu(MF_BYCOMMAND,CM_N,"N");
   forceMenu.AppendMenu(MF_BYCOMMAND,CM_KN,"kN");
   forceMenu.AppendMenu(MF_BYPOSITION | MF_SEPARATOR,2);
   forceMenu.AppendMenu(MF_BYCOMMAND,CM_LBS,"lbs");
   forceMenu.AppendMenu(MF_BYCOMMAND,CM_KIPS,"kips");
}

TForceEdit::~TForceEdit()
{
}

void TForceEdit::EvRButtonDown (uint modKeys, TPoint& point)
{
    TEdit::EvRButtonDown(modKeys, point);

    // INSERT>> Your code here.

    TPoint screen = point;
    ClientToScreen(screen);
    forceMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,screen,0,HWindow);
}

void TForceEdit::CmUnit(WPARAM wp)
{
   TForceUnit fu;

   switch (wp)
      {
      case CM_LBS:
           fu = lbs;
           break;
      case CM_KIPS:
           fu = kips;
           break;
      case CM_N:
           fu = N;
           break;
      case CM_KN:
           fu = kN;
           break;
      };

   staticUnit->SetUnit(fu);
}

//
//
//
TLengthEdit::TLengthEdit (TWindow* parent, int resourceID,uint Width,uint Precision,TLengthUnit lu, TModule* module):
TUnitEdit (parent,resourceID,Width,Precision,module)
{
   staticUnit = new TStaticLengthUnit(parent,resourceID+1,lu);

   lengthMenu.AppendMenu(MF_BYCOMMAND,CM_MILLIMETER,"millimeter");
   lengthMenu.AppendMenu(MF_BYCOMMAND,CM_CENTIMETER,"centimeter");
   lengthMenu.AppendMenu(MF_BYCOMMAND,CM_METER,"meter");
   lengthMenu.AppendMenu(MF_BYPOSITION | MF_SEPARATOR,2);
   lengthMenu.AppendMenu(MF_BYCOMMAND,CM_INCH,"inch");
   lengthMenu.AppendMenu(MF_BYCOMMAND,CM_FEET,"feet");
}

TLengthEdit::~TLengthEdit()
{
}

void TLengthEdit::EvRButtonDown (uint modKeys, TPoint& point)
{
    TEdit::EvRButtonDown(modKeys, point);

    // INSERT>> Your code here.

    TPoint screen = point;
    ClientToScreen(screen);
    lengthMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,screen,0,HWindow);
}

void TLengthEdit::CmUnit(WPARAM wp)
{
   TLengthUnit lu;

   switch (wp)
      {
      case CM_INCH:
           lu = inch;
           break;
      case CM_FEET:
           lu = feet;
           break;
      case CM_MILLIMETER:
           lu = mm;
           break;
      case CM_CENTIMETER:
           lu = cm;
           break;
      case CM_METER:
           lu = m;
           break;
      };

   staticUnit->SetUnit(lu);
}

//
//
//
TPressureEdit::TPressureEdit (TWindow* parent, int resourceID,uint Width,uint Precision,TPressureUnit pu, TModule* module):
TUnitEdit (parent,resourceID,Width,Precision,module)
{
   staticUnit = new TStaticPressureUnit(parent,resourceID+1,pu);

   pressureMenu.AppendMenu(MF_BYCOMMAND,CM_PA,"Pa");
   pressureMenu.AppendMenu(MF_BYCOMMAND,CM_KPA,"kPa");
   pressureMenu.AppendMenu(MF_BYCOMMAND,CM_MPA,"MPa");
   pressureMenu.AppendMenu(MF_BYPOSITION | MF_SEPARATOR,4);
   pressureMenu.AppendMenu(MF_BYCOMMAND,CM_PSI,"psi");
   pressureMenu.AppendMenu(MF_BYCOMMAND,CM_PSF,"psf");
   pressureMenu.AppendMenu(MF_BYCOMMAND,CM_KSI,"ksi");
   pressureMenu.AppendMenu(MF_BYCOMMAND,CM_KSF,"ksf");
}

TPressureEdit::~TPressureEdit()
{
}

void TPressureEdit::EvRButtonDown (uint modKeys, TPoint& point)
{
    TEdit::EvRButtonDown(modKeys, point);

    // INSERT>> Your code here.

    TPoint screen = point;
    ClientToScreen(screen);
    pressureMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,screen,0,HWindow);
}

void TPressureEdit::CmUnit(WPARAM wp)
{
   TPressureUnit pu;

   switch (wp)
      {
      case CM_PSI:
           pu = psi;
           break;
      case CM_PSF:
           pu = psf;
           break;
      case CM_KSI:
           pu = ksi;
           break;
      case CM_KSF:
           pu = ksf;
           break;
      case CM_PA:
           pu = Pa;
           break;
      case CM_MPA:
           pu = kPa;
           break;
      };

   staticUnit->SetUnit(pu);
}

//
//
//
TDensityEdit::TDensityEdit (TWindow* parent, int resourceID,uint Width,uint Precision,TForceDensityUnit du, TModule* module):
TUnitEdit (parent,resourceID,Width,Precision,module)
{
   staticUnit = new TStaticDensityUnit(parent,resourceID+1,du);

   densityMenu.AppendMenu(MF_BYCOMMAND,CM_NCM,"N/m^3");
   densityMenu.AppendMenu(MF_BYCOMMAND,CM_KNCM,"kN/m^3");
   densityMenu.AppendMenu(MF_BYPOSITION | MF_SEPARATOR,4);
   densityMenu.AppendMenu(MF_BYCOMMAND,CM_PCI,"pci");
   densityMenu.AppendMenu(MF_BYCOMMAND,CM_PCF,"pcf");
   densityMenu.AppendMenu(MF_BYCOMMAND,CM_KCI,"kci");
   densityMenu.AppendMenu(MF_BYCOMMAND,CM_KCF,"kcf");
}

TDensityEdit::~TDensityEdit()
{
}

void TDensityEdit::EvRButtonDown (uint modKeys, TPoint& point)
{
    TEdit::EvRButtonDown(modKeys, point);

    // INSERT>> Your code here.

    TPoint screen = point;
    ClientToScreen(screen);
    densityMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,screen,0,HWindow);
}

void TDensityEdit::CmUnit(WPARAM wp)
{
   TForceDensityUnit du;

   switch (wp)
      {
      case CM_PCI:
           du = pci;
           break;
      case CM_PCF:
           du = pcf;
           break;
      case CM_KCI:
           du = kci;
           break;
      case CM_KCF:
           du = kcf;
           break;
      case CM_NCM:
           du = Ncm;
           break;
      case CM_KNCM:
           du = kNcm;
           break;
      };

   staticUnit->SetUnit(du);
}


//
//
//
TMomentEdit::TMomentEdit (TWindow* parent, int resourceID,uint Width,uint Precision,TMomentUnit mu, TModule* module):
TUnitEdit (parent,resourceID,Width,Precision,module)
{
   staticUnit = new TStaticMomentUnit(parent,resourceID+1,mu);

   momentMenu.AppendMenu(MF_BYCOMMAND,CM_NM,"N-m");
   momentMenu.AppendMenu(MF_BYCOMMAND,CM_KNM,"kN-m");
   momentMenu.AppendMenu(MF_BYPOSITION | MF_SEPARATOR,4);
   momentMenu.AppendMenu(MF_BYCOMMAND,CM_INLBS,"in-lbs");
   momentMenu.AppendMenu(MF_BYCOMMAND,CM_INKIP,"in-kips");
   momentMenu.AppendMenu(MF_BYCOMMAND,CM_FTLBS,"ft-lbs");
   momentMenu.AppendMenu(MF_BYCOMMAND,CM_FTKIP,"ft-kips");
}

TMomentEdit::~TMomentEdit()
{
}

void TMomentEdit::EvRButtonDown (uint modKeys, TPoint& point)
{
    TEdit::EvRButtonDown(modKeys, point);

    // INSERT>> Your code here.

    TPoint screen = point;
    ClientToScreen(screen);
    momentMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,screen,0,HWindow);
}

void TMomentEdit::CmUnit(WPARAM wp)
{
   TMomentUnit mu;

   switch (wp)
      {
      case CM_INLBS:
           mu = inlbs;
           break;
      case CM_INKIP:
           mu = inkips;
           break;
      case CM_FTLBS:
           mu = ftlbs;
           break;
      case CM_FTKIP:
           mu = ftkips;
           break;
      case CM_NM:
           mu = Nm;
           break;
      case CM_KNM:
           mu = kNm;
           break;
      };

   staticUnit->SetUnit(mu);
}

