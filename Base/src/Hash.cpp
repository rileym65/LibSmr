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

#include "string.h"
#include "SmrFramework.h"

namespace SmrFramework {

UInt32 _md5_s[] = {
                     7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
                     5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
                     4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
                     6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
                  };

UInt32 _md5_k[] = {
                    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
                    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
                    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
                    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
                    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
                    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
                    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
                    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
                    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
                    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
                    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
                    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
                    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
                    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
                    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
                    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
                  };

UInt32 _sha256_k[] = {
                    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
                    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
                    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
                    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
                    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
                    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
                    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
                    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
                    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
                    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
                    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
                    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
                    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
                    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
                    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
                    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
                  };

  Hash::Hash() {
    objectType = (char*)"Hash";
    }
  
  Hash::~Hash() {
    }
  
  String Hash::Md5(const char* msg,UInt32 size) {
    UInt32 i,j;
    UInt32 a0,b0,c0,d0;
    UInt32 a,b,c,d,f,g;
    UInt64 ml;
    UInt32 w[16];
    Boolean oneWritten;
    Int32 sz;
    Byte   buffer[80];
    ml = size * 8;
    a0 = 0x67452301;
    b0 = 0xefcdab89;
    c0 = 0x98badcfe;
    d0 = 0x10325476;
    sz = size;
    oneWritten = false;
    while (sz >= 0) {
      if (sz >= 64) {
        for (i=0; i<64; i++) buffer[i] = *msg++;
        sz -= 64;
        }
      else if (sz <= 55) {
        for (i=0; i<(UInt32)sz; i++) buffer[i] = *msg++;
        if (!oneWritten) buffer[sz++] = 0x80;
        while (sz < 56) buffer[sz++] = 0x00;
        buffer[sz++] = ml & 0xff;
        buffer[sz++] = (ml >> 8) & 0xff;
        buffer[sz++] = (ml >> 16) & 0xff;
        buffer[sz++] = (ml >> 24) & 0xff;
        buffer[sz++] = (ml >> 32) & 0xff;
        buffer[sz++] = (ml >> 40) & 0xff;
        buffer[sz++] = (ml >> 48) & 0xff;
        buffer[sz++] = (ml >> 56) & 0xff;
        sz = -1;
        }
      else {
        for (i=0; i<(UInt32)sz; i++) buffer[i] = *msg++;
        buffer[sz++] = 0x80;
        oneWritten = true;
        while (sz < 64) buffer[sz++] = 0x00;
        sz = 0;
        }
      for (i=0; i<16; i++) {
        w[i] = 0;
        for (j=0; j<4; j++) {
          w[i] = (w[i] >> 8) | (buffer[i*4+j] << 24);
          }
        }
      a = a0;
      b = b0;
      c = c0;
      d = d0;
      for (i=0; i<64; i++) {
        if (i>=0 && i<=15) {
          f = (b & c) | ((~b) & d);
          g = i;
          }
        if (i>=16 && i<=31) {
          f = (d & b) | ((~d) & c);
          g = (i * 5 + 1) & 0xf;
          }
        if (i>=32 && i<=47) {
          f = b ^ c ^ d;
          g = (i * 3 + 5) & 0xf;
          }
        if (i>=48 && i<=63) {
          f = c ^ (b | (~d));
          g = (i * 7) & 0xf;
          }
        f = f + a + _md5_k[i] + w[g];
        a = d;
        d = c;
        c = b;
        b = b + Utils::RotateLeft(f, _md5_s[i]);
        }
      a0 += a;
      b0 += b;
      c0 += c;
      d0 += d;
      }
    buffer[0] = a0 & 0xff;
    buffer[1] = (a0 >> 8) & 0xff;
    buffer[2] = (a0 >> 16) & 0xff;
    buffer[3] = (a0 >> 24) & 0xff;
    buffer[4] = b0 & 0xff;
    buffer[5] = (b0 >> 8) & 0xff;
    buffer[6] = (b0 >> 16) & 0xff;
    buffer[7] = (b0 >> 24) & 0xff;
    buffer[8] = c0 & 0xff;
    buffer[9] = (c0 >> 8) & 0xff;
    buffer[10] = (c0 >> 16) & 0xff;
    buffer[11] = (c0 >> 24) & 0xff;
    buffer[12] = d0 & 0xff;
    buffer[13] = (d0 >> 8) & 0xff;
    buffer[14] = (d0 >> 16) & 0xff;
    buffer[15] = (d0 >> 24) & 0xff;
    return String(buffer,16);
    }

