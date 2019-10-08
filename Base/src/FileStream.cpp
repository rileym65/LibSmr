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
#include "SmrFramework.h"

namespace SmrFramework {

  FileStream::FileStream(const char* path) : Stream(ReadOnly) {
    file = NULL;
    accessMode = ReadOnly;
    objectType = (char*)"FileStream";
    file = new File(path, ReadOnly);
    if (file == NULL) throw InvalidOpException(this, "Could not open file");
    }

  FileStream::FileStream(const char* path, Byte mode) : Stream(mode) {
    file = NULL;
    accessMode = mode;
    objectType = (char*)"FileStream";
    file = new File(path, mode);
    if (file == NULL) throw InvalidOpException(this, "Could not open file");
    }

  FileStream::FileStream(String path) : Stream(ReadOnly) {
    file = NULL;
    accessMode = ReadOnly;
    objectType = (char*)"FileStream";
    file = new File(path, ReadOnly);
    if (file == NULL) throw InvalidOpException(this, "Could not open file");
    }

  FileStream::FileStream(String path, Byte mode) : Stream(mode) {
    file = NULL;
    accessMode = mode;
    objectType = (char*)"FileStream";
    file = new File(path, mode);
    if (file == NULL) throw InvalidOpException(this, "Could not open file");
    }

  FileStream::FileStream(String* path) : Stream(ReadOnly) {
    file = NULL;
    accessMode = ReadOnly;
    objectType = (char*)"FileStream";
    file = new File(path, ReadOnly);
    if (file == NULL) throw InvalidOpException(this, "Could not open file");
    }

  FileStream::FileStream(String* path, Byte mode) : Stream(mode) {
    file = NULL;
    accessMode = mode;
    objectType = (char*)"FileStream";
    file = new File(path, mode);
    if (file == NULL) throw InvalidOpException(this, "Could not open file");
    }

  FileStream::~FileStream() {
    if (file != NULL && deleteResource) {
      file->Close();
      delete(file);
      }
    }

  void FileStream::Close() {
    if (file == NULL) throw InvalidOpException(this, "Already closed");
    file->Close();
    delete(file);
    file = NULL;
    }

  bool FileStream::EndOfStream() {
    if (file == NULL) throw InvalidOpException(this, "File not opened");
    return file->EndOfFile();
    }

  void FileStream::Flush() {
    if (file == NULL) throw InvalidOpException(this, "File not opened");
    file->Flush();
    }

  UInt32 FileStream::Position() {
    if (file == NULL) throw InvalidOpException(this, "File not opened");
    return file->Position();
    }

  UInt32 FileStream::Position(UInt32 pos) {
    if (file == NULL) throw InvalidOpException(this, "File not opened");
    return file->Position(pos);
    }

  UInt32 FileStream::Write(Byte b) {
    if (file == NULL) throw InvalidOpException(this, "File not opened");
    if (accessMode == ReadOnly) throw InvalidOpException(this, "File is read only");
    return file->Write(b);
    }

  UInt32 FileStream::Write(Word i) {
    if (file == NULL) throw InvalidOpException(this, "File not opened");
    if (accessMode == ReadOnly) throw InvalidOpException(this, "File is read only");
    return file->Write(i);
    }

  UInt32 FileStream::Write(Dword d) {
    if (file == NULL) throw InvalidOpException(this, "File not opened");
    if (accessMode == ReadOnly) throw InvalidOpException(this, "File is read only");
    return file->Write(d);
    }

  UInt32 FileStream::Write(Qword q) {
    if (file == NULL) throw InvalidOpException(this, "File not opened");
    if (accessMode == ReadOnly) throw InvalidOpException(this, "File is read only");
    return file->Write(q);
    }

  UInt32 FileStream::Write(Byte* b,UInt32 len) {
    if (accessMode == ReadOnly) throw InvalidOpException(this, "File is read only");
    if (file == NULL) throw InvalidOpException(this, "File not opened");
    return file->Write(b, len);
    }

  Byte FileStream::ReadByte() {
    if (file == NULL) throw InvalidOpException(this, "File not opened");
    if (accessMode == WriteOnly) throw InvalidOpException(this, "File is write only");
    return file->ReadByte();
    }

  Word FileStream::ReadWord() {
    if (file == NULL) throw InvalidOpException(this, "File not opened");
    if (accessMode == WriteOnly) throw InvalidOpException(this, "File is write only");
    return file->ReadUInt16();
    }

  Dword FileStream::ReadDword() {
    if (file == NULL) throw InvalidOpException(this, "File not opened");
    if (accessMode == WriteOnly) throw InvalidOpException(this, "File is write only");
    return file->ReadUInt32();
    }

  Qword FileStream::ReadQword() {
    if (file == NULL) throw InvalidOpException(this, "File not opened");
    if (accessMode == WriteOnly) throw InvalidOpException(this, "File is write only");
    return file->ReadUInt64();
    }

  UInt32 FileStream::ReadByteArray(Byte* b,UInt32 maxlen) {
    if (file == NULL) throw InvalidOpException(this, "File not opened");
    if (accessMode == WriteOnly) throw InvalidOpException(this, "File is write only");
    return file->ReadBytes(b, maxlen);
    }

  UInt32 FileStream::Rewind() {
    return Position(0);
    }
  }

