#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestArray() {
  Array<String*>  *array;
  Array<Byte>     *byteArray;
  String          *string1;
  String          *string2;
  printf("\nInfo: Array tests\n");
  array = new Array<String*>(5);
  if (array != NULL) Good("Was able to instantiate an Array");
    else Bad("Could not instantiate an Array");
  if (strcmp(array->Type(), "Array") == 0)
    Good("Array reported correctly on Type");
    else Bad("Array did not report correctly on Type");

  string1 = new String((char*)"String 1");
  string2 = new String((char*)"String 2");
//  array[1] = string1;
//  array[3] = string2;

  delete(string1);
  delete(string2);
  delete(array);
  }
