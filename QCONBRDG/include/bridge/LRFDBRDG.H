///////////////////////////////////////////////////////////////////////
// Bridge32 - Bridge Modeling Framework
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

#if !defined (LRFDBRDG_H_)
#define LRFDBRDG_H_

#include <bridge\brdgdll.h>
#include <bridge\bridge.h>
#include <bridge\truck.h>
#include <fem2d\femall.h> // for MbrFace
#include <classlib\arrays.h>

#define DW_LOAD   1
#define DC_LOAD   2

#define TANDEM       0
#define VARTRUCK     1
#define DUALTRUCKS   2
#define DUALTANDEMS  3
#define FATIGUE      4
#define PEDESTRIAN   5
#define ALL          6
#define STRENGTH_I   7
#define SERVICE_I    8
#define SERVICE_II   9
#define SERVICE_III 10
#define FATIGUE_LS  11

#define MOMENT 0
#define SHEAR  1

#define MAX    0
#define MIN    1

enum DiagramCatagory {dcDC,dcDW,dcAll,dcDesignTandem,dcDesignTruck,
                      dcDualTruckTrain,dcDualTandemTrain,dcFatigueTruck,dcPedestrian,
                      dcStrengthI,dcServiceI,dcServiceII,dcServiceIII,
                      dcFatigue};
enum DiagramType {dtMoment,dtShear,dtReaction,dtMinMoment,dtMaxMoment,dtMinShear,dtMaxShear,dtMinReaction,dtMaxReaction,
					   rtFx, rtFy, rtMz, rtFxMin,rtFxMax,rtFyMin,rtFyMax,rtMzMin,rtMzMax};

class _BRIDGECLASS LRFDBridge : public Bridge
{
public:
   LRFDBridge();
   ~LRFDBridge();

   void Clear(bool init = true);

   // Typical Bridge Loadings
   void SelfWgtEnabled(bool isEnabled);
   bool SelfWgtEnabled() const;
   void TrafficBarrierEnabled(bool isEnabled);
   bool TrafficBarrierEnabled() const;
   void WearSurfEnabled(bool isEnabled);
   bool WearSurfEnabled() const;
   void UtilityEnabled(bool isEnabled);
   bool UtilityEnabled() const;
   void MiscDCEnabled(bool isEnabled);
   bool MiscDCEnabled() const;
   void MiscDWEnabled(bool isEnabled);
   bool MiscDWEnabled() const;

   void   TrafficBarrierLd(double ld);
   double TrafficBarrierLd() const;
   void   WearSurfLd(double ld);
   double WearSurfLd() const;
   void   UtilityLd(double ld);
   double UtilityLd() const;

   void   PedestrianLd(double ld) {pedestrianLd = ld;}
   double PedestrianLd() const    {return pedestrianLd;}

   // Live Load Generation
   bool   DualTruckLoadEnabled(int train) const;
   void   DualTruckLoadEnabled(int train,bool enable);
   bool   DualTruckRangeEnabled(int train,int range);
   void   DualTruckRangeEnabled(int train,int range,bool enable);
   double FromHeadwaySpacing(int train,int range);
   void   FromHeadwaySpacing(int train,int range,double from);
   double ToHeadwaySpacing(int train,int range);
   void   ToHeadwaySpacing(int train,int range,double to);
   int    NumHeadwayInc(int train,int range);
   void   NumHeadwayInc(int train,int range,int inc);

   double GetTruckImpact()   const    {return IMtruck;}
   void   SetTruckImpact(double im)   {IMtruck = im;}
   double GetLaneImpact()    const    {return IMlane;}
   void   SetLaneImpact(double im)    {IMlane = im;}
   double GetFatigueImpact() const    {return IMfatigue;}
   void   SetFatigueImpact(double im) {IMfatigue = im;}

