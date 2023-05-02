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



#include <qconbrdg\spandlg.h>
#include <qconbrdg\qcbapp.h>
#include <qconbrdg\gt0valid.h>
#include <qconbrdg\sprpwiz.h>
#include <qconbrdg\common.rh>
#include <bridge\segment.h>
#include <shape\genshape.h>
#include <material\genmat.h>

#include <Context.hh>

#include <checks.h>

#define IDPAGE_PROPERTIES  0
#define IDPAGE_BC          1
#define IDPAGE_HINGE       2
#define IDPAGE_LIMSTATE    3
#define IDPAGE_SUPPORTDATA 4
#define IDPAGE_INSERTIONPT 5

TEditSpanDlgXferBuffer::TEditSpanDlgXferBuffer(SystemOfUnits units)
{
   DerivedUnits::Length lengthConvFactor;

   lengthConvFactor = (units == unitsSI) ? SIConstants::meter : USConstants::feet;
   properties.length = DerivedUnits::Length(1.0*lengthConvFactor).value();

   properties.table.DeleteRow(0);
   properties.table.AppendRow(units == unitsSI);

   bc.leftBC = BF_CHECKED;
   bc.rightBC = BF_CHECKED;

   hinges.numHinges = 0;
   hinges.hinge1Loc = 0;
   hinges.hinge2Loc = 0;

   limitStateFactors.nd_strength = 1;
   limitStateFactors.nr_strength = 1;
   limitStateFactors.ni_strength = 1;
   limitStateFactors.nd_service  = 1;
   limitStateFactors.nr_service  = 1;
   limitStateFactors.ni_service  = 1;
}

TEditSpanDlgXferBuffer::TEditSpanDlgXferBuffer(const LRFDSpan *span)
{
   Segment *segment;
   TShape *sect;
   LLDistFactors *lldf;
   int seg, numSegments;
   int i, nbrHinges;
   Hinge *hinge;

   properties.length = span->Length();

   numSegments = span->NumSegments();
   for (seg = 0; seg < numSegments; seg++)
      {
      // Should be in-sync
      segment = span->GetSegment(seg);
      lldf    = span->GetLLDistFactors(seg);
      sect = segment->GetSection();
      sect->ComputeProperties();

      if (seg != 0)
         properties.table.AppendRow(true); // dummy units here

      properties.table.GetRow(seg).loc = segment->GetLocation();
      properties.table.GetRow(seg).Ax = sect->GetArea();
      properties.table.GetRow(seg).Iz = sect->GetCentroidalInertia().Ixx;
      properties.table.GetRow(seg).ModE = sect->GetModE();
      properties.table.GetRow(seg).unitWeight = sect->GetUnitWeight();

      // Live Load Distribution Factors
      properties.table.GetRow(seg).s_gMoment = lldf->s_gPosMoment;
      properties.table.GetRow(seg).s_gShear = lldf->s_gShear;
      properties.table.GetRow(seg).f_gMoment = lldf->f_gPosMoment;
      properties.table.GetRow(seg).f_gShear = lldf->f_gShear;
      }

   bc.leftBC  = (span->StartContinuity() == ctHinged) ? BF_CHECKED : BF_UNCHECKED;
   bc.rightBC = (span->EndContinuity()   == ctHinged) ? BF_CHECKED : BF_UNCHECKED;

   nbrHinges = span->NumHinges();
   hinges.numHinges = nbrHinges + IDC_HINGE0;
   // Init hinge locations
   hinges.hinge1Loc = 0;
   hinges.hinge2Loc = 0;

   // Assign actual hinge locations
   for (i = 0; i < nbrHinges; i++)
      {
      hinge = span->GetHinge(i);
      if (i == 0)
         hinges.hinge1Loc = hinge->GetLocation();
      else
         hinges.hinge2Loc = hinge->GetLocation();
      }

   // Limit State Factors
   limitStateFactors.nd_strength = span->GetDuctilityFactor(lsStrength);
   limitStateFactors.nr_strength = span->GetRedundancyFactor(lsStrength);
   limitStateFactors.ni_strength = span->GetImportanceFactor(lsStrength);
   limitStateFactors.nd_service = span->GetDuctilityFactor(lsService);
   limitStateFactors.nr_service = span->GetRedundancyFactor(lsService);
   limitStateFactors.ni_service = span->GetImportanceFactor(lsService);
}

