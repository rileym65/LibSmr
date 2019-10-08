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

  /* ******************** */
  /* ***** Read XBM ***** */
  /* ******************** */
  void Image::readXBM() {
  int       i;
  String    line;
  String    number;
  char      start;
  int       linePos;
  UInt32    x,y;
  UInt16    pos;
  bool      done;
  Byte      value;
  Byte      chr;
  Byte      mask;
  bool      isHex;
  bool      term;
  StreamReader* file;
  width = -1;
  height = -1;
  start = 0;
  file = new StreamReader(this->file);
  this->file = NULL;
  while (start != 7) {
    line = file->ReadLine().ToLower();
    linePos = line.LastIndexOf("width");
    if (linePos >= 0) {
      linePos += 5;
      while (line.CharAt(linePos) == ' ') linePos++;
      number = line.Substring(linePos);
      width = Convert::ToUInt32(&number);
      start |= 1;
      }
    linePos = line.LastIndexOf("height");
    if (linePos >= 0) {
      linePos += 6;
      while (line.CharAt(linePos) == ' ') linePos++;
      number = line.Substring(linePos);
      height = Convert::ToUInt32(&number);
      start |= 2;
      }
    linePos = line.LastIndexOf('{');
    if (linePos >= 0) {
      start |= 4;
      }
    }
  data = (Byte*)malloc(width * height * 3);
  if (data == NULL) throw InvalidOpException(this, "Could not allocate memory");
  x = 0;
  y = 0;
  done = false;
  line = file->ReadLine();
  pos = 0;
  while (!done) {
    if (pos >= line.Length()) {
      line = file->ReadLine();
      pos = 0;
      }
    while (line.CharAt(pos) == ' ') {
      pos++;
      if (pos >= line.Length()) {
        line = file->ReadLine();
        pos = 0;
        }
      }
    isHex = false;
    value = 0;
    term = false;
    while (!term) {
      if (pos >= line.Length()) term = true;
      else {
        chr = line.CharAt(pos++);
        if (chr >= '0' && chr <= '9') {
          value *= (isHex) ? 16 : 10;
          value += chr - '0';
          }
        if (chr >= 'a' && chr <= 'f' && isHex) {
          value <<= 4;
          value += chr - 'a' + 10;
          }
        if (chr >= 'A' && chr <= 'F' && isHex) {
          value <<= 4;
          value += chr - 'A' + 10;
          }
        if (chr == 'x' || chr == 'X') isHex = true;
        if (chr == ',') term = true;
	if (chr == '}') term = true;
        if (chr == ' ') term = true;
        }
      }
    mask = 1;
    for (i=0; i<8; i++) {
      if (value & mask) {
        data[y*width*3+x*3+0] = 0;
        data[y*width*3+x*3+1] = 0;
        data[y*width*3+x*3+2] = 0;
        } else {
        data[y*width*3+x*3+0] = 255;
        data[y*width*3+x*3+1] = 255;
        data[y*width*3+x*3+2] = 255;
        }
      mask <<= 1;
      x++;
      if (x >= width) {
        x = 0;
        y++;
        i=8;
        if (y >= height) done = -1;
        }
      }
    }
  }

  }

