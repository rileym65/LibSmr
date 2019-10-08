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

  Swap::Swap() {
    objectType = (char*)"Swap";
    }
  
  Swap::~Swap() {
    }
  
  void Swap::Exchange(Byte* a, Byte* b) {
    Byte tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    }
  
  void Swap::Exchange(Int8* a, Int8* b) {
    Int8 tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    }
  
  void Swap::Exchange(Int16* a, Int16* b) {
    Int16 tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    }
  
  void Swap::Exchange(UInt16* a, UInt16* b) {
    UInt16 tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    }
  
  void Swap::Exchange(Int32* a, Int32* b) {
    Int32 tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    }
  
  void Swap::Exchange(UInt32* a, UInt32* b) {
    UInt32 tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    }
  
  void Swap::Exchange(Int64* a, Int64* b) {
    Int64 tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    }
  
  void Swap::Exchange(UInt64* a, UInt64* b) {
    UInt64 tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    }
  
  void Swap::Exchange(Single* a, Single* b) {
    Single tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    }
  
  void Swap::Exchange(Double* a, Double* b) {
    Double tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    }

  }

