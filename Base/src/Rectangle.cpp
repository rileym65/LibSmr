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
#include "SmrFramework.h"

namespace SmrFramework {

  Rectangle::Rectangle() {
    objectType = (char*)"Rectangle";
    this->start = Point(0, 0);
    this->width = width;
    this->height = height;
    }

  Rectangle::Rectangle(double x, double y, double width, double height) {
    objectType = (char*)"Rectangle";
    this->start = Point(x, y);
    this->width = width;
    this->height = height;
    }

  Rectangle::Rectangle(Point s, Point e) {
    objectType = (char*)"Rectangle";
    this->start = Point(s.X(), s.Y());
    this->width = e.X() - s.X();
    this->height = e.Y() - s.Y();
    }

  Rectangle::Rectangle(Point* s, Point* e) {
    objectType = (char*)"Rectangle";
    this->start = Point(s->X(), s->Y());
    this->width = e->X() - s->X();
    this->height = e->Y() - s->Y();
    }

  Rectangle::~Rectangle() {
    }

  Double Rectangle::Area() {
    return width * height;
    }

  Point Rectangle::Center() {
    return Point(start.X() + width / 2,
                 start.Y() + height / 2);
    }

  double Rectangle::Height() {
    return height;
    }

  Point Rectangle::Start() {
    return start;
    }

  Rectangle Rectangle::Translate(double deltaX, double deltaY) {
    return Rectangle(start.X() + deltaX, start.Y()+deltaY, width, height);
    }

  Rectangle Rectangle::Translate(Point delta) {
    return Rectangle(start.X() + delta.X(), start.Y()+delta.Y(), width, height);
    }

  Rectangle Rectangle::Translate(Point* delta) {
    return Rectangle(start.X() + delta->X(), start.Y()+delta->Y(), width, height);
    }

  double Rectangle::Width() {
    return width;
    }
  
  double Rectangle::X() {
    return start.X();
    }

  double Rectangle::Y() {
    return start.Y();
    }

  }

