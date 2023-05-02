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



#include <qconbrdg\sprpwiz.h>
#include <qconbrdg\flprint.h>
#include <qconbrdg\sphelp.rh>

#include <shape\genshape.h>
#include <shape\plategdr.h>
#include <material\genmat.h>
#include <code\lrfdcode.h>
#include <esl\strutil.h>
#include <code\lldist.h>
#include <esl\engnotat.h>
#include <owl\printer.h>
#include <owl\profile.h>
#include <dir.h> // For MAXPATH

#pragma hdrstop
#include <esl\siconst.h>

//{{TSectPropWizard Implementation}}


TSectPropWizard::TSectPropWizard (TWindow *parent,TSectPropData &data,TModule *module) :
TWizard(parent,"Section Properties Calculator",0,PSH_DEFAULT | PSH_HASHELP | PSH_NOAPPLYNOW,module),
data(data)
{
    // INSERT>> Your constructor code here.
    page1  = new Page1(this,module);
    page2  = new Page2(this,module);
    page3a = new Page3a(this,module);
    page3b = new Page3b(this,module);
    page3c = new Page3c(this,module);
    page4a = new Page4a(this,module);
    page4b = new Page4b(this,module);
    page4c = new Page4c(this,module);
    page5  = new Page5(this,module);
    page6a = new Page6a(this,module);
    page6b = new Page6b(this,module);

    AddPage(*page1);
    AddPage(*page2);
    AddPage(*page3a);
    AddPage(*page3b);
    AddPage(*page3c);
    AddPage(*page4a);
    AddPage(*page4b);
    AddPage(*page4c);
    AddPage(*page5);
    AddPage(*page6a);
    AddPage(*page6b);
}


TSectPropWizard::~TSectPropWizard ()
{
    Destroy();

    delete page1;
    delete page2;
    delete page3a;
    delete page3b;
    delete page3c;
    delete page4a;
    delete page4b;
    delete page4c;
    delete page5;
    delete page6a;
    delete page6b;
}


/*
void TSectPropWizard::BNClicked_Help ()
{
    TQConBridgeApp *theApp = TYPESAFE_DOWNCAST(GetApplication(),TQConBridgeApp);

    int contextId;
    switch(GetPageRef().ThisTab())
       {
       case IDD_PAGE1:    contextId = SPHELP_SUPPORT;    break;
       case IDD_PAGE2:    contextId = SPHELP_DECKTYPE;   break;
       case IDD_PAGE3A:   contextId = SPHELP_WOODBEAM;   break;
       case IDD_PAGE3B:   contextId = SPHELP_STEELBEAM;  break;
       case IDD_PAGE3C:   contextId = SPHELP_IBEAM;      break;
       case IDD_PAGE4A:   contextId = SPHELP_WOODDECK;   break;
       case IDD_PAGE4B:   contextId = SPHELP_STEELGRID;  break;
       case IDD_PAGE4C:   contextId = SPHELP_CONCDECK;   break;
       case IDD_PAGE5:    contextId = SPHELP_SKEWANGLE;  break;
       case IDD_PAGE6A:
       case IDD_PAGE6B:   contextId = SPHELP_RESULTS;    break;
       }

    theApp->AppHelp(HELP_CONTEXT,contextId);
}
*/

//
//
//

ostream& operator << (ostream& os,TSectPropWizard::Page1& page1)
{
   os << "Supporting Component: ";

   switch(page1.GetBeamType())
      {
      case IDC_STEELBEAM:
           os << "Steel Beam";
           break;

      case IDC_CLOSEDBOX:
      case IDC_OPENBOX:
      case IDC_CIPBOX:
      case IDC_CIPTEE:
      case IDC_PRECASTSLAB_COMP:
      case IDC_PRECASTSLAB_INTG:
      case IDC_PRECASTCHANNEL:
      case IDC_PRECASTDOUBLETEE:
      case IDC_PRECASTTEE:
           break;

      case IDC_PRECASTIBEAM:
           os << "Precast I-Beam or Bulb-Tee Section";
           break;

      case IDC_WOOD:
           os << "Wood Beam";
           break;
      }

   return os;
}

