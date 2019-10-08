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
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include "SmrFramework.h"

namespace SmrFramework {

  Directory::Directory() {
    }

  Directory::~Directory() {
    }

  void Directory::CreateDirectory(const char* path) {
    int result;
    result = mkdir(path, 0777);
    if (result == 0) return;
    if (errno == EACCES)
      throw InvalidOpException(NULL, "No permission");
    if (errno == EEXIST)
      throw InvalidOpException(NULL, "Already exists");
    throw InvalidOpException(NULL, "Could not create");
    }

  void Directory::CreateDirectory(String* path) {
    CreateDirectory(path->AsCharArray());
    }

  void Directory::Delete(const char* path) {
    int result;
    result = rmdir(path);
    if (result == 0) return;
    if (errno == EACCES)
      throw InvalidOpException(NULL, "No permission");
    if (errno == EBUSY)
      throw InvalidOpException(NULL, "Directory busy");
    if (errno == ENOENT)
      throw InvalidOpException(NULL, "Directory does not exist");
    throw InvalidOpException(NULL, "Could not delete directory");
    }

  void Directory::Delete(String* path) {
    Delete(path->AsCharArray());
    }

  int Directory::Exists(const char* path) {
    DIR* directory;
    directory = opendir(path);
    if (directory == NULL) return 0;
    closedir(directory);
    return -1;
    }

  int Directory::Exists(String* path) {
    return Exists(path->AsCharArray());
    }

  String* Directory::GetCurrentDirectory() {
    char* buffer;
    String* ret;
    buffer = getcwd(NULL, 0);
    ret = new String(buffer);
    free(buffer);
    return ret;
    }

  List<String*>* Directory::GetAllFiles(const char* path,List<String*>* results) {
    DIR*           directory;
    struct dirent* entry;
    struct stat    st;
    char           fullname[4096];
    if (results == NULL) results = new List<String*>();
    directory = opendir(path);
    if (directory == NULL)
      throw InvalidOpException(NULL, "Path not found");
    while ((entry = readdir(directory)) != NULL) {
      strcpy(fullname, path);
      if (fullname[strlen(fullname)-1] != '/') strcat(fullname,"/");
      strcat(fullname,entry->d_name);
      if (lstat(fullname, &st) == 0) {
        if ((st.st_mode & S_IFMT) == S_IFREG) { 
          results->Add(new String(fullname));
          }
        else if ((st.st_mode & S_IFMT) == S_IFDIR) { 
          if (entry->d_name[0] != '.')
            GetAllFiles(fullname, results);
          }
        }
      }
    closedir(directory);
    return results;
    }

  List<String*>* Directory::GetFiles(const char* path) {
    List<String*>* files;
    DIR*           directory;
    struct dirent* entry;
    struct stat    st;
    directory = opendir(path);
    if (directory == NULL)
      throw InvalidOpException(NULL, "Path not found");
    files = new List<String*>();
    while ((entry = readdir(directory)) != NULL) {
      if (lstat(entry->d_name, &st) == 0) {
        if ((st.st_mode & S_IFMT) == S_IFDIR)
          files->Add(new String(String("/") + String(entry->d_name)));
        else
          files->Add(new String(entry->d_name));
        }
      else 
        files->Add(new String(entry->d_name));
      }
    closedir(directory);
    return files;
    }

  List<String*>* Directory::GetFiles(String* path) {
    return Directory::GetFiles(path->AsCharArray());
    }

  void Directory::SetCurrentDirectory(const char* path) {
    int result;
    result = chdir(path);
    if (result == 0) return;
    if (errno == EACCES)
      throw InvalidOpException(NULL, "No permission");
    if (errno == ENOENT)
      throw InvalidOpException(NULL, "Does not exist");
    throw InvalidOpException(NULL, "Could not change");
    }
    
  void Directory::SetCurrentDirectory(String* path) {
    return Directory::SetCurrentDirectory(path->AsCharArray());
    }

  }
