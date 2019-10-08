/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

#include <stdio.h>
#include "string.h"
#include "SmrFramework.h"

using namespace SmrFramework;

ComplexNumber::ComplexNumber(double r, double i) {
  objectType = (char*)"ComplexNumber";
  this->real = r;
  this->imaginary = i;
  }

ComplexNumber::~ComplexNumber() {
  }

void ComplexNumber::operator =(const ComplexNumber& b) {
  this->real = b.real;
  this->imaginary = b.imaginary;
  }

Boolean ComplexNumber::operator == (const ComplexNumber& b) {
  if (this->real != b.real) return false;
  if (this->imaginary != b.imaginary) return false;
  return true;
  }

Boolean ComplexNumber::operator != (const ComplexNumber& b) {
  if (this->real == b.real) return false;
  if (this->imaginary == b.imaginary) return false;
  return true;
  }

ComplexNumber ComplexNumber::operator +(const ComplexNumber& b) {
  double r,i;
  r = this->real + b.real;
  i = this->imaginary + b.imaginary;
  return ComplexNumber(r, i);
  }

ComplexNumber ComplexNumber::operator -(const ComplexNumber& b) {
  double r,i;
  r = this->real - b.real;
  i = this->imaginary - b.imaginary;
  return ComplexNumber(r, i);
  }

ComplexNumber ComplexNumber::operator *(const ComplexNumber& b) {
  double r,i;
  r = (this->real * b.real)      - (this->imaginary * b.imaginary);
  i = (this->real * b.imaginary) + (this->imaginary * b.real);
  return ComplexNumber(r, i);
  }

ComplexNumber ComplexNumber::operator /(const ComplexNumber& b) {
  double ar, ai, br, bi;
  double r, i;
  ar = this->real;
  ai = this->imaginary;
  br = b.real;
  bi = b.imaginary;
  r = (ar * br + ai * bi) / (br * br + bi * bi);
  i = (ai * br - ar * bi) / (br * br + bi * bi);
  return ComplexNumber(r, i);
  }

ComplexNumber ComplexNumber::operator +=(const ComplexNumber& b) {
  double r,i;
  r = this->real + b.real;
  i = this->imaginary + b.imaginary;
  this->real = r;
  this->imaginary = i;
  return ComplexNumber(r, i);
  }

ComplexNumber ComplexNumber::operator -=(const ComplexNumber& b) {
  double r,i;
  r = this->real - b.real;
  i = this->imaginary - b.imaginary;
  this->real = r;
  this->imaginary = i;
  return ComplexNumber(r, i);
  }

ComplexNumber ComplexNumber::operator *=(const ComplexNumber& b) {
  double r,i;
  r = (this->real * b.real)      - (this->imaginary * b.imaginary);
  i = (this->real * b.imaginary) + (this->imaginary * b.real);
  this->real = r;
  this->imaginary = i;
  return ComplexNumber(r, i);
  }

ComplexNumber ComplexNumber::operator /=(const ComplexNumber& b) {
  double ar, ai, br, bi;
  double r, i;
  ar = this->real;
  ai = this->imaginary;
  br = b.real;
  bi = b.imaginary;
  r = (ar * br + ai * bi) / (br * br + bi * bi);
  i = (ai * br - ar * bi) / (br * br + bi * bi);
  this->real = r;
  this->imaginary = i;
  return ComplexNumber(r, i);
  }

double ComplexNumber::Abs() {
  return Math::Sqrt(Math::Sqr(real) + Math::Sqr(imaginary));
  }

ComplexNumber ComplexNumber::Conj() {
  double r, i;
  r = this->real;
  i = -(this->imaginary);
  return ComplexNumber(r, i);
  }

ComplexNumber ComplexNumber::Cos() {
  double r, i;
  r = Math::Cos(this->real) * Math::CosH(this->imaginary);
  i = -Math::Sin(this->real) * Math::SinH(this->imaginary);
  return ComplexNumber(r, i);
  }

ComplexNumber ComplexNumber::Cosec() {
  ComplexNumber s(0,0);
  ComplexNumber one(1,0);
  ComplexNumber ret(0,0);
  s = Sin();
  ret = one.Div(s);
  return ret;
  }

