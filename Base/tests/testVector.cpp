#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestVector() {
  Vector* v1;
  Vector  va;
  Vector  vb;
  Vector  vc;
  printf("\nInfo: Vector tests\n");
  v1 = new Vector(1,2,3);
  if (v1 != NULL) Good("Was able to instantiate a Vector");
    else Bad("Could not instantiate a Vector");
  if (strcmp(v1->Type(), "Vector") == 0)
    Good("Vector reported correctly on Type");
    else Bad("Vector did not report correctly on Type");
  delete(v1);
  va = Vector(1,2,3);
  if (va.X() == 1) 
    Good("X was properly set on vector creation");
    else Bad("X as not properly set on vector creation");
  if (va.Y() == 2) 
    Good("Y was properly set on vector creation");
    else Bad("Y as not properly set on vector creation");
  if (va.Z() == 3) 
    Good("Z was properly set on vector creation");
    else Bad("Z as not properly set on vector creation");

  va = Vector(1,2,3);
  vb = Vector(4,5,6);
  vc = va + vb;
  if (vc.X() == 5)
    Good("X was correct after va + vb");
    else Bad("X was not correct after va + vb");
  if (vc.Y() == 7)
    Good("Y was correct after va + vb");
    else Bad("Y was not correct after va + vb");
  if (vc.Z() == 9)
    Good("Z was correct after va + vb");
    else Bad("Z was not correct after va + vb");

  vc = va - vb;
  if (vc.X() == -3)
    Good("X was correct after va - vb");
    else Bad("X was not correct after va - vb");
  if (vc.Y() == -3)
    Good("Y was correct after va - vb");
    else Bad("Y was not correct after va - vb");
  if (vc.Z() == -3)
    Good("Z was correct after va - vb");
    else Bad("Z was not correct after va - vb");

  va = Vector(1, 3, -5);
  vb = Vector(4, -2, -1);
  if (va.Dot(vb) == 3)
    Good("(1,3,-5) Dot (4,-2,-1) produced 3");
    else Bad("(1,3,-5) Dot (4,-2,-1) did not produce 3");

  va = Vector(1,2,3);
  vb = Vector(4,5,6);
  vc = va.Cross(vb);
  if (vc.X() == -3)
    Good("X was correct after (1,2,3) cross (4,5,6)");
    else Bad("X was not correct after (1,2,3) cross (4,5,6)");
  if (vc.Y() == 6)
    Good("Y was correct after (1,2,3) cross (4,5,6)");
    else Bad("Y was not correct after (1,2,3) cross (4,5,6)");
  if (vc.Z() == -3)
    Good("Z was correct after (1,2,3) cross (4,5,6)");
    else Bad("Z was not correct after (1,2,3) cross (4,5,6)");

  va = Vector(1, 3, -5);
  vb = Vector(4, -2, -1);
  va += vb;
  if (va.X() == 5)
    Good("X was correct after va += vb");
    else Bad("X was not correct after va += vb");
  if (va.Y() == 1)
    Good("Y was correct after va += vb");
    else Bad("Y was not correct after va += vb");
  if (va.Z() == -6)
    Good("Z was correct after va += vb");
    else Bad("Z was not correct after va += vb");

  va -= vb;
  if (va.X() == 1)
    Good("X was correct after va -= vb");
    else Bad("X was not correct after va -= vb");
  if (va.Y() == 3)
    Good("Y was correct after va -= vb");
    else Bad("Y was not correct after va -= vb");
  if (va.Z() == -5)
    Good("Z was correct after va -= vb");
    else Bad("Z was not correct after va -= vb");

  vb = va.Neg();
  if (vb.X() == -1)
    Good("X was correct after Neg");
    else Bad("X was not correct after Neg");
  if (vb.Y() == -3)
    Good("Y was correct after Neg");
    else Bad("Y was not correct after Neg");
  if (vb.Z() == 5)
    Good("Z was correct after Neg");
    else Bad("Z was not correct after Neg");

  va = Vector(4,6,8);
  if ((int)(va.Length() * 10000) == 107703)
    Good("Length produced correct result");
    else Bad("Length did not produce correct result");

  vb = va.Norm();
  if ((int)(vb.Length() * 10000) == 10000)
    Good("Length of normalized vector was 1.0");
    else Bad("Length of normalized vector was not 1.0");
  if ((int)(vb.X() * 10000) == 3713)
    Good("X was correct after Norm");
    else Bad("X was not correct after Norm");
  if ((int)(vb.Y() * 10000) == 5570)
    Good("Y was correct after Norm");
    else Bad("Y was not correct after Norm");
  if ((int)(vb.Z() * 10000) == 7427)
    Good("Z was correct after Norm");
    else Bad("Z was not correct after Norm");

  vb = va.Scale(3);
  if (vb.X() == 12)
    Good("X was correct after Scale");
    else Bad("X was not correct after Scale");
  if (vb.Y() == 18)
    Good("Y was correct after Scale");
    else Bad("Y was not correct after Scale");
  if (vb.Z() == 24)
    Good("Z was correct after Scale");
    else Bad("Z was not correct after Scale");

  vb = va * 2.0;
  if (vb.X() == 8)
    Good("X was correct after *");
    else Bad("X was not correct after *");
  if (vb.Y() == 12)
    Good("Y was correct after *");
    else Bad("Y was not correct after *");
  if (vb.Z() == 16)
    Good("Z was correct after *");
    else Bad("Z was not correct after *");

  va *= 5;
  if (va.X() == 20)
    Good("X was correct after *=");
    else Bad("X was not correct after *=");
  if (va.Y() == 30)
    Good("Y was correct after *=");
    else Bad("Y was not correct after *=");
  if (va.Z() == 40)
    Good("Z was correct after *=");
    else Bad("Z was not correct after *=");


  }
