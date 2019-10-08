/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/


#include "string.h"
#include "SmrFramework.h"

namespace SmrFramework {

  Environment::Environment(char** envp) {
    environment = envp;
    objectType = (char*)"Environment";
    }
  
  Environment::~Environment() {
    }
  
  String Environment::Get(const char* name) {
    UInt32 i,pos;
    UInt32 len;
    char key[2048];
    strcpy(key, name);
    strcat(key,"=");
    len = strlen(key);
    for (i=0; environment[i] != NULL; i++)
      if (strncasecmp(environment[i],key,len) == 0) {
        pos = 0;
        while (environment[i][pos] != '=') pos++;
        while (environment[i][pos] == '=') pos++;
        while (environment[i][pos] == ' ') pos++;
        return String(environment[i] + pos);
        }
    return String("");
    }

  }

