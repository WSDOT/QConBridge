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

#include <code\combine.h>
#include <code\lrfd_eqn.h>
#include <code\version.h>
#include <code\debug.h>

DIAG_DEFINE_GROUP(LRFDCode_LoadCombiner,ENABLED,LEVEL2);

LoadCombiner::LoadCombiner()
{
#if defined (__BORLANDC__)
   loadings = new LoadingContainer(10,0,10);
   loadFactors = new LoadFactorContainer(10,0,10);
#else
   loadings = new LoadingContainer;
   loadFactors = new LoadFactorContainer;
#endif

   Reset();
}

LoadCombiner::~LoadCombiner()
{
   delete loadings;
   delete loadFactors;
}

void LoadCombiner::Reset()
{
   nd = 1.0;
   nr = 1.0;
   ni = 1.0;

   g = 1.0;
   m = 1.0;

   loadings->Flush();
   loadFactors->Flush();

   comboMode = Force;
   EHmode = Active;
   EVmode = OverallStability;
   extremeMode = Earthquake;
}


//
// Modes and Options ----------------------------------------------------------
//

void LoadCombiner::SetCombinationMode(CombinationMode mode)
{
   comboMode = mode;
}

LoadCombiner::CombinationMode LoadCombiner::GetCombinationMode() const
{
   return comboMode;
}

void LoadCombiner::SetHorizEarthPressureMode(HorizEarthPressureMode mode)
{
   EHmode = mode;
}

LoadCombiner::HorizEarthPressureMode LoadCombiner::GetHorizEarthPressureMode() const
{
   return EHmode;
}

void LoadCombiner::SetVertEarthPressureMode(VertEarthPressureMode mode)
{
   EVmode = mode;
}

LoadCombiner::VertEarthPressureMode LoadCombiner::GetVertEarthPressureMode() const
{
   return EVmode;
}

void LoadCombiner::SetExtremeLoad(ExtremeLoad mode)
{
   extremeMode = mode;
}

LoadCombiner::ExtremeLoad LoadCombiner::GetExtremeLoad() const
{
   return extremeMode;
}

//
// Load Factors --------------------------------------------------------------
//

void LoadCombiner::SetLoadFactor(LoadCombiner::LoadFactor ldfact,
                                 LoadCombiner::LimitState limitState,
                                 double value)
{
   TRACEX(LRFDCode_LoadCombiner,LEVEL1,"LoadCombiner::SetLoadFactor()");

   int idx;
   LoadFactorRecord find;
   LoadFactorRecord *lfr;

   find.loadFactor = ldfact;
   find.limitState = limitState;
   idx = loadFactors->Find(&find);

   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"idx = " << idx);

   if (idx == INT_MAX)
      {
      TRACEX(LRFDCode_LoadCombiner,LEVEL3,"No record found, create one");
      lfr = new LoadFactorRecord;
      lfr->loadFactor = ldfact;
      lfr->limitState = limitState;
      lfr->value = value;
      }
   else
      {
      TRACEX(LRFDCode_LoadCombiner,LEVEL3,"Record found, modify it");
      lfr = (*loadFactors)[idx];
      lfr->value = value;
      }
}

double LoadCombiner::GetLoadFactor(LoadCombiner::LoadFactor ldfact,
                                   LoadCombiner::LimitState limitState) const
{
   double lf;

   switch(ldfact)
      {
      case dcMin:  lf = GetDCMinLoadFactor(limitState);  break;
      case dcMax:  lf = GetDCMaxLoadFactor(limitState);  break;
      case ddMin:  lf = GetDDMinLoadFactor(limitState);  break;
      case ddMax:  lf = GetDDMaxLoadFactor(limitState);  break;
      case dwMin:  lf = GetDWMinLoadFactor(limitState);  break;
      case dwMax:  lf = GetDWMaxLoadFactor(limitState);  break;
      case ehMin:  lf = GetEHMinLoadFactor(limitState);  break;
      case ehMax:  lf = GetEHMaxLoadFactor(limitState);  break;
      case evMin:  lf = GetEVMinLoadFactor(limitState);  break;
      case evMax:  lf = GetEVMaxLoadFactor(limitState);  break;
      case esMin:  lf = GetESMinLoadFactor(limitState);  break;
      case esMax:  lf = GetESMaxLoadFactor(limitState);  break;
      case llim:   lf = GetLLIMLoadFactor(limitState);   break;
      case ce:     lf = GetCELoadFactor(limitState);     break;
      case br:     lf = GetBRLoadFactor(limitState);     break;
      case pl:     lf = GetPLLoadFactor(limitState);     break;
      case ls:     lf = GetLSLoadFactor(limitState);     break;
      case wa:     lf = GetWALoadFactor(limitState);     break;
      case ws:     lf = GetWSLoadFactor(limitState);     break;
      case wl:     lf = GetWLLoadFactor(limitState);     break;
      case fr:     lf = GetFRLoadFactor(limitState);     break;
      case tu:     lf = GetTULoadFactor(limitState);     break;
      case cr:     lf = GetCRLoadFactor(limitState);     break;
      case sh:     lf = GetSHLoadFactor(limitState);     break;
      case tg:     lf = GetTGLoadFactor(limitState);     break;
      case se:     lf = GetSELoadFactor(limitState);     break;
      case eq:     lf = GetEQLoadFactor(limitState);     break;
      case ic:     lf = GetICLoadFactor(limitState);     break;
      case ct:     lf = GetCTLoadFactor(limitState);     break;
      case cv:     lf = GetCVLoadFactor(limitState);     break;
      }

   return lf;
}

