#include <stdio.h>
#include <string.h>
#include "SmrFramework.h"

using namespace SmrFramework;

/* *************************************** */
/* ***** Base object of JSON objects ***** */
/* *************************************** */
JsonBase::JsonBase() {
  jsonType = 0;
  }

JsonBase::~JsonBase() {
  }

Byte JsonBase::JsonType() {
  return jsonType;
  }

String* JsonBase::ToString() {
  return new String("");
  }


/* ********************** */
/* ***** JSON Array ***** */
/* ********************** */

JsonArray::JsonArray() {
  jsonType = Json::ARRAY;
  items = new List<JsonBase*>();
  }

JsonArray::JsonArray(const char* s) : JsonBase() {
  char value[65536];
  Boolean quote;
  Boolean dot;
  int  pos;
  int  count;
  jsonType = Json::ARRAY;
  items = new List<JsonBase*>();
  while (*s > 0 && *s <= ' ') s++;
  if (*s != '[')
    throw InvalidOpException("String did not start with [");
  s++;
  while (*s != 0 && *s != ']') {
    while (*s > 0 && *s <= ' ') s++;
    if (strncasecmp(s, "true", 4) == 0) {
      s += 4;
      if (*s != ' ' && *s != '\t' && *s != ',' && *s != ']')
        throw InvalidOpException("Invalid boolean value");
      this->Add(true);
      }
    else if (strncasecmp(s, "false", 5) == 0) {
      s += 5;
      if (*s != ' ' && *s != '\t' && *s != ',' && *s != ']')
        throw InvalidOpException("Invalid boolean value");
      this->Add(false);
      }
    else if (strncasecmp(s, "null", 4) == 0) {
      s += 4;
      if (*s != ' ' && *s != '\t' && *s != ',' && *s != ']')
        throw InvalidOpException("Invalid null value");
      this->Add(new JsonNull());
      }
    else if (*s == '[') {
      count = 1;
      pos = 0;
      value[pos++] = *s++;
      while (*s != 0 && count > 0) {
        if (*s == '[') {
          s++;
          count++;
          }
        else if (*s == ']') {
          value[pos++] = *s;
          if (count > 1) s++;
          count--;
          }
        else value[pos++] = *s++;
        }
      value[pos] = 0;
      if (*s != ']') 
        throw InvalidOpException("Unexpected end of string");
      s++;
      this->Add(new JsonArray(value));
      }
    else if (*s == '{') {
      count = 1;
      pos = 0;
      value[pos++] = *s++;
      while (*s != 0 && count > 0) {
        if (*s == '{') {
          s++;
          count++;
          }
        else if (*s == '}') {
          value[pos++] = *s;
          if (count > 1) s++;
          count--;
          }
        else value[pos++] = *s++;
        }
      value[pos] = 0;
      if (*s != '}') 
        throw InvalidOpException("Unexpected end of string");
      s++;
      this->Add(new Json(value));
      }
    else if (*s == '"') {
      s++;
      pos = 0;
      while (*s != 0 && *s != '"') value[pos++] = *s++;
      if (*s == 0)
        throw InvalidOpException("Unexpected end of string");
      s++;
      value[pos] = 0;
      this->Add(value);
      }
    else {
      if (*s == '+' || *s == '-' || *s == '.' ||
          (*s >= '0' && *s <= '9')) {
        pos = 0;
        dot = false;
        if (*s == '+') s++;
        else if (*s == '-') value[pos++] = *s++;
        else if (*s == '.') {
          value[pos++] = *s++;
          dot = true;
          }
        while ((*s >= '0' && *s <= '9') || *s == '.') {
          if (*s == '.' && dot == false) {
            value[pos++] = *s++;
            dot = true;
            }
          else if (*s == '.')
            throw InvalidOpException("Multiple periods");
          else if (*s >= '0' && *s <= '9') value[pos++] = *s++;
          }
        value[pos] = 0;
        if (dot) {
          this->Add(atof(value));
          }
        else {
          this->Add(atoi(value));
          }
        }
      else {
        throw InvalidOpException("Unexpected character");
        }
      }
    while (*s > 0 && *s <= ' ') s++;
    if (*s == 0)
      throw InvalidOpException("Unexpected end of string");
    if (*s != ',' && *s != ']')
      throw InvalidOpException("Unexpected character");
    if (*s == ',') s++;
    }
  }

