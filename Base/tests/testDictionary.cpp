#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestDictionary() {
  Dictionary<int,int>*   dict;
  List<int>*             list;

  printf("\nInfo: Dictionary tests\n");
  dict = new Dictionary<int,int>();
  if (dict != NULL) Good("Was able to instantiate a Dictionary");
    else Bad("Could not instantiate an Dictionary");
  if (strcmp(dict->Type(), "Dictionary") == 0)
    Good("Dictionary reported correctly on Type");
    else Bad("Dictionary did not report correctly on Type");
  if (dict->Count() == 0) Good("New dictionary was empty");
    else Bad("New dictionary was not empty");

  dict->Add(1,2);
  if (dict->Count() == 1) Good("Count was 1 after adding 1 entry");
    else Bad("Count was not 1 after adding 1 entry");
  dict->Add(2,4);
  if (dict->Count() == 2) Good("Count was 2 after adding second entry");
    else Bad("Count was not 2 after adding second entry");

  try {
    if (dict->Lookup(1) == 2) Good("Looking up first value returned correct value");
      else Bad("Looking up first value returned wrong value");
    }
  catch (NotFoundException &err) {
    Bad("Not found exception thrown looking up first item");
    }

  try {
    if (dict->Lookup(2) == 4) Good("Looking up second value returned correct value");
      else Bad("Looking up second value returned wrong value");
    }
  catch (NotFoundException &err) {
    Bad("Not found exception thrown looking up second item");
    }

  try {
    if (dict->Lookup(3) == 0) Bad("Looking up missing entry did not throw exception");
      else Bad("Looking up missing entry did not throw exception");
    }
  catch (NotFoundException &err) {
    Good("NotFoundException exception thrown when looking up missing entry");
    }
  catch (...) {
    Bad("Exception other than NotFoundException was thrown looking up missing entry");
    }

  if (dict->ContainsKey(1)) Good("ContainsKey returned true for key in the dictionary");
    else Bad("ContainsKey returned false for key in the dictionary");
  if (dict->ContainsKey(2)) Good("ContainsKey returned true for second key in the dictionary");
    else Bad("ContainsKey returned false for second key in the dictionary");
  if (!dict->ContainsKey(3)) Good("ContainsKey returned false for key not in the dictionary");
    else Bad("ContainsKey returned true for key not in the dictionary");

  if (dict->ContainsValue(2)) Good("ContainsValue returned true for value in the dictionary");
    else Bad("ContainsValue returned false for value in the dictionary");
  if (dict->ContainsValue(4)) Good("ContainsValue returned true for second value in the dictionary");
    else Bad("ContainsValue returned false for second value in the dictionary");
  if (!dict->ContainsValue(3)) Good("ContainsValue returned false for key value in the dictionary");
    else Bad("ContainsValue returned true for value not in the dictionary");

  list = dict->Keys();
  if (list != NULL) Good("Keys did not return a NULL");
    else Bad("Keys returned NULL");
  if (list->Count() == 2) Good("Keys returned list of correct size");
    else Bad("Keys did not return the correct size of list");
  if (list->At(0) == 1) Good("First value in list had first key");
    else Bad("First value in list did not have correct key");
  if (list->At(1) == 2) Good("Second value in list had second key");
    else Bad("Second value in list did not have correct key");
  delete(list);

  dict->Add(2,8);
  if (dict->Count() == 2) Good("Dictionary size did not change reassigning a key");
    else Bad("Dictionary size changed reassigning a key");

  try {
    if (dict->Lookup(2) == 8) Good("Looking up reassigned key had correct value");
      else Bad("Looking up reassigned key did not have correct value");
    }
  catch (NotFoundException &err) {
    Bad("Not found exception thrown looking up reassigned key");
    }

  delete(dict);

  }
