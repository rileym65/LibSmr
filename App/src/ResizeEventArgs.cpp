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
  
  ResizeEventArgs::ResizeEventArgs(int w, int h) {
    i1 = w;
    i2 = h;
    this->objectType = (char*)"ResizeEventArgs";
    }

  ResizeEventArgs::~ResizeEventArgs() {
    }

  Int32 ResizeEventArgs::Width() {
    return i1;
    }

  Int32 ResizeEventArgs::Height() {
    return i2;
    }

  }