JsonArray::~JsonArray() {
  int i;
  for (i=0; i<items->Count(); i++)
    delete(items->At(i));
  delete(items);
  }

void JsonArray::Add(Boolean value) {
  items->Add(new JsonBoolean(value));
  }

void JsonArray::Add(Int32 value) {
  items->Add(new JsonNumber(value));
  }

void JsonArray::Add(Double value) {
  items->Add(new JsonNumber(value));
  }

void JsonArray::Add(const char* value) {
  items->Add(new JsonString(value));
  }

void JsonArray::Add(String value) {
  items->Add(new JsonString(value));
  }

void JsonArray::Add(String* value) {
  items->Add(new JsonString(value));
  }

void JsonArray::Add(JsonBase *obj) {
  items->Add(obj);
  }

JsonBase* JsonArray::At(UInt32 i) {
  if (i >= items->Count()) return NULL;
  return items->At(i);
  }

UInt32 JsonArray::Count() {
  return items->Count();
  }

String* JsonArray::ToString() {
  int i;
  String *item;
  char buffer[65536];
  strcpy(buffer,"[");
  for (i=0; i<items->Count(); i++) {
    if (i!= 0) strcat(buffer,",");
    item = items->At(i)->ToString();
    strcat(buffer,item->AsCharArray());
    delete(item);
    }
  strcat(buffer,"]");
  return new String(buffer);
  }


/* ************************ */
/* ***** JSON Boolean ***** */
/* ************************ */

JsonBoolean::JsonBoolean(Boolean b) : JsonBase() {
  jsonType = Json::BOOLEAN;
  value = b;
  }

JsonBoolean::JsonBoolean(char *b) : JsonBase() {
  jsonType = Json::BOOLEAN;
  if (strcasecmp(b, "true") == 0) value = true;
    else value = false;
  }

JsonBoolean::JsonBoolean(String b) : JsonBase() {
  jsonType = Json::BOOLEAN;
  if (strcasecmp(b.AsCharArray(), "true") == 0) value = true;
    else value = false;
  }

JsonBoolean::JsonBoolean(String *b) : JsonBase() {
  jsonType = Json::BOOLEAN;
  if (strcasecmp(b->AsCharArray(), "true") == 0) value = true;
    else value = false;
  }

JsonBoolean::~JsonBoolean() {
  }

String* JsonBoolean::ToString() {
  if (value) return new String("true");
  return new String("false");
  }

Boolean JsonBoolean::Value() {
  return value;
  }


/* ********************* */
/* ***** JSON Null ***** */
/* ********************* */

JsonNull::JsonNull() : JsonBase() {
  jsonType = Json::BOOLEAN;
  }

JsonNull::~JsonNull() {
  }

String* JsonNull::ToString() {
  return new String("null");
  }

/* *********************** */
/* ***** JSON number ***** */
/* *********************** */

JsonNumber::JsonNumber(UInt32 i) : JsonBase() {
  jsonType = Json::NUMBER;
  ivalue = i;
  dvalue = i;
  }

JsonNumber::JsonNumber(Int32 i) : JsonBase() {
  jsonType = Json::NUMBER;
  ivalue = i;
  dvalue = i;
  }

JsonNumber::JsonNumber(Double d) : JsonBase() {
  jsonType = Json::NUMBER;
  ivalue = (Int32)d;
  dvalue = d;
  }

JsonNumber::~JsonNumber() {
  }

Int32 JsonNumber::AsInteger() {
  return ivalue;
  }

