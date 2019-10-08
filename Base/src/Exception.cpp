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

  Exception::Exception(const char* msg) {
    if (msg == NULL) {
      strcpy(message, "NULL");
      }
    else if (strlen(msg) < 255) {
      strcpy(message, msg);
      }
    else {
      strncpy(message, msg, 255);
      message[255] = 0;
      }
    sender = NULL;
    }

  Exception::Exception(Object* obj, const char* msg) {
    if (msg == NULL) {
      strcpy(message, "NULL");
      }
    else if (strlen(msg) < 255) {
      strcpy(message, msg);
      }
    else {
      strncpy(message, msg, 255);
      message[255] = 0;
      }
    sender = obj;
    }

  Exception::~Exception() throw () {
    }

  char* Exception::Message() {
    return message;
    }

  Object* Exception::Sender() {
    return sender;
    }
  }

