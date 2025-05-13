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

  DateTime::DateTime(double jd) {
    struct tm time;
    double q;
    int seconds;
    int hours;
    int minutes;
    int z,w,x,a,b,c,d,e,f,day,month,year;
    q = (jd - (int)jd) * 86400;
    seconds = (int)q;
    q = jd + 1;
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
    hours = seconds / 3600;
    seconds -= (hours * 3600);
    minutes = seconds / 60;
    seconds -= (minutes * 60);
    time.tm_hour = hours;
    time.tm_min = minutes;
    time.tm_sec = seconds;
    time.tm_mon = month - 1;
    time.tm_mday = day;
    time.tm_year = year - 1900;
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
    int seconds;
    int hours;
    int minutes;
    int z,w,x,a,b,c,d,e,f,day,month,year;
    q = (jd - (int)jd) * 86400;
    seconds = (int)q;
    q = jd + 1;
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
    hours = seconds / 3600;
    seconds -= (hours * 3600);
    minutes = seconds / 60;
    seconds -= (minutes * 60);
    return DateTime(month, day, year, hours, minutes, seconds);
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
    jd = c + d + e + f - 1525;
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

  String DateTime::ToString(const char* format) {
    int i;
    char buffer[1024];
    char tmp[16];
    int  pos;
    pos = 0;
    while (*format != 0) {
      if (*format == 'd' && *(format+1) == 'o' && *(format+2) == 'w' &&
          *(format+3) == 'l') {
        format += 4;
        switch(DayOfWeek()) {
          case 0: strcpy(tmp, "Monday"); break;
          case 1: strcpy(tmp, "Tuesday"); break;
          case 2: strcpy(tmp, "Wednesday"); break;
          case 3: strcpy(tmp, "Thursday"); break;
          case 4: strcpy(tmp, "Friday"); break;
          case 5: strcpy(tmp, "Saturday"); break;
          case 6: strcpy(tmp, "Sunday"); break;
          }
        }
      else if (*format == 'D' && *(format+1) == 'O' && *(format+2) == 'W' &&
               *(format+3) == 'L') {
        format += 4;
        switch(DayOfWeek()) {
          case 0: strcpy(tmp, "MONDAY"); break;
          case 1: strcpy(tmp, "TUESDAY"); break;
          case 2: strcpy(tmp, "WEDNESDAY"); break;
          case 3: strcpy(tmp, "THURSDAY"); break;
          case 4: strcpy(tmp, "FRIDAY"); break;
          case 5: strcpy(tmp, "SATURDAY"); break;
          case 6: strcpy(tmp, "SUNDAY"); break;
          }
        }
      else if (*format == 'd' && *(format+1) == 'o' && *(format+2) == 'w') {
        format += 3;
        switch(DayOfWeek()) {
          case 0: strcpy(tmp, "Mon"); break;
          case 1: strcpy(tmp, "Tue"); break;
          case 2: strcpy(tmp, "Wed"); break;
          case 3: strcpy(tmp, "Thu"); break;
          case 4: strcpy(tmp, "Fri"); break;
          case 5: strcpy(tmp, "Sat"); break;
          case 6: strcpy(tmp, "Sun"); break;
          }
        }
      else if (*format == 'D' && *(format+1) == 'O' && *(format+2) == 'W') {
        format += 3;
        switch(DayOfWeek()) {
          case 0: strcpy(tmp, "MON"); break;
          case 1: strcpy(tmp, "TUE"); break;
          case 2: strcpy(tmp, "WED"); break;
          case 3: strcpy(tmp, "THU"); break;
          case 4: strcpy(tmp, "FRI"); break;
          case 5: strcpy(tmp, "SAT"); break;
          case 6: strcpy(tmp, "SUN"); break;
          }
        }
      else if (*format == 'd' && *(format+1) == 'w') {
        format += 2;
        switch(DayOfWeek()) {
          case 0: strcpy(tmp, "Mo"); break;
          case 1: strcpy(tmp, "Tu"); break;
          case 2: strcpy(tmp, "We"); break;
          case 3: strcpy(tmp, "Th"); break;
          case 4: strcpy(tmp, "Fr"); break;
          case 5: strcpy(tmp, "Sa"); break;
          case 6: strcpy(tmp, "Su"); break;
          }
        }
      else if (*format == 'D' && *(format+1) == 'W') {
        format += 2;
        switch(DayOfWeek()) {
          case 0: strcpy(tmp, "MO"); break;
          case 1: strcpy(tmp, "TU"); break;
          case 2: strcpy(tmp, "WE"); break;
          case 3: strcpy(tmp, "TH"); break;
          case 4: strcpy(tmp, "FR"); break;
          case 5: strcpy(tmp, "SA"); break;
          case 6: strcpy(tmp, "SU"); break;
          }
        }
      else if (*format == 'M' && *(format+1) != 'M') {
        format += 1;
        sprintf(tmp,"%d",month);
        }
      else if (*format == 'M' && *(format+1) == 'M' && *(format+2) != 'M') {
        format += 2;
        sprintf(tmp,"%02d",month);
        }
      else if (*format == 'M' && *(format+1) == 'M' && *(format+2) == 'M' &&
               *(format+3) != 'M') {
        format += 3;
        switch (month) {
          case 1: strcpy(tmp, "JAN"); break;
          case 2: strcpy(tmp, "FEB"); break;
          case 3: strcpy(tmp, "MAR"); break;
          case 4: strcpy(tmp, "APR"); break;
          case 5: strcpy(tmp, "MAY"); break;
          case 6: strcpy(tmp, "JUN"); break;
          case 7: strcpy(tmp, "JUL"); break;
          case 8: strcpy(tmp, "AUG"); break;
          case 9: strcpy(tmp, "SEP"); break;
          case 10: strcpy(tmp, "OCT"); break;
          case 11: strcpy(tmp, "NOV"); break;
          case 12: strcpy(tmp, "DEC"); break;
          }
        }
      else if (*format == 'm' && *(format+1) == 'm' && *(format+2) == 'm' &&
               *(format+3) != 'm') {
        format += 3;
        switch (month) {
          case 1: strcpy(tmp, "Jan"); break;
          case 2: strcpy(tmp, "Feb"); break;
          case 3: strcpy(tmp, "Mar"); break;
          case 4: strcpy(tmp, "Apr"); break;
          case 5: strcpy(tmp, "May"); break;
          case 6: strcpy(tmp, "Jun"); break;
          case 7: strcpy(tmp, "Jul"); break;
          case 8: strcpy(tmp, "Aug"); break;
          case 9: strcpy(tmp, "Sep"); break;
          case 10: strcpy(tmp, "Oct"); break;
          case 11: strcpy(tmp, "Nov"); break;
          case 12: strcpy(tmp, "Ddec"); break;
          }
        }
      else if (*format == 'M' && *(format+1) == 'M' && *(format+2) == 'M' &&
               *(format+3) == 'M' && *(format+4) != 'M') {
        format += 4;
        switch (month) {
          case 1: strcpy(tmp, "JANUARY"); break;
          case 2: strcpy(tmp, "FEBRUARY"); break;
          case 3: strcpy(tmp, "MARCH"); break;
          case 4: strcpy(tmp, "APRIL"); break;
          case 5: strcpy(tmp, "MAY"); break;
          case 6: strcpy(tmp, "JUNE"); break;
          case 7: strcpy(tmp, "JULY"); break;
          case 8: strcpy(tmp, "AUGUST"); break;
          case 9: strcpy(tmp, "SEPTEMBER"); break;
          case 10: strcpy(tmp, "OCTOBER"); break;
          case 11: strcpy(tmp, "NOVEMBER"); break;
          case 12: strcpy(tmp, "DECEMBER"); break;
          }
        }
      else if (*format == 'm' && *(format+1) == 'm' && *(format+2) == 'm' &&
               *(format+3) == 'm' && *(format+4) != 'm') {
        format += 4;
        switch (month) {
          case 1: strcpy(tmp, "January"); break;
          case 2: strcpy(tmp, "February"); break;
          case 3: strcpy(tmp, "March"); break;
          case 4: strcpy(tmp, "April"); break;
          case 5: strcpy(tmp, "May"); break;
          case 6: strcpy(tmp, "June"); break;
          case 7: strcpy(tmp, "July"); break;
          case 8: strcpy(tmp, "August"); break;
          case 9: strcpy(tmp, "September"); break;
          case 10: strcpy(tmp, "October"); break;
          case 11: strcpy(tmp, "November"); break;
          case 12: strcpy(tmp, "December"); break;
          }
        }
      else if (*format == 'D' && *(format+1) != 'D') {
        format += 1;
        sprintf(tmp,"%d",day);
        }
      else if (*format == 'D' && *(format+1) == 'D' && *(format+2) != 'D') {
        format += 2;
        sprintf(tmp,"%02d",day);
        }
      else if (*format == 'Y' && *(format+1) == 'Y' && *(format+2) != 'Y') {
        format += 2;
        sprintf(tmp,"%02d",year % 100);
        }
      else if (*format == 'Y' && *(format+1) == 'Y' && *(format+2) == 'Y' &&
               *(format+3) == 'Y' && *(format+4) != 'Y') {
        format += 4;
        sprintf(tmp,"%04d",year);
        }
      else if (*format == 'h' && *(format+1) != 'h') {
        format += 1;
        sprintf(tmp,"%d",hour);
        }
      else if (*format == 'h' && *(format+1) == 'h' && *(format+2) != 'h') {
        format += 2;
        sprintf(tmp,"%02d",hour);
        }
      else if (*format == 'H' && *(format+1) != 'H') {
        format += 1;
        sprintf(tmp,"%d",hour % 12);
        }
      else if (*format == 'H' && *(format+1) == 'H' && *(format+2) != 'H') {
        format += 2;
        sprintf(tmp,"%02d",hour % 12);
        }
      else if (*format == 'm' && *(format+1) != 'm') {
        format += 1;
        sprintf(tmp,"%d",minute);
        }
      else if (*format == 'm' && *(format+1) == 'm' && *(format+2) != 'm') {
        format += 2;
        sprintf(tmp,"%02d",minute);
        }
      else if (*format == 's' && *(format+1) != 's') {
        format += 1;
        sprintf(tmp,"%d",second);
        }
      else if (*format == 's' && *(format+1) == 's' && *(format+2) != 's') {
        format += 2;
        sprintf(tmp,"%02d",second);
        }
      else if (*format == 'a' && *(format+1) == 'p') {
        format += 2;
        if (hour >= 12) strcpy(tmp, "pm");
          else strcpy(tmp,"am");
        }
      else if (*format == 'A' && *(format+1) == 'P') {
        format += 2;
        if (hour >= 12) strcpy(tmp, "PM");
          else strcpy(tmp,"AM");
        }
      else {
        tmp[0] = *format++;
        tmp[1] = 0;
        }
      for (i=0; i<strlen(tmp); i++) buffer[pos++] = tmp[i];
      }
    buffer[pos] = 0;
    return String(buffer);
    }

  String DateTime::ToString(String format) {
    return ToString(format.AsCharArray());
    }

  String DateTime::ToString(String *format) {
    return ToString(format->AsCharArray());
    }

  }

