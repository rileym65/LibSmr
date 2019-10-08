#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestFileInfo() {
  FileInfo*   file;
  DateTime*   dt;
  printf("\nInfo: FileInfo tests\n");
  file = new FileInfo((char*)"test");
  if (file != NULL) Good("Was able to instantiate a FileInfo");
    else Bad("Could not instantiate an FileInfo");
  if (strcmp(file->Type(), "FileInfo") == 0)
    Good("DateTime reported correctly on Type");
    else Bad("DateTime did not report correctly on Type");

  printf("Info: File owner id: %d\n", file->Owner());
  printf("Info: File group id: %d\n", file->Group());
  printf("Info: File device  : %d\n", file->Device());
  printf("Info: File Inode   : %d\n", file->Inode());
  printf("Info: File links   : %d\n", file->Links());
  printf("Info: File size    : %d\n", file->Size());
  printf("Info: Block size   : %d\n", file->BlockSize());
  printf("Info: Block count  : %d\n", file->BlockCount());
  printf("Info: File type    : %d\n", file->FileType());
  printf("Info: Owner read   : %d\n", file->OwnerRead());
  printf("Info: Owner write  : %d\n", file->OwnerWrite());
  printf("Info: Owner exec   : %d\n", file->OwnerExecute());
  printf("Info: Group read   : %d\n", file->GroupRead());
  printf("Info: Group write  : %d\n", file->GroupWrite());
  printf("Info: Group exec   : %d\n", file->GroupExecute());
  printf("Info: Other read   : %d\n", file->OtherRead());
  printf("Info: Other write  : %d\n", file->OtherWrite());
  printf("Info: Other exec   : %d\n", file->OtherExecute());
  dt = file->LastAccessTime();
  printf("Info: Last Access  : %02d/%02d/%04d %02d:%02d:%02d\n",
    dt->Month(), dt->Day(), dt->Year(), dt->Hour(), dt->Minute(), dt->Second());
  dt = file->LastModificationTime();
  printf("Info: Last Change  : %02d/%02d/%04d %02d:%02d:%02d\n",
    dt->Month(), dt->Day(), dt->Year(), dt->Hour(), dt->Minute(), dt->Second());
  dt = file->LastStatusTime();
  printf("Info: Last Status  : %02d/%02d/%04d %02d:%02d:%02d\n",
    dt->Month(), dt->Day(), dt->Year(), dt->Hour(), dt->Minute(), dt->Second());


  delete(file);
  }