   void EnableDesignTandem(bool enable) {enableDesignTandem = enable;}
   bool EnableDesignTandem() const      {return enableDesignTandem;}
   void EnableDesignTruck(bool enable)  {enableDesignTruck = enable;}
   bool EnableDesignTruck() const       {return enableDesignTruck;}
   void EnableFatigueTruck(bool enable) {enableFatigueTruck = enable;}
   bool EnableFatigueTruck() const      {return enableFatigueTruck;}
   void SetNumAxleInc(int num) {numAxleInc = num;}
   int  GetNumAxleInc() const  {return numAxleInc;}

   // LRFD Code Parameters for combinations
   void SetStrengthILoadFactors(double _dcMin,double _dcMax,double _dwMin,double _dwMax,double _ll);
   void GetStrengthILoadFactors(double &_dcMin,double &_dcMax,double &_dwMin,double &_dwMax,double &_ll);
   void SetServiceILoadFactors(double _dc,double _dw,double _ll);
   void GetServiceILoadFactors(double &_dc,double &_dw,double &_ll);
   void SetServiceIILoadFactors(double _dc,double _dw,double _ll);
   void GetServiceIILoadFactors(double &_dc,double &_dw,double &_ll);
   void SetServiceIIILoadFactors(double _dc,double _dw,double _ll);
   void GetServiceIIILoadFactors(double &_dc,double &_dw,double &_ll);
   void SetFatigueLoadFactors(double _dc,double _dw,double _ll);
   void GetFatigueLoadFactors(double &_dc,double &_dw,double &_ll);

   // Analysis
   bool Analyze(ProgressIndicator *pi);

   // Results
   int GetDiagramId(DiagramCatagory c,DiagramType t);
   int GetAnalysisResultsId(DiagramCatagory c,DiagramType t);

   double GetResult(int spanId,int spanPnt,MbrFace face,DiagramCatagory c,DiagramType t);
   double GetResult(int poiId,MbrFace face,DiagramCatagory c,DiagramType t);

   double GetReaction(int pierId,DiagramCatagory c,DiagramType t);

protected:
   // Typical Bridge Loads
   int selfWgtId;
   int trafficBarrierId;
   int wearSurfId;
   int utilityId;
   int dcId;
   int dwId;
   int unitLoadId;
   int unitLoadShearResultsId;
   int unitLoadMomentResultsId;
   int unitLoadReactionResultsId;

   bool selfWgtEnabled;
   bool trafficBarrierEnabled;
   bool wearSurfEnabled;
   bool utilityEnabled;

   double trafficBarrierLd;
   double wearSurfLd;
   double utilityLd;

   double pedestrianLd;

   // Live Load Generation Parameters
   struct LiveloadGenerationParameters
      {
      bool   enableGeneration;
      bool   rangeEnabled[3];
      double fromHeadwaySpacing[3];
      double toHeadwaySpacing[3];
      int    numHeadwayInc[3];
      } dualTruckTrain, dualTandemTrain;

   int    numAxleInc;
   bool enableDesignTandem;
   bool enableDesignTruck;
   bool enableFatigueTruck;

   // Dynamic Load Allowance Factors
   double IMtruck;
   double IMlane;
   double IMfatigue;

   // Load Factors
   double StrengthI_dcMin;
   double StrengthI_dcMax;
   double StrengthI_dwMin;
   double StrengthI_dwMax;
   double StrengthI_ll;

   double ServiceI_dc;
   double ServiceI_dw;
   double ServiceI_ll;

   double ServiceII_dc;
   double ServiceII_dw;
   double ServiceII_ll;

   double ServiceIII_dc;
   double ServiceIII_dw;
   double ServiceIII_ll;

   double Fatigue_dc;
   double Fatigue_dw;
   double Fatigue_ll;

   // Diagram tracking information
   struct DiagramId
      {
      int id;            // look-up key
      DiagramCatagory c; // load case
      DiagramType t;     // load type
      bool operator == (const DiagramId &id) const {return (c == id.c && t == id.t);}
      };
   typedef TIArrayAsVector<DiagramId> DiagramIdContainer;
   typedef TIArrayAsVectorIterator<DiagramId> DiagramIdIterator;
   DiagramIdContainer *diagramIds;
   void SetDiagramId(DiagramCatagory c,DiagramType t,int id);

