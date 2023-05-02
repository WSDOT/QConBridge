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

#if !defined (COMBINE_H)
#define COMBINE_H

#include <code\lrfddll.h>

#if defined(__BORLANDC__)
#include <classlib\arrays.h>
#elif defined(_MSC_VER)
#include <afxtempl.h>
#else
# error Unknown compiler
#endif


// LoadCombiner
//
// Description:
// Performs LRFD Load Combinations
//
// Code Reference: 1st Edition 1994 Article 3.4
//
// Units: Consistant Units
class _CODECLASS LoadCombiner
{
public:
   LoadCombiner();
   virtual ~LoadCombiner();

   void Reset();

   // enumerations

   enum LimitState {StrengthI,StrengthII,StrengthIII,StrengthIV,StrengthV,
                    ExtremeI,ExtremeII,
                    ServiceI,ServiceII,ServiceIII,
                    Fatigue
                    };
   enum CombinationMode {Force,Deflection};
   enum HorizEarthPressureMode {Active, AtRest};
   enum VertEarthPressureMode {OverallStability,
                               RetainingStructure,
                               RigidBuriedStructure,
                               RigidFrame,
                               FlexibleBuriedStructure,
                               FlexibleMetalBoxCulverts};
   enum ExtremeLoad {Earthquake,Ice,VehicularCollision,VesselCollision};

   enum Loading  {loadingDC,  loadingDD,loadingDW,loadingEH,loadingEV,loadingES,
                  loadingLLIM,loadingCE,loadingBR,loadingPL,loadingLS,loadingWA,
                  loadingWS,  loadingWL,loadingFR,loadingTU,loadingCR,loadingSH,
                  loadingTG,  loadingSE,loadingEQ,loadingIC,loadingCT,loadingCV};

   enum LoadFactor {dcMin,dcMax,ddMin,ddMax,dwMin,dwMax,ehMin,ehMax,
                    evMin,evMax,esMin,esMax,llim,ce,br,pl,ls,wa,ws,wl,fr,tu,
                    cr,sh,tg,se,eq,ic,ct,cv};

   // Modes and Options
   void SetCombinationMode(CombinationMode mode);
   LoadCombiner::CombinationMode GetCombinationMode() const;

   void SetHorizEarthPressureMode(HorizEarthPressureMode mode);
   LoadCombiner::HorizEarthPressureMode GetHorizEarthPressureMode() const;

   void SetVertEarthPressureMode(VertEarthPressureMode mode);
   LoadCombiner::VertEarthPressureMode GetVertEarthPressureMode() const;

   void SetExtremeLoad(ExtremeLoad l);
   LoadCombiner::ExtremeLoad GetExtremeLoad() const;

   void SetLiveloadDistFactor(double lldf);
   double GetLiveLoadDistFactor() const;

   void SetMultiplePresenceFactor(double mpf);
   double GetMultiplePresenceFactor() const;

   void   SetLoading(Loading loading,double value);
   double GetLoading(Loading ld) const;
   void   SetLoadFactor(LoadFactor ldfact,LimitState ls,double value);
   double GetLoadFactor(LoadFactor ldfact,LimitState ls) const;

   void SetLimitStateFactors(double _nd,double _nr,double _ni);
   void GetLimitStateFactors(double &_nd,double &_nr,double &_ni) const;

   double CombineLoads(LimitState ls);

protected:
   double nd; // limit state factor for ductility
   double nr; // limit state factor for redundancy
   double ni; // limit state factor for importance

   double g; // live load distribution factor
   double m; // multiple presence factor

   // Modes
   CombinationMode comboMode;
   HorizEarthPressureMode EHmode;
   VertEarthPressureMode EVmode;
   ExtremeLoad extremeMode;

   struct LoadingRecord
      {
      Loading loading;
      double value;
      int operator == (const LoadingRecord &lr) const
         {return (loading == lr.loading);}
      };

   struct LoadFactorRecord
      {
      LoadFactor loadFactor;
      LimitState limitState;
      double value;
      int operator == (const LoadFactorRecord &lfr) const
         {return ((loadFactor == lfr.loadFactor) &&
                  (limitState == lfr.limitState));}
      };

   // If arrays prove to be too slow for searching, use something else.
   // Arrays are easy to implement and if they are fast enough, then great.

#if defined (__BORLANDC__)
   typedef TIArrayAsVector<LoadCombiner::LoadingRecord>         LoadingContainer;
   typedef TIArrayAsVectorIterator<LoadCombiner::LoadingRecord> LoadingIterator;
   typedef TIArrayAsVector<LoadCombiner::LoadFactorRecord>         LoadFactorContainer;
   typedef TIArrayAsVectorIterator<LoadCombiner::LoadFactorRecord> LoadFactorIterator;
#else
   typedef CTypedPtrArray<CPtrArray,LoadCombiner::LoadingRecord*> LoadingContainer;
   typedef CTypedPtrArray<CPtrArray,LoadCombiner::LoadFactorRecord*> LoadFactorContainer;
#endif