Double JsonNumber::AsDouble() {
  return dvalue;
  }

String* JsonNumber::ToString() {
  char tmp[32];
  if (ivalue == dvalue) sprintf(tmp,"%d",ivalue);
    else sprintf(tmp,"%f",dvalue);
  return new String(tmp);
  }


/* *********************** */
/* ***** JSON String ***** */
/* *********************** */

JsonString::JsonString(char* s) : JsonBase() {
  jsonType = Json::STRING;
  value = new String(s);
  }

JsonString::JsonString(String s) : JsonBase() {
  jsonType = Json::STRING;
  value = new String(s);
  }

JsonString::JsonString(String *s) : JsonBase() {
  jsonType = Json::STRING;
  value = new String(s);
  }

JsonString::~JsonString() {
  delete(value);
  }

String* JsonString::ToString() {
  char *tmp;
  String *result;
  tmp = (char*)malloc(value->Length()+3);
  strcpy(tmp,"\"");
  strcat(tmp,value->AsCharArray());
  strcat(tmp,"\"");
  result = new String(tmp);
  free(tmp);
  return result;
  }

String* JsonString::Value() {
  return value;
  }


/* *********************** */
/* ***** JSON object ***** */
/* *********************** */

Json::Json() : JsonBase() {
  jsonType = Json::OBJECT;
  keys = new List<String*>();
  values = new List<JsonBase*>();
  }

Json::Json(const char* s) : JsonBase() {
  char key[1024];
  char value[65536];
  Boolean quote;
  Boolean dot;
  int  pos;
  int  count;
  jsonType = Json::OBJECT;
  keys = new List<String*>();
  values = new List<JsonBase*>();
  while (*s > 0 && *s <= ' ') s++;
  if (*s != '{')
    throw InvalidOpException("String did not start with {");
  s++;
  while (*s != 0 && *s != '}') {
    while (*s > 0 && *s <= ' ') s++;
    quote = false;
    pos = 0;
    if (*s == '"') {
      quote = true;
      s++;
      }
    while (*s != 0 && *s != ':' && *s != '"') {
      key[pos++] = *s++;
      }
    key[pos] = 0;
    if (*s == 0)
      throw InvalidOpException("Unexpected end of string");
    if (*s == ':' && quote)
      throw InvalidOpException("Missing quotation mark");
    if (*s == '"' && quote == false)
      throw InvalidOpException("Unexpected quotation mark");
    if (pos == 0)
      throw InvalidOpException("Key had zero length");
    if (*s == '"') s++;
    while (*s > 0 && *s <= ' ') s++;
   
    if (*s != ':')
      throw InvalidOpException("Missing :");
    s++;
    while (*s > 0 && *s <= ' ') s++;
    if (strncasecmp(s, "true", 4) == 0) {
      s += 4;
      if (*s != ' ' && *s != '\t' && *s != ',' && *s != '}')
        throw InvalidOpException("Invalid boolean value");
      this->Add(key, true);
      }
    else if (strncasecmp(s, "false", 5) == 0) {
      s += 5;
      if (*s != ' ' && *s != '\t' && *s != ',' && *s != '}')
        throw InvalidOpException("Invalid boolean value");
      this->Add(key, false);
      }
    else if (strncasecmp(s, "null", 4) == 0) {
      s += 4;
      if (*s != ' ' && *s != '\t' && *s != ',' && *s != '}')
        throw InvalidOpException("Invalid null value");
      this->Add(key, new JsonNull());
      }
    else if (*s == '[') {
      count = 1;
      pos = 0;
      value[pos++] = *s++;
      while (*s != 0 && count > 0) {
        if (*s == '[') {
          s++;
          count++;
          }
        else if (*s == ']') {
          value[pos++] = *s;
          if (count > 1) s++;
          count--;
          }
        else value[pos++] = *s++;
        }
      value[pos] = 0;
      if (*s != ']') 
        throw InvalidOpException("Unexpected end of string");
      s++;
      this->Add(key, new JsonArray(value));
      }
    else if (*s == '{') {
      count = 1;
      pos = 0;
      value[pos++] = *s++;
      while (*s != 0 && count > 0) {
        if (*s == '{') {
          s++;
          count++;
          }
        else if (*s == '}') {
          value[pos++] = *s;
          if (count > 1) s++;
          count--;
          }
        else value[pos++] = *s++;
        }
      value[pos] = 0;
      if (*s != '}') 
        throw InvalidOpException("Unexpected end of string");
      s++;
      this->Add(key, new Json(value));
      }
    else if (*s == '"') {
      s++;
      pos = 0;
      while (*s != 0 && *s != '"') value[pos++] = *s++;
      if (*s == 0)
        throw InvalidOpException("Unexpected end of string");
      s++;
      value[pos] = 0;
      this->Add(key, value);
      }
    else {
      if (*s == '+' || *s == '-' || *s == '.' ||
          (*s >= '0' && *s <= '9')) {
        pos = 0;
        dot = false;
        if (*s == '+') s++;
        else if (*s == '-') value[pos++] = *s++;
        else if (*s == '.') {
          value[pos++] = *s++;
          dot = true;
          }
        while ((*s >= '0' && *s <= '9') || *s == '.') {
          if (*s == '.' && dot == false) {
            value[pos++] = *s++;
            dot = true;
            }
          else if (*s == '.')
            throw InvalidOpException("Multiple periods");
          else if (*s >= '0' && *s <= '9') value[pos++] = *s++;
          }
        value[pos] = 0;
        if (dot) {
          this->Add(key, atof(value));
          }
        else {
          this->Add(key, atoi(value));
          }
        }
      else {
        throw InvalidOpException("Unexpected character");
        }
      }
    while (*s > 0 && *s <= ' ') s++;
    if (*s == 0)
      throw InvalidOpException("Unexpected end of string");
    if (*s != ',' && *s != '}')
      throw InvalidOpException("Unexpected character");
    if (*s == ',') s++;
    }
  }

