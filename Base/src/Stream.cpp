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

  Stream::Stream(Byte mode) {
    accessMode = mode;
    objectType = (char*)"Stream";
    deleteResource = true;
    }

  Stream::~Stream() {
    }

  void Stream::Close() {
    }

  Boolean Stream::DeleteResource() {
    return deleteResource;
    }

  Boolean Stream::DeleteResource(Boolean b) {
    deleteResource = b;
    return deleteResource;
    }

  bool Stream::EndOfStream() {
    return true;
    }

  void Stream::Flush() {
    }

  UInt32 Stream::Position() {
    return 0;
    }

  UInt32 Stream::Position(UInt32 pos) {
    return 0;
    }

  UInt32 Stream::Write(Byte b) {
    return 0;
    }

  UInt32 Stream::Write(Word i) {
    return 0;
    }

  UInt32 Stream::Write(Dword d) {
    return 0;
    }

  UInt32 Stream::Write(Qword q) {
    return 0;
    }

  UInt32 Stream::Write(Byte* b,UInt32 len) {
    return 0;
    }

  Byte Stream::ReadByte() {
    return 0;
    }

  Word Stream::ReadWord() {
    return 0;
    }

  Dword Stream::ReadDword() {
    return 0;
    }

  Qword Stream::ReadQword() {
    return 0;
    }

  UInt32 Stream::ReadByteArray(Byte* b,UInt32 maxlen) {
    return 0;
    }

  UInt32 Stream::Rewind() {
    return 0;
    }
  }

