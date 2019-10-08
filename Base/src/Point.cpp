/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/


#include "SmrFramework.h"

namespace SmrFramework {

  Point::Point() {
    this->x = 0;
    this->y = 0;
    }

  Point::Point(double x, double y) {
    objectType = (char*)"Point";
    this->x = x;
    this->y = y;
    }

  Point::~Point() {
    }

  Point Point::Translate(double deltaX, double deltaY) {
    return Point(x + deltaX, y + deltaY);
    }

  Point Point::Translate(Point point) {
    return Point(x + point.X(), y + point.Y());
    }

  Point Point::Translate(Point* point) {
    return Point(x + point->X(), y + point->Y());
    }

  double Point::X() {
    return x;
    }

  double Point::Y() {
    return y;
    }

  }

