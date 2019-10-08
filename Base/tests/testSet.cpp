#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestSet() {
  Set<Object*>*   list;
  String* string1;
  String* string2;
  String* string3;
  String* string4;
  String* string5;
  printf("\nInfo: Set tests\n");
  list = new Set<Object*>();
  if (list != NULL) Good("Was able to instantiate a Set");
    else Bad("Could not instantiate an Set");
  if (strcmp(list->Type(), "Set") == 0)
    Good("Set reported correctly on Type");
    else Bad("Set did not report correctly on Type");
  if (list->Count() == 0)
    Good("New Set has count of 0");
    else Bad("New Set did not have a count of 0");

  string1 = new String((char*)"String 1");
  string2 = new String((char*)"String 2");
  string3 = new String((char*)"String 3");
  string4 = new String((char*)"String 4");

  list->Add(string1);
  if (list->Count() == 1)
    Good("Set returned count of 1 after adding 1 item");
    else Bad("Set did not return a count of 1 after adding 1 item");

  list->Add(string2);
  if (list->Count() == 2)
    Good("Set returned count of 2 after adding second item");
    else Bad("Set did not return a count of 2 after adding second item");

  list->Add(string3);
  list->Add(string4);
  if (list->Count() == 4)
    Good("Set returned count of 4 after adding four items");
    else Bad("Set did not return a count of 4 after adding four items");

  list->Add(string1);
  if (list->Count() == 4)
    Good("Adding a duplicate item was ignored");
    else Bad("Adding a duplicate item added the item to the set");
  
  if (list->Contains(string2))
    Good("Contains reported true when Set contained item");
    else Bad("Contains did not report true for an item in Set");

  string5 = new String((char*)"Another string");
  if (!list->Contains(string5))
    Good("Contains reported false for an item not in the list");
    else Bad("Contains reported true for an item not in the list");

  if (list->IndexOf(string2) == 1)
    Good("IndexOf reported correct position for an item in the list");
    else Bad("IndexOf did not report correct position for item in the list");

  if (list->IndexOf(string5) == -1)
    Good("IndexOf reported -1 for an item not in the list");
    else Bad("IndexOf did not report -1 for item not in the list");

  if (list->LastIndexOf(string1) == 0)
    Good("LastIndexOf reported correct position for an item in the list");
    else Bad("LastIndexOf did not report correct position for item in the list");

  if (list->LastIndexOf(string5) == -1)
    Good("LastIndexOf reported -1 for an item not in the list");
    else Bad("LastIndexOf did not report -1 for item not in the list");

  if (list->First() == string1)
    Good("First returned the first item in the list");
    else Bad("First did not return the first item in the list");

  if (list->Next() == string2)
    Good("Next returned the next item in the list");
    else Bad("Next did not return the next item in the list");

  if (list->AtEnd() == 0)
    Good("AtEnd did not report at end when not at end");
    else Bad("AtEnd reported at end when not at end");

  list->Next();
  list->Next();
  list->Next();
  if (list->Next() == NULL)
    Good("Next returned NULL when at end of list");
    else Bad("Next did not return NULL when at end of list");

  if (list->AtEnd())
    Good("AtEnd reported true when at end");
    else Bad("AtEnd did not report true when at end");

  if (list->At(1) == string2)
    Good("At returned the correct item");
    else Bad("At did not return the correct item");

  try {
    if (list->At(-1) == NULL)
      Bad("At did not throw exception on negative position");
      else Bad("At did not throw exception on negative position");
    }
  catch (RangeException &e) {
      Good("At threw exception on negative position");
    }

  try {
    if (list->At(10) == NULL)
      Bad("At did not throw exception when position past end");
      else Bad("At did not throw exception when position past end");
    }
  catch (RangeException &e) {
    Good("At threw exception when position past end");
    }

  list->AtPut(2, string5);
  if (list->Contains(string3) == 0)
    Good("Correct item was replaced after AtPut");
    else Bad("Correct item was not replaced after AtPut");

  if (list->Contains(string5))
    Good("Item was found in list after AtPut");
    else Bad("Item was not found in list after AtPut");

  list->InsertAt(2, string3);
  if (list->Contains(string3))
    Good("Inserted itam appears in list after InsertAt");
    else Bad("Inserted item does not appear in list after InsertAt");

  delete(list);
  }