   // Results tracking information
   struct AnalysisResultsId
      {
      int id;            // look-up key
      DiagramCatagory c; // load case
      DiagramType t;     // load type
      bool operator == (const AnalysisResultsId &id) const {return (c == id.c && t == id.t);}
      };
   typedef TIArrayAsVector<AnalysisResultsId> AnalysisResultsIdContainer;
   typedef TIArrayAsVectorIterator<AnalysisResultsId> AnalysisResultsIdIterator;
   AnalysisResultsIdContainer *analysisResultsIds;
   void SetAnalysisResultsId(DiagramCatagory c,DiagramType t,int id);

   // Influence loading tracking information
   //
   // Tracks the relationship between a point on the bridge and a femModel
   // loading
   struct InfluenceLoadRecord
      {
      InfluenceLoadRecord(int poiId,int loadId) : poiId(poiId),loadId(loadId) {}
      int poiId;   // unit load placed at this poi
      int loadId;  // corrosponding fem load id

      bool operator == (const InfluenceLoadRecord &ilr) const {return poiId == ilr.poiId;}
      };
   typedef TIArrayAsVector<InfluenceLoadRecord> InfluenceLoadRecordContainer;
   typedef TIArrayAsVectorIterator<InfluenceLoadRecord> InfluenceLoadRecordIterator;
   InfluenceLoadRecordContainer *inflLoadRecords;

   // Influence results tracking information
   //
   // Tracks the relationship between a point on the bridge and the
   // id key used to retreive the analysis results (influence values).
   struct InfluenceResultsRecord
      {
      InfluenceResultsRecord(int poiId,int arMomentId,int arShearId) :
                             poiId(poiId),arMomentId(arMomentId),arShearId(arShearId)
                             {}
      int poiId; // poi for which these influence results apply
      int arMomentId; // analysis results id for moment influence
      int arShearId; // analysis results id for shear influence

      bool operator == (const InfluenceResultsRecord &irr) const {return poiId == irr.poiId;}
      };
   typedef TIArrayAsVector<InfluenceResultsRecord> InfluenceResultsRecordContainer;
   typedef TIArrayAsVectorIterator<InfluenceResultsRecord> InfluenceResultsRecordIterator;
   InfluenceResultsRecordContainer *inflResultsRecords;

   struct InfluenceReactionRecord
   {
   	InfluenceReactionRecord(int supportId,int arFxId,int arFyId,int arMzId) :
      	supportId(supportId),arFxId(arFxId),arFyId(arFyId),arMzId(arMzId)
         {}

      int supportId; // support for which these influence results apply
      int arFxId; // analysis results id for Fx reaction influence
      int arFyId; // analysis results id for Fy reaction influence
      int arMzId; // analysis results id for Mz reaction influence
      bool operator == (const InfluenceReactionRecord& irr) const { return supportId == irr.supportId; }
   };
   typedef TIArrayAsVector<InfluenceReactionRecord> InfluenceReactionRecordContainer;
   typedef TIArrayAsVectorIterator<InfluenceReactionRecord> InfluenceReactionRecordIterator;
   InfluenceReactionRecordContainer *inflReactionRecords;

   // Influence line record
   //
   // Holds influence line information used for live load evaluation. This
   // is temporary information that is created just before live load analysis
   // and is destroyed just after it is complete.
   struct InfluenceLineRecord
      {
      InfluenceLineRecord(int poiId,int spanId,double lOffset,
                          InfluenceLine *lfMoment,InfluenceLine *rfMoment,
                          InfluenceLine *lfShear,InfluenceLine *rfShear) :
                          poiId(poiId),spanId(spanId),lOffset(lOffset),
                          lfMoment(lfMoment),rfMoment(rfMoment),
                          lfShear(lfShear), rfShear(rfShear)
                          {}
      ~InfluenceLineRecord() {delete lfMoment; delete rfMoment; delete lfShear; delete rfShear;}
      int poiId; // poi for which these influence lines apply
      int spanId; // span id
      double lOffset; // local offset the the poi
      InfluenceLine *lfMoment; // left face moment influece line
      InfluenceLine *rfMoment; // right face moment influece line
      InfluenceLine *lfShear;  // left face shear influence line
      InfluenceLine *rfShear;  // right face shear influence line

