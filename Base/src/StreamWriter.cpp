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

  StreamWriter::StreamWriter(const char* filename) {
    objectType = (char*)"StreamWriter";
    stream = new FileStream(filename,Stream::Truncate);
    autoFlush = 0;
    newLine = (char*)malloc(sizeof(char) * 3);
    strcpy(newLine,"\n");
    }

  StreamWriter::StreamWriter(String filename) {
    objectType = (char*)"StreamWriter";
    stream = new FileStream(filename,Stream::Truncate);
    autoFlush = 0;
    newLine = (char*)malloc(sizeof(char) * 3);
    strcpy(newLine,"\n");
    }

  StreamWriter::StreamWriter(String* filename) {
    objectType = (char*)"StreamWriter";
    stream = new FileStream(filename,Stream::Truncate);
    autoFlush = 0;
    newLine = (char*)malloc(sizeof(char) * 3);
    strcpy(newLine,"\n");
    }

  StreamWriter::~StreamWriter() {
    if (stream != NULL) delete(stream);
    if (newLine != NULL) free(newLine);
    }
  
  void StreamWriter::AutoFlush(int i) {
    autoFlush = (i) ? -1 : 0;
    }

  void StreamWriter::NewLine(const char* term) {
    if (newLine != NULL) free(newLine);
    if (term != NULL) {
      newLine = (char*)malloc(strlen(term) + 1);
      strcpy(newLine, term);
      }
    else {
      newLine = NULL;
      }
    }
  
  void StreamWriter::Flush() {
    stream->Flush();
    }

  int StreamWriter::Write(char c) {
    int ret;
    ret = stream->Write((Byte)c);
    if (autoFlush) Flush();
    return ret;
    }
  
  int StreamWriter::Write(const char* c) {
    int ret;
    ret = stream->Write((Byte*)c, strlen(c));
    if (autoFlush) Flush();
    return ret;
    }

  int StreamWriter::Write(const char* c, int count) {
    int ret;
    ret = stream->Write((Byte*)c, count);
    if (autoFlush) Flush();
    return ret;
    }

  int StreamWriter::Write(String* s) {
    return Write(s->AsCharArray(), s->Length());
    }

  int StreamWriter::Write(String s) {
    return Write(s.AsCharArray(), s.Length());
    }

  int StreamWriter::Write(int i) {
    int ret;
    char temp[64];
    sprintf(temp,"%d",i);
    ret = stream->Write((Byte*)temp, strlen(temp));
    if (autoFlush) Flush();
    return ret;
    }

  int StreamWriter::Write(double d) {
    int ret;
    char temp[64];
    sprintf(temp,"%f",d);
    ret = stream->Write((Byte*)temp, strlen(temp));
    if (autoFlush) Flush();
    return ret;
    }

  int StreamWriter::Write(bool b) {
    int ret;
    if (b) ret = stream->Write((Byte*)"true",4);
      else stream->Write((Byte*)"false",5);
    if (autoFlush) Flush();
    return ret;
    }

  int StreamWriter::WriteLine() {
    int ret;
    if (newLine == NULL) return 0;
    ret = stream->Write((Byte*)newLine, strlen(newLine));
    if (autoFlush) Flush();
    return ret;
    }

  int StreamWriter::WriteLine(char c) {
    int ret;
    ret = Write(c);
    ret += WriteLine();
    return ret;
    }

  int StreamWriter::WriteLine(const char* c) {
    int ret;
    ret = Write(c);
    ret += WriteLine();
    return ret;
    }

  int StreamWriter::WriteLine(const char* c, int count) {
    int ret;
    ret = Write(c, count);
    ret += WriteLine();
    return ret;
    }

  int StreamWriter::WriteLine(String* s) {
    int ret;
    ret = Write(s);
    ret += WriteLine();
    return ret;
    }

  int StreamWriter::WriteLine(String s) {
    int ret;
    ret = Write(s);
    ret += WriteLine();
    return ret;
    }

  int StreamWriter::WriteLine(int i) {
    int ret;
    ret = Write(i);
    ret += WriteLine();
    return ret;
    }

  int StreamWriter::WriteLine(double d) {
    int ret;
    ret = Write(d);
    ret += WriteLine();
    return ret;
    }

  int StreamWriter::WriteLine(bool b) {
    int ret;
    ret = Write(b);
    ret += WriteLine();
    return ret;
    }

  }
