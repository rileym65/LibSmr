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
#include <string.h>
#include "SmrFramework.h"

namespace SmrFramework {

  Lexer::Lexer() {
    source = NULL;
    objectType = (char*)"Lexer";
    init();
    }
  Lexer::Lexer(StreamReader* s) {
    source = s;
    objectType = (char*)"Lexer";
    init();
    }
  
  Lexer::~Lexer() {
    UInt32 i;
    for (i=0; i<regExes->Count(); i++) delete(regExes->At(i));
    delete(regExes);
    delete(regExValues);
    delete(regExCallbacks);
    }
  
  void Lexer::init() {
    regExes = new List<RegEx*>();
    regExValues = new List<UInt32>();
    regExCallbacks = new List<Callback*>();
    holdingBufferPos = 0;
    }

  Boolean Lexer::addByteToToken() {
    if (source == NULL) throw InvalidOpException(this, "No source");
    if (holdingBufferPos > 0) {
      token[tokenPos++] = holdingBuffer[0];
      token[tokenPos] = 0;
      holdingBufferPos--;
      memmove(holdingBuffer, holdingBuffer+1, holdingBufferPos);
      holdingBuffer[holdingBufferPos] = 0;
      return true;
      }
    if (source->EndOfStream()) {
      return false;
      }
    token[tokenPos++] = source->ReadByte();
    token[tokenPos] = 0;
    return true;
    }

  void Lexer::moveToHoldingBuffer(UInt32 pos, UInt32 count) {
    memmove(holdingBuffer+count, holdingBuffer, holdingBufferPos);
    memmove(holdingBuffer, token + pos, count);
    holdingBufferPos += count;
    holdingBuffer[holdingBufferPos] = 0;
    token[pos] = 0;
    }

  UInt32 Lexer::attemptMatch() {
    UInt32  i;
    UInt32  index;
    UInt32  foundCount;
    UInt32  matchCount;
    UInt32  matchIndex;
    Boolean match;
    match = true;
    index = 0xffffffff;
    foundCount = 0;
    while (match) {
      matchCount = 0;
      match = false;
      if (!addByteToToken()) {
        if (foundCount != 0) {
          value = String((char*)token);
          return index;
          }
        return 0xffffffff;
        }
      matchIndex = 0xffffffff;
      for (i=0; i<regExes->Count(); i++) {
        if (regExes->At(i)->Match((char*)token))  {
          if (matchIndex == 0xffffffff) matchIndex = i;
          match = true;
          matchCount++;
          }
        }
      if (match) {
        foundCount = matchCount;
        index = matchIndex;
        }
      }
    if (foundCount == 0) return 0xffffffff;
    tokenPos--;
    moveToHoldingBuffer(tokenPos, 1);
    value = String((char*)token);
    return index;
    }

  void Lexer::AddPattern(const char* s, UInt32 v, Callback* cb) {
    regExes->Add(new RegEx(s));
    regExValues->Add(v);
    regExCallbacks->Add(cb);
    }

  Boolean Lexer::AtEnd() {
    return source->EndOfStream();
    }

  UInt32 Lexer::Next() {
    Boolean returnFlag;
    tokenPos = 0;
    result = 0;
    while (1) {
      tokenPos = 0;
      result = attemptMatch();
      if (result == 0xffffffff) throw InvalidOpException(this, "No token found");
      returnFlag = true;
      if (regExCallbacks->At(result) != NULL) {
        returnFlag = regExCallbacks->At(result)->Call(this);
        }
      if (returnFlag && (regExValues->At(result) != 0)) return regExValues->At(result);
      }
    }

  void Lexer::Reset() {
    holdingBufferPos = 0;
    }

  void Lexer::Source(StreamReader* s) {
    source = s;
    }

  String Lexer::Value() {
    return value;
    }


  }

