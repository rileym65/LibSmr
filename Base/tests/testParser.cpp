#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestParser() {
  Parser*        obj;
  String        input;

  obj = new Parser();
  printf("\nInfo: Parser tests\n");
  if (obj != NULL) Good("Was able to instantiate a Parser");
    else Bad("Could not instantiate a Parser");
  if (strcmp(obj->Type(), "Parser") == 0)
    Good("Parser reported correctly on Type");
    else Bad("Parser did not report correctly on Type");

  try {
    obj->AddToken(String("SUB"), String("^-$"));
    obj->AddToken(String("ADD"), String("^\\+$"));
    obj->AddToken(String("MUL"), String("^\\*$"));
    obj->AddToken(String("DIV"), String("^/$"));
    obj->AddToken(String("NUM"), String("^[0-9]+$"));
    obj->AddToken(String("CHAR"), String("^.$"));
    Good("No exceptions thrown adding tokens");
    }
  catch (...) {
    Bad("An exception was thrown adding a token");
    printf("Info: Aborting further Parser tests\n");
    delete(obj);
    return;
    }

  delete(obj);
  }
