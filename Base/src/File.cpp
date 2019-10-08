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
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include "string.h"
#include "SmrFramework.h"

namespace SmrFramework {

  File::File() {
    objectType = (char*)"File";
    file = -1;
    mode = 0;
    }

  File::File(int f,int m) {
    objectType = (char*)"File";
    file = f;
    mode = m;
    }
  
  File::File(const char* fname) {
    file = open(fname, O_CREAT | O_WRONLY, 0666);
    if (file < 0) throw InvalidOpException(this, strerror(errno));
    mode = WriteOnly;
    }
  
  File::File(String fname) {
    file = open(fname.AsCharArray(), O_CREAT | O_WRONLY, 0666);
    if (file < 0) throw InvalidOpException(this, strerror(errno));
    mode = WriteOnly;
    }
  
  File::File(String* fname) {
    file = open(fname->AsCharArray(), O_CREAT | O_WRONLY, 0666);
    if (file < 0) throw InvalidOpException(this, strerror(errno));
    mode = WriteOnly;
    }
  
  File::File(const char* fname,int m) {
    file = -1;
    if (m == WriteOnly) file = open(fname, O_CREAT | O_WRONLY, 0666);
    if (m == ReadOnly)  file = open(fname, O_CREAT | O_RDONLY, 0666);
    if (m == ReadWrite) file = open(fname, O_CREAT | O_RDWR, 0666);
    if (m == Truncate) file = open(fname, O_CREAT | O_TRUNC | O_WRONLY, 0666);
    if (file < 0) throw InvalidOpException(this, strerror(errno));
    mode = WriteOnly;
    }
  
  File::File(String fname,int m) {
    file = -1;
    if (m == WriteOnly) file = open(fname.AsCharArray(), O_CREAT | O_WRONLY, 0666);
    if (m == ReadOnly)  file = open(fname.AsCharArray(), O_CREAT | O_RDONLY, 0666);
    if (m == ReadWrite) file = open(fname.AsCharArray(), O_CREAT | O_RDWR, 0666);
    if (m == Truncate) file = open(fname.AsCharArray(), O_CREAT | O_TRUNC | O_WRONLY, 0666);
    if (file < 0) throw InvalidOpException(this, strerror(errno));
    mode = WriteOnly;
    }
  
  File::File(String* fname,int m) {
    file = -1;
    if (m == WriteOnly) file = open(fname->AsCharArray(), O_CREAT | O_WRONLY, 0666);
    if (m == ReadOnly)  file = open(fname->AsCharArray(), O_CREAT | O_RDONLY, 0666);
    if (m == ReadWrite) file = open(fname->AsCharArray(), O_CREAT | O_RDWR, 0666);
    if (m == Truncate) file = open(fname->AsCharArray(), O_CREAT | O_TRUNC | O_WRONLY, 0666);
    if (file < 0) throw InvalidOpException(this, strerror(errno));
    mode = WriteOnly;
    }

  File::~File() {
    if (file >= 0) close(file);
    }
  
  String File::Basename(const char* path) {
    char *start;
    char *end;
    char *pos;
    char *temp;
    String ret;
    int   len;
    if (path == NULL) return ret;
    if (strlen(path) == 0) return ret;
    start = (char*)path;
    end = (char*)path;
    pos = (char*)path;
    while (*pos != 0) {
      if (*pos == '/' || *pos == '\\') start = pos;
      if (*pos == '.') end = pos;
      pos++;
      }
    while (*start == '/') start++;
    while (*start == '\\') start++;
    len = (end > start) ? end - start : strlen(start);
    if (len == 0) return ret;
    temp = (char*)malloc(len + 1);
    strncpy(temp,start,len);
    temp[len] = 0;
    ret = String(temp);
    free(temp);
    return ret;
    }

  String File::Basename(String* path) {
    return Basename(path->AsCharArray());
    }

  String File::Basename(String path) {
    return Basename(path.AsCharArray());
    }

