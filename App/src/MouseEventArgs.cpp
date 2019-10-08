/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/


#include "SmrAppFramework.h"

namespace SmrFramework {
  
  MouseEventArgs::MouseEventArgs(int x, int y, int button, int state) {
    i1 = x;
    i2 = y;
    i3 = button;
    i4 = state;
    this->objectType = (char*)"MouseEventArgs";
    }

  MouseEventArgs::~MouseEventArgs() {
    }

  Int32 MouseEventArgs::X() {
    return i1;
    }

  Int32 MouseEventArgs::Y() {
    return i2;
    }

  Int32 MouseEventArgs::Button() {
    return i3;
    }

  Int32 MouseEventArgs::State() {
    return i4;
    }

  }

