/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/


#include "string.h"
#include "SmrFramework.h"

namespace SmrFramework {

  Length::Length() {
    value = 0;
    objectType = (char*)"Length";
    }
  
  Length::Length(Double v, Byte unit) {
    value = convert(v, unit);
    objectType = (char*)"Length";
    }

  Length::~Length() {
    }
  
  Double Length::convert(Double v, Byte unit) {
    switch (unit) {
      case 0: return v;
      case 1: return v * 1000;
      case 2: return v / 100;
      case 3: return Convert::InchesToMeters(v);
      case 4: return Convert::FeetToMeters(v);
      case 5: return Convert::YardsToMeters(v);
      case 6: return Convert::MilesToMeters(v);
      case 7: return Convert::LeaguesToMeters(v);
      case 8: return Convert::FathomsToMeters(v);
      case 9: return Convert::FurlongsToMeters(v);
      default: throw InvalidOpException(this, "Invalid conversion");
      }
    }

  Length Length::operator+ (const Length& a) {
    return Length(value + a.value, METERS);
    }

  Length Length::operator- (const Length& a) {
    return Length(value - a.value, METERS);
    }

  Length Length::operator* (const Length& a) {
    return Length(value * a.value, METERS);
    }

  Length Length::operator/ (const Length& a) {
    return Length(value / a.value, METERS);
    }

  Boolean Length::operator> (const Length& a) {
    return (value > a.value) ? true : false;
    }

  Boolean Length::operator< (const Length& a) {
    return (value < a.value) ? true : false;
    }

  Boolean Length::operator>= (const Length& a) {
    return (value >= a.value) ? true : false;
    }

  Boolean Length::operator<= (const Length& a) {
    return (value <= a.value) ? true : false;
    }

  Boolean Length::operator== (const Length& a) {
    return (value == a.value) ? true : false;
    }

  Boolean Length::operator!= (const Length& a) {
    return (value != a.value) ? true : false;
    }

  Length Length::operator+= (const Length& a) {
    value += a.value;
    return Length(value, METERS);
    }

  Length Length::operator-= (const Length& a) {
    value -= a.value;
    return Length(value, METERS);
    }

  Length Length::operator*= (const Length& a) {
    value *= a.value;
    return Length(value, METERS);
    }

  Length Length::operator/= (const Length& a) {
    value /= a.value;
    return Length(value, METERS);
    }

  Length Length::Add(Length a) {
    return Length(value + a.AsMeters(), METERS);
    }

  Length Length::Add(Double v, Byte unit) {
    return Length(value + convert(v, unit), METERS);
    }

  Double Length::AsCentimeters() {
    return value * 100.0;
    }

  Double Length::AsFathoms() {
    return Convert::MetersToFathoms(value);
    }

  Double Length::AsFeet() {
    return Convert::MetersToFeet(value);
    }

  Double Length::AsFurlongs() {
    return Convert::MetersToFurlongs(value);
    }

  Double Length::AsInches() {
    return Convert::MetersToInches(value);
    }

  Double Length::AsKilometers() {
    return value / 1000.0;
    }

  Double Length::AsLeagues() {
    return Convert::MetersToLeagues(value);
    }

  Double Length::AsMeters() {
    return value;
    }

  Double Length::AsMiles() {
    return Convert::MetersToMiles(value);
    }

  Double Length::AsYards() {
    return Convert::MetersToYards(value);
    }

  Length Length::Div(Length a) {
    return Length(value / a.AsMeters(), METERS);
    }

  Length Length::Div(Double v, Byte unit) {
    return Length(value / convert(v, unit), METERS);
    }

  Boolean Length::Equals(Length* b) {
    return (this->value == b->value);
    }

  Length Length::Mul(Length a) {
    return Length(value * a.AsMeters(), METERS);
    }

  Length Length::Mul(Double v, Byte unit) {
    return Length(value * convert(v, unit), METERS);
    }

  Length Length::Sub(Length a) {
    return Length(value - a.AsMeters(), METERS);
    }

  Length Length::Sub(Double v, Byte unit) {
    return Length(value - convert(v, unit), METERS);
    }
  
  }