   LoadingContainer *loadings;
   LoadFactorContainer *loadFactors;

private:
   // Loads
   double GetDC() const;
   double GetDD() const;
   double GetDW() const;
   double GetEH() const;
   double GetEV() const;
   double GetES() const;
   double GetLLIM() const;
   double GetCE() const;
   double GetBR() const;
   double GetPL() const;
   double GetLS() const;
   double GetWA() const;
   double GetWS() const;
   double GetWL() const;
   double GetFR() const;
   double GetTU() const;
   double GetCR() const;
   double GetSH() const;
   double GetTG() const;
   double GetSE() const;
   double GetEQ() const;
   double GetIC() const;
   double GetCT() const;
   double GetCV() const;

   double GetUserLoading(Loading ld) const;

   // Load Factors
   // Get returns actual load factor used. If user supplies a lf of -1 or
   // does not define a lf (default), then the code value is used.
   void   SetDCMinLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetDCMinLoadFactor(LoadCombiner::LimitState limitState) const;
   void   SetDCMaxLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetDCMaxLoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetDDMinLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetDDMinLoadFactor(LoadCombiner::LimitState limitState) const;
   void   SetDDMaxLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetDDMaxLoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetDWMinLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetDWMinLoadFactor(LoadCombiner::LimitState limitState) const;
   void   SetDWMaxLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetDWMaxLoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetEHMinLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetEHMinLoadFactor(LoadCombiner::LimitState limitState) const;
   void   SetEHMaxLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetEHMaxLoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetEVMinLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetEVMinLoadFactor(LoadCombiner::LimitState limitState) const;
   void   SetEVMaxLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetEVMaxLoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetESMinLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetESMinLoadFactor(LoadCombiner::LimitState limitState) const;
   void   SetESMaxLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetESMaxLoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetLLIMLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetLLIMLoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetCELoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetCELoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetBRLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetBRLoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetPLLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetPLLoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetLSLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetLSLoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetWALoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetWALoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetWSLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetWSLoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetWLLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetWLLoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetFRLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetFRLoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetTULoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetTULoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetCRLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetCRLoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetSHLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetSHLoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetTGLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetTGLoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetSELoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetSELoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetEQLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetEQLoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetICLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetICLoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetCTLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetCTLoadFactor(LoadCombiner::LimitState limitState) const;

   void   SetCVLoadFactor(LoadCombiner::LimitState limitState,double lf);
   double GetCVLoadFactor(LoadCombiner::LimitState limitState) const;

   double GetUserLoadFactor(LoadFactor ldfact,LimitState ls) const;

   double CombineLoads_1stEdition94(LoadCombiner::LimitState limitState);
   double CombineLoads_1stEdition94_96Interim(LoadCombiner::LimitState limitState);
};


//
// inline functions --------------------------------------------------
//

inline void   LoadCombiner::SetLiveloadDistFactor(double lldf) {g = lldf;}
inline double LoadCombiner::GetLiveLoadDistFactor() const {return g;}

inline void   LoadCombiner::SetMultiplePresenceFactor(double mpf) {m = mpf;}
inline double LoadCombiner::GetMultiplePresenceFactor() const {return m;}


// Get Loads
inline double LoadCombiner::GetDC()   const {return GetUserLoading(loadingDC);}
inline double LoadCombiner::GetDD()   const {return GetUserLoading(loadingDD);}
inline double LoadCombiner::GetDW()   const {return GetUserLoading(loadingDW);}
inline double LoadCombiner::GetEH()   const {return GetUserLoading(loadingEH);}
inline double LoadCombiner::GetEV()   const {return GetUserLoading(loadingEV);}
inline double LoadCombiner::GetES()   const {return GetUserLoading(loadingES);}
inline double LoadCombiner::GetLLIM() const {return GetUserLoading(loadingLLIM);}
inline double LoadCombiner::GetCE()   const {return GetUserLoading(loadingCE);}
inline double LoadCombiner::GetBR()   const {return GetUserLoading(loadingBR);}
inline double LoadCombiner::GetPL()   const {return GetUserLoading(loadingPL);}
inline double LoadCombiner::GetLS()   const {return GetUserLoading(loadingLS);}
inline double LoadCombiner::GetWA()   const {return GetUserLoading(loadingWA);}
inline double LoadCombiner::GetWS()   const {return GetUserLoading(loadingWS);}
inline double LoadCombiner::GetWL()   const {return GetUserLoading(loadingWL);}
inline double LoadCombiner::GetFR()   const {return GetUserLoading(loadingFR);}
inline double LoadCombiner::GetTU()   const {return GetUserLoading(loadingTU);}
inline double LoadCombiner::GetCR()   const {return GetUserLoading(loadingCR);}
inline double LoadCombiner::GetSH()   const {return GetUserLoading(loadingSH);}
inline double LoadCombiner::GetTG()   const {return GetUserLoading(loadingTG);}
inline double LoadCombiner::GetSE()   const {return GetUserLoading(loadingSE);}
inline double LoadCombiner::GetEQ()   const {return GetUserLoading(loadingEQ);}
inline double LoadCombiner::GetIC()   const {return GetUserLoading(loadingIC);}
inline double LoadCombiner::GetCT()   const {return GetUserLoading(loadingCT);}
inline double LoadCombiner::GetCV()   const {return GetUserLoading(loadingCV);}

