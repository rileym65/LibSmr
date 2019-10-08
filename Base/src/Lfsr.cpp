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

namespace SmrFramework {

  Lfsr::Lfsr() {
    objectType = (char*)"Lfsr";
    this->seed = 1;
    this->taps = 1;
    }

  Lfsr::Lfsr(UInt32 s, UInt32 t) {
    objectType = (char*)"Lfsr";
    this->seed = s;
    this->taps = t;
    }

  Lfsr::~Lfsr() {
    }

  void Lfsr::Shift(int count) {
    UInt32 bits;
    int    i;
    for (i=0; i<count; i++) {
      bits = seed & taps;
      bits = (bits & 0xffff) ^ ((bits >> 16) & 0xffff);
      bits = (bits & 0x00ff) ^ ((bits >>  8) & 0x00ff);
      bits = (bits & 0x000f) ^ ((bits >>  4) & 0x000f);
      bits = (bits & 0x0003) ^ ((bits >>  2) & 0x0003);
      bits = (bits & 0x0001) ^ ((bits >>  1) & 0x0001);
      bits ^= 1;
      if (seed == 0) bits = 1;
      seed >>= 1;
      seed &= 0x7fffffff;
      if (bits != 0) seed |= 0x80000000;
      }
    }

  UInt32 Lfsr::Next(int bits) {
    int    i;
    UInt32 ret;
    ret = 0;
    for (i=0; i<bits; i++) {
      ret <<= 1;
      ret |= (seed & 0x00000001);
      Shift(1);
      }
    return ret;
    }

  Byte Lfsr::NextByte() {
    UInt32 ret;
    ret = Next(8);
    return (ret & 0x000000ff);
    }

  Word Lfsr::NextWord() {
    UInt32 ret;
    ret = Next(16);
    return (ret & 0x0000ffff);
    }

  Dword Lfsr::NextDword() {
    UInt32 ret;
    ret = Next(32);
    return (ret & 0xffffffff);
    }

  Qword Lfsr::NextQword() {
    UInt64 ret;
    UInt32 tmp;
    ret = Next(32);
    ret <<= 32;
    tmp = Next(32);
    ret |= tmp;
    return ret;
    }

  }