      bool operator == (const InfluenceLineRecord &ilr) const {return poiId == ilr.poiId;}
      };
   typedef TIArrayAsVector<InfluenceLineRecord> InfluenceLineRecordContainer;
   typedef TIArrayAsVectorIterator<InfluenceLineRecord> InfluenceLineRecordIterator;

   struct ReactionInfluenceLineRecord
      {
      ReactionInfluenceLineRecord(int supportId,
                          InfluenceLine *ilFx,InfluenceLine *ilFy,InfluenceLine *ilMz) :
                          supportId(supportId),
                          ilFx(ilFx),ilFy(ilFy),ilMz(ilMz)
                          {}
      ~ReactionInfluenceLineRecord() {delete ilFx; delete ilFy; delete ilMz;}
      int supportId; // support for which these influence lines apply
      InfluenceLine *ilFx; // influence for Fx
      InfluenceLine *ilFy; // influence for Fy
      InfluenceLine *ilMz; // influence for Mz

      bool operator == (const ReactionInfluenceLineRecord &ilr) const {return supportId == ilr.supportId;}
      };
   typedef TIArrayAsVector<ReactionInfluenceLineRecord> ReactionInfluenceLineRecordContainer;
   typedef TIArrayAsVectorIterator<ReactionInfluenceLineRecord> ReactionInfluenceLineRecordIterator;

   // Contraflexure Points
   class UnitLoadMomentZone
      {
      public:
      UnitLoadMomentZone(int spanId);
      void SetContraflexurePoint(double offset,int leftSign,int rightSign);
      bool NegMomentRegion(double offset,MbrFace face);

      protected:
      int spanId;
      int nPoints;
      double loc[2];
      int leftSign[2];
      int rightSign[2];

      public:
      bool operator == (const UnitLoadMomentZone &ulmz) const {return spanId == ulmz.spanId;}
      friend LRFDBridge;
      friend UnitLoadMomentZone;
      };
   typedef TIArrayAsVector<UnitLoadMomentZone> UnitLoadMomentZoneContainer;
   typedef TIArrayAsVectorIterator<UnitLoadMomentZone> UnitLoadMomentZoneIterator;
   UnitLoadMomentZoneContainer *ulMomentZones;

protected:
   // Typical Bridge Loads
   void SelfWgtId(int id);
   int  SelfWgtId() const;
   void TrafficBarrierId(int id);
   int  TrafficBarrierId() const;
   void WearSurfId(int id);
   int  WearSurfId() const;
   void UtilityId(int id);
   int  UtilityId() const;

   // Analysis Methods

   // Virtuals from Bridge (Modeling)
   bool GenerateModel();
   bool ApplyLoadings();
   bool GeneratePOI();

   bool ApplyUnitLoad();
   bool ApplySuperimposedLoads();
   bool GenerateInfluenceLoadings();
   bool CombineDeadLoads();
   bool FindContraflexurePoints();

   // Live Load Analysis
   bool GenerateLiveloadEnvelopes();
   bool CreateEnvelope(DiagramCatagory dc,
                       const char *desc,
                       TruckTrainContainer *trains,
                       InfluenceLineRecordContainer *il,
                       ReactionInfluenceLineRecordContainer* ilReaction,
                       bool envMinMoment = true,
                       bool envMaxMoment = true,
                       bool envMinShear  = true,
                       bool envMaxShear  = true,
                       bool envMinReaction = true,
                       bool envMaxReaction = true);

