#include <stdio.h>
#include <SmrFramework.h>

int goodTests;
int badTests;

void Good(const char* message) {
  goodTests++;
  printf("Good: %s\n",message);
  }

void Bad(const char* message) {
  badTests++;
  printf("Bad : %s\n",message);
  }

