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
#include <string.h>
#include <stdlib.h>
#include "SmrFramework.h"

namespace SmrFramework {

  String::String() {
    objectType = (char*)"String";
    pos = 0;
    length = 0;
    string = (Byte*)malloc(1);
    string[0] = 0;
    }

  String::String(const char* s) {
    objectType = (char*)"String";
    string = NULL;
    _setValue((Byte*)s, (s != NULL) ? strlen(s) : 0);
    }

  String::String(Byte* s) {
    objectType = (char*)"String";
    string = NULL;
    _setValue(s, (s != NULL) ? strlen((char*)s) : 0);
    }

  String::String(const char* s, UInt32 len) {
    objectType = (char*)"String";
    string = NULL;
    _setValue((Byte*)s, len);
    }

  String::String(Byte* s, UInt32 len) {
    objectType = (char*)"String";
    string = NULL;
    _setValue(s, len);
    }

  String::String(String* s) {
    objectType = (char*)"String";
    string = NULL;
    _setValue((Byte*)s->AsCharArray(), s->Length());
    }
  
  String::String(const String &s) {
    objectType = (char*)"String";
    string = NULL;
    _setValue(s.string, s.length);
    }
  
  String::~String() {
    if (string != NULL) free(string);
    }
  
  void String::_setValue(Byte* s, UInt32 l) {
    if (string != NULL) free(string);
    string = NULL;
    if (s != NULL) {
      string = (Byte*)malloc(l+1);
      memcpy(string, s, l);
    } else {
      l = 0;
      string = (Byte*)malloc(1);
      }
    string[l] = 0;
    length = l;
    size = l;
    pos = 0;
    }

  String& String::operator = (const String &s) {
    _setValue(s.string, s.length);
    return *this;
    }
  
  void String::operator = (const char* s) {
    _setValue((Byte*)s, strlen(s));
    }
  
  bool String::operator == (const String &s) {
    return Equals(s);
    }

  bool String::operator == (const char* s) {
    return Equals(s);
    }
  
  bool String::operator != (const String &s) {
    return !Equals(s);
    }

  bool String::operator != (const char* s) {
    return !Equals(s);
    }

  bool String::operator > (const String &s) {
    return (CompareTo(s) > 0) ? true : false;
    }

  bool String::operator > (const char* s) {
    return (CompareTo(s) > 0) ? true : false;
    }

  bool String::operator >= (const String &s) {
    return (CompareTo(s) >= 0) ? true : false;
    }

  bool String::operator >= (const char* s) {
    return (CompareTo(s) >= 0) ? true : false;
    }

  bool String::operator < (const String &s) {
    return (CompareTo(s) < 0) ? true : false;
    }

  bool String::operator < (const char* s) {
    return (CompareTo(s) < 0) ? true : false;
    }

  bool String::operator <= (const String &s) {
    return (CompareTo(s) <= 0) ? true : false;
    }

  bool String::operator <= (const char* s) {
    return (CompareTo(s) <= 0) ? true : false;
    }

  String String::operator + (char c) {
    return Append(c);
    }

  String String::operator + (const char* s) {
    return Append(s);
    }

  String String::operator + (String* s) {
    return Append(s);
    }

  String String::operator + (String s) {
    return Append(s);
    }

  Byte &String::operator [] (UInt32 i) {
    if (i > Length()) throw RangeException(this, "Index out of range");
    return string[i];
    }

  String String::Append(char c) {
    Byte*   temp;
    String ret;
    int     len;
    len = (string == NULL) ? 0 : length;
    temp = (Byte*)malloc(len + 2);
    if (string != NULL) memcpy(temp,string,length);
    temp[len] = c;
    temp[len+1] = 0;
    ret = String(temp);
    free(temp);
    return ret;
    }

  String String::Append(const char* s) {
    UInt32  i;
    Byte*   temp;
    UInt32  len1;
    UInt32  len2;
    String ret;
    len1 = (string == NULL) ? 0 : length;
    len2 = (s == NULL) ? 0 : strlen(s);
    temp = (Byte*)malloc(len1 + len2 + 1);
    for (i=0; i<len1; i++) temp[i] = string[i];
    for (i=0; i<len2; i++) temp[i+len1] = s[i];
    temp[len1 + len2] = 0;
    ret = String(temp);
    free(temp);
    return ret;
    }

