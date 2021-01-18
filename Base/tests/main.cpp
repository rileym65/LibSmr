#include <stdio.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

int main(int argc, char** argv) {
  Log* log;
  log = new Log("test.log");
  log->Write("Starting tests");
  printf("Starting test suite for libSmrFramework\n\n");
  goodTests = 0;
  badTests = 0;

  try {
  TestObject();
  TestSet();
  TestStack();
  TestFileInfo();
  TestDirectory();
  TestSystem();
  TestTests();
  TestVector();
  TestTimeSpan();
  TestBase64();
  TestForthContext();
  TestForth();
  TestStreamReader();
  TestMath();
  TestFraction();
  TestDateTime();
  TestRandom();
  TestFile();
  TestQueue();
  TestList();
  TestLine();
  TestRegEx();
//  TestStopWatch();
  TestLexer();
  TestCallback();
  TestParser();
  TestDictionary();
  TestHash();
  TestHexGrid();
  TestUtils();
  TestConvert();
  TestArray();
  TestString();

  printf("\nAll test groups completed\n");
  }
  catch (InvalidOpException &err) {
    printf("\nUnexpected exception ocurred\n");
    printf("InvalidOpException: %s\n",err.Message());
    printf("All further tests not run due to unexpected exception\n");
    exit(1);
  }
  catch (...) {
    printf("\nUnexpected unknown exception ocurred\n");
    printf("All further tests not run due to unexpected exception\n");
    exit(1);
    }

  printf("\n");
  printf("Good tests: %d\n",goodTests);
  printf("Bad tests : %d\n",badTests);

  log->Write("Tests complete");
  delete(log);
  if (badTests > 0) exit(1);
  return 0;
  }

