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

  HttpListener::HttpListener() {
    objectType = (char*)"HttpListener";
    }
  
  HttpListener::~HttpListener() {
    }
  
  Byte* HttpListener::Request(HttpServer* server, UInt32 *resultLength) {
    *resultLength = 0;
    return NULL;
    }

  }

