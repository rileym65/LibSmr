/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/


#include "string.h"
#include "SmrFramework.h"

namespace SmrFramework {

  Object::Object() {
    objectType = (char*)"Object";
    }
  
  Object::~Object() {
    }
  
  char* Object::Type() {
    return objectType;
    }
  
  Boolean Object::Equals(Object* b) {
    return (this == b);
    }
  
  char* Object::ToCharArray() {
    char* ret;
    ret = (char*)malloc(strlen(objectType) + 1);
    strcpy(ret, objectType);
    return ret;
    }
  
  char* Object::AsCharArray() {
    return objectType;
    }

  String Object::ToString() {
    return String(objectType);
    }

  }

