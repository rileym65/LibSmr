#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestFile() {
  Byte b_ret;
  Int16 i16_ret;
  UInt16 u16_ret;
  Int32 i32_ret;
  UInt32 u32_ret;
  Int64 i64_ret;
  UInt64 u64_ret;
  Single s_ret;
  Double d_ret;
  String s;
  int i;
  File* file;
  file = new File();
  printf("\nInfo: File tests\n");
  if (file != NULL) Good("Was able to instantiate a File");
    else Bad("Could not instantiate a File");
  if (strcmp(file->Type(), "File") == 0)
    Good("File reported correctly on Type");
    else Bad("File did not report correctly on Type");
  if (File::Exists((char*)"Makefile")) Good("Exists showed true when file exists");
    else Bad("Exists reported false on a file that exists");
  if (!File::Exists((char*)"Makexxxx")) Good("Exists showed false when file does not exist");
    else Bad("Exists reported true on a file that does not exist");
  delete(file);

  try {
    file = File::Create((char*)"xyz.xyz");
    Good("Was able to create a file");
    }
  catch (InvalidOpException* err) {
    Bad("Exception was thrown trying to create a file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    file->Close();
    Good("File was successfully closed");
    }
  catch (InvalidOpException* err) {
    Bad("Exception was thrown trying to close file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    printf("Info: Aborting further file tests\n");
    return;
    }

  if (File::Exists((char*)"xyz.xyz")) Good("Created file does exist");
    else Bad("Created file does not exist");

  try {
    File::Delete((char*)"xyz.xyz");
    Good("Delete did not throw exception deleting file");
    }
  catch (InvalidOpException* err) {
    Bad("Exception was thrown trying to delete file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    printf("Info: Aborting further file tests\n");
    return;
    }

  if (!File::Exists((char*)"xyz.xyz")) Good("Deleted file does not exist");
    else Bad("Deleted file still exists");

  delete(file);

  try {
    file = new File("xxx.xxx");
    Good("Was able to instantiate a File with a filename");
    file->Close();
    delete(file);
    File::Delete("xxx.xxx");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to instantiate a File with a filename");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    file = new File("xy.xy",File::ReadWrite);
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to instantiate a File with a filename");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    printf("Info: Aborting further file tests\n");
    return;
    }
  
  try {
    if (file->Position() == 0) Good("File was at position 0 when opened");
    else Bad("File was not at position 0 when opened");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to get position of file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    printf("Info: Aborting further file tests\n");
    return;
    }

  i = 0;
  try {
    i += file->Write((Byte)1);
    i += file->Write((Byte)2);
    i += file->Write((Byte)3);
    i += file->Write((Byte)4);
    if (i == 4) Good("Writing 4 bytes returned count of 4");
    else Bad("Writing 4 bytes did not return count of 4");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to get write to file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    if (file->Position() == 4) Good("File position correctly shows 4");
      else Bad("File position does not show as 4");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to get file position");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    if (file->Write((Int16)0x1234) == 2)
      Good("Correct return value when writing an Int16");
    else
      Bad("Incorrect return value when writing an Int16");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to get write to file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    if (file->Write((UInt16)-10) == 2)
      Good("Correct return value when writing an UInt16");
    else
      Bad("Incorrect return value when writing an UInt16");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to get write to file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    if (file->Write((Int32)0x12345678) == 4)
      Good("Correct return value when writing an Int32");
    else
      Bad("Incorrect return value when writing an Int32");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to get write to file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    if (file->Write((UInt32)-5) == 4)
      Good("Correct return value when writing an UInt32");
    else
      Bad("Incorrect return value when writing an UInt32");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to get write to file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    if (file->Write((Int64)0x123456789abcdef0) == 8)
      Good("Correct return value when writing an Int64");
    else
      Bad("Incorrect return value when writing an Int64");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to get write to file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    if (file->Write((UInt64)-3) == 8)
      Good("Correct return value when writing an UInt64");
    else
      Bad("Incorrect return value when writing an UInt64");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to get write to file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    if (file->Write((Single)2.75) == 4)
      Good("Correct return value when writing a Single");
    else
      Bad("Incorrect return value when writing a Single");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to get write to file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    if (file->Write((Double)7.625) == 8)
      Good("Correct return value when writing a Double");
    else
      Bad("Incorrect return value when writing a Double");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to get write to file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    if (file->Reset() == 0) Good("Reset produced result of 0");
      else Bad("Reset did not produce result of 0");
    if (file->Position() == 0)
      Good("File position was 0 after Reset");
    else
      Bad("File position was not 0 after Reset");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to reset file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    b_ret = file->ReadByte();
    if (b_ret == 1) Good("Correct value read from file");
      else Bad("Incorrect value read from file");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to read file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    if (file->Position(4) == 4)
      Good("Position correctly set file position");
    else
      Bad("Position did not correcly set file position");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to position file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    if (!file->EndOfFile()) Good("File does not indicate at end when not at end");
      else Bad("File indicates at end when not at end");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown testing end of file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    i16_ret = file->ReadInt16();
    if (i16_ret == 0x1234) Good("Correctly read an Int16");
    else Bad("Could not read an Int16 correctly");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to read file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    u16_ret = file->ReadUInt16();
    if (u16_ret == 65526) Good("Correctly read an UInt16");
    else Bad("Could not read an UInt16 correctly");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to read file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    i32_ret = file->ReadInt32();
    if (i32_ret == 0x12345678) Good("Correctly read an Int32");
    else Bad("Could not read an Int32 correctly");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to read file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    u32_ret = file->ReadUInt32();
    if (u32_ret == 0xfffffffb) Good("Correctly read an UInt32");
    else Bad("Could not read an UInt32 correctly");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to read file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    i64_ret = file->ReadInt64();
    if (i64_ret == 0x123456789abcdef0) Good("Correctly read an Int64");
    else Bad("Could not read an Int64 correctly");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to read file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    u64_ret = file->ReadUInt64();
    if (u64_ret == 0xfffffffffffffffd) Good("Correctly read an UInt64");
    else Bad("Could not read an UInt64 correctly");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to read file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    s_ret = file->ReadSingle();
    if (s_ret == 2.75) Good("Correctly read a Single");
    else Bad("Could not read a Single correctly");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to read file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    d_ret = file->ReadDouble();
    if (d_ret == 7.625) Good("Correctly read a Double");
    else Bad("Could not read a Double correctly");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown trying to read file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    if (file->EndOfFile()) Good("File does indicate at end when at end");
      else Bad("File does not indicate at end when at end");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown testing end of file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }




  file->Close();
  delete(file);
  
  try {
    File::Copy("xy.xy","xyz.xyz");
    Good("Copy did not produce an acception");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown copying a file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }

  try {
    File::Move("xyz.xyz","x.y");
    Good("Move did not throw an exception");
    }
  catch (InvalidOpException* err) {
    Bad("Exception thrown moving a file");
    printf("      Excpetion: %s\n",err->Message());
    delete(err);
    delete(file);
    printf("Info: Aborting further file tests\n");
    return;
    }


  File::Delete("xy.xy");
  File::Delete("x.y");

  s = File::Basename("/test/123/abc.txt");
  if (s.Equals("abc")) Good("Basename returned correct value");
    else Bad("Basename did not return correct value");

  s = File::Extension("/test/123/abc.txt");
  if (s.Equals("txt")) Good("Extension returned correct value");
    else Bad("Extension did not return correct value");

  s = File::Path("/test/123/abc.txt");
  if (s.Equals("/test/123/")) Good("Extension returned correct value");
    else Bad("Extension did not return correct value");
  }
