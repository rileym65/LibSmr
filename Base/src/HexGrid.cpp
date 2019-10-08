/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

#include <math.h>
#include <stdio.h>
#include "string.h"
#include "SmrFramework.h"

namespace SmrFramework {

  HexGrid::HexGrid() {
    width = 0;
    height = 0;
    objectType = (char*)"HexGrid";
    Clear();
    }
  
  HexGrid::HexGrid(UInt32 w, UInt32 h) {
    width = w;
    height = h;
    objectType = (char*)"HexGrid";
    Clear();
    }

  HexGrid::~HexGrid() {
    }
  
  GridObject* HexGrid::At(UInt32 pos) {
    UInt32 x,y;
    x = pos % 100;
    y = pos / 100;
    if (y >= height) return NULL;
    if ((y & 1) == 1) {
      if (x >= (width - 1)) return NULL;
      }
    if (x >= width) return NULL;
    return grid[y][x];
    }

  GridObject* HexGrid::AtPut(UInt32 pos, GridObject* v) {
    UInt32 x,y;
    x = pos % 100;
    y = pos / 100;
    if (y >= height) return NULL;
    if ((y & 1) == 1) {
      if (x >= (width - 1)) return NULL;
      }
    if (x >= width) return NULL;
    grid[y][x] = v;
    return grid[y][x];
    }

  void HexGrid::Clear() {
    UInt32 x,y;
    for (y=0; y<100; y++)
      for (x=0; x<100; x++) {
        grid[y][x] = NULL;
        }
    }

  Byte HexGrid::DirectionTo(UInt32 spos, UInt32 tpos) {
    UInt32 x1,y1,x2,y2;
    x1 = spos % 100;
    y1 = spos / 100;
    x2 = tpos % 100;
    y2 = tpos / 100;
    if (y1 >= height || y2 >= height) return 0;
    if ((y1 & 1) == 1) {
      if (x1 >= (width - 1)) return 0;
      }
    if ((y2 & 1) == 1) {
      if (x2 >= (width - 1)) return 0;
      }
    if (x1 >= width || x2 >= width) return 0;
    if (x1 == x2) {
      if (y1 < y2) return DirSouth;
      if (y1 > y2) return DirNorth;
      return 0;
      }
    if (y1 == y2) {
      if (x1 < x2) return (y1 != 0) ? DirNorthEast : DirSouthEast;
      if (x1 > x2) return (y1 != 0) ? DirNorthWest : DirSouthWest;
      return 0;
      }
    if (x1 < x2 && y1 > y2) return DirNorthEast;
    if (x1 < x2 && y1 < y2) return DirSouthEast;
    if (x1 > x2 && y1 > y2) return DirNorthWest;
    if (x1 > x2 && y1 < y2) return DirSouthWest;
    return 0;
    }

  Int32 HexGrid::Distance(UInt32 pos1, UInt32 pos2) {
    Int32 x1,y1,x2,y2;
    Int32 ret;
    Int32 dx;
    Int32 dy;
    x1 = pos1 % 100;
    y1 = pos1 / 100;
    x2 = pos2 % 100;
    y2 = pos2 / 100;
    if ((UInt32)y1 >= height || (UInt32)y2 >= height) return -1;
    if ((y1 & 1) == 1) {
      if ((UInt32)x1 >= (width - 1)) return -1;
      }
    if ((y2 & 1) == 1) {
      if ((UInt32)x2 >= (width - 1)) return -1;
      }
    if ((UInt32)x1 >= width || (UInt32)x2 >= width) return -1;
    dx = 2 * (x2-x1);
    if (dx < 0) dx = -dx;
    dy = y2 - y1;
    if (dy < 0) dy = -dy;
    dy = (dy + 1) / 2;
    if ((y1 & 1) != (y2 & 1)) {
      if ((y1 & 1) == 0) {
        if (x2 < x1) dx--;
        if (x2 > x1) dx++;
        }
      else {
        if (x2 < x1) dx++;
        if (x2 > x1) dx--;
        }
      }
    if (dx <= dy * 2)
      ret = (dy * 2 - dx) / 2 + dx;
    else ret = dx;
    return ret;
    }

  Int32 HexGrid::Find(GridObject* obj) {
    UInt32 x,y;
    for (y=0; y<height; y++)
      for (x=0; x<width; x++)
        if (grid[y][x] == obj) return y * 100 + x;
    return -1;
    }

  UInt32 HexGrid::Height() {
    return height;
    }
  
  UInt32 HexGrid::Height(UInt32 i) {
    height = i;
    return height;
    }

  GridObject* HexGrid::Move(UInt32 psrc, UInt32 pdst) {
    GridObject* v;
    v = At(psrc);
    if (v == NULL) return NULL;
    AtPut(pdst, v);
    AtPut(psrc, NULL);
    return v;
    }

  Int32 HexGrid::Next(UInt32 pos, Byte dir) {
    UInt32 x,y;
    if (dir < 1 || dir > 6) return -1;
    x = pos % 100;
    y = pos / 100;
    if (y >= height) return -1;
    if ((y & 1) == 1) {
      if (x >= (width - 1)) return -1;
      }
    if (x >= width) return -1;
    switch (dir) {
      case 1:
           y -= 2;
           break;
      case 2:
           x += ((y & 1) == 1) ? 1 : 0;
           y--;
           break;
      case 3:
           x += ((y & 1) == 1) ? 1 : 0;
           y++;
           break;
      case 4:
           y += 2;
           break;
      case 5:
           x -= ((y & 1) == 1) ? 0 : 1;
           y++;
           break;
      case 6:
           x -= ((y & 1) == 1) ? 0 : 1;
           y--;
           break;
      }
    if (y >= height) return -1;
    if ((y & 1) == 1) {
      if (x >= (width - 1)) return -1;
      }
    if (x >= width) return -1;
    return (y * 100) + x;
    }

  UInt32 HexGrid::Width() {
    return width;
    }
  
  UInt32 HexGrid::Width(UInt32 i) {
    height = i;
    return width;
    }

  }

