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
#include <pwd.h>
#include <grp.h>
#include <sys/utsname.h>
#include "SmrFramework.h"

namespace SmrFramework {

  System::System() {
    objectType = (char*)"System";
    }

  System::~System() {
    }

  String System::DomainName() {
    char buffer[1024];
    getdomainname(buffer, 1024);
    return String(buffer);
    }

  int System::GroupID() {
    return getgid();
    }

  String System::GroupName() {
    struct group *group;
    group = getgrgid(getgid());
    return String(group->gr_name);
    }

  String System::GroupName(int groupID) {
    struct group *group;
    group = getgrgid(groupID);
    if (group == NULL) return String();
    return String(group->gr_name);
    }

  List<int>* System::Groups() {
    int i;
    gid_t groups[1000];
    int   numGroups;
    List<int>* ret;
    ret = new List<int>();
    numGroups = getgroups(1000, groups);
    for (i=0; i<numGroups; i++)
      ret->Add(groups[i]);
    return ret;
    }

  String System::HostName() {
    char buffer[1024];
    gethostname(buffer, 1024);
    return String(buffer);
    }

  String System::Machine() {
    struct utsname uts;
    uname(&uts);
    return String(uts.machine);
    }

  List<String*>* System::Members(int groupID) {
    int i;
    List<String*>* ret;
    struct group *group;
    group = getgrgid(groupID);
    if (group == NULL) return NULL;
    ret = new List<String*>();
    i = 0;
    while (group->gr_mem[i] != NULL)
      ret->Add(new String(group->gr_mem[i]));
    return ret;
    }

  String System::Release() {
    struct utsname uts;
    uname(&uts);
    return String(uts.release);
    }

  String System::OperatingSystem() {
    struct utsname uts;
    uname(&uts);
    return String(uts.sysname);
    }

  int System::UserID() {
    return getuid();
    }

  String System::UserName() {
    struct passwd *user;
    user = getpwuid(getuid());
    return String(user->pw_name);
    }

  String System::Version() {
    struct utsname uts;
    uname(&uts);
    return String(uts.version);
    }

  }

