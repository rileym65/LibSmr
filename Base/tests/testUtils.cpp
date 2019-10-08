#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestUtils() {
  Byte b1,b2;
  Int8 i81, i82;
  Int16 i161, i162;
  Int32 i321, i322;
  Int64 i641, i642;
  Single s1, s2;
  Single d1, d2;
  printf("\nInfo: Utils tests\n");

  b1 = 1;
  b2 = 2;
  Utils::Swap(b1,b2);
  if (b1 == 2 && b2 == 1) Good("Swap worked correctly for bytes");
    else Bad("Swap did not work correctly for bytes");

  i81 = 5;
  i82 = 7;
  Utils::Swap(i81,i82);
  if (i81 == 7 && i82 == 5) Good("Swap worked correctly for Int8s");
    else Bad("Swap did not work correctly for Int8s");

  i161 = 105;
  i162 = 117;
  Utils::Swap(i161,i162);
  if (i161 == 117 && i162 == 105) Good("Swap worked correctly for Int16s");
    else Bad("Swap did not work correctly for Int16s");

  i321 = 102105;
  i322 = 207117;
  Utils::Swap(i321,i322);
  if (i321 == 207117 && i322 == 102105) Good("Swap worked correctly for Int32s");
    else Bad("Swap did not work correctly for Int32s");

  i641 = 10234102105;
  i642 = 10234207117;
  Utils::Swap(i641,i642);
  if (i641 == 10234207117 && i642 == 10234102105) Good("Swap worked correctly for Int64s");
    else Bad("Swap did not work correctly for Int64s");

  s1 = 7.75;
  s2 = 2.125;
  Utils::Swap(s1,s2);
  if (s1 == 2.125 && s2 == 7.75) Good("Swap worked correctly for Singles");
    else Bad("Swap did not work correctly for Singles");

  d1 = 7.75;
  d2 = 2.125;
  Utils::Swap(d1,d2);
  if (d1 == 2.125 && d2 == 7.75) Good("Swap worked correctly for Doubles");
    else Bad("Swap did not work correctly for Doubles");
  }