//
// Loads ----------------------------------------------------------------------
//

void LoadCombiner::SetLoading(Loading loading,double value)
{
   TRACEX(LRFDCode_LoadCombiner,LEVEL1,"LoadCombiner::SetLoading()");

   int idx;
   LoadingRecord find;
   LoadingRecord *lr;

   find.loading = loading;
   idx = loadings->Find(&find);

   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"idx = " << idx);

   if (idx == INT_MAX)
      {
      TRACEX(LRFDCode_LoadCombiner,LEVEL3,"No record found, create a new one");
      lr = new LoadingRecord;
      lr->loading = loading;
      lr->value = value;
      loadings->Add(lr);
      }
   else
      {
      TRACEX(LRFDCode_LoadCombiner,LEVEL3,"Record found, modify it");
      lr = (*loadings)[idx];
      lr->value = value;
      }
}

double LoadCombiner::GetLoading(Loading ld) const
{
   double loading;

   switch(ld)
      {
      case loadingDC:   loading = GetDC();    break;
      case loadingDD:   loading = GetDD();    break;
      case loadingDW:   loading = GetDW();    break;
      case loadingEH:   loading = GetEH();    break;
      case loadingEV:   loading = GetEV();    break;
      case loadingES:   loading = GetES();    break;
      case loadingLLIM: loading = GetLLIM();  break;
      case loadingCE:   loading = GetCE();    break;
      case loadingBR:   loading = GetBR();    break;
      case loadingPL:   loading = GetPL();    break;
      case loadingLS:   loading = GetLS();    break;
      case loadingWA:   loading = GetWA();    break;
      case loadingWS:   loading = GetWS();    break;
      case loadingWL:   loading = GetWL();    break;
      case loadingFR:   loading = GetFR();    break;
      case loadingTU:   loading = GetTU();    break;
      case loadingCR:   loading = GetCR();    break;
      case loadingSH:   loading = GetSH();    break;
      case loadingTG:   loading = GetTG();    break;
      case loadingSE:   loading = GetSE();    break;
      case loadingEQ:   loading = GetEQ();    break;
      case loadingIC:   loading = GetIC();    break;
      case loadingCT:   loading = GetCT();    break;
      case loadingCV:   loading = GetCV();    break;
      }

   return loading;
}

//
// Load Factors
//

double LoadCombiner::GetDCMinLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthV:
      case ExtremeI:
      case ExtremeII:
           lfCode = 0.90;
           break;

      case StrengthIV:
           lfCode = 1.50;
           break;

      case ServiceI:
      case ServiceII:
      case ServiceIII:
           lfCode = 1.00;
           break;

      case Fatigue:
           lfCode = 0.00;
           break;
      }

   lf = GetUserLoadFactor(dcMin,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}

double LoadCombiner::GetDCMaxLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthV:
      case ExtremeI:
      case ExtremeII:
           lfCode = 1.25;
           break;

      case StrengthIV:
           lfCode = 1.50;
           break;

      case ServiceI:
      case ServiceII:
      case ServiceIII:
           lfCode = 1.00;
           break;

      case Fatigue:
           lfCode = 0.00;
           break;
      }

   lf = GetUserLoadFactor(dcMax,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}

double LoadCombiner::GetDDMinLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthV:
      case ExtremeI:
      case ExtremeII:
           lfCode = 0.45;
           break;

      case StrengthIV:
           // DD Not defined for this limit state. Assume 0.00
           lfCode = 0.00;
           break;

      case ServiceI:
      case ServiceII:
      case ServiceIII:
           lfCode = 1.00;
           break;

      case Fatigue:
           lfCode = 0.00;
           break;
      }

   lf = GetUserLoadFactor(ddMin,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}

double LoadCombiner::GetDDMaxLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthV:
      case ExtremeI:
      case ExtremeII:
           lfCode = 1.80;
           break;

      case StrengthIV:
           // DD Not defined for this limit state. Assume 0.00
           lfCode = 0.00;
           break;

      case ServiceI:
      case ServiceII:
      case ServiceIII:
           lfCode = 1.00;
           break;

      case Fatigue:
           lfCode = 0.00;
           break;
      }

   lf = GetUserLoadFactor(ddMax,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}

