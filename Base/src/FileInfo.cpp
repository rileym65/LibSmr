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
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "SmrFramework.h"

namespace SmrFramework {

  FileInfo::FileInfo(const char* path) {
    objectType = (char*)"FileInfo";
    lastAccessTime = NULL;
    lastModificationTime = NULL;
    lastStatusTime = NULL;
    if (getFileInfo(path) != 0)
      throw InvalidOpException(this, "File not found");
    }

  FileInfo::FileInfo(String* path) {
    if (getFileInfo(path->AsCharArray()) != 0)
      throw InvalidOpException(this, "File not found");
    }

  FileInfo::~FileInfo() {
    if (lastAccessTime != NULL) delete(lastAccessTime);
    if (lastModificationTime != NULL) delete(lastModificationTime);
    if (lastStatusTime != NULL) delete(lastStatusTime);
    }

  int FileInfo::getFileInfo(const char* path) {
    struct stat info;
    struct tm   tm;
    if (stat(path, &info) != 0) return -1;
    this->device = info.st_dev;
    this->inode = info.st_ino;
    this->owner = info.st_uid;
    this->group = info.st_gid;
    this->links = info.st_nlink;
    this->size = info.st_size;
    this->blockSize = info.st_blksize;
    this->blockCount = info.st_blocks;
    switch (info.st_mode & S_IFMT) {
      case S_IFREG : this->fileType = FileInfo::RegularFile; break;
      case S_IFLNK : this->fileType = FileInfo::SymbolicLink; break;
      case S_IFSOCK: this->fileType = FileInfo::Socket; break;
      case S_IFBLK : this->fileType = FileInfo::BlockDevice; break;
      case S_IFDIR : this->fileType = FileInfo::Directory; break;
      case S_IFCHR : this->fileType = FileInfo::CharDevice; break;
      case S_IFIFO : this->fileType = FileInfo::NamedPipe; break;
      default      : this->fileType = FileInfo::Unknown; break;
      }
    this->access = info.st_mode;
    localtime_r((time_t*)&info.st_atim.tv_sec, &tm);
    lastAccessTime = new DateTime(tm.tm_mon+1,tm.tm_mday,tm.tm_year+1900,
                                  tm.tm_hour, tm.tm_min, tm.tm_sec);
    localtime_r((time_t*)&info.st_mtim.tv_sec, &tm);
    lastModificationTime = new DateTime(tm.tm_mon+1,tm.tm_mday,tm.tm_year+1900,
                                        tm.tm_hour, tm.tm_min, tm.tm_sec);
    localtime_r((time_t*)&info.st_ctim.tv_sec, &tm);
    lastStatusTime = new DateTime(tm.tm_mon+1,tm.tm_mday,tm.tm_year+1900,
                                  tm.tm_hour, tm.tm_min, tm.tm_sec);
    return 0;
    }

  int FileInfo::BlockCount() {
    return blockCount;
    }

  int FileInfo::BlockSize() {
    return blockSize;
    }

  int FileInfo::Device() {
    return device;
    }

  int FileInfo::FileType() {
    return fileType;
    }

  int FileInfo::Inode() {
    return inode;
    }

  int FileInfo::Group() {
    return group;
    }

  int FileInfo::Links() {
    return links;
    }

  int FileInfo::Owner() {
    return owner;
    }

  int FileInfo::Size() {
    return size;
    }

  int FileInfo::OwnerRead() {
    return (access & S_IRUSR) ? -1 : 0;
    }

  int FileInfo::OwnerWrite() {
    return (access & S_IWUSR) ? -1 : 0;
    }

  int FileInfo::OwnerExecute() {
    return (access & S_IXUSR) ? -1 : 0;
    }

  int FileInfo::GroupRead() {
    return (access & S_IRGRP) ? -1 : 0;
    }

  int FileInfo::GroupWrite() {
    return (access & S_IWGRP) ? -1 : 0;
    }

  int FileInfo::GroupExecute() {
    return (access & S_IXGRP) ? -1 : 0;
    }

  int FileInfo::OtherRead() {
    return (access & S_IROTH) ? -1 : 0;
    }

  int FileInfo::OtherWrite() {
    return (access & S_IWOTH) ? -1 : 0;
    }

  int FileInfo::OtherExecute() {
    return (access & S_IXOTH) ? -1 : 0;
    }

  DateTime* FileInfo::LastAccessTime() {
    return lastAccessTime;
    }

  DateTime* FileInfo::LastModificationTime() {
    return lastModificationTime;
    }

  DateTime* FileInfo::LastStatusTime() {
    return lastStatusTime;
    }

  }

