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

#include <math.h>
#include "tdc.hpp"

#define EPS  1.0e-10
#undef   zero_val
#define  zero_val( x )     (((-EPS)<=(x)) && ((x)<=(EPS)))

TFPoint::TFPoint(void)
{
   x=0;
   y=0;
}

TFPoint::TFPoint(double _x,double _y)
{
   x=_x;
   y=_y;
}

TFPoint::TFPoint(const POINT& point)
{
   x=(double)point.x;
   y=(double)point.y;
}

TFPoint::TFPoint(const TPoint& point)
{
   x=(double)point.x;
   y=(double)point.y;
}

TFPoint::TFPoint(const SIZE& size)
{
   x=(double)size.cx;
   y=(double)size.cy;
}

TFPoint::TFPoint(const TSize& size)
{
   x=(double)size.cx;
   y=(double)size.cy;
}

TFPoint::TFPoint(const TFSize& size)
{
   x=size.dx;
   y=size.dy;
}

TFPoint::TFPoint(uint32 dw)
{
   x=(double)LOWORD(dw);
   y=(double)HIWORD(dw);
}

double TFPoint::Distance(TFPoint &p)
{
   return sqrt(pow((this->x - p.x),2) + pow((this->y - p.y),2));
}

TFPoint& TFPoint::Offset(double dx,double dy)
{
   this->x += dx;
   this->y += dy;

   return (*this);
}

TFPoint  TFPoint::OffsetBy(double dx,double dy) const
{
   return TFPoint(dx+x,dy+y);
}

TFPoint TFPoint::operator + (const TFSize  &s) const
{
   return TFPoint(x+s.dx,y+s.dy);
}

TFPoint TFPoint::operator + (const TFPoint &p) const
{
   return TFPoint(x+p.x,y+p.y);
}

TFPoint TFPoint::operator - (const TFSize  &s) const
{
   return TFPoint(x-s.dx,y-s.dy);
}

TFSize TFPoint::operator - (const TFPoint &p) const
{
   return TFSize(x-p.x,y-p.y);
}

TFPoint TFPoint::operator - () const
{
   return TFPoint(-x,-y);
}

bool TFPoint::operator == (const TFPoint &other) const
{
   return (zero_val(x-other.x) && zero_val(y-other.y));
}

bool TFPoint::operator != (const TFPoint &other) const
{
   return !(zero_val(x-other.x) && zero_val(y-other.y));
}

TFPoint& TFPoint::operator += (const TFSize  &size)
{
   x += size.dx;
   y += size.dy;
   return (*this);
}

TFPoint& TFPoint::operator += (const TFPoint &point)
{
   x += point.x;
   y += point.y;
   return (*this);
}

TFPoint& TFPoint::operator -= (const TFSize  &size)
{
   x -= size.dx;
   y -= size.dy;
   return (*this);
}

TFPoint& TFPoint::operator -= (const TFPoint &point)
{
   x -= point.x;
   y -= point.y;
   return (*this);
}

//
// TFSize
//
TFSize::TFSize(void)
{
   dx = 0;
   dy = 0;
}

TFSize::TFSize(double _dx,double _dy)
{
   dx=_dx;
   dy=_dy;
}

TFSize::TFSize(const POINT &point)
{
   dx=(double)point.x;
   dy=(double)point.y;
}

TFSize::TFSize(const TFPoint &point)
{
   dx = point.x;
   dy = point.y;
}

TFSize::TFSize(const SIZE &size)
{
   dx=(double)size.cx;
   dy=(double)size.cy;
}

TFSize::TFSize(const TFSize &size)
{
   dx = size.dx;
   dy = size.dy;
}

TFSize::TFSize(uint32 dw)
{
   dx=(double)LOWORD(dw);
   dy=(double)HIWORD(dw);
}

double TFSize::Magintude() const
{
   return sqrt(pow(dx,2) + pow(dy,2));
}

TFSize TFSize::operator +  (const TFSize &size)  const
{
   return TSize(dx+size.dx,dy+size.dy);
}

