/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

#include <string.h>
#include <stdio.h>
#include "SmrFramework.h"

namespace SmrFramework {

  TimeSpan::TimeSpan() {
    this->days = 0;
    this->hours = 0;
    this->minutes = 0;
    this->seconds = 0;
    objectType = (char*)"TimeSpan";
    }

  TimeSpan::TimeSpan(String* str) {
    Int32 d, h, m, s;
    List<String*>* parts;
    objectType = (char*)"TimeSpan";
    if (str == NULL) {
      this->days = 0;
      this->hours = 0;
      this->minutes = 0;
      this->seconds = 0;
      return;
      }
    parts = str->Split(':');
    d = 0;
    h = 0;
    m = 0;
    s = 0;
    switch (parts->Count()) {
      case 1:
           s = Convert::ToUInt32(parts->At(0));
           break;
      case 2:
           m= Convert::ToUInt32(parts->At(0));
           s= Convert::ToUInt32(parts->At(1));
           break;
      case 3:
           h= Convert::ToUInt32(parts->At(0));
           m= Convert::ToUInt32(parts->At(1));
           s= Convert::ToUInt32(parts->At(2));
           break;
      case 4:
           d= Convert::ToUInt32(parts->At(0));
           h= Convert::ToUInt32(parts->At(1));
           m= Convert::ToUInt32(parts->At(2));
           s= Convert::ToUInt32(parts->At(3));
           break;
      }
    TimeSpan::Normalize(&d, &h, &m, &s);
    this->days = d;
    this->hours = h;
    this->minutes = m;
    this->seconds = s;
    }

  TimeSpan::TimeSpan(int d,int h,int m, int s) {
    TimeSpan::Normalize(&d, &h, &m, &s);
    this->days = d;
    this->hours = h;
    this->minutes = m;
    this->seconds = s;
    objectType = (char*)"TimeSpan";
    }

  TimeSpan::TimeSpan(double value) {
    int secs;
    int hours;
    int days;
    days = (int)value;
    value -= days;
    secs = (86400 * value);
    hours = secs / 3600;
    secs -= hours * 3600;
    minutes = secs / 60;
    secs -= minutes * 60;
    this->days = days;
    this->hours = hours;
    this->minutes = minutes;
    this->seconds = secs;
    }

  TimeSpan::~TimeSpan() {
    }

  int TimeSpan::Compare(TimeSpan* tsa, TimeSpan* tsb) {
    double a,b;
    a = tsa->ToDouble();
    b = tsb->ToDouble();
    if (a > b) return 1;
    if (a < b) return -1;
    return 0;
    }

  TimeSpan TimeSpan::Add(TimeSpan value) {
    int hours;
    int minutes;
    int seconds;
    int days;
    seconds = this->seconds + value.Seconds();
    minutes = this->minutes + value.Minutes();
    hours = this->hours + value.Hours();
    days = this->days + value.Days();
    TimeSpan::Normalize(&days, &hours, &minutes, &seconds);
    return TimeSpan(days, hours, minutes, seconds);
    }

  TimeSpan TimeSpan::Add(TimeSpan* value) {
    int hours;
    int minutes;
    int seconds;
    int days;
    seconds = this->seconds + value->Seconds();
    minutes = this->minutes + value->Minutes();
    hours = this->hours + value->Hours();
    days = this->days + value->Days();
    TimeSpan::Normalize(&days, &hours, &minutes, &seconds);
    return TimeSpan(days, hours, minutes, seconds);
    }

  TimeSpan TimeSpan::AddDays(int amount) {
    int hours;
    int minutes;
    int seconds;
    int days;
    seconds = this->seconds;
    minutes = this->minutes;
    hours = this->hours;
    days = this->days + amount;
    TimeSpan::Normalize(&days, &hours, &minutes, &seconds);
    return TimeSpan(days, hours, minutes, seconds);
    }