  String String::Append(String* str) {
    UInt32  i;
    Byte*   temp;
    char*   s;
    UInt32  len1;
    UInt32  len2;
    String ret;
    len1 = (string == NULL) ? 0 : length;
    len2 = (str == NULL) ? 0 : str->Length();
    s = str->AsCharArray();
    temp = (Byte*)malloc(len1 + len2 + 1);
    for (i=0; i<len1; i++) temp[i] = string[i];
    for (i=0; i<len2; i++) temp[i+len1] = s[i];
    temp[len1 + len2] = 0;
    ret = String(temp);
    free(temp);
    return ret;
    }

  String String::Append(String str) {
    UInt32  i;
    Byte*   temp;
    char*   s;
    UInt32  len1;
    UInt32  len2;
    String ret;
    len1 = (string == NULL) ? 0 : length;
    len2 = str.Length();
    s = str.AsCharArray();
    temp = (Byte*)malloc(len1 + len2 + 1);
    for (i=0; i<len1; i++) temp[i] = string[i];
    for (i=0; i<len2; i++) temp[i+len1] = s[i];
    temp[len1 + len2] = 0;
    ret = String(temp);
    free(temp);
    return ret;
    }

  char* String::AsCharArray() {
    return (char*)string;
    }

  char* String::ToCharArray() {
    char* ret;
    if (string == NULL) return NULL;
    ret = (char*)malloc(length + 1);
    memcpy(ret, string, length);
    ret[length] = 0;
    return ret;
    }

  char String::At(UInt32 pos) {
    if (string == NULL) return 0;
    if (pos >= length) return 0;
    return string[pos];
    }

  char String::CharAt(UInt32 pos) {
    if (string == NULL) return 0;
    if (pos >= length) return 0;
    return string[pos];
    }

  void String::Clear() {
    if (string != NULL) free(string);
    string = NULL;
    }

  int String::CompareTo(const char* s) {
    UInt32 i;
    if (string == NULL && s == NULL) return 0;
    if (string == NULL) return -1;
    if (s == NULL) return 1;
    if (length == 0) return 0;
    for (i=0; i<length; i++) {
      if (string[i] > s[i]) return 1;
      if (string[i] < s[i]) return -1;
      }
    if (s[length] == 0) return 0;
    return -1;
    }
  
  int String::CompareTo(String* s) {
    if (s == NULL) return 0;
    if (s->Length() == 0) return 0;
    return CompareTo(s->AsCharArray());
    }
  
  int String::CompareTo(String s) {
    if (s.Length() == 0) return 0;
    return CompareTo(s.AsCharArray());
    }

  bool String::Contains(char c) {
    if (string == NULL) return false;
    if (IndexOf(c) >= 0) return true;
    return false;
    }

  UInt32 String::Count(char c) {
    UInt32 ret;
    UInt32 len;
    UInt32 i;
    if (string == NULL) return 0;
    ret = 0;
    len = Length();
    for (i=0; i<len; i++)
      if (string[i] == c) ret++;
    return ret;
    }

  bool String::Contains(const char* s) {
    if (string == NULL) return false;
    if (s == NULL) return false;
    if (IndexOf(s) >= 0) return true;
    return false;
    }

  bool String::Contains(String* s) {
    if (string == NULL) return false;
    if (s == NULL) return false;
    if (IndexOf(s) >= 0) return true;
    return false;
    }

  bool String::Contains(String s) {
    if (string == NULL) return false;
    if (s.Length() == 0) return false;
    if (IndexOf(s) >= 0) return true;
    return false;
    }

  void String::Dispose() {
    if (string != NULL) free(string);
    string = NULL;
    }

  bool String::EndsWith(const char* s) {
    UInt32 p;
    p = length - strlen(s);
    if (p < 0) return false;
    while (*s != 0) {
      if (p >= length) return false;
      if (string[p++] != *s++) return false;
      }
    return true;
    }

  bool String::EndsWith(String* s) {
    if (s == NULL) return false;
    return EndsWith(s->AsCharArray());
    }

  bool String::EndsWith(String s) {
    if (s.Length() == 0) return false;
    return EndsWith(s.AsCharArray());
    }

  bool String::Equals(const char* s) {
    UInt32 i;
    if (string == NULL && s == NULL) return true;
    if (string == NULL) return false;
    if (s == NULL) return false;
    for (i=0; i<length; i++)
      if (string[i] != s[i]) return false;
    if (s[length] != 0) return false;
    return true;
    }

  bool String::Equals(String* s) {
    if (s == NULL) return false;
    if (s->Length() == 0) return false;
    return Equals(s->AsCharArray());
    }

  bool String::Equals(String s) {
    if (s.Length() == 0) return false;
    return Equals(s.AsCharArray());
    }

  char String::First() {
    pos = 0;
    return Next();
    }