LRFDSpan *TEditSpanDlgXferBuffer::GetSpan()
{
   LRFDSpan *span;
   int i, nHinges;
   int row, numRows;
   GeneralShape* newShape;
   double loc,Ax,Iz,ModE,unitWeight;
   double s_gPosMoment,s_gNegMoment,s_gShear;
   double f_gPosMoment,f_gNegMoment,f_gShear;

   loc = properties.table.GetRow(0).loc;
   Ax = properties.table.GetRow(0).Ax;
   Iz = properties.table.GetRow(0).Iz;
   ModE = properties.table.GetRow(0).ModE;
   unitWeight = properties.table.GetRow(0).unitWeight;
   s_gPosMoment = properties.table.GetRow(0).s_gMoment;
   s_gNegMoment = properties.table.GetRow(0).s_gMoment;
   s_gShear     = properties.table.GetRow(0).s_gShear;
   f_gPosMoment = properties.table.GetRow(0).f_gMoment;
   f_gNegMoment = properties.table.GetRow(0).f_gMoment;
   f_gShear     = properties.table.GetRow(0).f_gShear;


   newShape = new GeneralShape(Ax,0,0,Iz,0,0,0,0,new GeneralMaterial(ModE,unitWeight));
   newShape->ComputeProperties();
   span = new LRFDSpan(properties.length,newShape);
   span->AddLLDistFactors(new LLDistFactors(loc,
                                            s_gPosMoment,
                                            s_gNegMoment,
                                            s_gShear,
                                            f_gPosMoment,
                                            f_gNegMoment,
                                            f_gShear));

   numRows = properties.table.NumRows();
   for (row = 1; row < numRows; row++)
      {
      loc = properties.table.GetRow(row).loc;
      Ax = properties.table.GetRow(row).Ax;
      Iz = properties.table.GetRow(row).Iz;
      ModE = properties.table.GetRow(row).ModE;
      unitWeight = properties.table.GetRow(row).unitWeight;
      s_gPosMoment = properties.table.GetRow(row).s_gMoment;
      s_gNegMoment = properties.table.GetRow(row).s_gMoment;
      s_gShear     = properties.table.GetRow(row).s_gShear;
      f_gPosMoment = properties.table.GetRow(row).f_gMoment;
      f_gNegMoment = properties.table.GetRow(row).f_gMoment;
      f_gShear     = properties.table.GetRow(row).f_gShear;

      newShape = new GeneralShape(Ax,0,0,Iz,0,0,0,0,new GeneralMaterial(ModE,unitWeight));
      newShape->ComputeProperties();
      span->AddSegment(new Segment(loc,newShape));
      span->AddLLDistFactors(new LLDistFactors(loc,s_gPosMoment,s_gNegMoment,s_gShear,f_gPosMoment,f_gNegMoment,f_gShear));
      }

   span->StartContinuity((bc.leftBC == BF_UNCHECKED) ? ctContinuous : ctHinged);
   span->EndContinuity((bc.rightBC == BF_UNCHECKED) ? ctContinuous : ctHinged);

   nHinges = NumHinges();
   for (i = 0; i < nHinges; i++)
      {
      double location;

      if (i == 0)
         location = hinges.hinge1Loc;
      else
         location = hinges.hinge2Loc;

      span->AddHinge(new Hinge(location));
      }

   // Limit State Factors
   span->SetDuctilityFactor(limitStateFactors.nd_strength,lsStrength);
   span->SetRedundancyFactor(limitStateFactors.nr_strength,lsStrength);
   span->SetImportanceFactor(limitStateFactors.ni_strength,lsStrength);

   span->SetDuctilityFactor(limitStateFactors.nd_service,lsService);
   span->SetRedundancyFactor(limitStateFactors.nr_service,lsService);
   span->SetImportanceFactor(limitStateFactors.ni_service,lsService);

   return span;
}

int TEditSpanDlgXferBuffer::NumHinges() const
{
   int num;
   switch(hinges.numHinges)
      {
      case IDC_HINGE0: num = 0; break;
      case IDC_HINGE1: num = 1; break;
      case IDC_HINGE2: num = 2; break;
      }
   return num;
}
//
//
//

