#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

Int32 tests[][3] = {
  { 205, 804, 4 },
  { 205, 302, 5 },
  { 205, 308, 7 },
  { 205, 309, 9 },
  { 309, 205, 9 },
  {   0,   1, 2 },
  {   0, 100, 1 },
  {   0, 200, 1 },
  {   0,   2, 4 },
  {   0, 101, 3 },
  {   0, 201, 2 },
  {   0, 300, 2 },
  {   0, 400, 2 },
  {   0,   3, 6 },
  {   0, 102, 5 },
  {   0, 301, 3 },
  {   0, 401, 3 },
  {   0, 500, 3 },
  {   0, 600, 3 },
  {   0, 202, 4 },
  {   0, 402, 4 },
  {   0, 501, 4 },
  {   0, 601, 4 },
  {   0, 700, 4 },
  {   0, 800, 4 },
  {   0, 301, 3 },
  {   0, 500, 3 },
  { 501,   0, 4 },
  {   0, 404, 8 },
  {   0, 304, 9 },
  {   0, 204, 8 },
  {   0, 504, 9 },
  { 201, 210, 18 },
  { 201, 310, 19 },
  { 201, 410, 18 },
  { 201, 510, 19 },
  { 201, 610, 18 },
  { 201, 710, 19 },
  { 201, 810, 18 },
  { 201, 910, 19 },
  { 201, 806, 10 },
  { 201, 906, 11 },
  { 201,1006, 10 },
  { 201,1106, 11 },
  { 201,1206, 10 },
  { 201,1306, 11 },
  { 201,1205,  9 },
  { 201,1305, 10 },
  { 201,1204,  8 },
  { 201,1304,  9 },
  { 201,1203,  7 },
  { 201,1303,  8 },
  { 201,1202,  6 },
  { 201,1302,  7 },
  { 201,1201,  5 },
  { 201,1301,  6 },
  { 201,1200,  6 },
  { 201,1300,  6 },
  { 804, 205,  4 },
  { 804, 308,  9 },
  { 804, 302,  4 },
  { 308, 205,  7 },
  { 308, 804,  9 },
  { 308, 302, 12 },
  { 302, 205,  5 },
  { 302, 804,  4 },
  { 302, 308, 12 },
  {-1, -1, -1}
  };

Int32 dir[][3] = {
  { 804, 1, 604 },
  { 804, 2, 704 },
  { 804, 3, 904 },
  { 804, 4, 1004 },
  { 804, 5, 903 },
  { 804, 6, 703 },
  { 302, 1, 102 },
  { 302, 2, 203 },
  { 302, 3, 403 },
  { 302, 4, 502 },
  { 302, 5, 402 },
  { 302, 6, 202 },
  { 0, 1, -1 },
  { 0, 2, -1 },
  { 0, 3, 100 },
  { 0, 4, 200 },
  { 0, 5, -1 },
  { 0, 6, -1 },
  { 16, 1, -1 },
  { 16, 2, -1 },
  { 16, 3, -1 },
  { 16, 4, 216 },
  { 16, 5, 115 },
  { 16, 6, -1 },
  { 1800, 1, 1600 },
  { 1800, 2, 1700 },
  { 1800, 3, -1 },
  { 1800, 4, -1 },
  { 1800, 5, -1 },
  { 1800, 6, -1 },
  { 1816, 1, 1616 },
  { 1816, 2, -1 },
  { 1816, 3, -1 },
  { 1816, 4, -1 },
  { 1816, 5, -1 },
  { 1816, 6, 1715 },
  { 100, 1, -1 },
  { 100, 2, 1 },
  { 100, 3, 201 },
  { 100, 4, 300 },
  { 100, 5, 200 },
  { 100, 6, 0 },
  { 115, 1, -1 },
  { 115, 2, 16 },
  { 115, 3, 216 },
  { 115, 4, 315 },
  { 115, 5, 215 },
  { 115, 6, 15 },
  { 1715, 1, 1515 },
  { 1715, 2, 1616 },
  { 1715, 3, 1816 },
  { 1715, 4, -1 },
  { 1715, 5, 1815 },
  { 1715, 6, 1615 },
  { 1700, 1, 1500 },
  { 1700, 2, 1601 },
  { 1700, 3, 1801 },
  { 1700, 4, -1 },
  { 1700, 5, 1800 },
  { 1700, 6, 1600 },
  {-1, -1, -1}
  };

