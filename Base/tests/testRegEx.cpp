#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestRegEx() {
  RegEx* obj;
  String var;
  printf("\nInfo: RegEx tests\n");
  try {
    obj = new RegEx("(a?b+c*.+)[0-9a-fA-F]*[^a-c]+[0-9]{3}[a-f]{,4}[A-F]{3,}[0-9]{3,5}");
    Good("No exception thrown creating regular expression");
    }
  catch (...) {
    Bad("Exception was thrown creating a valid regular expression");
    printf("Info: Further tests will be aborted");
    if (obj != NULL) delete(obj);
    return;
    }
  if (obj != NULL) Good("Was able to instantiate a RegEx");
    else Bad("Could not instantiate a RegEx");
  if (strcmp(obj->Type(), "RegEx") == 0)
    Good("RegEx reported correctly on Type");
    else Bad("RegEx did not report correctly on Type");
  delete(obj);

  obj = new RegEx("abcd");
  if (obj->Match("abcd")) Good("Simple literal pattern successfully matched");
    else Bad("Simple literal pattern was not successfully matched");
  if (obj->Match("123abcd")) Good("abcd matched 123abcd");
    else Bad("abcd did not match 123abcd");
  delete(obj);

  obj = new RegEx("ab*cd");
  if (obj->Match("abcd")) Good("ab*cd matched abcd");
    else Bad("ab*cd did not match abcd");
  if (obj->Match("acd")) Good("ab*cd matched acd");
    else Bad("ab*cd did not match acd");
  if (obj->Match("abbbbbbbcd")) Good("ab*cd matched abbbbbbbcd");
    else Bad("ab*cd did not match abbbbbbbcd");
  if (!obj->Match("abbbbbbbCd")) Good("ab*cd did not match abbbbbbbCd");
    else Bad("ab*cd matched abbbbbbbCd");
  delete(obj);

  obj = new RegEx("ab+cd");
  if (obj->Match("abcd")) Good("ab+cd matched abcd");
    else Bad("ab+cd did not match abcd");
  if (obj->Match("abbbbcd")) Good("ab+cd matched abbbbcd");
    else Bad("ab+cd did not match abbbbcd");
  if (obj->Match("abbbbcdef")) Good("ab+cd matched abbbbcdef");
    else Bad("ab+cd did not match abbbbcdef");
  if (!obj->Match("acd")) Good("ab+cd did not match acd");
    else Bad("ab+cd matched acd");
  delete(obj);

  obj = new RegEx("ab?cd");
  if (obj->Match("abcd")) Good("ab?cd matched abcd");
    else Bad("ab?cd did not match abcd");
  if (obj->Match("acd")) Good("ab?cd matched acd");
    else Bad("ab?cd did not match acd");
  if (!obj->Match("abbcd")) Good("ab?cd did not match abbcd");
    else Bad("ab?cd matched abbcd");
  if (!obj->Match("abccd")) Good("ab?cd did not match abccd");
    else Bad("ab?cd matched abccd");
  if (obj->Match("abbbcdabcd")) Good("ab?cd matched abbbcdabcd");
    else Bad("ab?cd did not match abbbcdabcd");
  delete(obj);

  obj = new RegEx("ab{3}cd");
  if (obj->Match("abbbcd")) Good("ab{3}cd matched abbbcd");
    else Bad("ab{3}cd did not match abbbcd");
  if (!obj->Match("abbcd")) Good("ab{3}cd did not match abbcd");
    else Bad("ab{3}cd matched abbcd");
  if (!obj->Match("abcd")) Good("ab{3}cd did not match abcd");
    else Bad("ab{3}cd matched abcd");
  if (!obj->Match("acd")) Good("ab{3}cd did not match acd");
    else Bad("ab{3}cd matched acd");
  if (!obj->Match("acd")) Good("ab{3}cd did not match abbbbcd");
    else Bad("ab{3}cd matched abbbbcd");
  delete(obj);

  obj = new RegEx("ab{1,3}cd");
  if (obj->Match("abbbcd")) Good("ab{1,3}cd matched abbbcd");
    else Bad("ab{1,3}cd did not match abbbcd");
  if (obj->Match("abbcd")) Good("ab{1,3}cd matched abbcd");
    else Bad("ab{1,3}cd did not match abbcd");
  if (obj->Match("abcd")) Good("ab{1,3}cd matched abcd");
    else Bad("ab{1,3}cd did not match abcd");
  if (!obj->Match("acd")) Good("ab{1,3}cd did not match acd");
    else Bad("ab{1,3}cd matched acd");
  if (!obj->Match("abbbbcd")) Good("ab{1,3}cd did not match abbbbcd");
    else Bad("ab{1,3}cd matched abbbbcd");
  delete(obj);

  obj = new RegEx("ab{2,}cd");
  if (obj->Match("abbcd")) Good("ab{2,}cd matched abbcd");
    else Bad("ab{2,}cd did not match abbcd");
  if (obj->Match("abbbcd")) Good("ab{2,}cd matched abbbcd");
    else Bad("ab{2,}cd did not match abbbcd");
  if (obj->Match("abbbbcd")) Good("ab{2,}cd matched abbbbcd");
    else Bad("ab{2,}cd did not match abbbbcd");
  if (!obj->Match("abcd")) Good("ab{2,}cd did not match abcd");
    else Bad("ab{2,}cd matched abcd");
  if (!obj->Match("acd")) Good("ab{2,}cd did not match acd");
    else Bad("ab{2,}cd matched acd");
  if (!obj->Match("abbccd")) Good("ab{2,}cd did not match abbccd");
    else Bad("ab{2,}cd matched abbccd");
  delete(obj);

  obj = new RegEx("ab{,3}cd");
  if (obj->Match("abbcd")) Good("ab{,3}cd matched abbcd");
    else Bad("ab{,3}cd did not match abbcd");
  if (obj->Match("abcd")) Good("ab{,3}cd matched abcd");
    else Bad("ab{,3}cd did not match abcd");
  if (obj->Match("acd")) Good("ab{,3}cd matched acd");
    else Bad("ab{,3}cd did not match acd");
  if (obj->Match("abbbcd")) Good("ab{,3}cd matched abbbcd");
    else Bad("ab{,3}cd did not match abbbcd");
  if (!obj->Match("abbbbcd")) Good("ab{,3}cd did not match abbbbcd");
    else Bad("ab{,3}cd matched abbbbcd");
  if (!obj->Match("abbbbbcd")) Good("ab{,3}cd did not match abbbbbcd");
    else Bad("ab{,3}cd matched abbbbbcd");
  if (obj->Match("abbbbbcdabbcd")) Good("ab{,3}cd matched abbbbbcdabbcd");
    else Bad("ab{,3}cd did not match abbbbbcdabbcd");
  delete(obj);

  obj = new RegEx("ab.cd");
  if (obj->Match("abxcd")) Good("ab.cd matched abxcd");
    else Bad("ab.cd did not match abxcd");
  if (obj->Match("abBcd")) Good("ab.cd matched abBcd");
    else Bad("ab.cd did not match abBcd");
  if (!obj->Match("abcd")) Good("ab.cd did not match abcd");
    else Bad("ab.cd matched abcd");
  if (!obj->Match("abxxcd")) Good("ab.cd did not match abxxcd");
    else Bad("ab.cd matched abxxcd");
  delete(obj);

  obj = new RegEx("ab.*cd");
  if (obj->Match("abcd")) Good("ab.*cd matched abcd");
    else Bad("ab.*cd did not match abcd");
  if (obj->Match("ab123cd")) Good("ab.*cd matched ab123cd");
    else Bad("ab.cd did not match ab123cd");
  if (!obj->Match("ab123c")) Good("ab.*cd did not match ab123c");
    else Bad("ab.cd matched ab123c");
  if (!obj->Match("b123cd")) Good("ab.*cd did not match b123cd");
    else Bad("ab.cd matched b123cd");
  delete(obj);

  obj = new RegEx("ab.*");
  if (obj->Match("abcd")) Good("ab.* matched abcd");
    else Bad("ab.* did not match abcd");
  if (obj->Match("ab")) Good("ab.* matched ab");
    else Bad("ab.* did not match ab");
  if (obj->Match("ab12356")) Good("ab.* matched ab12356");
    else Bad("ab.* did not match ab12356");
  if (!obj->Match("acb")) Good("ab.* did not match acb");
    else Bad("ab.* matched acb");
  delete(obj);

  obj = new RegEx("ab.+cd");
  if (obj->Match("abXcd")) Good("ab.+cd matched abXcd");
    else Bad("ab.+cd did not match abXcd");
  if (obj->Match("abXYZcd")) Good("ab.+cd matched abXYZcd");
    else Bad("ab.+cd did not match abXYZcd");
  if (!obj->Match("abcd")) Good("ab.+cd did not match abcd");
    else Bad("ab.+cd matched abcd");
  delete(obj);

  obj = new RegEx("ab.?cd");
  if (obj->Match("abcd")) Good("ab.?cd matched abcd");
    else Bad("ab.?cd did not match abcd");
  if (obj->Match("ab1cd")) Good("ab.?cd matched ab1cd");
    else Bad("ab.?cd did not match ab1cd");
  if (!obj->Match("ab12cd")) Good("ab.?cd did not match ab12cd");
    else Bad("ab.?cd matched ab12cd");
  delete(obj);

  obj = new RegEx("0x[0-9a-f]");
  if (obj->Match("0x5")) Good("0x[0-9a-f] matched 0x5");
    else Bad("0x[0-9a-f] did not match 0x5");
  if (obj->Match("0xa")) Good("0x[0-9a-f] matched 0xa");
    else Bad("0x[0-9a-f] did not match 0xa");
  if (obj->Match("0x5h")) Good("0x[0-9a-f] matched 0x5h");
    else Bad("0x[0-9a-f] did not match 0x5h");
  if (!obj->Match("0xg")) Good("0x[0-9a-f] did not match 0xg");
    else Bad("0x[0-9a-f] matched 0xg");
  delete(obj);

  obj = new RegEx("0x[0-9a-fA-F]+");
  if (obj->Match("0x5")) Good("0x[0-9a-fA-F]+ matched 0x5");
    else Bad("0x[0-9a-fA-F]+ did not match 0x5");
  if (obj->Match("0x50eF")) Good("0x[0-9a-fA-F]+ matched 0x50eF");
    else Bad("0x[0-9a-fA-F]+ did not match 0x50eF");
  if (!obj->Match("0x")) Good("0x[0-9a-fA-F]+ did not match 0x");
    else Bad("0x[0-9a-fA-F]+ matched 0x");
  if (!obj->Match("0xz")) Good("0x[0-9a-fA-F]+ did not match 0xz");
    else Bad("0x[0-9a-fA-F]+ matched 0xz");
  delete(obj);

  obj = new RegEx("ab[^cde]fg");
  if (obj->Match("ab1fg")) Good("ab[^cde]fg matched ab1fg");
    else Bad("ab[^cde]fg did not match ab1fg");
  if (!obj->Match("abfg")) Good("ab[^cde]fg did not match abfg");
    else Bad("ab[^cde]fg matched abfg");
  if (!obj->Match("abcfg")) Good("ab[^cde]fg did not match abcfg");
    else Bad("ab[^cde]fg matched abcfg");
  delete(obj);

  obj = new RegEx("[0-9]{3}");
  if (obj->Match("123")) Good("[0-9]{3} matched 123");
    else Bad("[0-9]{3} did not match 123");
  if (obj->Match("1234")) Good("[0-9]{3} matched 1234");
    else Bad("[0-9]{3} did not match 1234");
  if (!obj->Match("12")) Good("[0-9]{3} did not match 12");
    else Bad("[0-9]{3} matched 12");
  delete(obj);

  obj = new RegEx("[0-9]{3}[- ][0-9]{3}[- ][0-9]{4}");
  if (obj->Match("123 456-7890"))
    Good("[0-9]{3}[- ][0-9]{3}[- ][0-9]{4} matched 123 456-7890");
  else
    Bad("[0-9]{3}[- ][0-9]{3}[- ][0-9]{4} did not match 123 456-7890");
  if (obj->Match("123-456-7890"))
    Good("[0-9]{3}[- ][0-9]{3}[- ][0-9]{4} matched 123-456-7890");
  else
    Bad("[0-9]{3}[- ][0-9]{3}[- ][0-9]{4} did not match 123-456-7890");
  if (!obj->Match("123456-7890"))
    Good("[0-9]{3}[- ][0-9]{3}[- ][0-9]{4} did not match 123456-7890");
  else
    Bad("[0-9]{3}[- ][0-9]{3}[- ][0-9]{4} matched 123456-7890");
  delete(obj);
  obj = new RegEx("[0-9]{3}[- ]?[0-9]{3}[- ][0-9]{4}");
  if (obj->Match("123456-7890"))
    Good("[0-9]{3}[- ]?[0-9]{3}[- ][0-9]{4} matched 123456-7890");
  else
    Bad("[0-9]{3}[- ]?[0-9]{3}[- ][0-9]{4} did not match 123456-7890");
  delete(obj);

  obj = new RegEx("^abc");
  if (obj->Match("abc")) Good("^abc matched abc");
    else Bad("^abc did not match abc");
  if (!obj->Match("123abc")) Good("^abc did not match 123abc");
    else Bad("^abc matched 123abc");
  delete(obj);

  obj = new RegEx("abc$");
  if (obj->Match("abc")) Good("abc$ matched abc");
    else Bad("abc$ did not match abc");
  if (obj->Match("123abc")) Good("abc$ matched 123abc");
    else Bad("abc$ did not match 123abc");
  if (!obj->Match("abc123")) Good("abc$ did not match abc123");
    else Bad("abc$ matched abc123");
  delete(obj);

  obj = new RegEx("^abc$");
  if (obj->Match("abc")) Good("^abc$ matched abc");
    else Bad("^abc$ did not match abc");
  if (!obj->Match("123abc")) Good("^abc$ did not match 123abc");
    else Bad("^abc$ matched 123abc");
  if (!obj->Match("abc123")) Good("^abc$ did not match abc123");
    else Bad("^abc$ matched abc123");
  if (!obj->Match("123abc123")) Good("^abc$ did not match 123abc123");
    else Bad("^abc$ matched 123abc123");
  delete(obj);

  obj = new RegEx("a\\*b");
  if (obj->Match("a*b")) Good("a\\*b matched a*b");
    else Bad("a\\*b did not match a*b");
  if (!obj->Match("ab")) Good("a\\*b did not match ab");
    else Bad("a\\*b matched ab");
  if (!obj->Match("aab")) Good("a\\*b did not match aab");
    else Bad("a\\*b matched aab");
  delete(obj);

  obj = new RegEx("ab(cde)fg");
  if (obj->Match("abcdefg")) Good("ab(cde)fg matched abcdefg");
    else Bad("ab(cde)fg did not match abcdefg");
  if (obj->VarCount() == 1) Good("ab(cde)fg had correct var count");
    else Bad("ab(cde)fg did not have correct var count");
  if (obj->Var(0).Equals("cde")) Good("ab(cde)fg captured cde to var 0");
    else Bad("ab(cde)fg did not capture cde to var 0");
  delete(obj);

  obj = new RegEx("([0-9]{3})[- ]?([0-9]{3})[- ]?([0-9]{4})");
  if (obj->Match("702-555-1234")) Good("Matched a telephone with variable capture");
    else Bad("Did not match a telephone number with variable capture");
  if (obj->VarCount() == 3) Good("3 variables were captured");
    else Bad("3 variables were not captured");
  if (obj->Var(0).Equals("702")) Good("First variable had 702");
    else Bad("First variable did not have 702");
  if (obj->Var(1).Equals("555")) Good("Second variable had 555");
    else Bad("Second variable did not have 555");
  if (obj->Var(2).Equals("1234")) Good("Third variable had 1234");
    else Bad("Third variable did not have 1234");
  delete(obj);


  obj = new RegEx("ab(.*)(cd)");
  if (obj->Match("ab1234cd")) Good("ab(.*)(cd) matched ab1234cd");
    else Bad("ab(.*)(cd) did not match ab1234cd");
  if (obj->VarCount() == 2) Good("ab(.*)(cd) had correct var count");
    else Bad("ab(.*)(cd) did not have correct var count");
  if (obj->Var(0).Equals("1234")) Good("ab(.*)(cd) captured 1234 to var 0");
    else Bad("ab(.*)(cd) did not capture 1234 to var 0");
  if (obj->Var(1).Equals("cd")) Good("ab(.*)(cd) captured cd to var 1");
    else Bad("ab(.*)(cd) did not capture cd to var 1");
  delete(obj);

  obj = new RegEx("a\\db");
  if (obj->Match("a3b")) Good("a\\db matched a3b");
    else Bad("a\\db did not match a3b");
  if (!obj->Match("ab")) Good("a\\db did not match ab");
    else Bad("a\\db matched ab");
  if (!obj->Match("acb")) Good("a\\db did not match acb");
    else Bad("a\\db matched acb");
  delete(obj);

  obj = new RegEx("a\\Db");
  if (obj->Match("acb")) Good("a\\Db matched acb");
    else Bad("a\\Db did not match acb");
  if (!obj->Match("a3b")) Good("a\\Db did not match a3b");
    else Bad("a\\Db matched a3b");
  delete(obj);

  obj = new RegEx("!\\w*!");
  if (obj->Match("!abc123!")) Good("!\\w*! matched !abc123!");
    else Bad("!\\w*! did not match !abc123!");
  if (!obj->Match("!$%^!")) Good("!\\w*! did not match !$%^!");
    else Bad("!\\w*! matched !$%^!");
  delete(obj);

  obj = new RegEx("!\\W*!");
  if (obj->Match("!$%^!")) Good("!\\W*! matched !$%^!");
    else Bad("!\\W*! did not match !$%^!");
  if (!obj->Match("!abc!")) Good("!\\W*! did not match !abc!");
    else Bad("!\\W*! matched !abc!");
  delete(obj);

  obj = new RegEx("!\\a*!");
  if (obj->Match("!abc!")) Good("!\\a*! matched !abc!");
    else Bad("!\\a*! did not match !abc!");
  if (!obj->Match("!a2c!")) Good("!\\a*! did not match !a2c!");
    else Bad("!\\a*! matched !a2c!");
  delete(obj);

  obj = new RegEx("!\\A*!");
  if (obj->Match("!123!")) Good("!\\A*! matched !123!");
    else Bad("!\\A*! did not match !123!");
  if (!obj->Match("!a2c!")) Good("!\\A*! did not match !a2c!");
    else Bad("!\\A*! matched !a2c!");
  delete(obj);

  obj = new RegEx("!\\s*!");
  if (obj->Match("!   !")) Good("!\\s*! matched !   !");
    else Bad("!\\s*! did not match !   !");
  if (!obj->Match("! 2 !")) Good("!\\s*! did not match ! 2 !");
    else Bad("!\\s*! matched ! 2 !");
  delete(obj);

  obj = new RegEx("!\\S*!");
  if (obj->Match("!a2$!")) Good("!\\S*! matched !a2$!");
    else Bad("!\\S*! did not match !a2$!");
  if (!obj->Match("! 2 !")) Good("!\\S*! did not match ! 2 !");
    else Bad("!\\S*! matched ! 2 !");
  delete(obj);

  obj = new RegEx("a\\p*b");
  if (obj->Match("a#$%b")) Good("a\\p*b matched a#$%b");
    else Bad("a\\p*b did not match a#$%b");
  if (!obj->Match("a#4%b")) Good("a\\p*b did not match a#4%b");
    else Bad("a\\p*b matched a#4%b");
  delete(obj);

  obj = new RegEx("!\\u*!");
  if (obj->Match("!ABC!")) Good("!\\u*! matched !ABC!");
    else Bad("!\\u*! did not match !ABC!");
  if (!obj->Match("!AbC!")) Good("!\\u*! did not match !AbC!");
    else Bad("!\\u*! matched !AbC!");
  delete(obj);

  obj = new RegEx("!\\l*!");
  if (obj->Match("!abc!")) Good("!\\l*! matched !abc!");
    else Bad("!\\l*! did not match !abc!");
  if (!obj->Match("!AbC!")) Good("!\\l*! did not match !AbC!");
    else Bad("!\\l*! matched !AbC!");
  delete(obj);

  obj = new RegEx("0x\\x*$");
  if (obj->Match("0x12aC")) Good("0x\\x*$ matched 0x12aC");
    else Bad("0x\\x*$ did not match 0x12aC");
  if (!obj->Match("0x12aG")) Good("0x\\x*$ did not match 0x12aG");
    else Bad("0x\\x*$ matched 0x12aG");
  delete(obj);

  obj = new RegEx("\\s+");
  if (obj->Match(" ")) Good("\\s+ matched a single space");
    else Bad("\\s+ did not match a single space");
  delete(obj);

  obj = NULL;
  try {
    obj = new RegEx("a^b");
    Bad("Exception not thrown when using ^ other than at beginning");
    delete(obj);
    }
  catch (...) {
    Good("Exception thrown when using ^ other than at beginning");
    if (obj != NULL) delete(obj);
    }

  obj = NULL;
  try {
    obj = new RegEx("a$b");
    Bad("Exception not thrown when using $ other than at end");
    delete(obj);
    }
  catch (...) {
    Good("Exception thrown when using $ other than at end");
    if (obj != NULL) delete(obj);
    }

  obj = NULL;
  try {
    obj = new RegEx("a**b");
    Bad("Exception not thrown when using * out of place");
    delete(obj);
    }
  catch (...) {
    Good("Exception thrown when using * out of place");
    if (obj != NULL) delete(obj);
    }

  obj = NULL;
  try {
    obj = new RegEx("a*+b");
    Bad("Exception not thrown when using + out of place");
    delete(obj);
    }
  catch (...) {
    Good("Exception thrown when using + out of place");
    if (obj != NULL) delete(obj);
    }

  obj = NULL;
  try {
    obj = new RegEx("a*?b");
    Bad("Exception not thrown when using ? out of place");
    delete(obj);
    }
  catch (...) {
    Good("Exception thrown when using ? out of place");
    if (obj != NULL) delete(obj);
    }

  obj = NULL;
  try {
    obj = new RegEx("a*{b");
    Bad("Exception not thrown when using { out of place");
    delete(obj);
    }
  catch (...) {
    Good("Exception thrown when using { out of place");
    if (obj != NULL) delete(obj);
    }

  obj = NULL;
  try {
    obj = new RegEx("a*}b");
    Bad("Exception not thrown when using } out of place");
    delete(obj);
    }
  catch (...) {
    Good("Exception thrown when using } out of place");
    if (obj != NULL) delete(obj);
    }

  obj = NULL;
  try {
    obj = new RegEx("a*}b");
    Bad("Exception not thrown when using ] out of place");
    delete(obj);
    }
  catch (...) {
    Good("Exception thrown when using ] out of place");
    if (obj != NULL) delete(obj);
    }

  obj = NULL;
  try {
    obj = new RegEx("a[}b");
    Bad("Exception not thrown when end of string with open character class");
    delete(obj);
    }
  catch (...) {
    Good("Exception thrown when end of string with open character class");
    if (obj != NULL) delete(obj);
    }

  obj = NULL;
  try {
    obj = new RegEx("a[z-a]");
    Bad("Exception not thrown when range in character class reversed");
    delete(obj);
    }
  catch (...) {
    Good("Exception thrown when range in character class reversed");
    if (obj != NULL) delete(obj);
    }

  obj = NULL;
  try {
    obj = new RegEx("abc\\");
    Bad("Exception not thrown when escape found at end of string");
    delete(obj);
    }
  catch (...) {
    Good("Exception thrown when escape found at end of string");
    if (obj != NULL) delete(obj);
    }

  }