TAddSpanDlgXferBuffer::TAddSpanDlgXferBuffer(bool sel,SystemOfUnits units) :
TEditSpanDlgXferBuffer(units)
{
   insertionPoint.point = sel ? IDC_CURRENT : IDC_END;
   supportData.simple = BF_CHECKED;
   supportData.height = 0;
   supportData.offset = 0;
   supportData.ax = 0;
   supportData.iz = 0;
   supportData.modE = 0;
   supportData.unitWeight = 0;
   supportData.fixity = IDC_PINNED;
   supportData.hinge = BF_UNCHECKED;
}

TAddSpanDlgXferBuffer::TAddSpanDlgXferBuffer(const LRFDSpan *span) :
TEditSpanDlgXferBuffer(span)
{
   insertionPoint.point = IDC_CURRENT;
   supportData.simple = BF_CHECKED;
   supportData.height = 0;
   supportData.offset = 0;
   supportData.ax = 0;
   supportData.iz = 0;
   supportData.modE = 0;
   supportData.unitWeight = 0;
   supportData.fixity = IDC_PINNED;
   supportData.hinge = BF_UNCHECKED;
}

AddSpanLocation TAddSpanDlgXferBuffer::GetLocation() const
{
   AddSpanLocation l;

   switch(insertionPoint.point)
      {
      case IDC_START:
           l = aslAtStart;
           break;

      case IDC_END:
           l = aslAtEnd;
           break;

      case IDC_CURRENT:
           l = aslAtCurrent;
           break;

      default:
           #pragma warn -ccc
           CHECKX(false,"TAddSpanDlgXferBuffer::GetLocation() - bad location");
           #pragma warn .ccc
      }

   return l;
}

Support *TAddSpanDlgXferBuffer::GetSupport()
{
   Support *support;
   GeneralShape* shape;
   if (supportData.simple == BF_CHECKED)
      {
      support = new Support;
      }
   else
      {
      shape = new GeneralShape(supportData.ax,0,0,supportData.iz,0,0,0,0,
                            new GeneralMaterial(supportData.modE,supportData.unitWeight));
      shape->ComputeProperties();
      support = new Support(supportData.height,supportData.offset,shape);
      support->TopContinuity( (supportData.hinge == BF_CHECKED) ? ctHinged : ctContinuous );
      }

   switch(supportData.fixity)
      {
      case IDC_PINNED:
           support->SupportFixity(fxPinned);
           break;

      case IDC_ROLLER:
           support->SupportFixity(fxRoller);
           break;

      case IDC_FIXED:
           support->SupportFixity(fxFixed);
           break;
      }

   return support;
}

class SpanProperties : public TMyPropertyPage
{
public:
   SpanProperties(TPropertySheet* parent,SystemOfUnits units, TModule *module = 0);
   ~SpanProperties();

   void Help(TNotify far&);

protected:
   SystemOfUnits units;

   TLengthEdit *length;
   SpanTable *spanTable;

   void BNClicked_Calculate();
   void BNClicked_Sort();
   DECLARE_RESPONSE_TABLE(SpanProperties);
   DECLARE_HELPCONTEXT(SpanProperties);
};

DEFINE_RESPONSE_TABLE1(SpanProperties, TMyPropertyPage)
    EV_BN_CLICKED(IDC_SORT,BNClicked_Sort),
    EV_BN_CLICKED(IDC_CALCULATOR, BNClicked_Calculate),
END_RESPONSE_TABLE;

DEFINE_HELPCONTEXT(SpanProperties)
   HCENTRY_CONTROL(IDC_LENGTH,IDC_LENGTH),
   HCENTRY_CONTROL(IDC_AX,IDC_AX),
END_HELPCONTEXT;

SpanProperties::SpanProperties(TPropertySheet* parent,SystemOfUnits units, TModule *module) :
TMyPropertyPage(parent,IDD_SPAN_PROPERTIES,"Properties",0,module),
units(units)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   if (units == unitsSI)
      length = new TLengthEdit(this,IDC_LENGTH,SIConstants::meter,11,3,IDC_LENGTH_TAG,"m");
   else
      length = new TLengthEdit(this,IDC_LENGTH,USConstants::feet,11,3,IDC_LENGTH_TAG,"feet");

   spanTable = new SpanTable(this,units,IDC_TABLE);
   spanTable->SetLengthEdit(length);
}