double LoadCombiner::GetDWMinLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthIV:
      case StrengthV:
      case ExtremeI:
      case ExtremeII:
           lfCode = 0.65;
           break;

      case ServiceI:
      case ServiceII:
      case ServiceIII:
           lfCode = 1.00;
           break;

      case Fatigue:
           lfCode = 0.00;
           break;
      }

   lf = GetUserLoadFactor(dwMin,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}

double LoadCombiner::GetDWMaxLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthIV:
      case StrengthV:
      case ExtremeI:
      case ExtremeII:
           lfCode = 1.50;
           break;

      case ServiceI:
      case ServiceII:
      case ServiceIII:
           lfCode = 1.00;
           break;

      case Fatigue:
           lfCode = 0.00;
           break;
      }

   lf = GetUserLoadFactor(dwMax,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}

double LoadCombiner::GetEHMinLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthIV:
      case StrengthV:
      case ExtremeI:
      case ExtremeII:
           switch(EHmode)
              {
              case Active:
                   lfCode = 0.90;
                   break;

              case AtRest:
                   lfCode = 0.90;
                   break;
              }
           break;

      case ServiceI:
      case ServiceII:
      case ServiceIII:
           lfCode = 1.00;
           break;

      case Fatigue:
           lfCode = 0.00;
           break;
      }

   lf = GetUserLoadFactor(ehMin,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}

double LoadCombiner::GetEHMaxLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthIV:
      case StrengthV:
      case ExtremeI:
      case ExtremeII:
           switch(EHmode)
              {
              case Active:
                   lfCode = 1.50;
                   break;

              case AtRest:
                   lfCode = 1.35;
                   break;
              }
           break;

      case ServiceI:
      case ServiceII:
      case ServiceIII:
           lfCode = 1.00;
           break;

      case Fatigue:
           lfCode = 0.00;
           break;
      }

   lf = GetUserLoadFactor(ehMax,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}


double LoadCombiner::GetEVMinLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthIV:
      case StrengthV:
      case ExtremeI:
      case ExtremeII:
           switch(EVmode)
             {
             case OverallStability:
                  lfCode = 1.35;
                  break;

             case RetainingStructure:
                  lfCode = 1.00;
                  break;

             case RigidBuriedStructure:
             case RigidFrame:
             case FlexibleBuriedStructure:
             case FlexibleMetalBoxCulverts:
                  lfCode = 0.90;
                  break;
             }
           break;

      case ServiceI:
      case ServiceII:
      case ServiceIII:
           lfCode = 1.00;
           break;

      case Fatigue:
           lfCode = 0.00;
           break;
      }

   lf = GetUserLoadFactor(evMin,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}

double LoadCombiner::GetEVMaxLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthIV:
      case StrengthV:
      case ExtremeI:
      case ExtremeII:
           switch(EVmode)
             {
             case OverallStability:
                  lfCode = 1.35;
                  break;

             case RetainingStructure:
                  lfCode = 1.35;
                  break;

             case RigidBuriedStructure:
                  lfCode = 1.30;
                  break;

             case RigidFrame:
                  lfCode = 1.35;
                  break;

             case FlexibleBuriedStructure:
                  lfCode = 1.95;
                  break;

             case FlexibleMetalBoxCulverts:
                  lfCode = 1.50;
                  break;
             }
           break;

      case ServiceI:
      case ServiceII:
      case ServiceIII:
           lfCode = 1.00;
           break;

      case Fatigue:
           lfCode = 0.00;
           break;
      }

   lf = GetUserLoadFactor(evMax,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}


double LoadCombiner::GetESMinLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthIV:
      case StrengthV:
      case ExtremeI:
      case ExtremeII:
           lfCode = 0.75;
           break;

      case ServiceI:
      case ServiceII:
      case ServiceIII:
           lfCode = 1.00;
           break;

      case Fatigue:
           lfCode = 0.00;
           break;
      }

   lf = GetUserLoadFactor(esMin,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}

double LoadCombiner::GetESMaxLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthIV:
      case StrengthV:
      case ExtremeI:
      case ExtremeII:
           lfCode = 1.50;
           break;

      case ServiceI:
      case ServiceII:
      case ServiceIII:
           lfCode = 1.00;
           break;

      case Fatigue:
           lfCode = 0.00;
           break;
      }

   lf = GetUserLoadFactor(esMax,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}


