#include <string.h>
#include <math.h>
#include "SmrFramework.h"

using namespace SmrFramework;

PreProcessor::PreProcessor(const char* filename) {
  objectType = (char*)"PreProcessor";
  errors = 0;
  numCond = 0;
  numDefines = 0;
  redefine = false;
  strcpy(fileNames[0], filename);
  files[0] = fopen(filename, "r");
  if (files[0] == NULL)
    throw NotFoundException("File not found");
  numFiles = 1;
  totalLines = 0;
  fileLines[0] = 0;
  }

PreProcessor::~PreProcessor() {
  UInt32 i;
  if (numDefines > 0) {
    for (i=0; i<numDefines; i++)
      if (defineNames[i] != NULL) {
        free(defineNames[i]);
        free(defineValues[i]);
        }
    free(defineNames);
    free(defineValues);
    }
  }

Boolean PreProcessor::EndOfFile() {
  if (numFiles > 1) return false;
  if (feof(files[0])) return true;
  return false;
  }

char *PreProcessor::trim(char* line) {
  while (*line == ' ' || *line == '\t') line++;
  return line;
  }

void PreProcessor::replace(char* line, UInt32 start, UInt32 len, const char* value) {
  char tmp[4096];
  strncpy(tmp, line, start);
  tmp[start] = 0;
  strcat(tmp, value);
  strcat(tmp, line+start+len);
  strcpy(line, tmp);
  }

