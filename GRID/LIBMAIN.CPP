/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// libmain.cpp                                                             //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "griddll.h"
#pragma hdrstop

TModule *GridModule = 0;

#ifdef __WIN32__
BOOL WINAPI DllEntryPoint (HINSTANCE instance, DWORD reason, LPVOID)
   {
   static int Attaches = 0;

   switch (reason)
      {
      case DLL_PROCESS_ATTACH:
         if (!Attaches++)
            {
            GridModule = new TModule ("TGrid", instance);
            InitGridLib ();
            }
         break;

      case DLL_PROCESS_DETACH:
         if (!--Attaches)
            {
            delete GridModule;
            GridModule = 0;
            }
         break;
      }

   return true;
   }

#else
int FAR PASCAL LibMain (HINSTANCE instance, WORD, WORD, char far *)
   {
   if (!GridModule)
      {
      GridModule = new TModule ("TGrid", instance);
      InitGridLib ();
      }
   return true;
   }

int WINAPI WEP(int /*nParameter*/)
   {
   delete GridModule;
   GridModule = 0;
   return true;
   }

#endif

// EOF
