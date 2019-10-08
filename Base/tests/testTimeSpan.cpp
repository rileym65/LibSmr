#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestTimeSpan() {
  TimeSpan* obj;
  TimeSpan tsa;
  TimeSpan tsb;
  obj = new TimeSpan(2,4,30,15);
  printf("\nInfo: TimeSpan tests\n");
  if (obj != NULL) Good("Was able to instantiate a TimeSpan");
    else Bad("Could not instantiate a TimeSpan");
  if (strcmp(obj->Type(), "TimeSpan") == 0)
    Good("TimeSpan reported correctly on Type");
    else Bad("TimeSpan did not report correctly on Type");
  if (obj->Days() == 2)
    Good("TimeSpan reported correct days");
    else Bad("TimeSpan did not report correct days");
  if (obj->Hours() == 4)
    Good("TimeSpan reported correct hours");
    else Bad("TimeSpan did not report correct hours");
  if (obj->Minutes() == 30)
    Good("TimeSpan reported correct minutes");
    else Bad("TimeSpan did not report correct minutes");
  if (obj->Seconds() == 15)
    Good("TimeSpan reported correct seconds");
    else Bad("TimeSpan did not report correct seconds");
  delete(obj);

  tsa = TimeSpan(2,4,30,15);
  tsb = tsa.AddDays(5);
  if (tsb.Days() == 7)
    Good("TimeSpan worked correctly on AddDays");
    else Bad("TimeSpan did not work correctly on AddDays");
  tsb = tsa.AddHours(2);
  if (tsb.Hours() == 6)
    Good("TimeSpan worked correctly on AddHours");
    else Bad("TimeSpan did not work correctly on AddHours");
  tsb = tsa.AddMinutes(15);
  if (tsb.Minutes() == 45)
    Good("TimeSpan worked correctly on AddMinutes");
    else Bad("TimeSpan did not work correctly on AddMinutes");
  tsb = tsa.AddSeconds(20);
  if (tsb.Seconds() == 35)
    Good("TimeSpan worked correctly on AddSeconds");
    else Bad("TimeSpan did not work correctly on AddSeconds");

  tsa = TimeSpan(1,25,60,65);
  if (tsa.Days() == 2)
    Good("Day was correct after normalizing 1:25:60:65");
    else Bad("Day was not correct after normalizing 1:25:60:65");
  if (tsa.Hours() == 2)
    Good("Hours was correct after normalizing 1:25:60:65");
    else Bad("Hours was not correct after normalizing 1:25:60:65");
  if (tsa.Minutes() == 1)
    Good("Minutes was correct after normalizing 1:25:60:65");
    else Bad("Minutes was not correct after normalizing 1:25:60:65");
  if (tsa.Seconds() == 5)
    Good("Seconds was correct after normalizing 1:25:60:65");
    else Bad("Seconds was not correct after normalizing 1:25:60:65");
  
  tsa = TimeSpan(1, 12, 30, 15);
  tsb = tsa.Add(TimeSpan(2, 6, 10, 50));
  if (tsb.Days() == 3 &&
      tsb.Hours() == 18 &&
      tsb.Minutes() == 41 &&
      tsb.Seconds() == 5)
    Good("Add correctly added two TimeSpans");
    else Bad("Add did not correctly added two TimeSpans");

  }
