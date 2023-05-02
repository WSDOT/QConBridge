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

#if !defined ESL_GEOMETRY_H_
#define ESL_GEOMETRY_H_

#include <math.h>

#undef  EPS
#define EPS  1.0e-10
#undef   zero_val
#define  zero_val( x )     (((-EPS)<=(x)) && ((x)<=(EPS)))

// Forward declaration
template <class Type> class TAnyPoint;
template <class Type> class TAnySize;
template <class Type> class TAnyRect;

//
//-------------------------------------------------------------------
//

template <class Type> class TAnyPoint
{
// Data Members
public:
   Type x;
   Type y;

// Constructors
public:
   TAnyPoint(void)                       : x(0),y(0) {};
   TAnyPoint(Type x,Type y)              : x(x),y(y) {};
   TAnyPoint(const TAnyPoint<Type>& pnt) : x(pnt.x),y(pnt.y) {};

// Methods
public:
   Type Distance(TAnyPoint<Type>& p)
      {
      return sqrt(pow((this->x - p.x),2) + pow((this->y - p.y),2));
      };

   TAnyPoint<Type>& Offset(Type dx,Type dy)
      {
      this->x += dx;
      this->y += dy;
      return (*this);
      };

   TAnyPoint<Type> OffsetBy(Type dx,Type dy) const
      {
      return TAnyPoint<Type>(dx+x,dy+y);
      };

// Operators
public:
   TAnyPoint<Type> operator + (const TAnySize<Type>& size) const
      {
      return TAnyPoint<Type>(x+size.dx,y+size.dy);
      };

   TAnySize<Type> operator - (const TAnyPoint<Type>& point) const
      {
      return TAnySize<Type>(x-point.x,y-point.y);
      };

   TAnyPoint<Type> operator - () const
      {
      return TAnyPoint<Type>(-x,-y);
      }

   bool operator == (const TAnyPoint<Type>& other) const
      {
      return (zero_val(x-other.x) && zero_val(y-other.y));
      };

   bool operator != (const TAnyPoint<Type>& other) const
      {
      return !(zero_val(x-other.x) && zero_val(y-other.y));
      };

   TAnyPoint<Type>& operator += (const TAnySize<Type> &size)
      {
      x += size.dx;
      y += size.dy;
      return (*this);
      };

   TAnyPoint<Type>& operator -= (const TAnySize<Type> &size)
      {
      x -= size.dx;
      y -= size.dy;
      return (*this);
      };
};

//
//-------------------------------------------------------------
//

template <class Type> class TAnySize
{
public:
   Type dx, dy;

public:
   TAnySize(void)
      {
      dx = 0;
      dy = 0;
      };

   TAnySize(Type _dx,Type _dy)
      {
      dx=_dx;
      dy=_dy;
      };

   TAnySize(const TAnyPoint<Type> &point)
      {
      dx = point.x;
      dy = point.y;
      };

   TAnySize(const TAnySize &size)
      {
      dx = size.dx;
      dy = size.dy;
      };

   Type Magintude() const
      {
      return (Type)sqrt(pow(dx,2) + pow(dy,2));
      };

   TAnySize operator +  (const TAnySize &size)  const
      {
      return TAnySize<Type>(dx+size.dx,dy+size.dy);
      };

   TAnySize operator -  (const TAnySize &size)  const
      {
      return TAnySize<Type>(dx-size.dx,dy-size.dy);
      };

   TAnySize operator - () const
      {
      return TAnySize<Type>(-dx,-dy);
      }

   bool operator == (const TAnySize &other) const
      {
      return (zero_val(dx-other.dx) && zero_val(dy-other.dy));
      };

   bool operator != (const TAnySize &other) const
      {
      return !(zero_val(dx-other.dx) && zero_val(dy-other.dy));
      };

   TAnySize& operator += (const TAnySize &size)
      {
      dx += size.dx;
      dy += size.dy;
      return (*this);
      };

   TAnySize& operator -= (const TAnySize &size)
      {
      dx -= size.dx;
      dy -= size.dy;
      return (*this);
      };
};

//
//-----------------------------------------------------------------------------
//

// minX,minY is in the bottom left corner
// maxX,maxY is in the top right corner
// This is opposite of OWL's TRect
template <class Type> class TAnyRect
{
public:
   Type minX,minY,maxX,maxY;

public:
   TAnyRect(void)
      {
      minX = minY = maxX = maxY = 0;
      };

   TAnyRect(Type _minX,Type _minY,Type _maxX,Type _maxY)
      {
      minX = _minX;
      minY = _minY;
      maxX = _maxX;
      maxY = _maxY;
      };

   TAnyRect(const TAnyPoint<Type> &loLeft,const TAnyPoint<Type> &upRight)
      {
      minX = loLeft.x;
      minY = loLeft.y;
      maxX = upRight.x;
      maxY = upRight.y;
      };

   TAnyRect(const TAnyPoint<Type> &loLeft,const TAnySize<Type> &size)
      {
      minX = loLeft.x;
      minY = loLeft.y;
      maxX = loLeft.x + size.dx;
      maxY = loLeft.y + size.dy;
      };

   TAnyPoint<Type> TAnyRect::TopLeft() const
      {
      return TAnyPoint<Type>(minX,maxY);
      }

   TAnyPoint<Type> TAnyRect::TopRight() const
      {
      return TAnyPoint<Type>(maxX,maxY);
      }

   TAnyPoint<Type> TAnyRect::BottomLeft() const
      {
      return TAnyPoint<Type>(minX,minY);
      }

   TAnyPoint<Type> TAnyRect::BottomRight() const
      {
      return TAnyPoint<Type>(maxX,minY);
      }

   TAnySize<Type> TAnyRect::Size() const
      {
      return TAnySize<Type>(maxX - minX,maxY - minY);
      }

   bool Contains(const TAnyPoint<Type>& point) const
      {
      return ( (minX <= point.x && point.x < maxX) &&
               (minY <= point.y && point.y < maxY) );
      };

   bool Contains(const TAnyRect<Type>& other) const
      {
      return ( (minX <= other.minX && other.maxX <= maxX) &&
               (minY <= other.minY && other.maxY <= maxY) );
      };

   bool Touches(const TAnyRect<Type>& other) const
      {
      return ( (minX < other.maxX && other.minX < maxX) &&
               (minY < other.maxY && other.minY < maxY) );
      };
};


#endif // ESL_GEOMETRY_H_
