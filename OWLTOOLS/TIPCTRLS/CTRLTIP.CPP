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

#include <owl/pch.h>
#include <owltools\ctrltip.h>

#define TIPID 1

DEFINE_RESPONSE_TABLE1(TControlTipMixin, TEventHandler)
  EV_TTN_NEEDTEXT(TIPID, EvNeedText),
END_RESPONSE_TABLE;

TControlTipMixin::TControlTipMixin(const char* tip) :
tipText(tip),
msgResourceId(-1)
{
   tooltip = 0;
}

TControlTipMixin::TControlTipMixin(int msgResourceId) :
tipText(""),
msgResourceId(msgResourceId)
{
   tooltip = 0;
}


TControlTipMixin::~TControlTipMixin()
{
   delete tooltip;
}

void TControlTipMixin::SetupTips()
{
  TWindow* myself = TYPESAFE_DOWNCAST(this,TWindow);

  TRect rect = myself->GetClientRect();

  tooltip = new TTooltip(myself);
  tooltip->Create();

  TToolInfo ti(*myself,rect,TIPID);
  tooltip->AddTool(ti);
}

bool TControlTipMixin::PreProcessMsg(MSG& msg)
{
    if (tooltip && tooltip->IsWindow())
      {
      switch(msg.message)
        {
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            tooltip->RelayEvent(msg);
            break;
      }
    }

  return false;
}

void TControlTipMixin::EvNeedText(TTooltipText far& ttText)
{
   if (msgResourceId < 0)
      ttText.CopyText(tipText.c_str());
   else
      {
      TWindow* myself = TYPESAFE_DOWNCAST(this,TWindow);
      ttText.SetText(msgResourceId,*myself->GetApplication());
      }
}