TFSize TFSize::operator -  (const TFSize &size)  const
{
   return TSize(dx-size.dx,dy-size.dy);
}

TSize TFSize::operator -  () const
{
   return TSize(-dx,-dy);
}

bool TFSize::operator == (const TFSize &other) const
{
   return (zero_val(dx-other.dx) && zero_val(dy-other.dy));
}

bool TFSize::operator != (const TFSize &other) const
{
   return !(zero_val(dx-other.dx) && zero_val(dy-other.dy));
}

TFSize& TFSize::operator += (const TFSize &size)
{
   dx += size.dx;
   dy += size.dy;
   return (*this);
}

TFSize& TFSize::operator -= (const TFSize &size)
{
   dx -= size.dx;
   dy -= size.dy;
   return (*this);
}

//
// TFRect
//
TFRect::TFRect(void)
{
   left = top = right = bottom = 0.;
}

TFRect::TFRect(double _left,double _top,double _right,double _bottom)
{
   left   = _left;
   top    = _top;
   right  = _right;
   bottom = _bottom;
}

TFRect::TFRect(const TFPoint &upLeft,const TFPoint &loRight)
{
   left   = upLeft.x;
   top    = upLeft.y;
   right  = loRight.x;
   bottom = loRight.y;
};

TFRect::TFRect(const TFPoint &origin,const TFSize &extent)
{
   left   = origin.x;
   top    = origin.y;
   right  = left + extent.dx;
   bottom = top + extent.dy;
}

//
// TRealDC
//
TRealDC::TRealDC(TDC &dc) : TDC(HDC(dc))
{
   // save the current mapping mode.
   // it will be reset in the destructor
   mapMode = SetMapMode(MM_TEXT);

   WorldOrg = TFPoint(0,0);
   WorldExt = TFSize(1,1);

   ViewOrg = TPoint(0,0);
   ViewExt = TSize(1,1);
}

TRealDC::~TRealDC(void)
{
   // restore the previous mapping mode
   SetMapMode(mapMode);
}

bool TRealDC::MoveTo(double x,double y)
{
   return MoveTo(TFPoint(x,y));
}

bool TRealDC::MoveTo(TFPoint &point)
{
   TPoint p;

   WPtoDP(point,p);

   return TDC::MoveTo(p);
}

bool TRealDC::LineTo(double x,double y)
{
   return LineTo(TFPoint(x,y));
}

bool TRealDC::LineTo(TFPoint &point)
{
   TPoint p;

   WPtoDP(point,p);

   return TDC::LineTo(p);
}

bool TRealDC::Polygon(const TFPoint *points,int count)
{
   TPoint *p;
   int i;

   p = new TPoint[count];
   for (i = 0; i < count; i++)
      {
      WPtoDP(points[i],p[i]);
      }

   bool retval;
   retval = TDC::Polygon(p,count);

   delete[] p;

   return retval;
}

bool TRealDC::Polygon(const TFPointContainer &points)
{
   TFPointIterator i(points);
   TPoint *p2;
   int count;

   p2 = new TPoint[points.GetItemsInContainer()];

   count = 0;
   while(i)
      {
      TFPoint *p1 = i++;
      WPtoDP(*p1,p2[count]);
      count++;
      }

   bool retval;
   retval = TDC::Polygon(p2,count);

   delete[] p2;

   return retval;
}

bool TRealDC::Rectangle(double left,double top,double right,double bottom)
{
   int _left,_top,_right,_bottom;

   WPtoDP(left,top,_left,_top);
   WPtoDP(right,bottom,_right,_bottom);

   return TDC::Rectangle(_left,_top,_right,_bottom);
}

bool TRealDC::Rectangle(const TFPoint &p1,const TFPoint &p2)
{
   return Rectangle(p1.x,p1.y,p2.x,p2.y);
}

bool TRealDC::Rectangle(const TFPoint &point,const TFSize &size)
{
   return Rectangle(point.x,point.y,point.x + size.dx,point.y + size.dy);
}

