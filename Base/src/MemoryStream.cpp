/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "SmrFramework.h"

namespace SmrFramework {

  MemoryStream::MemoryStream(UInt32 size) : Stream(ReadWrite) {
    UInt32 i;
    data = (Byte*)malloc(size);
    if (data == NULL) throw InvalidOpException(this, "Could not allocate memory");
    dataSize = size;
    for (i=0; i<size; i++) data[i] = 0;
    objectType = (char*)"MemoryStream";
    dataPointer = 0;
    }

  MemoryStream::MemoryStream(Byte* buffer, UInt32 size) : Stream(ReadOnly) {
    data = buffer;
    dataSize = size;
    objectType = (char*)"MemoryStream";
    dataPointer = 0;
    deleteResource = false;
    };

  MemoryStream::MemoryStream(const char* path) : Stream(ReadOnly) {
    File*     file;
    FileInfo* info;
    info = new FileInfo(path);
    data = (Byte*)malloc(info->Size());
    dataSize = info->Size();
    file = new File(path, ReadOnly);
    if (file->ReadBytes(data, dataSize) < dataSize) {
      delete(file);
      delete(info);
      free(data);
      throw InvalidOpException(this, "Could not read file");
      }
    dataPointer = 0;
    delete(info);
    delete(file);
    }

  MemoryStream::MemoryStream(const char* path, Byte mode) : Stream(mode) {
    File*     file;
    FileInfo* info;
    info = new FileInfo(path);
    data = (Byte*)malloc(info->Size());
    dataSize = info->Size();
    file = new File(path, ReadOnly);
    if (file->ReadBytes(data, dataSize) < dataSize) {
      delete(file);
      delete(info);
      free(data);
      throw InvalidOpException(this, "Could not read file");
      }
    dataPointer = 0;
    delete(info);
    delete(file);
    }

  MemoryStream::~MemoryStream() {
    if (data != NULL && deleteResource) free(data);
    }

  void MemoryStream::Close() {
    if (data == NULL) throw InvalidOpException(this, "Already closed");
    free(data);
    dataSize = 0;
    dataPointer = 0;
    data = NULL;
    }

  bool MemoryStream::EndOfStream() {
    if (data == NULL) throw InvalidOpException(this, "Memory not opened");
    return (dataPointer >= dataSize);
    }

  void MemoryStream::Expand(UInt32 size) {
    if (data == NULL) throw InvalidOpException(this, "Memory not opened");
    data = (Byte*)realloc(data,dataSize + size);
    if (data == NULL) throw InvalidOpException(this, "Failed to expand memory");
    }

  void MemoryStream::Flush() {
    if (data == NULL) throw InvalidOpException(this, "Memory not opened");
    }

  UInt32 MemoryStream::Position() {
    if (data == NULL) throw InvalidOpException(this, "Memory not opened");
    return dataPointer;
    }

  UInt32 MemoryStream::Position(UInt32 pos) {
    if (data == NULL) throw InvalidOpException(this, "Memory not opened");
    if (pos >= dataSize) throw InvalidOpException(this, "Position exceeds size");
    dataPointer = pos;
    return pos;
    }

  void MemoryStream::Save(const char* filename) {
    File* file;
    if (data == NULL) throw InvalidOpException(this, "Memory not opened");
    file = new File(filename, WriteOnly);
    file->Write(data, dataSize);
    file->Close();
    delete(file);
    }

  UInt32 MemoryStream::Write(Byte b) {
    if (data == NULL) throw InvalidOpException(this, "Memory not opened");
    if (accessMode == ReadOnly) throw InvalidOpException(this, "Memory is read only");
    if (dataPointer >= dataSize) throw InvalidOpException(this, "Attempt write past end");
    data[dataPointer++] = b;
    return 1;
    }

  UInt32 MemoryStream::Write(Word i) {
    if (data == NULL) throw InvalidOpException(this, "Memory not opened");
    if (accessMode == ReadOnly) throw InvalidOpException(this, "Memory is read only");
    if (dataPointer+1 >= dataSize) throw InvalidOpException(this, "Attempt write past end");
    data[dataPointer++] = i & 0xff;
    data[dataPointer++] = (i >> 8) & 0xff;
    return 2;
    }

