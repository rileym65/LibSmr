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

  Line::Line() {
    objectType = (char*)"Line";
    start = Point(0,0);
    end = Point(0,0);
    computeStats();
    }

  Line::Line(double x1, double y1, double x2, double y2) {
    objectType = (char*)"Line";
    start = Point(x1, y1);
    end = Point(x2, y2);
    computeStats();
    }

  Line::Line(Point s, Point e) {
    objectType = (char*)"Line";
    this->start = s;
    this->end = e;
    computeStats();
    }

  Line::Line(Point* s, Point* e) {
    objectType = (char*)"Line";
    this->start = *s;
    this->end = *e;
    computeStats();
    }

  Line::~Line() {
    }

  void Line::computeStats() {
    double deltaX;
    double deltaY;
    deltaX = end.X() - start.X();
    deltaY = end.Y() - start.Y();
    if (deltaX == 0 && deltaY == 0) {
      slope = 0;
      offset = 0;
      return;
      }
    if (deltaX == 0) deltaX = 1e-127;
    slope = deltaY / deltaX;
    offset = start.Y() - (slope * start.X());
    }

  Point Line::Center() {
    return Point(start.X() + (end.X() - start.X()) / 2,
                 start.Y() + (end.Y() - start.Y()) / 2);
    }

  Point Line::End() {
    return end;
    }

  double Line::Height() {
    return end.Y() - start.Y();
    }

  Boolean Line::Intersects(Line *b) {
    Double intersection;
    intersection = IntersectPos(b);
    if (start.X() <= end.X()) {
      if (intersection < start.X()) return false;
      if (intersection > end.X()) return false;
      }
    else {
      if (intersection < end.X()) return false;
      if (intersection > start.X()) return false;
      }
    if (b->Start().X() <= b->End().X()) {
      if (intersection < b->Start().X()) return false;
      if (intersection > b->End().X()) return false;
      }
    else {
      if (intersection < b->End().X()) return false;
      if (intersection > b->Start().X()) return false;
      }
    return true;
    }

  Double Line::IntersectPos(Line *b) {
    return (b->Offset() - offset) / (slope - b->Slope());
    }

  double Line::Length() {
    double x;
    double y;
    x = end.X() - start.X();
    y = end.Y() - start.Y();
    x *= x;
    y *= y;
    return sqrt(x + y);
    }

  double Line::Offset() {
    return offset;
    }

  void Line::PointToRef(Point p, Double* dist, Double* fract) {
    Double d;
    Double x;
    Double y;
    x = end.X() - start.X();
    y = end.Y() - start.Y();
    d = sqrt(x * x + y * y);
    *fract = ((p.X()-start.X())*x + (p.Y()-start.Y())*y)/(d*d);
    *dist =  ((((p.X()-start.X())*-y + (p.Y()-start.Y())*x)/d)/d);
    }

  Point Line::RefToPoint(Double dist, Double fract) {
    Double odist;
    Double x;
    Double y;
    Double xd;
    Double yd;
    xd = end.X() - start.X();
    yd = end.Y() - start.Y();
    odist = sqrt(xd*xd + yd*yd);
    dist *= odist;
    x = start.X() + fract * xd - dist * yd / odist + .5;
    y = start.Y() + fract * yd + dist * xd / odist + .5;
    return Point(x, y);
    }

  double Line::Slope() {
    return slope;
    }

  Point Line::Start() {
    return start;
    }

  Line Line::Translate(double deltaX, double deltaY) {
    return Line(start.Translate(deltaX, deltaY), end.Translate(deltaX, deltaY));
    }

  Line Line::Translate(Point delta) {
    return Line(start.Translate(delta), end.Translate(delta));
    }

  Line Line::Translate(Point* delta) {
    return Line(start.Translate(delta), end.Translate(delta));
    }

  double Line::Width() {
    return end.X() - start.X();
    }

  }