String *PreProcessor::Read() {
  char  *line;
  char  *pchar;
  int    flag;
  int    rflag;
  char   tmp[1024];
  char   tmp2[16];
  char   buffer[4096];
  DateTime dt;
  int    i;
  int    j;
  Double value;
  Calculator *calc;
  strcpy(buffer, "");
  flag = 0;
  while (flag == 0) {
    flag = -1;
    line=buffer;
    while (numFiles > 1 && feof(files[numFiles-1])) {
      fclose(files[numFiles-1]);
      numFiles--;
      }
    if (feof(files[numFiles-1])) {
      return NULL;
      }
    fgets(line, 4095, files[numFiles-1]);
    totalLines++;
    fileLines[numFiles-1]++;
    while (*line != 0 && line[strlen(line)-1] < ' ') line[strlen(line)-1] = 0;
    rflag = -1;
    if (strncmp(line, "#ifdef ", 7) == 0) rflag = 0;
    if (strncmp(line, "#ifndef ", 8) == 0) rflag = 0;
    if (strncmp(line, "#undef ", 7) == 0) rflag = 0;
    while (rflag) {
      rflag = 0;
      pchar = strstr(buffer, "__LINE__");
      if (pchar != NULL) {
        sprintf(tmp2,"%d",fileLines[numFiles-1]);
        replace(buffer, pchar-buffer, 8, tmp2);
        rflag = -1;
        }
      pchar = strstr(buffer, "__DATE__");
      if (pchar != NULL) {
        dt = DateTime::Now();
        sprintf(tmp2,"%02d-%02d-%04d",dt.Month(), dt.Day(), dt.Year());
        replace(buffer, pchar-buffer, 8, tmp2);
        rflag = -1;
        }
      pchar = strstr(buffer, "__MONTH__");
      if (pchar != NULL) {
        dt = DateTime::Now();
        sprintf(tmp2,"%02d",dt.Month());
        replace(buffer, pchar-buffer, 9, tmp2);
        rflag = -1;
        }
      pchar = strstr(buffer, "__DAY__");
      if (pchar != NULL) {
        dt = DateTime::Now();
        sprintf(tmp2,"%02d",dt.Day());
        replace(buffer, pchar-buffer, 7, tmp2);
        rflag = -1;
        }
      pchar = strstr(buffer, "__YEAR__");
      if (pchar != NULL) {
        dt = DateTime::Now();
        sprintf(tmp2,"%02d",dt.Year());
        replace(buffer, pchar-buffer, 8, tmp2);
        rflag = -1;
        }
      pchar = strstr(buffer, "__TIME__");
      if (pchar != NULL) {
        dt = DateTime::Now();
        sprintf(tmp2,"%02d:%02d:%02d",dt.Hour(), dt.Minute(), dt.Second());
        replace(buffer, pchar-buffer, 8, tmp2);
        rflag = -1;
        }
      pchar = strstr(buffer, "__HOUR__");
      if (pchar != NULL) {
        dt = DateTime::Now();
        sprintf(tmp2,"%02d",dt.Hour());
        replace(buffer, pchar-buffer, 8, tmp2);
        rflag = -1;
        }
      pchar = strstr(buffer, "__MINUTE__");
      if (pchar != NULL) {
        dt = DateTime::Now();
        sprintf(tmp2,"%02d",dt.Minute());
        replace(buffer, pchar-buffer, 10, tmp2);
        rflag = -1;
        }
      pchar = strstr(buffer, "__SECOND__");
      if (pchar != NULL) {
        dt = DateTime::Now();
        sprintf(tmp2,"%02d",dt.Second());
        replace(buffer, pchar-buffer, 10, tmp2);
        rflag = -1;
        }
      pchar = strstr(buffer, "__FILE__");
      if (pchar != NULL) {
        replace(buffer, pchar-buffer, 8, fileNames[numFiles-1]);
        rflag = -1;
        }
      for (i=0; (UInt32)i<numDefines; i++) {
        pchar = strstr(buffer, defineNames[i]);
        if (pchar != NULL) {
          if (*(pchar-1) >= 'a' && *(pchar-1) <= 'z') pchar = NULL;
          else if (*(pchar-1) >= 'A' && *(pchar-1) <= 'Z') pchar = NULL;
          else if (*(pchar-1) >= '0' && *(pchar-1) <= '9') pchar = NULL;
          }
        if (pchar != NULL) {
          if (*(pchar+strlen(defineNames[i])) >= 'a' &&
              *(pchar+strlen(defineNames[i])) <= 'z') pchar = NULL;
          else if (*(pchar+strlen(defineNames[i])) >= 'A' &&
                   *(pchar+strlen(defineNames[i])) <= 'Z') pchar = NULL;
          else if (*(pchar+strlen(defineNames[i])) >= '0' &&
                   *(pchar+strlen(defineNames[i])) <= '9') pchar = NULL;
          }
        if (pchar != NULL) {
          replace(buffer, pchar-buffer, strlen(defineNames[i]), defineValues[i]);
/*
          strncpy(tmp, buffer, pchar-buffer);
          tmp[pchar-buffer] = 0;
          strcat(tmp, defineValues[i]);
          strcat(tmp, pchar+strlen(defineNames[i]));
          strcpy(buffer,tmp);
*/
          rflag = -1;
          }
        }
      }


    while (strlen(line) > 0 && line[strlen(line)-1] <= ' ')
      line[strlen(line)-1] = 0;
    if (strncmp(line,"#include ",9) == 0) {
      if (numFiles == 32)
        throw InvalidOpException("Too many nested files");
      flag = 0;
      line += 9;
      line = trim(line);
      files[numFiles] = fopen(line,"r");
      if (files[numFiles] == NULL) {
        sprintf(tmp,"Could not open include file: <<%s>>",line);
        throw NotFoundException(tmp);
        }
      fileLines[numFiles] = 0;
      strcpy(fileNames[numFiles], line);
      numFiles++;
      }

    else if (strncmp(line,"#define ",8) == 0) {
      flag = 0;
      line += 8;
      line = trim(line);
      pchar = strchr(line,' ');
      if (pchar != NULL) {
        *pchar = 0;
        pchar++;
        pchar = trim(pchar);
        SetDefine(line, pchar);
        }
      else {
        SetDefine(line,"1");
        }
      }


    else if (strncmp(line,"#ifdef ",7) == 0) {
      flag = 0;
      if (numCond > 0 &&
          (cond[numCond-1] == 'N' || cond[numCond-1] == 'I')) {
        cond[numCond++] = 'I';
        }
      else {
        line += 6;
        line = trim(line);
        pchar = strchr(line,' ');
        if (pchar != NULL) *pchar = 0;
        pchar = GetDefine(line);
        if (pchar == NULL) {
          cond[numCond++] = 'N';
          }
        else if (strcmp(pchar,"0") == 0) {
          cond[numCond++] = 'N';
          }
        else cond[numCond++] = 'Y';
        }
      }

    else if (strncmp(line,"#ifndef ",8) == 0) {
      flag = 0;
      if (numCond > 0 &&
          (cond[numCond-1] == 'N' || cond[numCond-1] == 'I')) {
        cond[numCond++] = 'I';
        }
      else {
        line += 7;
        line = trim(line);
        pchar = strchr(line,' ');
        if (pchar != NULL) *pchar = 0;
        pchar = GetDefine(line);
        if (pchar == NULL) {
          cond[numCond++] = 'Y';
          }
        else if (strcmp(pchar,"0") == 0) {
          cond[numCond++] = 'Y';
          }
        else cond[numCond++] = 'N';
        }
      }

    else if (strncmp(line,"#if ",4) == 0) {
      flag = 0;
      if (numCond > 0 &&
          (cond[numCond-1] == 'N' || cond[numCond-1] == 'I')) {
        cond[numCond++] = 'I';
        }
      else {
        line += 4;
        line = trim(line);
        calc = new Calculator();
        value = calc->Calculate(line);
        delete(calc);
//        getNumber(line, &value);
        if (value == 0) {
          cond[numCond++] = 'N';
          }
        else cond[numCond++] = 'Y';
        }
      }

    else if (strncmp(line,"#else",5) == 0) {
      flag = 0;
      if (numCond == 0) {
        throw InvalidOpException("#else outside #ifdef");
        }
      else {
        if (cond[numCond-1] == 'N') cond[numCond-1] = 'Y';
        else if (cond[numCond-1] == 'Y') cond[numCond-1] = 'N';
        }
      }

    else if (strncmp(line,"#endif",6) == 0) {
      flag = 0;
      if (numCond == 0) {
        throw InvalidOpException("Unmatched #endif");
        }
      else numCond--;
      }

    else if (strncmp(line,"#undef ",7) == 0) {
      flag = 0;
      line += 7;
      line = trim(line);
      pchar = strchr(line,' ');
      if (pchar != NULL) *pchar = 0;
        i = findDefine(line);
        if (i >= 0) {
          free(defineNames[i]);
          free(defineValues[i]);
          for (j=i; (UInt32)j<numDefines-1; j++) {
            defineNames[j] = defineNames[j+1];
            defineValues[j] = defineValues[j+1];
            }
          numDefines--;
          if (numDefines == 0) {
            free(defineNames);
            free(defineValues);
            defineNames = NULL;
            defineValues = NULL;
            }
          }
      }

    else if (strncmp(line,"#print ",7) == 0) {
      flag = 0;
      if (numCond > 0 &&
          (cond[numCond-1] == 'N' || cond[numCond-1] == 'I')) {
        cond[numCond++] = 'I';
        }
      else {
        line += 7;
        line = trim(line);
        printf("%s\n",line);
        }
      }

    else if (strncmp(line,"#error ",7) == 0) {
      flag = 0;
      if (numCond > 0 &&
          (cond[numCond-1] == 'N' || cond[numCond-1] == 'I')) {
        cond[numCond++] = 'I';
        }
      else {
        line += 7;
        line = trim(line);
        printf("%s\n",line);
        errors++;
        }
      }

    else if (numCond > 0 && cond[numCond-1] == 'I') flag = 0;
    else if (numCond > 0 && cond[numCond-1] == 'N') flag = 0;
    }

  return new String(buffer);
  }


