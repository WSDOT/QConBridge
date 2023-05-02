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

#if !defined(__newbridg_h)              // Sentry, use file only if it's not already included.
#define __newbridg_h


#include <qconbrdg\prototyp.h>
#include <qconbrdg\mytypes.h>
#include <qconbrdg\newbridg.rh>

#include <owl\dialog.h>
#include <owl\checkbox.h>
#include <owltools\unitedit.h>
#include <owltools\radiogrp.h>
#include <owltools\nbredit.h>
#include <owltools\spin.h>
#include <owltools\matedit.h>
#include <owltools\wizard.h>


#define IDPAGE_STEP1    0
#define IDPAGE_STEP2    1
#define IDPAGE_STEP3    2
#define IDPAGE_STEP4    3
#define IDPAGE_STEP5    4
#define IDPAGE_STEP6    5

struct TNewBridgeXferBuffer
{
   TNewBridgeXferBuffer(SystemOfUnits units) {
                                              step1.SetUnits(units);
                                              step2.ResetValues(units);
                                              step3.ResetValues(units);
                                              step4.ResetValues(units);
                                              step5.ResetValues(units);
                                              }

   BridgePrototype *GetPrototype();

   struct Page1
      {
      int numSpans;
      int continuity;
      int units;

      Page1() {numSpans = 1; continuity = IDC_CONTINUOUSSPANS; units = IDC_SI;}

      int NumSpans() {return numSpans;}
      bool IsSimple() {return continuity == IDC_SIMPLESPANS;}
      bool IsContinuous() {return continuity == IDC_CONTINUOUSSPANS;}
      void SetUnits(SystemOfUnits u) { units = (u == unitsSI) ? IDC_SI : IDC_ENGLISH;}
      SystemOfUnits GetUnits() {return (units == IDC_SI) ? unitsSI : unitsEnglish;}
      } step1;

   struct Page2
      {
      Page2(SystemOfUnits units = unitsSI);
      void ResetValues(SystemOfUnits units);

      double l1;
      double l2;
      int leftSupport;
      int interiorSupport;
      int rightSupport;

      Fixity LeftSupportFixity();
      Fixity InteriorSupportFixity();
      Fixity RightSupportFixity();

      bool HasColumn() {return interiorSupport == IDC_IS_COLUMN;}
      } step2;

   struct Page3
      {
      double ax;
      double iz;
      double modE;
      double unitWeight;
      double s_gMoment;
      double s_gShear;
      double f_gMoment;
      double f_gShear;

      Page3(SystemOfUnits units = unitsSI);
      void ResetValues(SystemOfUnits units);
      } step3,step4;

   struct Page4
      {
      double h;
      double ax;
      double iz;
      double modE;
      double unitWeight;
      int support;
      int hinge;

      Page4(SystemOfUnits units = unitsSI);
      void ResetValues(SystemOfUnits units);

      Fixity SupportFixity();

      bool IsHinged()
         {
         return (hinge == BF_CHECKED);
         }

      } step5;
};


//{{TDialog = TNewBridgeWizard}}
class TNewBridgeWizard : public TWizard
{
public:
    TNewBridgeWizard (TWindow* parent,SystemOfUnits units,TModule* module = 0);
    virtual ~TNewBridgeWizard ();
    void SetupTransferBuffer(TNewBridgeXferBuffer *buffer);

   // placeholder definitions
   class TWizardPage1;
   class TWizardPage2;
   class TWizardPage3;
   class TWizardPage4;
   class TWizardPage5;

   class TWizardPage1 : public TWizardStep // Start Page
      {
      public:
         TWizardPage1(TNewBridgeWizard* wizard,TModule *module = 0);
         ~TWizardPage1();
         void SetupWindow();
         void EvValueChanged();
         void EvContinuityChanged();

         int GetNumSpans() const;
         SystemOfUnits GetUnits() const;

