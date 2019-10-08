#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestFraction() {
  Fraction* obj;
  Fraction  f1(0, 0, 0);
  Fraction  f2(0, 0, 0);
  Fraction  f3(0, 0, 0);
  String    str;
  obj = new Fraction(2,3);
  printf("\nInfo: Fraction tests\n");
  if (obj != NULL) Good("Was able to instantiate a Fraction");
    else Bad("Could not instantiate a Fraction");
  if (strcmp(obj->Type(), "Fraction") == 0)
    Good("Fraction reported correctly on Type");
    else Bad("Fraction did not report correctly on Type");
  if (obj->Whole() == 0) Good("Fraction had correct whole portion");
    else Bad("Fraction did not have correct whole portion");
  if (obj->Numerator() == 2) Good("Fraction had correct numerator");
    else Bad("Fraction did not have correct numerator");
  if (obj->Denominator() == 3) Good("Fraction had correct denominator");
    else Bad("Fraction did not have correct denominator");
  str = obj->ToString();
  if (str.Equals("2/3")) Good("ToString worked correctly on 2/3");
    else Bad("ToString did not work correctly on 2/3");
  delete(obj);

  f1 = Fraction(1,4,5);
  if (f1.Whole() == 1 && f1.Numerator() == 4 && f1.Denominator() == 5)
    Good("Instantiating 1 4/5 worked correctly");
  else
    Bad("Instantiating 1 4/5 did not work correctly");
  str = f1.ToString();
  if (str.Equals("1 4/5")) Good("ToString worked correctly on 1 4/5");
    else Bad("ToString did not work correctly on 1 4/5");

  f1 = Fraction(0, 8, 6);
  if (f1.Whole() == 1 && f1.Numerator() == 1 && f1.Denominator() == 3)
    Good("Reducing 8/6 worked correctly");
  else
    Bad("Reducing 8/6 did not work correclty");

  f1 = Fraction(0, 6, 16);
  if (f1.Whole() == 0 && f1.Numerator() == 3 && f1.Denominator() == 8)
    Good("Reducing 6/16 worked correctly");
  else
    Bad("Reducing 6/16 did not work correclty");

  f1 = Fraction(0, 1, 4);
  f2 = Fraction(0, 1, 6);
  f3 = f1 + f2;
  if (f3.Whole() == 0 && f3.Numerator() == 5 && f3.Denominator() == 12)
    Good("1/4+1/6 resulted in 5/12");
  else
    Bad("1/4+1/6 did not result in 5/12");

  f1 = Fraction(0, 4, 5);
  f2 = Fraction(0, 2, 3);
  f3 = f1 + f2;
  if (f3.Whole() == 1 && f3.Numerator() == 7 && f3.Denominator() == 15)
    Good("4/5+2/3 resulted in 1 7/15");
  else
    Bad("4/5+2/3 did not result in 1 7/15");

  f1 = Fraction(0, 3, 8);
  f2 = Fraction(0, 3, 8);
  f3 = f1 + f2;
  if (f3.Whole() == 0 && f3.Numerator() == 3 && f3.Denominator() == 4)
    Good("3/8+3/8 resulted in 3/4");
  else
    Bad("3/8+3/8 did not result in 3/4");

  f1 = Fraction(1, 1, 2);
  f2 = Fraction(1, 1, 4);
  f3 = f1 + f2;
  if (f3.Whole() == 2 && f3.Numerator() == 3 && f3.Denominator() == 4)
    Good("1 1/2 + 1 1/4 resulted in 2 3/4");
  else
    Bad("1 1/2 + 1 1/4 did not result in 2 3/4");

  f1 = Fraction(0, 1, 8);
  f2 = Fraction(0, -1, 2);
  f3 = f1 + f2;
  if (f3.Whole() == 0 && f3.Numerator() == 3 && f3.Denominator() == 8 && 
      f3.Negative())
    Good("1/8 + -1/2 resulted in -3/8");
  else
    Bad("1/8 + -1/2 did not result in -3/8");
  str = f3.ToString();
  if (str.Equals("-3/8")) Good("ToString worked correctly on -3/8");
    else Bad("ToString did not work correctly on -3/8");

  f1 = Fraction(0, 1, 3);
  f2 = Fraction(0, 1, 4);
  f3 = f1 - f2;
  if (f3.Whole() == 0 && f3.Numerator() == 1 && f3.Denominator() == 12)
    Good("1/3 - 1/4 resulted in 1/12");
  else
    Bad("1/3 - 1/4 did not result in 1/12");

  f1 = Fraction(0, 1, 8);
  f2 = Fraction(0, 1, 2);
  f3 = f1 - f2;
  if (f3.Whole() == 0 && f3.Numerator() == 3 && f3.Denominator() == 8 && 
      f3.Negative())
    Good("1/8 - 1/2 resulted in -3/8");
  else
    Bad("1/8 - 1/2 did not result in -3/8");

  f1 = Fraction(0, 1, 4);
  f2 = Fraction(0, 1, 3);
  f3 = f1 * f2;
  if (f3.Whole() == 0 && f3.Numerator() == 1 && f3.Denominator() == 12 && 
      !f3.Negative())
    Good("1/4 * 1/3 resulted in 1/12");
  else
    Bad("1/4 * 1/3 did not result in 1/12");

  f1 = Fraction(5, 0, 0);
  f2 = Fraction(0,-1, 3);
  f3 = f1 * f2;
  if (f3.Whole() == 1 && f3.Numerator() == 2 && f3.Denominator() == 3 && 
      f3.Negative())
    Good("5 * -1/3 resulted in -1 2/3");
  else
    Bad("5 * -1/3 did not result in -1 2/3");

  f1 = Fraction(0, 2, 3);
  f2 = Fraction(0, 3, 4);
  f3 = f1 / f2;
  if (f3.Whole() == 0 && f3.Numerator() == 8 && f3.Denominator() == 9 && 
      !f3.Negative())
    Good("2/3 / 3/4 resulted in 8/9");
  else
    Bad("2/3 / 3/4 did not result in 8/9");

  f1 = Fraction(0, 2, 3);
  f2 = Fraction(0, 2, 3);
  if (f1 == f2)
    Good("2/3 was equal to 2/3");
  else
    Bad("2/3 was not equal to 2/3");

  f1 = Fraction(0, 2, 3);
  f2 = Fraction(0, 1, 3);
  if (f1 == f2)
    Bad("2/3 equal to 1/3");
  else
    Good("2/3 was not equal to 1/3");

  f1 = Fraction(0, 2, 3);
  f2 = Fraction(0, 1, 3);
  if (f1 != f2)
    Good("2/3 was unequal to 1/3");
  else
    Bad("2/3 was not unequal to 1/3");

  f1 = Fraction(0, 2, 3);
  f2 = Fraction(0, 2, 3);
  if (f1 != f2)
    Bad("2/3 was not unequal to 2/3");
  else
    Good("2/3 was equal to 2/3");

  f1 = Fraction(0.5);
  if (f1.Whole() == 0 && f1.Numerator() == 1 && f1.Denominator() == 2 && !f1.Negative())
    Good("Fraction(0.5) resulted in 1/2");
  else
    Bad("Fraction(0.5) did not result in 1/2");

  f1 = Fraction(-0.5);
  if (f1.Whole() == 0 && f1.Numerator() == 1 && f1.Denominator() == 2 && f1.Negative())
    Good("Fraction(-0.5) resulted in -1/2");
  else
    Bad("Fraction(-0.5) did not result in -1/2");

  f1 = Fraction(0.75);
  if (f1.Whole() == 0 && f1.Numerator() == 3 && f1.Denominator() == 4 && !f1.Negative())
    Good("Fraction(0.75) resulted in 3/4");
  else
    Bad("Fraction(0.75) did not result in 3/2");

  f1 = Fraction(-0.75);
  if (f1.Whole() == 0 && f1.Numerator() == 3 && f1.Denominator() == 4 && f1.Negative())
    Good("Fraction(-0.75) resulted in -3/4");
  else
    Bad("Fraction(-0.75) did not result in -3/2");


  }