Json::~Json() {
  int i;
  for (i=0; i<keys->Count(); i++) delete(keys->At(i));
  for (i=0; i<values->Count(); i++) delete(values->At(i));
  delete(keys);
  delete(values);
  }

void Json::Add(const char* key, Boolean value) {
  keys->Add(new String(key));
  values->Add(new JsonBoolean(value));
  }

void Json::Add(const char* key, Int32 value) {
  keys->Add(new String(key));
  values->Add(new JsonNumber(value));
  }

void Json::Add(const char* key, Double value) {
  keys->Add(new String(key));
  values->Add(new JsonNumber(value));
  }

void Json::Add(const char* key, const char* value) {
  keys->Add(new String(key));
  values->Add(new JsonString(value));
  }

void Json::Add(const char* key, String value) {
  keys->Add(new String(key));
  values->Add(new JsonString(value));
  }

void Json::Add(const char* key, String* value) {
  keys->Add(new String(key));
  values->Add(new JsonString(value));
  }

void Json::Add(const char* key, JsonBase *obj) {
  keys->Add(new String(key));
  values->Add(obj);
  }

JsonBase* Json::Get(const char* key) {
  int i;
  for (i=0; i<keys->Count(); i++)
    if (keys->At(i)->Equals(key)) return values->At(i);
  return NULL;
  }

String* Json::ToString() {
  int i;
  String *item;
  char buffer[65536];
  strcpy(buffer,"{");
  for (i=0; i<keys->Count(); i++) {
    if (i!= 0) strcat(buffer,",\"");
      else strcat(buffer,"\"");
    strcat(buffer,keys->At(i)->AsCharArray());
    strcat(buffer,"\":");
    item = values->At(i)->ToString();
    strcat(buffer,item->AsCharArray());
    delete(item);
    }
  strcat(buffer,"}");
  return new String(buffer);
  }

