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

#if !defined(__sprpwiz_h)              // Sentry, use file only if it's not already included.
#define __sprpwiz_h

#include <qconbrdg\qcbapp.h>
#include <owltools\wizard.h>
#include <owltools\radiogrp.h>
#include <owltools\unitedit.h>
#include <owltools\intedit.h>
#include <owl\combobox.h>
#include <owl\checkbox.h>
#include <shape\ibeam.h>
#include <code\lldist.h>
#include <qconbrdg\sprpwiz.rh>
#include <owltools\matedit.h>
#include <dir.h>

#define IDD_PAGE1    0
#define IDD_PAGE2    1
#define IDD_PAGE3A   2
#define IDD_PAGE3B   3
#define IDD_PAGE3C   4
#define IDD_PAGE4A   5
#define IDD_PAGE4B   6
#define IDD_PAGE4C   7
#define IDD_PAGE5    8
#define IDD_PAGE6A   9
#define IDD_PAGE6B   10

struct TSectPropData
{
   SystemOfUnits dispUnits;
   double spanLength;
   double effSpanLength;
   double ax;
   double iz;
   double modE;
   double unitWeight;
   double s_gMoment;
   double s_gShear;
   double f_gMoment;
   double f_gShear;
};

class TSectPropWizard : public TWizard
{
public:
 	 TSectPropWizard (TWindow *pParent,TSectPropData &data,TModule *module = 0);
    virtual ~TSectPropWizard ();
    TSectPropData &data;

    class Page1 : public TWizardStep
       {
       public:
          Page1 (TSectPropWizard *wiz, TModule *module = 0);
          ~Page1();
          void SetupWindow();
          int GetBeamType() const { return beamType->GetSelRadioButton();}

          friend ostream& operator << (ostream& os,TSectPropWizard::Page1& page1);

       protected:
          TRadioGroupBox* beamType;
          TSectPropWizard* wizard;

          void EvTypeChanged();
          void Help(TNotify far&);

       DECLARE_RESPONSE_TABLE(Page1);
       };

    class Page2 : public TWizardStep
       {
       public:
          Page2 (TSectPropWizard *wiz,TModule *module = 0);
          ~Page2();
          void SetupWindow();
          int GetDeckType() const { return deckType->GetSelRadioButton();}
          void GetDeckType(char* type,int max);

          int SetActive(TNotify far& nmHdr);

          friend ostream& operator << (ostream& os,TSectPropWizard::Page2& page2);

       protected:
          TRadioGroupBox* deckType;
          TSectPropWizard* wizard;

          void EvTypeChanged();
          void Help(TNotify far&);

       DECLARE_RESPONSE_TABLE(Page2);
       };

    class Page3a : public TWizardStep
       {
       public:
          Page3a (TSectPropWizard *wiz, TModule *module = 0);
          ~Page3a();
          void SetupWindow();

          double GetDepth() const {return depth->GetValue();}
          double GetWidth() const {return width->GetValue();}
          double GetUnitWgt() const {return (unitWgt) ? unitWgt->GetValue() : density->GetValue();}
          double GetDensity() const {return GetUnitWgt()/SIConstants::g.value();}
          double GetModE() const {return modE->GetValue();}
          bool IsInteriorGirder() const {return GetGirderLocation() == IDC_INTGIRDER;}
          int GetGirderLocation() const {return location->GetSelRadioButton();}
          double GetSpacing() const {return spacing->GetValue();}
          TShape* GetBeam();

          friend ostream& operator << (ostream& os,TSectPropWizard::Page3a& page3a);

       protected:
          TSectPropWizard* wizard;
          TLengthEdit*          depth;
          TLengthEdit*          width;
          TUnitWeightEdit*      unitWgt;
          TMaterialDensityEdit* density;
          TPressureEdit*        modE;
          TRadioGroupBox*       location;
          TLengthEdit* spacing;

          void Help(TNotify far&);
       };

    class Page3b : public TWizardStep
       {
       public:
          Page3b (TSectPropWizard *wiz,TModule *module = 0);
          ~Page3b();
          void SetupWindow();
          int SetActive(TNotify far& nmHdr);

