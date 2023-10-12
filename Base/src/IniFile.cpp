#include <stdio.h>
#include <string.h>
#include "SmrFramework.h"

using namespace SmrFramework;

IniFile::IniFile(const char *filename) {
  objectType = (char*)"IniFile";
  _readIniFile(filename);
  }

IniFile::IniFile(String filename) {
  objectType = (char*)"IniFile";
  _readIniFile(filename.AsCharArray());
  }

IniFile::IniFile(String *filename) {
  objectType = (char*)"IniFile";
  _readIniFile(filename->AsCharArray());
  }

IniFile::~IniFile() {
  UInt32 i;
  if (modified) Save();
  if (numSections > 0) {
    for (i=0; i<numSections; i++)
      free(sections[i]);
    free(sections);
    numSections = 0;
    }
  if (numEntries > 0) {
    for (i=0; i<numEntries; i++) {
      free(entryKey[i]);
      free(entryValue[i]);
      }
    free(entryKey);
    free(entryValue);
    free(entrySection);
    }
  delete(iniName);
  }

void IniFile::_readIniFile(const char* filename) {
  UInt32 i;
  FILE  *file;
  char   line[4096];
  char  *pline;
  numEntries = 0;
  sections = NULL;
  lastSection = 0;
  modified = false;
  iniName = new String(filename);
  file = fopen(filename,"r");
  if (file == NULL) {
    throw NotFoundException("File not found");
    }
  sections = (char**)malloc(sizeof(char*));
  if (sections == NULL)
    throw MemoryException("Could not allocate memory");
  sections[0] = (char*)malloc(sizeof("")+1);
  if (sections[0] == NULL)
    throw MemoryException("Could not allocate memory");
  strcpy(sections[0], "");
  numSections = 1;
  while (fgets(line, 4095, file) != NULL) {
    i = strlen(line);
    while (i > 0 && line[i-1] <= ' ') {
      line[i-1] = 0;
      i--;
      }
    pline = line;
    while (*pline == ' ' || *pline == '\t') pline++;
    if (*pline == '[') _readSection(pline);
    else if (*pline == ';' || *pline == '#') ;
    else if (strlen(pline) > 0)
      _readEntry(pline);
    }
  fclose(file);
  }

void IniFile::_readSection(const char* line) {
  char   name[256];
  UInt32 i;
  line++;
  i = 0;
  while (*line != 0 && *line != ']')
    name[i++] = *line++;
  name[i] = 0;
  if (*line != ']')
    printf("Warning: Section %s improperly terminated\n",name);
  for (i=0; i<numSections; i++)
    if (strcasecmp(name, sections[i]) == 0) {
      lastSection = i;
      return;
      }
  numSections++;
  if (numSections == 1)
    sections = (char**)malloc(sizeof(char*));
  else
    sections = (char**)realloc(sections, sizeof(char*) * numSections);
  if (sections == NULL)
    throw MemoryException("Could not allocate memory");
  sections[numSections-1] = (char*)malloc(strlen(name)+1);
  if (sections[numSections-1] == NULL)
    throw MemoryException("Could not allocate memory");
  strcpy(sections[numSections-1], name);
  lastSection = numSections-1;
  }

