#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestRandom() {
  int i;
  Random* rng;
  printf("\nInfo: Random tests\n");
  rng = new Random();
  printf("Info: Random(10) x 10:");
  for (i=0; i<10; i++)
    printf(" %d",rng->Next(10));
  printf("\n");

  delete(rng);
  }