          int GetGirderLocation() const {return location->GetSelRadioButton();}
          bool IsInteriorGirder() const {return GetGirderLocation() == IDC_INTGIRDER;}
          double GetUnitWgt() const {return (unitWgt) ? unitWgt->GetValue() : density->GetValue();}
          double GetDensity() const {return GetUnitWgt()/SIConstants::g.value();}
          double GetModE() const {return modE->GetValue();}
          bool UseList() const {return (useList->GetCheck() == BF_CHECKED);}
          double GetSpacing() const {return spacing->GetValue();}
          double GetWebThickness() const;
          double GetWebHeight() const {return webHeight->GetValue();}
          double GetTopFlangeWidth() const;
          double GetTopFlangeThickness() const {return tfThick->GetValue();}
          double GetBottomFlangeWidth() const {return bfWidth->GetValue();}
          double GetBottomFlangeThickness() const {return bfThick->GetValue();}
          double GetHeight() const;
          TShape* GetBeam() const;
          double GetSpanLength() const {return spanLength->GetValue();}
          int GetNumBeams() const {return numBeams->GetValue();}
          int GetNumLanes() const {return numLanes->GetValue();}
          bool XFrames() const {return (xFrames->GetCheck() == BF_CHECKED);}

          void BNClicked_BeamCheck();
          void BNClicked_GirderLocation();

          friend ostream& operator << (ostream& os,TSectPropWizard::Page3b& page3b);

       protected:
          TSectPropWizard* wizard;
          TCheckBox* useList;
          TComboBox* list;
          TLengthEdit* tfThick;
          TLengthEdit* tfWidth;
          TLengthEdit* webThick;
          TLengthEdit* webHeight;
          TLengthEdit* bfThick;
          TLengthEdit* bfWidth;
          TUnitWeightEdit* unitWgt;
          TMaterialDensityEdit* density;
          TPressureEdit*        modE;
          TRadioGroupBox* location;
          TCheckBox* xFrames;
          TLengthEdit* spanLength;
          TLengthEdit* spacing;
          TIntEdit* numBeams;
          TIntEdit* numLanes;

          void LoadBeamTable();
          void Help(TNotify far&);

          struct BeamDimensions
             {
             double ax;
             double iz;
             double d;
             double tw;
             double bf;
             BeamDimensions(double ax,double iz,double d,double tw,double bf) :
             ax(ax),iz(iz),d(d),tw(tw),bf(bf) {}
             };

       DECLARE_RESPONSE_TABLE(Page3b);
       };

    class Page3c : public TWizardStep
       {
       public:
          Page3c (TSectPropWizard *wiz, TModule *module = 0);
          ~Page3c();
          void SetupWindow();

          void BNClicked_BeamCheck();
          void BNClicked_GirderLocation();

          int GetGirderLocation() const {return location->GetSelRadioButton();}
          bool IsInteriorGirder() const {return GetGirderLocation() == IDC_INTGIRDER;}
          double GetUnitWgt() const {return (unitWgt) ? unitWgt->GetValue() : density->GetValue();}
          double GetDensity() const {return GetUnitWgt()/SIConstants::g.value();}
          double GetModE() const;
          double GetFc() const {return fc->GetValue();}
          bool UseList() const {return (useList->GetCheck() == BF_CHECKED);}
          double GetAx() const {return ax->GetValue();}
          double GetIz() const {return iz->GetValue();}
          double GetYbar() const {return yb->GetValue();}
          double GetHeight() const;
          TShape* GetBeam() const;
          double GetSpacing() const {return spacing->GetValue();}
          double GetSpanLength() const {return spanLength->GetValue();}
          double GetWebThickness() const;
          double GetTopFlangeWidth() const;
          int GetNumBeams() const {return numBeams->GetValue();}
          int GetNumLanes() const {return numLanes->GetValue();}
          bool XFrames() const {return (xFrames->GetCheck() == BF_CHECKED);}

          friend ostream& operator << (ostream& os,TSectPropWizard::Page3c& page3c);

       protected:
          TSectPropWizard* wizard;
          TCheckBox* useList;
          TComboBox* list;
          TLength2Edit* ax;
          TLength4Edit* iz;
          TLengthEdit* yb;
          TLengthEdit* h;
          TLengthEdit* tfWidth;
          TLengthEdit* wWeb;
          TUnitWeightEdit* unitWgt;
          TMaterialDensityEdit* density;
          TPressureEdit*        fc;
          TRadioGroupBox* location;
          TCheckBox* xFrames;
          TLengthEdit* spanLength;
          TLengthEdit* spacing;
          TIntEdit* numBeams;
          TIntEdit* numLanes;

          void Help(TNotify far&);
          void LoadBeamTable();

