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

  Word _flate_bases[] = {   3,  4,  5,  6,  7,  8,  9, 10, 11, 13,
                           15, 17, 19, 23, 27, 31, 35, 43, 51, 59,
                           67, 83, 99,115,131,163,195,227,258 };
  Word _flate_extra[] = {   0,  0,  0,  0,  0,  0,  0,  0,  1,  1,
                            1,  1,  2,  2,  2,  2,  3,  3,  3,  3,
                            4,  4,  4,  4,  5,  5,  5,  5,  0 };
  Word _flate_dbases[] = {   1,    2,    3,    4,    5,    7,    9,   13,   17,   25,
                            33,   49,   65,   97,  129,  193,  257,  385,  513,  769,
                          1025, 1537, 2049, 3073, 4097, 6145, 8193,12289,16385,24577 };
  Word _flate_dextra[] = {   0,    0,    0,    0,    1,    1,    2,    2,    3,    3,
                             4,    4,    5,    5,    6,    6,    7,    7,    8,    8,
                             9,    9,   10,   10,   11,   11,   12,   12,   13,   13 };
  
  Flate::Flate(BitStream* s,char m) {
    objectType = (char*)"Flate";
    stream = s;
    huffmanMode = ' ';
    lzWindow = (Byte*)malloc(32768);
    buffer = (Byte*)malloc(65536);
    blength = 0;
    bpos = 1;
    wPos = 0;
    mask = 256;
    mode = m;
    readMode = 'B';
    final = 0;
    lengths = new Huffman(288);
    distances = new Huffman(33);
    if (mode != 'W') {
      compression = stream->Read(4);
      window = stream->Read(4);
      check  = stream->Read(5);
      preset = stream->Read(1);
      flevel = stream->Read(2);
      }
    }
  
  Flate::~Flate() {
    if (lzWindow != NULL) free(lzWindow);
    if (buffer != NULL) free(buffer);
    delete(lengths);
    delete(distances);
    }

  void  Flate::_flate_build_huffman(Int32 count, Dword* lengths, Huffman* table) {
    int  i;
    Dword counts[33];
    Dword nextCode[33];
    Dword code;
    table->Clear();
    for (i=0; i<33; i++) counts[i] = 0;
    for (i=0; i<count; i++) counts[lengths[i]]++;
    counts[0] = 0;
    code = 0;
    for (i=1; i<=32; i++) {
      code = (code + counts[i-1]) << 1;
      nextCode[i] = code;
      }
    for (i=0; i<count; i++) {
      if (lengths[i] != 0) {
        table->Add(nextCode[lengths[i]], i, lengths[i]);
        nextCode[lengths[i]]++;
        } // else codes[i] = -1;
      }
    }

  void Flate::_fixed_flate_huffman() {
    int i;
    Dword sizes[288];
    for (i=0; i<144; i++) sizes[i] = 8;
    for (i=144; i<256; i++) sizes[i] = 9;
    for (i=256; i<280; i++) sizes[i] = 7;
    for (i=280; i<288; i++) sizes[i] = 8;
    _flate_build_huffman(288,sizes,lengths);
    for (i=0; i<32; i++) sizes[i] = 5;
    _flate_build_huffman(32,sizes,distances);
    huffmanMode = 'F';
    }

  Word Flate::_flate_read_huffman(UInt32 max, Huffman* table) {
    return table->Next(stream);
/*
    Dword code;
    UInt32 i;
    int  flag;
    int  len;
    flag = 0;
    code = 0;
    len  = 0;
    while (flag == 0) {
      len++;
      code = (code << 1) | stream->Read(1);
      i = table->Lookup(code, len);
      if (i != 0xffffffff) return i;
      if (len > 18) {
        throw InvalidOpException(this, "Failed to find huffman code");
        }
      }
    return -1;
*/
    }

  void Flate::_flate_read_lengths(int count, Dword* lengths, Huffman* table) {
    UInt32  i;
    int  index;
    Dword code;
    Dword num;
    index = 0;
    while (index < count) {
      code = _flate_read_huffman(19,table);
      if (code < 16) {
        lengths[index++] = code;
        }
      else if (code == 16) {
        num = 3 + stream->Read(2);
        for (i=0; i<num; i++) {
          lengths[index] = lengths[index - 1];
          index++;
          }
        }
      else if (code == 17) {
        num = 3 + stream->Read(3);
        for (i=0; i<num; i++) {
          lengths[index++] = 0;
          }
        }
      else if (code == 18) {
        num = 11 + stream->Read(7);
        for (i=0; i<num; i++) {
          lengths[index++] = 0;
          }
        }
      }
    }

  void Flate::_dynamic_flate_huffman() {
    Dword sizes[288];
    Dword lens[19];
    Dword literals,distances,lengths;
    literals  = stream->Read(5) + 257;
    distances = stream->Read(5) + 1;
    lengths   = stream->Read(4) + 4;
    lens[16] = (lengths > 0) ? stream->Read(3) : 0;
    lens[17] = (lengths > 1) ? stream->Read(3) : 0;
    lens[18] = (lengths > 2) ? stream->Read(3) : 0;
    lens[0]  = (lengths > 3) ? stream->Read(3) : 0;
    lens[8]  = (lengths > 4) ? stream->Read(3) : 0;
    lens[7]  = (lengths > 5) ? stream->Read(3) : 0;
    lens[9]  = (lengths > 6) ? stream->Read(3) : 0;
    lens[6]  = (lengths > 7) ? stream->Read(3) : 0;
    lens[10] = (lengths > 8) ? stream->Read(3) : 0;
    lens[5]  = (lengths > 9) ? stream->Read(3) : 0;
    lens[11] = (lengths > 10) ? stream->Read(3) : 0;
    lens[4]  = (lengths > 11) ? stream->Read(3) : 0;
    lens[12] = (lengths > 12) ? stream->Read(3) : 0;
    lens[3]  = (lengths > 13) ? stream->Read(3) : 0;
    lens[13] = (lengths > 14) ? stream->Read(3) : 0;
    lens[2]  = (lengths > 15) ? stream->Read(3) : 0;
    lens[14] = (lengths > 16) ? stream->Read(3) : 0;
    lens[1]  = (lengths > 17) ? stream->Read(3) : 0;
    lens[15] = (lengths > 18) ? stream->Read(3) : 0;
    _flate_build_huffman(19,lens,this->distances);
    _flate_read_lengths(literals,sizes,this->distances);
    _flate_build_huffman(literals,sizes,this->lengths);
    _flate_read_lengths(distances,sizes,this->distances);
    _flate_build_huffman(distances,sizes,this->distances);
    huffmanMode = 'D';
    }

  Int32 Flate::_read_flate() {
    Dword cmptyp;
    Dword code;
    Dword dst;
    Byte adler[4];
    while (1) {
      if (readMode == 'B') {
        if (final == 1) return -1;
        final = stream->Read(1);
        cmptyp = stream->Read(2);
        readMode = 'L';
        if (cmptyp == 0) {
          mask = 256;
          stream->Flush();
          adler[0] = stream->Read(8);
          adler[1] = stream->Read(8);
          length = (adler[1] << 8) + adler[0];
          adler[0] = stream->Read(8);
          adler[1] = stream->Read(8);
          readMode = 'U';
          }
        else if (cmptyp == 2) {
          _dynamic_flate_huffman();
          }
        else {
          if (huffmanMode != 'F') _fixed_flate_huffman();
          }
        }
      if (readMode == 'U') {
        code = stream->Read(8);
        length--;
        if (length == 0) {
          readMode = 'B';
          }
        return code;
        }
      if (readMode == 'C') {
        code = lzWindow[cPos];
        cPos = (cPos + 1) & 0x7fff;
        lzWindow[wPos] = code;
        wPos = (wPos + 1) & 0x7fff;
        length--;
        if (length == 0) {
          readMode = 'L';
          }
        return code;
        }
      code = _flate_read_huffman(288,lengths);
      if (code < 256) {
        lzWindow[wPos] = code;
        wPos = (wPos + 1) & 0x7fff;
        return code;
        }
      if (code == 256) {
        readMode = 'B';
        if (final == 1) {
          stream->Read(32);
          }
        }
      if (code > 256) {
        length = _flate_bases[code - 257] + stream->Read(_flate_extra[code-257]);
        dst = _flate_read_huffman(31,distances);
        dst = _flate_dbases[dst] + stream->Read(_flate_dextra[dst]);
        cPos = (32768 + wPos - dst) & 0x7fff;
        readMode = 'C';
        }
      }
    return -1;
    }
  

  Int32 Flate::Read(Byte* buffer, int len) {
    int   code;
    int   count;
    Byte* pBuffer;
    pBuffer = buffer;
    count = 0;
    if (readMode == '*') {
      return 0;
      }
    while (len > 0) {
      code = _read_flate();
      if (code < 0) return count;
      *pBuffer++ = code;
      count++;
      len--;
      }
    return count;
    }
  }

