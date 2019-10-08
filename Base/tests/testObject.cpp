#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestObject() {
  Object* obj;
  obj = new Object();
  printf("\nInfo: Object tests\n");
  if (obj != NULL) Good("Was able to instantiate an Object");
    else Bad("Could not instantiate an Object");
  if (strcmp(obj->Type(), "Object") == 0)
    Good("Object reported correctly on Type");
    else Bad("Object did not report correctly on Type");
  delete(obj);
  }
