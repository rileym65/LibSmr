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

  Utils::Utils() {
    objectType = (char*)"Utils";
    }
  
  Utils::~Utils() {
    }
  
  void Utils::Swap(Byte* a, Byte* b) {
    Byte tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    }
  
  void Utils::Swap(Int8* a, Int8* b) {
    Int8 tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    }
  
  void Utils::Swap(Int16* a, Int16* b) {
    Int16 tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    }
  
  void Utils::Swap(UInt16* a, UInt16* b) {
    UInt16 tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    }
  
  void Utils::Swap(Int32* a, Int32* b) {
    Int32 tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    }
  
  void Utils::Swap(UInt32* a, UInt32* b) {
    UInt32 tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    }
  
  void Utils::Swap(Int64* a, Int64* b) {
    Int64 tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    }
  
  void Utils::Swap(UInt64* a, UInt64* b) {
    UInt64 tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    }
  
  void Utils::Swap(Single* a, Single* b) {
    Single tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    }
  
  void Utils::Swap(Double* a, Double* b) {
    Double tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    }
  
  void Utils::Swap(Byte &a, Byte &b) {
    Byte tmp;
    tmp = a;
    a = b;
    b = tmp;
    }
  
  void Utils::Swap(Int8 &a, Int8 &b) {
    Int8 tmp;
    tmp = a;
    a = b;
    b = tmp;
    }
  
  void Utils::Swap(Int16 &a, Int16 &b) {
    Int16 tmp;
    tmp = a;
    a = b;
    b = tmp;
    }
  
  void Utils::Swap(Int32 &a, Int32 &b) {
    Int32 tmp;
    tmp = a;
    a = b;
    b = tmp;
    }
  
  void Utils::Swap(Int64 &a, Int64 &b) {
    Int64 tmp;
    tmp = a;
    a = b;
    b = tmp;
    }
  
  void Utils::Swap(Single &a, Single &b) {
    Single tmp;
    tmp = a;
    a = b;
    b = tmp;
    }
  
  void Utils::Swap(Double &a, Double &b) {
    Double tmp;
    tmp = a;
    a = b;
    b = tmp;
    }

  Byte Utils::RotateLeft(Byte v, UInt32 count) {
    Byte t;
    while (count > 0) {
      t = (v & 0x80) >> 7;
      v = (v << 1) | t;
      count--;
      }
    return v;
    }

  UInt16 Utils::RotateLeft(UInt16 v, UInt32 count) {
    UInt16 t;
    while (count > 0) {
      t = (v & 0x8000) >> 15;
      v = (v << 1) | t;
      count--;
      }
    return v;
    }

  UInt32 Utils::RotateLeft(UInt32 v, UInt32 count) {
    UInt32 t;
    while (count > 0) {
      t = (v & 0x80000000) >> 31;
      v = (v << 1) | t;
      count--;
      }
    return v;
    }

  UInt64 Utils::RotateLeft(UInt64 v, UInt32 count) {
    UInt64 t;
    while (count > 0) {
      t = (v & 0x8000000000000000) >> 63;
      v = (v << 1) | t;
      count--;
      }
    return v;
    }

  Byte Utils::RotateRight(Byte v, UInt32 count) {
    Byte t;
    while (count > 0) {
      t = (v & 0x1) << 7;
      v = (v >> 1) | t;
      count--;
      }
    return v;
    }

  UInt16 Utils::RotateRight(UInt16 v, UInt32 count) {
    UInt16 t;
    while (count > 0) {
      t = (v & 0x1) << 15;
      v = (v >> 1) | t;
      count--;
      }
    return v;
    }

  UInt32 Utils::RotateRight(UInt32 v, UInt32 count) {
    UInt32 t;
    while (count > 0) {
      t = (v & 0x1) << 31;
      v = (v >> 1) | t;
      count--;
      }
    return v;
    }

  UInt64 Utils::RotateRight(UInt64 v, UInt32 count) {
    UInt64 t;
    while (count > 0) {
      t = (v & 0x1) << 63;
      v = (v >> 1) | t;
      count--;
      }
    return v;
    }

  char* Utils::Strip(char* buffer) {
    while (buffer[strlen(buffer)-1] > 0 && buffer[strlen(buffer)-1] <= ' ')
      buffer[strlen(buffer)-1] = 0;
    return buffer;
    }

  }