   bool CreateDesignTandemEnvelope(InfluenceLineRecordContainer *il,ReactionInfluenceLineRecordContainer* ilReactions);
   bool CreateDesignTruckEnvelope(InfluenceLineRecordContainer *il,ReactionInfluenceLineRecordContainer* ilReactions);
   bool CreateDualTruckEnvelope(InfluenceLineRecordContainer *il,ReactionInfluenceLineRecordContainer* ilReactions);
   bool CreateDualTandemEnvelope(InfluenceLineRecordContainer *il,ReactionInfluenceLineRecordContainer* ilReactions);
   bool CreateLiveLoadEnvelope();
   bool CreateFatigueTruckEnvelope(InfluenceLineRecordContainer *il,ReactionInfluenceLineRecordContainer* ilReactions);
   bool CreatePedestrianEnvelope(InfluenceLineRecordContainer *il,ReactionInfluenceLineRecordContainer* ilReactions);

   // Results
   void StoreAnalysisResults(DiagramCatagory c,const char *desc,int loadId);
   void StoreAnalysisResults(int shearId,int momentId,int reactionId,const char *desc,int loadId);
   bool CreateInfluenceLines();
   bool CreateSuperstructureInfluenceLines();
   bool CreateReactionInfluenceLines();
   bool CreateLoadCombinations();

   // Misc
   void Init();
   Diagram *CreateDiagram(DiagramCatagory dc,DiagramType dt,int id);
   void StoreAnalysisResultsAsDiagrams(DiagramCatagory dc,DiagramType dt);

private:
   // Live Load Generation "Helper" functions
   InfluenceLineRecordContainer *CreateInfluenceLineRecords();
   ReactionInfluenceLineRecordContainer *CreateReactionInfluenceLineRecords();
   void AllocateEnvelopes(DiagramCatagory dc,const char *desc);
   void InitializeEnvelopes(AnalysisResults *maxMomentEnv,
                            AnalysisResults *minMomentEnv,
                            AnalysisResults *maxShearEnv,
                            AnalysisResults *minShearEnv,
                            AnalysisResults *maxReactionEnv,
                            AnalysisResults *minReactionEnv);
   void InitializeEnvelope(AnalysisResults *envelope,double value);
   void EnvelopeMaxPoint(TruckTrain *train,InfluenceLine *lfInflLine,InfluenceLine *rfInflLine,
                          double &lfValue,double &rfValue);
   void EnvelopeMinPoint(TruckTrain *train,InfluenceLine *lfInflLine,InfluenceLine *rfInflLine,
                          double &lfValue,double &rfValue);
	void EnvelopeMaxPoint(TruckTrain *train,InfluenceLine *ilFx,InfluenceLine *ilFy,InfluenceLine *ilMz,
                          double &Fx,double &Fy,double &Mz);
	void EnvelopeMinPoint(TruckTrain *train,InfluenceLine *ilFx,InfluenceLine *ilFy,InfluenceLine *ilMz,
                          double &Fx,double &Fy,double &Mz);

   // Unit Loads
   bool UnitLoadNegMomentRegion(int spanId,double offset,MbrFace face = mbrLeftFace);

   // POI
   bool ContraflexurePOI();

#if defined GEN_DEBUG_CODE
   void DumpContraflexurePoints();
   void DumpResults();
   void DumpDiagrams();
#endif
};

//
// inline methods
//

inline void LRFDBridge::SelfWgtEnabled(bool isEnabled)
{
   selfWgtEnabled = isEnabled;
}


inline bool LRFDBridge::SelfWgtEnabled() const
{
   return selfWgtEnabled;
}


inline void LRFDBridge::TrafficBarrierEnabled(bool isEnabled)
{
   trafficBarrierEnabled = isEnabled;
}


inline bool LRFDBridge::TrafficBarrierEnabled() const
{
   return trafficBarrierEnabled;
}


inline void LRFDBridge::WearSurfEnabled(bool isEnabled)
{
   wearSurfEnabled = isEnabled;
}


inline bool LRFDBridge::WearSurfEnabled() const
{
   return wearSurfEnabled;
}


inline void LRFDBridge::UtilityEnabled(bool isEnabled)
{
   utilityEnabled = isEnabled;
}


inline bool LRFDBridge::UtilityEnabled() const
{
   return utilityEnabled;
}


