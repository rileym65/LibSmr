/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

#include <stdlib.h>
#include "string.h"
#include "SmrFramework.h"

namespace SmrFramework {

  ForthProgram::ForthProgram(Forth* f) {
    forth = f;
    program = NULL;
    numTokens = 0;
    pos = 0;
    objectType = (char*)"ForthProgram";
    name = NULL;
    }

  ForthProgram::~ForthProgram() {
    if (numTokens != 0) free(program);
    if (name != NULL) free(name);
    }

  void ForthProgram::Add(UInt32 token) {
    numTokens++;
    if (numTokens == 1) program = (UInt32*)malloc(sizeof(UInt32));
      else program = (UInt32*)realloc(program,sizeof(UInt32) * numTokens);
    program[numTokens-1] = token;
    }

  UInt32 ForthProgram::At(UInt32 p) {
    if (p < 0 || p >= numTokens) return 0xffffffff;
    return program[p];
    }

  bool ForthProgram::AtEnd() {
    if (pos >= numTokens) return true;
    return false;
    }

  UInt32 ForthProgram::Count() {
    return numTokens;
    }

  UInt32 ForthProgram::Exec() {
    return forth->Exec(this);
    }

  char* ForthProgram::GetName() {
    return name;
    }

  UInt32 ForthProgram::GetPos() {
    return pos;
    }

  UInt32 ForthProgram::Next() {
    if (pos >= numTokens) return 0xffffffff;
    return program[pos++];
    }

  void ForthProgram::Reset() {
    pos = 0;
    }

  void ForthProgram::SetName(const char* n) {
    if (name != NULL) free(name);
    name = (char*)malloc(strlen(n) + 1);
    strcpy(name, n);
    }

  void ForthProgram::SetPos(UInt32 p) {
    pos = p;
    if (pos > numTokens) pos = numTokens;
    }

  }

