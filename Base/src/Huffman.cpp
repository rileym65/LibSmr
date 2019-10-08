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

  Huffman::Huffman(Int32 max) : Object() {
    int i;
    objectType = (char*)"Huffman";
    numEntries = 0;
    maxEntries = max;
    if (max > 0) {
      codes  = (Dword*)malloc(sizeof(Dword) * max);
      values = (Dword*)malloc(sizeof(Dword) * max);
      bits   = (Byte*)malloc(sizeof(Byte) * max);
      }
    else {
      codes = NULL;
      values = NULL;
      bits = NULL;
      }
    for (i=0; i<32; i++) starts[i] = -1;
    }

  Huffman::~Huffman() {
    if (codes != NULL) free(codes);
    if (values != NULL) free(values);
    if (bits != NULL) free(bits);
    }

  bool Huffman::Add(Dword code, Dword value, Byte size) {
    int i;
    int pos;
    if (maxEntries > 0) {
      if (numEntries >= maxEntries) return false;
      }
    numEntries++;
    if (maxEntries == 0) {
      if (numEntries == 1) {
        codes = (Dword*)malloc(sizeof(Dword));
        values = (Dword*)malloc(sizeof(Dword));
        bits = (Byte*)malloc(sizeof(Byte));
        }
      else {
        codes = (Dword*)realloc(codes,sizeof(Dword) * numEntries);
        values = (Dword*)realloc(values,sizeof(Dword) * numEntries);
        bits = (Byte*)realloc(bits,sizeof(Byte) * numEntries);
        }
      }
    pos = -1;
    i = 0;
    if (starts[size] >= 0) i = starts[size];
    while (i < numEntries && pos < 0) {
      if (size < bits[i]) pos = i;
      else if (size == bits[i] && code < codes[i]) pos = i;
      else i++;
      }
    if (pos < 0) pos = numEntries - 1;
    for (i=numEntries-1; i>pos; i--) {
      codes[i] = codes[i-1];
      values[i] = values[i-1];
      bits[i] = bits[i-1];
      }
    codes[pos] = code;
    values[pos] = value;
    bits[pos] = size;
    if (starts[size] < 0 || pos < starts[size])
      starts[size] = pos;
    for (i=size+1; i<32; i++)
      if (starts[i] >= 0) starts[i]++;
    return true;
    }


  void Huffman::Clear() {
    int i;
    for (i=0; i<32; i++) starts[i] = -1;
    if (maxEntries > 0) {
      numEntries = 0;
      return;
      }
    if (codes != NULL) free(codes);
    if (values != NULL) free(values);
    if (bits != NULL) free(bits);
    codes = NULL;
    values = NULL;
    bits = NULL;
    }

  Dword Huffman::Lookup(Dword code, Byte size) {
    int i;
    int start;
    int end;
    int mid;
    if (starts[size] < 0) return 0xffffffff;
    start = starts[size];
    i = size+1;
    while (i < 32 && starts[i] < 0) i++;
    end = (i < 32) ? starts[i]-1 : numEntries-1;
    mid = start + ((end - start) >> 1);
    while (mid != end && mid != start) {
      if (codes[mid] == code) return values[mid];
      if (codes[mid] < code) {
        start = mid;
        mid = start + ((end - start) >> 1);
        }
      else {
        end = mid;
        mid = start + ((end - start) >> 1);
        }
      }
    if (codes[start] == code) return values[start];
    if (codes[end] == code) return values[end];
    return 0xffffffff;
    }

  Dword Huffman::Next(BitStream* stream) {
    Dword code;
    Dword i;
    int  flag;
    int  len;
    flag = 0;
    code = 0;
    len  = 0;
    while (flag == 0) {
      len++;
      code = (code << 1) | stream->Read(1);
      i = Lookup(code, len);
      if (i != 0xffffffff) return i;
      if (len > 18) {
        throw InvalidOpException(this, "Failed to find huffman code");
        }
      }
    return -1;

    }

  }

