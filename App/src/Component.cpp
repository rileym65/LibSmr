/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/


#include <stdio.h>
#include "SmrAppFramework.h"

using namespace SmrFramework;

Component::Component() : Object() {
  objectType = (char*)"Component";
  }

Component::~Component() {
  }

int Component::IsContainer() {
  return 0;
  }

int Component::IsRadioButton() {
  return 0;
  }
