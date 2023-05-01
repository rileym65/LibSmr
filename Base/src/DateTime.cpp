/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/


#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include "SmrFramework.h"

namespace SmrFramework {

  DateTime::DateTime() {
    struct timeval tv;
    tzOffset = timezone / 3600;
    this->objectType = (char*)"DateTime";
    epochSeconds = time(NULL);
    gettimeofday(&tv, NULL);
    this->microsecond= (int)tv.tv_usec;
    timeMode = 'L';
    setupTime();
    }

  DateTime::DateTime(const char* dt) {
    _convertFromString(dt);
    }

  DateTime::DateTime(String dt) {
    _convertFromString(dt.AsCharArray());
    }

  DateTime::DateTime(String* dt) {
    _convertFromString(dt->AsCharArray());
    }

  DateTime::DateTime(char mode) {
    struct timeval tv;
    this->objectType = (char*)"DateTime";
    if (mode != 'l' && mode != 'L' && mode != 'u' && mode != 'U') mode = 'L';
    epochSeconds = time(NULL);
    gettimeofday(&tv, NULL);
    this->microsecond= (int)tv.tv_usec;
    timeMode = mode;
    setupTime();
    }

  DateTime::DateTime(const char* dt,char mode) {
    if (mode != 'l' && mode != 'L' && mode != 'u' && mode != 'U') mode = 'L';
    timeMode = mode;
    _convertFromString(dt);
    }

  DateTime::DateTime(String dt, char mode) {
    if (mode != 'l' && mode != 'L' && mode != 'u' && mode != 'U') mode = 'L';
    timeMode = mode;
    _convertFromString(dt.AsCharArray());
    }

  DateTime::DateTime(String* dt, char mode) {
    if (mode != 'l' && mode != 'L' && mode != 'u' && mode != 'U') mode = 'L';
    timeMode = mode;
    _convertFromString(dt->AsCharArray());
    }

  DateTime::DateTime(int mo,int dy,int yr,int hr,int mn,int sc) {
    struct tm time;
    time.tm_hour = hr;
    time.tm_min = mn;
    time.tm_sec = sc;
    time.tm_mon = mo - 1;
    time.tm_mday = dy;
    time.tm_year = yr - 1900;
    time.tm_isdst = -1;
    epochSeconds = mktime(&time);
    timeMode = 'L';
    setupTime();
    microsecond = 0;
    }

  DateTime::DateTime(int mo,int dy,int yr,int hr,int mn,int sc,int ms) {
    struct tm time;
    time.tm_hour = hr;
    time.tm_min = mn;
    time.tm_sec = sc;
    time.tm_mon = mo - 1;
    time.tm_mday = dy;
    time.tm_year = yr - 1900;
    time.tm_isdst = -1;
    epochSeconds = mktime(&time);
    timeMode = 'L';
    setupTime();
    microsecond = ms;
    }

  DateTime::~DateTime() {
    }

  void DateTime::_convertFromString(const char* dt) {
    int hr,mn,sc,mo,dy,yr;
    char c1,c2;
    struct tm time;
    timeMode = 'L';
    yr = 0; mo = 0; dy = 0; hr = 0; mn = 0; sc = 0;
    while (*dt > 0 && *dt <= ' ') dt++;
    c1 = dt[1]; c2 = dt[2];
    while (*dt >= '0' && *dt <= '9') yr = (yr * 10) + (*dt++ - '0');
    if (*dt != '/' && *dt != '-')
      throw InvalidOpException(this, "Unknown Date/Time format");
    dt++;
    while (*dt >= '0' && *dt <= '9') mo = (mo * 10) + (*dt++ - '0');
    if (*dt != '/' && *dt != '-')
      throw InvalidOpException(this, "Unknown Date/Time format");
    dt++;
    while (*dt >= '0' && *dt <= '9') dy = (dy * 10) + (*dt++ - '0');
    if (c1 == '-' || c1 == '/' || c2 == '-' || c2 == '/') {
      Utils::Swap(yr, dy);
      Utils::Swap(mo, dy);
      }
    if (*dt != ' ' && *dt != 't' && *dt != 'T' && *dt != 0)
      throw InvalidOpException(this, "Unknown Date/Time format");
    if (*dt != 0) {
      dt++;
      while (*dt >= '0' && *dt <= '9') hr = (hr * 10) + (*dt++ - '0');
      if (*dt != ':')
        throw InvalidOpException(this, "Unknown Date/Time format");
      dt++;
      while (*dt >= '0' && *dt <= '9') mn = (mn * 10) + (*dt++ - '0');
      if (*dt != ':' && *dt != 0)
        throw InvalidOpException(this, "Unknown Date/Time format");
      if (*dt != 0) {
        dt++;
        while (*dt >= '0' && *dt <= '9') sc = (sc * 10) + (*dt++ - '0');
        }
      if (*dt != 0 && *dt != 'z' && *dt != 'Z')
        throw InvalidOpException(this, "Unknown Date/Time format");
      if (*dt == 'z' || *dt == 'Z') timeMode = 'U';
      }
    if (mo < 1 || mo > 12) throw RangeException(this, "Month not in range 1-12");
    if (dy < 1 || dy > DaysInMonth(mo,yr))
      throw RangeException(this, "Day is not a valid value");
    if (hr < 0 || hr > 23) throw RangeException(this, "Hour is out of range");
    if (mn < 0 || mn > 59) throw RangeException(this, "Minute is out of range");
    if (sc < 0 || sc > 59) throw RangeException(this, "Second is out of range");
    time.tm_hour = hr;
    time.tm_min = mn;
    time.tm_sec = sc;
    time.tm_mon = mo - 1;
    time.tm_mday = dy;
    time.tm_year = yr - 1900;
    time.tm_isdst = -1;
    epochSeconds = mktime(&time);
    setupTime();
    }

