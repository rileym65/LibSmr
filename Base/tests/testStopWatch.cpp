#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestStopWatch() {
  StopWatch* obj;
  TimeSpan   ts;
  obj = new StopWatch();
  printf("\nInfo: StopWatch tests\n");
  if (obj != NULL) Good("Was able to instantiate a StopWatch");
    else Bad("Could not instantiate a StopWatch");
  if (strcmp(obj->Type(), "StopWatch") == 0)
    Good("StopWatch reported correctly on Type");
    else Bad("StopWatch did not report correctly on Type");

  obj->Start();
  usleep(2000000);
  obj->Stop();
  ts = obj->Time();
  if (ts.AsSeconds() == 2) Good("StopWatch correctly timed 2 second pause");
    else Bad("StopWatch did not correctly time 2 second pause");

  delete(obj);
  }