  void File::Copy(const char* src, const char* dest) {
    int sfile, dfile;
    char buffer[1000];
    int  count;
    sfile = open(src,O_RDONLY, 0666);
    if (sfile < 0) throw InvalidOpException(NULL, "Source file not found");
    dfile = open(dest,O_CREAT | O_TRUNC | O_WRONLY, 0666);
    if (dfile < 0) {
      close(sfile);
      throw InvalidOpException(NULL, "Could not open destination file");
      }
    while ((count = read(sfile, buffer, 1000)) > 0) {
      write(dfile, buffer, count);
      }
    close(sfile);
    close(dfile);
    }

  void File::Delete(const char* pathname) {
    if (unlink(pathname) >= 0) return;
    throw InvalidOpException(NULL, strerror(errno));
    }

  bool File::Exists(const char* pathname) {
    int f;
    bool ret;
    f = open(pathname,O_RDONLY, 0666);
    ret = (f >= 0) ? true : false;
    if (f >= 0) close(f);
    return ret;
    }
  
  String File::Extension(const char* path) {
    char *end;
    char *pos;
    char *temp;
    String ret;
    int   len;
    if (path == NULL) return ret;
    if (strlen(path) == 0) return ret;
    end = (char*)path;
    pos = (char*)path;
    while (*pos != 0) {
      if (*pos == '.') end = pos;
      pos++;
      }
    if (end == path) return ret;
    while (*end == '.') end++;
    len = strlen(end);
    if (len == 0) return ret;
    temp = (char*)malloc(len + 1);
    strcpy(temp,end);
    ret = String(temp);
    free(temp);
    return ret;
    }

  String File::Extension(String* path) {
    return Extension(path->AsCharArray());
    }

  String File::Extension(String path) {
    return Extension(path.AsCharArray());
    }

  File* File::Create(const char* pathname) {
    int file;
    file = open(pathname, O_CREAT | O_WRONLY | O_EXCL, 0666);
    if (file >= 0) return new File(file,WriteOnly);
    throw InvalidOpException(NULL, strerror(errno));
    }

  void File::Move(const char* oldname, const char* newname) {
    if (rename(oldname, newname) < 0)
      throw InvalidOpException(NULL, strerror(errno));
    }

  File* File::OpenForAppend(const char* pathname) {
    int file;
    File* f;
    file = open(pathname, O_CREAT | O_WRONLY, 0666);
    if (file >= 0) {
      f = new File(file,WriteOnly);
      f->Append();
      return f;
      }
    throw InvalidOpException(NULL, strerror(errno));
    }

  File* File::OpenForReading(const char* pathname) {
    int file;
    file = open(pathname, O_CREAT | O_RDONLY, 0666);
    if (file >= 0) return new File(file,ReadOnly);
    throw InvalidOpException(NULL, strerror(errno));
    }

  File* File::OpenForReadingWriting(const char* pathname) {
    int file;
    file = open(pathname, O_CREAT | O_RDWR, 0666);
    if (file >= 0) return new File(file,ReadWrite);
    throw InvalidOpException(NULL, strerror(errno));
    }

  File* File::OpenForWriting(const char* pathname) {
    int file;
    file = open(pathname, O_CREAT | O_WRONLY, 0666);
    if (file >= 0) return new File(file,WriteOnly);
    throw InvalidOpException(NULL, strerror(errno));
    }
  
  String File::Path(const char* path) {
    char *end;
    char *pos;
    char *temp;
    String ret;
    int   len;
    if (path == NULL) return ret;
    if (strlen(path) == 0) return ret;
    end = (char*)path;
    pos = (char*)path;
    while (*pos != 0) {
      if (*pos == '/' || *pos == '\\') end = pos;
      pos++;
      }
    while (*end == '/' || *end == '\\') end++;
    len = (end > path) ? end - path : strlen(path);
    if (len == 0) return ret;
    temp = (char*)malloc(len + 1);
    strncpy(temp,path,len);
    temp[len] = 0;
    ret = String(temp);
    free(temp);
    return ret;
    }

  String File::Path(String* path) {
    return Path(path->AsCharArray());
    }

  String File::Path(String path) {
    return Path(path.AsCharArray());
    }

  UInt32 File::Append() {
    int ret;
    if (file < 0) throw InvalidOpException(this, "File not open");
    if (mode == ReadOnly) throw InvalidOpException(this, "File is open read only");
    ret = lseek(file, 0, SEEK_END);
    if (ret >= 0) return ret;
    throw InvalidOpException(this, strerror(errno));
    }

