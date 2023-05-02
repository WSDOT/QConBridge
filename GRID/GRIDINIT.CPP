/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// gridinit.cpp                                                            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "griddll.h"
#pragma hdrstop

bool isChicago = false;

void InitGridLib ()
   {
   InitLanguage ();
   #ifdef __WIN32__
      if ((::GetVersion()&0xFF) >= 4)
         isChicago = true;
   #else
      if ((::GetVersion()&0xFF) >= 3 && ((::GetVersion()>>8)&0xFF) >= 95)
         isChicago = true;
   #endif
   }

// EOF