Int32 PreProcessor::findDefine(const char *name) {
  UInt32 i;
  for (i=0; i<numDefines; i++)
    if (strcmp(name, defineNames[i]) == 0)
      return i;
  return -1;
  }

char* PreProcessor::GetDefine(const char *name) {
  Int32 d;
  d = findDefine(name);
  if (d >= 0) return defineValues[d];
  return NULL;
  }


Boolean PreProcessor::Redefine() {
  return redefine;
  }

Boolean PreProcessor::Redefine(Boolean b) {
  redefine = b;
  return redefine;
  }

void PreProcessor::SetDefine(const char* name, const char* value) {
  Int32  d;
  d = findDefine(name);
  if (d >= 0) {
    if (redefine) {
      free(defineValues[d]);
      defineValues[d] = (char*)malloc(strlen(value)+1);
      if (defineValues[d] == NULL)
        throw MemoryException("Could not allocate memory");
      strcpy(defineValues[d], value);
      return;
      }
    else
      throw InvalidOpException("Redefinition not allowed");
    }
  numDefines++;
  if (numDefines == 1) {
    defineNames = (char**)malloc(sizeof(char*));
    defineValues = (char**)malloc(sizeof(char*));
    }
  else {
    defineNames = (char**)realloc(defineNames,sizeof(char*)*numDefines);
    defineValues = (char**)realloc(defineValues,sizeof(char*)*numDefines);
    }
  defineNames[numDefines-1] = (char*)malloc(strlen(name)+1);
  strcpy(defineNames[numDefines-1], name);
  defineValues[numDefines-1] = (char*)malloc(strlen(value)+1);
  strcpy(defineValues[numDefines-1], value);
  }

UInt32 PreProcessor::Errors() {
  return errors;
  }

UInt32 PreProcessor::LineCount() {
  return fileLines[numFiles-1];
  }

UInt32 PreProcessor::FileCount() {
  return numFiles;
  }