inline void LRFDBridge::TrafficBarrierLd(double ld)
{
   trafficBarrierLd = ld;
}


inline double LRFDBridge::TrafficBarrierLd() const
{
   return trafficBarrierLd;
}


inline void LRFDBridge::WearSurfLd(double ld)
{
   wearSurfLd = ld;
}


inline double LRFDBridge::WearSurfLd() const
{
   return wearSurfLd;
}


inline void LRFDBridge::UtilityLd(double ld)
{
   utilityLd = ld;
}


inline double LRFDBridge::UtilityLd() const
{
   return utilityLd;
}


inline void LRFDBridge::SelfWgtId(int id)
{
   selfWgtId = id;
}


inline int  LRFDBridge::SelfWgtId() const
{
   return selfWgtId;
}


inline void LRFDBridge::TrafficBarrierId(int id)
{
   trafficBarrierId = id;
}


inline int  LRFDBridge::TrafficBarrierId() const
{
   return trafficBarrierId;
}


inline void LRFDBridge::WearSurfId(int id)
{
   wearSurfId = id;
}


inline int  LRFDBridge::WearSurfId() const
{
   return wearSurfId;
}


inline void LRFDBridge::UtilityId(int id)
{
   utilityId = id;
}


inline int  LRFDBridge::UtilityId() const
{
   return utilityId;
}

inline void LRFDBridge::SetStrengthILoadFactors(double _dcMin,double _dcMax,double _dwMin,double _dwMax,double _ll)
   {
   StrengthI_dcMin = _dcMin;
   StrengthI_dcMax = _dcMax;
   StrengthI_dwMin = _dwMin;
   StrengthI_dwMax = _dwMax;
   StrengthI_ll    = _ll;
   }

inline void LRFDBridge::GetStrengthILoadFactors(double &_dcMin,double &_dcMax,double &_dwMin,double &_dwMax,double &_ll)
   {
   _dcMin = StrengthI_dcMin;
   _dcMax = StrengthI_dcMax;
   _dwMin = StrengthI_dwMin;
   _dwMax = StrengthI_dwMax;
   _ll    = StrengthI_ll;
   }

inline void LRFDBridge::SetServiceILoadFactors(double _dc,double _dw,double _ll)
   {
   ServiceI_dc = _dc;
   ServiceI_dw = _dw;
   ServiceI_ll = _ll;
   }

inline void LRFDBridge::GetServiceILoadFactors(double &_dc,double &_dw,double &_ll)
   {
   _dc = ServiceI_dc;
   _dw = ServiceI_dw;
   _ll = ServiceI_ll;
   }

inline void LRFDBridge::SetServiceIILoadFactors(double _dc,double _dw,double _ll)
   {
   ServiceII_dc = _dc;
   ServiceII_dw = _dw;
   ServiceII_ll = _ll;
   }

inline void LRFDBridge::GetServiceIILoadFactors(double &_dc,double &_dw,double &_ll)
   {
   _dc = ServiceII_dc;
   _dw = ServiceII_dw;
   _ll = ServiceII_ll;
   }

inline void LRFDBridge::SetServiceIIILoadFactors(double _dc,double _dw,double _ll)
   {
   ServiceIII_dc = _dc;
   ServiceIII_dw = _dw;
   ServiceIII_ll = _ll;
   }

inline void LRFDBridge::GetServiceIIILoadFactors(double &_dc,double &_dw,double &_ll)
   {
   _dc = ServiceIII_dc;
   _dw = ServiceIII_dw;
   _ll = ServiceIII_ll;
   }

inline void LRFDBridge::SetFatigueLoadFactors(double _dc,double _dw,double _ll)
   {
   Fatigue_dc = _dc;
   Fatigue_dw = _dw;
   Fatigue_ll = _ll;
   }

inline void LRFDBridge::GetFatigueLoadFactors(double &_dc,double &_dw,double &_ll)
   {
   _dc = Fatigue_dc;
   _dw = Fatigue_dw;
   _ll = Fatigue_ll;
   }

#endif // LRFDBRDG_H_
