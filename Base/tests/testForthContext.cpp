#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

class myContext : public ForthContext {
  protected:
    UInt32 stack[100];
    UInt32 stackPointer;
    UInt32 memory[100];
  public:
    myContext() {
      int i;
      stackPointer = 0;
      for (i=0; i<100; i++) {
        stack[i] = 0;
        memory[i] = 0;
        }
      }
    Int32 At(UInt32 pos) {
      return stack[stackPointer-1-pos];
      }
    void Push(Int32 value) {
      stack[stackPointer++] = value;
      }
    Int32 Pop() {
      return stack[--stackPointer];
      }
    void Clear() {
      stackPointer = 0;
      }
    UInt32 Count() {
      return stackPointer;
      }
    void Output(const char* msg) {
      }
    void Write(UInt32 address,Int32 value) {
      memory[address] = value;
      }
    Int32 Read(UInt32 address) {
      return memory[address];
      }
    void Function(UInt32 number) {
      if (number == 1) Push(111);
      if (number == 2) Push(222);
      }
  };

void TestForthContext() {
  myContext* obj;
  obj = new myContext();
  printf("\nInfo: ForthContext tests\n");
  if (obj != NULL) Good("Was able to instantiate a ForthContext");
    else Bad("Could not instantiate a ForthContext");
  if (strcmp(obj->Type(), "ForthContext") == 0)
    Good("ForthContext reported correctly on Type");
    else Bad("ForthContext did not report correctly on Type");

  if (obj->Count() == 0) Good("Initial stack size was zero");
    else Bad("Initial stack size was not zero");

  obj->Push(123);
  if (obj->Count() == 1) Good("Stack size increased by one after a single push");
    else Bad("Stack size did not increase by one after a single push");

  obj->Push(456);
  if (obj->Pop() == 456) Good("Pop returned the correct value");
    else Bad("Pop did not return the correct value");
  if (obj->Count() == 1) Good("Stack size decreased by one after a single pop");
    else Bad("Stack size did not decrease by one after a single pop");

  if (obj->Pop() == 123) Good("Second Pop returned the correct value");
    else Bad("Second Pop did not return the correct value");
  if (obj->Count() == 0) Good("Stack size decreased by one after a single pop");
    else Bad("Stack size did not decrease by one after a single pop");

  obj->Write(12,890);
  if (obj->Read(12) == 890) Good("Write/Read worked correctly");
    else Bad("Write/Read did not work correctly");

  obj->Push(1);
  obj->Push(2);
  obj->Clear();
  if (obj->Count() == 0) Good("Clear cleared the stack");
    else Bad("Clear did not clear the stack");

  obj->Function(1);
  if (obj->Pop() == 111) Good("Function 1 worked correctly");
    else Bad("Function 1 did not work correctly");

  obj->Function(2);
  if (obj->Pop() == 222) Good("Function 2 worked correctly");
    else Bad("Function 2 did not work correctly");


  delete(obj);
  }
