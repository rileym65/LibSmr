#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestStack() {
  Stack<int>*  stack;
  stack = new Stack<int>();
  printf("\nInfo: Stack tests\n");
  if (stack != NULL) Good("Was able to instantiate a Stack");
    else Bad("Could not instantiate an Stack");
  if (strcmp(stack->Type(), "Stack") == 0)
    Good("Stack reported correctly on Type");
    else Bad("Stack did not report correctly on Type");
  if (stack->Count() == 0)
    Good("Stack contained no items after creation");
    else Bad("Stack contained items after creation");

  stack->Push(101);
  if (stack->Count() == 1)
    Good("Stack count was 1 after pushing an item");
    else Bad("Stack count was not 1 after pushing an item");

  stack->Push(102);
  if (stack->Count() == 2)
    Good("Stack count was 2 after pushing second item");
    else Bad("Stack count was not 2 after pushing second item");

  if (stack->Pop() == 102)
    Good("Stack returned correct item after Pop");
    else Bad("Stack did not return correct item after Pop");
  if (stack->Count() == 1)
    Good("Stack count was 1 after Pop");
    else Bad("Stack count was not 1 after Pop");

  stack->Push(103);
  if (stack->Peek() == 103)
    Good("Peek correctly showed top of stack");
    else Bad("Peek did not show correct item");
  if (stack->Count() == 2)
    Good("Count was not affected by Peek");
    else Bad("Count was affected by Peek");

  if (stack->First() == 103)
    Good("First returned correct item");
    else Bad("First did not return correct item");
  if (stack->Count() == 2)
    Good("Count was not affected by First");
    else Bad("Count was affected by First");

  if (stack->Next() == 101)
    Good("Next returned correct item");
    else Bad("Next did not return correct item");
  if (stack->Count() == 2)
    Good("Count was not affected by Next");
    else Bad("Count was affected by Next");

  try {
    stack->Next();
    Bad("Calling Next after bottom of stack did not throw exception");
    }
  catch (InvalidOpException* e) {
    Good("Calling Next after bottom threw an InvalidOpException");
    }
  catch (...) {
    Bad("Calling Next after bottom threw the wrong exception");
    }

  stack->Pop();
  stack->Pop();

  try {
    stack->Pop();
    Bad("Calling Pop on empty stack did not throw exception");
    }
  catch (InvalidOpException* e) {
    Good("Calling Pop on empty stack threw an InvalidOpException");
    }
  catch (...) {
    Bad("Calling Pop on empty stack threw the wrong exception");
    }
      
  stack->Push(201);
  stack->Push(202);
  stack->Push(203);
  if (stack->Count() != 3)
    Bad("Pushing 3 items on empty stack did not produce a count of 3");
  stack->Clear();
  if (stack->Count() == 0)
    Good("Clear did clear the stack");
    else Bad("Clear did not clear the stack");

  stack->Push(301);
  stack->Dup();
  if (stack->Count() == 2)
    Good("Stack had count of 2 after dup on single item stack");
    else Bad("Stack did not have count of 2 after dup of single item stack");
  if (stack->Pop() == 301 && stack->Pop() == 301)
    Good("Correct value was duplicated");
    else Bad("Dup did not corectly duplicate the value");

  stack->Push(501);
  stack->Push(502);
  stack->Swap();
  if (stack->Pop() == 501)
    Good("Top of stack was correct after Swap");
    else Bad("Top of stack was not correct after swap");
  if (stack->Pop() == 502)
    Good("Second on stack was correct after Swap");
    else Bad("Second on stack was not correct after swap");

  stack->Clear();
  stack->Push(601);
  stack->Push(602);
  stack->Push(603);
  stack->Push(604);
  if (stack->At(1) == 603)
    Good("At returned the correct item");
    else Bad("At did not return the correct item");

  try {
    stack->At(-1);
    Bad("Calling At with negative index did not throw exception");
    }
  catch (InvalidOpException* e) {
    Good("Calling At with negative index threw an InvalidOpException");
    }
  catch (...) {
    Bad("Calling At with negative index threw the wrong exception");
    }

  try {
    stack->At(4);
    Bad("Calling At with too large index did not throw exception");
    }
  catch (InvalidOpException* e) {
    Good("Calling At with too large index threw an InvalidOpException");
    }
  catch (...) {
    Bad("Calling At with too large index threw the wrong exception");
    }

  if (stack->Contains(602))
    Good("Contains returned true for value on stack");
    else Bad("Contains returned false for value on stack");

  if (stack->Contains(702) == 0)
    Good("Contains returned false for value not on stack");
    else Bad("Contains returned true for value not on stack");


  delete(stack);
  }