double LoadCombiner::GetLLIMLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
           lfCode = 1.75;
           break;

      case StrengthII:
           lfCode = 1.35;
           break;

      case StrengthIII:
           lfCode = 0.00;
           break;

      case StrengthIV:
           lfCode = 0.00;
           break;

      case StrengthV:
           lfCode = 1.35;
           break;

      case ExtremeI:
           lfCode = 0.00;
           break;

      case ExtremeII:
           lfCode = 0.50;
           break;

      case ServiceI:
           lfCode = 1.00;
           break;

      case ServiceII:
           lfCode = 1.30;
           break;

      case ServiceIII:
           lfCode = 0.80;
           break;

      case Fatigue:
           lfCode = 0.75;
           break;
      }

   lf = GetUserLoadFactor(llim,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}


double LoadCombiner::GetCELoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
           lfCode = 1.75;
           break;

      case StrengthII:
           lfCode = 1.35;
           break;

      case StrengthIII:
           lfCode = 0.00;
           break;

      case StrengthIV:
           lfCode = 0.00;
           break;

      case StrengthV:
           lfCode = 1.35;
           break;

      case ExtremeI:
           lfCode = 0.00;
           break;

      case ExtremeII:
           lfCode = 0.50;
           break;

      case ServiceI:
           lfCode = 1.00;
           break;

      case ServiceII:
           lfCode = 1.30;
           break;

      case ServiceIII:
           lfCode = 0.80;
           break;

      case Fatigue:
           lfCode = 0.75;
           break;
      }

   lf = GetUserLoadFactor(ce,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}


double LoadCombiner::GetBRLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
           lfCode = 1.75;
           break;

      case StrengthII:
           lfCode = 1.35;
           break;

      case StrengthIII:
           lfCode = 0.00;
           break;

      case StrengthIV:
           lfCode = 0.00;
           break;

      case StrengthV:
           lfCode = 1.35;
           break;

      case ExtremeI:
           lfCode = 0.00;
           break;

      case ExtremeII:
           lfCode = 0.50;
           break;

      case ServiceI:
           lfCode = 1.00;
           break;

      case ServiceII:
           lfCode = 1.30;
           break;

      case ServiceIII:
           lfCode = 0.80;
           break;

      case Fatigue:
           lfCode = 0.75;
           break;
      }

   lf = GetUserLoadFactor(br,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}


double LoadCombiner::GetPLLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
           lfCode = 1.75;
           break;

      case StrengthII:
           lfCode = 1.35;
           break;

      case StrengthIII:
           lfCode = 0.00;
           break;

      case StrengthIV:
           lfCode = 0.00;
           break;

      case StrengthV:
           lfCode = 1.35;
           break;

      case ExtremeI:
           lfCode = 0.00;
           break;

      case ExtremeII:
           lfCode = 0.50;
           break;

      case ServiceI:
           lfCode = 1.00;
           break;

      case ServiceII:
           lfCode = 1.30;
           break;

      case ServiceIII:
           lfCode = 0.80;
           break;

      case Fatigue:
           lfCode = 0.75;
           break;
      }

   lf = GetUserLoadFactor(pl,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}


double LoadCombiner::GetLSLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
           lfCode = 1.75;
           break;

      case StrengthII:
           lfCode = 1.35;
           break;

      case StrengthIII:
           lfCode = 0.00;
           break;

      case StrengthIV:
           lfCode = 0.00;
           break;

      case StrengthV:
           lfCode = 1.35;
           break;

      case ExtremeI:
           lfCode = 0.00;
           break;

      case ExtremeII:
           lfCode = 0.50;
           break;

      case ServiceI:
           lfCode = 1.00;
           break;

      case ServiceII:
           lfCode = 1.30;
           break;

      case ServiceIII:
           lfCode = 0.80;
           break;

      case Fatigue:
           lfCode = 0.75;
           break;
      }

   lf = GetUserLoadFactor(ls,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}


double LoadCombiner::GetWALoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthIV:
      case StrengthV:
      case ExtremeI:
      case ExtremeII:
      case ServiceI:
      case ServiceII:
      case ServiceIII:
           lfCode = 1.00;
           break;

      case Fatigue:
           lfCode = 0.00;
           break;
      }

   lf = GetUserLoadFactor(wa,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}


double LoadCombiner::GetWSLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIV:
      case ExtremeI:
      case ExtremeII:
      case ServiceII:
      case ServiceIII:
      case Fatigue:
           lfCode = 0.00;
           break;

      case StrengthIII:
           lfCode = 1.40;
           break;

      case StrengthV:
           lfCode = 0.40;
           break;

      case ServiceI:
           lfCode = 0.30;
           break;
      }

   lf = GetUserLoadFactor(ws,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}


double LoadCombiner::GetWLLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthIV:
      case ExtremeI:
      case ExtremeII:
      case ServiceII:
      case ServiceIII:
      case Fatigue:
           lfCode = 0.00;
           break;

      case StrengthV:
           lfCode = 0.40;
           break;

      case ServiceI:
           lfCode = 0.30;
           break;
      }

   lf = GetUserLoadFactor(wl,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}