ComplexNumber ComplexNumber::Cotan() {
  ComplexNumber c(0,0);
  ComplexNumber s(0,0);
  ComplexNumber ret(0,0);
  s = Sin();
  c = Cos();
  ret = c.Div(s);
  return ret;
  }

ComplexNumber ComplexNumber::Exp() {
  double r, i, mag;
  mag = Math::Exp(this->real);
  r = mag * Math::Cos(this->imaginary);
  i = mag * Math::Sin(this->imaginary);
  return ComplexNumber(r, i);
  }

double ComplexNumber::Imaginary() {
  return this->imaginary;
  }

ComplexNumber ComplexNumber::Log() {
  double ar, ai;
  double r, i;
  ar = this->real;
  ai = this->imaginary;
  r = Math::Log(Math::Sqrt(ar * ar + ai * ai));
  i = Math::ATan(ai / ar);
  return ComplexNumber(r, i);
  }

ComplexNumber ComplexNumber::Neg() {
  return ComplexNumber(-(this->real), -(this->imaginary));
  }

ComplexNumber ComplexNumber::Pow(ComplexNumber b) {
  ComplexNumber l(0,0);
  ComplexNumber m(0,0);
  ComplexNumber ret(0,0);
  l = Log();
  m = b.Mul(l);
  ret = m.Exp();
  return ret;
  }

double ComplexNumber::Real() {
  return this->real;
  }

ComplexNumber ComplexNumber::Sec() {
  ComplexNumber c(0,0);
  ComplexNumber one(1,0);
  ComplexNumber ret(0,0);
  c = Cos();
  ret = one.Div(c);
  return ret;
  }

ComplexNumber ComplexNumber::Sin() {
  double r, i;
  r = Math::Sin(this->real) * Math::CosH(this->imaginary);
  i = Math::Cos(this->real) * Math::SinH(this->imaginary);
  return ComplexNumber(r, i);
  }

ComplexNumber ComplexNumber::Sqr() {
  return Mul(*this);
  }

ComplexNumber ComplexNumber::Sqrt() {
  double r, i;
  i = Math::Sqrt((Abs() - this->real) / 2);
  r = this->imaginary / (2 * i);
  return ComplexNumber(r, i);
  }

ComplexNumber ComplexNumber::Tan() {
  ComplexNumber c(0,0);
  ComplexNumber s(0,0);
  ComplexNumber ret(0,0);
  s = Sin();
  c = Cos();
  ret = s.Div(c);
  return ret;
  }

ComplexNumber ComplexNumber::Add(ComplexNumber b) {
  double r,i;
  r = this->real + b.real;
  i = this->imaginary + b.imaginary;
  return ComplexNumber(r, i);
  }

ComplexNumber ComplexNumber::Sub(ComplexNumber b) {
  double r,i;
  r = this->real - b.real;
  i = this->imaginary - b.imaginary;
  return ComplexNumber(r, i);
  }

ComplexNumber ComplexNumber::Mul(ComplexNumber b) {
  double r,i;
  r = (this->real * b.real)      - (this->imaginary * b.imaginary);
  i = (this->real * b.imaginary) + (this->imaginary * b.real);
  return ComplexNumber(r, i);
  }

ComplexNumber ComplexNumber::Div(const ComplexNumber b) {
  double ar, ai, br, bi;
  double r, i;
  ar = this->real;
  ai = this->imaginary;
  br = b.real;
  bi = b.imaginary;
  r = (ar * br + ai * bi) / (br * br + bi * bi);
  i = (ai * br - ar * bi) / (br * br + bi * bi);
  return ComplexNumber(r, i);
  }

String ComplexNumber::ToString() {
  char buffer[64];
  char buffer2[32];
  sprintf(buffer,"%f",real);
  if (imaginary < 0) {
    sprintf(buffer2,"-%fi",-imaginary);
    }
  else {
    sprintf(buffer2,"+%fi",imaginary);
    }
  strcat(buffer, buffer2);
  return String(buffer);
  }


