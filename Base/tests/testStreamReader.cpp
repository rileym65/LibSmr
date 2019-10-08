#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestStreamReader() {
  StreamReader* obj;
  String        line;
  obj = new StreamReader("Makefile");
  printf("\nInfo: StreamReader tests\n");
  if (obj != NULL) Good("Was able to instantiate a StreamReader");
    else Bad("Could not instantiate a StreamReader");
  if (strcmp(obj->Type(), "StreamReader") == 0)
    Good("StreamReader reported correctly on Type");
    else Bad("StreamReader did not report correctly on Type");

  line = obj->ReadLine();
printf(">>%s<<\n",line.AsCharArray());
  if (line.CompareTo("PROJECT = test") == 0)
    Good("First line was read correctly");
  else
    Bad("First line was not read correctly");

  line = obj->ReadLine();
printf(">>%s<<\n",line.AsCharArray());
  if (line.CompareTo("CC = g++") == 0)
    Good("Second line was read correctly");
  else
    Bad("Second line was not read correctly");

  delete(obj);
  }