  void File::Close() {
    if (file < 0) throw InvalidOpException(this, "File not open");
    if (close(file) < 0) {
      throw InvalidOpException(this, strerror(errno));
      }
    file = -1;
    }

  bool File::EndOfFile() {
    struct stat info;
    if (file < 0) throw InvalidOpException(this, "File not open");
    if (fstat(file, &info) < 0)
      throw InvalidOpException(this, strerror(errno));
    if (lseek(file,0,SEEK_CUR) >= info.st_size) return true;
    return false;
    }

  void File::Flush() {
    if (file < 0) throw InvalidOpException(this, "File not open");
    fsync(file);
    }

  UInt32 File::Position() {
    UInt32 ret;
    if (file < 0) throw InvalidOpException(this, "File not open");
    ret = lseek(file, 0, SEEK_CUR);
    if (ret >= 0) return ret;
    throw InvalidOpException(this, strerror(errno));
    }

  UInt32 File::Position(UInt32 pos) {
    UInt32 ret;
    if (file < 0) throw InvalidOpException(this, "File not open");
    ret = lseek(file, pos, SEEK_SET);
    if (ret >= 0) return ret;
    throw InvalidOpException(this, strerror(errno));
    }

  Byte File::ReadByte() {
    Byte ret;
    int  count;
    if (file < 0) throw InvalidOpException(this, "File not open");
    count = read(file, &ret, 1);
    if (count == 1) return ret;
    if (count >= 0)
      throw InvalidOpException(this, "Did not read correct number of bytes");
    throw InvalidOpException(this, strerror(errno));
    }

  UInt32 File::ReadBytes(Byte* buffer, UInt32 count) {
    int  cnt;
    if (file < 0) throw InvalidOpException(this, "File not open");
    cnt = read(file, buffer, count);
    if (cnt >= 0) return cnt;
    throw InvalidOpException(this, strerror(errno));
    }

  Int16 File::ReadInt16() {
    Int16 ret;
    UInt32 i;
    Byte* ptr;
    Byte buffer[2];
    if (ReadBytes(buffer, 2) != 2)
      throw InvalidOpException(this, "Could not read correct number of bytes");
    ptr = (Byte*)&ret;
    for (i=0; i<2; i++) ptr[i] = buffer[i];
    return ret;
    }

  UInt16 File::ReadUInt16() {
    UInt16 ret;
    UInt32 i;
    Byte* ptr;
    Byte buffer[2];
    if (ReadBytes(buffer, 2) != 2)
      throw InvalidOpException(this, "Could not read correct number of bytes");
    ptr = (Byte*)&ret;
    for (i=0; i<2; i++) ptr[i] = buffer[i];
    return ret;
    }

  Int32 File::ReadInt32() {
    Int32 ret;
    UInt32 i;
    Byte* ptr;
    Byte buffer[4];
    if (ReadBytes(buffer, 4) != 4)
      throw InvalidOpException(this, "Could not read correct number of bytes");
    ptr = (Byte*)&ret;
    for (i=0; i<4; i++) ptr[i] = buffer[i];
    return ret;
    }

  UInt32 File::ReadUInt32() {
    UInt32 ret;
    UInt32 i;
    Byte* ptr;
    Byte buffer[4];
    if (ReadBytes(buffer, 4) != 4)
      throw InvalidOpException(this, "Could not read correct number of bytes");
    ptr = (Byte*)&ret;
    for (i=0; i<4; i++) ptr[i] = buffer[i];
    return ret;
    }

  Int64 File::ReadInt64() {
    Int64 ret;
    UInt32 i;
    Byte* ptr;
    Byte buffer[8];
    if (ReadBytes(buffer, 8) != 8)
      throw InvalidOpException(this, "Could not read correct number of bytes");
    ptr = (Byte*)&ret;
    for (i=0; i<8; i++) ptr[i] = buffer[i];
    return ret;
    }