  TimeSpan TimeSpan::AddHours(int amount) {
    int hours;
    int minutes;
    int seconds;
    int days;
    seconds = this->seconds;
    minutes = this->minutes;
    hours = this->hours + amount;
    days = this->days;
    TimeSpan::Normalize(&days, &hours, &minutes, &seconds);
    return TimeSpan(days, hours, minutes, seconds);
    }

  TimeSpan TimeSpan::AddMinutes(int amount) {
    int hours;
    int minutes;
    int seconds;
    int days;
    seconds = this->seconds;
    minutes = this->minutes + amount;
    hours = this->hours;
    days = this->days;
    TimeSpan::Normalize(&days, &hours, &minutes, &seconds);
    return TimeSpan(days, hours, minutes, seconds);
    }

  TimeSpan TimeSpan::AddSeconds(int amount) {
    int hours;
    int minutes;
    int seconds;
    int days;
    seconds = this->seconds + amount;
    minutes = this->minutes;
    hours = this->hours;
    days = this->days;
    TimeSpan::Normalize(&days, &hours, &minutes, &seconds);
    return TimeSpan(days, hours, minutes, seconds);
    }

  Double TimeSpan::AsSeconds() {
    Double ret;
    ret = seconds;
    ret += (minutes * 60);
    ret += (hours * 3600);
    ret += (days * 86400);
    return ret;
    }

  Double TimeSpan::AsMinutes() {
    Double ret;
    ret = ((Double)seconds / 60.0);
    ret += minutes;
    ret += (hours * 60);
    ret += (days * 1440);
    return ret;
    }

  Double TimeSpan::AsHours() {
    Double ret;
    ret = seconds + (minutes * 60);
    ret = ret / 3600.0;
    ret += hours;
    ret += (days * 24);
    return ret;
    }

  Double TimeSpan::AsDays() {
    Double ret;
    ret = seconds;
    ret += (minutes * 60);
    ret += (hours * 3600);
    ret = ret / 86400.0;
    ret += days;
    return ret;
    }

  int TimeSpan::CompareTo(TimeSpan* tsb) {
    return TimeSpan::Compare(this, tsb);
    }

  int TimeSpan::Days() {
    return this->days;
    }

  int TimeSpan::Hours() {
    return this->hours;
    }

  int TimeSpan::Minutes() {
    return this->minutes;
    }

  Boolean TimeSpan::Negative() {
    return (days >= 0);
    }

  void TimeSpan::Normalize(int* days,int* hours,int* mins,int* secs) {
    while (*secs < 0) {
      *secs += 60;
      *mins -= 1;
      }
    while (*secs > 59) {
      *secs -= 60;
      *mins += 1;
      }
    while (*mins < 0) {
      *mins += 60;
      *hours -= 1;
      }
    while (*mins > 59) {
      *mins -= 60;
      *hours += 1;
      }
    while (*hours < 0) {
      *hours += 24;
      *days -= 1;
      }
    while (*hours > 23) {
      *hours -= 24;
      *days += 1;
      }
    }

  int TimeSpan::Seconds() {
    return this->seconds;
    }

  TimeSpan TimeSpan::Subtract(TimeSpan* value) {
    int hours;
    int minutes;
    int seconds;
    int days;
    seconds = this->seconds - value->Seconds();
    minutes = this->minutes - value->Minutes();
    hours = this->hours - value->Hours();
    days = this->days - value->Days();
    TimeSpan::Normalize(&days, &hours, &minutes, &seconds);
    return TimeSpan(days, hours, minutes, seconds);
    }

  double TimeSpan::ToDouble() {
    double days, hours, minutes, seconds;
    double fractional;
    days = this->days;
    hours = this->hours;
    minutes = this->minutes;
    seconds = this->seconds;
    fractional = (seconds + (minutes * 60) + (hours * 3600)) / 86400.0;
    return days + fractional;
    }

  String TimeSpan::ToString() {
    char temp[64];
    sprintf(temp, "%d:%02d:%02d:%02d", days, hours, minutes, seconds);
    return String(temp);
    }


  }