  int String::IndexOf(char c) {
    UInt32 i;
    for (i=0; i<length; i++)
      if (string[i] == c) return i;
    return -1;
    }
  
  int String::IndexOf(const char* s) {
    UInt32 i;
    UInt32 j;
    UInt32 len;
    Boolean flag;
    len = strlen(s);
    for (i=0; i<length-len; i++) {
      flag = true;
      for (j=0; j<len; j++)
        if (string[i+j] != s[j]) flag = false;
      if (flag) return i;
      }
    return -1;
    }

  int String::IndexOf(String* s) {
    if (s == NULL) return -1;
    if (s->Length() == 0) return -1;
    return IndexOf(s->AsCharArray());
    }

  int String::IndexOf(String s) {
    if (s.Length() == 0) return -1;
    return IndexOf(s.AsCharArray());
    }
  
  char String::Last() {
    pos = length - 1;
    if (pos < 0) pos = 0;
    return Next();
    }

  int String::LastIndexOf(char c) {
    int i;
    for (i=length-1; i>=0; i--)
      if (string[i] == c) return i;
    return -1;
    }
  
  int String::LastIndexOf(const char* s) {
    UInt32 i;
    UInt32 j;
    UInt32 len;
    Boolean flag;
    len = strlen(s);
    for (i=length-len; i>=0; i--) {
      flag = true;
      for (j=0; j<len; j++)
        if (string[i+j] != s[j]) flag = false;
      if (flag) return i;
      }
    return -1;
    }

  int String::LastIndexOf(String* s) {
    if (s == NULL) return -1;
    if (s->Length() == 0) return -1;
    return LastIndexOf(s->AsCharArray());
    }

  int String::LastIndexOf(String s) {
    if (s.Length() == 0) return -1;
    return LastIndexOf(s.AsCharArray());
    }

  UInt32 String::Length() {
    if (string == NULL) return 0;
    return length;
    }

  char String::Next() {
    if (pos >= length) return 0;
    return string[pos++];
    }

  String String::PadLeft(UInt32 len) {
    return PadLeft(len, ' ');
    }

  String String::PadLeft(UInt32 len, char c) {
    UInt32 i;
    UInt32 j;
    UInt32 l;
    Byte* temp;
    String ret;
    if (Length() >= len) return String(string);
    temp = (Byte*)malloc(len + 1);
    l = Length();
    i = 0;
    while (i < (len - l)) {
      temp[i++] = c;
      }
    for (j=0; j<length; j++) temp[i++] = string[j];
    temp[i] = 0;
    ret = String(temp);
    free(temp);
    return ret;
    }

  String String::PadRight(UInt32 len) {
    return PadRight(len, ' ');
    }

  String String::PadRight(UInt32 len, char c) {
    UInt32 i;
    UInt32 p;
    Byte* temp;
    String ret;
    if (Length() >= len) return String(string);
    temp = (Byte*)malloc(len + 1);
    p = 0;
    for (i=0; i<length; i++) temp[p++] = string[i];
    while (p < len) temp[p++] = c;
    temp[p] = 0;
    ret = String(temp);
    free(temp);
    return ret;
    }

  String String::Reverse() {
    String ret;
    int     len;
    int     i;
    Byte*   temp;
    if (string == NULL) return new String((char*)NULL);
    len = length;
    temp = (Byte*)malloc(len + 1);
    for (i=0; i<len; i++)
      temp[i] = string[len-i-1];
    temp[len] = 0;
    ret = String(temp);
    free(temp);
    return ret;
    }

  String String::Rot13() {
    String ret;
    Byte*   temp;
    UInt32  i;
    if (string == NULL) return new String((char*)NULL);
    temp = (Byte*)malloc(length + 1);
    memcpy(temp, string, length);
    for (i=0; i<length; i++) {
      if (temp[i] >= 'A' && temp[i] <= 'Z') {
        temp[i] += 13;
        if (temp[i] > 'Z') temp[i] -= 26;
        }
      if (temp[i] >= 'a' && temp[i] <= 'z') {
        temp[i] += 13;
        if (temp[i] > 'z') temp[i] -= 26;
        }
      }
    temp[length] = 0;
    ret = String((char*)temp);
    free(temp);
    return ret;
    }

  void String::SetValue(const char* s) {
    if (string != NULL) free(string);
    string = NULL;
    if (s == NULL) string = NULL;
    else {
      _setValue((Byte*)s, strlen(s));
      }
    }

