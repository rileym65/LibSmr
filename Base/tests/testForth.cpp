#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

char forthOutput[1024];

class myContext2 : public ForthContext {
  protected:
    Int32 stack[100];
    UInt32 stackPointer;
    Int32 memory[100];
  public:
    myContext2() {
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
      strcpy(forthOutput, msg);
      }
    void Write(UInt32 address,Int32 value) {
      memory[address] = value;
      }
    Int32 Read(UInt32 address) {
      return memory[address];
      }
    void Function(UInt32 number) {
      Int32 a;
      if (number == 1) Push(111);
      if (number == 2) {
        a = Pop();
        Push(a * a * a);
        }
      }
  };

void TestForth() {
  Forth*        forth;
  myContext2*   context;
  ForthProgram* program;

  context = new myContext2();
  forth = new Forth(context);
  printf("\nInfo: Forth tests\n");
  if (forth != NULL) Good("Was able to instantiate a Forth");
    else Bad("Could not instantiate a Forth");
  if (strcmp(forth->Type(), "Forth") == 0)
    Good("Forth reported correctly on Type");
    else Bad("Forth did not report correctly on Type");

  try {
    program = forth->Parse("dup dup");
    Good("Parse dup dup did not throw an exception");
    }
  catch (InvalidOpException &err) {
    Bad("Parse dup dup threw an exception");
    printf("      %s\n",err.Message());
    printf("      Aborting further Forth tests\n");
    delete(forth);
    delete(context);
    return;
    }

  if (program->Count() == 2) Good("Parse of dup dup produced correct count");
    else Bad("Parse of dup dup did not produce correct count");

  context->Push(5);
  try {
    program->Exec();
    Good("Exec did not throw an exception");
    }
  catch (InvalidOpException &err) {
    Bad("Exec threw an exception");
    printf("      %s\n",err.Message());
    printf("      Aborting further Forth tests\n");
    delete(forth);
    delete(context);
    return;
    }

  if (context->Count() == 3) Good("Stack had 3 items after Exec of dup dup");
    else Bad("Stack did not have 3 items after Exec of dup dup");

  if (context->At(0) == 5 && context->At(1) == 5 && context->At(2) ==5)
    Good("dup dup was correctly executed");
  else Bad("dup dup was not correctly executed");
  delete(program);

  program = forth->Parse("clear");
  program->Exec();
  if (context->Count() == 0) Good("Clear cleared the stack");
    else Bad("Clear did not clear the stack");
  delete(program);
 
  program = forth->Parse("drop");
  context->Push(4);
  context->Push(6);
  program->Exec();
  if (context->Pop() == 4) Good("Drop did drop an item from the stack");
    else Bad("Drop did not drop 1 item from the stack");
  delete(program);

  program = forth->Parse("swap");
  context->Push(10);
  context->Push(20);
  program->Exec();
  if (context->Pop() == 10) Good("TOS was correct after swap");
    else Bad("TOS was not correct after swap");
  if (context->Pop() == 20) Good("SOS was correct after swap");
    else Bad("SOS was not correct after swap");
  delete(program);

  program = forth->Parse("depth");
  context->Clear();
  context->Push(5);
  context->Push(7);
  program->Exec();
  if (context->Count() == 3) Good("Stack size was correct after depth");
    else Bad("Stack size was not correct after depth");
  if (context->Pop() == 2) Good("depth pushed correct value to stack");
    else Bad("depth did not push correct value to stack");

  program = forth->Parse("true false");
  context->Clear();
  program->Exec();
  if (context->Pop() == 0) Good("False pushed correct value");
    else Bad("False did not push correct value");
  if (context->Pop() == -1) Good("True pushed correct value");
    else Bad("True did not push correct value");
  delete(program);

try {
  program = forth->Parse("1 2");
  context->Clear();
  program->Exec();
  if (context->Pop() == 2) Good("TOS was correct after 1 2");
    else Bad("TOS was not correct after 1 2");
  if (context->Pop() == 1) Good("SOS was correct after 1 2");
    else Bad("SOS was not correct after 1 2");
  delete(program);
  }
catch (RangeException &e) {
  printf("range exception: %s\n",e.Message());
  }

  program = forth->Parse("1 -2");
  context->Clear();
  program->Exec();
  if (context->Pop() == -2) Good("Was able to push a negative number");
    else Bad("Was not able to push a negative number");
  delete(program);

  program = forth->Parse("3 7 +");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after 3 7 +");
    else Bad("Stack size was not correct after 3 7 +");
  if (context->Pop() == 10) Good("+ worked correct");
    else Bad("+ did not work correctly");
  delete(program);

  program = forth->Parse("3 -7 +");
  context->Clear();
  program->Exec();
  if (context->Pop() == -4) Good("+ worked correctly with a negative number");
    else Bad("+ did not work correctly with a negative number");
  delete(program);

  program = forth->Parse("5 8 -");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after 5 8 -");
    else Bad("Stack size was not correct after 5 8 -");
  if (context->Pop() == -3) Good("- worked correctly");
    else Bad("- did not work correctly");
  delete(program);

  program = forth->Parse("-5 3 *");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after -5 3 *");
    else Bad("Stack size was not correct after -5 3 *");
  if (context->Pop() == -15) Good("* worked correctly");
    else Bad("* did not work correctly");
  delete(program);

  program = forth->Parse("20 -5 /");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after 20 -5 /");
    else Bad("Stack size was not correct after 20 -5 /");
  if (context->Pop() == -4) Good("/ worked correctly");
    else Bad("/ did not work correctly");
  delete(program);

  program = forth->Parse("4 4 =");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after 4 4 =");
    else Bad("Stack size was not correct after 4 4 =");
  if (context->Pop() != 0) Good("= worked correctly on matching numbers");
    else Bad("/ did not work correctly on matching numbers");
  delete(program);

  program = forth->Parse("4 -4 =");
  context->Clear();
  program->Exec();
  if (context->Pop() == 0) Good("= worked correctly on non-matching numbers");
    else Bad("= did not work correctly on non-matching numbers");
  delete(program);

  program = forth->Parse("4 -4 <>");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after 4 -4 <>");
    else Bad("Stack size was not correct after 4 -4 <>");
  if (context->Pop() != 0) Good("<> worked correctly on non-matching numbers");
    else Bad("<> did not work correctly on non-matching numbers");
  delete(program);

  program = forth->Parse("4 4 <>");
  context->Clear();
  program->Exec();
  if (context->Pop() == 0) Good("<> worked correctly on matching numbers");
    else Bad("<> did not work correctly on matching numbers");
  delete(program);

  program = forth->Parse("4 -4 >");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after 4 -4 >");
    else Bad("Stack size was not correct after 4 -4 >");
  if (context->Pop() != 0) Good("> worked correctly on first number greater");
    else Bad("> did not work correctly on first number greater");
  delete(program);

  program = forth->Parse("4 5 >");
  context->Clear();
  program->Exec();
  if (context->Pop() == 0) Good("> worked correctly on first number smaller");
    else Bad("> did not work correctly on first number smaller");
  delete(program);

  program = forth->Parse("5 5 >");
  context->Clear();
  program->Exec();
  if (context->Pop() == 0) Good("> worked correctly on numbers equal");
    else Bad("> did not work correctly on numbers equal");
  delete(program);

  program = forth->Parse("3 4 <");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after 3 4 <");
    else Bad("Stack size was not correct after 3 4 <");
  if (context->Pop() != 0) Good("< worked correctly on first number less");
    else Bad("< did not work correctly on first number less");
  delete(program);

  program = forth->Parse("5 4 <");
  context->Clear();
  program->Exec();
  if (context->Pop() == 0) Good("< worked correctly on first number greater");
    else Bad("< did not work correctly on first number greater");
  delete(program);

  program = forth->Parse("5 5 <");
  context->Clear();
  program->Exec();
  if (context->Pop() == 0) Good("< worked correctly on numbers equal");
    else Bad("< did not work correctly on numbers equal");
  delete(program);

  program = forth->Parse("4 -4 >=");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after 4 -4 >=");
    else Bad("Stack size was not correct after 4 -4 >=");
  if (context->Pop() != 0) Good(">= worked correctly on first number greater");
    else Bad(">= did not work correctly on first number greater");
  delete(program);

  program = forth->Parse("4 5 >=");
  context->Clear();
  program->Exec();
  if (context->Pop() == 0) Good(">= worked correctly on first number smaller");
    else Bad(">= did not work correctly on first number smaller");
  delete(program);

  program = forth->Parse("5 5 >=");
  context->Clear();
  program->Exec();
  if (context->Pop() != 0) Good(">= worked correctly on numbers equal");
    else Bad(">= did not work correctly on numbers equal");
  delete(program);

  program = forth->Parse("3 4 <=");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after 3 4 <=");
    else Bad("Stack size was not correct after 3 4 <=");
  if (context->Pop() != 0) Good("<= worked correctly on first number less");
    else Bad("<= did not work correctly on first number less");
  delete(program);

  program = forth->Parse("5 4 <=");
  context->Clear();
  program->Exec();
  if (context->Pop() == 0) Good("<= worked correctly on first number greater");
    else Bad("<= did not work correctly on first number greater");
  delete(program);

  program = forth->Parse("5 5 <=");
  context->Clear();
  program->Exec();
  if (context->Pop() != 0) Good("<= worked correctly on numbers equal");
    else Bad("<= did not work correctly on numbers equal");
  delete(program);

  program = forth->Parse("1 -1 if 2 then");
  context->Clear();
  program->Exec();
  if (context->Count() == 2) Good("Stack size was correct after successful if");
    else Bad("Stack size was not correct after successful if");
  if (context->Pop() == 2) Good("Code between if and then appears to be correct");
    else Bad("Code between if and then appears not to have been executed");
  delete(program);

  program = forth->Parse("1 0 if 2 then");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after failed if");
    else Bad("Stack size was not correct after failed if");
  if (context->Pop() == 1) Good("Code between if and then appears to be skipped");
    else Bad("Code between if and then appears not to have been skipped");
  delete(program);

  program = forth->Parse("1 -1 if 2 else 3 then");
  context->Clear();
  program->Exec();
  if (context->Count() == 2) Good("Stack size was correct after successful if/else/then");
    else Bad("Stack size was not correct after successful if/else/then");
  if (context->Pop() == 2) Good("If/else/then worked on successful if");
    else Bad("If/else/then did not work for successful if");
  delete(program);

  program = forth->Parse("1 0 if 2 else 3 then");
  context->Clear();
  program->Exec();
  if (context->Count() == 2) Good("Stack size was correct after failed if/else/then");
    else Bad("Stack size was not correct after failed if/else/then");
  if (context->Pop() == 3) Good("If/else/then worked on failed if");
    else Bad("If/else/then did not work for failed if");
  delete(program);

  program = forth->Parse("0 5 1 do i + loop");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after do/loop");
    else Bad("Stack size was not correct after do/loop");
  if (context->Pop() == 10) Good("do/loop worked correctly in positive direction");
    else Bad("do/loop did not work correctly in positive direction");
  delete(program);

  program = forth->Parse("0 0 5 do i + loop");
  context->Clear();
  program->Exec();
  if (context->Pop() == 15) Good("do/loop worked correctly in negative direction");
    else Bad("do/loop did not work correctly in negative direction");
  delete(program);

  program = forth->Parse("0 10 0 do i + 2 +loop");
  context->Clear();
  program->Exec();
  if (context->Pop() == 20) Good("do/+loop worked correctly");
    else Bad("do/+loop did not work correctly");
  delete(program);

  program = forth->Parse("0 begin 2 + dup 10 > until");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after begin/until");
    else Bad("Stack size was not correct after begin/until");
  if (context->Pop() == 12) Good("begin/until worked correctly");
    else Bad("begin/until did not work correctly");
  delete(program);

  program = forth->Parse("1 true while 2 + dup 10 < repeat");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after while/repeat");
    else Bad("Stack size was not correct after while/repeat");
  if (context->Pop() == 11) Good("while/repeat worked correctly");
    else Bad("while/repeat did not work correctly");
  delete(program);

  program = forth->Parse("14 17 !");
  context->Clear();
  program->Exec();
  if (context->Count() == 0) Good("Stack size was correct after !");
    else Bad("Stack size was not correct after !");
  if (context->Read(17) == 14) Good("Memory was correct after !");
    else Bad("Memory was not correct after !");
  delete(program);

  program = forth->Parse("25 @");
  context->Clear();
  context->Write(25,123);
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after @");
    else Bad("Stack size was not correct after !");
  if (context->Pop() == 123) Good("Memory was correctly read by @");
    else Bad("Memory was not correctly read by @");
  delete(program);

  program = forth->Parse("65 emit");
  context->Clear();
  program->Exec();
  if (context->Count() == 0) Good("Stack size was correct after emit");
    else Bad("Stack size was not correct after emit");
  if (strcmp(forthOutput,"A") == 0) Good("Correct value was output by emit");
    else Bad("Incorrect value was output by emit");
  delete(program);

  program = forth->Parse("2 4 6 rot");
  context->Clear();
  program->Exec();
  if (context->Count() == 3) Good("Stack size was correct after rot");
    else Bad("Stack size was not correct after rot");
  if (context->At(2) == 4 && context->At(1) == 6 && context->At(0) == 2)
    Good("Rot worked correctly");
  else
    Bad("Rot did not work correctly");
  delete(program);

  program = forth->Parse("2 4 6 -rot");
  context->Clear();
  program->Exec();
  if (context->Count() == 3) Good("Stack size was correct after -rot");
    else Bad("Stack size was not correct after -rot");
  if (context->At(2) == 6 && context->At(1) == 2 && context->At(0) == 4)
    Good("-Rot worked correctly");
  else
    Bad("-Rot did not work correctly");
  delete(program);

  program = forth->Parse("2 4 over");
  context->Clear();
  program->Exec();
  if (context->Count() == 3) Good("Stack size was correct after over");
    else Bad("Stack size was not correct after over");
  if (context->At(0) == 2 && context->At(1) == 4 && context->At(2) == 2)
    Good("over worked correctly");
  else
    Bad("over did not work correctly");
  delete(program);

  program = forth->Parse("2 aug");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after aug");
    else Bad("Stack size was not correct after aug");
  if (context->Pop() == 3) Good("Aug worked correctly on positive number");
    else Bad("Aug did not work correctly on positive number");
  delete(program);

  program = forth->Parse("-3 aug");
  context->Clear();
  program->Exec();
  if (context->Pop() == -4) Good("Aug worked correctly on negative number");
    else Bad("Aug did not work correctly on negative number");
  delete(program);

  program = forth->Parse("0 aug");
  context->Clear();
  program->Exec();
  if (context->Pop() == 0) Good("Aug worked correctly on zero");
    else Bad("Aug did not work correctly on zero");
  delete(program);

  program = forth->Parse("2 dim");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after dim");
    else Bad("Stack size was not correct after dim");
  if (context->Pop() == 1) Good("Dim worked correctly on positive number");
    else Bad("Dim did not work correctly on positive number");
  delete(program);

  program = forth->Parse("-3 dim");
  context->Clear();
  program->Exec();
  if (context->Pop() == -2) Good("Dim worked correctly on negative number");
    else Bad("Dim did not work correctly on negative number");
  delete(program);

  program = forth->Parse("0 dim");
  context->Clear();
  program->Exec();
  if (context->Pop() == 0) Good("Dim worked correctly on zero");
    else Bad("Dim did not work correctly on zero");
  delete(program);

  program = forth->Parse("65 .");
  context->Clear();
  program->Exec();
  if (context->Count() == 0) Good("Stack size was correct after .");
    else Bad("Stack size was not correct after .");
  if (strcmp(forthOutput,"65") == 0) Good("Correct value was output by .");
    else Bad("Incorrect value was output by .");
  delete(program);

  program = forth->Parse("5 abs");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after abs");
    else Bad("Stack size was not correct after abs");
  if (context->Pop() == 5) Good("Abs worked correctly on positive number");
    else Bad("Abs did not work correctly on positive number");
  delete(program);

  program = forth->Parse("-5 abs");
  context->Clear();
  program->Exec();
  if (context->Pop() == 5) Good("Abs worked correctly on negative number");
    else Bad("Abs did not work correctly on negative number");
  delete(program);

  program = forth->Parse("0 abs");
  context->Clear();
  program->Exec();
  if (context->Pop() == 0) Good("Abs worked correctly on zero");
    else Bad("Abs did not work correctly on zero");
  delete(program);

  program = forth->Parse("6 { 7 8 }");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after { } sequence");
    else Bad("Stack size was not correct after { } sequence");
  if (context->Pop() == 6) Good("Stack was correct after { } sequence");
    else Bad("Stack was not correct after { } sequence");
  delete(program);

  program = forth->Parse("5 6 12 +max");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after +max");
    else Bad("Stack size was not correct after +max");
  if (context->Pop() == 11) Good("Stack was correct after +max not exceeding max");
    else Bad("Stack was not correct after +max not exceeding max");
  delete(program);

  program = forth->Parse("5 6 7 +max");
  context->Clear();
  program->Exec();
  if (context->Pop() == 7) Good("Stack was correct after +max exceeding max");
    else Bad("Stack was not correct after +max exceeding max");
  delete(program);

  program = forth->Parse("10 6 2 -min");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after -min");
    else Bad("Stack size was not correct after -min");
  if (context->Pop() == 4) Good("Stack was correct after -min not exceeding min");
    else Bad("Stack was not correct after -min not exceeding min");
  delete(program);

  program = forth->Parse("10 9 2 -min");
  context->Clear();
  program->Exec();
  if (context->Pop() == 2) Good("Stack was correct after -min exceeding min");
    else Bad("Stack was not correct after -min exceeding min");
  delete(program);

  program = forth->Parse("5 6 10 +min");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after +min");
    else Bad("Stack size was not correct after +min");
  if (context->Pop() == 11) Good("Stack was correct after +min exceeding min");
    else Bad("Stack was not correct after +min exceeding min");
  delete(program);

  program = forth->Parse("5 6 15 +min");
  context->Clear();
  program->Exec();
  if (context->Pop() == 15) Good("Stack was correct after +min not exceeding min");
    else Bad("Stack was not correct after +min not exceeding min");
  delete(program);

  program = forth->Parse("2 4 6 8 0 pick");
  context->Clear();
  program->Exec();
  if (context->Count() == 5) Good("Stack size was correct after pick");
    else Bad("Stack size was not correct after pick");
  if (context->Pop() == 8) Good("Stack was correct after pick");
    else Bad("Stack was not correct after pick");
  delete(program);

  program = forth->Parse("2 4 6 8 3 pick");
  context->Clear();
  program->Exec();
  if (context->Count() == 5) Good("Stack size was correct after pick");
    else Bad("Stack size was not correct after pick");
  if (context->Pop() == 2) Good("Stack was correct after pick");
    else Bad("Stack was not correct after pick");
  delete(program);

  program = forth->Parse("2 4 6 nip");
  context->Clear();
  program->Exec();
  if (context->Count() == 2) Good("Stack size was correct after nip");
    else Bad("Stack size was not correct after nip");
  if (context->At(1) == 2 && context->At(0) == 6)
    Good("Nip worked correctly");
  else
    Bad("Nip did not work correctly");
  delete(program);

  program = forth->Parse("1 2 4 6 tuck");
  context->Clear();
  program->Exec();
  if (context->Count() == 5) Good("Stack size was correct after tuck");
    else Bad("Stack size was not correct after tuck");
  if (context->At(4) == 1 && context->At(3) == 2 && context->At(2) == 6 &&
      context->At(1) == 4 && context->At(0) == 6)
    Good("Tuck worked correctly");
  else
    Bad("Tuck did not work correctly");
  delete(program);

  program = forth->Parse("10 12 and");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after and");
    else Bad("Stack size was not correct after and");
  if (context->Pop() == 8) Good("Stack was correct after and");
    else Bad("Stack was not correct after and");
  delete(program);

  program = forth->Parse("10 12 or");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after or");
    else Bad("Stack size was not correct after or");
  if (context->Pop() == 14) Good("Stack was correct after or");
    else Bad("Stack was not correct after or");
  delete(program);

  program = forth->Parse("10 12 xor");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after xor");
    else Bad("Stack size was not correct after xor");
  if (context->Pop() == 6) Good("Stack was correct after xor");
    else Bad("Stack was not correct after xor");
  delete(program);

  program = forth->Parse("10 not");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after not");
    else Bad("Stack size was not correct after not");
  if (context->Pop() == 0) Good("Stack was correct after not with 10");
    else Bad("Stack was not correct after not with 10");
  delete(program);

  program = forth->Parse("false not");
  context->Clear();
  program->Exec();
  if (context->Pop() == -1) Good("Stack was correct after not with false");
    else Bad("Stack was not correct after not with false");
  delete(program);

  program = forth->Parse("true not");
  context->Clear();
  program->Exec();
  if (context->Pop() == 0) Good("Stack was correct after not with true");
    else Bad("Stack was not correct after not with true");
  delete(program);

  program = forth->Parse("10 =0");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after =0");
    else Bad("Stack size was not correct after =0");
  if (context->Pop() == 0) Good("Stack was correct after =0 with 10");
    else Bad("Stack was not correct after =0 with 10");
  delete(program);

  program = forth->Parse("0 =0");
  context->Clear();
  program->Exec();
  if (context->Pop() != 0) Good("Stack was correct after =0 with 0");
    else Bad("Stack was not correct after =0 with 0");
  delete(program);

  program = forth->Parse("10 >0");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after >0");
    else Bad("Stack size was not correct after >0");
  if (context->Pop() != 0) Good("Stack was correct after >0 with 10");
    else Bad("Stack was not correct after >0 with 10");
  delete(program);

  program = forth->Parse("-10 >0");
  context->Clear();
  program->Exec();
  if (context->Pop() == 0) Good("Stack was correct after >0 with -10");
    else Bad("Stack was not correct after >0 with -10");
  delete(program);

  program = forth->Parse("0 >0");
  context->Clear();
  program->Exec();
  if (context->Pop() == 0) Good("Stack was correct after >0 with 0");
    else Bad("Stack was not correct after >0 with 0");
  delete(program);

  program = forth->Parse("10 <0");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after <0");
    else Bad("Stack size was not correct after <0");
  if (context->Pop() == 0) Good("Stack was correct after <0 with 10");
    else Bad("Stack was not correct after <0 with 10");
  delete(program);

  program = forth->Parse("-10 <0");
  context->Clear();
  program->Exec();
  if (context->Pop() != 0) Good("Stack was correct after <0 with -10");
    else Bad("Stack was not correct after <0 with -10");
  delete(program);

  program = forth->Parse("0 <0");
  context->Clear();
  program->Exec();
  if (context->Pop() == 0) Good("Stack was correct after <0 with 0");
    else Bad("Stack was not correct after <0 with 0");
  delete(program);

  program = forth->Parse("16 1+");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after 1+");
    else Bad("Stack size was not correct after 1+");
  if (context->Pop() == 17) Good("Stack was correct after 1+");
    else Bad("Stack was not correct after 1+");
  delete(program);

  program = forth->Parse("16 1-");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after 1-");
    else Bad("Stack size was not correct after 1-");
  if (context->Pop() == 15) Good("Stack was correct after 1-");
    else Bad("Stack was not correct after 1-");
  delete(program);

  program = forth->Parse("bl");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after bl");
    else Bad("Stack size was not correct after bl");
  if (context->Pop() == 32) Good("Stack was correct after bl");
    else Bad("Stack was not correct after bl");
  delete(program);

  program = forth->Parse("1 2 3 4 5 6 3 clearto");
  context->Clear();
  program->Exec();
  if (context->Count() == 2) Good("Stack size was correct after clearto");
    else Bad("Stack size was not correct after clearto");
  if (context->At(0) == 2 && context->At(1) == 1) Good("Stack was correct after clearto");
    else Bad("Stack was not correct after clearto");
  delete(program);

  program = forth->Parse("1 2 end 3 4");
  context->Clear();
  program->Exec();
  if (context->Count() == 2) Good("Stack size was correct after end");
    else Bad("Stack size was not correct after end");
  if (context->At(0) == 2 && context->At(1) == 1) Good("Stack was correct after end");
    else Bad("Stack was not correct after end");
  delete(program);

  program = forth->Parse("4 2 host");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after host");
    else Bad("Stack size was not correct after host");
  if (context->Pop() == 64) Good("Stack was correct after host");
    else Bad("Stack was not correct after host");
  delete(program);

  program = forth->Parse("10 invert");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after invert");
    else Bad("Stack size was not correct after invert");
  if (context->Pop() == -11) Good("Stack was correct after invert");
    else Bad("Stack was not correct after invert");
  delete(program);

  program = forth->Parse("1 2 3 4 2 ndrop");
  context->Clear();
  program->Exec();
  if (context->Count() == 2) Good("Stack size was correct after ndrop");
    else Bad("Stack size was not correct after ndrop");
  if (context->At(0) == 2 && context->At(1) == 1) Good("Stack was correct after ndrop");
    else Bad("Stack was not correct after ndrop");
  delete(program);

  program = forth->Parse("4 sgn");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after sgn");
    else Bad("Stack size was not correct after sgn");
  if (context->Pop() == 1) Good("Stack was correct after sgn with positive");
    else Bad("Stack was not correct after sgn with positive");
  delete(program);

  program = forth->Parse("-4 sgn");
  context->Clear();
  program->Exec();
  if (context->Pop() == -1) Good("Stack was correct after sgn with negative");
    else Bad("Stack was not correct after sgn with negative");
  delete(program);

  program = forth->Parse("0 sgn");
  context->Clear();
  program->Exec();
  if (context->Pop() == 0) Good("Stack was correct after sgn with zero");
    else Bad("Stack was not correct after sgn with zero");
  delete(program);

  program = forth->Parse("10 2 do r> r>");
  context->Clear();
  program->Exec();
  if (context->Count() == 2) Good("Stack size was correct after r>");
    else Bad("Stack size was not correct after r>");
  if (context->At(0) == 10 && context->At(1) == 2) Good("Stack was correct after r>");
    else Bad("Stack was not correct after r>");
  delete(program);

  program = forth->Parse("1 10 0 do r> drop 9 >r 1+ loop");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after >r");
    else Bad("Stack size was not correct after >r");
  if (context->Pop() == 2) Good("Stack was correct after >r");
    else Bad("Stack was not correct after >r");
  delete(program);

  program = forth->Parse("5 10 ! 7 10 +!");
  context->Clear();
  program->Exec();
  if (context->Count() == 0) Good("Stack size was correct after +!");
    else Bad("Stack size was not correct after +!");
  if (context->Read(10) == 12) Good("Memory was correct after +!");
    else Bad("Memory was not correct after +!");
  delete(program);

  program = forth->Parse("5 10 ! 7 10 -!");
  context->Clear();
  program->Exec();
  if (context->Count() == 0) Good("Stack size was correct after -!");
    else Bad("Stack size was not correct after -!");
  if (context->Read(10) == -2) Good("Memory was correct after -!");
    else Bad("Memory was not correct after -!");
  delete(program);

  program = forth->Parse("10 4 -?0");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after -?0");
    else Bad("Stack size was not correct after -?0");
  if (context->Pop() == 6) Good("Stack was correct after -?0 with >0 result");
    else Bad("Stack was not correct after -?0 with >0 result");
  delete(program);

  program = forth->Parse("10 14 -?0");
  context->Clear();
  program->Exec();
  if (context->Pop() == 0) Good("Stack was correct after -?0 with <0 result");
    else Bad("Stack was not correct after -?0 with <0 result");
  delete(program);

  program = forth->Parse("10 ?dup");
  context->Clear();
  program->Exec();
  if (context->Count() == 2) Good("Stack size was correct after ?dup with non-zero");
    else Bad("Stack size was not correct after ?dup with non-zero");
  if (context->At(0) == 10 && context->At(1) == 10) 
    Good("Stack was correct after ?dup with non-zero");
    else Bad("Stack was not correct after ?dup with non-zero");
  delete(program);

  program = forth->Parse("0 ?dup");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after ?dup with zero");
    else Bad("Stack size was not correct after ?dup with zero");
  if (context->Pop() == 0) Good("Stack was correct after ?dup with zero");
    else Bad("Stack was not correct after ?dup with zero");
  delete(program);

  program = forth->Parse("0 67 66 65 s.");
  context->Clear();
  program->Exec();
  if (context->Count() == 0) Good("Stack size was correct after s.");
    else Bad("Stack size was not correct after s.");
  if (strcmp(forthOutput,"ABC") == 0) Good("Correct value was output by s.");
    else Bad("Incorrect value was output by s.");
  delete(program);

  program = forth->Parse("14 0 67 66 65 sdrop");
  context->Clear();
  program->Exec();
  if (context->Count() == 1) Good("Stack size was correct after sdrop");
    else Bad("Stack size was not correct after sdrop");
  if (context->Pop() == 14) Good("Stack was correct after sdrop");
    else Bad("Stack was not correct after sdrop");
  delete(program);

  program = forth->Parse("\"ABC\"");
  context->Clear();
  program->Exec();
  if (context->Count() == 4) Good("Stack size was correct after constant string");
    else Bad("Stack size was not correct after constant string");
  if (context->At(0) == 65 && context->At(1) == 66 && context->At(2) == 67 &&
      context->At(3) == 0)
    Good("Stack was correct after constant string");
    else Bad("Stack was not correct after constant string");
  delete(program);

  program = forth->Parse("\"ABCDEF\" 2 left");
  context->Clear();
  program->Exec();
  if (context->Count() == 3) Good("Stack size was correct after left");
    else Bad("Stack size was not correct after left");
  if (context->At(0) == 65 && context->At(1) == 66 && context->At(2) == 0)
    Good("Stack was correct after left");
    else Bad("Stack was not correct after left");
  delete(program);

  program = forth->Parse("\"ABCDEF\" 2 right");
  context->Clear();
  program->Exec();
  if (context->Count() == 3) Good("Stack size was correct after right");
    else Bad("Stack size was not correct after right");
  if (context->At(0) == 69 && context->At(1) == 70 && context->At(2) == 0)
    Good("Stack was correct after right");
    else Bad("Stack was not correct after right");
  delete(program);

  program = forth->Parse("\"ABCDEF\" 3 2 mid");
  context->Clear();
  program->Exec();
  if (context->Count() == 3) Good("Stack size was correct after mid");
    else Bad("Stack size was not correct after mid");
  if (context->At(0) == 68 && context->At(1) == 69 && context->At(2) == 0)
    Good("Stack was correct after mid");
    else Bad("Stack was not correct after mid");
  delete(program);

  program = forth->Parse("\"ABC\" sdup");
  context->Clear();
  program->Exec();
  if (context->Count() == 8) Good("Stack size was correct after sdup");
    else Bad("Stack size was not correct after sdup");
  if (context->At(0) == 65 && context->At(1) == 66 && context->At(2) == 67 &&
      context->At(3) == 0 &&
      context->At(4) == 65 && context->At(5) == 66 && context->At(6) == 67 &&
      context->At(7) == 0)
    Good("Stack was correct after sdup");
    else Bad("Stack was not correct after sdup");
  delete(program);

  program = forth->Parse("\"ABC123\" len");
  context->Clear();
  program->Exec();
  if (context->Count() == 8) Good("Stack size was correct after len");
    else Bad("Stack size was not correct after len");
  if (context->Pop() == 6) Good("Stack was correct after len");
    else Bad("Stack was not correct after len");
  delete(program);




  forth->Variable("testvar", 37);
  program = forth->Parse("456 testvar !");
  context->Clear();
  program->Exec();
  if (context->Count() == 0) Good("Stack size was correct after variable usage");
    else Bad("Stack size was not correct after variable usage");
  if (context->Read(37) == 456) Good("Memory was correct after variable usage");
    else Bad("Memory was not correct after variable usage");
  delete(program);


  context->Clear();
  try {
    program = forth->Parse(": tripple 3 * ;");
    Good("No exception thrown during function definition");
    }
  catch (InvalidOpException &err) {
    Bad("Exception thrown during function definition");
    printf("Err : %s\n",err.Message());
    printf("Info: Aborting all further Forth tests\n");
    delete(forth);
    delete(context);
    return;
    }
  if (context->Count() == 0) Good("Stack size was correct after function definition");
    else Bad("Stack size was not correct after function definition");

  try {
    program = forth->Parse("5 tripple");
    Good("No exception thrown parsing program using custom function");
    }
  catch (InvalidOpException &err) {
    Bad("Exception thrown parsing program using custom function");
    printf("Err : %s\n",err.Message());
    printf("Info: Aborting all further Forth tests\n");
    delete(forth);
    delete(context);
    return;
    }
  context->Clear();
  try {
    program->Exec();
    Good("No exception thrown running program with custom function");
    }
  catch (InvalidOpException &err) {
    Bad("Exception thrown executing program using custom function");
    printf("Err : %s\n",err.Message());
    printf("Info: Aborting all further Forth tests\n");
    delete(forth);
    delete(context);
    return;
    }
  if (context->Count() == 1) Good("Stack size was correct after function execution");
    else Bad("Stack size was not correct after function execution");
  if (context->Pop() == 15) Good("Stack was correct after function execution");
    else Bad("Stack was not correct after function execution");
  delete(program);

  program = forth->Parse(": thirty 10 * tripple ;");
  program = forth->Parse("5 thirty");
  context->Clear();
  try {
    program->Exec();
    Good("No exception thrown running program with custom function calling custom function");
    }
  catch (InvalidOpException &err) {
    Bad("Exception thrown executing program using custom function calling custom function");
    printf("Err : %s\n",err.Message());
    printf("Info: Aborting all further Forth tests\n");
    delete(forth);
    delete(context);
    return;
    }
  if (context->Count() == 1) Good("Stack size was correct after function in function execution");
    else Bad("Stack size was not correct after function in function execution");
  if (context->Pop() == 150) Good("Stack was correct after function in function execution");
    else Bad("Stack was not correct after function in function execution");



  delete(forth);
  delete(context);
  }