double LoadCombiner::GetFRLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthIV:
      case StrengthV:
      case ExtremeI:
      case ExtremeII:
      case ServiceI:
      case ServiceII:
      case ServiceIII:
           lfCode = 1.00;
           break;

      case Fatigue:
           lfCode = 0.00;
           break;
      }

   lf = GetUserLoadFactor(fr,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}


double LoadCombiner::GetTULoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthIV:
      case StrengthV:
           lfCode = ((comboMode == Force) ? 0.50 : 1.20);
           break;

      case ExtremeI:
      case ExtremeII:
           lfCode = 0.00;
           break;

      case ServiceI:
      case ServiceII:
      case ServiceIII:
           lfCode = ((comboMode == Force) ? 1.00 : 1.20);
           break;

      case Fatigue:
           lfCode = 0.00;
           break;
      }

   lf = GetUserLoadFactor(tu,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}


double LoadCombiner::GetCRLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthIV:
      case StrengthV:
           lfCode = ((comboMode == Force) ? 0.50 : 1.20);
           break;

      case ExtremeI:
      case ExtremeII:
           lfCode = 0.00;
           break;

      case ServiceI:
      case ServiceII:
      case ServiceIII:
           lfCode = ((comboMode == Force) ? 1.00 : 1.20);
           break;

      case Fatigue:
           lfCode = 0.00;
           break;
      }

   lf = GetUserLoadFactor(cr,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}


double LoadCombiner::GetSHLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthIV:
      case StrengthV:
           lfCode = ((comboMode == Force) ? 0.50 : 1.20);
           break;

      case ExtremeI:
      case ExtremeII:
           lfCode = 0.00;
           break;

      case ServiceI:
      case ServiceII:
      case ServiceIII:
           lfCode = ((comboMode == Force) ? 1.00 : 1.20);
           break;

      case Fatigue:
           lfCode = 0.00;
           break;
      }

   lf = GetUserLoadFactor(sh,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}


double LoadCombiner::GetTGLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthV:
      case ServiceI:
      case ServiceIII:
           lfCode = 0.00;
           break;

      case StrengthIV:
      case ExtremeI:
      case ExtremeII:
      case ServiceII:
      case Fatigue:
           lfCode = 0.00;
           break;
      }

   lf = GetUserLoadFactor(tg,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}


double LoadCombiner::GetSELoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthV:
      case ServiceI:
      case ServiceIII:
           lfCode = 0.00;
           break;

      case StrengthIV:
      case ExtremeI:
      case ExtremeII:
      case ServiceII:
      case Fatigue:
           lfCode = 0.00;
           break;
      }

   lf = GetUserLoadFactor(se,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}


double LoadCombiner::GetEQLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthIV:
      case StrengthV:
      case ExtremeII:
      case ServiceI:
      case ServiceII:
      case ServiceIII:
      case Fatigue:
           lfCode = 0.00;
           break;

      case ExtremeI:
           lfCode = 1.00;
           break;
      }

   lf = GetUserLoadFactor(eq,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}


double LoadCombiner::GetICLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthIV:
      case StrengthV:
      case ExtremeI:
      case ServiceI:
      case ServiceII:
      case ServiceIII:
      case Fatigue:
           lfCode = 0.00;
           break;

      case ExtremeII:
           lfCode = 1.00;
           break;
      }

   lf = GetUserLoadFactor(ic,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}


double LoadCombiner::GetCTLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthIV:
      case StrengthV:
      case ExtremeI:
      case ServiceI:
      case ServiceII:
      case ServiceIII:
      case Fatigue:
           lfCode = 0.00;
           break;

      case ExtremeII:
           lfCode = 1.00;
           break;
      }

   lf = GetUserLoadFactor(ct,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}


double LoadCombiner::GetCVLoadFactor(LoadCombiner::LimitState limitState) const
{
   double lf;
   double lfCode;

   switch (limitState)
      {
      case StrengthI:
      case StrengthII:
      case StrengthIII:
      case StrengthIV:
      case StrengthV:
      case ExtremeI:
      case ServiceI:
      case ServiceII:
      case ServiceIII:
      case Fatigue:
           lfCode = 0.00;
           break;

      case ExtremeII:
           lfCode = 1.00;
           break;
      }

   lf = GetUserLoadFactor(cv,limitState);
   lf = (lf < 0) ? lfCode : lf;

   return lf;
}

void LoadCombiner::SetLimitStateFactors(double _nd,double _nr,double _ni)
{
   nd = _nd;
   nr = _nr;
   ni = _ni;
}

void LoadCombiner::GetLimitStateFactors(double &_nd,double &_nr,double &_ni) const
{
   _nd = nd;
   _nr = nr;
   _ni = ni;
}