          struct GirderDimensions
             {
             double A;
             double B;
             double C;
             double D;
             double E;
             double F;
             double G;
             double H;
             double J;
             double V;
             double W;
             double X;
             double Y;
             double Z;
             GirderDimensions(double a,double b,double c,double d,double e,double f,
                              double g,double h,double j,double v,double w,double x,
                              double y,double z)
             {
             A=a; B=b; C=c; D=d; E=e; F=f; G=g; H=h; J=j; V=v; W=w; X=x; Y=y; Z=z;
             }
             };

       DECLARE_RESPONSE_TABLE(Page3c);
       };

    class Page4a : public TWizardStep
       {
       public:
          Page4a (TSectPropWizard *wiz,TModule *module = 0);
          ~Page4a();
          void SetupWindow();

          bool IsInteriorGirder();
          double GetTribWidth() const {return width->GetValue();}
          double GetDepth() const {return depth->GetValue();}
          double Getde() const {return de->GetValue();}
          double GetUnitWgt() const {return (unitWgt) ? unitWgt->GetValue() : density->GetValue();}
          double GetDensity() const {return GetUnitWgt()/SIConstants::g.value();}
          double GetDesignLaneWidth() const {return designLane->GetValue();}

          int SetActive(TNotify far& nmHdr);

          friend ostream& operator << (ostream& os,TSectPropWizard::Page4a& page4a);

       protected:
          TSectPropWizard* wizard;
          TLengthEdit*          width;
          TLengthEdit*          depth;
          TLengthEdit*          de;
          TUnitWeightEdit*      unitWgt;
          TMaterialDensityEdit* density;
          TLengthEdit*          designLane;

          void Help(TNotify far&);
       };

    class Page4b : public TWizardStep
       {
       public:
          Page4b (TSectPropWizard *wiz,TModule *module = 0);
          ~Page4b();
          void SetupWindow();

          int SetActive(TNotify far& nmHdr);

          bool IsInteriorGirder();
          double Gettg() const {return tg->GetValue();}
          double Getde() const {return de->GetValue();}
          double GetWeight() const {return weight->GetValue();}
          double GetDesignLaneWidth() const {return designLane->GetValue();}

          friend ostream& operator << (ostream& os,TSectPropWizard::Page4b& page4b);

       protected:
          TSectPropWizard* wizard;
          TLengthEdit* tg;
          TLengthEdit* de;
          TLinearForceEdit* weight;
          TLengthEdit*          designLane;

          void Help(TNotify far&);
       };

    class Page4c : public TWizardStep
       {
       public:
          Page4c (TSectPropWizard *wiz,TModule *module = 0);
          ~Page4c();
          void SetupWindow();

          int SetActive(TNotify far& nmHdr);

          double GetUnitWgt() const {return (unitWgt) ? unitWgt->GetValue() : density->GetValue();}
          double GetDensity() const {return GetUnitWgt()/SIConstants::g.value();}
          double GetModE() const;
          double GetFc() const {return fc->GetValue();}
          double GetEffSpanLength() const {return effSpanLength->GetValue();}
          double GetSlabDepth() const {return depth->GetValue();}
          double GetPadDepth() const {return padDepth->GetValue();}
          double GetSacraficalDepth() const {return sacDepth->GetValue();}
          double GetOverhang() const {return overhang->GetValue();}
          double Getde() const {return de->GetValue();}
          double GetDesignLaneWidth() const {return designLane->GetValue();}
          double GetEffSlabWidth() const;
          TCompSlab* GetSlab() const;

          friend ostream& operator << (ostream& os,TSectPropWizard::Page4c& page4c);

       protected:
          TSectPropWizard* wizard;
          TLengthEdit* depth;
          TLengthEdit* padDepth;
          TLengthEdit* sacDepth;
          TLengthEdit* overhang;
          TLengthEdit* de;
          TUnitWeightEdit* unitWgt;
          TMaterialDensityEdit* density;
          TPressureEdit* fc;
          TLengthEdit* effSpanLength;
          TLengthEdit*          designLane;

          bool IsInteriorGirder();
          void Help(TNotify far&);
       };

    class Page5 : public TWizardStep
       {
       public:
          Page5 (TSectPropWizard *wiz,TModule *module = 0);
          ~Page5();
          void SetupWindow();
          void BNClicked_CorrectForSkew();