  UInt32 MemoryStream::Write(Dword d) {
    if (data == NULL) throw InvalidOpException(this, "Memory not opened");
    if (accessMode == ReadOnly) throw InvalidOpException(this, "Memory is read only");
    if (dataPointer+3 >= dataSize) throw InvalidOpException(this, "Attempt write past end");
    data[dataPointer++] = d & 0xff;
    data[dataPointer++] = (d >> 8) & 0xff;
    data[dataPointer++] = (d >> 16) & 0xff;
    data[dataPointer++] = (d >> 24) & 0xff;
    return 4;
    }

  UInt32 MemoryStream::Write(Qword q) {
    if (data == NULL) throw InvalidOpException(this, "Memory not opened");
    if (accessMode == ReadOnly) throw InvalidOpException(this, "Memory is read only");
    if (dataPointer+7 >= dataSize) throw InvalidOpException(this, "Attempt write past end");
    data[dataPointer++] = q & 0xff;
    data[dataPointer++] = (q >> 8) & 0xff;
    data[dataPointer++] = (q >> 16) & 0xff;
    data[dataPointer++] = (q >> 24) & 0xff;
    data[dataPointer++] = (q >> 32) & 0xff;
    data[dataPointer++] = (q >> 40) & 0xff;
    data[dataPointer++] = (q >> 48) & 0xff;
    data[dataPointer++] = (q >> 56) & 0xff;
    return 8;
    }

  UInt32 MemoryStream::Write(Byte* b,UInt32 len) {
    if (accessMode == ReadOnly) throw InvalidOpException(this, "Memory is read only");
    if (data == NULL) throw InvalidOpException(this, "Memory not opened");
    if (dataPointer+len-1 >= dataSize) throw InvalidOpException(this, "Attempt write past end");
    memcpy(data+dataPointer, b, len);
    dataPointer += len;
    return len;
    }

  Byte MemoryStream::ReadByte() {
    Byte b;
    if (data == NULL) throw InvalidOpException(this, "Memory not opened");
    if (accessMode == WriteOnly) throw InvalidOpException(this, "Memory is write only");
    if (dataPointer >= dataSize) throw InvalidOpException(this, "Attempt read past end");
    b = data[dataPointer++];
    return b;
    }

  Word MemoryStream::ReadWord() {
    Word i;
    if (data == NULL) throw InvalidOpException(this, "Memory not opened");
    if (accessMode == WriteOnly) throw InvalidOpException(this, "Memory is write only");
    if (dataPointer+1 >= dataSize) throw InvalidOpException(this, "Attempt read past end");
    i = data[dataPointer] | (data[dataPointer+1] << 8);
    dataPointer += 2;
    return i;
    }

  Dword MemoryStream::ReadDword() {
    Dword d;
    if (data == NULL) throw InvalidOpException(this, "Memory not opened");
    if (accessMode == WriteOnly) throw InvalidOpException(this, "Memory is write only");
    if (dataPointer+3 >= dataSize) throw InvalidOpException(this, "Attempt read past end");
    d = data[dataPointer] | (data[dataPointer+1] << 8) | (data[dataPointer+2] << 16) |
        (data[dataPointer+3] << 24);
    dataPointer += 4;
    return d;
    }

  Qword MemoryStream::ReadQword() {
    Qword q,qtmp;
    if (data == NULL) throw InvalidOpException(this, "Memory not opened");
    if (accessMode == WriteOnly) throw InvalidOpException(this, "Memory is write only");
    if (dataPointer+7 >= dataSize) throw InvalidOpException(this, "Attempt read past end");
    q = data[dataPointer] | (data[dataPointer+1] << 8) | (data[dataPointer+2] << 16) |
        (data[dataPointer+3] << 24);
    qtmp = (data[dataPointer+4]) | (data[dataPointer+5] << 8) |
           (data[dataPointer+6] << 16) | (data[dataPointer+7] << 24);
    dataPointer += 8;
    q |= (qtmp << 32);
    return q;
    }

  UInt32 MemoryStream::ReadByteArray(Byte* b,UInt32 maxlen) {
    if (data == NULL) throw InvalidOpException(this, "Memory not opened");
    if (accessMode == WriteOnly) throw InvalidOpException(this, "Memory is write only");
    if (dataPointer+maxlen-1 >= dataSize) throw InvalidOpException(this, "Attempt read past end");
    memcpy(b, data+dataPointer, maxlen);
    dataPointer += maxlen;
    return maxlen;
    }

  UInt32 MemoryStream::Rewind() {
    return Position(0);
    }

  }