void IniFile::_readEntry(const char* line) {
  UInt32 i;
  char   key[256];
  char   value[2048];
  i = 0;
  while (*line > 0 && *line != '=') {
    key[i++] = *line++;
    if (i == 256)
      throw InvalidOpException("Key exceeds 255 characters");
    }
  key[i] = 0;
  while (i > 0 && key[i-1] <= ' ') {
    key[i-1] = 0;
    i--;
    }
  if (*line != '=')
    throw InvalidOpException("Invalid key/value pair");
  line++;
  while (*line != 0 && (*line == ' ' || *line == '\t')) line++;
  i = 0;
  while (*line > 0) {
    value[i++] = *line++;
    if (i == 2048)
      throw InvalidOpException("Value exceeds 2048 characters");
    }
  value[i] = 0;
  while (i > 0 && value[i-1] <= ' ') {
    value[i-1] = 0;
    i--;
    }
  for (i=0; i<numEntries; i++) {
    if (entrySection[i] == lastSection &&
        strcasecmp(key, entryKey[i]) == 0)
      throw InvalidOpException("Duplicate section/key");
    }
  numEntries++;
  if (numEntries == 1) {
    entrySection = (UInt32*)malloc(sizeof(UInt32));
    entryKey = (char**)malloc(sizeof(char*));
    entryValue = (char**)malloc(sizeof(char*));
    }
  else {
    entrySection = (UInt32*)realloc(entrySection, sizeof(UInt32) * numEntries);
    entryKey = (char**)realloc(entryKey, sizeof(char*) * numEntries);
    entryValue = (char**)realloc(entryValue, sizeof(char*) * numEntries);
    }
  if (entrySection == NULL || entryKey == NULL || entryValue == NULL)
    throw MemoryException("Could not allocate memory");
  entrySection[numEntries-1] = lastSection;
  entryKey[numEntries-1] = (char*)malloc(strlen(key)+1);
  if (entryKey[numEntries-1] == NULL)
    throw MemoryException("Could not allocate memory");
  strcpy(entryKey[numEntries-1], key);
  entryValue[numEntries-1] = (char*)malloc(strlen(key)+1);
  if (entryValue[numEntries-1] == NULL)
    throw MemoryException("Could not allocate memory");
  strcpy(entryValue[numEntries-1], value);
  }

Int32 IniFile::_findSection(const char* section) {
  UInt32 i;
  for (i=0; i<numSections; i++) {
    if (strcasecmp(section, sections[i]) == 0) return i;
    }
  return -1;
  }

Int32 IniFile::_findEntry(UInt32 section, const char* key) {
  UInt32 i;
  for (i=0; i<numEntries; i++)
    if (entrySection[i] == section && strcasecmp(entryKey[i], key) == 0)
      return i;
  return -1;
  }

void IniFile::Save() {
  UInt32 s;
  UInt32 e;
  Boolean written;
  FILE* file;
  if (!modified) return;
  written = false;
  file = fopen(iniName->AsCharArray(), "w");
  if (file == NULL)
    throw Exception("Could not open file");
  for (s=0; s<numSections; s++) {
    if (written) fprintf(file,"\n");
    if (s != 0) fprintf(file,"[%s]\n",sections[s]);
    for (e=0; e<numEntries; e++) {
      if (entrySection[e] == s) {
        written = true;
        fprintf(file,"%s=%s\n",entryKey[e], entryValue[e]);
        }
      }
    }
  fclose(file);
  modified = false;
  }

String *IniFile::Read(const char* section, const char* key) {
  UInt32 i;
  Int32 s;
  s = _findSection(section);
  if (s < 0)
    throw InvalidOpException("Section not found");
  i = _findEntry(s, key);
  if (i < 0) throw InvalidOpException("Key not found");
  return new String(entryValue[i]);
  }

String *IniFile::Read(const char* section, const char* key, const char* def) {
  try {
    return Read(section, key);
    }
  catch (...) {
    return new String(def);
    }
  }

Int32 IniFile::ReadInteger(const char* section, const char* key) {
  Int32 i;
  Int32 s;
  s = _findSection(section);
  if (s < 0)
    throw InvalidOpException("Section not found");
  i = _findEntry(s, key);
  if (i < 0) throw InvalidOpException("Key not found");
  return Convert::ToInt32(entryValue[i]);
  }

Int32 IniFile::ReadInteger(const char* section, const char* key, Int32 def) {
  try {
    return ReadInteger(section, key);
    }
  catch (...) {
    return def;
    }
  }

