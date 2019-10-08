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

  Tests::Tests() {
    objectType = (char*)"Tests";
    }

  Tests::~Tests() {
    }

  Boolean Tests::IsAlpha(char value) {
    if (value >= 'a' && value <= 'z') return true;
    if (value >= 'A' && value <= 'Z') return true;
    return false;
    }

  Boolean Tests::IsAlpha(const char* str) {
    if (str == NULL) return false;
    while (*str != 0) {
      if (!(*str >= 'a' && *str <= 'z') &&
          !(*str >= 'A' && *str <= 'Z')) return false;
      str++;
      }
    return true;
    }

  Boolean Tests::IsAlpha(String* str) {
    return IsAlpha(str->AsCharArray());
    }

  Boolean Tests::IsAlphaNumeric(char value) {
    if (value >= 'a' && value <= 'z') return true;
    if (value >= 'A' && value <= 'Z') return true;
    if (value >= '0' && value <= '9') return true;
    return false;
    }

  Boolean Tests::IsAlphaNumeric(const char* str) {
    if (str == NULL) return false;
    while (*str != 0) {
      if (!(*str >= '0' && *str <= '9') &&
          !(*str >= 'a' && *str <= 'z') &&
          !(*str >= 'A' && *str <= 'Z')) return false;
      str++;
      }
    return true;
    }

  Boolean Tests::IsAlphaNumeric(String* str) {
    return IsAlphaNumeric(str->AsCharArray());
    }

  Boolean Tests::IsBcd(const char* str) {
    while (*str != 0) {
      if ((*str & 0xf) > 9) return false;
      if (((*str >> 4) & 0xf) > 9) return false;
      str++;
      }
    return true;
    }

  Boolean Tests::IsBcd(String* str) {
    return IsBcd(str->AsCharArray());
    }

  Boolean Tests::IsBinary(const char* str) {
    if (str == NULL) return false;
    if (*str == '-' || *str == '+') str++;
    if (*str == 0) return false;
    while (*str != 0) {
      if (*str < '0' || *str > '1') return false;
      str++;
      }
    return true;
    }

  Boolean Tests::IsBinary(String* str) {
    return IsBinary(str->AsCharArray());
    }

  Boolean Tests::IsInteger(const char* str) {
    if (str == NULL) return false;
    if (*str == '-' || *str == '+') str++;
    if (*str == 0) return false;
    while (*str != 0) {
      if (*str < '0' || *str > '9') return false;
      str++;
      }
    return true;
    }

  Boolean Tests::IsInteger(String* str) {
    return IsInteger(str->AsCharArray());
    }

  Boolean Tests::IsFloat(const char* str) {
    Boolean dots;
    if (str == NULL) return false;
    if (*str == '-' || *str == '+') str++;
    if (*str == 0) return false;
    dots = false;
    while (*str != 0) {
      if (*str == '.') {
        if (dots) return false;
        dots = true;
        }
      else if (*str < '0' || *str > '9') return false;
      str++;
      }
    return true;
    }

  Boolean Tests::IsFloat(String* str) {
    return IsFloat(str->AsCharArray());
    }

  Boolean Tests::IsHex(const char* str) {
    if (str == NULL) return false;
    if (*str == '-' || *str == '+') str++;
    if (*str == 0) return false;
    while (*str != 0) {
      if (!(*str >= '0' && *str <= '9') &&
          !(*str >= 'a' && *str <= 'f') &&
          !(*str >= 'A' && *str <= 'F')) return false;
      str++;
      }
    return true;
    }

  Boolean Tests::IsHex(String* str) {
    return IsHex(str->AsCharArray());
    }

  Boolean Tests::IsNumeric(char value) {
    if (value >= '0' && value <= '9') return true;
    return false;
    }

  Boolean Tests::IsNumeric(const char* str) {
    if (str == NULL) return false;
    while (*str != 0) {
      if (!(*str >= '0' && *str <= '9')) return false;
      str++;
      }
    return true;
    }

  Boolean Tests::IsNumeric(String* str) {
    return IsNumeric(str->AsCharArray());
    }

  Boolean Tests::IsOctal(const char* str) {
    if (str == NULL) return false;
    if (*str == '-' || *str == '+') str++;
    if (*str == 0) return false;
    while (*str != 0) {
      if (*str < '0' || *str > '7') return false;
      str++;
      }
    return true;
    }

  Boolean Tests::IsOctal(String* str) {
    return IsOctal(str->AsCharArray());
    }

  }