// Set Load Factors

inline void LoadCombiner::SetDCMinLoadFactor(LoadCombiner::LimitState limitState,double lf) {SetLoadFactor(dcMin,limitState,lf);}
inline void LoadCombiner::SetDCMaxLoadFactor(LoadCombiner::LimitState limitState,double lf) {SetLoadFactor(dcMax,limitState,lf);}
inline void LoadCombiner::SetDDMinLoadFactor(LoadCombiner::LimitState limitState,double lf) {SetLoadFactor(ddMin,limitState,lf);}
inline void LoadCombiner::SetDDMaxLoadFactor(LoadCombiner::LimitState limitState,double lf) {SetLoadFactor(ddMax,limitState,lf);}
inline void LoadCombiner::SetDWMinLoadFactor(LoadCombiner::LimitState limitState,double lf) {SetLoadFactor(dwMin,limitState,lf);}
inline void LoadCombiner::SetDWMaxLoadFactor(LoadCombiner::LimitState limitState,double lf) {SetLoadFactor(dwMax,limitState,lf);}
inline void LoadCombiner::SetEHMinLoadFactor(LoadCombiner::LimitState limitState,double lf) {SetLoadFactor(ehMin,limitState,lf);}
inline void LoadCombiner::SetEHMaxLoadFactor(LoadCombiner::LimitState limitState,double lf) {SetLoadFactor(ehMax,limitState,lf);}
inline void LoadCombiner::SetEVMinLoadFactor(LoadCombiner::LimitState limitState,double lf) {SetLoadFactor(evMin,limitState,lf);}
inline void LoadCombiner::SetEVMaxLoadFactor(LoadCombiner::LimitState limitState,double lf) {SetLoadFactor(evMax,limitState,lf);}
inline void LoadCombiner::SetESMinLoadFactor(LoadCombiner::LimitState limitState,double lf) {SetLoadFactor(esMin,limitState,lf);}
inline void LoadCombiner::SetESMaxLoadFactor(LoadCombiner::LimitState limitState,double lf) {SetLoadFactor(esMax,limitState,lf);}
inline void LoadCombiner::SetLLIMLoadFactor(LoadCombiner::LimitState limitState,double lf)  {SetLoadFactor(llim,limitState,lf);}
inline void LoadCombiner::SetCELoadFactor(LoadCombiner::LimitState limitState,double lf)    {SetLoadFactor(ce,limitState,lf);}
inline void LoadCombiner::SetBRLoadFactor(LoadCombiner::LimitState limitState,double lf)    {SetLoadFactor(br,limitState,lf);}
inline void LoadCombiner::SetPLLoadFactor(LoadCombiner::LimitState limitState,double lf)    {SetLoadFactor(pl,limitState,lf);}
inline void LoadCombiner::SetLSLoadFactor(LoadCombiner::LimitState limitState,double lf)    {SetLoadFactor(ls,limitState,lf);}
inline void LoadCombiner::SetWALoadFactor(LoadCombiner::LimitState limitState,double lf)    {SetLoadFactor(wa,limitState,lf);}
inline void LoadCombiner::SetWSLoadFactor(LoadCombiner::LimitState limitState,double lf)    {SetLoadFactor(ws,limitState,lf);}
inline void LoadCombiner::SetWLLoadFactor(LoadCombiner::LimitState limitState,double lf)    {SetLoadFactor(wl,limitState,lf);}
inline void LoadCombiner::SetFRLoadFactor(LoadCombiner::LimitState limitState,double lf)    {SetLoadFactor(fr,limitState,lf);}
inline void LoadCombiner::SetTULoadFactor(LoadCombiner::LimitState limitState,double lf)    {SetLoadFactor(tu,limitState,lf);}
inline void LoadCombiner::SetCRLoadFactor(LoadCombiner::LimitState limitState,double lf)    {SetLoadFactor(cr,limitState,lf);}
inline void LoadCombiner::SetSHLoadFactor(LoadCombiner::LimitState limitState,double lf)    {SetLoadFactor(sh,limitState,lf);}
inline void LoadCombiner::SetTGLoadFactor(LoadCombiner::LimitState limitState,double lf)    {SetLoadFactor(tg,limitState,lf);}
inline void LoadCombiner::SetSELoadFactor(LoadCombiner::LimitState limitState,double lf)    {SetLoadFactor(se,limitState,lf);}
inline void LoadCombiner::SetEQLoadFactor(LoadCombiner::LimitState limitState,double lf)    {SetLoadFactor(eq,limitState,lf);}
inline void LoadCombiner::SetICLoadFactor(LoadCombiner::LimitState limitState,double lf)    {SetLoadFactor(ic,limitState,lf);}
inline void LoadCombiner::SetCTLoadFactor(LoadCombiner::LimitState limitState,double lf)    {SetLoadFactor(ct,limitState,lf);}
inline void LoadCombiner::SetCVLoadFactor(LoadCombiner::LimitState limitState,double lf)    {SetLoadFactor(cv,limitState,lf);}

#endif // COMBINE_H
