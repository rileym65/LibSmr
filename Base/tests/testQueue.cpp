#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestQueue() {
  Queue<Byte>*   list;
  Byte           i;
  Byte           value;
  bool           flag;
  printf("\nInfo: Queue tests\n");
  list = new Queue<Byte>();
  if (list != NULL) Good("Was able to instantiate a Queue");
    else Bad("Could not instantiate an Queue");
  if (strcmp(list->Type(), "Queue") == 0)
    Good("Queue reported correctly on Type");
    else Bad("Queue did not report correctly on Type");
  if (list->Count() == 0)
    Good("New Queue has count of 0");
    else Bad("New Queue did not have a count of 0");
  if (list->Size() == 0)
    Good("New Queue without max size has size of 0");
    else Bad("New Queue without max size did not have a size of 0");
  if (!list->Overwrite())
    Good("New Queue has overwrite turned off");
    else Bad("New queue has overwrite turned on");

  if (list->Overwrite(true))
    Good("Return value from setting overwrite was correct");
    else Bad("Return value from setting overwrite was incorrect");
  if (list->Overwrite())
    Good("Was able to set overwrite to true");
    else Bad("Was not able to set overwrite to true");
  if (!list->Overwrite(false))
    Good("Return value from setting overwrite was correct");
    else Bad("Return value from setting overwrite was incorrect");
  if (!list->Overwrite())
    Good("Was able to set overwrite to false");
    else Bad("Was not able to set overwrite to false");

  delete(list);

  list = new Queue<Byte>(4);
  if (list != NULL) Good("Was able to instantiate a Queue with size");
    else Bad("Could not instantiate a Queue with size");
  if (list->Size() == 4)
    Good("New Queue with max size has correct size");
    else Bad("New Queue with max size did not have correct size");

  if (list->Add(101)) Good("Adding first item returned true");
    else Bad("Adding first item returned false");
  if (list->Count() == 1) Good("Count was 1 after adding 1 item");
    else Bad("Count was not 1 after adding 1 item");
  if (list->Add(102)) Good("Adding second item returned true");
    else Bad("Adding second item returned false");
  if (list->Count() == 2) Good("Count was 2 after adding 2 items");
    else Bad("Count was not 2 after adding 2 items");
  if (list->Add(103)) Good("Adding third item returned true");
    else Bad("Adding third item returned false");
  if (list->Count() == 3) Good("Count was 3 after adding 3 items");
    else Bad("Count was not 3 after adding 3 items");
  if (list->Add(104)) Good("Adding forth item returned true");
    else Bad("Adding forth item returned false");
  if (list->Count() == 4) Good("Count was 4 after adding 4 items");
    else Bad("Count was not 4 after adding 4 items");
  try {
    if (list->Add(105)) Bad("Adding fifth did not throw an exception on limited queue");
    }
  catch (...) {
    Good("Exception was thrown adding fifth item to limited queue");
    }
  if (list->Count() == 4) Good("Count was 4 after failing fifth item");
    else Bad("Count was not 4 after failing fifth item");

  value = list->Next();
  if (value == 101) Good("First retrieved value was correct");
    else Bad("First retrieved value was wrong");
  value = list->Next();
  if (value == 102) Good("Second retrieved value was correct");
    else Bad("Second retrieved value was wrong");
  value = list->Next();
  if (value == 103) Good("Third retrieved value was correct");
    else Bad("Third retrieved value was wrong");
  if (!list->AtEnd()) Good("Queue does not show at end while not empty");
    else Bad("Queue shows AtEnd while not empty");
  value = list->Next();
  if (value == 104) Good("Forth retrieved value was correct");
    else Bad("Forth retrieved value was wrong");
  if (list->AtEnd()) Good("Queue shows at end while empty");
    else Bad("Queue does not show AtEnd while empty");
  try {
    value = list->Next();
    Bad("Queue did not throw exception when Next called on empty queue");
    }
  catch (...) {
    Good("Queue threw exception when Next called on empty queue");
    }

  flag = true;
  list->Add(1);
  list->Add(2);
  for (i=3; i<=100; i++) {
    list->Add(i);
    value = list->Next();
    if (value != i-2) flag = false;
    }
  value = list->Next();
  if (value != 99) flag = false;
  value = list->Next();
  if (value != 100) flag = false;
  if (flag) Good("Queue passed 100 item rolling test");
    else Bad("Queue did not pass 100 item rolling test");
  if (list->Count() == 0) Good("Queue was empty at end of test");
    else Bad("Queue was not empty at end of test");

  delete(list);


  }