ostream& operator << (ostream& os,TSectPropWizard::Page2& page2)
{
   os << "Deck Type           : ";

   switch(page2.GetDeckType())
      {
      case IDC_CONCRETE:
           os << "CIP/Precast Concrete";
           break;

      case IDC_STEEL_GRID:
           os << "Steel Grid";
           break;

      case IDC_WOOD_PLANK:
           os << "Wood Plank";
           break;

      case IDC_STRESS_LAMINATED:
           os << "Stress Laminated";
           break;

      case IDC_SPIKE_LAMINATED:
           os << "Spike Laminated";
           break;

      case IDC_GLUE_LAMINATED:
           os << "Glue Laminated";
           break;

      case IDC_FILLED_GRID:
           os << "Filled/Partially Filled Steel Grid";
           break;

      default:
           break; // Throw an exception, non-supported types
      }

   return os;
}

ostream& operator << (ostream& os,TSectPropWizard::Page3a& page3a)
{
   bool useSI = (page3a.wizard->data.dispUnits == unitsSI);
   double value;

   os << (page3a.IsInteriorGirder() ? "Interior Girder" : "Exterior Girder") << endl;

   value = DerivedUnits::Length(page3a.GetDepth()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
   os << "Depth = " <<  value << (useSI ? " mm" : " inch") << endl;
   value = DerivedUnits::Length(page3a.GetWidth()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
   os << "Width = " <<  value << (useSI ? " mm" : " inch") << endl;

   if (useSI)
      os << "Density = " << DerivedUnits::Density(page3a.GetDensity()).ConvertTo(SIConstants::kilogram_per_meter3).value() << " kg/m^3" << endl;
   else
      os << "Unit Wgt = " << DerivedUnits::UnitWeight(page3a.GetUnitWgt()).ConvertTo(USConstants::pound_per_feet3).value() << " pcf" <<endl;

   value = DerivedUnits::Pressure(page3a.GetModE()).ConvertTo(useSI ? SIConstants::MPa : USConstants::pound_per_inch2).value();
   os << "Mod. E = "<< value << (useSI ? " MPa" : " psi") << endl;

   return os;
}

ostream& operator << (ostream& os,TSectPropWizard::Page3b& page3b)
{
   bool useSI = (page3b.wizard->data.dispUnits == unitsSI);
   double value;

   os << (page3b.IsInteriorGirder() ? "Interior Girder" : "Exterior Girder") << endl;

   if (page3b.UseList())
      {
      int idx = page3b.list->GetSelIndex();
      char* buffer = new char[page3b.list->GetStringLen(idx)+1];
      page3b.list->GetString(buffer,idx);
      os << buffer << endl;
      delete[] buffer;
      }
   else
      {
      os << "Top Flange" << "  ";
      value = DerivedUnits::Length(page3b.GetTopFlangeThickness()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
      os << "t = " << value << (useSI ? " mm" : " inch") << "  ";
      value = DerivedUnits::Length(page3b.GetTopFlangeWidth()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
      os << "w = " << value << (useSI ? " mm" : " inch") << endl;
      os << "Web" << "  ";
      value = DerivedUnits::Length(page3b.GetWebThickness()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
      os << "t = " << value << (useSI ? " mm" : " inch") << "  ";
      value = DerivedUnits::Length(page3b.GetWebHeight()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
      os << "h = " << value << (useSI ? " mm" : " inch") << endl;
      os << "Bottom Flange" << "  ";
      value = DerivedUnits::Length(page3b.GetBottomFlangeThickness()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
      os << "t = " << value << (useSI ? " mm" : " inch") << "  ";
      value = DerivedUnits::Length(page3b.GetBottomFlangeWidth()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
      os << "w = " << value << (useSI ? " mm" : " inch") << endl;
      }

   if (useSI)
      os << "Density = " << DerivedUnits::Density(page3b.GetDensity()).ConvertTo(SIConstants::kilogram_per_meter3).value() << " kg/m^3" << endl;
   else
      os << "Unit Wgt = " << DerivedUnits::UnitWeight(page3b.GetUnitWgt()).ConvertTo(USConstants::pound_per_feet3).value() << " pcf" <<endl;

   value = DerivedUnits::Pressure(page3b.GetModE()).ConvertTo(useSI ? SIConstants::MPa : USConstants::pound_per_inch2).value();
   os << "Mod. E = "<< value << (useSI ? " MPa" : " psi") << endl;

   if (!page3b.IsInteriorGirder())
      os << (page3b.XFrames() ? "Cross Frames are present" : "Cross Frames are not present") <<  endl;

   value = DerivedUnits::Length(page3b.GetSpanLength()).ConvertTo(useSI ? SIConstants::meter : USConstants::feet).value();
   os << "Span Length = " << value << (useSI ? " m" : " feet") << endl;
   value = DerivedUnits::Length(page3b.GetSpacing()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::feet).value();
   os << "Girder Spacing = " << value << (useSI ? " mm" : " feet") << endl;
   os << "Num. Beams = " << page3b.GetNumBeams() << endl;

   if (!page3b.IsInteriorGirder())
      os << "Num. Lanes = " << page3b.GetNumLanes() << endl;

   return os;
}

ostream& operator << (ostream& os,TSectPropWizard::Page3c& page3c)
{
   bool useSI = (page3c.wizard->data.dispUnits == unitsSI);
   double value;

   os << (page3c.IsInteriorGirder() ? "Interior Girder" : "Exterior Girder") << endl;

   if (page3c.UseList())
      {
      int idx = page3c.list->GetSelIndex();
      char* buffer = new char[page3c.list->GetStringLen(idx)+1];
      page3c.list->GetString(buffer,idx);
      os << buffer << endl;
      delete[] buffer;
      }
   else
      {
      os << "Noncomposite Properties" << endl;
      value = DerivedUnits::Length2(page3c.GetAx()).ConvertTo(useSI ? SIConstants::millimeter2 : USConstants::inch2).value();
      os << "Ax = " << value << (useSI ? " mm2" : " inch2") << endl;
      value = DerivedUnits::Length4(page3c.GetIz()).ConvertTo(useSI ? SIConstants::millimeter4 : USConstants::inch4).value();
      os << "Iz = " << value << (useSI ? " mm4" : " inch4") << endl;
      value = DerivedUnits::Length(page3c.GetYbar()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
      os << "CG = " << value << (useSI ? " mm" : " inch") << endl;
      value = DerivedUnits::Length(page3c.GetHeight()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
      os << "H = " << value << (useSI ? " mm" : " inch") << endl;
      value = DerivedUnits::Length(page3c.GetTopFlangeWidth()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
      os << "Top Flg Width = " << value << (useSI ? " mm" : " inch") << endl;
      value = DerivedUnits::Length(page3c.GetWebThickness()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
      os << "Web Width = " << value << (useSI ? " mm" : " inch") << endl;
      }


   if (useSI)
      os << "Density = " << DerivedUnits::Density(page3c.GetDensity()).ConvertTo(SIConstants::kilogram_per_meter3).value() << " kg/m^3" << endl;
   else
      os << "Unit Wgt = " << DerivedUnits::UnitWeight(page3c.GetUnitWgt()).ConvertTo(USConstants::pound_per_feet3).value() << " pcf" <<endl;

   value = DerivedUnits::Pressure(page3c.GetFc()).ConvertTo(useSI ? SIConstants::MPa : USConstants::pound_per_inch2).value();
   os << "f'c = "<< value << (useSI ? " MPa" : " psi") << endl;

   if (!page3c.IsInteriorGirder())
      os << (page3c.XFrames() ? "Cross Frames are present" : "Cross Frames are not present") <<  endl;

   value = DerivedUnits::Length(page3c.GetSpanLength()).ConvertTo(useSI ? SIConstants::meter : USConstants::feet).value();
   os << "Span Length = " << value << (useSI ? " m" : " feet") << endl;
   value = DerivedUnits::Length(page3c.GetSpacing()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::feet).value();
   os << "Girder Spacing = " << value << (useSI ? " mm" : " feet") << endl;
   os << "Num. Beams = " << page3c.GetNumBeams() << endl;

   if (!page3c.IsInteriorGirder())
      os << "Num. Lanes = " << page3c.GetNumLanes() << endl;

   return os;
}

ostream& operator << (ostream& os,TSectPropWizard::Page4a& page4a)
{
   bool useSI = (page4a.wizard->data.dispUnits == unitsSI);
   double value;

   value = DerivedUnits::Length(page4a.GetTribWidth()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
   os << "Tributary Width = " << value << (useSI ? " mm" : " inch") << endl;

   value = DerivedUnits::Length(page4a.GetDepth()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
   os << "Depth = " << value << (useSI ? " mm" : " inch") << endl;

   if (!page4a.IsInteriorGirder())
      {
      value = DerivedUnits::Length(page4a.Getde()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
      os << "de = " << value << (useSI ? " mm" : " inch") << endl;
      }

   if (useSI)
      os << "Density = " << DerivedUnits::Density(page4a.GetDensity()).ConvertTo(SIConstants::kilogram_per_meter3).value() << " kg/m^3" << endl;
   else
      os << "Unit Wgt = " << DerivedUnits::UnitWeight(page4a.GetUnitWgt()).ConvertTo(USConstants::pound_per_feet3).value() << " pcf" <<endl;

   return os;
}

ostream& operator << (ostream& os,TSectPropWizard::Page4b& page4b)
{
   bool useSI = (page4b.wizard->data.dispUnits == unitsSI);
   double value;

   value = DerivedUnits::Length(page4b.Gettg()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
   os << "tg = " << value << (useSI ? " mm" : " inch") << endl;

   if (!page4b.IsInteriorGirder())
      {
      value = DerivedUnits::Length(page4b.Getde()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
      os << "de = " << value << (useSI ? " mm" : " inch") << endl;
      }

   value = DerivedUnits::LinearForce(page4b.GetWeight()).ConvertTo(useSI ? SIConstants::newton_per_meter : USConstants::pound_per_feet).value();
   os << "Weight = " << value << (useSI ? " N/m" : " plf") << endl;

   value = DerivedUnits::Length(page4b.GetDesignLaneWidth()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::feet).value();
   os << "Design Lane Width = " << value << (useSI ? " mm" : " feet") << endl;

   return os;
}

ostream& operator << (ostream& os,TSectPropWizard::Page4c& page4c)
{
   bool useSI = (page4c.wizard->data.dispUnits == unitsSI);
   double value;

   value = DerivedUnits::Length(page4c.GetSlabDepth()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
   os << "Slab Depth = " << value << (useSI ? " mm" : " inch") << endl;

   value = DerivedUnits::Length(page4c.GetPadDepth()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
   os << "Pad Depth  = " << value << (useSI ? " mm" : " inch") << endl;

   value = DerivedUnits::Length(page4c.GetSacraficalDepth()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
   os << "Sacrafical Depth = " << value << (useSI ? " mm" : " inch") << endl;

   if (!page4c.IsInteriorGirder())
      {
      value = DerivedUnits::Length(page4c.GetOverhang()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
      os << "Overhang = " << value << (useSI ? " mm" : " inch") << endl;

      value = DerivedUnits::Length(page4c.Getde()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
      os << "de = " << value << (useSI ? " mm" : " inch") << endl;
      }

   if (useSI)
      os << "Density = " << DerivedUnits::Density(page4c.GetDensity()).ConvertTo(SIConstants::kilogram_per_meter3).value() << " kg/m^3" << endl;
   else
      os << "Unit Wgt = " << DerivedUnits::UnitWeight(page4c.GetUnitWgt()).ConvertTo(USConstants::pound_per_feet3).value() << " pcf" <<endl;


   value = DerivedUnits::Pressure(page4c.GetFc()).ConvertTo(useSI ? SIConstants::MPa : USConstants::pound_per_inch2).value();
   os << "f'c = "<< value << (useSI ? " MPa" : " psi") << endl;

   value = DerivedUnits::Length(page4c.GetEffSpanLength()).ConvertTo(useSI ? SIConstants::meter : USConstants::feet).value();
   os << "Eff. Span Length = " << value << (useSI ? " m" : " feet") << endl;

   value = DerivedUnits::Length(page4c.GetDesignLaneWidth()).ConvertTo(useSI ? SIConstants::millimeter : USConstants::feet).value();
   os << "Design Lane Width = " << value << (useSI ? " mm" : " feet") << endl;

   return os;
}

ostream& operator << (ostream& os,TSectPropWizard::Page5& page5)
{
   os << "Distribution factors for moment are " << (page5.CorrectMoment() ? "" : "not ") << "corrected for skew" << endl;
   os << "Distribution factors for shear are " << (page5.CorrectShear() ? "" : "not ") << "corrected for skew" << endl;

   if (page5.CorrectMoment() || page5.CorrectShear())
      os << "Skew Angle = " << page5.GetSkewAngle() << " deg" << endl;

   return os;
}

ostream& operator << (ostream& os,TSectPropWizard::Page6a& page6a)
{
   double value;
   bool useSI = (page6a.wizard->data.dispUnits == unitsSI);

   os << "Girder Properties" << endl;

   value = DerivedUnits::Length2(page6a.results.girderAx).ConvertTo(useSI ? SIConstants::millimeter2 : USConstants::inch2).value();
   os << "Ax = " << EngNotation(value,3) << (useSI ? " mm^2" : " inch^2") << endl;
   value = DerivedUnits::Length4(page6a.results.girderIz).ConvertTo(useSI ? SIConstants::millimeter4 : USConstants::inch4).value();
   os << "Iz = " << EngNotation(value,3) << (useSI ? " mm^4" : " inch^4") << endl;
   value = DerivedUnits::Length(page6a.results.girderCG).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
   os << "CG = " << EngNotation(value,3) << (useSI ? " mm" : " inch") << endl;

   if (useSI)
      os << "Density = " << DerivedUnits::Density(page6a.results.effUnitWgt/SIConstants::g.value()).ConvertTo(SIConstants::kilogram_per_meter3).value() << " kg/m^3" << endl;
   else
      os << "Unit Wgt = " << DerivedUnits::UnitWeight(page6a.results.effUnitWgt).ConvertTo(USConstants::pound_per_feet3).value() << " pcf" <<endl;

   value = DerivedUnits::Pressure(page6a.results.modE).ConvertTo(useSI ? SIConstants::Pascal : USConstants::pound_per_inch2).value();
   os << "Mod. E = " << EngNotation(value,3) << (useSI ? " Pa" : " psi") << endl;

   os << endl;
   os << "Distribution Factors" << endl;

   os << "Strength/Service Limit State" << endl;
   os << "  Moment" << endl;
   os << "  1 Loaded Lane   = " << page6a.results.s_1_gMoment << endl;
   os << "  2+ Loaded Lanes = " << page6a.results.s_2_gMoment << endl;
   os << "  Shear" << endl;
   os << "  1 Loaded Lane   = " << page6a.results.s_1_gShear << endl;
   os << "  2+ Loaded Lanes = " << page6a.results.s_2_gShear << endl;
   os << endl;
   os << "Fatigue Limit State" << endl;
   os << "  gMoment = " << page6a.results.f_gMoment << endl;
   os << "  gShear  = " << page6a.results.f_gShear  << endl;

   return os;
}

ostream& operator << (ostream& os,TSectPropWizard::Page6b& page6b)
{
   double value;
   bool useSI = (page6b.wizard->data.dispUnits == unitsSI);

   os << "Girder Properties" << endl;

   value = DerivedUnits::Length2(page6b.results.girderAx).ConvertTo(useSI ? SIConstants::millimeter2 : USConstants::inch2).value();
   os << "Ax = " << EngNotation(value,3) << (useSI ? " mm^2" : " inch^2") << endl;
   value = DerivedUnits::Length4(page6b.results.girderIz).ConvertTo(useSI ? SIConstants::millimeter4 : USConstants::inch4).value();
   os << "Iz = " << EngNotation(value,3) << (useSI ? " mm^4" : " inch^4") << endl;
   value = DerivedUnits::Length(page6b.results.girderCG).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
   os << "CG = " << EngNotation(value,3) << (useSI ? " mm" : " inch") << endl;

   os << "Slab Properties" << endl;
   value = DerivedUnits::Length(page6b.results.effSlabWidth).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
   os << "Eff. Flange Width = " << EngNotation(value,3) << (useSI ? " mm" : " inch") << endl;
   value = DerivedUnits::Pressure(page6b.results.slabModE).ConvertTo(useSI ? SIConstants::Pascal : USConstants::pound_per_inch2).value();
   os << "Mod. E = " << EngNotation(value,3) << (useSI ? " Pa" : " psi") << endl;

   os << endl;
   os << "Composite Properties" << endl;
   value = DerivedUnits::Length2(page6b.results.cmpAx).ConvertTo(useSI ? SIConstants::millimeter2 : USConstants::inch2).value();
   os << "Ax = " << EngNotation(value,3) << (useSI ? " mm^2" : " inch^2") << endl;
   value = DerivedUnits::Length4(page6b.results.cmpIz).ConvertTo(useSI ? SIConstants::millimeter4 : USConstants::inch4).value();
   os << "Iz = " << EngNotation(value,3) << (useSI ? " mm^4" : " inch^4") << endl;
   value = DerivedUnits::Length(page6b.results.cmpCG).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
   os << "CG = " << EngNotation(value,3) << (useSI ? " mm" : " inch") << endl;

   if (useSI)
      os << "Density = " << DerivedUnits::Density(page6b.results.effUnitWgt/SIConstants::g.value()).ConvertTo(SIConstants::kilogram_per_meter3).value() << " kg/m^3" << endl;
   else
      os << "Unit Wgt = " << DerivedUnits::UnitWeight(page6b.results.effUnitWgt).ConvertTo(USConstants::pound_per_feet3).value() << " pcf" <<endl;

   value = DerivedUnits::Pressure(page6b.results.cmpModE).ConvertTo(useSI ? SIConstants::Pascal : USConstants::pound_per_inch2).value();
   os << "Mod. E = " << EngNotation(value,3) << (useSI ? " Pa" : " psi") << endl;

   os << "n = " << page6b.results.n << endl;

   os << endl;
   os << "Distribution Factors" << endl;

   value = DerivedUnits::Length(page6b.results.eg).ConvertTo(useSI ? SIConstants::millimeter : USConstants::inch).value();
   os << "eg = " << EngNotation(value,3) << (useSI ? " mm" : " inch") << endl;
   value = DerivedUnits::Length4(page6b.results.Kg).ConvertTo(useSI ? SIConstants::millimeter4 : USConstants::inch4).value();
   os << "Kg = " << EngNotation(value,3) << (useSI ? " mm^4" : " inch^4") << endl;

   os << endl;
   os << "Strength/Service Limit State" << endl;
   os << "  Moment" << endl;
   os << "  1 Loaded Lane   = " << page6b.results.s_1_gMoment << endl;
   os << "  2+ Loaded Lanes = " << page6b.results.s_2_gMoment << endl;
   os << "  Shear" << endl;
   os << "  1 Loaded Lane   = " << page6b.results.s_1_gShear << endl;
   os << "  2+ Loaded Lanes = " << page6b.results.s_2_gShear << endl;

   os << endl;
   os << "Fatigue Limit State" << endl;
   os << "  gMoment = " << page6b.results.f_gMoment << endl;
   os << "  gShear  = " << page6b.results.f_gShear  << endl;

   return os;
}

