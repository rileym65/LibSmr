/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

#include <math.h>
#include "string.h"
#include "SmrFramework.h"

namespace SmrFramework {

  Polygon::Polygon() {
    objectType = (char*)"Polygon";
    points = new List<Point*>();
    pos = 0;
    }
  
  Polygon::~Polygon() {
    UInt32 i;
    for (i=0; i<points->Count(); i++) delete(points->At(i));
    delete(points);
    }
  
  UInt32 Polygon::Add(Point p) {
    points->Add(new Point(p.X(), p.Y()));
    return points->Count();
    }

  Boolean Polygon::AtEnd() {
    return (pos >= points->Count());
    }

  UInt32 Polygon::Count() {
    return points->Count();
    }

  Line Polygon::First() {
    Line  ret;
    Point* p1;
    Point* p2;
    pos = 0;
    if (points->Count() < 2) throw InvalidOpException(this, "No lines defined");
    if (pos >= points->Count()) throw RangeException(this, "Past end");
    p1 = points->At(pos++);
    p2 = points->At(pos);
    ret = Line(p1->X(), p1->Y(), p2->X(), p2->Y());
    return ret;
    }

  Line Polygon::Next() {
    Line  ret;
    Point* p1;
    Point* p2;
    pos = 0;
    if (points->Count() < 2) throw InvalidOpException(this, "No lines defined");
    if (pos >= points->Count()) throw RangeException(this, "Past end");
    p1 = points->At(pos++);
    p2 = points->At(pos);
    ret = Line(p1->X(), p1->Y(), p2->X(), p2->Y());
    return ret;
    }

  Double Polygon::Perimeter() {
    UInt32 i;
    Point* p1;
    Point* p2;
    Double ret;
    if (points->Count() < 2) throw InvalidOpException(this, "No lines defined");
    ret = 0;
    for (i=0; i<points->Count(); i++) {
      p1 = points->At(i);
      p2 = (i < points->Count()-1) ? points->At(i+1) : points->At(0);
      ret += sqrt((p1->X() - p2->X()) * (p1->X() - p2->X()) +
                  (p1->Y() - p2->Y()) * (p1->Y() - p2->Y()));
      }
    return ret;
    }

  List<Point*>* Polygon::Points() {
    return points;
    }

  void Polygon::Reset() {
    pos = 0;
    }

  }

