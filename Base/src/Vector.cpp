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

  Vector::Vector() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->objectType = (char*)"Vector";
    }

  Vector::Vector(Double x, Double y, Double z) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->objectType = (char*)"Vector";
    }

  Vector::~Vector() {
    }

  Vector Vector::operator+ (const Vector& a) {
    return Vector(this->x + a.x, this->y + a.y, this->z + a.z);
    }

  Vector Vector::operator- (const Vector& a) {
    return Vector(this->x - a.x, this->y - a.y, this->z - a.z);
    }

  void Vector::operator+= (const Vector& a) {
    this->x += a.x;
    this->y += a.y;
    this->z += a.z;
    }

  void Vector::operator-= (const Vector& a) {
    this->x -= a.x;
    this->y -= a.y;
    this->z -= a.z;
    }

  Vector Vector::operator* (const Double& a) {
    Double x,y,z;
    x = this->x * a;
    y = this->y * a;
    z = this->z * a;
    return Vector(x, y, z);
    }

  void Vector::operator*= (const Double& a) {
    this->x *= a;
    this->y *= a;
    this->z *= a;
    }

  Vector Vector::Cross(const Vector& b) {
    Double x, y, z;
    x = (this->Y() * b.z) - (this->Z() * b.y);
    y = (this->Z() * b.x) - (this->X() * b.z);
    z = (this->X() * b.y) - (this->Y() * b.x);
    return Vector(x, y, z);
    }

  Double Vector::Dot(const Vector& a) {
    return (this->x * a.x) + (this->y * a.y) + (this->z * a.z);
    }

  Double Vector::Length() {
    Double x, y, z;
    x = Math::Sqr(this->X());
    y = Math::Sqr(this->Y());
    z = Math::Sqr(this->Z());
    return Math::Sqrt(x + y + z);
    }

  Vector Vector::Neg() {
    Double x, y, z;
    x = -this->X();
    y = -this->Y();
    z = -this->Z();
    return Vector(x, y, z);
    }

  Vector Vector::Norm() {
    Double x, y, z, scale;
    scale = 1.0 / Length();
    x = this->X() * scale;
    y = this->Y() * scale;
    z = this->Z() * scale;
    return Vector(x, y, z);
    }

  Vector Vector::Scale(Double scale) {
    Double x, y, z;
    x = this->X() * scale;
    y = this->Y() * scale;
    z = this->Z() * scale;
    return Vector(x, y, z);
    }

  Double Vector::X() {
    return x;
    }

  Double Vector::Y() {
    return y;
    }

  Double Vector::Z() {
    return z;
    }

  Double Vector::X(Double v) {
    x = v;
    return x;
    }

  Double Vector::Y(Double v) {
    y = v;
    return y;
    }

  Double Vector::Z(Double v) {
    z = v;
    return z;
    }

  }

