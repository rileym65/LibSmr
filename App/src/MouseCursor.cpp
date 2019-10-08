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
#include <stdlib.h>
#include <string.h>
#include "SmrAppFramework.h"

namespace SmrFramework {

  MouseCursor::MouseCursor() {
    cursorType = Predefined;
    cursorNumber = 2;
    cursor = XCreateFontCursor(application->GetDisplay(), cursorNumber);
    }

  MouseCursor::MouseCursor(UInt32 i) {
    cursorType = Predefined;
    cursorNumber = i;
    cursor = XCreateFontCursor(application->GetDisplay(), cursorNumber);
    }

  MouseCursor::~MouseCursor() {
    }
  
  UInt32 MouseCursor::CursorNumber() {
    return cursorNumber;
    }
  
  UInt32 MouseCursor::CursorNumber(UInt32 i) {
    cursorNumber = i;
    return cursorNumber;
    }

  Byte MouseCursor::CursorType() {
    return cursorType;
    }
  
  Byte MouseCursor::CursorType(Byte b) {
    cursorType = b;
    return cursorType;
    }

  } 