  String Hash::Sha1(const char* msg,UInt32 size) {
    UInt32 i,j;
    UInt32 h0,h1,h2,h3,h4;
    UInt32 a,b,c,d,e,f,k,temp;
    UInt32 w[80];
    UInt64 ml;
    Boolean oneWritten;
    Int32 sz;
    Byte   buffer[80];
    ml = size * 8;
    h0 = 0x67452301;
    h1 = 0xefcdab89;
    h2 = 0x98badcfe;
    h3 = 0x10325476;
    h4 = 0xc3d2e1f0;
    sz = size;
    oneWritten = false;
    while (sz >= 0) {
      if (sz >= 64) {
        for (i=0; i<64; i++) buffer[i] = *msg++;
        sz -= 64;
        }
      else if (sz <= 55) {
        for (i=0; i<(UInt32)sz; i++) buffer[i] = *msg++;
        if (!oneWritten) buffer[sz++] = 0x80;
        while (sz < 56) buffer[sz++] = 0x00;
        buffer[sz++] = (ml >> 56) & 0xff;
        buffer[sz++] = (ml >> 48) & 0xff;
        buffer[sz++] = (ml >> 40) & 0xff;
        buffer[sz++] = (ml >> 32) & 0xff;
        buffer[sz++] = (ml >> 24) & 0xff;
        buffer[sz++] = (ml >> 16) & 0xff;
        buffer[sz++] = (ml >> 8) & 0xff;
        buffer[sz++] = ml & 0xff;
        sz = -1;
        }
      else {
        for (i=0; i<(UInt32)sz; i++) buffer[i] = *msg++;
        buffer[sz++] = 0x80;
        oneWritten = true;
        while (sz < 64) buffer[sz++] = 0x00;
        sz = 0;
        }
      for (i=0; i<16; i++) {
        w[i] = 0;
        for (j=0; j<4; j++) {
          w[i] = (w[i] << 8) | buffer[i*4+j];
          }
        }
      for (i=16; i<80; i++)
        w[i] = Utils::RotateLeft((w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16]), 1);
      a = h0;
      b = h1;
      c = h2;
      d = h3;
      e = h4;
      for (i=0; i<80; i++) {
        if (i>=0 && i<=19) {
          f = (b & c) | ((~b) & d);
          k = 0x5a827999;
          }
        else if (i>=20 && i<=39) {
          f = b ^ c ^ d;
          k = 0x6ed9eba1;
          }
        else if (i>=40 && i<=59) {
          f = (b & c) | (b & d) | (c & d);
          k = 0x8f1bbcdc;
          }
        else if (i>=60 && i<=79) {
          f = b ^ c ^ d;
          k = 0xca62c1d6;
          }
        temp = Utils::RotateLeft(a, 5) + f + e + k + w[i];
        e = d;
        d = c;
        c = Utils::RotateLeft(b, 30);
        b = a;
        a = temp;
        }
      h0 += a;
      h1 += b;
      h2 += c;
      h3 += d;
      h4 += e;
      }
    buffer[0] = (h0 >> 24) & 0xff;
    buffer[1] = (h0 >> 16) & 0xff;
    buffer[2] = (h0 >> 8) & 0xff;
    buffer[3] = h0 & 0xff;
    buffer[4] = (h1 >> 24) & 0xff;
    buffer[5] = (h1 >> 16) & 0xff;
    buffer[6] = (h1 >> 8) & 0xff;
    buffer[7] = h1 & 0xff;
    buffer[8] = (h2 >> 24) & 0xff;
    buffer[9] = (h2 >> 16) & 0xff;
    buffer[10] = (h2 >> 8) & 0xff;
    buffer[11] = h2 & 0xff;
    buffer[12] = (h3 >> 24) & 0xff;
    buffer[13] = (h3 >> 16) & 0xff;
    buffer[14] = (h3 >> 8) & 0xff;
    buffer[15] = h3 & 0xff;
    buffer[16] = (h4 >> 24) & 0xff;
    buffer[17] = (h4 >> 16) & 0xff;
    buffer[18] = (h4 >> 8) & 0xff;
    buffer[19] = h4 & 0xff;
    buffer[20] = 0;
    return String(buffer,20);
    }

  String Hash::Sha256(const Byte* msg,UInt32 size) {
    UInt32 i;
    Byte end[128];
    Byte block[64];
    UInt32 h0, h1, h2, h3, h4, h5, h6, h7;
    UInt32 a,b,c,d,e,f,g,h;
    UInt32 temp1, temp2, ch, maj;
    Int32 sz;
    UInt32 s0, s1;
    UInt32 w[64];
    UInt64 l;
    UInt32 pos;
    Byte buffer[64];
    pos = 0;
    end[pos++] = 0x80;
    while (((size + pos + 8) % 64) != 0) end[pos++] = 0x00;
    l = size;
    end[pos++] = (((l * 8) >> 56) & 0xff);
    end[pos++] = (((l * 8) >> 48) & 0xff);
    end[pos++] = (((l * 8) >> 40) & 0xff);
    end[pos++] = (((l * 8) >> 32) & 0xff);
    end[pos++] = (((l * 8) >> 24) & 0xff);
    end[pos++] = (((l * 8) >> 16) & 0xff);
    end[pos++] = (((l * 8) >> 8) & 0xff);
    end[pos++] = ((l * 8) & 0xff);
    h0 = 0x6a09e667;
    h1 = 0xbb67ae85;
    h2 = 0x3c6ef372;
    h3 = 0xa54ff53a;
    h4 = 0x510e527f;
    h5 = 0x9b05688c;
    h6 = 0x1f83d9ab;
    h7 = 0x5be0cd19;
    sz = size + pos;
    pos = 0;
    while (sz > 0) {
      for (i=0; i<64; i++) {
        if (size > 0) {
          block[i] = *msg++;
          size--;
          }
        else {
          block[i] = end[pos++];
          }
        }
      for (i=0; i<16; i++) {
        w[i] = (block[i*4+0] << 24) | (block[i*4+1] << 16) | (block[i*4+2] << 8) | block[i*4+3];
//        w[i] = (block[i*4+3] << 24) | (block[i*4+2] << 16) | (block[i*4+1] << 8) | block[i*4+0];
        }

      for (i=16; i<64; i++) {
        s0 = Utils::RotateRight(w[i-15], 7) ^ Utils::RotateRight(w[i-15], 18) ^ (w[i-15] >> 3);
        s1 = Utils::RotateRight(w[i-2], 17) ^ Utils::RotateRight(w[i-2], 19) ^ (w[i-2] >> 10);
        w[i] = w[i-16] + s0 + w[i-7] + s1;
        }

      a = h0;
      b = h1;
      c = h2;
      d = h3;
      e = h4;
      f = h5;
      g = h6;
      h = h7;

      for (i=0; i<64; i++) {
        s1 = Utils::RotateRight(e, 6) ^ Utils::RotateRight(e, 11) ^ Utils::RotateRight(e, 25);
        ch = (e & f) ^ ((~e) & g);
        temp1 = h + s1 + ch + _sha256_k[i] + w[i];
        s0 = Utils::RotateRight(a, 2) ^ Utils::RotateRight(a, 13) ^ Utils::RotateRight(a, 22);
        maj = (a & b) ^ (a & c) ^ (b & c);
        temp2 = s0 + maj;
        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
        }
    
      h0 = h0 + a;
      h1 = h1 + b;
      h2 = h2 + c;
      h3 = h3 + d;
      h4 = h4 + e;
      h5 = h5 + f;
      h6 = h6 + g;
      h7 = h7 + h;

      sz -= 64;
      }
    buffer[0] = (h0 >> 24) & 0xff;
    buffer[1] = (h0 >> 16) & 0xff;
    buffer[2] = (h0 >> 8) & 0xff;
    buffer[3] = h0 & 0xff;
    buffer[4] = (h1 >> 24) & 0xff;
    buffer[5] = (h1 >> 16) & 0xff;
    buffer[6] = (h1 >> 8) & 0xff;
    buffer[7] = h1 & 0xff;
    buffer[8] = (h2 >> 24) & 0xff;
    buffer[9] = (h2 >> 16) & 0xff;
    buffer[10] = (h2 >> 8) & 0xff;
    buffer[11] = h2 & 0xff;
    buffer[12] = (h3 >> 24) & 0xff;
    buffer[13] = (h3 >> 16) & 0xff;
    buffer[14] = (h3 >> 8) & 0xff;
    buffer[15] = h3 & 0xff;
    buffer[16] = (h4 >> 24) & 0xff;
    buffer[17] = (h4 >> 16) & 0xff;
    buffer[18] = (h4 >> 8) & 0xff;
    buffer[19] = h4 & 0xff;
    buffer[20] = (h5 >> 24) & 0xff;
    buffer[21] = (h5 >> 16) & 0xff;
    buffer[22] = (h5 >> 8) & 0xff;
    buffer[23] = h5 & 0xff;
    buffer[24] = (h6 >> 24) & 0xff;
    buffer[25] = (h6 >> 16) & 0xff;
    buffer[26] = (h6 >> 8) & 0xff;
    buffer[27] = h6 & 0xff;
    buffer[28] = (h7 >> 24) & 0xff;
    buffer[29] = (h7 >> 16) & 0xff;
    buffer[30] = (h7 >> 8) & 0xff;
    buffer[31] = h7 & 0xff;
    buffer[32] = 0;
    return String(buffer,32);
    }

  }

