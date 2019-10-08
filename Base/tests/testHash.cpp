#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestHash() {
  Hash* obj;
  String str1;
  obj = new Hash();
  printf("\nInfo: Hash tests\n");
  if (obj != NULL) Good("Was able to instantiate a Hash");
    else Bad("Could not instantiate a Hash");
  if (strcmp(obj->Type(), "Hash") == 0)
    Good("Hash reported correctly on Type");
    else Bad("Hash did not report correctly on Type");

  str1 = obj->Sha1("The quick brown fox jumps over the lazy dog", 43);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 20);
  if (str1.CompareTo("2fd4e1c67a2d28fced849ee1bb76e7391b93eb12") == 0)
    Good("First string computed correct sha-1 hash");
  else
    Bad("First string did not compute correct sha-1 hash");


  str1 = Hash::Sha1("The quick brown fox jumps over the lazy cog", 43);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 20);
  if (str1.CompareTo("de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3") == 0)
    Good("Second string computed correct sha-1 hash");
  else
    Bad("Second string did not compute correct sha-1 hash");

  str1 = Hash::Sha1("",0);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 20);
  if (str1.CompareTo("da39a3ee5e6b4b0d3255bfef95601890afd80709") == 0)
    Good("Empty string computed correct sha-1 hash");
  else
    Bad("Empty string did not compute correct sha-1 hash");

  str1 = Hash::Sha1("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123",55);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 20);
  if (str1.CompareTo("ddac9b560f902a65caffe17769ee09eb3ac96e7c") == 0)
    Good("String with length 55 computed correct sha-1 hash");
  else
    Bad("String with length 55 did not compute correct sha-1 hash");

  str1 = Hash::Sha1("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234",56);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 20);
  if (str1.CompareTo("19b3e31556ef105e66f117b29781d690c9f4efef") == 0)
    Good("String with length 56 computed correct sha-1 hash");
  else
    Bad("String with length 56 did not compute correct sha-1 hash");

  str1 = Hash::Sha1("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ12345",57);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 20);
  if (str1.CompareTo("4a81f088eaf5e4de9f136afac2c394b3cdd30bda") == 0)
    Good("String with length 57 computed correct sha-1 hash");
  else
    Bad("String with length 57 did not compute correct sha-1 hash");

  str1 = Hash::Sha1("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456",58);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 20);
  if (str1.CompareTo("2c893d621e73661d8a70c5b67680421aaab28983") == 0)
    Good("String with length 58 computed correct sha-1 hash");
  else
    Bad("String with length 58 did not compute correct sha-1 hash");

  str1 = Hash::Sha1("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567",59);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 20);
  if (str1.CompareTo("c33a1d049cb66fb8023f39973f76b609312d9e5a") == 0)
    Good("String with length 59 computed correct sha-1 hash");
  else
    Bad("String with length 59 did not compute correct sha-1 hash");

  str1 = Hash::Sha1("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ12345678",60);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 20);
  if (str1.CompareTo("8732b8d03ff4013f1aac415d2fcff92596d6e2bb") == 0)
    Good("String with length 60 computed correct sha-1 hash");
  else
    Bad("String with length 60 did not compute correct sha-1 hash");

  str1 = Hash::Sha1("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456789",61);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 20);
  if (str1.CompareTo("4c53ce549ccde04d43a5b7f193124c692393d186") == 0)
    Good("String with length 61 computed correct sha-1 hash");
  else
    Bad("String with length 61 did not compute correct sha-1 hash");

  str1 = Hash::Sha1("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890",62);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 20);
  if (str1.CompareTo("faa62a12e3969824a7daf66b62c939dc0324b6b8") == 0)
    Good("String with length 62 computed correct sha-1 hash");
  else
    Bad("String with length 62 did not compute correct sha-1 hash");

  str1 = Hash::Sha1("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890<",63);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 20);
  if (str1.CompareTo("fa3f36ad26362dda613add2df0bc04cffd955d9e") == 0)
    Good("String with length 63 computed correct sha-1 hash");
  else
    Bad("String with length 63 did not compute correct sha-1 hash");

  str1 = Hash::Sha1("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890<>",64);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 20);
  if (str1.CompareTo("18940a98852c8f1cf545f3f7dd585e864ee92997") == 0)
    Good("String with length 64 computed correct sha-1 hash");
  else
    Bad("String with length 64 did not compute correct sha-1 hash");

  str1 = Hash::Sha1("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890<>=",65);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 20);
  if (str1.CompareTo("2a95373c22227ddd059e6cda67a8332809c3bffb") == 0)
    Good("String with length 65 computed correct sha-1 hash");
  else
    Bad("String with length 65 did not compute correct sha-1 hash");


  str1 = Hash::Md5("The quick brown fox jumps over the lazy dog",43);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 16);
  if (str1.CompareTo("9e107d9d372bb6826bd81d3542a419d6") == 0)
    Good("String with length 43 computed correct MD5 hash");
  else
    Bad("String with length 43 did not compute correct MD5 hash");

  str1 = Hash::Md5("The quick brown fox jumps over the lazy dog.",44);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 16);
  if (str1.CompareTo("e4d909c290d0fb1ca068ffaddf22cbd0") == 0)
    Good("String with length 44 computed correct MD5 hash");
  else
    Bad("String with length 44 did not compute correct MD5 hash");

  str1 = Hash::Md5("",0);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 16);
  if (str1.CompareTo("d41d8cd98f00b204e9800998ecf8427e") == 0)
    Good("String with length 0 computed correct MD5 hash");
  else
    Bad("String with length 0 did not compute correct MD5 hash");

  str1 = Hash::Md5("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123",55);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 16);
  if (str1.CompareTo("f9592d9381ae52ef096e0c2a28e3c823") == 0)
    Good("String with length 55 computed correct MD5 hash");
  else
    Bad("String with length 55 did not compute correct MD5 hash");

  str1 = Hash::Md5("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234",56);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 16);
  if (str1.CompareTo("5570136b9e1b8015e0c70b680dd27bc3") == 0)
    Good("String with length 56 computed correct MD5 hash");
  else
    Bad("String with length 56 did not compute correct MD5 hash");

  str1 = Hash::Md5("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ12345",57);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 16);
  if (str1.CompareTo("56922f822589327cb6085e54a546674d") == 0)
    Good("String with length 57 computed correct MD5 hash");
  else
    Bad("String with length 57 did not compute correct MD5 hash");

  str1 = Hash::Md5("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456",58);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 16);
  if (str1.CompareTo("3ce164e7e334197c7ce015961905d2ae") == 0)
    Good("String with length 58 computed correct MD5 hash");
  else
    Bad("String with length 58 did not compute correct MD5 hash");

  str1 = Hash::Md5("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567",59);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 16);
  if (str1.CompareTo("1c4dccc925b7ba1e7141b86a31c9d84d") == 0)
    Good("String with length 59 computed correct MD5 hash");
  else
    Bad("String with length 59 did not compute correct MD5 hash");

  str1 = Hash::Md5("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ12345678",60);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 16);
  if (str1.CompareTo("3f52f457e44ab6b6a32937e8587b7716") == 0)
    Good("String with length 60 computed correct MD5 hash");
  else
    Bad("String with length 60 did not compute correct MD5 hash");

  str1 = Hash::Md5("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456789",61);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 16);
  if (str1.CompareTo("977a22f7e9cf60111bcc9a1d7f6d4e33") == 0)
    Good("String with length 61 computed correct MD5 hash");
  else
    Bad("String with length 61 did not compute correct MD5 hash");

  str1 = Hash::Md5("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890",62);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 16);
  if (str1.CompareTo("6e6989fd4e38b3b4944d161078cf0c92") == 0)
    Good("String with length 62 computed correct MD5 hash");
  else
    Bad("String with length 62 did not compute correct MD5 hash");

  str1 = Hash::Md5("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890<",63);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 16);
  if (str1.CompareTo("dc055feb0627a82bbac96f9002ab187e") == 0)
    Good("String with length 63 computed correct MD5 hash");
  else
    Bad("String with length 63 did not compute correct MD5 hash");

  str1 = Hash::Md5("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890<>",64);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 16);
  if (str1.CompareTo("15ed3e1afba3a8abe5e952efbf28869c") == 0)
    Good("String with length 64 computed correct MD5 hash");
  else
    Bad("String with length 64 did not compute correct MD5 hash");

  str1 = Hash::Md5("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890<>=",65);
  str1 = Convert::ToHexString((Byte*)str1.AsCharArray(), 16);
  if (str1.CompareTo("b712771fe24cba099a8bdc5bc0b01546") == 0)
    Good("String with length 65 computed correct MD5 hash");
  else
    Bad("String with length 65 did not compute correct MD5 hash");

  delete(obj);
  }