  int DateTime::DaysInMonth(int month, int year) {
    if (month==1 || month==3 || month==5 || month==7 || month==8 ||
        month==10 || month==12) return 31;
    if (month==4 || month==6 || month==9 || month==11) return 30;
    return (DateTime::DaysInYear(year)==366)?29:28;
    }

  int DateTime::DaysInYear(int year) {
    if (year % 400 == 0) return 366;
    if (year % 100 == 0) return 365;
    if (year % 4   == 0) return 366;
    return 365;
    }

  DateTime DateTime::FromJulianDay(double jd) {
    double q;
    int z,w,x,a,b,c,d,e,f,day,month,year;
    q = jd + 0.5;
    z = (int)q;
    w = (z - 1867216.25) / 36524.25;
    x = w / 4;
    a = z + 1 + w - x;
    b = a + 1524;
    c = (b - 122.1) / 365.25;
    d = 365.25 * c;
    e = (b - d) / 30.6001;
    f = 30.6001 * e;
    day = b - d - f + (q - z);
    month = e - 1;
    if (month > 12) month -= 12;
    year = (month <= 2) ? c - 4715 : c - 4716;
    return DateTime(month, day, year, 0, 0, 0);
    }

  int DateTime::IsLeapYear(int year) {
    return (DateTime::DaysInYear(year) == 365) ? 0 : -1;
    }

  double DateTime::JulianDay(int m, int d, int y) {
    int a,b,c,e,f;
    double jd;
    if (m <= 2) {
      y--;
      m += 12;
      }
    a = y / 100;
    b = a / 4;
    c = 2 - a + b;
    e = 365.25 * (y + 4716);
    f = 30.6001 * (m + 1);
    jd = c + d + e + f - 1524.5;
    return jd;
    }

  int DateTime::Compare(DateTime* a, DateTime* b) {
    double jda, jdb;
    jda = a->ToDouble();
    jdb = b->ToDouble();
    if (jda > jdb) return 1;
    if (jda < jdb) return -1;
    return 0;
    }

  DateTime DateTime::Now() {
    return DateTime();
    }

  DateTime DateTime::Now(char mode) {
    return DateTime(mode);
    }

  void DateTime::operator = (const DateTime &dt) {
    this->month = dt.month;
    this->day = dt.day;
    this->year = dt.year;
    this->hour = dt.hour;
    this->minute = dt.minute;
    this->second = dt.second;
    this->epochSeconds = dt.epochSeconds;
    this->timeMode = dt.timeMode;
    this->microsecond = dt.microsecond;
    }

  Boolean DateTime::operator == (const DateTime &dt) {
    if (this->month != dt.month) return false;
    if (this->day != dt.day) return false;
    if (this->year != dt.year) return false;
    if (this->hour != dt.hour) return false;
    if (this->minute != dt.minute) return false;
    if (this->second != dt.second) return false;
    if (this->microsecond != dt.microsecond) return false;
    return true;
    }

  Boolean DateTime::operator != (const DateTime &dt) {
    if (this->month != dt.month) return true;
    if (this->day != dt.day) return true;
    if (this->year != dt.year) return true;
    if (this->hour != dt.hour) return true;
    if (this->minute != dt.minute) return true;
    if (this->second != dt.second) return true;
    if (this->microsecond != dt.microsecond) return true;
    return false;
    }