SpanProperties::~SpanProperties()
{
   delete length;
   delete spanTable;
}

void SpanProperties::BNClicked_Calculate()
{
   TRACE("Entering SpanProperties::BNClicked_Calculate()");
   TSectPropData data;
   data.dispUnits = units;
   data.spanLength = length->GetValue();
   data.effSpanLength = length->GetValue();

   TSectPropWizard wizard(this,data);


   TRACE("Before wizard.Execute()");
   if (wizard.Execute() == IDOK)
      {
      length->SetValue(data.spanLength);

      spanTable->TransferItem(spanTable->GetCurrentLine(),IDC_AX,&data.ax);
      spanTable->TransferItem(spanTable->GetCurrentLine(),IDC_IZ,&data.iz);
      spanTable->TransferItem(spanTable->GetCurrentLine(),IDC_UNITWEIGHT,&data.unitWeight);
      spanTable->TransferItem(spanTable->GetCurrentLine(),IDC_MODE,&data.modE);
      spanTable->TransferItem(spanTable->GetCurrentLine(),IDC_S_GMOMENT,&data.s_gMoment);
      spanTable->TransferItem(spanTable->GetCurrentLine(),IDC_S_GSHEAR,&data.s_gShear);
      spanTable->TransferItem(spanTable->GetCurrentLine(),IDC_F_GMOMENT,&data.f_gMoment);
      spanTable->TransferItem(spanTable->GetCurrentLine(),IDC_F_GSHEAR,&data.f_gShear);
      spanTable->Refresh();
      }
   TRACE("Leaving SpanProperties::BNClicked_Calculate()");
}

void SpanProperties::BNClicked_Sort()
{
    spanTable->Sort();
}

void SpanProperties::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Properties);
}

//
//---------------------------------------------------------------
//

class BoundryConditions : public TMyPropertyPage
{
public:
   BoundryConditions(TPropertySheet* parent,SystemOfUnits, TModule *module = 0);
   ~BoundryConditions();

   void Help(TNotify far&);

protected:
	TCheckBox *left;
   TCheckBox *right;
};

BoundryConditions::BoundryConditions(TPropertySheet* parent,SystemOfUnits,TModule *module) :
TMyPropertyPage(parent,IDD_SPAN_BC,"Boundary Conditions",0,module)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   left = new TCheckBox(this,IDC_LEFTEND);
   right = new TCheckBox(this,IDC_RIGHTEND);
}

BoundryConditions::~BoundryConditions()
{
   delete left;
   delete right;
}

void BoundryConditions::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Boundary_Conditions);
}

//
//---------------------------------------------------------------
//

class Hinges : public TMyPropertyPage
{
public:
   Hinges(TPropertySheet* parent,SystemOfUnits units, TModule *module = 0);
   ~Hinges();

   void SetupWindow();
   void Help(TNotify far&);

protected:
   void EvNumHingesChanged();

   TRadioGroupBox *hinges;
   TLengthEdit *hinge1Loc;
   TLengthEdit *hinge2Loc;

   DECLARE_RESPONSE_TABLE(Hinges);
};

DEFINE_RESPONSE_TABLE1(Hinges, TMyPropertyPage)
    EV_BN_CLICKED(IDC_HINGE0,EvNumHingesChanged),
    EV_BN_CLICKED(IDC_HINGE1,EvNumHingesChanged),
    EV_BN_CLICKED(IDC_HINGE2,EvNumHingesChanged),
END_RESPONSE_TABLE;

Hinges::Hinges(TPropertySheet* parent,SystemOfUnits units, TModule *module) :
TMyPropertyPage(parent,IDD_SPAN_HINGE,"Hinges",0,module)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   LengthCtrlData hingeLoc1Data[] = {LengthCtrlData(IDC_HINGE1LOCATION,SIConstants::meter,11,3,IDC_HINGE1_TAG,"m"),
                                     LengthCtrlData(IDC_HINGE1LOCATION,USConstants::feet,11,3,IDC_HINGE1_TAG,"feet")};
   LengthCtrlData hingeLoc2Data[] = {LengthCtrlData(IDC_HINGE2LOCATION,SIConstants::meter,11,3,IDC_HINGE2_TAG,"m"),
                                     LengthCtrlData(IDC_HINGE2LOCATION,USConstants::feet,11,3,IDC_HINGE2_TAG,"feet")};

   hinges    = new TRadioGroupBox(this,IDC_HINGEGRP,IDC_HINGE0,IDC_HINGE2);
   hinge1Loc = new TLengthEdit(this,DLGCTRLDATA(hingeLoc1Data,units));
   hinge2Loc = new TLengthEdit(this,DLGCTRLDATA(hingeLoc2Data,units));
}

