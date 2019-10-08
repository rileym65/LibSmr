#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestLine() {
  Line* obj;
  Line  line1;
  Line  line2;
  obj = new Line();
  printf("\nInfo: Line tests\n");
  if (obj != NULL) Good("Was able to instantiate an Line");
    else Bad("Could not instantiate an object");
  if (strcmp(obj->Type(), "Line") == 0)
    Good("Line reported correctly on Type");
    else Bad("Line did not report correctly on Type");
  delete(obj);

  line1 = Line(1,1,5,5);
printf("Slope %f\n",line1.Slope());
printf("Offset %f\n",line1.Offset());
  if (line1.Slope() == 1) Good("Line had correct slope");
    else Bad("Line did not have correct slope");
  if (line1.Offset() == 0) Good("Line had correct offset");
    else Bad("Line did not have correct offset");

  line1 = Line(1,4,5,8);
printf("Slope %f\n",line1.Slope());
printf("Offset %f\n",line1.Offset());
  if (line1.Slope() == 1) Good("Line had correct slope");
    else Bad("Line did not have correct slope");
  if (line1.Offset() == 3) Good("Line had correct offset");
    else Bad("Line did not have correct offset");

  line1 = Line(5,8,1,4);
printf("Slope %f\n",line1.Slope());
printf("Offset %f\n",line1.Offset());
  if (line1.Slope() == 1) Good("Line had correct slope");
    else Bad("Line did not have correct slope");
  if (line1.Offset() == 3) Good("Line had correct offset");
    else Bad("Line did not have correct offset");

  line1 = Line(1,1,5,2);
printf("Slope %f\n",line1.Slope());
printf("Offset %f\n",line1.Offset());
  if (line1.Slope() == 0.25) Good("Line had correct slope");
    else Bad("Line did not have correct slope");
  if (line1.Offset() == 0.75) Good("Line had correct offset");
    else Bad("Line did not have correct offset");

  line1 = Line(1,2,4,5);
printf("Slope %f\n",line1.Slope());
printf("Offset %f\n",line1.Offset());
  line2 = Line(1,5,5,1);
printf("Slope %f\n",line2.Slope());
printf("Offset %f\n",line2.Offset());
printf("intersect %f\n",line1.IntersectPos(&line2));
  if (line1.IntersectPos(&line2) == 2.5) Good("Intersect found intersection at correct spot");
    else Bad("Intersect did not produce the correct result");
  if (line1.Intersects(&line2)) Good("Intersects returned true for intersecting lines");
    else Bad("Intersects returned false for intersecting lines");
  }