          bool CorrectMoment() const {return (correctMoment->GetCheck() == BF_CHECKED);}
          bool CorrectShear()  const {return (correctShear->GetCheck()  == BF_CHECKED);}
          double GetSkewAngle() const {return angle->GetValue();}

          friend ostream& operator << (ostream& os,TSectPropWizard::Page5& page5);

       protected:
          TSectPropWizard* wizard;
          TCheckBox* correctMoment;
          TCheckBox* correctShear;
          TNumberEdit* angle;

          void Help(TNotify far&);

       DECLARE_RESPONSE_TABLE(Page5);
       };

    class Page6a : public TWizardStep
       {
       public:
          Page6a (TSectPropWizard *wiz,TModule *module = 0);
          ~Page6a();
          void SetupWindow();
          int SetActive(TNotify far& nmHdr);

          friend ostream& operator << (ostream& os,TSectPropWizard::Page6a& page6a);

       protected:
          TSectPropWizard* wizard;
          LLDF* lldf;
          char filename[MAXPATH];

          void ResultsToDlg();

          void WoodBeam_Deck();
//          void WoodBeam_WoodDeck();
//          void WoodBeam_ConcDeck();
          void SteelBeam_WoodDeck();
          void SteelBeam_GridDeck();

          void WriteReport();
          void Help(TNotify far&);
          void BNClicked_Print();

          struct Results
             {
             double girderAx;
             double girderIz;
             double girderCG;
             double effUnitWgt;
             double modE;
             double s_1_gMoment;
             double s_2_gMoment;
             double s_1_gShear;
             double s_2_gShear;
             double f_gMoment;
             double f_gShear;
             } results;

       DECLARE_RESPONSE_TABLE(Page6a);
       };

    class Page6b : public TWizardStep
       {
       public:
          Page6b (TSectPropWizard *wiz, TModule *module = 0);
          ~Page6b();
          void SetupWindow();
          int SetActive(TNotify far& nmHdr);

          friend ostream& operator << (ostream& os,TSectPropWizard::Page6b& page6b);
       protected:
          TSectPropWizard* wizard;
          LLDF* lldf;
          char filename[MAXPATH];

          double Compute_eg(TShape* beam,TCompSlab* slab);
          double ComputeKg(TShape* beam,TCompSlab* slab,double eg);

          void ResultsToDlg();

          void SteelBeam_ConcreteDeck();
          void PrecastIBeam_ConcreteDeck();

          void WriteReport();
          void Help(TNotify far&);
          void BNClicked_Print();

          struct Results
             {
             double girderAx;
             double girderIz;
             double girderCG;
             double effSlabWidth;
             double slabModE;
             double cmpAx;
             double cmpIz;
             double cmpCG;
             double effUnitWgt;
             double cmpModE;
             double n;
             double eg;
             double Kg;
             double s_1_gMoment;
             double s_2_gMoment;
             double s_1_gShear;
             double s_2_gShear;
             double f_gMoment;
             double f_gShear;
             } results;

       DECLARE_RESPONSE_TABLE(Page6b);
       };

friend Page1;
friend Page2;
friend Page3a;
friend Page3b;
friend Page3c;
friend Page4a;
friend Page4b;
friend Page4c;
friend Page5;
friend Page6a;
friend Page6b;

private:
   Page1*  page1;
   Page2*  page2;
   Page3a* page3a;
   Page3b* page3b;
   Page3c* page3c;
   Page4a* page4a;
   Page4b* page4b;
   Page4c* page4c;
   Page5*  page5;
   Page6a* page6a;
   Page6b* page6b;
};    //{{TSectPropWizard}}


ostream& operator << (ostream& os,TSectPropWizard::Page1& page1);
ostream& operator << (ostream& os,TSectPropWizard::Page2& page2);
ostream& operator << (ostream& os,TSectPropWizard::Page3a& page3a);
ostream& operator << (ostream& os,TSectPropWizard::Page3b& page3b);
ostream& operator << (ostream& os,TSectPropWizard::Page3c& page3c);
ostream& operator << (ostream& os,TSectPropWizard::Page4a& page4a);
ostream& operator << (ostream& os,TSectPropWizard::Page4b& page4b);
ostream& operator << (ostream& os,TSectPropWizard::Page4c& page4c);
ostream& operator << (ostream& os,TSectPropWizard::Page5& page5);
ostream& operator << (ostream& os,TSectPropWizard::Page6a& page6a);
ostream& operator << (ostream& os,TSectPropWizard::Page6b& page6b);

#endif                                      // __clientdl_h sentry.
