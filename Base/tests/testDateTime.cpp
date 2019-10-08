#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

int checkDate(DateTime dt,int mon,int day,int yr,int hr, int min, int sec) {
  if (dt.Month() != mon) return 0;
  if (dt.Day() != day) return 0;
  if (dt.Year() != yr) return 0;
  if (dt.Hour() != hr) return 0;
  if (dt.Minute() != min) return 0;
  if (dt.Second() != sec) return 0;
  return -1;
  }

int checkTimeSpan(TimeSpan ts,int day,int hr, int min, int sec) {
  if (ts.Days() != day) return 0;
  if (ts.Hours() != hr) return 0;
  if (ts.Minutes() != min) return 0;
  if (ts.Seconds() != sec) return 0;
  return -1;
  }

void TestDateTime() {
  DateTime   *pdt;
  DateTime   dt;
  DateTime   dt1;
  TimeSpan*   ts;
  TimeSpan    ts1;
  String*     st;
  char        buffer[256];
  printf("\nInfo: DateTime tests\n");
  pdt = new DateTime();
  if (pdt != NULL) Good("Was able to instantiate a DateTime");
    else Bad("Could not instantiate an DateTime");
  if (strcmp(pdt->Type(), "DateTime") == 0)
    Good("DateTime reported correctly on Type");
    else Bad("DateTime did not report correctly on Type");

  if (DateTime::DaysInYear(2015) == 365)
    Good("DaysInYear reported 365 for non leap year");
    else Bad("DaysInYear did not report 365 for non leap year");

  if (DateTime::DaysInYear(2016) == 366)
    Good("DaysInYear reported 366 for leap year");
    else Bad("DaysInYear did not report 366 for leap year");

  sprintf(buffer,"%02d/%02d/%04d %02d:%02d:%02d\n",
    pdt->Month(), pdt->Day(), pdt->Year(), pdt->Hour(), pdt->Minute(), pdt->Second());
  printf("Info: Localtime on creation: %s",buffer);

  delete(pdt);
  
  pdt = new DateTime(6,8,1965,6,47,00);
  if (pdt != NULL) Good("Was able to instantiate a DateTime with given date");
    else Bad("Could not instantiate an DateTime with given date");

  sprintf(buffer,"%02d/%02d/%04d %02d:%02d:%02d\n",
    pdt->Month(), pdt->Day(), pdt->Year(), pdt->Hour(), pdt->Minute(), pdt->Second());
  printf("Info: Localtime on creation: %s",buffer);

  if (checkDate(*pdt,6,8,1965,6,47,0))
    Good("Specified date was correct on instantiation");
    else Bad("Specified date was not correct on instantiation");

  dt = DateTime(6, 8, 1965, 6, 47, 00);
  dt1 = dt.AddDays(4);
  if (checkDate(dt1,6,12,1965,6,47,0))
    Good("AddDays correctly added time");
    else Bad("AddDays did not correctly add time");

  dt1 = dt.AddDays(30);
  if (checkDate(dt1,7,8,1965,6,47,0))
    Good("AddDays correctly added time across a month");
    else Bad("AddDays did not correctly add time month");

  dt1 = dt.AddDays(60);
  if (checkDate(dt1,8,7,1965,6,47,0))
    Good("AddDays correctly added time across two months");
    else Bad("AddDays did not correctly add time across two months");

  dt1 = dt.AddDays(90);
  if (checkDate(dt1,9,6,1965,6,47,0))
    Good("AddDays correctly added time across three months");
    else Bad("AddDays did not correctly add time across three months");

  dt1 = dt.AddMonths(1);
  if (checkDate(dt1,7,8,1965,6,47,0))
    Good("AddMonths correctly added time across one month");
    else Bad("AddMonths did not correctly add time across one month");

  dt1 = dt.AddMonths(24);
  if (checkDate(dt1,6,8,1967,6,47,0))
    Good("AddMonths correctly added time across 24 months");
    else Bad("AddMonths did not correctly add time across 24 months");

  dt1 = dt.AddYears(3);
  if (checkDate(dt1,6,8,1968,6,47,0))
    Good("AddYears correctly added time across 3 years");
    else Bad("AddYears did not correctly add time across 3 years");

  dt1 = dt.AddSeconds(65);
  if (checkDate(dt1,6,8,1965,6,48,5))
    Good("AddSeconds correctly added time");
    else Bad("AddSeconds did not correctly add time");

  dt1 = dt.AddMinutes(20);
  if (checkDate(dt1,6,8,1965,7,07,0))
    Good("AddMinutes correctly added time");
    else Bad("AddMinutes did not correctly add time");

  dt1 = dt.AddHours(49);
  if (checkDate(dt1,6,10,1965,7,47,0))
    Good("AddHours correctly added time");
    else Bad("AddHours did not correctly add time");


  dt = DateTime(12,31,2016,23,59,59);
  dt1 = dt.AddSeconds(1);
  if (checkDate(dt1,1,1,2017,0,0,0))
    Good("Adding 1 second to last second of year worked correctly");
    else Bad("Adding 1 second to last second of year did not work correctly");

  dt = DateTime(6,8,1965,0,0,0);
  if (dt.ToJulianDay() == 2438919.5)
    Good("ToJulianDay worked correctly");
    else Bad("ToJulianDay did not work correctly");

  dt1 = DateTime::FromJulianDay(2438919.5);
  if (checkDate(dt1,6,8,1965,0,0,0))
    Good("Correctly make a DateTime from a julian day");
    else Bad("Making a DateTime from a julian day failed");

  ts = new TimeSpan(30, 4, 10, 30);
  dt1 = dt.Add(ts);
  if (checkDate(dt1,7,8,1965,4,10,30))
    Good("Correctly added a TimeSpan to a DateTime");
    else Bad("Adding a TimeSpan to a DateTime produced wrong result");
  delete(ts);
  
  dt1 = DateTime(6,8,1965,10,0,0);
  if (dt.CompareTo(&dt1) < 0)
    Good("CompareTo was correct when date was less");
    else Bad("CompareTo was not correct when date was less");
  
  dt1 = DateTime(6,7,1965,23,59,59);
  if (dt.CompareTo(&dt1) > 0)
    Good("CompareTo was correct when date was greater");
    else Bad("CompareTo was not correct when date was greater");
  
  dt1 = DateTime(6,8,1965,0,0,0);
  if (dt.CompareTo(&dt1) == 0)
    Good("CompareTo was correct when date was equal");
    else Bad("CompareTo was not correct when date was equal");

  if (DateTime::IsLeapYear(2000) != 0)
    Good("IsLeapYear returns true for year 2000");
    else Bad("IsLeapYear returns false for year 2000");

  if (DateTime::IsLeapYear(2003) == 0)
    Good("IsLeapYear returns false for year 2003");
    else Bad("IsLeapYear returns true for year 2003");

  if (DateTime::IsLeapYear(2100) == 0)
    Good("IsLeapYear returns false for year 2100");
    else Bad("IsLeapYear returns true for year 2100");

  if (DateTime::IsLeapYear(2004) != 0)
    Good("IsLeapYear returns true for year 2004");
    else Bad("IsLeapYear returns false for year 2004");

  ts = new TimeSpan(2, 3, 0, 0);
  dt1 = dt.Subtract(ts);
  if (checkDate(dt1,6,5,1965,21,0,0))
    Good("Correctly subtracted a TimeSpan from a DateTime");
    else Bad("Subtracted a TimeSpan from a DateTime produced wrong result");
  delete(ts);

  dt = DateTime(6,8,1965,9,30,0);
  dt1= DateTime(6,7,1965,7,25,0);
  ts1 = dt.Subtract(&dt1);
  if (checkTimeSpan(ts1,1,2,5,0) == 0 ||
      checkTimeSpan(ts1,1,2,4,59) == 0 ||
      checkTimeSpan(ts1,1,2,5,1) == 0)
    Good("Correctly subtracted one date from another");
    else Bad("Could not properly subtract one date from another");

  dt = DateTime("5/23/2019 12:45:15");
  if (dt.Month() == 5 &&
      dt.Day() == 23 &&
      dt.Year() == 2019 &&
      dt.Hour() == 12 &&
      dt.Minute() == 45 &&
      dt.Second() == 15)
    Good("DateTime correctly created from character string");
    else Bad("DateTime was not correctly created from character string");

  dt = DateTime(String("6/24/2020 1:30"));
  if (dt.Month() == 6 &&
      dt.Day() == 24 &&
      dt.Year() == 2020 &&
      dt.Hour() == 1 &&
      dt.Minute() == 30 &&
      dt.Second() == 0)
    Good("DateTime correctly created from String object");
    else Bad("DateTime was not correctly created from String object");

  st = new String("2/21/2018 13:20:15");
  dt = DateTime(st);
  if (dt.Month() == 2 &&
      dt.Day() == 21 &&
      dt.Year() == 2018 &&
      dt.Hour() == 13 &&
      dt.Minute() == 20 &&
      dt.Second() == 15)
    Good("DateTime correctly created from String pointer");
    else Bad("DateTime was not correctly created from String pointer");
  delete(st);

  try {
    dt = DateTime("2/21/2018 1:60:00");
    Bad("DateTime allowed bad minute value");
    }
  catch (RangeException &e) {
    Good("Bad minute threw a range exception");
    }
  }
