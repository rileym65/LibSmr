#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestString() {
  UInt32 i;
  String string1;
  String string2;
  String string3;
  String* pstring;
  List<String*>* list;

  pstring = new String((char*)"\t\tAbc\t\t");
  printf("\nInfo: String tests\n");
  if (pstring != NULL) Good("Was able to instantiate a String");
    else Bad("Could not instantiate an String");
  if (strcmp(pstring->Type(), "String") == 0)
    Good("String reported correctly on Type");
    else Bad("String did not report correctly on Type");
  if (pstring->Length() == 7)
    Good("String reported correct length");
    else Bad("String did not report correct length");
  if (strcmp(pstring->AsCharArray(), "\t\tAbc\t\t") == 0)
    Good("String returned correct value");
    else Bad("String did not return correct value");
  string2 = pstring->Trim();
  if (strcmp(string2.AsCharArray(),"Abc") == 0)
    Good("Trim worked correctly");
    else Bad("Trim did not work correctly");
  delete(pstring);

  string1 = String((char*)"Abc123Abc");
  string2 = string1.ToUpper();
  if (strcmp(string2.AsCharArray(),"ABC123ABC") == 0)
    Good("ToUpper worked correctly");
    else Bad("ToUpper did not work correctly");
  string2 = string1.ToLower();
  if (strcmp(string2.AsCharArray(),"abc123abc") == 0)
    Good("ToLower worked correctly");
    else Bad("ToLower did not work correctly");

  string2 = new String((char*)"bc");
  if (string1.IndexOf('c') == 2)
    Good("IndexOf(char) returned correct value");
    else Bad("IndexOf(char) did not return correct value");
  if (string1.IndexOf((char*)"bc") == 1)
    Good("IndexOf(char*) returned correct value");
    else Bad("IndexOf(char*) did not return correct value");
  if (string1.IndexOf(string2) == 1)
    Good("IndexOf(String*) returned correct value");
    else Bad("IndexOf(String*) did not return correct value");
  if (string1.IndexOf('e') == -1)
    Good("IndexOf(char) returned -1 when character not found");
    else Bad("IndexOf(char) did not return -1 when character not found");

  if (string1.LastIndexOf('b') == 7)
    Good("LastIndexOf(char) returned correct value");
    else Bad("LastIndexOf(char) did not return correct value");
  if (string1.LastIndexOf((char*)"Ab") == 6)
    Good("LastIndexOf(char*) returned correct value");
    else Bad("LastIndexOf(char*) did not return correct value");
  if (string1.LastIndexOf(string2) == 7)
    Good("LastIndexOf(String*) returned correct value");
    else Bad("LastIndexOf(String*) did not return correct value");

  string1.SetValue((char*)"Test Value");
  if (strcmp(string1.AsCharArray(),"Test Value") == 0)
    Good("SetValue worked correctly");
    else Bad("SetValue did not work correctly");

  if (string1.Equals((char*)"Test Value"))
    Good("Equals(char*) worked correctly on equal values");
    else Bad("Equals(char*) did not work on equal values");
  string2 = new String((char*)"Test Value");
  if (string1.Equals(string2))
    Good("Equals(String*) worked correctly on equal values");
    else Bad("Equals(String*) did not work on equal values");
  
  string2.SetValue((char*)"Test");
  if (string1.Equals((char*)"Test") == 0)
    Good("Equals(char*) worked correctly on unequal values");
    else Bad("Equals(char*) did not work correctly on unequal values");
  if (string1.Equals(string2) == 0)
    Good("Equals(String*) worked correctly on unequal values");
    else Bad("Equals(String*) did not work correctly on unequal values");

  string2.SetValue((char*)"Test Value");
  if (string1.CompareTo((char*)"Test Value") == 0)
    Good("CompareTo(char*) worked correctly on equal values");
    else Bad("CompareTo(char*) did not work correctly on equal values");
  if (string1.CompareTo(string2) == 0)
    Good("CompareTo(String*) worked correctly on equal values");
    else Bad("CompareTo(String*) did not work correctly on equal values");

  string2.SetValue((char*)"Abc");
  if (string1.CompareTo((char*)"Abc") > 0)
    Good("CompareTo(char*) worked correctly on first string greater");
    else Bad("CompareTo(char*) did not work correctly on first string greater");
  if (string1.CompareTo(string2) > 0)
    Good("CompareTo(String*) worked correctly on first string greater");
    else Bad("CompareTo(String*) did not work correctly on first string greater");

  string2.SetValue((char*)"Xyz");
  if (string1.CompareTo((char*)"Xyz") < 0)
    Good("CompareTo(char*) worked correctly on first string lesser");
    else Bad("CompareTo(char*) did not work correctly on first string lesser");
  if (string1.CompareTo(string2) < 0)
    Good("CompareTo(String*) worked correctly on first string lesser");
    else Bad("CompareTo(String*) did not work correctly on first string lesser");

  string1.SetValue((char*)"Abcdefg");
  string2 = string1.Substring(3);
  if (string2.Equals((char*)"defg"))
    Good("Substring starting in the middle worked");
    else Bad("Substring starting in the middle did not work");

  string2 = string1.Substring(0);
  if (string2.Equals((char*)"Abcdefg"))
    Good("Substring starting at the beginning worked");
    else Bad("Substring starting at the beginning did not work");

  string2 = string1.Substring(6);
  if (string2.Equals((char*)"g"))
    Good("Substring starting at the end worked");
    else Bad("Substring starting at the end did not work");

  string2 = string1.Substring(-1);
  if (string2.Length() == 0)
    Good("Substring returned NULL on negative start");
    else Bad("Substring did not return NULL on negative start");

  string2 = string1.Substring(10);
  if (string2.Length() == 0)
    Good("Substring returned NULL on start past end");
    else Bad("Substring did not return NULL on start past end");

  string2 = string1.Substring(-1,1);
  if (string2.Length() == 0)
    Good("Substring with length returned NULL on negative start");
    else Bad("Substring with length did not return NULL on negative start");

  string2 = string1.Substring(10,1);
  if (string2.Length() == 0)
    Good("Substring with length returned NULL on start past end");
    else Bad("Substring with length did not return NULL on start past end");

  string2 = string1.Substring(1,-1);
  if (string2.Length() == 0)
    Good("Substring with length returned NULL on negative length");
    else Bad("Substring with length did not return NULL on negative length");

  string2 = string1.Substring(1,3);
  if (string2.Equals((char*)"bcd"))
    Good("Substring with length wholey in middle works");
    else Bad("Substring with length wholey in middle does not work");

  string2 = string1.Substring(4,6);
  if (string2.Equals((char*)"efg"))
    Good("Substring with length reading past end worked");
    else Bad("Substring with length reading past end does not work");

  string2 = string1.Substring(4,0);
  if (strlen(string2.AsCharArray()) == 0)
    Good("Substring with length zero produced zero length string");
    else Bad("Substring with length zero did not produce zero length string");

  string1.SetValue((char*)" 	Test	 ");
  string2 = string1.TrimLeft();
  if (string2.Equals((char*)"Test	 "))
    Good("TrimLeft worked correctly");
    else Bad("TrimLeft did not work correctly");

  string2 = string1.TrimRight();
  if (string2.Equals((char*)" 	Test"))
    Good("TrimRight worked correctly");
    else Bad("TrimRight did not work correctly");

  string1.SetValue((char*)"Ab");
  if (string1.First() == 'A')
    Good("First returned the correct value");
    else Bad("First did not return the correct value");
  if (string1.Next() == 'b')
    Good("Next returned the correct value");
    else Bad("Next did not return the correct value");
  if (string1.Next() == 0)
    Good("Next past end returned the correct value");
    else Bad("Next past end did not return the correct value");
  if (string1.Last() == 'b')
    Good("Last returned the correct value");
    else Bad("Last did not return the correct value");

  string1.SetValue((char*)"Abcdefg");
  string2 = string1.Reverse();
  if (string2.Equals((char*)"gfedcbA"))
    Good("Reverse correctly reversed the string");
    else Bad("Reverse did not properly reverse the string");

  string2 = string1.Append('X');
  if (string2.Equals((char*)"AbcdefgX"))
    Good("Append(char) worked correctly");
    else Bad("Append(char) did not work correctly");

  string2 = string1.Append((char*)"Xyz");
  if (string2.Equals((char*)"AbcdefgXyz"))
    Good("Append(char*) worked correctly");
    else Bad("Append(char*) did not work correctly");

  string3 = String((char*)"123");
  string2 = string1.Append(string3);
  if (string2.Equals((char*)"Abcdefg123"))
    Good("Append(String*) worked correctly");
    else Bad("Append(String*) did not work correctly");

  if (string1.CharAt(1) == 'b')
    Good("CharAt returned correct character");
    else Bad("CharAt did not return correct character");

  if (string1.CharAt(-1) == 0)
    Good("CharAt returned 0 on negative position");
    else Bad("CharAt did not return 0 on negative position");

  if (string1.CharAt(90) == 0)
    Good("CharAt returned 0 when position past end");
    else Bad("CharAt did not return 0 when position past end");

  string1.SetValue((char*)"ABCDEFGHIJKLMNOPQRSTUVWXYZ123abcdefghijklmnopqrstuvwxyz");
  string2 = string1.Rot13();
  if (string2.Equals((char*)"NOPQRSTUVWXYZABCDEFGHIJKLM123nopqrstuvwxyzabcdefghijklm"))
    Good("Rot13 correctly translated the string");
    else Bad("Rot13 did not correctly translate the string");

  string1 = String("Abc").PadRight(6);
  if (string1 == "Abc   ") Good("Pad right correctly added spaces to the right");
    else Bad("PadRight did not correctly add spaces to the right");

  string1 = String("Abc").PadRight(6, '*');
  if (string1 == "Abc***") Good("Pad right correctly added asterisks to the right");
    else Bad("PadRight did not correctly add asterisks to the right");

  string1 = String("Abcdef").PadRight(6);
  if (string1 == "Abcdef") Good("Padright did nothing for string already correct length");
    else Bad("PadRight modified string already correct length");

  string1 = String("Abcdefghi").PadRight(6);
  if (string1 == "Abcdefghi") Good("Padright did nothing for larger string");
    else Bad("PadRight modified larger string");

  string1 = String("Abc").PadLeft(6);
  if (string1 == "   Abc") Good("PadLeft correctly added spaces to the left");
    else Bad("PadLeft did not correctly add spaces to the left");

  string1 = String("Abc").PadLeft(6, '*');
  if (string1 == "***Abc") Good("PadLeft correctly added asterisks to the left");
    else Bad("PadLeft did not correctly add asterisks to the left");

  string1 = String("Abcdef").PadLeft(6);
  if (string1 == "Abcdef") Good("PadLeft did nothing for string already correct length");
    else Bad("PadLeft modified string already correct length");

  string1 = String("Abcdefghi").PadLeft(6);
  if (string1 == "Abcdefghi") Good("PadLeft did nothing for larger string");
    else Bad("PadLeft modified larger string");

  if (string1[2] == 'c') Good("[] operator retrieved correct character");
    else Bad("[] operator did not retrieve the correct character");

  string1 = String("abcde");
  string1[2] = 'z';
  if (string1 == "abzde") Good("[] operator succeded in setting a character");
    else Bad("[] operator did not correctly set a character");

  string1 = String("12:34:56:78");
  list = string1.Split(':');
  if (list->Count() == 4) Good("Split produced 4 items from 12:34:56:78");
    else Bad("Split did not produce 4 items from 12:34:56:78");
  if (list->At(0)->Equals("12")) Good("First item was 12");
    else Bad("first item was not 12");
  if (list->At(1)->Equals("34")) Good("Second item was 34");
    else Bad("Second item was not 34");
  if (list->At(2)->Equals("56")) Good("Third item was 56");
    else Bad("Third item was not 56");
  if (list->At(3)->Equals("78")) Good("Fourth item was 78");
    else Bad("Fourth item was not 78");
  for (i=0; i<list->Count(); i++) delete(list->At(i));
  delete(list);

  string1 = String("      ");
  try {
    string1 = string1.Trim();
    if (string1.Length() == 0)
      Good("Trim returned a zero length string on string of spaces");
    else
      Bad("Trim did not return a zero length string on string of psaces");
    }
  catch (Exception &e) {
    Bad("Exception thrown when executing Trim on string of spaces");
    }

  string1 = String("      ");
  try {
    string1 = string1.TrimLeft();
    if (string1.Length() == 0)
      Good("TrimLeft returned a zero length string on string of spaces");
    else
      Bad("TrimLeft did not return a zero length string on string of psaces");
    }
  catch (Exception &e) {
    Bad("Exception thrown when executing TrimLeft on string of spaces");
    }

  string1 = String("      ");
  try {
    string1 = string1.TrimRight();
    if (string1.Length() == 0)
      Good("TrimRight returned a zero length string on string of spaces");
    else
      Bad("TrimRight did not return a zero length string on string of psaces");
    }
  catch (Exception &e) {
    Bad("Exception thrown when executing TrimRight on string of spaces");
    }

  string1 = String("a+bb * c /e+(abc) = d");
  list = string1.Tokenize(String("+-*/= \t"));
  if (list->Count() == 11) Good("Tokenize returned a list of the correct size");
    else Bad("Tokenize did not return the correct size list");
  if (list->At(0)->Equals("a")) Good("First token was correct");
    else Bad("First token was not correct");
  if (list->At(1)->Equals("+")) Good("Second token was correct");
    else Bad("Second token was not correct");
  if (list->At(2)->Equals("bb")) Good("Third token was correct");
    else Bad("Third token was not correct");
  if (list->At(3)->Equals("*")) Good("Fourth token was correct");
    else Bad("Fourth token was not correct");
  if (list->At(4)->Equals("c")) Good("Fifth token was correct");
    else Bad("Fifth token was not correct");
  if (list->At(5)->Equals("/")) Good("Sixth token was correct");
    else Bad("Sixth token was not correct");
  if (list->At(6)->Equals("e")) Good("Seventh token was correct");
    else Bad("Seventh token was not correct");
  if (list->At(7)->Equals("+")) Good("Eighth token was correct");
    else Bad("Eighth token was not correct");
  if (list->At(8)->Equals("(abc)")) Good("Ninth token was correct");
    else Bad("Ninth token was not correct");
  if (list->At(9)->Equals("=")) Good("Tenth token was correct");
    else Bad("Tenth token was not correct");
  if (list->At(10)->Equals("d")) Good("Eleventh token was correct");
    else Bad("Eleventh token was not correct");
  }
