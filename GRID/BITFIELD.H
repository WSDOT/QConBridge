/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// bitfield.h                                                              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Grid control library - Version 1.40 - 22 oct 1995                       //
// TGrid - Copyright © 1994,1995 H. Picot, All Rights Reserved.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


#ifndef BITFIELD_H
#define BITFIELD_H

const ulong bitNoFind = 0xffffffffL;
const int bfieldDefIncr = 32;

class _GRIDCLASS TBitField
   {
   public:
      TBitField (ulong nbits, int delta = bfieldDefIncr);
      TBitField (const TBitField &bf);
      TBitField ();
      virtual ~TBitField ();

      TBitField &operator = (const TBitField &bf);

      void ClearAll ();
      void SetAll ();
      void ToggleAll ();

      void ClearRange  (ulong start, ulong end);
      void SetRange    (ulong start, ulong end);
      void ToggleRange (ulong start, ulong end);

      void Clear (ulong pos);
      void Set   (ulong pos);
      bool Read  (ulong pos) const;
      bool Toggle(ulong pos);

      ulong FindFirst (bool set = true);
      ulong FindNext (bool set = true);

      void SetDelta (int ps) { delta = ps; }
      int GetDelta () const { return delta; }

   protected:
      ulong *bits;
      ulong nbits;
      ulong size;
      ulong current; 
      int index;
      int delta;  // number of words to add when the size has to be increased
      void CalcSize ();
      void CheckSize (ulong newsize);
   };

#endif 

// EOF
