#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

class Test1 : public Callback {
  protected:
  public:
    Test1() {
      }
    Boolean Call(void* arg) {
      int *i;
      i = (int*)arg;
      if (*i != 0) return true;
      return false;
      }
  };

class Test2 {
  protected:
    Callback* cb;
  public:
    Test2(Callback* c) { cb = c; }
    void TestTrue() {
      int i;
      i = 1;
      if (cb->Call(&i)) Good("Callback returned a true");
        else Bad("Callback did not return a true");
      }
    void TestFalse() {
      int i;
      i = 0;
      if (!cb->Call(&i)) Good("Callback returned a false");
        else Bad("Callback did not return a false");
      }
    };

void TestCallback() {
  Test1* obj;
  Test2* test;
  obj = new Test1();
  test = new Test2(obj);
  printf("\nInfo: Callback tests\n");
  if (obj != NULL) Good("Was able to instantiate a Callback");
    else Bad("Could not instantiate a Callback");
  if (strcmp(obj->Type(), "Callback") == 0)
    Good("Callback reported correctly on Type");
    else Bad("Callback did not report correctly on Type");
  test->TestTrue();
  test->TestFalse();
  delete(test);
  delete(obj);
  }