Hinges::~Hinges()
{
   delete hinges;
   delete hinge1Loc;
   delete hinge2Loc;
}

void Hinges::SetupWindow()
{
   TMyPropertyPage::SetupWindow();

   EvNumHingesChanged();
}

void Hinges::EvNumHingesChanged()
{
   bool enable1,enable2;

   switch(hinges->GetSelRadioButton())
      {
      case IDC_HINGE0:
           enable1 = false;
           enable2 = false;
           break;

      case IDC_HINGE1:
           enable1 = true;
           enable2 = false;
           break;

      case IDC_HINGE2:
           enable1 = true;
           enable2 = true;
           break;
      }

   hinge1Loc->EnableWindow(enable1);
   TWindow(GetDlgItem(IDC_HINGE1_TAG)).EnableWindow(enable1);
   hinge2Loc->EnableWindow(enable2);
   TWindow(GetDlgItem(IDC_HINGE2_TAG)).EnableWindow(enable2);
}

void Hinges::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Hinges);
}

//
//---------------------------------------------------------------
//

class LimitStateFactors : public TMyPropertyPage
{
public:
   LimitStateFactors(TPropertySheet* parent,SystemOfUnits units, TModule *module = 0);
   ~LimitStateFactors();

   void Help(TNotify far&);

protected:
   TNumberEdit *nd_strength;
   TNumberEdit *nr_strength;
   TNumberEdit *ni_strength;
   TNumberEdit *nd_service;
   TNumberEdit *nr_service;
   TNumberEdit *ni_service;
};


LimitStateFactors::LimitStateFactors(TPropertySheet* parent,SystemOfUnits , TModule *module) :
TMyPropertyPage(parent,IDD_SPAN_LIMSTATE,"Limit State Factors",0,module)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   nd_strength = new TNumberEdit(this,IDC_ND_STRENGTH,11,3);
   nr_strength = new TNumberEdit(this,IDC_NR_STRENGTH,11,3);
   ni_strength = new TNumberEdit(this,IDC_NI_STRENGTH,11,3);

   nd_service = new TNumberEdit(this,IDC_ND_SERVICE,11,3);
   nr_service = new TNumberEdit(this,IDC_NR_SERVICE,11,3);
   ni_service = new TNumberEdit(this,IDC_NI_SERVICE,11,3);
}

LimitStateFactors::~LimitStateFactors()
{
   delete nd_strength;
   delete nr_strength;
   delete ni_strength;

   delete nd_service;
   delete nr_service;
   delete ni_service;
}

void LimitStateFactors::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Limit_State_Factors);
}

//
//---------------------------------------------------------------
//

class SupportPage : public TMyPropertyPage
{
public:
   SupportPage(TPropertySheet* parent,SystemOfUnits units,TModule *module = 0);
   ~SupportPage();

   void SetupWindow();
   void BNClicked_Simple ();
   void Help(TNotify far&);

protected:
   TCheckBox            *simpleChk;
   TLengthEdit          *height;
   TLengthEdit          *offset;
   TLength2Edit         *ax;
   TLength4Edit         *iz;
   TPressureEdit        *modE;
   TUnitWeightEdit      *unitWeight;
   TMaterialDensityEdit *density;
   TRadioGroupBox       *fixity;
   TCheckBox            *hinge;

   SystemOfUnits units;

   DECLARE_RESPONSE_TABLE(SupportPage);
};

DEFINE_RESPONSE_TABLE1(SupportPage, TMyPropertyPage)
    EV_BN_CLICKED(IDC_SIMPLE, BNClicked_Simple),
END_RESPONSE_TABLE;