double LoadCombiner::CombineLoads(LoadCombiner::LimitState limitState)
{
   double value;

   switch(GetCodeEdition())
      {
      case lrfdFirstEdition94:
           value = CombineLoads_1stEdition94(limitState);
           break;

      case lrfdFirstEdition94_96Interim:
           value = CombineLoads_1stEdition94_96Interim(limitState);
           break;
      }

   return value;
}

double LoadCombiner::CombineLoads_1stEdition94(LoadCombiner::LimitState limitState)
{
   // Limit State Factor
   double n = LimitStateFactor(nd,nr,ni);
   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"Limit State Factor = " << n);


   // Load Factors
   double gDCmin = GetDCMinLoadFactor(limitState);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gDC min " << gDCmin);
   double gDCmax = GetDCMaxLoadFactor(limitState);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gDC max " << gDCmax);
   double gDDmin = GetDDMinLoadFactor(limitState);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gDD min " << gDDmin);
   double gDDmax = GetDDMaxLoadFactor(limitState);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gDD max " << gDDmax);
   double gDWmin = GetDWMinLoadFactor(limitState);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gDW min " << gDWmin);
   double gDWmax = GetDWMaxLoadFactor(limitState);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gDW max " << gDWmax);
   double gEHmin = GetEHMinLoadFactor(limitState);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gEH min " << gEHmin);
   double gEHmax = GetEHMaxLoadFactor(limitState);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gEH max " << gEHmax);
   double gEVmin = GetEVMinLoadFactor(limitState);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gEV min " << gEVmin);
   double gEVmax = GetEVMaxLoadFactor(limitState);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gEV max " << gEVmax);
   double gESmin = GetESMinLoadFactor(limitState);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gES min " << gESmin);
   double gESmax = GetESMaxLoadFactor(limitState);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gES max " << gESmax);
   double gLLIM = GetLLIMLoadFactor(limitState);     TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gLLIM   " << gLLIM);
   double gCE = GetCELoadFactor(limitState);         TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gCE     " << gCE);
   double gBR = GetBRLoadFactor(limitState);         TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gBR     " << gBR);
   double gPL = GetPLLoadFactor(limitState);         TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gPL     " << gPL);
   double gLS = GetLSLoadFactor(limitState);         TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gLS     " << gLS);
   double gWA = GetWALoadFactor(limitState);         TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gWA     " << gWA);
   double gWS = GetWSLoadFactor(limitState);         TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gWS     " << gWS);
   double gWL = GetWLLoadFactor(limitState);         TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gWL     " << gWL);
   double gFR = GetFRLoadFactor(limitState);         TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gFR     " << gFR);
   double gTU = GetTULoadFactor(limitState);         TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gTU     " << gTU);
   double gCR = GetCRLoadFactor(limitState);         TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gCR     " << gCR);
   double gSH = GetSHLoadFactor(limitState);         TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gSH     " << gSH);
   double gTG = GetTGLoadFactor(limitState);         TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gTG     " << gTG);
   double gSE = GetSELoadFactor(limitState);         TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gSE     " << gSE);
   double gEQ = GetEQLoadFactor(limitState);         TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gEQ     " << gEQ);
   double gIC = GetICLoadFactor(limitState);         TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gIC     " << gIC);
   double gCT = GetCTLoadFactor(limitState);         TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gCT     " << gCT);
   double gCV = GetCVLoadFactor(limitState);         TRACEX(LRFDCode_LoadCombiner,LEVEL3,"gCV     " << gCV);

   double Q; // Combined loads
   double qDC   = GetUserLoading(loadingDC);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qDC       " << qDC);
   double qDD   = GetUserLoading(loadingDD);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qDD       " << qDD);
   double qDW   = GetUserLoading(loadingDW);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qDW       " << qDW);
   double qEH   = GetUserLoading(loadingEH);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qEH       " << qEH);
   double qEV   = GetUserLoading(loadingEV);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qEV       " << qEV);
   double qES   = GetUserLoading(loadingES);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qES       " << qES);
   double qLLIM = GetUserLoading(loadingLLIM); TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qLLIM     " << qLLIM);
   double qCE   = GetUserLoading(loadingCE);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qCE       " << qCE);
   double qBR   = GetUserLoading(loadingBR);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qBR       " << qBR);
   double qPL   = GetUserLoading(loadingPL);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qPL       " << qPL);
   double qLS   = GetUserLoading(loadingLS);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qLS       " << qLS);
   double qWA   = GetUserLoading(loadingWA);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qWA       " << qWA);
   double qWS   = GetUserLoading(loadingWS);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qWS       " << qWS);
   double qWL   = GetUserLoading(loadingWL);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qWL       " << qWL);
   double qFR   = GetUserLoading(loadingFR);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qFR       " << qFR);
   double qTU   = GetUserLoading(loadingTU);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qTU       " << qTU);
   double qCR   = GetUserLoading(loadingCR);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qCR       " << qCR);
   double qSH   = GetUserLoading(loadingSH);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qSH       " << qSH);
   double qTG   = GetUserLoading(loadingTG);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qTG       " << qTG);
   double qSE   = GetUserLoading(loadingSE);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qSE       " << qSE);
   double qEQ   = GetUserLoading(loadingSE);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qEQ       " << qEQ);
   double qIC   = GetUserLoading(loadingIC);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qIC       " << qIC);
   double qCT   = GetUserLoading(loadingCT);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qCT       " << qCT);
   double qCV   = GetUserLoading(loadingCV);   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"qCV       " << qCV);

   // Combine all loads except for DC, DD, DW, EH, EV, ES, EQ, IC, CT, and CV
   Q =    (gLLIM*g*m*qLLIM)
            + (gCE * qCE)
            + (gBR * qBR)
            + (gPL * qPL)
            + (gLS * qLS)
            + (gWA * qWA)
            + (gWS * qWS)
            + (gWL * qWL)
            + (gFR * qFR)
            + (gTU * qTU)
            + (gCR * qCR)
            + (gSH * qSH)
            + (gTG * qTG)
            + (gSE * qSE);

   // Combine EQ, IC, CT, or CV
   switch(extremeMode)
      {
      case Earthquake:             Q += (gEQ * qEQ); break;
      case Ice:                    Q += (gIC * qIC); break;
      case VehicularCollision:     Q += (gCT * qCT); break;
      case VesselCollision:        Q += (gCV * qCV); break;
      }

   // Combine DC, DD, DW, EH, EV, and ES, accounting for min/max
   // load factors
   if (qDC < 0 && Q < 0 || qDC >= 0 && Q >= 0)
      qDC *= gDCmax;
   else
      qDC *= gDCmin;

   if (qDD < 0 && Q < 0 || qDD >= 0 && Q >= 0)
      qDD *= gDDmax;
   else
      qDD *= gDDmin;

   if (qDW < 0 && Q < 0 || qDW >= 0 && Q >= 0)
      qDW *= gDWmax;
   else
      qDW *= gDWmin;

   if (qEH < 0 && Q < 0 || qEH >= 0 && Q >= 0)
      qEH *= gEHmax;
   else
      qEH *= gEHmin;

   if (qEV < 0 && Q < 0 || qEV >= 0 && Q >= 0)
      qEV *= gEVmax;
   else
      qEV *= gEVmin;

   if (qES < 0 && Q < 0 || qES >= 0 && Q >= 0)
      qES *= gESmax;
   else
      qES *= gESmin;

   Q += qDC + qDD + qDW + qEH + qEV + qES;

   // Apply Limit State Factor
   Q *= n;

   return Q;
}

