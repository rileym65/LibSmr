#include "SmrFramework.h"

using namespace SmrFramework;

Cipher::Cipher() {
  objectType = (char*)"Cipher";
  }

Cipher::~Cipher() {
  }

void Cipher::Init(const char* cipherKey) {
  }

void Cipher::Init(const char* cipherKey, UInt32 size) {
  }

char* Cipher::Encrypt(char* inP, int len, char* dest, Int32* length) {
  return NULL;
  }

char* Cipher::Decrypt(char* inP, int len, char* dest, Int32* length) {
  return NULL;
  }



