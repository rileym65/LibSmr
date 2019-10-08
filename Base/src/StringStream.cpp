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
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "SmrFramework.h"

namespace SmrFramework {

  StringStream::StringStream(String* s) : Stream(ReadOnly) {
    data = s;
    if (data == NULL) throw InvalidOpException(this, "Empty stream");
    objectType = (char*)"StringStream";
    dataPointer = 0;
    dataSize = s->Length();
    }

  StringStream::~StringStream() {
    if (deleteResource) delete(data);
    }

  void StringStream::Close() {
    dataSize = 0;
    dataPointer = 0;
    data = NULL;
    }

  bool StringStream::EndOfStream() {
    if (data == NULL) throw InvalidOpException(this, "Stream not opened");
    return (dataPointer >= dataSize);
    }

  UInt32 StringStream::Position() {
    if (data == NULL) throw InvalidOpException(this, "Stream not opened");
    return dataPointer;
    }

  UInt32 StringStream::Position(UInt32 pos) {
    if (data == NULL) throw InvalidOpException(this, "Stream not opened");
    if (pos >= dataSize) throw InvalidOpException(this, "Position exceeds size");
    dataPointer = pos;
    return pos;
    }

  UInt32 StringStream::Write(Byte b) {
    throw InvalidOpException(this, "Stream is read only");
    }

  UInt32 StringStream::Write(Word i) {
    throw InvalidOpException(this, "Stream is read only");
    }

  UInt32 StringStream::Write(Dword d) {
    throw InvalidOpException(this, "Stream is read only");
    }

  UInt32 StringStream::Write(Qword q) {
    throw InvalidOpException(this, "Stream is read only");
    }

  UInt32 StringStream::Write(Byte* b,UInt32 len) {
    throw InvalidOpException(this, "Stream is read only");
    }

  Byte StringStream::ReadByte() {
    Byte b;
    if (data == NULL) throw InvalidOpException(this, "Stream not opened");
    if (dataPointer >= dataSize) throw InvalidOpException(this, "Attempt read past end");
    b = data->At(dataPointer++);
    return b;
    }

  Word StringStream::ReadWord() {
    Word i;
    if (data == NULL) throw InvalidOpException(this, "Stream not opened");
    if (dataPointer+1 >= dataSize) throw InvalidOpException(this, "Attempt read past end");
    i = data->At(dataPointer) | (data->At(dataPointer+1) << 8);
    dataPointer += 2;
    return i;
    }

  Dword StringStream::ReadDword() {
    Dword d;
    if (data == NULL) throw InvalidOpException(this, "Stream not opened");
    if (dataPointer+3 >= dataSize) throw InvalidOpException(this, "Attempt read past end");
    d = data->At(dataPointer) | (data->At(dataPointer+1) << 8) |
       (data->At(dataPointer+2) << 16) | (data->At(dataPointer+3) << 24);
    dataPointer += 4;
    return d;
    }

  Qword StringStream::ReadQword() {
    Qword q,qtmp;
    if (data == NULL) throw InvalidOpException(this, "Stream not opened");
    if (dataPointer+7 >= dataSize) throw InvalidOpException(this, "Attempt read past end");
    q = data->At(dataPointer) | (data->At(dataPointer+1) << 8) |
        (data->At(dataPointer+2) << 16) | (data->At(dataPointer+3) << 24);
    qtmp = (data->At(dataPointer+4)) | (data->At(dataPointer+5) << 8) |
           (data->At(dataPointer+6) << 16) | (data->At(dataPointer+7) << 24);
    dataPointer += 8;
    q |= (qtmp << 32);
    return q;
    }

  UInt32 StringStream::ReadByteArray(Byte* b,UInt32 maxlen) {
    if (data == NULL) throw InvalidOpException(this, "Stream not opened");
    if (dataPointer+maxlen-1 >= dataSize) throw InvalidOpException(this, "Attempt read past end");
    memcpy(b, data+dataPointer, maxlen);
    dataPointer += maxlen;
    return maxlen;
    }

  UInt32 StringStream::Rewind() {
    return Position(0);
    }
  }

