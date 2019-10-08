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
  
  CustomEventArgs::CustomEventArgs(Byte* data, Byte len) {
    Byte i;
    this->objectType = (char*)"CustomEventArgs";
    for (i=0; i<len; i++) this->data[i] = data[i];
    }

  CustomEventArgs::~CustomEventArgs() {
    }

  Byte* CustomEventArgs::Data() {
    return data;
    }

  }

