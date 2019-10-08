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

  Random::Random() {
    UInt32 tim;
    UInt32 seed;
    UInt32 taps;
    tim = time(NULL);
    objectType = (char*)"Random";
    seed = tim;
    taps = (1 << 16) | (1 << 23) | (1 << 3) | (1 << 0);
    clocking = Lfsr(seed, taps);
    taps = (1 << 24) | (1 << 21) | (1 << 2) | (1 << 0);
    seed = -seed;
    rng = Lfsr(seed, taps);

    }

  Random::Random(UInt32 s) {
    UInt32 seed;
    UInt32 taps;
    objectType = (char*)"Random";
    seed = s;
    taps = (1 << 16) | (1 << 23) | (1 << 3) | (1 << 0);
    clocking = Lfsr(seed, taps);
    taps = (1 << 24) | (1 << 21) | (1 << 2) | (1 << 0);
    seed = -s;
    rng = Lfsr(seed, taps);
    }

  Random::~Random() {
    }

  UInt32 Random::bitsize(UInt32 n) {
    UInt32 bits;
    UInt32 max;
    bits = 1;
    max = 2;
    while (n > max) {
      bits++;
      max <<= 1;
      }
    return bits;
    }

  UInt32 Random::Next(UInt32 range) {
    UInt32 ret;
    UInt32 seed;
    seed = rng.Next(bitsize(range));
    ret = seed % range;
    rng.Shift(clocking.Next(5));
    return ret;
    }

  }
