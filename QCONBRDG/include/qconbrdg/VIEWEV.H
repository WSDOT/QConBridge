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

#if !defined VIEWEV_H
#define VIEWEV_H

#include <bridge\lrfdbrdg.h>

// Place-holder declarations
class _BRIDGECLASS Span;
class _BRIDGECLASS Support;
class _BRIDGECLASS BridgeLoad;

// Message Structures
typedef struct tagSSNOTIFY
{
   Span    *oldSpan;
   Support *oldSupport;
   Span    *newSpan;
   Support *newSupport;

   tagSSNOTIFY() {oldSpan    = NULL;
                  oldSupport = NULL;
                  newSpan    = NULL;
                  newSupport = NULL;}
} SSNOTIFY;

typedef struct tagLDNOTIFY
{
   BridgeLoad *load;
   tagLDNOTIFY() {load = NULL;};
} LDNOTIFY;

// View Notification Names
const int vnDisplayDimensions   = vnCustomBase +  0;
const int vnDisplayLabels       = vnCustomBase +  1;
const int vnDisplaySegmentTicks = vnCustomBase +  2;
const int vnDisplayGrid         = vnCustomBase +  3;
const int vnInvertMomDiags      = vnCustomBase +  4;
const int vnAnalyzed            = vnCustomBase +  5;
const int vnRebuildDisplay      = vnCustomBase +  6;
const int vnClearAll            = vnCustomBase +  7;
const int vnAddSpan             = vnCustomBase +  8;
const int vnAddSupport          = vnCustomBase +  9;
const int vnAddSpanSupport      = vnCustomBase + 10;
const int vnRemoveSpan          = vnCustomBase + 11;
const int vnRemoveSupport       = vnCustomBase + 12;
const int vnRemoveSpanSupport   = vnCustomBase + 13;
const int vnReplaceSpan         = vnCustomBase + 14;
const int vnReplaceSupport      = vnCustomBase + 15;
const int vnReplaceSpanSupport  = vnCustomBase + 16;
const int vnRemoveLoad          = vnCustomBase + 17;
const int vnAddLoad             = vnCustomBase + 18;
const int vnUnitsChanged        = vnCustomBase + 19;
const int vnPlotStyleChanged    = vnCustomBase + 20;
const int vnStdDeadLoadsChanged = vnCustomBase + 21;
const int vnPedestrianLoadChanged = vnCustomBase + 22;

// Notification function signatures
NOTIFY_SIG(vnDisplayDimensions,  bool)
NOTIFY_SIG(vnDisplayLabels,      bool)
NOTIFY_SIG(vnDisplaySegmentTicks,bool)
NOTIFY_SIG(vnDisplayGrid,        bool)
NOTIFY_SIG(vnInvertMomDiags,     int)
NOTIFY_SIG(vnAnalyzed,           bool)
NOTIFY_SIG(vnRebuildDisplay,     void)
NOTIFY_SIG(vnClearAll,           void)
NOTIFY_SIG(vnAddSpan,            long)
NOTIFY_SIG(vnAddSupport,         long)
NOTIFY_SIG(vnAddSpanSupport,     long)
NOTIFY_SIG(vnRemoveSpan,         long)
NOTIFY_SIG(vnRemoveSupport,      long)
NOTIFY_SIG(vnRemoveSpanSupport,  long)
NOTIFY_SIG(vnReplaceSpan,        long)
NOTIFY_SIG(vnReplaceSupport,     long)
NOTIFY_SIG(vnReplaceSpanSupport, long)
NOTIFY_SIG(vnRemoveLoad,         long)
NOTIFY_SIG(vnAddLoad,            long)
NOTIFY_SIG(vnUnitsChanged,       void)
NOTIFY_SIG(vnPlotStyleChanged,   DiagramCatagory)
NOTIFY_SIG(vnStdDeadLoadsChanged,void)
NOTIFY_SIG(vnPedestrianLoadChanged,void)

// Notification Macros
#define EV_VN_DISPLAYDIMENSIONS   VN_DEFINE(vnDisplayDimensions  ,VnDisplayDimensions  ,int)
#define EV_VN_DISPLAYLABELS       VN_DEFINE(vnDisplayLabels      ,VnDisplayLabels      ,int)
#define EV_VN_DISPLAYSEGMENTTICKS VN_DEFINE(vnDisplaySegmentTicks,VnDisplaySegmentTicks,int)
#define EV_VN_DISPLAYGRID         VN_DEFINE(vnDisplayGrid        ,VnDisplayGrid        ,int)
#define EV_VN_INVERTMOMENTDIAGRAMS VN_DEFINE(vnInvertMomDiags    ,VnInvertMomDiags     ,int)
#define EV_VN_ANALYZED            VN_DEFINE(vnAnalyzed,VnAnalyzed,int)
#define EV_VN_REBUILDDISPLAY      VN_DEFINE(vnRebuildDisplay,VnRebuildDisplay,void)
#define EV_VN_CLEARALL            VN_DEFINE(vnClearAll,VnClearAll,void)
#define EV_VN_ADDSPAN             VN_DEFINE(vnAddSpan,       VnAddSpan,       long)
#define EV_VN_ADDSUPPORT          VN_DEFINE(vnAddSupport,    VnAddSupport,    long)
#define EV_VN_ADDSPANSUPPORT      VN_DEFINE(vnAddSpanSupport,VnAddSpanSupport,long)
#define EV_VN_REMOVESPAN          VN_DEFINE(vnRemoveSpan,       VnRemoveSpan,       long)
#define EV_VN_REMOVESUPPORT       VN_DEFINE(vnRemoveSupport,    VnRemoveSupport,    long)
#define EV_VN_REMOVESPANSUPPORT   VN_DEFINE(vnRemoveSpanSupport,VnRemoveSpanSupport,long)
#define EV_VN_REPLACESPAN         VN_DEFINE(vnReplaceSpan,       VnReplaceSpan,       long)
#define EV_VN_REPLACESUPPORT      VN_DEFINE(vnReplaceSupport,    VnReplaceSupport,    long)
#define EV_VN_REPLACESPANSUPPORT  VN_DEFINE(vnReplaceSpanSupport,VnReplaceSpanSupport,long)
#define EV_VN_REMOVELOAD          VN_DEFINE(vnRemoveLoad,       VnRemoveLoad,       long)
#define EV_VN_ADDLOAD             VN_DEFINE(vnAddLoad,       VnAddLoad,       long)
#define EV_VN_UNITSCHANGED        VN_DEFINE(vnUnitsChanged,VnUnitsChanged,void)
#define EV_VN_PLOTSTYLECHANGED    VN_DEFINE(vnPlotStyleChanged,VnPlotStyleChanged,int)
#define EV_VN_STDDEADLOADSCHANGED VN_DEFINE(vnStdDeadLoadsChanged,VnStdDeadLoadsChanged,void)
#define EV_VN_PEDESTRIANLOADCHANGED VN_DEFINE(vnPedestrianLoadChanged,VnPedestrianLoadChanged,void)

#endif // VIEWEV_H