Double IniFile::ReadDouble(const char* section, const char* key) {
  Int32 i;
  Int32 s;
  s = _findSection(section);
  if (s < 0)
    throw InvalidOpException("Section not found");
  i = _findEntry(s, key);
  if (i < 0) throw InvalidOpException("Key not found");
  return Convert::ToDouble(entryValue[i]);
  }

Double IniFile::ReadDouble(const char* section, const char* key, Double def) {
  try {
    return ReadDouble(section, key);
    }
  catch (...) {
    return def;
    }
  }

Boolean IniFile::ReadBoolean(const char* section, const char* key) {
  Int32 i;
  Int32 s;
  s = _findSection(section);
  if (s < 0)
    throw InvalidOpException("Section not found");
  i = _findEntry(s, key);
  if (i < 0) throw InvalidOpException("Key not found");
  return Convert::ToBoolean(entryValue[i]);
  }

Boolean IniFile::ReadBoolean(const char* section, const char* key, Boolean def) {
  try {
    return ReadBoolean(section, key);
    }
  catch (...) {
    return def;
    }
  }

void IniFile::Write(const char* section, const char* key, const char* value) {
  Int32 v;
  Int32 s;
  s = _findSection(section);
  if (s < 0) {
    numSections++;
    if (numSections == 1)
      sections = (char**)malloc(sizeof(char*));
    else
      sections = (char**)realloc(sections, sizeof(char*) * numSections);
    if (sections == NULL)
      throw MemoryException("Could not allocate memory");
    sections[numSections-1] = (char*)malloc(strlen(section)+1);
    if (sections[numSections-1] == NULL)
      throw MemoryException("Could not allocate memory");
    strcpy(sections[numSections-1], section);
    s = numSections - 1;
    }
  v = _findEntry(s, key);
  if (v < 0) {
    numEntries++;
    if (numEntries == 1) {
      entrySection = (UInt32*)malloc(sizeof(UInt32));
      entryKey = (char**)malloc(sizeof(char*));
      entryValue = (char**)malloc(sizeof(char*));
      }
    else {
      entrySection = (UInt32*)realloc(entrySection, sizeof(UInt32) * numEntries);
      entryKey = (char**)realloc(entryKey, sizeof(char*) * numEntries);
      entryValue = (char**)realloc(entryValue, sizeof(char*) * numEntries);
      }
    if (entrySection == NULL || entryKey == NULL || entryValue == NULL)
      throw MemoryException("Could not allocate memory");
    entrySection[numEntries-1] = s;
    entryKey[numEntries-1] = (char*)malloc(strlen(key)+1);
    if (entryKey[numEntries-1] == NULL)
      throw MemoryException("Could not allocate memory");
    strcpy(entryKey[numEntries-1], key);
    entryValue[numEntries-1] = (char*)malloc(strlen(key)+1);
    if (entryValue[numEntries-1] == NULL)
      throw MemoryException("Could not allocate memory");
    strcpy(entryValue[numEntries-1], value);
    }
  else {
    free(entryValue[v]);
    entryValue[v] = (char*)malloc(strlen(value) + 1);
    if (entryValue[v] == NULL)
      throw MemoryException("Could not allocate memory");
    strcpy(entryValue[v], value);
    }
  modified = true;
  }

void IniFile::Write(const char* section, const char* key, UInt32 value) {
  char buffer[32];
  sprintf(buffer,"%u",value);
  Write(section, key, buffer);
  }

void IniFile::Write(const char* section, const char* key, Int32 value) {
  char buffer[32];
  sprintf(buffer,"%d",value);
  Write(section, key, buffer);
  }

void IniFile::Write(const char* section, const char* key, Double value) {
  char buffer[32];
  sprintf(buffer,"%f",value);
  Write(section, key, buffer);
  }

void IniFile::Write(const char* section, const char* key, Boolean value) {
  if (value) Write(section, key, "true");
    else Write(section, key, "false");
  }


