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

  PaintEventArgs::PaintEventArgs(int x,int y,int width,int height,int count) {
    i1 = x;
    i2 = y;
    i3 = width;
    i4 = height;
    i5 = count;
    this->objectType = (char*)"PaintEventArgs";
    }
 
  PaintEventArgs::~PaintEventArgs() {
    }

  Int32 PaintEventArgs::X() {
    return i1;
    }

  Int32 PaintEventArgs::Y() {
    return i2;
    }

  Int32 PaintEventArgs::Width() {
    return i3;
    }

  Int32 PaintEventArgs::Height() {
    return i4;
    }

  Int32 PaintEventArgs::Count() {
    return i5;
    }

  }