void TestHexGrid() {
  int i;
  int failed;
  char buffer[1024];
  HexGrid* grid;
  GridObject*  obj1;
  GridObject*  obj2;
  GridObject*  obj3;
  GridObject*  obj4;
  grid = new HexGrid(17,19);
  obj1 = NULL;
  obj2 = NULL;
  obj3 = NULL;
  obj4 = NULL;
  printf("\nInfo: HexGrid tests\n");
  if (grid != NULL) Good("Was able to instantiate an HexGrid");
    else Bad("Could not instantiate an HexGrid");
  if (strcmp(grid->Type(), "HexGrid") == 0)
    Good("HexGrid reported correctly on Type");
    else Bad("HexGrid did not report correctly on Type");
  if (grid->Width() == 17) Good("Grid was created with specified width");
    else Bad("Grid was not created with specified width");
  if (grid->Height() == 19) Good("Grid was created with specified height");
    else Bad("Grid was not created with specified height");

  obj1 = grid->At(205);
  if (obj1 == NULL) Good("Tested grid cell contained NULL");
    else Bad("Tested grid cell did not contain NULL");

  obj1 = new GridObject();
  grid->AtPut(205, obj1);
  if (grid->At(205) == obj1) Good("Was able to set grid cell to object");
    else Bad("Was not able to set grid cell to object");

  obj2 = new GridObject();
  obj3 = new GridObject();
  obj4 = new GridObject();
  grid->AtPut(204, obj2);
  grid->AtPut(308, obj3);
  grid->AtPut(302, obj4);
  if (grid->At(205) == obj1 &&
      grid->At(204) == obj2 &&
      grid->At(308) == obj3 &&
      grid->At(302) == obj4) Good("Placing additional objects worked");
    else Bad("Placing additional objects failed");

  grid->Move(204, 804);
  if (grid->At(204) == NULL) Good("Move cleared cell at starting cell");
    else Bad("Move did not clear object at starting cell");
  if (grid->At(804) == obj2) Good("Move correctly moved object to new cell");
    else Bad("Move did not correctly move object to new cell");


  i = 0;
  failed = 0;
  while (dir[i][0] != -1) {
    if (grid->Next(dir[i][0],dir[i][1]) != dir[i][2]) {
      sprintf(buffer,"Next was wrong dir %d from %04d (%04d): %04d\n",
              dir[i][1],dir[i][0],dir[i][2],
              grid->Next(dir[i][0],dir[i][1]));
      Bad(buffer);
      failed++;
      }
    i++;
    }
  if (failed == 0) Good("All Next tests passed");

  i = 0;
  failed = 0;
  while (tests[i][0] != -1) {
    if (grid->Distance(tests[i][0],tests[i][1]) != tests[i][2]) {
      sprintf(buffer,"Distance from %04d to %04d [%d] was wrong: %d",
              tests[i][0], tests[i][1], tests[i][2],
              grid->Distance(tests[i][0],tests[i][1]));
      Bad(buffer);
      failed++;
      }
    i++;
    }
  if (failed == 0) Good("All distance tests passed");

  if (grid->Find(obj2) == 804)
    Good("Find found the object at the correct location");
  else
    Bad("Find did not find the object at the correct location");

  if (grid->DirectionTo(804, 204) == 1)
    Good("Direction from 0804 to 0204 was reported as N");
  else
    Bad("Direction from 0804 to 0204 was not reported as N");

  if (grid->DirectionTo(804, 1004) == 4)
    Good("Direction from 0804 to 1004 was reported as S");
  else
    Bad("Direction from 0804 to 1004 was not reported as S");

  if (grid->DirectionTo(804, 808) == 2)
    Good("Direction from 0804 to 0808 was reported as NE");
  else
    Bad("Direction from 0804 to 0808 was not reported as NE");

  if (grid->DirectionTo(804, 801) == 6)
    Good("Direction from 0804 to 0801 was reported as NW");
  else
    Bad("Direction from 0804 to 0801 was not reported as NW");

  if (grid->DirectionTo(804, 607) == 2)
    Good("Direction from 0804 to 0607 was reported as NE");
  else
    Bad("Direction from 0804 to 0607 was not reported as NE");

  if (grid->DirectionTo(804, 1008) == 3)
    Good("Direction from 0804 to 1008 was reported as SE");
  else
    Bad("Direction from 0804 to 1008 was not reported as SE");

  if (grid->DirectionTo(804, 1001) == 5)
    Good("Direction from 0804 to 1001 was reported as SW");
  else
    Bad("Direction from 0804 to 1001 was not reported as SW");

  if (grid->DirectionTo(804, 201) == 6)
    Good("Direction from 0804 to 0201 was reported as NW");
  else
    Bad("Direction from 0804 to 0201 was not reported as NW");

  if (obj1 != NULL) delete(obj1);
  if (obj2 != NULL) delete(obj2);
  if (obj3 != NULL) delete(obj3);
  if (obj4 != NULL) delete(obj4);
  delete(grid);
  }


/*
   00  01  02  03  04  05  06  07  08  09  10  11  12  13  14  15  16
    _00 _01 _02 _03 _04 _05 _06 _07 _08 _09 _10 _11 _12 _13 _14 _15 _
00 / \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \
01 \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/
02 / \_/1\_/ \_/ \_/ \_/A\_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \
03 \_/ \_/ \_/D\_/ \_/ \_/ \_/ \_/ \_/C\_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/
04 / \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \
05 \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/
06 / \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \
07 \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/
08 / \_/ \_/ \_/ \_/B\_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \
09 \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/
10 / \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \
11 \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/
12 / \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \
13 \_/2\_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/
14 / \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \
15 \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/
16 / \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \
17 \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/
18 / \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \
   \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/
*/

