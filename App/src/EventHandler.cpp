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

  EventHandler::EventHandler(void (*cb)(void*,void*,EventArgs),void* obj) {
    object = obj;
    callback_obj = cb;
    this->objectType = (char*)"EventHandler";
    }
  
  EventHandler::~EventHandler() {
    }
  
  void EventHandler::Call(void* sender,EventArgs args) {
    callback_obj(object,sender,args);
    }

  }
