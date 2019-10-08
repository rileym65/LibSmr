/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

#include <stdio.h>
#include "string.h"
#include "SmrFramework.h"

namespace SmrFramework {

  Palette::Palette() {
    objectType = (char*)"Palette";
    colors = NULL;
    counts = NULL;
    numColors = 0;
    redMask = 0xff;
    greenMask = 0xff;
    blueMask = 0xff;
    }
  
  Palette::~Palette() {
    if (colors != NULL) free(colors);
    if (counts != NULL) free(counts);
    }
  
  void Palette::Add(Byte red, Byte green, Byte blue) {
    UInt32 color;
    UInt32 start;
    UInt32 end;
    UInt32 mid;
    red &= redMask;
    green &= greenMask;
    blue &= blueMask;
    color = (red << 16) | (green << 8) | blue;
    if (numColors == 0) {
      colors = (UInt32*)malloc(sizeof(UInt32));
      counts = (UInt32*)malloc(sizeof(UInt32));
      colors[0] = color;
      counts[0] = 1;
      numColors++;
      return;
      }

/*
    i = 0;
    while (i < numColors && color <= colors[i]) {
      if (color == colors[i]) {
        counts[i]++;
        return;
        }
      i++;
      }
*/

    start = 0;
    end = numColors - 1;
    mid = start + ((end - start) / 2);
    while (mid != start && mid != end) {
      if (color == colors[mid]) {
        start = mid;
        end = mid;
        }
      if (color > colors[mid]) {
        start = mid;
        mid = start + ((end - start) / 2);
        }
      else if (color < colors[mid]) {
        end = mid;
        mid = start + ((end - start) / 2);
        }
      }

    while (mid > 0 && colors[mid-1] > color) mid--;
    while (mid < numColors-1 && colors[mid+1] < color) mid++;
    if (color == colors[mid]) {
      counts[mid]++;
      return;
      }

    numColors++;
    colors = (UInt32*)realloc(colors,sizeof(UInt32) * numColors);
    counts = (UInt32*)realloc(counts,sizeof(UInt32) * numColors);
    memmove(&(colors[mid+1]), &(colors[mid]), sizeof(UInt32) * (numColors - 1 - mid));
    colors[mid] = color;
    counts[mid] = 1;
    };

  UInt32 Palette::Count() {
    return numColors;
    }

  Byte Palette::Blue(UInt32 index) {
    if (index <0 || index >= numColors)
      throw RangeException(this, "Index out of range");
    return (colors[index] & 0xff);
    }

  Byte Palette::Green(UInt32 index) {
    if (index <0 || index >= numColors)
      throw RangeException(this, "Index out of range");
    return ((colors[index] >> 8) & 0xff);
    }

  Byte Palette::Red(UInt32 index) {
    if (index <0 || index >= numColors)
      throw RangeException(this, "Index out of range");
    return ((colors[index] >> 16) & 0xff);
    }

  Palette* Palette::Reduce(UInt32 maxColors) {
    UInt32  i;
//    UInt32  j;
//    UInt32  total;
    UInt32* chi;
    Palette* ret;
    bool     flag;
    UInt32   temp;
    UInt32   end;
    UInt32  r1,g1,b1;
//    UInt32  r1,r2,g1,g2,b1,b2;
    chi = (UInt32*)malloc(sizeof(UInt32) * numColors);
    if (chi == NULL) throw InvalidOpException(this, "Could not allocate memory");
/*
    for (i=0; i<numColors; i++) {
      r1 = ((colors[i] >> 16) & 0xff);
      g1 = ((colors[i] >> 8) & 0xff);
      b1 = (colors[i] & 0xff);
      total = 0;
      for (j=0; j<numColors; j++) {
        r2 = ((colors[j] >> 16) & 0xff);
        g2 = ((colors[j] >> 8) & 0xff);
        b2 = (colors[j] & 0xff);
        total += ((r1-r2)*(r1-r2));
        total += ((g1-g2)*(g1-g2));
        total += ((b1-b2)*(b1-b2));
        }
      chi[i] = total / numColors;
      }
*/
    flag = true;
    end = numColors;
    while (flag) {
      flag = false;
      for (i=0; i<end-1; i++)
//        if (chi[i] < chi[i+1]) {
        if (counts[i] < counts[i+1]) {
          flag = true;
          temp = chi[i]; chi[i] = chi[i+1]; chi[i+1] = temp;
          temp = counts[i]; counts[i] = counts[i+1]; counts[i+1] = temp;
          temp = colors[i]; colors[i] = colors[i+1]; colors[i+1] = temp;
          }
      end--;
      }
    ret = new Palette();
    for (i=0; i<maxColors; i++) {
      r1 = ((colors[i] >> 16) & 0xff);
      g1 = ((colors[i] >> 8) & 0xff);
      b1 = (colors[i] & 0xff);
      ret->Add(r1, g1, b1);
      }
    return ret;
    }

  UInt32 Palette::Lookup(Byte red, Byte green, Byte blue) {
    UInt32 i;
    Byte r,g,b;
    UInt32 total;
    UInt32 current;
    UInt32 index;
    current = 0xffffffff;
    for (i=0; i<numColors; i++) {
      r = ((colors[i] >> 16) & 0xff);
      g = ((colors[i] >> 8) & 0xff);
      b = (colors[i] & 0xff);
      total = (red-r)*(red-r);
      total += (green-g)*(green-g);
      total += (blue-b)*(blue-b);
      if (total < current) {
        current = total;
        index = i;
        }
      }
    return index;
    }

  void Palette::SignificantBits(Byte bits) {
    if (bits < 0 || bits > 8) throw RangeException(this, "Value out of range");
    redMask = 0xff << (8-bits);
    greenMask = 0xff << (8-bits);
    blueMask = 0xff << (8-bits);
    };

  }

