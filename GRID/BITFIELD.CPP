/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// bitfield.cpp                                                            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


#include "griddll.h"
#pragma hdrstop

static const int wordSize = 32;
static const int wordSizeMask = 31;
static const int wordSizeShift = 5;  // n >> 5 == n % 32
static const ulong allBits = 0xffffffffL;

TBitField::TBitField (ulong _nbits, int _delta)
   : current (0),
     index(0),
     nbits (_nbits),
     delta(_delta)
   {
   CalcSize ();
   bits = new ulong[uint(size)];
   memset (bits, 0, sizeof(ulong)*uint(size));
   }


TBitField::TBitField (const TBitField &bf)
   : current (0),
     index (0),
     nbits (bf.nbits),
     size (bf.size),
     delta (bf.delta)
   {
   bits = new ulong[uint(size)];
   memcpy (bits, bf.bits, sizeof(ulong)*uint(size));
   }


TBitField::TBitField ()
   : current (0),
     index (0),
     delta(bfieldDefIncr),
     nbits(0),
     size(0),
     bits(0)
   {
   }


TBitField::~TBitField ()
   {
   if (bits)
      delete [] bits;
   }



TBitField &TBitField::operator = (const TBitField &bf)
   {
   current = 0;
   nbits = bf.nbits;
   size = bf.size;
   if (bits)
      delete [] bits;
   bits = new ulong[uint(size)];
   memcpy (bits, bf.bits, uint(size)*sizeof(ulong));
   return *this;
   }


void TBitField::CalcSize ()
   {
   size = (nbits + wordSizeMask) >> wordSizeShift;
   size = (size  + delta - 1) / delta;
   size *= delta;
   nbits = size << wordSizeShift;
   }


void TBitField::Clear (ulong pos)
   {
   ulong offset = pos >> wordSizeShift;
   ulong mask = (1 << int(pos & wordSizeMask));
   mask ^= allBits;
   CheckSize (offset+1);
   bits[uint(offset)] &= mask;
   }


void TBitField::Set (ulong pos)
   {
   ulong offset = pos >> wordSizeShift;
   ulong mask = (1 << int(pos & wordSizeMask));
   CheckSize (offset+1);
   bits[uint(offset)] |= mask;
   }


bool TBitField::Read (ulong pos) const
   {
   ulong offset = pos >> wordSizeShift;

   if (offset >= size)
      return false;

   ulong mask = (1 << int(pos & wordSizeMask));
   ulong r = bits[uint(offset)] & mask;
   if (r)
      return true;
   else
      return false;
   }


bool TBitField::Toggle (ulong pos)
   {
   ulong offset = pos >> wordSizeShift;
   ulong mask = (1 << int(pos & wordSizeMask));
   CheckSize (offset+1);
   bits[uint(offset)] ^= mask;
   ulong r = bits[uint(offset)] & mask;
   if (r)
      return true;
   else
      return false;
   }


ulong TBitField::FindFirst (bool set)
   {
   index = 0;

   for (current = 0; current < size; current++)
      {
      if (set && bits[uint(current)] == 0)
         continue;
      else if (!set && bits[uint(current)] == allBits)
         continue;
      ulong b = bits[uint(current)];
      for (ulong mask = 1; index < wordSize; index++, mask <<= 1)
         {
         if (((b & mask) != 0) == set)
            return (current << wordSizeShift) + index;
         }
      index = 0;
      }

   return bitNoFind;
   }


ulong TBitField::FindNext (bool set)
   {
   if (++index >= wordSize)
      {
      index -= wordSize;
      ++current;
      }

   for (; current < size; current++)
      {
      if (set && bits[uint(current)] == 0)
         continue;
      else if (!set && bits[uint(current)] == allBits)
         continue;
      ulong b = bits[uint(current)];
      for (ulong mask = (index ? (1<<index) : 1); index < wordSize; index++, mask <<= 1)
         {
         if (((b & mask) != 0) == set)
            return (current << wordSizeShift) + index;
         }
      index = 0;
      }

   return bitNoFind;
   }


