/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

#include "SmrFramework.h"

using namespace SmrFramework;

Boolean::Boolean(int i) {
  value = (i) ? -1 : 0;
  objectType = (char*)"Boolean";
  }

Boolean::~Boolean() {
  }

char* Boolean::AsCharArray() {
  return (value) ? (char*)"true" : (char*)"false";
  }