double LoadCombiner::CombineLoads_1stEdition94_96Interim(LoadCombiner::LimitState limitState)
{
   // Limit State Factor
   double nMax = LimitStateFactor(nd,nr,ni,1); // when max load factor used
   double nMin = LimitStateFactor(nd,nr,ni,0); // when min load factor used

   // Load Factors
   double gDCmin = GetDCMinLoadFactor(limitState);
   double gDCmax = GetDCMaxLoadFactor(limitState);
   double gDDmin = GetDDMinLoadFactor(limitState);
   double gDDmax = GetDDMaxLoadFactor(limitState);
   double gDWmin = GetDWMinLoadFactor(limitState);
   double gDWmax = GetDWMaxLoadFactor(limitState);
   double gEHmin = GetEHMinLoadFactor(limitState);
   double gEHmax = GetEHMaxLoadFactor(limitState);
   double gEVmin = GetEVMinLoadFactor(limitState);
   double gEVmax = GetEVMaxLoadFactor(limitState);
   double gESmin = GetESMinLoadFactor(limitState);
   double gESmax = GetESMaxLoadFactor(limitState);
   double gLLIM = GetLLIMLoadFactor(limitState);
   double gCE = GetCELoadFactor(limitState);
   double gBR = GetBRLoadFactor(limitState);
   double gPL = GetPLLoadFactor(limitState);
   double gLS = GetLSLoadFactor(limitState);
   double gWA = GetWALoadFactor(limitState);
   double gWS = GetWSLoadFactor(limitState);
   double gWL = GetWLLoadFactor(limitState);
   double gFR = GetFRLoadFactor(limitState);
   double gTU = GetTULoadFactor(limitState);
   double gCR = GetCRLoadFactor(limitState);
   double gSH = GetSHLoadFactor(limitState);
   double gTG = GetTGLoadFactor(limitState);
   double gSE = GetSELoadFactor(limitState);
   double gEQ = GetEQLoadFactor(limitState);
   double gIC = GetICLoadFactor(limitState);
   double gCT = GetCTLoadFactor(limitState);
   double gCV = GetCVLoadFactor(limitState);

   double Q; // Combined loads
   double qDC;
   double qDD;
   double qDW;
   double qEH;
   double qEV;
   double qES;

   // Combine all loads except for DC, DD, DW, EH, EV, ES, EQ, IC, CT, and CV
   Q = nMax * (gLLIM*g*m*GetUserLoading(loadingLLIM)
            + (gCE * GetUserLoading(loadingCE))
            + (gBR * GetUserLoading(loadingBR))
            + (gPL * GetUserLoading(loadingPL))
            + (gLS * GetUserLoading(loadingLS))
            + (gWA * GetUserLoading(loadingWA))
            + (gWS * GetUserLoading(loadingWS))
            + (gWL * GetUserLoading(loadingWL))
            + (gFR * GetUserLoading(loadingFR))
            + (gTU * GetUserLoading(loadingTU))
            + (gCR * GetUserLoading(loadingCR))
            + (gSH * GetUserLoading(loadingSH))
            + (gTG * GetUserLoading(loadingTG))
            + (gSE * GetUserLoading(loadingSE)));

   // Combine EQ, IC, CT, or CV
   switch(extremeMode)
      {
      case Earthquake:             Q += nMax*(gEQ * GetUserLoading(loadingEQ)); break;
      case Ice:                    Q += nMax*(gIC * GetUserLoading(loadingIC)); break;
      case VehicularCollision:     Q += nMax*(gCT * GetUserLoading(loadingCT)); break;
      case VesselCollision:        Q += nMax*(gCV * GetUserLoading(loadingCV)); break;
      }

   // Combine DC, DD, DW, EH, EV, and ES, accounting for min/max
   // load factors
   double DC = GetUserLoading(loadingDC);
   if (DC < 0 && Q < 0 || DC >= 0 && Q >= 0)
      qDC = nMax * gDCmax * DC;
   else
      qDC = nMin * gDCmin * DC;

   double DD = GetUserLoading(loadingDD);
   if (DD < 0 && Q < 0 || DD >= 0 && Q >= 0)
      qDD = nMax * gDDmax * DD;
   else
      qDD = nMin * gDDmin * DD;

   double DW = GetUserLoading(loadingDW);
   if (DW < 0 && Q < 0 || DW >= 0 && Q >= 0)
      qDW = nMax * gDWmax * DW;
   else
      qDW = nMin * gDWmin * DW;

   double EH = GetUserLoading(loadingEH);
   if (EH < 0 && Q < 0 || EH >= 0 && Q >= 0)
      qEH = nMax * gEHmax * EH;
   else
      qEH = nMin * gEHmin * EH;

   double EV = GetUserLoading(loadingEV);
   if (EV < 0 && Q < 0 || EV >= 0 && Q >= 0)
      qEV = nMax * gEVmax * EV;
   else
      qEV = nMin * gEVmin * EV;

   double ES = GetUserLoading(loadingES);
   if (ES < 0 && Q < 0 || ES >= 0 && Q >= 0)
      qES = nMax * gESmax * ES;
   else
      qES = nMin * gESmin * ES;

   Q += qDC + qDD + qDW + qEH + qEV + qES;

   return Q;
}

