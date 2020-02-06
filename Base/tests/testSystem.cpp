#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestSystem() {
  UInt32 i;
  String str;
  String* pstr;
  List<int>* groups;
  List<String*>* members;
  printf("\nInfo: System tests\n");
  printf("Info: User ID     : %d\n",System::UserID());
  printf("Info: Group ID    : %d\n",System::GroupID());
  str = System::UserName();
  printf("Info: User Name   : %s\n",str.AsCharArray());
  str = System::GroupName();
  printf("Info: Group Name  : %s\n",str.AsCharArray());
  members = System::Members(System::GroupID());
  printf("Info: Members     :");
  pstr = members->First();
  while (pstr != NULL) {
    printf(" %s",pstr->AsCharArray());
    pstr = members->Next();
    }
  printf("\n");
  for (i=0; i<members->Count(); i++) delete(members->At(i));  
  delete(members);

  printf("Info: Groups      :");
  groups = System::Groups();
  for (i=0; i<groups->Count(); i++) {
    str = System::GroupName(groups->At(i));
    printf(" %s", str.AsCharArray());
    }
  printf("\n");
  delete(groups);


  str = System::HostName();
  printf("Info: Host Name   : %s\n",str.AsCharArray());
  str = System::DomainName();
  printf("Info: Domain Name : %s\n",str.AsCharArray());
  str = System::OperatingSystem();
  printf("Info: OS          : %s\n",str.AsCharArray());
  str = System::Release();
  printf("Info: Release     : %s\n",str.AsCharArray());
  str = System::Version();
  printf("Info: Version     : %s\n",str.AsCharArray());
  str = System::Machine();
  printf("Info: Machine     : %s\n",str.AsCharArray());
  }
