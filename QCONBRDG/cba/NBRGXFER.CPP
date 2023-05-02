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

#include <checks.h>
#include <qconbrdg\newbridg.h>

#include <shape\genshape.h>
#include <material\genmat.h>

BridgePrototype *TNewBridgeXferBuffer::GetPrototype()
{
   BridgePrototype *prototype;

   if (step1.numSpans == 1)
      {
      GeneralShape* pshape = new GeneralShape(step3.ax,
                                              0,0,
                                              step3.iz,
                                              0,0,0,0,
                                              new GeneralMaterial(step3.modE,step3.unitWeight));

      SingleSpanPrototype* p = new SingleSpanPrototype(step2.l1,
                                          step2.LeftSupportFixity(),
                                          step2.RightSupportFixity(),
                                          pshape,
                                          step3.s_gMoment,
                                          step3.s_gShear,
                                          step3.f_gMoment,
                                          step3.f_gShear);

         prototype = p;
      }
   else
      {
      MultiSpanData spanData;
      spanData.numSpans = step1.numSpans;
      spanData.L1 = step2.l1;
      spanData.L2 = step2.l2;
      spanData.simpleSpans = step1.IsSimple();
      spanData.hasColumn = step2.HasColumn();
      spanData.hCol = step5.h;
      spanData.isHinged = step5.IsHinged();
      spanData.leftSupport = step2.LeftSupportFixity();
      spanData.interiorSupport = (step2.HasColumn()) ? step5.SupportFixity() : step2.InteriorSupportFixity();
      spanData.rightSupport = step2.RightSupportFixity();
      spanData.Ax1 = step3.ax;
      spanData.Iz1 = step3.iz;
      spanData.Ax2 = step4.ax;
      spanData.Iz2 = step4.iz;
      spanData.AxCol = step5.ax;
      spanData.IzCol = step5.iz;
      spanData.ModE1 = step3.modE;
      spanData.UnitWeight1 = step3.unitWeight;
      spanData.ModE2 = step4.modE;
      spanData.UnitWeight2 = step4.unitWeight;
      spanData.ModECol = step5.modE;
      spanData.ColUnitWeight = step5.unitWeight;
      spanData.s_gMoment1 = step3.s_gMoment;
      spanData.s_gShear1 = step3.s_gShear;
      spanData.f_gMoment1 = step3.f_gMoment;
      spanData.f_gShear1 = step3.f_gShear;
      spanData.s_gMoment2 = step4.s_gMoment;
      spanData.s_gShear2 = step4.s_gShear;
      spanData.f_gMoment2 = step4.f_gMoment;
      spanData.f_gShear2 = step4.f_gShear;

      prototype = new MultiSpanBridgePrototype(spanData);
      }

   return prototype;
}

TNewBridgeXferBuffer::Page2::Page2(SystemOfUnits units)
{
   leftSupport = IDC_LS_ROLLER;
   interiorSupport = IDC_IS_PINNED;
   rightSupport = IDC_RS_ROLLER;

   ResetValues(units);
}

void TNewBridgeXferBuffer::Page2::ResetValues(SystemOfUnits units)
{
   if (units == unitsSI)
      {
      l1 = FundamentalUnits::Length(1.0*SIConstants::meter).value();
      l2 = FundamentalUnits::Length(1.0*SIConstants::meter).value();
      }
   else
      {
      l1 = FundamentalUnits::Length(1.0*USConstants::feet).value();
      l2 = FundamentalUnits::Length(1.0*USConstants::feet).value();
      }
}

Fixity TNewBridgeXferBuffer::Page2::LeftSupportFixity()
{
   return (leftSupport == IDC_LS_ROLLER) ? fxRoller : fxPinned;
}

Fixity TNewBridgeXferBuffer::Page2::InteriorSupportFixity()
{
   return (interiorSupport == IDC_IS_ROLLER) ? fxRoller : fxPinned;
}

Fixity TNewBridgeXferBuffer::Page2::RightSupportFixity()
{
   return (rightSupport == IDC_RS_ROLLER) ? fxRoller : fxPinned;
}

//
// Page 3
//

TNewBridgeXferBuffer::Page3::Page3(SystemOfUnits units)
{
   ResetValues(units);
}

void TNewBridgeXferBuffer::Page3::ResetValues(SystemOfUnits units)
{
   if (units == unitsSI)
      {
      double g = SIConstants::g.value();
      ax = DerivedUnits::Length2(1.0*SIConstants::millimeter2).value();
      iz = DerivedUnits::Length4(1.0*SIConstants::millimeter4).value();
      modE = DerivedUnits::Pressure(1.0*SIConstants::Pascal).value();
      unitWeight = DerivedUnits::UnitWeight(g*SIConstants::kilogram_per_meter3.value()).value();
      }
   else
      {
      ax = DerivedUnits::Length2(1.0*USConstants::inch2).value();
      iz = DerivedUnits::Length4(1.0*USConstants::inch4).value();
      modE = DerivedUnits::Pressure(1.0*USConstants::kip_per_inch2).value();
      unitWeight = DerivedUnits::UnitWeight(1.0*USConstants::pound_per_feet3).value();
      }

   s_gMoment = 1.0;
   s_gShear  = 1.0;
   f_gMoment = 1.0;
   f_gShear  = 1.0;
}

//
// Page 4
//

TNewBridgeXferBuffer::Page4::Page4(SystemOfUnits units)
{
   ResetValues(units);
}

void TNewBridgeXferBuffer::Page4::ResetValues(SystemOfUnits units)
{
   if (units == unitsSI)
      {
      double g = SIConstants::g.value();
      h = FundamentalUnits::Length(1.0*SIConstants::meter).value();
      ax = DerivedUnits::Length2(1.0*SIConstants::millimeter2).value();
      iz = DerivedUnits::Length4(1.0*SIConstants::millimeter4).value();
      modE = DerivedUnits::Pressure(1.0*SIConstants::Pascal).value();
      unitWeight = DerivedUnits::UnitWeight(g*SIConstants::kilogram_per_meter3.value()).value();
      }
   else
      {
      h = FundamentalUnits::Length(1.0*USConstants::feet).value();
      ax = DerivedUnits::Length2(1.0*USConstants::inch2).value();
      iz = DerivedUnits::Length4(1.0*USConstants::inch4).value();
      modE = DerivedUnits::Pressure(1.0*USConstants::kip_per_inch2).value();
      unitWeight = DerivedUnits::UnitWeight(1.0*USConstants::pound_per_feet3).value();
      }

   support = IDC_IS_COLUMN; // Fixed
   hinge = BF_CHECKED;
}

Fixity TNewBridgeXferBuffer::Page4::SupportFixity()
{
   Fixity fixity;
   switch(support)
      {
      case IDC_IS_ROLLER: fixity = fxRoller; break;
      case IDC_IS_PINNED: fixity = fxPinned; break;
      case IDC_IS_COLUMN: fixity = fxFixed;  break;
      }

   return fixity;
}
