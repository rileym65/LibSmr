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

  LZW::LZW(Int32 bits, Int32 maxBits) {
    bitSize = bits;
    maxCodes = 1 << maxBits;
    dictCode = NULL;
    dictChar = NULL;
    currentCode = -1;
    currentChar = -1;
    oldCode = -1;
    objectType = (char*)"LZW";
    InitDictionary();
    }
  
  LZW::~LZW() {
    if (dictCode != NULL) free(dictCode);
    if (dictChar != NULL) free(dictChar);
    }

  Int32 LZW::Compress(Int32 code) {
    int entry;
    int output;
    output = -1;
    if (code >= 0) {
      currentChar = code;
      entry = scanDictionary(currentCode, currentChar);
      if (entry >= 0) {
        currentCode = entry;
        } else {
        output = currentCode;
        dictCode[dictPos] = currentCode;
        dictChar[dictPos] = currentChar;
        dictPos++;
        currentCode = currentChar;
        }
      }
    else {
      if (currentCode >= 0) output = currentCode;
      }
    return output;
    }

  Int32 LZW::Decompress(Int32 newCode) {
    int i;
    int out;
    if (oldCode < 0) {
      output[0] = newCode;
      currentChar = newCode;
      oldCode = newCode;
      return 1;
      }
    if (newCode >= dictPos) {
      out = buildString(oldCode);
      for (i=out; i>0; i--) output[i] = output[i-1];
      output[0] = currentChar;
      out++;
      }
    else {
      out = buildString(newCode);
      }
    currentChar = output[out-1];
    if (dictPos < 4096) {
      dictCode[dictPos] = oldCode;
      dictChar[dictPos] = currentChar;
      dictPos++;
      }
    oldCode = newCode;
    return out;
    }

  Int32 LZW::DictPosition() {
    return dictPos;
    }

  void LZW::InitDictionary() {
    int i;
    if (dictCode != NULL) free(dictCode);
    if (dictChar != NULL) free(dictChar);
    dictCode = (Int32*)malloc(sizeof(Int32) * maxCodes);
    dictChar = (Byte*)malloc(sizeof(Byte) * maxCodes);
    dictPos = 1 << bitSize;
    for (i=0; i<dictPos+2; i++) {
      dictCode[i] = -1;
      dictChar[i] = i;
      }
    dictPos += 2;
    currentCode = -1;
    currentChar = -1;
    oldCode = -1;
    }

  Int32* LZW::Output() {
    return output;
    }

  Int32 LZW::buildString(Int32 entry) {
    int pos;
    pos = 0;
    output[pos++] = dictChar[entry];
    while (dictCode[entry] >= 0) {
      entry = dictCode[entry];
      output[pos++] = dictChar[entry];
      }
    return pos;
    }

  Int32 LZW::scanDictionary(Int32 code, Int32 chr) {
    int i;
    for (i=0; i<dictPos; i++)
      if (dictCode[i] == code && dictChar[i] == chr) return i;
    return -1;
    }

  }