void TBitField::CheckSize (ulong newsize)
   {
   if (newsize > size)
      {
      ulong *old = bits;
      // round up to nearest 'delta' ulongs
      newsize = (newsize+delta-1) / delta;
      newsize *= delta;
      bits = new ulong[uint(newsize)];
      memset (bits, 0, uint(newsize)*sizeof(ulong));
      if (old)
         {
         memcpy (bits, old, sizeof(ulong)*uint(size));
         delete [] old;
         }
      size = newsize;
      nbits = size << wordSizeShift;
      }
   }


void TBitField::ClearAll ()
   {
   if (bits)
      memset (bits, 0, sizeof(ulong)*uint(size));
   }


void TBitField::SetAll ()
   {
   if (bits)
      memset (bits, -1, sizeof(ulong)*uint(size));
   }


void TBitField::ToggleAll ()
   {
   if (bits)
      {
      ulong *p = bits;
      for (ulong i = 0; i < size; i++, p++)
         *p ^= allBits;
      }
   }


void TBitField::ClearRange (ulong start, ulong end)
   {
   ulong endoffset = (end+1) >> wordSizeShift;
   CheckSize (endoffset+1);
   ulong startoffset = start >> wordSizeShift;

   if (endoffset < startoffset)
      return;
   else
      {
      ulong maskstart = (allBits << int(start & wordSizeMask));
      ulong maskend   = allBits;
      maskend >>= (wordSize-((end+1) & wordSizeMask));
      if (endoffset == startoffset)
         {
         ulong mask = maskstart & maskend;
         mask ^= allBits;
         bits[uint(startoffset)] &= mask;
         }
      else
         {
         maskstart ^= allBits;
         bits[uint(startoffset)] &= maskstart;
         maskend ^= allBits;
         bits[uint(endoffset)] &= maskend;
         ulong n = endoffset-startoffset-1;
         if (n)
            memset (bits+uint(startoffset)+1, 0, sizeof(ulong)*uint(n));
         }
      }
   }


void TBitField::SetRange (ulong start, ulong end)
   {
   ulong endoffset = (end+1) >> wordSizeShift;
   CheckSize (endoffset+1);
   ulong startoffset = start >> wordSizeShift;

   if (endoffset < startoffset)
      return;
   else
      {
      ulong maskstart = (allBits << int(start & wordSizeMask));
      ulong maskend   = allBits;
      maskend >>= (wordSize-((end+1) & wordSizeMask));
      if (endoffset == startoffset)
         {
         ulong mask = maskstart & maskend;
         bits[uint(startoffset)] |= mask;
         }
      else
         {
         bits[uint(startoffset)] |= maskstart;
         bits[uint(endoffset)] |= maskend;
         ulong n = endoffset-startoffset-1;
         if (n)
            memset (bits+uint(startoffset)+1, -1, sizeof(ulong)*uint(n));
         }
      }
   }


void TBitField::ToggleRange (ulong start, ulong end)
   {
   ulong endoffset = (end+1) >> wordSizeShift;
   CheckSize (endoffset+1);
   ulong startoffset = start >> wordSizeShift;

   if (endoffset < startoffset)
      return;
   else
      {
      ulong maskstart = (allBits << int(start & wordSizeMask));
      ulong maskend   = allBits;
      maskend >>= (wordSize-((end+1) & wordSizeMask));
      if (endoffset == startoffset)
         {
         ulong mask = maskstart & maskend;
         bits[uint(startoffset)] ^= mask;
         }
      else
         {
         bits[uint(startoffset)] ^= maskstart;
         bits[uint(endoffset)] ^= maskend;
         ulong *p = bits+uint(startoffset)+1;
         for (ulong n = startoffset+1; n < endoffset; n++)
            *p ^= allBits;
         }
      }
   }

// EOF