double LoadCombiner::GetUserLoadFactor(LoadFactor ldfact,LimitState ls) const
{
   TRACEX(LRFDCode_LoadCombiner,LEVEL1,"LoadCombiner::GetUserLoadFactor()");

   int idx;
   double value;
   LoadFactorRecord find;
   LoadFactorRecord *lfr;

   find.loadFactor = ldfact;
   find.limitState = ls;
   idx = loadFactors->Find(&find);

   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"idx = " << idx);

   if (idx == INT_MAX)
      {
      TRACEX(LRFDCode_LoadCombiner,LEVEL3,"No record found");
      value = -1;
      }
   else
      {
      TRACEX(LRFDCode_LoadCombiner,LEVEL3,"Record found");
      lfr = (*loadFactors)[idx];
      value = lfr->value;
      }

   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"value = " << value);

   return value;
}

double LoadCombiner::GetUserLoading(Loading loading) const
{
   TRACEX(LRFDCode_LoadCombiner,LEVEL1,"LoadCombiner::GetUserLoading()");

   int idx;
   double value;
   LoadingRecord find;
   LoadingRecord *lr;

   find.loading = loading;
   idx = loadings->Find(&find);

   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"idx = " << idx);

   if (idx == INT_MAX)
      {
      TRACEX(LRFDCode_LoadCombiner,LEVEL3,"No record found");
      value = 0.00;
      }
   else
      {
      TRACEX(LRFDCode_LoadCombiner,LEVEL3,"Record found");
      lr = (*loadings)[idx];
      value = lr->value;
      }

   TRACEX(LRFDCode_LoadCombiner,LEVEL3,"value = " << value);

   return value;
}

