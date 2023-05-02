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

#if !defined(__unitctrl_h)              // Sentry, use file only if it's not already included.
#define __unitctrl_h


#include <owl\owlpch.h>
#pragma hdrstop

#include "owltools.h"
#include "units.h"
#include "nbredit.h"

class _OWLCLASS TStaticUnit : public TStatic
{
// Data Members
// Constructors/Destructor
public:
   TStaticUnit(TWindow *parent,int resourceId);

// Methods
public:
   virtual void SetupWindow() = 0;
};

class _OWLCLASS TStaticForceUnit : public TStaticUnit
{
// Data Members
protected:
   TForceUnit unit;

// Constructors/Destructor
public:
   TStaticForceUnit(TWindow *parent,int resourceId,TForceUnit fu = lbs);

// Methods
public:
   void SetUnit(TForceUnit fu);
   void SetupWindow();
   uint Transfer (void* buffer, TTransferDirection direction);
};

class _OWLCLASS TStaticLengthUnit : public TStaticUnit
{
// Data Members
protected:
   TLengthUnit unit;

// Constructors/Destructor
public:
   TStaticLengthUnit(TWindow *parent,int resourceId,TLengthUnit lu = inch);

// Methods
public:
   void SetUnit(TLengthUnit lu);
   void SetupWindow();
   uint Transfer (void* buffer, TTransferDirection direction);
};

class _OWLCLASS TStaticPressureUnit : public TStaticUnit
{
// Data Members
protected:
   TPressureUnit unit;

// Constructors/Destructor
public:
   TStaticPressureUnit(TWindow *parent,int resourceId,TPressureUnit pu = psi);

// Methods
public:
   void SetUnit(TPressureUnit pu);
   void SetupWindow();
   uint Transfer (void* buffer, TTransferDirection direction);
};

class _OWLCLASS TStaticDensityUnit : public TStaticUnit
{
// Data Members
protected:
   TForceDensityUnit unit;

// Constructors/Destructor
public:
   TStaticDensityUnit(TWindow *parent,int resourceId,TForceDensityUnit du = pcf);

// Methods
public:
   void SetUnit(TForceDensityUnit du);
   void SetupWindow();
   uint Transfer (void* buffer, TTransferDirection direction);
};

class _OWLCLASS TStaticMomentUnit : public TStaticUnit
{
// Data Members
protected:
   TMomentUnit unit;

// Constructors/Destructor
public:
   TStaticMomentUnit(TWindow *parent,int resourceId,TMomentUnit mu = ftkips);

// Methods
public:
   void SetUnit(TMomentUnit mu);
   void SetupWindow();
   uint Transfer (void* buffer, TTransferDirection direction);
};

class _OWLCLASS TUnitEdit : public TNumberEdit
{
public:
    TUnitEdit (TWindow* parent, int resourceID,uint Width,uint Precision, TModule* module = 0);
   ~TUnitEdit();

protected:
    virtual void EvRButtonDown (uint modKeys, TPoint& point) = 0;
    virtual void CmUnit(WPARAM wp) = 0;
};

class _OWLCLASS TForceEdit : public TUnitEdit
{
protected:
   TPopupMenu forceMenu;
   TStaticForceUnit *staticUnit;

public:
    TForceEdit (TWindow* parent, int resourceID,uint Width,uint Precision,TForceUnit fu = lbs, TModule* module = 0);
   ~TForceEdit();

protected:
    void EvRButtonDown (uint modKeys, TPoint& point);
    void CmUnit(WPARAM wp);

DECLARE_RESPONSE_TABLE(TForceEdit);
};

class _OWLCLASS TLengthEdit : public TUnitEdit
{
protected:
   TPopupMenu lengthMenu;
   TStaticLengthUnit *staticUnit;

public:
    TLengthEdit (TWindow* parent, int resourceID,uint Width,uint Precision,TLengthUnit lu = inch, TModule* module = 0);
   ~TLengthEdit();

protected:
    void EvRButtonDown (uint modKeys, TPoint& point);
    void CmUnit(WPARAM wp);

DECLARE_RESPONSE_TABLE(TLengthEdit);
};

class _OWLCLASS TPressureEdit : public TUnitEdit
{
protected:
   TPopupMenu pressureMenu;
   TStaticPressureUnit *staticUnit;

public:
    TPressureEdit (TWindow* parent, int resourceID,uint Width,uint Precision,TPressureUnit pu = psi, TModule* module = 0);
   ~TPressureEdit();

protected:
    void EvRButtonDown (uint modKeys, TPoint& point);
    void CmUnit(WPARAM wp);

DECLARE_RESPONSE_TABLE(TPressureEdit);
};

class _OWLCLASS TDensityEdit : public TUnitEdit
{
protected:
   TPopupMenu densityMenu;
   TStaticDensityUnit *staticUnit;

public:
    TDensityEdit (TWindow* parent, int resourceID,uint Width,uint Precision,TForceDensityUnit du = pcf, TModule* module = 0);
   ~TDensityEdit();

protected:
    void EvRButtonDown (uint modKeys, TPoint& point);
    void CmUnit(WPARAM wp);

DECLARE_RESPONSE_TABLE(TDensityEdit);
};

class _OWLCLASS TMomentEdit : public TUnitEdit
{
protected:
   TPopupMenu momentMenu;
   TStaticMomentUnit *staticUnit;

public:
    TMomentEdit (TWindow* parent, int resourceID,uint Width,uint Precision,TMomentUnit mu = ftkips, TModule* module = 0);
   ~TMomentEdit();

protected:
    void EvRButtonDown (uint modKeys, TPoint& point);
    void CmUnit(WPARAM wp);

DECLARE_RESPONSE_TABLE(TMomentEdit);
};

class _OWLCLASS TIntEdit : public TEdit
{
public:
    TIntEdit (TWindow* parent, int resourceID,bool restrictToPositive = false, TModule* module = 0);
    virtual ~TIntEdit ();

public:
    virtual void SetupWindow ();
    virtual uint Transfer (void* buffer, TTransferDirection direction);

protected:
   bool restrict;
};

#endif                                      // __unitctrl_h sentry.

