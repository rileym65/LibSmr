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

  ForthContext::ForthContext() {
    objectType = (char*)"ForthContext";
    }

  ForthContext::~ForthContext() {
    }

  Int32 ForthContext::At(UInt32 pos) {
    return 0;
    }

  void ForthContext::Push(Int32 value) {
    }

  Int32 ForthContext::Pop() {
    return 0;
    }

  void ForthContext::Clear() {
    }

  UInt32 ForthContext::Count() {
    return 0;
    }

  void ForthContext::Output(const char* msg) {
    }

  void ForthContext::Write(UInt32 address, Int32 value) {
    }

  Int32 ForthContext::Read(UInt32 address) {
    return 0;
    }

  void ForthContext::Function(UInt32 number) {
    }

  }