  Boolean DateTime::operator > (const DateTime &dt) {
    if (this->year < dt.year) return false;
    if (this->year > dt.year) return true;
    if (this->month < dt.month) return false;
    if (this->month > dt.month) return true;
    if (this->day < dt.day) return false;
    if (this->day > dt.day) return true;
    if (this->hour < dt.hour) return false;
    if (this->hour > dt.hour) return true;
    if (this->minute < dt.minute) return false;
    if (this->minute > dt.minute) return true;
    if (this->second < dt.second) return false;
    if (this->second > dt.second) return true;
    if (this->microsecond < dt.microsecond) return false;
    if (this->microsecond > dt.microsecond) return true;
    return false;
    }

  Boolean DateTime::operator >= (const DateTime &dt) {
    if (this->year < dt.year) return false;
    if (this->year > dt.year) return true;
    if (this->month < dt.month) return false;
    if (this->month > dt.month) return true;
    if (this->day < dt.day) return false;
    if (this->day > dt.day) return true;
    if (this->hour < dt.hour) return false;
    if (this->hour > dt.hour) return true;
    if (this->minute < dt.minute) return false;
    if (this->minute > dt.minute) return true;
    if (this->second < dt.second) return false;
    if (this->second > dt.second) return true;
    if (this->microsecond < dt.microsecond) return false;
    if (this->microsecond > dt.microsecond) return true;
    return true;
    }

  Boolean DateTime::operator < (const DateTime &dt) {
    if (this->year > dt.year) return false;
    if (this->year < dt.year) return true;
    if (this->month > dt.month) return false;
    if (this->month < dt.month) return true;
    if (this->day > dt.day) return false;
    if (this->day < dt.day) return true;
    if (this->hour > dt.hour) return false;
    if (this->hour < dt.hour) return true;
    if (this->minute > dt.minute) return false;
    if (this->minute < dt.minute) return true;
    if (this->second > dt.second) return false;
    if (this->second < dt.second) return true;
    if (this->microsecond > dt.microsecond) return false;
    if (this->microsecond < dt.microsecond) return true;
    return false;
    }

  Boolean DateTime::operator <= (const DateTime &dt) {
    if (this->year > dt.year) return false;
    if (this->year < dt.year) return true;
    if (this->month > dt.month) return false;
    if (this->month < dt.month) return true;
    if (this->day > dt.day) return false;
    if (this->day < dt.day) return true;
    if (this->hour > dt.hour) return false;
    if (this->hour < dt.hour) return true;
    if (this->minute > dt.minute) return false;
    if (this->minute < dt.minute) return true;
    if (this->second > dt.second) return false;
    if (this->second < dt.second) return true;
    if (this->microsecond > dt.microsecond) return false;
    if (this->microsecond < dt.microsecond) return true;
    return true;
    }

  DateTime DateTime::Add(TimeSpan* value) {
    int month, day, year, hours, minutes, seconds;
    month = this->month;
    day = this->day + value->Days();
    year = this->year;
    hours = this->hour + value->Hours();
    minutes = this->minute + value->Minutes();
    seconds = this->second + value->Seconds();
    Normalize(&month,&day,&year,&hours,&minutes,&seconds);
    return DateTime(month,day,year,hours,minutes,seconds, this->microsecond);
    }

  DateTime DateTime::AddDays(int amount) {
    int month, day, year, hour, minute, second;
    month = this->month;
    day = this->day + amount;
    year = this->year;
    hour = this->hour;
    minute = this->minute;
    second = this->second;
    Normalize(&month,&day,&year,&hour,&minute,&second);
    return DateTime(month, day, year, hour, minute, second, this->microsecond);
    }

  DateTime DateTime::AddHours(int amount) {
    int month, day, year, hour, minute, second;
    month = this->month;
    day = this->day;
    year = this->year;
    hour = this->hour + amount;
    minute = this->minute;
    second = this->second;
    Normalize(&month,&day,&year,&hour,&minute,&second);
    return DateTime(month, day, year, hour, minute, second, this->microsecond);
    }

  DateTime DateTime::AddMinutes(int amount) {
    int month, day, year, hour, minute, second;
    month = this->month;
    day = this->day;
    year = this->year;
    hour = this->hour;
    minute = this->minute + amount;
    second = this->second;
    Normalize(&month,&day,&year,&hour,&minute,&second);
    return DateTime(month, day, year, hour, minute, second, this->microsecond);
    }

  DateTime DateTime::AddMonths(int amount) {
    int month, day, year, hour, minute, second;
    month = this->month + amount;
    day = this->day;
    year = this->year;
    hour = this->hour;
    minute = this->minute;
    second = this->second;
    Normalize(&month,&day,&year,&hour,&minute,&second);
    return DateTime(month, day, year, hour, minute, second, this->microsecond);
    }

  DateTime DateTime::AddSeconds(int amount) {
    int month, day, year, hour, minute, second;
    month = this->month;
    day = this->day;
    year = this->year;
    hour = this->hour;
    minute = this->minute;
    second = this->second + amount;
    Normalize(&month,&day,&year,&hour,&minute,&second);
    return DateTime(month, day, year, hour, minute, second, this->microsecond);
    }

