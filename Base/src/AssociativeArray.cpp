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

  AssociativeArray::AssociativeArray() {
    keys = new List<String*>();
    values = new List<Object*>();
    objectType = (char*)"AssociativeArray";
    }
  
  AssociativeArray::~AssociativeArray() {
    UInt32 i;
    for (i=0; i<keys->Count(); i++)
      delete(keys->At(i));
    for (i=0; i<values->Count(); i++)
      delete(values->At(i));
    delete(keys);
    delete(values);
    }
  
  void AssociativeArray::Add(char *s, Object* o) {
    keys->Add(new String(s));
    values->Add(o);
    }
  
  void AssociativeArray::Add(String s, Object* o) {
    keys->Add(new String(s));
    values->Add(o);
    }
  
  void AssociativeArray::Add(String* s, Object* o) {
    keys->Add(new String(s));
    values->Add(o);
    }

  UInt32  AssociativeArray::Count() {
    return keys->Count();
    }

  Object* AssociativeArray::At(char* s) {
    UInt32 i;
    for (i=0; i<keys->Count(); i++)
      if (keys->At(i)->Equals(s)) return values->At(i);
    return NULL;
    }

  Object* AssociativeArray::At(String s) {
    return At(s.AsCharArray());
    }

  Object* AssociativeArray::At(String *s) {
    return At(s->AsCharArray());
    }

  void    AssociativeArray::AtPut(char* s, Object* o) {
    UInt32 i;
    for (i=0; i<keys->Count(); i++)
      if (keys->At(i)->Equals(s)) {
        delete(values->At(i));
        values->AtPut(i, o);
        return;
        }
    throw RangeException("Index not found");
    }

  void    AssociativeArray::AtPut(String s, Object* o) {
    AtPut(s.AsCharArray(), o);
    }

  void    AssociativeArray::AtPut(String *s, Object* o) {
    AtPut(s->AsCharArray(), o);
    }

  }

