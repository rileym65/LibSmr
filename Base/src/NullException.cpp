/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/


#include <stdlib.h>
#include <string.h>
#include "SmrFramework.h"

namespace SmrFramework {

  NullException::NullException(const char* msg) : Exception(msg) {
    }

  NullException::NullException(Object* obj, const char* msg) : Exception(obj, msg) {
    }

  NullException::~NullException() throw () {
    }

  }