  UInt64 File::ReadUInt64() {
    UInt64 ret;
    UInt32 i;
    Byte* ptr;
    Byte buffer[8];
    if (ReadBytes(buffer, 8) != 8)
      throw InvalidOpException(this, "Could not read correct number of bytes");
    ptr = (Byte*)&ret;
    for (i=0; i<8; i++) ptr[i] = buffer[i];
    return ret;
    }

  Single File::ReadSingle() {
    Single ret;
    UInt32 i;
    Byte* ptr;
    Byte buffer[16];
    if (ReadBytes(buffer, sizeof(Single)) != sizeof(Single))
      throw InvalidOpException(this, "Could not read correct number of bytes");
    ptr = (Byte*)&ret;
    for (i=0; i<sizeof(Single); i++) ptr[i] = buffer[i];
    return ret;
    }

  Double File::ReadDouble() {
    Double ret;
    UInt32 i;
    Byte* ptr;
    Byte buffer[16];
    if (ReadBytes(buffer, sizeof(Double)) != sizeof(Double))
      throw InvalidOpException(this, "Could not read correct number of bytes");
    ptr = (Byte*)&ret;
    for (i=0; i<sizeof(Double); i++) ptr[i] = buffer[i];
    return ret;
    }

  UInt32 File::Reset() {
    UInt32 ret;
    if (file < 0) throw InvalidOpException(this, "File not open");
    ret = lseek(file, 0, SEEK_SET);
    if (ret >= 0) return ret;
    throw InvalidOpException(this, strerror(errno));
    }
  
  UInt32 File::Write(Byte value) {
    UInt32 ret;
    if (file < 0) throw InvalidOpException(this, "File not open");
    ret = write(file, &value, 1);
    if (ret >= 0) return ret;
    throw InvalidOpException(this, strerror(errno));
    }

  UInt32 File::Write(void* buffer, int count) {
    UInt32 ret;
    if (file < 0) throw InvalidOpException(this, "File not open");
    ret = write(file, buffer, count);
    if (ret >= 0) return ret;
    throw InvalidOpException(this, strerror(errno));
    }

  UInt32 File::Write(Int16 value) {
    int i;
    Byte* ptr;
    Byte buffer[2];
    ptr = (Byte*)&value;
    for (i=0; i<2; i++) buffer[i] = ptr[i];
    return Write(buffer, 2);
    }

  UInt32 File::Write(UInt16 value) {
    int i;
    Byte* ptr;
    Byte buffer[2];
    ptr = (Byte*)&value;
    for (i=0; i<2; i++) buffer[i] = ptr[i];
    return Write(buffer, 2);
    }

  UInt32 File::Write(Int32 value) {
    int i;
    Byte* ptr;
    Byte buffer[4];
    ptr = (Byte*)&value;
    for (i=0; i<4; i++) buffer[i] = ptr[i];
    return Write(buffer, 4);
    }

  UInt32 File::Write(UInt32 value) {
    int i;
    Byte* ptr;
    Byte buffer[4];
    ptr = (Byte*)&value;
    for (i=0; i<4; i++) buffer[i] = ptr[i];
    return Write(buffer, 4);
    }

  UInt32 File::Write(Int64 value) {
    int i;
    Byte* ptr;
    Byte buffer[8];
    ptr = (Byte*)&value;
    for (i=0; i<8; i++) buffer[i] = ptr[i];
    return Write(buffer, 8);
    }

  UInt32 File::Write(UInt64 value) {
    int i;
    Byte* ptr;
    Byte buffer[8];
    ptr = (Byte*)&value;
    for (i=0; i<8; i++) buffer[i] = ptr[i];
    return Write(buffer, 8);
    }

  UInt32 File::Write(Single value) {
    UInt32 i;
    Byte* ptr;
    Byte buffer[16];
    ptr = (Byte*)&value;
    for (i=0; i<sizeof(Single); i++) buffer[i] = ptr[i];
    return Write(buffer, sizeof(Single));
    }

  UInt32 File::Write(Double value) {
    UInt32 i;
    Byte* ptr;
    Byte buffer[16];
    ptr = (Byte*)&value;
    for (i=0; i<sizeof(Double); i++) buffer[i] = ptr[i];
    return Write(buffer, sizeof(Double));
    }


}