SupportPage::SupportPage(TPropertySheet* parent,SystemOfUnits units,TModule *module) :
TMyPropertyPage(parent,IDD_SPAN_SUPPORTDATA,"Support",0,module),
units(units)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   simpleChk = new TCheckBox(this,IDC_SIMPLE);

   if (units == unitsSI)
      {
      height = new TLengthEdit(this,IDC_HEIGHT,SIConstants::meter,11,3,IDC_HEIGHT_TAG,"m");
      height->SetValidator(new TGTZeroValidator("Height","m"));

      offset = new TLengthEdit(this,IDC_OFFSET,SIConstants::meter,11,3,IDC_OFFSET_TAG,"m");

      ax = new TLength2Edit(this,IDC_AX,SIConstants::millimeter2,11,3,IDC_AX_TAG,"mm^2");
      ax->SetValidator(new TGTZeroValidator("Ax","mm^2"));

      iz = new TLength4Edit(this,IDC_IZ,SIConstants::millimeter4,11,3,IDC_IZ_TAG,"mm^4");
      iz->SetValidator(new TGTZeroValidator("Iz","mm^4"));

      modE = new TPressureEdit(this,IDC_MODE,SIConstants::Pascal,11,3,IDC_MODE_TAG,"Pa");
      modE->SetValidator(new TGTZeroValidator("Mod. E","Pa"));

      unitWeight = NULL;
      density = new TMaterialDensityEdit(this,IDC_DENSITY,SIConstants::kilogram_per_meter3,11,3,IDC_DENSITY_TAG,"kg/m^3");
      density->SetValidator(new TGTZeroValidator("Density","kg/m^3"));
      }
   else
      {
      height = new TLengthEdit(this,IDC_HEIGHT,USConstants::feet,11,3,IDC_HEIGHT_TAG,"feet");
      height->SetValidator(new TGTZeroValidator("Height","feet"));

      offset = new TLengthEdit(this,IDC_OFFSET,USConstants::feet,11,3,IDC_OFFSET_TAG,"feet");

      ax = new TLength2Edit(this,IDC_AX,USConstants::inch2,11,3,IDC_AX_TAG,"in^2");
      ax->SetValidator(new TGTZeroValidator("Ax","in^2"));

      iz = new TLength4Edit(this,IDC_IZ,USConstants::inch4,11,3,IDC_IZ_TAG,"in^4");
      iz->SetValidator(new TGTZeroValidator("Iz","in^4"));

      modE = new TPressureEdit(this,IDC_MODE,USConstants::kip_per_inch2,11,3,IDC_MODE_TAG,"ksi");
      modE->SetValidator(new TGTZeroValidator("Mod. E","ksi"));

      density = NULL;
      unitWeight = new TUnitWeightEdit(this,IDC_DENSITY,USConstants::pound_per_feet3,11,3,IDC_DENSITY_TAG,"pcf");
      unitWeight->SetValidator(new TGTZeroValidator("Unit Wgt","pcf"));
      }

   fixity = new TRadioGroupBox(this,IDC_BCGROUP,IDC_PINNED,IDC_FIXED);
   hinge = new TCheckBox(this,IDC_HINGED);
}

SupportPage::~SupportPage()
{
    Destroy();

   delete simpleChk;
   delete height;
   delete offset;
   delete ax;
   delete iz;
   delete modE;
   delete unitWeight;
   delete density;
   delete fixity;
   delete hinge;
}

void SupportPage::SetupWindow ()
{
    TMyPropertyPage::SetupWindow();

    // INSERT>> Your code here.
    SetDlgItemText(IDC_DENSITY_LABEL,(units == unitsSI) ? "Density" : "Unit Wgt" );
    BNClicked_Simple();
}

void SupportPage::BNClicked_Simple ()
{
    // INSERT>> Your code here.
    if (simpleChk->GetCheck() == BF_CHECKED)
       {
       // Disable the fixed option. Make sure it is unchecked.
       // Check Pinned option so that something will be checked
       fixity->GetRadioButton(RGB_BYCOMMAND,IDC_FIXED)->EnableWindow(false);
       fixity->CheckRadioButton(RGB_BYCOMMAND,IDC_PINNED);

       // Disable others
       height->EnableWindow(false);
       offset->EnableWindow(false);
       ax->EnableWindow(false);
       iz->EnableWindow(false);
       modE->EnableWindow(false);
       if (density)
          density->EnableWindow(false);
       if (unitWeight)
          unitWeight->EnableWindow(false);
       hinge->EnableWindow(false);
       }
    else
       {
       // Enable the fixed option.
       fixity->GetRadioButton(RGB_BYCOMMAND,IDC_FIXED)->EnableWindow(true);

       // Enable others
       height->EnableWindow(true);
       offset->EnableWindow(true);
       ax->EnableWindow(true);
       iz->EnableWindow(true);
       modE->EnableWindow(true);
       if (density)
          density->EnableWindow(true);
       if (unitWeight)
          unitWeight->EnableWindow(true);
       hinge->EnableWindow(true);
       }
}

