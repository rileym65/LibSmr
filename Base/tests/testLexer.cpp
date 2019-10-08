#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestLexer() {
  Lexer*        obj;
  String        input;
  StringStream *stringStream;
  StreamReader *reader;
  UInt32        token;

  obj = new Lexer();
  printf("\nInfo: Lexer tests\n");
  if (obj != NULL) Good("Was able to instantiate a Lexer");
    else Bad("Could not instantiate a Lexer");
  if (strcmp(obj->Type(), "Lexer") == 0)
    Good("Lexer reported correctly on Type");
    else Bad("Lexer did not report correctly on Type");

  try {
    obj->AddPattern("^this$",     10001);
    obj->AddPattern("^is$",       10002);
    obj->AddPattern("^a$",        10003);
    obj->AddPattern("^test$",     10004);
    obj->AddPattern("^th$",       10005);
    obj->AddPattern("^for$",      10006);
    obj->AddPattern("^=$",        10007);
    obj->AddPattern("^;$",        10008);
    obj->AddPattern("^<$",        10009);
    obj->AddPattern("^\\($",      10010);
    obj->AddPattern("^\\)$",      10011);
    obj->AddPattern("^\\+$",      10012);
    obj->AddPattern("^\\+\\+$",   10013);
    obj->AddPattern("^<=$",       10014);
    obj->AddPattern("^thinking$", 90006);
    obj->AddPattern("^[0-9]+$",   20001);
    obj->AddPattern("^\\a\\w*$",  20002);
    obj->AddPattern("^\\s+$",         0);
    obj->AddPattern("^.$", 90006);
    Good("No exceptions thrown adding patterns");
    }
  catch (...) {
    Bad("An exception was thrown adding a RegEx");
    printf("Info: Aborting further Lexer tests\n");
    delete(obj);
    return;
    }

  input = "this is a test";
  printf("Info: Lexing \"%s\"\n",input.AsCharArray());
  stringStream = new StringStream(&input);
  reader = new StreamReader(stringStream);
  reader->DeleteResource(false);
  obj->Source(reader);
  token = obj->Next();
  if (token == 10001) Good("First token was properly read");
    else Bad("First token was not properly read");
  if (obj->Value().Equals("this")) Good("Value was correct for first token");
    else Bad("Value was not correct for first token");
  if (!obj->AtEnd()) Good("AtEnd returned false when not at end");
    else Bad("AtEnd returned true when not at end");
  token = obj->Next();
  if (token == 10002) Good("Second token was properly read");
    else Bad("Second token was not properly read");
  if (obj->Value().Equals("is")) Good("Value was correct for second token");
    else Bad("Value was not correct for second token");
  token = obj->Next();
  if (token == 10003) Good("Third token was properly read");
    else Bad("Third token was not properly read");
  if (obj->Value().Equals("a")) Good("Value was correct for third token");
    else Bad("Value was not correct for third token");
  token = obj->Next();
  if (token == 10004) Good("Fourth token was properly read");
    else Bad("Fourth token was not properly read");
  if (obj->Value().Equals("test")) Good("Value was correct for fourth token");
    else Bad("Value was not correct for fourth token");
  if (obj->AtEnd()) Good("AtEnd returned true when at end");
    else Bad("AtEnd returned false when at end");
  try {
    token = obj->Next();
     Bad("Exception was not thrown trying to read past end");
    }
  catch (InvalidOpException &err) {
    Good("Exception was thrown trying to read past end");
    }
  delete(reader);

  input = "this 1900 Abcd";
  printf("Info: Lexing \"%s\"\n",input.AsCharArray());
  reader = new StreamReader(new StringStream(&input));
  reader->DeleteResource(false);
  obj->Source(reader);
  token = obj->Next();
  if (token == 10001) Good("First token was properly read");
    else Bad("First token was not properly read");
  if (obj->Value().Equals("this")) Good("Value was correct for first token");
    else Bad("Value was not correct for first token");
  token = obj->Next();
  if (token == 20001) Good("Second token was properly read");
    else Bad("Second token was not properly read");
  if (obj->Value().Equals("1900")) Good("Value was correct for second token");
    else Bad("Value was not correct for second token");
  token = obj->Next();
  if (token == 20002) Good("Third token was properly read");
    else Bad("Third token was not properly read");
  if (obj->Value().Equals("Abcd")) Good("Value was correct for third token");
    else Bad("Value was not correct for third token");
  delete(reader);

  input = "for (i=0; i<=10; i++);";
  printf("Info: Lexing \"%s\"\n",input.AsCharArray());
  reader = new StreamReader(new StringStream(&input));
  reader->DeleteResource(false);
  obj->Source(reader);
  token = obj->Next();
  if (token == 10006) Good("First token was properly read");
    else Bad("First token was not properly read");
  token = obj->Next();
  if (token == 10010) Good("Second token was properly read");
    else Bad("Second token was not properly read");
  token = obj->Next();
  if (token == 20002) Good("Third token was properly read");
    else Bad("Third token was not properly read");
  if (obj->Value().Equals("i")) Good("Value was correct for third token");
    else Bad("Value was not correct for third token");
  token = obj->Next();
  if (token == 10007) Good("Fourth token was properly read");
    else Bad("Fourth token was not properly read");
  token = obj->Next();
  if (token == 20001) Good("Fifth token was properly read");
    else Bad("Fifth token was not properly read");
  if (obj->Value().Equals("0")) Good("Value was correct for fifth token");
    else Bad("Value was not correct for fifth token");
  token = obj->Next();
  if (token == 10008) Good("Sixth token was properly read");
    else Bad("Sixth token was not properly read");
  token = obj->Next();
  if (token == 20002) Good("Seventh token was properly read");
    else Bad("Seventh token was not properly read");
  if (obj->Value().Equals("i")) Good("Value was correct for seventh token");
    else Bad("Value was not correct for seventh token");
  token = obj->Next();
  if (token == 10014) Good("Eighth token was properly read");
    else Bad("Eighth token was not properly read");
  token = obj->Next();
  if (token == 20001) Good("Ninth token was properly read");
    else Bad("Ninth token was not properly read");
  if (obj->Value().Equals("10")) Good("Value was correct for ninth token");
    else Bad("Value was not correct for ninth token");
  token = obj->Next();
  if (token == 10008) Good("Tenth token was properly read");
    else Bad("Tenth token was not properly read");
  token = obj->Next();
  if (token == 20002) Good("Eleventh token was properly read");
    else Bad("Eleventh token was not properly read");
  if (obj->Value().Equals("i")) Good("Value was correct for eleventh token");
    else Bad("Value was not correct for eleventh token");
  token = obj->Next();
  if (token == 10013) Good("Twelvth token was properly read");
    else Bad("Twelvth token was not properly read");
  token = obj->Next();
  if (token == 10011) Good("Thirteenth token was properly read");
    else Bad("Thirteenth token was not properly read");
  token = obj->Next();
  if (token == 10008) Good("Fourteenth token was properly read");
    else Bad("Fourteenth token was not properly read");
  try {
    token = obj->Next();
     Bad("Exception was not thrown trying to read past end");
    }
  catch (InvalidOpException &err) {
    Good("Exception was thrown trying to read past end");
    }
  delete(reader);
  delete(obj);
  }
