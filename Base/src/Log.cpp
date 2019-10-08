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
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "SmrFramework.h"

namespace SmrFramework {

  Log::Log(const char* filename) {
    objectType = (char*)"Log";
    filenames = new List<String*>();
    filenames->Add(new String(filename));
    }

  Log::Log(String* filename) {
    objectType = (char*)"Log";
    filenames = new List<String*>();
    filenames->Add(new String(filename->AsCharArray()));
    }

  Log::~Log() {
    while (this->filenames->Count() > 0) {
      delete(filenames->At(0));
      filenames->RemoveAt(0);
      }
    delete(filenames);
    }

  void Log::Add(const char* filename) {
    filenames->Add(new String(filename));
    }

  void Log::Add(String* filename) {
    filenames->Add(new String(filename->AsCharArray()));
    }

  void Log::Close() {
    while (this->filenames->Count() > 0) {
      delete(filenames->At(0));
      }
    }

  void Log::Write(const char* message) {
    UInt32 i;
    FILE* file;
    struct tm tim;
    time_t tt;
    tt = time(NULL);
    localtime_r(&tt, &tim);
    for (i=0; i<this->filenames->Count(); i++) {
      file = fopen(this->filenames->At(i)->AsCharArray(),"a");
      fprintf(file,"%2d/%2d/%4d %2d:%2d:%2d %s\n",
        tim.tm_mon+1, tim.tm_mday, tim.tm_year + 1900,
        tim.tm_hour, tim.tm_min, tim.tm_sec, message);
      fclose(file);
      }
    }
  }