  DateTime DateTime::AddYears(int amount) {
    int month, day, year, hour, minute, second;
    month = this->month;
    day = this->day;
    year = this->year + amount;
    hour = this->hour;
    minute = this->minute;
    second = this->second;
    Normalize(&month,&day,&year,&hour,&minute,&second);
    return DateTime(month, day, year, hour, minute, second, this->microsecond);
    }

  int DateTime::CompareTo(DateTime* b) {
    return DateTime::Compare(this, b);
    }

  int DateTime::Day() {
    return this->day;
    }

  int DateTime::Equals(DateTime* a,DateTime* b) {
    if (a->Day() != b->Day()) return 0;
    if (a->Month() != b->Month()) return 0;
    if (a->Year() != b->Year()) return 0;
    if (a->Hour() != b->Hour()) return 0;
    if (a->Minute() != b->Minute()) return 0;
    if (a->Second() != b->Second()) return 0;
    return -1;
    }

  int DateTime::Equals(DateTime* b) {
    return DateTime::Equals(this, b);
    }

  int DateTime::Hour() {
    return this->hour;
    }

  int DateTime::MicroSecond() {
    return this->microsecond;
    }

  int DateTime::Minute() {
    return this->minute;
    }

  int DateTime::Month() {
    return this->month;
    }

  void DateTime::Normalize(int* month,int* day,int* year,
                           int* hr,int* min,int* sec) {
    while (*sec < 0) {
      *sec += 60;
      *min -= 1;
      }
    while (*sec > 59) {
      *sec -= 60;
      *min += 1;
      }
    while (*min < 0) {
      *min += 60;
      *hr -= 1;
      }
    while (*min > 59) {
      *min -= 60;
      *hr += 1;
      }
    while (*hr < 0) {
      *hr += 24;
      *day -= 1;
      }
    while (*hr > 23) {
      *hr -= 24;
      *day += 1;
      }
    while (*day > DateTime::DaysInMonth(*month, *year)) {
      *day -= DateTime::DaysInMonth(*month, *year);
      *month += 1;
      if (*month > 12) {
        *month = 1;
        *year += 1;
        }
      }
    while (*month < 0) {
      *month += 12;
      *year -= 1;
      }
    while (*month > 12) {
      *month -= 12;
      *year += 1;
      }
    }

  int DateTime::Second() {
    return this->second;
    }

  void DateTime::setupTime() {
    struct tm time;
    if (timeMode == 'U')
      gmtime_r(&(this->epochSeconds), &(time));
    else
      localtime_r(&(this->epochSeconds), &(time));
    this->hour = time.tm_hour;
    this->minute = time.tm_min;
    this->second = time.tm_sec;
    this->month = time.tm_mon + 1;
    this->day = time.tm_mday;
    this->year = time.tm_year + 1900;
    }

  TimeSpan DateTime::Subtract(DateTime* b) {
    double jda, jdb;
    jda = this->ToDouble();
    jdb = b->ToDouble();
    return TimeSpan(jda - jdb);
    }

  DateTime DateTime::Subtract(TimeSpan* value) {
    int month, day, year, hours, minutes, seconds;
    month = this->month;
    day = this->day - value->Days();
    year = this->year;
    hours = this->hour - value->Hours();
    minutes = this->minute - value->Minutes();
    seconds = this->second - value->Seconds();
    Normalize(&month,&day,&year,&hours,&minutes,&seconds);
    return DateTime(month,day,year,hours,minutes,seconds);
    }

  double DateTime::ToDouble() {
    double seconds;
    seconds = (this->minute * 60) + (this->hour * 3600) + this->second;
//    seconds += ((Double)microsecond / 1000000.0);
    seconds /= 86400.0;
    return ToJulianDay() + seconds;
    }

  double DateTime::ToJulianDay() {
    return DateTime::JulianDay(this->month, this->day, this->year);
    }

  String DateTime::ToString() {
    char temp[64];
    sprintf(temp, "%04d-%02d-%02dT%02d:%02d:%02d", year, month, day, hour, minute, second);
    if (timeMode == 'U') strcat(temp,"Z");
    return String(temp);
    }

  int DateTime::Year() {
    return year;
    }

  int DateTime::DayOfWeek() {
    int dw;
    dw = ToJulianDay();
    dw = (dw % 7) + 1;
    if (dw > 6) dw -= 7;
    return dw;
    }

  int DateTime::DayOfYear() {
    int d1,d2;
    d1 = ToJulianDay();
    d2 = JulianDay(1, 1, year);
    return d1-d2;
    }

  }

