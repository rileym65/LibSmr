#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestBase64() {
  Base64* obj;
  String str1;
  obj = new Base64();
  printf("\nInfo: Base64 tests\n");
  if (obj != NULL) Good("Was able to instantiate a Base64");
    else Bad("Could not instantiate a Base64");
  if (strcmp(obj->Type(), "Base64") == 0)
    Good("Base64 reported correctly on Type");
    else Bad("Base64 did not report correctly on Type");

  str1 = obj->Encode((const Byte*)"pleasure.",9);
  if (str1.Equals("cGxlYXN1cmUu"))
    Good("Base64 correctly encoded 'pleasure.'");
  else
    Bad("Base64 did not correctly encode 'pleasure.'");

  str1 = obj->Encode((const Byte*)"leasure.",8);
  if (str1.Equals("bGVhc3VyZS4="))
    Good("Base64 correctly encoded 'leasure.'");
  else
    Bad("Base64 did not correctly encode 'leasure.'");

  str1 = obj->Encode((const Byte*)"easure.",7);
  if (str1.Equals("ZWFzdXJlLg=="))
    Good("Base64 correctly encoded 'easure.'");
  else
    Bad("Base64 did not correctly encode 'easure.'");

  str1 = obj->Decode("cGxlYXN1cmUu");
  if (str1.Equals("pleasure."))
    Good("Base64 correctly decoded 'cGxlYXN1cmUu'");
  else
    Bad("Base64 did not correctly decode 'cGxlYXN1cmUu'");

  str1 = obj->Decode("bGVhc3VyZS4=");
  if (str1.Equals("leasure."))
    Good("Base64 correctly decoded bGVhc3VyZS4=''");
  else
    Bad("Base64 did not correctly decode 'bGVhc3VyZS4='");

  str1 = obj->Decode("ZWFzdXJlLg==");
  if (str1.Equals("easure."))
    Good("Base64 correctly decoded 'ZWFzdXJlLg=='");
  else
    Bad("Base64 did not correctly decode 'ZWFzdXJlLg=='");

  delete(obj);
  }
