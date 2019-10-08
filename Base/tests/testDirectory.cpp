#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestDirectory() {
  UInt32 i;
  List<String*>* files;
  String*        file;
  printf("\nInfo: Directory tests\n");
  files = Directory::GetFiles((char*)".");
  printf("Info: Number of files: %d\n",files->Count());
  file = files->First();
  printf("      %s\n",file->AsCharArray());
  while ((file = files->Next()) != NULL) {
    printf("      %s\n",file->AsCharArray());
    }
  for (i=0; i<files->Count(); i++)
    delete(files->At(i));
  delete(files);

  try {
    files = Directory::GetFiles((char*)"./xx1245ds");
    Bad("Did not throw exception when path does not exist");
    }
  catch (InvalidOpException &e) {
    Good("Threw InvalidOpException when path does not exist");
    }
  catch (...) {
    Bad("Wrong exception was thrown");
    }

  file = Directory::GetCurrentDirectory();
  printf("Info: Current directory : %s\n",file->AsCharArray());

  if (Directory::Exists((char*)"./testdir") == 0)
    Good("Exists returned false for non-existant directory");
    else Bad("Exists returned true for non-existant directory");

  try {
    Directory::CreateDirectory((char*)"./testdir");
    Good("Was able to create new directory");
    }
  catch (...) {
    Bad("Exception thrown trying to create a directory");
    }

  if (Directory::Exists((char*)"./testdir") != 0)
    Good("Exists returned true for existing directory");
    else Bad("Exists returned false for existing directory");

  try {
    Directory::CreateDirectory((char*)"./testdir");
    Bad("Was able to create directory over existing directory");
    }
  catch (...) {
    Good("Exception thrown trying to create a duplicate directory");
    }

  try {
    Directory::SetCurrentDirectory((char*)"./testdir");
    Good("Was able to change into directory");
    }
  catch (...) {
    Bad("Exception thrown changing to existing directory");
    }

  file = Directory::GetCurrentDirectory();
  printf("Info: Current directory : %s\n",file->AsCharArray());

  try {
    Directory::SetCurrentDirectory((char*)"./testdir");
    Bad("Exception was not thrown chaning to non-existant directory");
    }
  catch (...) {
    Good("Exception thrown changing to non-existant directory");
    }

  try {
    Directory::SetCurrentDirectory((char*)"..");
    Good("Was able to change into parent directory");
    }
  catch (...) {
    Bad("Exception thrown changing to parent directory");
    }

  file = Directory::GetCurrentDirectory();
  printf("Info: Current directory : %s\n",file->AsCharArray());

  try {
    Directory::Delete((char*)"./testdir");
    Good("Was able to remove directory");
    }
  catch (...) {
    Bad("Exception thrown trying to remove directory");
    }

  try {
    Directory::Delete((char*)"./testdir");
    Bad("Exception not thrown tyring to remove non-existant directory");
    }
  catch (...) {
    Good("Exception thrown trying to remove non-existant directory");
    }

  }
