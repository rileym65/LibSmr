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
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "SmrFramework.h"

namespace SmrFramework {

  StreamReader::StreamReader(Stream* s) {
    objectType = (char*)"StreamReader";
    stream = s;
    }

  StreamReader::StreamReader(const char* filename) {
    objectType = (char*)"StreamReader";
    stream = new FileStream(filename,Stream::ReadOnly);
    }

  StreamReader::StreamReader(String filename) {
    objectType = (char*)"StreamReader";
    stream = new FileStream(filename,Stream::ReadOnly);
    }

  StreamReader::StreamReader(String* filename) {
    objectType = (char*)"StreamReader";
    stream = new FileStream(filename,Stream::ReadOnly);
    }

  StreamReader::~StreamReader() {
    if (stream != NULL) delete(stream);
    }
  
  Boolean StreamReader::DeleteResource() {
    return stream->DeleteResource();
    }

  Boolean StreamReader::DeleteResource(Boolean b) {
    return stream->DeleteResource(b);
    }

  bool StreamReader::EndOfStream() {
    if (stream == NULL) throw InvalidOpException(this, "Stream not opened");
    return stream->EndOfStream();
    }

  Byte StreamReader::Peek() {
    UInt32 pos;
    Byte   value;
    if (stream == NULL) throw InvalidOpException(this, "Stream not opened");
    pos = stream->Position();
    value = stream->ReadByte();
    stream->Position(pos);
    return value;
    }

  Byte StreamReader::Read() {
    if (stream == NULL) throw InvalidOpException(this, "Stream not opened");
    return stream->ReadByte();
    }

  Byte StreamReader::ReadByte() {
    if (stream == NULL) throw InvalidOpException(this, "Stream not opened");
    return stream->ReadByte();
    }

  Word StreamReader::ReadWord() {
    if (stream == NULL) throw InvalidOpException(this, "Stream not opened");
    return stream->ReadWord();
    }

  Dword StreamReader::ReadDword() {
    if (stream == NULL) throw InvalidOpException(this, "Stream not opened");
    return stream->ReadDword();
    }
    
  UInt32 StreamReader::Read(Byte* buffer, int len) {
    if (stream == NULL) throw InvalidOpException(this, "Stream not opened");
    return stream->ReadByteArray(buffer, len);
    }

  String StreamReader::ReadLine() {
    Byte*   buffer;
    UInt32  bufferSize;
    UInt32  bufferPointer;
    String ret;
    bool    finished;
    Byte    ending;
    Byte    value;
    if (stream == NULL) throw InvalidOpException(this, "Stream not opened");
    buffer = (Byte*)malloc(81);
    bufferSize = 81;
    bufferPointer = 0;
    finished = false;
    while (!finished) {
      if (stream->EndOfStream()) {
        buffer[bufferPointer] = 0;
        finished = true;
        }
      else {
        value = stream->ReadByte();
        if (value == 10 || value == 13) {
          buffer[bufferPointer] = 0;
          ending = value;
          if (stream->EndOfStream()) finished = true;
          else {
            value = Peek();
            if ((value == 10 || value == 13) && value != ending) {
              value = stream->ReadByte();
              }
            finished = true;
            }
          }
        else {
          buffer[bufferPointer++] = value;
          if (bufferPointer+1 >= bufferSize) {
            bufferSize += 80;
            buffer = (Byte*)realloc(buffer,bufferSize);
            if (buffer == NULL)
              throw InvalidOpException(this, "Could not allocate memory");
            }
          }
        }
      }
    ret = String((const char*)buffer);
    free(buffer);
    return ret;
    }

  }
