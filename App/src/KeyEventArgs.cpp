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

  KeyEventArgs::KeyEventArgs(int key) {
    i1 = key;
    this->objectType = (char*)"KeyEventArgs";
    }

  KeyEventArgs::~KeyEventArgs() {
    }

  Int32 KeyEventArgs::Key() {
    return i1;
    }

  }

