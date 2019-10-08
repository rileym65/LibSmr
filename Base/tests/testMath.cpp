#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestMath() {
  double d;
  printf("\nInfo: Math tests\n");
  d = Math::Log(10);
  printf("Info: Log(10) = %f\n",d);
  d = Math::Exp(10);
  printf("Info: Exp(10) = %f\n",d);
  d = Math::Pow(3,2.5);
  printf("Info: Pow(3, 2.5) = %f\n",d);
  d = Math::Sqrt(425);
  printf("Info: Sqrt(425) = %f\n",d);
  d = Math::Cos(2.5);
  printf("Info: Cos(2.5) = %f\n",d);
  d = Math::Sin(2.5);
  printf("Info: Sin(2.5) = %f\n",d);
  d = Math::Tan(2.5);
  printf("Info: Tan(2.5) = %f\n",d);
  d = Math::Log10(75);
  printf("Info: Log10(75) = %f\n",d);
  d = Math::Exp10(2.5);
  printf("Info: Exp10(2.5) = %f\n",d);
  d = Math::ACos(.4);
  printf("Info: ACos(.4) = %f\n",d);
  d = Math::ASin(.4);
  printf("Info: ASin(.4) = %f\n",d);
  d = Math::ATan(.4);
  printf("Info: ATan(.4) = %f\n",d);
  d = Math::Root(100, 3);
  printf("Info: Root(100, 3) = %f\n",d);
  d = Math::CosH(2.5);
  printf("Info: CosH(2.5) = %f\n",d);
  d = Math::SinH(2.5);
  printf("Info: SinH(2.5) = %f\n",d);
  d = Math::TanH(2.5);
  printf("Info: TanH(2.5) = %f\n",d);
  if (Math::Trunc((Single)2.3) == 2) Good("Trunc(2.3) returned 2");
    else Bad("Trunc(2.3) did not return 2");
printf("%d\n",Math::Trunc((Single)-2.3));
  if (Math::Trunc((Single)-2.3) == -2) Good("Trunc(-2.3) returned -2");
    else Bad("Trunc(-2.3) did not return -2");
  if (Math::Trunc((Single)2.7) == 2) Good("Trunc(2.7) returned 2");
    else Bad("Trunc(2.7) did not return 2");
  if (Math::Trunc((Single)-2.7) == -2) Good("Trunc(-2.7) returned -2");
    else Bad("Trunc(-2.7) did not return -2");

  if (Math::Ceiling((Double)2.3) == 3) Good("Ceiling(2.3) returned 3");
    else Bad("Ceiling(2.3) did not return 3");
  if (Math::Ceiling((Double)-2.3) == -2) Good("Ceiling(-2.3) returned -2");
    else Bad("Ceiling(-2.3) did not return -2");
  if (Math::Ceiling((Double)2.7) == 3) Good("Ceiling(2.7) returned 3");
    else Bad("Ceiling(2.7) did not return 3");
  if (Math::Ceiling((Double)-2.7) == -2) Good("Ceiling(-2.7) returned -2");
    else Bad("Ceiling(-2.7) did not return -2");
  if (Math::Ceiling((Double)2.0) == 2) Good("Ceiling(2.0) returned 2");
    else Bad("Ceiling(2.0) did not return 2");
  if (Math::Ceiling((Double)-2.0) == -2) Good("Ceiling(-2.0) returned -2");
    else Bad("Ceiling(-2.0) did not return -2");
  if (Math::Ceiling((Double)0.0) == 0) Good("Ceiling(0.0) returned 0");
    else Bad("Ceiling(0.0) did not return 0");

  if (Math::Floor((Double)2.3) == 2) Good("Floor(2.3) returned 2");
    else Bad("Floor(2.3) did not return 2");
  if (Math::Floor((Double)-2.3) == -3) Good("Floor(-2.3) returned -3");
    else Bad("Floor(-2.3) did not return -3");
  if (Math::Floor((Double)2.7) == 2) Good("Floor(2.7) returned 2");
    else Bad("Floor(2.7) did not return 2");
  if (Math::Floor((Double)-2.7) == -3) Good("Floor(-2.7) returned -3");
    else Bad("Floor(-2.7) did not return -3");
  if (Math::Floor((Double)2.0) == 2) Good("Floor(2.0) returned 2");
    else Bad("Floor(2.0) did not return 2");
  if (Math::Floor((Double)-2.0) == -2) Good("Floor(-2.0) returned -2");
    else Bad("Floor(-2.0) did not return -2");
  if (Math::Floor((Double)0.0) == 0) Good("Floor(0.0) returned 0");
    else Bad("Floor(0.0) did not return 0");

  if (Math::Round((Double)2.3) == 2) Good("Round(2.3) returned 2");
    else Bad("Round(2.3) did not return 2");
  if (Math::Round((Double)-2.3) == -2) Good("Round(-2.3) returned -2");
    else Bad("Round(-2.3) did not return -2");
  if (Math::Round((Double)2.7) == 3) Good("Round(2.7) returned 3");
    else Bad("Round(2.7) did not return 3");
  if (Math::Round((Double)-2.7) == -3) Good("Round(-2.7) returned -3");
    else Bad("Round(-2.7) did not return -3");
  if (Math::Round((Double)2.0) == 2) Good("Round(2.0) returned 2");
    else Bad("Round(2.0) did not return 2");
  if (Math::Round((Double)-2.0) == -2) Good("Round(-2.0) returned -2");
    else Bad("Round(-2.0) did not return -2");
  if (Math::Round((Double)0.0) == 0) Good("Round(0.0) returned 0");
    else Bad("Round(0.0) did not return 0");
  }