      protected:
         TRangeSpinEdit *numSpans;
         TRadioGroupBox *continuity;
         TRadioGroupBox *units;

         TNewBridgeWizard* Wizard;

         void Help(TNotify far&);

      DECLARE_RESPONSE_TABLE(TNewBridgeWizard::TWizardPage1);
      };

   class TWizardPage2 : public TWizardStep // Span Geometry Page
      {
      public:
         TWizardPage2(TNewBridgeWizard* wizard,SystemOfUnits units, TModule *module = 0);
         ~TWizardPage2();

      protected:
         void SetupWindow();
         int  SetActive(TNotify far& notifyInfo);
         void EvLeftSupportChanged();
         void EvInteriorSupportChanged();
         void EvRightSupportChanged();

         void Help(TNotify far&);

      private:
         int GetNumSpans() const;
         void UpdateUnitsTags();

         TLengthEdit *l1;
         TLengthEdit *l2;
         TRadioGroupBox *leftSupport;
         TRadioGroupBox *interiorSupport;
         TRadioGroupBox *rightSupport;

         TNewBridgeWizard* Wizard;

      friend TWizardPage3;
      DECLARE_RESPONSE_TABLE(TNewBridgeWizard::TWizardPage2);
      };

   class TWizardPage3 : public TWizardStep // Span Properties Page
      {
      public:
         TWizardPage3(TNewBridgeWizard* wizard,SystemOfUnits units, bool exteriorGirder, TModule *module = 0);
         ~TWizardPage3();
         void SetupWindow();
         int  SetActive(TNotify far& notifyInfo);

      protected:
         void Help(TNotify far&);
         void BNClicked_SectPropCalculator();
         bool exteriorGirder;

         TLength2Edit *ax;
         TLength4Edit *iz;
         TPressureEdit *modE;
         TUnitWeightEdit *unitWeight;
         TMaterialDensityEdit *density;
         TNumberEdit *s_gMoment;
         TNumberEdit *s_gShear;
         TNumberEdit *f_gMoment;
         TNumberEdit *f_gShear;

         TNewBridgeWizard* Wizard;

      private:
         void UpdateUnitsTags();
         void SetSpanTypeLabel(const char *label);

      DECLARE_RESPONSE_TABLE(TNewBridgeWizard::TWizardPage3);
      };

   class TWizardPage4 : public TWizardStep // Column Properties Page
      {
      public:
         TWizardPage4(TNewBridgeWizard* wizard,SystemOfUnits units,TModule *module = 0);
         ~TWizardPage4();
         void SetupWindow();
         void BoundryConditions(int firstSupport,int lastSupport);
         void EnableHinged(bool enable);

      protected:
         TLengthEdit *h;
         TLength2Edit *ax;
         TLength4Edit *iz;
         TPressureEdit *modE;
         TUnitWeightEdit *unitWeight;
         TMaterialDensityEdit *density;
         TRadioGroupBox *support;
         TCheckBox *hingeChk;

         int  SetActive(TNotify far& notifyInfo);
         void Help(TNotify far&);

         TNewBridgeWizard* Wizard;

      private:
         void UpdateUnitsTags();
      };

   class TWizardPage5 : public TWizardStep // Finish Page
      {
      public:
         TWizardPage5(TNewBridgeWizard* wizard, TModule *module = 0);
         ~TWizardPage5();

         TNewBridgeWizard* Wizard;
      };

   private:
      TWizardPage1* step1;
      TWizardPage2* step2;
      TWizardPage3* step3;
      TWizardPage3* step4;
      TWizardPage4* step5;
      TWizardPage5* step6;

   public:
      TWizardPage1* GetStep1();
      TWizardPage2* GetStep2();
      TWizardPage3* GetStep3();
      TWizardPage3* GetStep4();
      TWizardPage4* GetStep5();
      TWizardPage5* GetStep6();
};    //{{TNewBridgeWizard}}


#endif                                      // __newbridg_h sentry.

