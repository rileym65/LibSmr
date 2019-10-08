#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestConvert() {
  Int64 i64;
  Int32 i32;
  Int16 i16;
  Int8  i8;
  UInt64 u64;
  UInt32 u32;
  UInt16 u16;
  UInt8  u8;
  Byte   b;
  String s1;
  Double d;
  printf("\nInfo: Convert tests\n");
  printf("Info: Start with checking string to int formats\n");
  s1 = String("12345");
  i64 = Convert::ToInt64(s1);
  if (i64 == 12345) Good("Convert correctly converted 12345 to Int64");
    else Bad("Convert did not correctly convert 12345 to Int64");

  s1 = String("$12345");
  i64 = Convert::ToInt64(s1);
  if (i64 == 74565) Good("Convert correctly converted $12345 to Int64");
    else Bad("Convert did not correctly convert $12345 to Int64");

  s1 = String("0x54321");
  i64 = Convert::ToInt64(s1);
  if (i64 == 344865) Good("Convert correctly converted 0x54321 to Int64");
    else Bad("Convert did not correctly convert 0x54321 to Int64");

  s1 = String("#555222");
  i64 = Convert::ToInt64(s1);
  if (i64 == 5591586) Good("Convert correctly converted #555222 to Int64");
    else Bad("Convert did not correctly convert #555222 to Int64");

  s1 = String("%1001_0110");
  i64 = Convert::ToInt64(s1);
  if (i64 == 0x96) Good("Convert correctly converted %1001_0110 to Int64");
    else Bad("Convert did not correctly convert %1001_0110 to Int64");

  s1 = String("0b1101_0111");
  i64 = Convert::ToInt64(s1);
  if (i64 == 0xd7) Good("Convert correctly converted 0b1101_0111 to Int64");
    else Bad("Convert did not correctly convert 0b1101_0111 to Int64");

  s1 = String("-12345");
  i64 = Convert::ToInt64(s1);
  if (i64 == -12345) Good("Convert correctly converted -12345 to Int64");
    else Bad("Convert did not correctly convert -12345 to Int64");

  s1 = String("-0x54321");
  i64 = Convert::ToInt64(s1);
  if (i64 == -344865) Good("Convert correctly converted -0x54321 to Int64");
    else Bad("Convert did not correctly convert -0x54321 to Int64");

  s1 = String("1,456,112");
  i32 = Convert::ToInt32(s1);
  if (i32 == 1456112) Good("Convert correctly converted '1,456,112' to Int32");
    else Bad("Convert did not correctly convert '1,456,112' to Int32");

  s1 = String("4587 1234");
  try {
    i64 = Convert::ToInt64(s1);
    if (i64 == 4587) Good("Convert string ToInt64 stopped at a space");
      else Bad("Convert stringg ToInt64 did not stop at a space");
    }
  catch (Exception &e) {
    Bad("Convert string ToInt64 threw an exception when space in string");
    }

  s1 = String("128912	1234");
  try {
    i64 = Convert::ToInt64(s1);
    if (i64 == 128912) Good("Convert string ToInt64 stopped at a tab");
      else Bad("Convert stringg ToInt64 did not stop at a tab");
    }
  catch (Exception &e) {
    Bad("Convert string ToInt64 threw an exception when space in tab");
    }

  s1 = String("45911c1234");
  try {
    i64 = Convert::ToInt64(s1);
    Bad("Convert string ToInt64 did not throw an exception with invalid character");
    }
  catch (Exception &e) {
    Good("Convert string ToInt64 threw an exception on invalid character");
    }

  printf("Info: Checking string to int ranges\n");
  s1 = String("0x8000000000000000");
  try {
    i64 = Convert::ToInt64(s1);
    Bad("Convert string to Int64 did not throw exception when out of range");
    }
  catch (Exception &e) {
    Good("Convert string to Int64 threw exception when out of range");
    }

  s1 = String("0x10000000");
  try {
    i32 = Convert::ToInt32(s1);
    if (i32 == 0x10000000) Good("Convert string to Int32 converted in range number");
      else Bad("Convert string to Int32 did not correctly convert in range number");
    }
  catch (Exception &e) {
    Bad("Convert string to Int32 threw exception when not out of range");
    }

  s1 = String("0x80000000");
  try {
    i32 = Convert::ToInt32(s1);
    Bad("Convert string to Int32 did not throw exception when out of range");
    }
  catch (Exception &e) {
    Good("Convert string to Int32 threw exception when out of range");
    }

  s1 = String("0x80000000");
  try {
    u32 = Convert::ToUInt32(s1);
    if (u32 == 0x80000000) Good("Convert string to UInt32 converted in range number");
      else Bad("Convert string to UInt32 did not correctly convert in range number");
    }
  catch (Exception &e) {
    Bad("Convert string to UInt32 threw exception when in range");
    }

  s1 = String("0x7000");
  try {
    i16 = Convert::ToInt16(s1);
    if (i16 == 0x7000) Good("Convert string to Int16 converted in range number");
      else Bad("Convert string to Int16 did not convert in range value correctly");
    }
  catch (Exception &e) {
    Bad("Convert string to Int16 threw exception on in range number");
    }

  s1 = String("0x8000");
  try {
    i16 = Convert::ToInt16(s1);
    Bad("Convert string to Int16 did not throw exception on out of range value");
    }
  catch (Exception &e) {
    Good("Convert string to Int16 threw exception on out of range value");
    }

  s1 = String("0x8000");
  try {
    u16 = Convert::ToUInt16(s1);
    if (u16 == 0x8000) Good("Convert string to UInt16 converted in range value");
      else Bad("Convert string to UInt16 did not properly convert in range value");
    }
  catch (Exception &e) {
    Bad("Convert string to UInt16 threw exception on in range value");
    }

  printf("Info: Checking numeric types to strings\n");

  u8 = 178;
  s1 = Convert::ToString(u8);
  if (s1 == "178") Good("Convert properly converted UInt8 '178' to a string");
    else Bad("Convert did not properly convert UInt8 '178' to a string");

  u16 = 47912;
  s1 = Convert::ToString(u16);
  if (s1 == "47912") Good("Convert properly converted UInt16 '47912' to a string");
    else Bad("Convert did not properly convert UInt16 '47912' to a string");

  u32 = 3576123873;
  s1 = Convert::ToString(u32);
  if (s1 == "3576123873") Good("Convert properly converted UInt32 '3576123873' to a string");
    else Bad("Convert did not properly convert UInt32 '3576123873' to a string");

  u64 = 1673576123873;
  s1 = Convert::ToString(u64);
  if (s1 == "1673576123873") Good("Convert properly converted UInt64 '1673576123873' to a string");
    else Bad("Convert did not properly convert UInt64 '1673576123873' to a string");

  i8 = 112;
  s1 = Convert::ToString(i8);
  if (s1 == "112") Good("Convert properly converted Int8 '112' to a string");
    else Bad("Convert did not properly convert Int8 '112' to a string");

  i8 = -105;
  s1 = Convert::ToString(i8);
  if (s1 == "-105") Good("Convert properly converted Int8 '-105' to a string");
    else Bad("Convert did not properly convert Int8 '-105' to a string");

  i16 = 30451;
  s1 = Convert::ToString(i16);
  if (s1 == "30451") Good("Convert properly converted Int16 '30451' to a string");
    else Bad("Convert did not properly convert Int16 '30451' to a string");

  i16 = -27100;
  s1 = Convert::ToString(i16);
  if (s1 == "-27100") Good("Convert properly converted Int16 '-27100' to a string");
    else Bad("Convert did not properly convert Int16 '-27100' to a string");

  i32 = 1450375100;
  s1 = Convert::ToString(i32);
  if (s1 == "1450375100") Good("Convert properly converted Int32 '1450375100' to a string");
    else Bad("Convert did not properly convert Int32 '1450375100' to a string");

  i32 = -456802;
  s1 = Convert::ToString(i32);
  if (s1 == "-456802") Good("Convert properly converted Int32 '-456802' to a string");
    else Bad("Convert did not properly convert Int32 '-456802' to a string");

  i64 = 10241450375100;
  s1 = Convert::ToString(i64);
  if (s1 == "10241450375100") Good("Convert properly converted Int64 '10241450375100' to a string");
    else Bad("Convert did not properly convert Int64 '10241450375100' to a string");

  i64 = -20484096456802;
  s1 = Convert::ToString(i64);
  if (s1 == "-20484096456802") Good("Convert properly converted Int64 '-20484096456802' to a string");
    else Bad("Convert did not properly convert Int64 '-20484096456802' to a string");

  d = 125.0;
  s1 = Convert::ToString(d);
  if (s1 == "125") Good("Convert properly converted Double '125.0' to a string");
    else Bad("Convert did not properly convert Double '125.0' to a string");

  d = 125.5;
  s1 = Convert::ToString(d);
  if (s1 == "125.5") Good("Convert properly converted Double '125.5' to a string");
    else Bad("Convert did not properly convert Double '125.5' to a string");

  d = 125.75;
  s1 = Convert::ToString(d);
  if (s1 == "125.75") Good("Convert properly converted Double '125.75' to a string");
    else Bad("Convert did not properly convert Double '125.75' to a string");

  d = 125.0125;
  s1 = Convert::ToString(d);
  if (s1 == "125.0125") Good("Convert properly converted Double '125.0125' to a string");
    else Bad("Convert did not properly convert Double '125.0125' to a string");

  d = -125.0078125;
  s1 = Convert::ToString(d);
  if (s1 == "-125.0078125") Good("Convert properly converted Double '-125.0078125' to a string");
    else Bad("Convert did not properly convert Double '-125.0078125' to a string");

  printf("Info: Checking formatted numeric types to strings\n");

  i32 = 32767;
  s1 = Convert::ToString(i32, "D,");
  if (s1 == "32,767") Good("Convert properly converted value using 'D,'");
    else Bad("Convert did not properly convert value using 'D,'");

  i32 = 500;
  s1 = Convert::ToString(i32, "D+");
  if (s1 == "+500") Good("Convert properly converted value using 'D+'");
    else Bad("Convert did not properly convert value using 'D+'");

  i32 = 256;
  s1 = Convert::ToString(i32, "D6");
  if (s1 == "   256") Good("Convert properly converted value using 'D6'");
    else Bad("Convert did not properly convert value using 'D6'");

  i32 = 256;
  s1 = Convert::ToString(i32, "D06");
  if (s1 == "000256") Good("Convert properly converted value using 'D06'");
    else Bad("Convert did not properly convert value using 'D06'");

  i32 = 1234;
  s1 = Convert::ToString(i32, "D,010");
  if (s1 == "000001,234") Good("Convert properly converted value using 'D,010'");
    else Bad("Convert did not properly convert value using 'D,010'");

  d = 1234.75;
  s1 = Convert::ToString(d, "F,");
  if (s1 == "1,234.75") Good("Convert properly converted value using 'F,'");
    else Bad("Convert did not properly convert value using 'F,'");

  d = 1234.25;
  s1 = Convert::ToString(d, "F4");
  if (s1 == "1234.2500") Good("Convert properly converted value using 'F4'");
    else Bad("Convert did not properly convert value using 'F4'");

  d = 1234.25;
  s1 = Convert::ToString(d, "D");
  if (s1 == "1234") Good("Convert properly converted value using 'D'");
    else Bad("Convert did not properly convert value using 'D'");

  d = 1234.125;
  s1 = Convert::ToString(d, "F+");
  if (s1 == "+1234.125") Good("Convert properly converted value using 'F+'");
    else Bad("Convert did not properly convert value using 'F+'");

  i32 = 0;
  s1 = Convert::ToString(i32);
  if (s1 == "0") Good("Convert properly converted value '0' to string");
    else Bad("Convert did not properly convert value '0' to string");

  b = 0;
  s1 = Convert::ToString(b);
  if (s1 == "0") Good("Convert properly converted Byte value '0' to string");
    else Bad("Convert did not properly convert Byte value '0' to string");

printf("%s\n",s1.AsCharArray());
  }