void SupportPage::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Edit_Support);
}

//
//---------------------------------------------------------------
//

class InsertionPoint : public TMyPropertyPage
{
public:
   InsertionPoint(TPropertySheet* parent,SystemOfUnits units, TModule *module = 0);
   ~InsertionPoint();

   void SetupWindow();
   void Help(TNotify far&);

protected:
   TRadioGroupBox *grp;
};

InsertionPoint::InsertionPoint(TPropertySheet* parent,SystemOfUnits , TModule *module) :
TMyPropertyPage(parent,IDD_SPAN_INSERTIONPT,"Insertion Point",0,module)
{
   PageInfo.dwFlags |= PSP_HASHELP;

   grp = new TRadioGroupBox(this,IDC_LOCATIONGRP,IDC_START,IDC_CURRENT);
}

InsertionPoint::~InsertionPoint()
{
   delete grp;
}

void InsertionPoint::SetupWindow()
{
   TMyPropertyPage::SetupWindow();

   // Also see InsertionPoint::TransferData()
   TPropertySheet* sheet = GetSheet();
   TAddSpanDlg *dlg = TYPESAFE_DOWNCAST(sheet,TAddSpanDlg);
   if (dlg->editorView->bridgeDoc.GetNumSpans() == 0 ||
       !dlg->editorView->GetEditorDisplayObjectManager().IsSpanSelected())
      {
      grp->GetRadioButton(RGB_BYCOMMAND,IDC_CURRENT)->EnableWindow(false);
      grp->CheckRadioButton(RGB_BYCOMMAND,IDC_END);
      }
}

void InsertionPoint::Help(TNotify far&)
{
   TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);
   theApp->ActivateHelp(this,IDH_Insertion_Point);
}

//
//---------------------------------------------------------------
//

TEditSpanDlg::TEditSpanDlg (TWindow* parent,SystemOfUnits units,TModule* module) :
TMyPropertySheet(parent,"Edit Span",0,false,PSH_DEFAULT | PSH_HASHELP | PSH_NOAPPLYNOW,module)
{
   page1 = new SpanProperties(this,units,module);
   page2 = new BoundryConditions(this,units,module);
   page3 = new Hinges(this,units,module);
   page4 = new LimitStateFactors(this,units,module);

   AddPage(*page1);
   AddPage(*page2);
   AddPage(*page3);
   AddPage(*page4);
}


TEditSpanDlg::~TEditSpanDlg ()
{
    Destroy();

    delete page1;
    delete page2;
    delete page3;
    delete page4;
}

void TEditSpanDlg::SetupTransferBuffer(TEditSpanDlgXferBuffer *buffer)
{
   page1->SetTransferBuffer(&buffer->properties);
   page2->SetTransferBuffer(&buffer->bc);
   page3->SetTransferBuffer(&buffer->hinges);
   page4->SetTransferBuffer(&buffer->limitStateFactors);
}

//
//---------------------------------------------------------------
//

TAddSpanDlg::TAddSpanDlg (TBridgeEditorView* parent,SystemOfUnits units, TModule* module) :
TEditSpanDlg(parent->GetWindow(),units,module)
{
    editorView = parent;

    page5 = new SupportPage(this,units,module);
    page6 = new InsertionPoint(this,units,module);

    AddPage(*page5);
    AddPage(*page6);
}


TAddSpanDlg::~TAddSpanDlg ()
{
    Destroy();

    delete page5;
    delete page6;
}

void TAddSpanDlg::SetupTransferBuffer(TEditSpanDlgXferBuffer *buffer)
{
   TEditSpanDlg::SetupTransferBuffer(buffer);

   TAddSpanDlgXferBuffer *buf = (TAddSpanDlgXferBuffer *)buffer;

   page5->SetTransferBuffer(&buf->supportData);
   page6->SetTransferBuffer(&buf->insertionPoint);
}