  List<String*>* String::Split(char c) {
    UInt32 s, e;
    UInt32 len;
    List<String*>* ret;
    if (string == NULL) return NULL;
    ret = new List<String*>();
    s = 0;
    e = 0;
    len = Length();
    while (e < len) {
      if (string[e] == c) {
        if (e == s) ret->Add(new String(""));
          else ret->Add(new String(string+s, (e-s)));
        s = e+1;
        }
      e++;
      }
    if (e > s) ret->Add(new String(string+s, (e-s)));
    return ret;
    }

  bool String::StartsWith(const char* s) {
    UInt32 p;
    p = 0;
    while (*s != 0) {
      if (p >= length) return false;
      if (string[p++] != *s++) return false;
      }
    return true;
    }

  bool String::StartsWith(String* s) {
    if (s == NULL) return false;
    return StartsWith(s->AsCharArray());
    }

  bool String::StartsWith(String s) {
    if (s.Length() == 0) return false;
    return StartsWith(s.AsCharArray());
    }

  String String::Substring(int start) {
    UInt32  i;
    UInt32  count;
    Byte*   temp;
    String ret;
    if (start < 0) return ret;
    if ((UInt32)start >= Length()) return ret;
    count = Length() - start;
    temp = (Byte*)malloc(sizeof(char)*count + 1);
    for (i=0; i<count; i++) temp[i] = string[start+i];
    temp[count] = 0;
    ret = String(temp);
    free(temp);
    return ret;
    }

  String String::Substring(int start, int len) {
    UInt32  i;
    Byte*   temp;
    String ret;
    if (start < 0) return String();
    if (len < 0) return String();
    if (len == 0) return String("");
    if ((UInt32)start >= Length()) return String();
    if ((UInt32)start + (UInt32)len >= Length()) len = Length() - start;
    temp = (Byte*)malloc(sizeof(char)*len + 1);
    for (i=0; i<(UInt32)len; i++) temp[i] = string[start+i];
    temp[len] = 0;
    ret = String(temp);
    free(temp);
    return ret;
    }

  String String::ToLower() {
    UInt32  i;
    Byte*   temp;
    String ret;
    if (Length() == 0) return String();
    temp = (Byte*)malloc(length + 1);
    memcpy(temp, string, length);
    temp[length] = 0;
    for (i=0; i<length; i++)
      if (temp[i] >= 'A' && temp[i] <= 'Z') temp[i] += 32;
    ret = String(temp);
    free(temp);
    return ret;
    }

  String String::ToString() {
    return String(string);
    }

  String String::ToUpper() {
    UInt32  i;
    Byte*   temp;
    String ret;
    if (Length() == 0) return String();
    temp = (Byte*)malloc(length + 1);
    memcpy(temp, string, length);
    temp[length] = 0;
    for (i=0; i<length; i++)
      if (temp[i] >= 'a' && temp[i] <= 'z') temp[i] -= 32;
    ret = String(temp);
    free(temp);
    return ret;
    }

  String String::Trim() {
    UInt32  i;
    Byte*   temp;
    UInt32  start;
    UInt32  end;
    String ret;
    if (Length() == 0) return String();
    start = 0;
    while (string[start] == ' ' || string[start] == '\t') start++;
    if (start >= length) return String();
    end = length - 1;
    while (end > 0 && (string[end] == ' ' || string[end] == '\t')) end--;
    temp = (Byte*)malloc(end-start + 2);
    for (i=0; i<=(end-start); i++) temp[i] = string[start+i];
    temp[end-start+1] = 0;
    ret = String(temp);
    free(temp);
    return ret;
    }

  String String::TrimLeft() {
    UInt32  i;
    Byte*   temp;
    UInt32  start;
    UInt32  end;
    String ret;
    if (Length() == 0) return String();
    start = 0;
    while (string[start] == ' ' || string[start] == '\t') start++;
    if (start >= length) return String();
    end = length - 1;
    temp = (Byte*)malloc(end-start + 2);
    for (i=0; i<=(end-start); i++) temp[i] = string[start+i];
    temp[end-start+1] = 0;
    ret = String(temp);
    free(temp);
    return ret;
    }

  String String::TrimRight() {
    UInt32  i;
    Byte*   temp;
    UInt32  start;
    UInt32  end;
    String ret;
    if (Length() == 0) return String();
    start = 0;
    end = length - 1;
    while (end > 0 && (string[end] == ' ' || string[end] == '\t')) end--;
    if (end == 0) return String();
    temp = (Byte*)malloc(end-start + 2);
    for (i=0; i<=(end-start); i++) temp[i] = string[start+i];
    temp[end-start+1] = 0;
    ret = String(temp);
    free(temp);
    return ret;
    }

  }