bool TRealDC::Rectangle(const TFRect &rect)
{
   return Rectangle(rect.left,rect.top,rect.right,rect.bottom);
}

bool TRealDC::Ellipse(double left,double top,double right,double bottom)
{
   int _left,_top,_right,_bottom;

   WPtoDP(left,top,_left,_top);
   WPtoDP(right,bottom,_right,_bottom);

   return TDC::Ellipse(_left,_top,_right,_bottom);
}

bool TRealDC::Ellipse(const TFPoint &p1,const TFPoint &p2)
{
   return Ellipse(p1.x,p1.y,p2.x,p2.y);
}

bool TRealDC::Ellipse(const TFPoint &point,const TFSize &size)
{
   return Ellipse(point.x,point.y,point.x + size.dx,point.y + size.dy);
}

bool TRealDC::Ellipse(const TFRect &rect)
{
   return Ellipse(rect.left,rect.top,rect.right,rect.bottom);
}

bool TRealDC::SetWorldOrg(double x,double y)
{
   return SetWorldOrg(TFPoint(x,y));
}

bool TRealDC::SetWorldOrg(TFPoint &point)
{
   // Maps (point.x,point.y) to (ViewOrg.x,ViewOrg.y)
   WorldOrg = point;

   return TRUE;
}

bool TRealDC::SetWorldExt(double dx,double dy)
{
   WorldExt = TFSize(dx,dy);

   AdjustViewExt();

   return TRUE;
}

bool TRealDC::SetWorldExt(TFSize &extent)
{
   WorldExt = extent;

   AdjustViewExt();

   return TRUE;
}

bool TRealDC::SetViewOrg(int x,int y)
{
   return SetViewOrg(TPoint(x,y));
}

bool TRealDC::SetViewOrg(TPoint &point)
{
   // Maps (point.x,point.y) to (WorldOrg.x,WorldOrg.y)
   ViewOrg = point;

   return TRUE;
}

bool TRealDC::SetViewExt(int cx,int cy)
{
   ViewExt = TSize(cx,cy);

   AdjustViewExt();

   return TRUE;
}

bool TRealDC::SetViewExt(TSize &extent)
{
   ViewExt = extent;

   AdjustViewExt();

   return TRUE;
}

TFPoint TRealDC::GetWorldOrg(void)
{
   return WorldOrg;
}

TFSize TRealDC::GetWorldExt(void)
{
   return WorldExt;
}

TPoint TRealDC::GetViewOrg(void)
{
   return ViewOrg;
}

TSize TRealDC::GetViewExt(void)
{
   return ViewExt;
}

void TRealDC::AdjustViewExt(void)
{
   int xVE, yVE;

   xVE = (ViewExt.cx/abs(ViewExt.cx))*WorldExt.dx*abs(ViewExt.cy)/WorldExt.dy;
   yVE = (ViewExt.cy/abs(ViewExt.cy))*WorldExt.dy*abs(ViewExt.cx)/WorldExt.dx;

   if (xVE > ViewExt.cx)
      ViewExt.cy = yVE;
   else
      ViewExt.cx = xVE;
}

void TRealDC::WPtoDP(double wx,double wy,int &vx,int &vy)
{
   vx = (int)((wx - WorldOrg.x)*(ViewExt.cx)/WorldExt.dx + ViewOrg.x);
   vy = (int)((wy - WorldOrg.y)*(ViewExt.cy)/WorldExt.dy + ViewOrg.y);
}

void TRealDC::WPtoDP(const TFPoint &point,TPoint &p)
{
   WPtoDP(point.x,point.y,p.x,p.y);
}

bool TRealDC::Rotate(const TFPoint &center,const double angle,TFPoint *points,int count)
{
   TFPoint p1, p2;
   int i;

   if (count < 1)
      return FALSE;

   for (i = 0; i < count; i++)
      {
      p1 = points[i] - center;
      p2.x = p1.x*cos(angle) + p1.y*sin(angle);
      p2.y = p1.y*cos(angle) - p1.x*sin(angle);
      points[i] = p2 + center;
      }

   return TRUE;
}

