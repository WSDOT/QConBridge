/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// griddll.h                                                               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


// for compatibility with 4.0x
#if __BORLANDC__ < 0x460
#define false FALSE
#define true TRUE
#define bool BOOL 
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned long ulong;
typedef unsigned short ushort;
#endif

#if !defined (__WIN32__)
#define LANG_DUTCH                       0x13
#define LANG_ENGLISH                     0x09
#define LANG_FRENCH                      0x0c
#define LANG_GERMAN                      0x07
#define LANG_ITALIAN                     0x10
#define LANG_PORTUGUESE                  0x16
#define LANG_SPANISH                     0x0a
#endif


#ifndef GRIDDLL_H
#define GRIDDLL_H

#ifndef OEMRESOURCE
#define OEMRESOURCE
#endif

#if defined (_BUILDGRIDDLL)
#  define _GRIDCLASS    __declspec(dllexport)
#  define _GRIDFUNCTION __declspec(dllexport)
#  define _GRIDDATA     __declspec(dllexport)
#  define GRIDMODULE

#elif defined (_GRIDLIB)
#  define _GRIDCLASS
#  define _GRIDFUNCTION
#  define _GRIDDATA     

#else
#  define _GRIDCLASS    __declspec(dllimport)
#  ifdef __WIN32__
#     define _GRIDFUNCTION __declspec(dllimport)
#     define _GRIDDATA     __declspec(dllimport)
#  else
#     define _GRIDFUNCTION
#     define _GRIDDATA     
#  endif
#  define GRIDMODULE

#endif

#ifndef __OWL_EDIT_H
#include <owl/edit.h>
#endif
#ifndef __OWL_COMBOBOX_H
#include <owl/combobox.h>
#endif
#ifndef __OWL_BUTTON_H
#include <owl/button.h>
#endif
#ifndef __OWL_CHECKBOX_H
#include <owl/checkbox.h>
#endif
#ifndef __OWL_VALIDATE_H
#include <owl/validate.h>
#endif
#ifndef __STDIO_H
#include <stdio.h>
#endif

#include <bitfield.h>
#include <grid.h>
#include <gridbtn.h>
#include <gridcbox.h>
#include <datacbox.h>
#include <gridedit.h>
#include <gridstat.h>
#include <gridckbx.h>
#include <language.h>
#include <intldate.h>
#include <dateedit.h>
#include <tgtime.h>
#include <timeedit.h>
#include <numedit.h>

#ifdef GRIDMODULE
extern TModule *GridModule;
#endif

extern void InitGridLib ();
extern bool isChicago;

#endif
