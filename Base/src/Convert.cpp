/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

/*
  *********************************************************
  ***** String to numeric special cases               *****
  ***** $string  - hexadecimal ex. $c7                *****
  ***** #string  - hexadecimal ex. #a5                *****
  ***** %string  - binary      ex. %0011_1001         *****
  ***** 0xstring - hexadecimal ex. 0x12aa             *****
  ***** 0ostring - octal       ex. 0o732              *****
  ***** 0bstring - binary      ex. 0b1001_1100        *****
  ***** _ is invisible and can be used as a separator *****
  *********************************************************
*/

#include <stdio.h>
#include <string.h>
#include <exception>
#include "SmrFramework.h"

namespace SmrFramework {

#define _FLAGS_COMMAS  1
#define _FLAGS_ZEROES  2
#define _FLAGS_PLUS    4

// **************************************************
// ***** number <-> string conversion functions *****
// **************************************************
  char* _ltoa(Int64 value, char* buffer, Byte size, Byte flags) {
    Int32 pos;
    Int64 rem;
    Int32 cnt;
    Int32 p1,p2;
    Byte negative;
    Boolean commas;
    Boolean zeroes;
    char c;
    commas = ((flags & _FLAGS_COMMAS) != 0);
    zeroes = ((flags & _FLAGS_ZEROES) != 0);
    pos = 0;
    cnt = 0;
    negative = 0;
    if (value < 0) {
      negative = 1;
      value = -value;
      buffer[pos++] = '-';
      }
    else if ((flags & _FLAGS_PLUS) != 0) {
      negative = 1;
      buffer[pos++] = '+';
      }
    while (value != 0) {
      if (commas) {
        if (cnt == 3) {
          buffer[pos++] = ',';
          cnt = 0;
          }
        cnt++;
        }
      rem = value % 10;
      value = value / 10;
      buffer[pos++] = '0' + rem;
      }
    if (size > 0) {
      while (pos < size) {
        buffer[pos++] = (zeroes) ? '0' : ' ';
        }
      }
    p1 = negative;
    p2 = pos-1;
    while (p1 < p2) {
      c = buffer[p1];
      buffer[p1] = buffer[p2];
      buffer[p2] = c;
      p1++;
      p2--;
      }
    if (pos == 0) buffer[pos++] = '0';
    buffer[pos] = 0;
    return buffer;
    }

  char* _itoa(Int32 value, char* buffer, Byte size, Byte flags) {
    Int32 pos;
    Int32 rem;
    Int32 cnt;
    Int32 p1,p2;
    Byte negative;
    Boolean commas;
    Boolean zeroes;
    char c;
    commas = ((flags & _FLAGS_COMMAS) != 0);
    zeroes = ((flags & _FLAGS_ZEROES) != 0);
    pos = 0;
    cnt = 0;
    negative = 0;
    if (value < 0) {
      negative = 1;
      value = -value;
      buffer[pos++] = '-';
      }
    else if ((flags & _FLAGS_PLUS) != 0) {
      negative = 1;
      buffer[pos++] = '+';
      }
    while (value != 0) {
      if (commas) {
        if (cnt == 3) {
          buffer[pos++] = ',';
          cnt = 0;
          }
        cnt++;
        }
      rem = value % 10;
      value = value / 10;
      buffer[pos++] = '0' + rem;
      }
    if (size > 0) {
      while (pos < size) {
        buffer[pos++] = (zeroes) ? '0' : ' ';
        }
      }
    p1 = negative;
    p2 = pos-1;
    while (p1 < p2) {
      c = buffer[p1];
      buffer[p1] = buffer[p2];
      buffer[p2] = c;
      p1++;
      p2--;
      }
    if (pos == 0) buffer[pos++] = '0';
    buffer[pos] = 0;
    return buffer;
    }

  char* _lutoa(UInt64 value, char* buffer, Byte size, Byte flags) {
    Int32 pos;
    UInt64 rem;
    UInt32 cnt;
    Int32 p1,p2;
    Boolean commas;
    Boolean zeroes;
    char c;
    commas = ((flags & _FLAGS_COMMAS) != 0);
    zeroes = ((flags & _FLAGS_ZEROES) != 0);
    pos = 0;
    cnt = 0;
    while (value != 0) {
      if (commas) {
        if (cnt == 3) {
          buffer[pos++] = ',';
          cnt = 0;
          }
        cnt++;
        }
      rem = value % 10;
      value = value / 10;
      buffer[pos++] = '0' + rem;
      }
    if (size > 0) {
      while (pos < size) {
        buffer[pos++] = (zeroes) ? '0' : ' ';
        }
      }
    p1 = 0;
    p2 = pos-1;
    while (p1 < p2) {
      c = buffer[p1];
      buffer[p1] = buffer[p2];
      buffer[p2] = c;
      p1++;
      p2--;
      }
    if (pos == 0) buffer[pos++] = '0';
    buffer[pos] = 0;
    return buffer;
    }

  char* _utoa(UInt32 value, char* buffer, Byte size, Byte flags) {
    Int32 pos;
    UInt32 rem;
    UInt32 cnt;
    Int32 p1,p2;
    Boolean commas;
    Boolean zeroes;
    char c;
    commas = ((flags & _FLAGS_COMMAS) != 0);
    zeroes = ((flags & _FLAGS_ZEROES) != 0);
    pos = 0;
    cnt = 0;
    while (value != 0) {
      if (commas) {
        if (cnt == 3) {
          buffer[pos++] = ',';
          cnt = 0;
          }
        cnt++;
        }
      rem = value % 10;
      value = value / 10;
      buffer[pos++] = '0' + rem;
      }
    if (size > 0) {
      while (pos < size) {
        buffer[pos++] = (zeroes) ? '0' : ' ';
        }
      }
    p1 = 0;
    p2 = pos-1;
    while (p1 < p2) {
      c = buffer[p1];
      buffer[p1] = buffer[p2];
      buffer[p2] = c;
      p1++;
      p2--;
      }
    if (pos == 0) buffer[pos++] = '0';
    buffer[pos] = 0;
    return buffer;
    }

  char* _ftoa(Double value, char* buffer, Byte digits, Byte flags) {
    Int32 len;
    Int64 whole, frac;
    Int32 d;
    Double e;
    Double tmp;
    Int32 cnt;
    Boolean dynamic;
    dynamic = (digits == 0);
    e = 1;
    d = digits;
    if (d == 0) {
      tmp = value - ((int)value);
      cnt = 0;
      while (cnt < 8 && tmp != 0) {
        cnt++;
        digits++;
        e *= 10;
        tmp *= 10;
        tmp = tmp - ((int)tmp);
        }
      }
    else {
      while (d > 0) {
        e *= 10;
        d--;
        }
      }
    whole = (Int64)value;
    frac = (Int64)((value - whole) * e);
    if (frac < 0) frac = -frac;
    _ltoa(whole, buffer, 0, flags);
    len = strlen(buffer);
    if (frac != 0) {
      buffer[len++] = '.';
      buffer[len] = 0;
      _ltoa(frac, buffer+len, digits, _FLAGS_ZEROES);
      if (dynamic) {
        len = strlen(buffer);
        while (len > 0 && buffer[len-1] == '0') {
          buffer[--len] = 0;
          }
        }
      }
    return buffer;
    }

  char* _itoa_formatted(Int32 value, char* buffer, Byte size, Byte flags, const char* format) {
    char conv;
    if (format == NULL)
      throw NullException("Format was null");
    conv = 'D';
    while (*format != 0) {
      if (*format == 'd') conv = 'D';
      else if (*format == 'D') conv = 'D';
      else if (*format == 'x') conv = 'x';
      else if (*format == 'X') conv = 'X';
      else if (*format == 'f') conv = 'D';
      else if (*format == 'F') conv = 'D';
      else if (*format == '0') flags |= _FLAGS_ZEROES;
      else if (*format == ',') flags |= _FLAGS_COMMAS;
      else if (*format == '+') flags |= _FLAGS_PLUS;
      else if (*format >= '1' && *format <= '9') {
        size = 0;
        while (*format >='0' && *format <= '9') {
          size = size * 10 + (*format - '0');
          format++;
          }
        format--;
        }
      else throw InvalidOpException("Invalid character found in format");
      format++;
      }
    if (conv == 'D') return _itoa(value, buffer, size, flags);
    return NULL;
    }

  char* _utoa_formatted(UInt32 value, char* buffer, Byte size, Byte flags, const char* format) {
    char conv;
    if (format == NULL)
      throw NullException("Format was null");
    conv = 'D';
    while (*format != 0) {
      if (*format == 'd') conv = 'D';
      else if (*format == 'D') conv = 'D';
      else if (*format == 'x') conv = 'x';
      else if (*format == 'X') conv = 'X';
      else if (*format == 'f') conv = 'D';
      else if (*format == 'F') conv = 'D';
      else if (*format == '0') flags |= _FLAGS_ZEROES;
      else if (*format == ',') flags |= _FLAGS_COMMAS;
      else if (*format == '+') flags |= _FLAGS_PLUS;
      else if (*format >= '1' && *format <= '9') {
        size = 0;
        while (*format >='0' && *format <= '9') {
          size = size * 10 + (*format - '0');
          format++;
          }
        format--;
        }
      else throw InvalidOpException("Invalid character found in format");
      format++;
      }
    if (conv == 'D') return _utoa(value, buffer, size, flags);
    return NULL;
    }

  char* _ftoa_formatted(Double value, char* buffer, Byte size, Byte flags, const char* format) {
    char conv;
    if (format == NULL)
      throw NullException("Format was null");
    conv = 'F';
    while (*format != 0) {
      if (*format == 'd') conv = 'D';
      else if (*format == 'D') conv = 'D';
      else if (*format == 'x') conv = 'X';
      else if (*format == 'X') conv = 'X';
      else if (*format == 'f') conv = 'F';
      else if (*format == 'F') conv = 'F';
      else if (*format == '0') flags |= _FLAGS_ZEROES;
      else if (*format == ',') flags |= _FLAGS_COMMAS;
      else if (*format == '+') flags |= _FLAGS_PLUS;
      else if (*format >= '1' && *format <= '9') {
        size = 0;
        while (*format >='0' && *format <= '9') {
          size = size * 10 + (*format - '0');
          format++;
          }
        format--;
        }
      else throw InvalidOpException("Invalid character found in format");
      format++;
      }
    if (conv == 'D') return _itoa((Int32)value, buffer, size, flags);
    if (conv == 'F') return _ftoa(value, buffer, size, flags);
    return NULL;
    }

  UInt64 _StringToInt(const char* buffer) {
    UInt64 ret;
    char numType;
    numType = 'D';
    if (buffer[0] == '$') {
      numType = 'H';
      buffer++;
      }
    if (buffer[0] == '%') {
      numType = 'B';
      buffer++;
      }
    if (buffer[0] == '#') {
      numType = 'H';
      buffer++;
      }
    if (buffer[0] == '0' && (buffer[1] == 'x' || buffer[1] == 'X')) {
      numType = 'H';
      buffer += 2;
      }
    if (buffer[0] == '0' && buffer[1] == 'o') {
      numType = 'O';
      buffer += 2;
      }
    if (buffer[0] == '0' && (buffer[1] == 'b' || buffer[1] == 'B')) {
      numType = 'B';
      buffer += 2;
      }
    ret = 0;
    if (numType == 'H') {
      while (*buffer != ' ' && *buffer != 9 && *buffer != 0) {
        if (*buffer >= '0' && *buffer <= '9')
          ret = (ret << 4) | (*buffer - '0');
        else if (*buffer >= 'A' && *buffer <= 'F')
          ret = (ret << 4) | (*buffer - 'A' + 10);
        else if (*buffer >= 'a' && *buffer <= 'f')
          ret = (ret << 4) | (*buffer - 'a' + 10);
        else if (*buffer != '_' && *buffer != ',')
          throw InvalidOpException("Invalid character found during numeric conversion");
        buffer++;
        }
      }
    if (numType == 'D') {
      while (*buffer != ' ' && *buffer != 9 && *buffer != 0) {
        if (*buffer >= '0' && *buffer <= '9')
          ret = (ret * 10) + (*buffer - '0');
        else if (*buffer != '_' && *buffer != ',')
          throw InvalidOpException("Invalid character found during numeric conversion");
        buffer++;
        }
      }
    if (numType == 'O') {
      while (*buffer != ' ' && *buffer != 9 && *buffer != 0) {
        if (*buffer >= '0' && *buffer <= '7')
          ret = (ret << 3) | (*buffer - '0');
        else if (*buffer != '_' && *buffer != ',')
          throw InvalidOpException("Invalid character found during numeric conversion");
        buffer++;
        }
      }
    if (numType == 'B') {
      while (*buffer != ' ' && *buffer != 9 && *buffer != 0) {
        if (*buffer >= '0' && *buffer <= '1')
          ret = (ret << 1) | (*buffer - '0');
        else if (*buffer != '_' && *buffer != ',')
          throw InvalidOpException("Invalid character found during numeric conversion");
        buffer++;
        }
      }
    return ret;
    }

  Int64 _StringToInt64(const char* buffer) {
    UInt64 value;
    Boolean negative;
    negative = false;
    if (*buffer == '-') {
      negative = true;
      buffer++;
      }
    else if (*buffer == '+') {
      negative = false;
      buffer++;
      }
    value =  _StringToInt(buffer);
    if (value >= 0x8000000000000000)
      throw RangeException("Number exeeded range for signed Int64");
    if (negative) {
      return 0-(Int64)value;
      }
    return value;
    }

  Int64 _StringToUInt64(const char* buffer) {
    UInt64 value;
    if (*buffer == '-') {
      throw InvalidOpException("Negative number found on unsigned conversion");
      }
    else if (*buffer == '+') {
      buffer++;
      }
    value =  _StringToInt(buffer);
    return value;
    }

  Int32 _StringToInt32(const char* buffer) {
    UInt64 value;
    UInt32 value32;
    Boolean negative;
    negative = false;
    if (*buffer == '-') {
      negative = true;
      buffer++;
      }
    else if (*buffer == '+') {
      negative = false;
      buffer++;
      }
    value =  _StringToInt(buffer);
    if (value >= 0x80000000)
      throw RangeException("Number exeeded range for signed Int32");
    value32 = (value & 0xffffffff);
    if (negative) {
      return 0-(Int32)value32;
      }
    return value32;
    }

  UInt32 _StringToUInt32(const char* buffer) {
    UInt64 value;
    if (*buffer == '-') {
      throw InvalidOpException("Negative number found on unsigned conversion");
      }
    else if (*buffer == '+') {
      buffer++;
      }
    value =  _StringToInt(buffer);
    if (value > 0xffffffff)
      throw RangeException("Number exeeded range for unsigned Int32");
    return (value & 0xffffffff);
    }

  Int16 _StringToInt16(const char* buffer) {
    UInt64 value;
    UInt16 value16;
    Boolean negative;
    negative = false;
    if (*buffer == '-') {
      negative = true;
      buffer++;
      }
    else if (*buffer == '+') {
      negative = false;
      buffer++;
      }
    value =  _StringToInt(buffer);
    if (value >= 0x8000)
      throw RangeException("Number exeeded range for signed Int16");
    value16 = (value & 0xffff);
    if (negative) {
      return 0-(Int16)value16;
      }
    return value16;
    }

  UInt16 _StringToUInt16(const char* buffer) {
    UInt64 value;
    if (*buffer == '-') {
      throw InvalidOpException("Negative number found on unsigned conversion");
      }
    else if (*buffer == '+') {
      buffer++;
      }
    value =  _StringToInt(buffer);
    if (value > 0xffff)
      throw RangeException("Number exeeded range for unsigned Int16");
    return (value & 0xffff);
    }

  Int8 _StringToInt8(const char* buffer) {
    UInt64 value;
    UInt8 value8;
    Boolean negative;
    negative = false;
    if (*buffer == '-') {
      negative = true;
      buffer++;
      }
    else if (*buffer == '+') {
      negative = false;
      buffer++;
      }
    value =  _StringToInt(buffer);
    if (value >= 0x80)
      throw RangeException("Number exeeded range for signed Int8");
    value8 = (value & 0xff);
    if (negative) {
      return 0-(Int8)value8;
      }
    return value8;
    }

  UInt8 _StringToUInt8(const char* buffer) {
    UInt64 value;
    if (*buffer == '-') {
      throw InvalidOpException("Negative number found on unsigned conversion");
      }
    else if (*buffer == '+') {
      buffer++;
      }
    value =  _StringToInt(buffer);
    if (value > 0xff)
      throw RangeException("Number exeeded range for unsigned Int8");
    return (value & 0xff);
    }

// **************************************
// ***** Actual Convert object code *****
// **************************************
  
  Convert::Convert() {
    }

  Convert::~Convert() {
    }

// ******************
// ***** ToChar *****
// ******************

  char Convert::ToChar(char value) {
    return value;
    }

  char Convert::ToChar(const char* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    if (strlen(value) != 1) throw FormatException(NULL, "Length was not 1");
    return value[0];
    }

  char Convert::ToChar(String* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    if (value->Length() != 1) throw FormatException(NULL, "Length was not 1");
    return value->AsCharArray()[0];
    }

  char Convert::ToChar(String value) {
    if (value.Length() == 0) throw NullException(NULL, "Value is null");
    if (value.Length() != 1) throw FormatException(NULL, "Length was not 1");
    return value.AsCharArray()[0];
    }

  char Convert::ToChar(UInt8 value) {
    return (char)(value & 0xff);
    }

  char Convert::ToChar(UInt16 value) {
    if (value < 0) throw RangeException(NULL, "Negative value");
    if (value > 255) throw RangeException(NULL, "Value exceeds 255");
    return (char)(value & 0xff);
    }

  char Convert::ToChar(UInt32 value) {
    if (value < 0) throw RangeException(NULL, "Negative value");
    if (value > 255) throw RangeException(NULL, "Value exceeds 255");
    return (char)(value & 0xff);
    }

  char Convert::ToChar(UInt64 value) {
    if (value < 0) throw RangeException(NULL, "Negative value");
    if (value > 255) throw RangeException(NULL, "Value exceeds 255");
    return (char)(value & 0xff);
    }

  char Convert::ToChar(Int8 value) {
    return (char)(value & 0xff);
    }

  char Convert::ToChar(Int16 value) {
    if (value < 0) throw RangeException(NULL, "Negative value");
    if (value > 255) throw RangeException(NULL, "Value exceeds 255");
    return (char)(value & 0xff);
    }

  char Convert::ToChar(Int32 value) {
    if (value < 0) throw RangeException(NULL, "Negative value");
    if (value > 255) throw RangeException(NULL, "Value exceeds 255");
    return (char)(value & 0xff);
    }

  char Convert::ToChar(Int64 value) {
    if (value < 0) throw RangeException(NULL, "Negative value");
    if (value > 255) throw RangeException(NULL, "Value exceeds 255");
    return (char)(value & 0xff);
    }

  char Convert::ToChar(Double value) {
    int v;
    v = (int)value;
    if (v < 0) throw RangeException(NULL, "Negative value");
    if (v > 255) throw RangeException(NULL, "Value exceeds 255");
    return (char)(v & 0xff);
    }

  char Convert::ToChar(Single value) {
    int v;
    v = (int)value;
    if (v < 0) throw RangeException(NULL, "Negative value");
    if (v > 255) throw RangeException(NULL, "Value exceeds 255");
    return (char)(v & 0xff);
    }

  char Convert::ToChar(Fraction value) {
    int v;
    v = value.Whole();
    if (value.Negative()) v = -v;
    if (v< 0) throw RangeException(NULL, "Negative value");
    if (v> 255) throw RangeException(NULL, "Value exceeds 255");
    return (char)(v& 0xff);
    }

// ******************
// ***** ToInt8 *****
// ******************

  Int8 Convert::ToInt8(char value) {
    return (Int8)value;
    }

  Int8 Convert::ToInt8(const char* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return _StringToInt8(value);
    }

  Int8 Convert::ToInt8(String* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return _StringToInt8(value->AsCharArray());
    }

  Int8 Convert::ToInt8(String value) {
    if (value.Length() == 0) throw NullException(NULL, "Value is null");
    return _StringToInt8(value.AsCharArray());
    }

  Int8 Convert::ToInt8(UInt8 value) {
    if (value > 127) throw RangeException(NULL, "Range outside -128 to +127");
    return (Int8)value;
    }

  Int8 Convert::ToInt8(UInt16 value) {
    if (value > 127) throw RangeException(NULL, "Range outside -128 to +127");
    return (Int8)value;
    }

  Int8 Convert::ToInt8(UInt32 value) {
    if (value > 127) throw RangeException(NULL, "Range outside -128 to +127");
    return (Int8)value;
    }

  Int8 Convert::ToInt8(UInt64 value) {
    if (value > 127) throw RangeException(NULL, "Range outside -128 to +127");
    return (Int8)value;
    }

  Int8 Convert::ToInt8(Int8 value) {
    return value;
    }

  Int8 Convert::ToInt8(Int16 value) {
    if (value > 127) throw RangeException(NULL, "Range outside -128 to +127");
    if (value < -128) throw RangeException(NULL, "Range outside -128 to +127");
    return (Int8)value;
    }

  Int8 Convert::ToInt8(Int32 value) {
    if (value > 127) throw RangeException(NULL, "Range outside -128 to +127");
    if (value < -128) throw RangeException(NULL, "Range outside -128 to +127");
    return (Int8)value;
    }

  Int8 Convert::ToInt8(Int64 value) {
    if (value > 127) throw RangeException(NULL, "Range outside -128 to +127");
    if (value < -128) throw RangeException(NULL, "Range outside -128 to +127");
    return (Int8)value;
    }

  Int8 Convert::ToInt8(Double value) {
    Int32 v;
    v = (Int32)value;
    if (v> 127) throw RangeException(NULL, "Range outside -128 to +127");
    if (v< -128) throw RangeException(NULL, "Range outside -128 to +127");
    return (Int8)v;
    }

  Int8 Convert::ToInt8(Single value) {
    Int32 v;
    v = (Int32)value;
    if (v> 127) throw RangeException(NULL, "Range outside -128 to +127");
    if (v< -128) throw RangeException(NULL, "Range outside -128 to +127");
    return (Int8)v;
    }

  Int8 Convert::ToInt8(Fraction value) {
    Int32 v;
    v = value.Whole();
    if (value.Negative()) v = -v;
    if (v> 127) throw RangeException(NULL, "Range outside -128 to +127");
    if (v< -128) throw RangeException(NULL, "Range outside -128 to +127");
    return (Int8)v;
    }

// *******************
// ***** ToInt16 *****
// *******************

  Int16 Convert::ToInt16(char value) {
    return (Int16)value;
    }

  Int16 Convert::ToInt16(const char* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return _StringToInt16(value);
    }

  Int16 Convert::ToInt16(String* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return _StringToInt16(value->AsCharArray());
    }

  Int16 Convert::ToInt16(String value) {
    if (value.Length() == 0) throw NullException(NULL, "Value is null");
    return _StringToInt16(value.AsCharArray());
    }

  Int16 Convert::ToInt16(UInt8 value) {
    if (value > 32767) throw RangeException(NULL, "Range outside -32768 to +32767");
    return (Int16)value;
    }

  Int16 Convert::ToInt16(UInt16 value) {
    if (value > 32767) throw RangeException(NULL, "Range outside -32768 to +32767");
    return (Int16)value;
    }

  Int16 Convert::ToInt16(UInt32 value) {
    if (value > 32767) throw RangeException(NULL, "Range outside -32768 to +32767");
    return (Int16)value;
    }

  Int16 Convert::ToInt16(UInt64 value) {
    if (value > 32767) throw RangeException(NULL, "Range outside -32768 to +32767");
    return (Int16)value;
    }

  Int16 Convert::ToInt16(Int8 value) {
    return (Int16)value;
    }

  Int16 Convert::ToInt16(Int16 value) {
    return value;
    }

  Int16 Convert::ToInt16(Int32 value) {
    if (value > 32767) throw RangeException(NULL, "Range outside -32768 to +32767");
    if (value < -32768) throw RangeException(NULL, "Range outside -32768 to +32767");
    return (Int16)value;
    }

  Int16 Convert::ToInt16(Int64 value) {
    if (value > 32767) throw RangeException(NULL, "Range outside -32768 to +32767");
    if (value < -32768) throw RangeException(NULL, "Range outside -32768 to +32767");
    return (Int16)value;
    }

  Int16 Convert::ToInt16(Double value) {
    Int32 v;
    v = (Int32)value;
    if (v> 32767) throw RangeException(NULL, "Range outside -32768 to +32767");
    if (v< -32768) throw RangeException(NULL, "Range outside -32768 to +32767");
    return (Int16)v;
    }

  Int16 Convert::ToInt16(Single value) {
    Int32 v;
    v = (Int32)value;
    if (v> 32767) throw RangeException(NULL, "Range outside -32768 to +32767");
    if (v< -32768) throw RangeException(NULL, "Range outside -32768 to +32767");
    return (Int16)v;
    }

  Int16 Convert::ToInt16(Fraction value) {
    Int32 v;
    v = value.Whole();
    if (value.Negative()) v = -v;
    if (v> 32767) throw RangeException(NULL, "Range outside -32768 to +32767");
    if (v< -32768) throw RangeException(NULL, "Range outside -32768 to +32767");
    return (Int16)v;
    }

// *******************
// ***** ToInt32 *****
// *******************

  Int32 Convert::ToInt32(char value) {
    return (Int32)value;
    }

  Int32 Convert::ToInt32(const char* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return _StringToInt32(value);
    }

  Int32 Convert::ToInt32(String* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return _StringToInt32(value->AsCharArray());
    }

  Int32 Convert::ToInt32(String value) {
    if (value.Length() == 0) throw NullException(NULL, "Value is null");
    return _StringToInt32(value.AsCharArray());
    }

  Int32 Convert::ToInt32(UInt8 value) {
    if (value > 2147483647) throw RangeException(NULL, "Range outside -2147483648 to +2147483647");
    return (Int32)value;
    }

  Int32 Convert::ToInt32(UInt16 value) {
    if (value > 2147483647) throw RangeException(NULL, "Range outside -2147483648 to +2147483647");
    return (Int32)value;
    }

  Int32 Convert::ToInt32(UInt32 value) {
    if (value > 2147483647) throw RangeException(NULL, "Range outside -2147483648 to +2147483647");
    return (Int32)value;
    }

  Int32 Convert::ToInt32(UInt64 value) {
    if (value > 2147483647) throw RangeException(NULL, "Range outside -2147483648 to +2147483647");
    return (Int32)value;
    }

  Int32 Convert::ToInt32(Int8 value) {
    return (Int32)value;
    }

  Int32 Convert::ToInt32(Int16 value) {
    return (Int32)value;
    }

  Int32 Convert::ToInt32(Int32 value) {
    return (Int32)value;
    }

  Int32 Convert::ToInt32(Int64 value) {
    if (value > 2147483647) throw RangeException(NULL, "Range outside -2147483648 to +2147483647");
    if ((UInt64)value >= 0x8000000000000000 && (UInt64)value <= 0xffffffff7fffffff)
      throw RangeException(NULL, "Range outside -2147483648 to +2147483647");
    return (Int32)value;
    }

  Int32 Convert::ToInt32(Double value) {
    Int64 v;
    v = (Int64)value;
    if (v> 2147483647) throw RangeException(NULL, "Range outside -2147483648 to +2147483647");
    if ((UInt64)value >= 0x8000000000000000 && (UInt64)value <= 0xffffffff7fffffff)
      throw RangeException(NULL, "Range outside -2147483648 to +2147483647");
    return (Int32)v;
    }

  Int32 Convert::ToInt32(Single value) {
    Int64 v;
    v = (Int64)value;
    if (v> 2147483647) throw RangeException(NULL, "Range outside -2147483648 to +2147483647");
    if ((UInt64)value >= 0x8000000000000000 && (UInt64)value <= 0xffffffff7fffffff)
      throw RangeException(NULL, "Range outside -2147483648 to +2147483647");
    return (Int32)v;
    }

  Int32 Convert::ToInt32(Fraction value) {
    Int32 v;
    v = value.Whole();
    if (value.Negative()) v = -v;
    return (Int32)v;
    }

// *******************
// ***** ToInt64 *****
// *******************

  Int64 Convert::ToInt64(char value) {
    return (Int64)value;
    }

  Int64 Convert::ToInt64(const char* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return _StringToInt64(value);
    }

  Int64 Convert::ToInt64(String* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return _StringToInt64(value->AsCharArray());
    }

  Int64 Convert::ToInt64(String value) {
    if (value.Length() == 0) throw NullException(NULL, "Value is null");
    return _StringToInt64(value.AsCharArray());
    }

  Int64 Convert::ToInt64(UInt8 value) {
    return (Int64)value;
    }

  Int64 Convert::ToInt64(UInt16 value) {
    return (Int64)value;
    }

  Int64 Convert::ToInt64(UInt32 value) {
    return (Int64)value;
    }

  Int64 Convert::ToInt64(UInt64 value) {
    if (value > 0x8000000000000000) throw RangeException(NULL, "Out of range");
    return (Int64)value;
    }

  Int64 Convert::ToInt64(Int8 value) {
    return (Int64)value;
    }

  Int64 Convert::ToInt64(Int16 value) {
    return (Int64)value;
    }

  Int64 Convert::ToInt64(Int32 value) {
    return (Int64)value;
    }

  Int64 Convert::ToInt64(Int64 value) {
    return (Int64)value;
    }

  Int64 Convert::ToInt64(Double value) {
    Int64 v;
    v = (Int64)value;
    return (Int64)v;
    }

  Int64 Convert::ToInt64(Single value) {
    Int64 v;
    v = (Int64)value;
    return (Int64)v;
    }

  Int64 Convert::ToInt64(Fraction value) {
    Int32 v;
    v = value.Whole();
    if (value.Negative()) v = -v;
    return (Int64)v;
    }


// ******************
// ***** ToUInt8 *****
// ******************

  UInt8 Convert::ToUInt8(char value) {
    if (value > 127) throw RangeException(NULL, "Range outside 0 to 255");
    return (UInt8)value;
    }

  UInt8 Convert::ToUInt8(const char* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return _StringToUInt8(value);
    }

  UInt8 Convert::ToUInt8(String* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return _StringToUInt8(value->AsCharArray());
    }

  UInt8 Convert::ToUInt8(String value) {
    if (value.Length() == 0) throw NullException(NULL, "Value is null");
    return _StringToUInt8(value.AsCharArray());
    }

  UInt8 Convert::ToUInt8(UInt8 value) {
    if (value > 255) throw RangeException(NULL, "Range outside 0 to 255");
    return (UInt8)value;
    }

  UInt8 Convert::ToUInt8(UInt16 value) {
    if (value > 255) throw RangeException(NULL, "Range outside 0 to 255");
    return (UInt8)value;
    }

  UInt8 Convert::ToUInt8(UInt32 value) {
    if (value > 255) throw RangeException(NULL, "Range outside 0 to 255");
    return (UInt8)value;
    }

  UInt8 Convert::ToUInt8(UInt64 value) {
    if (value > 255) throw RangeException(NULL, "Range outside 0 to 255");
    return (UInt8)value;
    }

  UInt8 Convert::ToUInt8(Int8 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 255");
    if (value > 127) throw RangeException(NULL, "Range outside 0 to 255");
    return (UInt8)value;
    }

  UInt8 Convert::ToUInt8(Int16 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 255");
    if (value > 255) throw RangeException(NULL, "Range outside 0 to 255");
    return (UInt8)value;
    }

  UInt8 Convert::ToUInt8(Int32 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 255");
    if (value > 255) throw RangeException(NULL, "Range outside 0 to 255");
    return (UInt8)value;
    }

  UInt8 Convert::ToUInt8(Int64 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 255");
    if (value > 255) throw RangeException(NULL, "Range outside 0 to 255");
    return (UInt8)value;
    }

  UInt8 Convert::ToUInt8(Double value) {
    Int32 v;
    v = (Int32)value;
    if (v < 0) throw RangeException(NULL, "Range outside 0 to 255");
    if (v > 255) throw RangeException(NULL, "Range outside 0 to 255");
    return (UInt8)v;
    }

  UInt8 Convert::ToUInt8(Single value) {
    Int32 v;
    v = (Int32)value;
    if (v < 0) throw RangeException(NULL, "Range outside 0 to 255");
    if (v > 255) throw RangeException(NULL, "Range outside 0 to 255");
    return (UInt8)v;
    }

  UInt8 Convert::ToUInt8(Fraction value) {
    int v;
    v = value.Whole();
    if (value.Negative()) v = -v;
    if (v< 0) throw RangeException(NULL, "Negative value");
    if (v> 255) throw RangeException(NULL, "Value exceeds 255");
    return (UInt8)(v& 0xff);
    }

// ********************
// ***** ToUInt16 *****
// ********************

  UInt16 Convert::ToUInt16(char value) {
    if (value > 127) throw RangeException(NULL, "Range outside 0 to 65535");
    return (UInt16)value;
    }

  UInt16 Convert::ToUInt16(const char* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return _StringToUInt16(value);
    }

  UInt16 Convert::ToUInt16(String* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return _StringToUInt16(value->AsCharArray());
    }

  UInt16 Convert::ToUInt16(String value) {
    if (value.Length() == 0) throw NullException(NULL, "Value is null");
    return _StringToUInt16(value.AsCharArray());
    }

  UInt16 Convert::ToUInt16(UInt8 value) {
    return (UInt16)value;
    }

  UInt16 Convert::ToUInt16(UInt16 value) {
    return (UInt16)value;
    }

  UInt16 Convert::ToUInt16(UInt32 value) {
    if (value > 65535) throw RangeException(NULL, "Range outside 0 to 65535");
    return (UInt16)value;
    }

  UInt16 Convert::ToUInt16(UInt64 value) {
    if (value > 65535) throw RangeException(NULL, "Range outside 0 to 65535");
    return (UInt16)value;
    }

  UInt16 Convert::ToUInt16(Int8 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 65535");
    if (value > 65535) throw RangeException(NULL, "Range outside 0 to 65535");
    return (UInt16)value;
    }

  UInt16 Convert::ToUInt16(Int16 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 65535");
    if (value > 65535) throw RangeException(NULL, "Range outside 0 to 65535");
    return (UInt16)value;
    }

  UInt16 Convert::ToUInt16(Int32 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 65535");
    if (value > 65535) throw RangeException(NULL, "Range outside 0 to 65535");
    return (UInt16)value;
    }

  UInt16 Convert::ToUInt16(Int64 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 65535");
    if (value > 65535) throw RangeException(NULL, "Range outside 0 to 65535");
    return (UInt16)value;
    }

  UInt16 Convert::ToUInt16(Double value) {
    Int32 v;
    v = (Int32)value;
    if (v < 0) throw RangeException(NULL, "Range outside 0 to 65535");
    if (v > 65535) throw RangeException(NULL, "Range outside 0 to 65535");
    return (UInt16)v;
    }

  UInt16 Convert::ToUInt16(Single value) {
    Int32 v;
    v = (Int32)value;
    if (v < 0) throw RangeException(NULL, "Range outside 0 to 65535");
    if (v > 65535) throw RangeException(NULL, "Range outside 0 to 65535");
    return (UInt16)v;
    }

  UInt16 Convert::ToUInt16(Fraction value) {
    int v;
    v = value.Whole();
    if (value.Negative()) v = -v;
    if (v < 0) throw RangeException(NULL, "Range outside 0 to 65535");
    if (v > 65535) throw RangeException(NULL, "Range outside 0 to 65535");
    return (UInt16)v;
    }

// ********************
// ***** ToUInt32 *****
// ********************

  UInt32 Convert::ToUInt32(char value) {
    if (value > 127) throw RangeException(NULL, "Range outside 0 to 4294967295");
    return (UInt32)value;
    }

  UInt32 Convert::ToUInt32(const char* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return _StringToUInt32(value);
    }

  UInt32 Convert::ToUInt32(String* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return _StringToUInt32(value->AsCharArray());
    }

  UInt32 Convert::ToUInt32(String value) {
    if (value.Length() == 0) throw NullException(NULL, "Value is null");
    return _StringToUInt32(value.AsCharArray());
    }

  UInt32 Convert::ToUInt32(UInt8 value) {
    return (UInt32)value;
    }

  UInt32 Convert::ToUInt32(UInt16 value) {
    return (UInt32)value;
    }

  UInt32 Convert::ToUInt32(UInt32 value) {
    return (UInt32)value;
    }

  UInt32 Convert::ToUInt32(UInt64 value) {
    if (value > 0xffffffff) throw RangeException(NULL, "Range outside 0 to 4294967295");
    return (UInt32)value;
    }

  UInt32 Convert::ToUInt32(Int8 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 4294967295");
    return (UInt32)value;
    }

  UInt32 Convert::ToUInt32(Int16 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 4294967295");
    return (UInt32)value;
    }

  UInt32 Convert::ToUInt32(Int32 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 4294967295");
    return (UInt32)value;
    }

  UInt32 Convert::ToUInt32(Int64 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 4294967295");
    if (value > 0xffffffff) throw RangeException(NULL, "Range outside 0 to 4294967295");
    return (UInt32)value;
    }

  UInt32 Convert::ToUInt32(Double value) {
    Int64 v;
    v = (Int64)value;
    if (v < 0) throw RangeException(NULL, "Range outside 0 to 4294967295");
    if (v > (Int64)0xffffffff) throw RangeException(NULL, "Range outside 0 to 4294967295");
    return (UInt32)v;
    }

  UInt32 Convert::ToUInt32(Single value) {
    Int64 v;
    v = (Int64)value;
    if (v < 0) throw RangeException(NULL, "Range outside 0 to 4294967295");
    if (v > (Int64)0xffffffff) throw RangeException(NULL, "Range outside 0 to 4294967295");
    return (UInt32)v;
    }

  UInt32 Convert::ToUInt32(Fraction value) {
    int v;
    v = value.Whole();
    if (value.Negative()) v = -v;
    if (v < 0) throw RangeException(NULL, "Range outside 0 to 4294967295");
    if (v > (Int64)0xffffffff) throw RangeException(NULL, "Range outside 0 to 4294967295");
    return (UInt32)v;
    }

// ********************
// ***** ToUInt64 *****
// ********************

  UInt64 Convert::ToUInt64(char value) {
    if (value > 127) throw RangeException(NULL, "Range outside 0 to 4294967295");
    return (UInt64)value;
    }

  UInt64 Convert::ToUInt64(const char* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return _StringToUInt64(value);
    }

  UInt64 Convert::ToUInt64(String* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return _StringToUInt64(value->AsCharArray());
    }

  UInt64 Convert::ToUInt64(String value) {
    if (value.Length() == 0) throw NullException(NULL, "Value is null");
    return _StringToUInt64(value.AsCharArray());
    }

  UInt64 Convert::ToUInt64(UInt8 value) {
    return (UInt64)value;
    }

  UInt64 Convert::ToUInt64(UInt16 value) {
    return (UInt64)value;
    }

  UInt64 Convert::ToUInt64(UInt32 value) {
    return (UInt64)value;
    }

  UInt64 Convert::ToUInt64(UInt64 value) {
    return (UInt64)value;
    }

  UInt64 Convert::ToUInt64(Int8 value) {
    if (value < 0) throw RangeException(NULL, "Range error");
    return (UInt64)value;
    }

  UInt64 Convert::ToUInt64(Int16 value) {
    if (value < 0) throw RangeException(NULL, "Range error");
    return (UInt64)value;
    }

  UInt64 Convert::ToUInt64(Int32 value) {
    if (value < 0) throw RangeException(NULL, "Range error");
    return (UInt64)value;
    }

  UInt64 Convert::ToUInt64(Int64 value) {
    if (value < 0) throw RangeException(NULL, "Range error");
    return (UInt64)value;
    }

  UInt64 Convert::ToUInt64(Double value) {
    Int64 v;
    v = (Int64)value;
    if (v < 0) throw RangeException(NULL, "Range error");
    return (UInt64)v;
    }

  UInt64 Convert::ToUInt64(Single value) {
    Int64 v;
    v = (Int64)value;
    if (v < 0) throw RangeException(NULL, "Range error");
    return (UInt64)v;
    }

  UInt64 Convert::ToUInt64(Fraction value) {
    int v;
    v = value.Whole();
    if (value.Negative()) v = -v;
    return (UInt64)v;
    }


// ******************
// ***** ToByte *****
// ******************

  Byte Convert::ToByte(char value) {
    return (Byte)value;
    }

  Byte Convert::ToByte(const char* value) {
    Int32 tmp;
    if (value == NULL) throw NullException(NULL, "Value is null");
    sscanf(value,"%d",&tmp);
    if (tmp > 255) throw RangeException(NULL, "Range outside 0 to 255");
    return tmp;
    }

  Byte Convert::ToByte(String* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return ToByte(value->AsCharArray());
    }

  Byte Convert::ToByte(String value) {
    if (value.Length() == 0) throw NullException(NULL, "Value is null");
    return ToByte(value.AsCharArray());
    }

  Byte Convert::ToByte(UInt8 value) {
    if (value > 255) throw RangeException(NULL, "Range outside 0 to 255");
    return (Byte)value;
    }

  Byte Convert::ToByte(UInt16 value) {
    if (value > 255) throw RangeException(NULL, "Range outside 0 to 255");
    return (Byte)value;
    }

  Byte Convert::ToByte(UInt32 value) {
    if (value > 255) throw RangeException(NULL, "Range outside 0 to 255");
    return (Byte)value;
    }

  Byte Convert::ToByte(UInt64 value) {
    if (value > 255) throw RangeException(NULL, "Range outside 0 to 255");
    return (Byte)value;
    }

  Byte Convert::ToByte(Int8 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 255");
    if (value > 255) throw RangeException(NULL, "Range outside 0 to 255");
    return (Byte)value;
    }

  Byte Convert::ToByte(Int16 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 255");
    if (value > 255) throw RangeException(NULL, "Range outside 0 to 255");
    return (Byte)value;
    }

  Byte Convert::ToByte(Int32 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 255");
    if (value > 255) throw RangeException(NULL, "Range outside 0 to 255");
    return (Byte)value;
    }

  Byte Convert::ToByte(Int64 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 255");
    if (value > 255) throw RangeException(NULL, "Range outside 0 to 255");
    return (Byte)value;
    }

  Byte Convert::ToByte(Double value) {
    Int32 v;
    v = (Int32)value;
    if (v < 0) throw RangeException(NULL, "Range outside 0 to 255");
    if (v > 255) throw RangeException(NULL, "Range outside 0 to 255");
    return (Byte)v;
    }

  Byte Convert::ToByte(Single value) {
    Int32 v;
    v = (Int32)value;
    if (v < 0) throw RangeException(NULL, "Range outside 0 to 255");
    if (v > 255) throw RangeException(NULL, "Range outside 0 to 255");
    return (Byte)v;
    }

  Byte Convert::ToByte(Fraction value) {
    int v;
    v = value.Whole();
    if (value.Negative()) v = -v;
    if (v< 0) throw RangeException(NULL, "Negative value");
    if (v> 255) throw RangeException(NULL, "Value exceeds 255");
    return (Byte)(v& 0xff);
    }

// ******************
// ***** ToWord *****
// ******************

  Word Convert::ToWord(char value) {
    if (value > 127) throw RangeException(NULL, "Range outside 0 to 65535");
    return (Word)value;
    }

  Word Convert::ToWord(const char* value) {
    Int32 tmp;
    if (value == NULL) throw NullException(NULL, "Value is null");
    sscanf(value,"%d",&tmp);
    if (tmp > 65535) throw RangeException(NULL, "Range outside 0 to 65535");
    return (Word)tmp;
    }

  Word Convert::ToWord(String* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return ToWord(value->AsCharArray());
    }

  Word Convert::ToWord(String value) {
    if (value.Length() == 0) throw NullException(NULL, "Value is null");
    return ToWord(value.AsCharArray());
    }

  Word Convert::ToWord(UInt8 value) {
    return (Word)value;
    }

  Word Convert::ToWord(UInt16 value) {
    return (Word)value;
    }

  Word Convert::ToWord(UInt32 value) {
    if (value > 65535) throw RangeException(NULL, "Range outside 0 to 65535");
    return (Word)value;
    }

  Word Convert::ToWord(UInt64 value) {
    if (value > 65535) throw RangeException(NULL, "Range outside 0 to 65535");
    return (Word)value;
    }

  Word Convert::ToWord(Int8 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 65535");
    if (value > 65535) throw RangeException(NULL, "Range outside 0 to 65535");
    return (Word)value;
    }

  Word Convert::ToWord(Int16 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 65535");
    if (value > 65535) throw RangeException(NULL, "Range outside 0 to 65535");
    return (Word)value;
    }

  Word Convert::ToWord(Int32 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 65535");
    if (value > 65535) throw RangeException(NULL, "Range outside 0 to 65535");
    return (Word)value;
    }

  Word Convert::ToWord(Int64 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 65535");
    if (value > 65535) throw RangeException(NULL, "Range outside 0 to 65535");
    return (Word)value;
    }

  Word Convert::ToWord(Double value) {
    Int32 v;
    v = (Int32)value;
    if (v < 0) throw RangeException(NULL, "Range outside 0 to 65535");
    if (v > 65535) throw RangeException(NULL, "Range outside 0 to 65535");
    return (Word)v;
    }

  Word Convert::ToWord(Single value) {
    Int32 v;
    v = (Int32)value;
    if (v < 0) throw RangeException(NULL, "Range outside 0 to 65535");
    if (v > 65535) throw RangeException(NULL, "Range outside 0 to 65535");
    return (Word)v;
    }

  Word Convert::ToWord(Fraction value) {
    int v;
    v = value.Whole();
    if (value.Negative()) v = -v;
    if (v < 0) throw RangeException(NULL, "Range outside 0 to 65535");
    if (v > 65535) throw RangeException(NULL, "Range outside 0 to 65535");
    return (Word)v;
    }

// *******************
// ***** ToDword *****
// *******************

  Dword Convert::ToDword(char value) {
    if (value > 127) throw RangeException(NULL, "Range outside 0 to 4294967295");
    return (Dword)value;
    }

  Dword Convert::ToDword(const char* value) {
    Int64 tmp;
    if (value == NULL) throw NullException(NULL, "Value is null");
    sscanf(value,"%lld",&tmp);
    if (tmp > 0xffffffff) throw RangeException(NULL, "Range outside 0 to 4294967295");
    return (Dword)tmp;
    }

  Dword Convert::ToDword(String* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return ToDword(value->AsCharArray());
    }

  Dword Convert::ToDword(String value) {
    if (value.Length() == 0) throw NullException(NULL, "Value is null");
    return ToDword(value.AsCharArray());
    }

  Dword Convert::ToDword(UInt8 value) {
    return (Dword)value;
    }

  Dword Convert::ToDword(UInt16 value) {
    return (Dword)value;
    }

  Dword Convert::ToDword(UInt32 value) {
    return (Dword)value;
    }

  Dword Convert::ToDword(UInt64 value) {
    if (value > 0xffffffff) throw RangeException(NULL, "Range outside 0 to 4294967295");
    return (Dword)value;
    }

  Dword Convert::ToDword(Int8 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 4294967295");
    return (Dword)value;
    }

  Dword Convert::ToDword(Int16 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 4294967295");
    return (Dword)value;
    }

  Dword Convert::ToDword(Int32 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 4294967295");
    return (Dword)value;
    }

  Dword Convert::ToDword(Int64 value) {
    if (value < 0) throw RangeException(NULL, "Range outside 0 to 4294967295");
    if (value > 0xffffffff) throw RangeException(NULL, "Range outside 0 to 4294967295");
    return (Dword)value;
    }

  Dword Convert::ToDword(Double value) {
    Int64 v;
    v = (Int64)value;
    if (v < 0) throw RangeException(NULL, "Range outside 0 to 4294967295");
    if (v > (Int64)0xffffffff) throw RangeException(NULL, "Range outside 0 to 4294967295");
    return (Dword)v;
    }

  Dword Convert::ToDword(Single value) {
    Int64 v;
    v = (Int64)value;
    if (v < 0) throw RangeException(NULL, "Range outside 0 to 4294967295");
    if (v > (Int64)0xffffffff) throw RangeException(NULL, "Range outside 0 to 4294967295");
    return (Dword)v;
    }

  Dword Convert::ToDword(Fraction value) {
    int v;
    v = value.Whole();
    if (value.Negative()) v = -v;
    if (v < 0) throw RangeException(NULL, "Range outside 0 to 4294967295");
    if (v > (Int64)0xffffffff) throw RangeException(NULL, "Range outside 0 to 4294967295");
    return (Dword)v;
    }

// *******************
// ***** ToQword *****
// *******************

  Qword Convert::ToQword(char value) {
    if (value > 127) throw RangeException(NULL, "Range error");
    return (Qword)value;
    }

  Qword Convert::ToQword(const char* value) {
    Int64 tmp;
    if (value == NULL) throw NullException(NULL, "Value is null");
    sscanf(value,"%lld",&tmp);
    if (tmp < 0) throw RangeException(NULL, "Range error");
    return (Qword)tmp;
    }

  Qword Convert::ToQword(String* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return ToQword(value->AsCharArray());
    }

  Qword Convert::ToQword(String value) {
    if (value.Length() == 0) throw NullException(NULL, "Value is null");
    return ToQword(value.AsCharArray());
    }

  Qword Convert::ToQword(UInt8 value) {
    return (Qword)value;
    }

  Qword Convert::ToQword(UInt16 value) {
    return (Qword)value;
    }

  Qword Convert::ToQword(UInt32 value) {
    return (Qword)value;
    }

  Qword Convert::ToQword(UInt64 value) {
    return (Qword)value;
    }

  Qword Convert::ToQword(Int8 value) {
    if (value < 0) throw RangeException(NULL, "Range error");
    return (Qword)value;
    }

  Qword Convert::ToQword(Int16 value) {
    if (value < 0) throw RangeException(NULL, "Range error");
    return (Qword)value;
    }

  Qword Convert::ToQword(Int32 value) {
    if (value < 0) throw RangeException(NULL, "Range error");
    return (Qword)value;
    }

  Qword Convert::ToQword(Int64 value) {
    if (value < 0) throw RangeException(NULL, "Range error");
    return (Qword)value;
    }

  Qword Convert::ToQword(Double value) {
    Int64 v;
    v = (Int64)value;
    if (v < 0) throw RangeException(NULL, "Range error");
    return (Qword)v;
    }

  Qword Convert::ToQword(Single value) {
    Int64 v;
    v = (Int64)value;
    if (v < 0) throw RangeException(NULL, "Range error");
    return (Qword)v;
    }

  Qword Convert::ToQword(Fraction value) {
    int v;
    v = value.Whole();
    if (value.Negative()) v = -v;
    return (Qword)v;
    }

// ******************
// ***** Single *****
// ******************

  Single Convert::ToSingle(char value) {
    return (Single)value;
    }

  Single Convert::ToSingle(const char* value) {
    Single tmp;
    if (value == NULL) throw NullException(NULL, "Value is null");
    sscanf(value,"%f",&tmp);
    return (Single)tmp;
    }

  Single Convert::ToSingle(String* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return ToSingle(value->AsCharArray());
    }

  Single Convert::ToSingle(String value) {
    if (value.Length() == 0) throw NullException(NULL, "Value is null");
    return ToSingle(value.AsCharArray());
    }

  Single Convert::ToSingle(UInt8 value) {
    return (Single)value;
    }

  Single Convert::ToSingle(UInt16 value) {
    return (Single)value;
    }

  Single Convert::ToSingle(UInt32 value) {
    return (Single)value;
    }

  Single Convert::ToSingle(UInt64 value) {
    return (Single)value;
    }

  Single Convert::ToSingle(Int8 value) {
    return (Single)value;
    }

  Single Convert::ToSingle(Int16 value) {
    return (Single)value;
    }

  Single Convert::ToSingle(Int32 value) {
    return (Single)value;
    }

  Single Convert::ToSingle(Int64 value) {
    return (Single)value;
    }

  Single Convert::ToSingle(Double value) {
    return (Single)value;
    }

  Single Convert::ToSingle(Single value) {
    return (Single)value;
    }

  Single Convert::ToSingle(Fraction value) {
    Single v, n, d;
    v = value.Whole();
    n = value.Numerator();
    d = value.Denominator();
    v += (n / d);
    if (value.Negative()) v = -v;
    return v;
    }

// ******************
// ***** Double *****
// ******************

  Double Convert::ToDouble(char value) {
    return (Double)value;
    }

  Double Convert::ToDouble(const char* value) {
    Single tmp;
    if (value == NULL) throw NullException(NULL, "Value is null");
    sscanf(value,"%f",&tmp);
    return (Double)tmp;
    }

  Double Convert::ToDouble(String* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return ToDouble(value->AsCharArray());
    }

  Double Convert::ToDouble(String value) {
    if (value.Length() == 0) throw NullException(NULL, "Value is null");
    return ToDouble(value.AsCharArray());
    }

  Double Convert::ToDouble(UInt8 value) {
    return (Double)value;
    }

  Double Convert::ToDouble(UInt16 value) {
    return (Double)value;
    }

  Double Convert::ToDouble(UInt32 value) {
    return (Double)value;
    }

  Double Convert::ToDouble(UInt64 value) {
    return (Double)value;
    }

  Double Convert::ToDouble(Int8 value) {
    return (Double)value;
    }

  Double Convert::ToDouble(Int16 value) {
    return (Double)value;
    }

  Double Convert::ToDouble(Int32 value) {
    return (Double)value;
    }

  Double Convert::ToDouble(Int64 value) {
    return (Double)value;
    }

  Double Convert::ToDouble(Double value) {
    return (Double)value;
    }

  Double Convert::ToDouble(Single value) {
    return (Double)value;
    }

  Double Convert::ToDouble(Fraction value) {
    Double v, n, d;
    v = value.Whole();
    n = value.Numerator();
    d = value.Denominator();
    v += (n / d);
    if (value.Negative()) v = -v;
    return v;
    }

// ******************
// ***** String *****
// ******************

  String Convert::ToString(char value) {
    char buffer[2];
    buffer[0] = value;
    buffer[1] = 0;
    return String(buffer);
    }

  String Convert::ToString(const char* value) {
    return String(value);
    }

  String Convert::ToString(String* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return ToString(value->AsCharArray());
    }

  String Convert::ToString(UInt8 value) {
    char buffer[32];
    return String(_utoa(value, buffer, 0, 0));
    }

  String Convert::ToString(UInt16 value) {
    char buffer[32];
    return String(_utoa(value, buffer, 0, 0));
    }

  String Convert::ToString(UInt32 value) {
    char buffer[32];
    return String(_utoa(value, buffer, 0, 0));
    }

  String Convert::ToString(UInt64 value) {
    char buffer[128];
    return String(_lutoa(value, buffer, 0, 0));
    }

  String Convert::ToString(Int8 value) {
    char buffer[32];
    return String(_itoa(value, buffer, 0, 0));
    }

  String Convert::ToString(Int16 value) {
    char buffer[32];
    return String(_itoa(value, buffer, 0, 0));
    }

  String Convert::ToString(Int32 value) {
    char buffer[32];
    return String(_itoa(value, buffer, 0, 0));
    }

  String Convert::ToString(Int64 value) {
    char buffer[128];
    return String(_ltoa(value, buffer, 0, 0));
    }

  String Convert::ToString(Double value) {
    char buffer[128];
    return String(_ftoa(value, buffer, 0, 0));
    }

  String Convert::ToString(Single value) {
    char buffer[32];
    sprintf(buffer,"%f",value);
    return String(buffer);
    }

  String Convert::ToString(Fraction value) {
    return value.ToString();
    }

  String Convert::ToString(Boolean value) {
    return (value) ? String("true") : String("false");
    }

// ******************************
// ***** String with format *****
// ******************************

  String Convert::ToString(Int8 value, const char* format) {
    char buffer[32];
    return String(_itoa_formatted(value, buffer, 0, 0, format));
    }

  String Convert::ToString(Int8 value, String format) {
    char buffer[32];
    return String(_itoa_formatted(value, buffer, 0, 0, format.AsCharArray()));
    }

  String Convert::ToString(Int8 value, String *format) {
    char buffer[32];
    return String(_itoa_formatted(value, buffer, 0, 0, format->AsCharArray()));
    }

  String Convert::ToString(UInt8 value, const char* format) {
    char buffer[32];
    return String(_utoa_formatted(value, buffer, 0, 0, format));
    }

  String Convert::ToString(UInt8 value, String format) {
    char buffer[32];
    return String(_utoa_formatted(value, buffer, 0, 0, format.AsCharArray()));
    }

  String Convert::ToString(UInt8 value, String *format) {
    char buffer[32];
    return String(_utoa_formatted(value, buffer, 0, 0, format->AsCharArray()));
    }

  String Convert::ToString(Int16 value, const char* format) {
    char buffer[32];
    return String(_itoa_formatted(value, buffer, 0, 0, format));
    }

  String Convert::ToString(Int16 value, String format) {
    char buffer[32];
    return String(_itoa_formatted(value, buffer, 0, 0, format.AsCharArray()));
    }

  String Convert::ToString(Int16 value, String *format) {
    char buffer[32];
    return String(_itoa_formatted(value, buffer, 0, 0, format->AsCharArray()));
    }

  String Convert::ToString(UInt16 value, const char* format) {
    char buffer[32];
    return String(_utoa_formatted(value, buffer, 0, 0, format));
    }

  String Convert::ToString(UInt16 value, String format) {
    char buffer[32];
    return String(_utoa_formatted(value, buffer, 0, 0, format.AsCharArray()));
    }

  String Convert::ToString(UInt16 value, String *format) {
    char buffer[32];
    return String(_utoa_formatted(value, buffer, 0, 0, format->AsCharArray()));
    }

  String Convert::ToString(Int32 value, const char* format) {
    char buffer[32];
    return String(_itoa_formatted(value, buffer, 0, 0, format));
    }

  String Convert::ToString(Int32 value, String format) {
    char buffer[32];
    return String(_itoa_formatted(value, buffer, 0, 0, format.AsCharArray()));
    }

  String Convert::ToString(Int32 value, String *format) {
    char buffer[32];
    return String(_itoa_formatted(value, buffer, 0, 0, format->AsCharArray()));
    }

  String Convert::ToString(UInt32 value, const char* format) {
    char buffer[32];
    return String(_utoa_formatted(value, buffer, 0, 0, format));
    }

  String Convert::ToString(UInt32 value, String format) {
    char buffer[32];
    return String(_utoa_formatted(value, buffer, 0, 0, format.AsCharArray()));
    }

  String Convert::ToString(UInt32 value, String *format) {
    char buffer[32];
    return String(_utoa_formatted(value, buffer, 0, 0, format->AsCharArray()));
    }

  String Convert::ToString(Double value, const char* format) {
    char buffer[128];
    return String(_ftoa_formatted(value, buffer, 0, 0, format));
    }

  String Convert::ToString(Double value, String format) {
    char buffer[128];
    return String(_ftoa_formatted(value, buffer, 0, 0, format.AsCharArray()));
    }

  String Convert::ToString(Double value, String *format) {
    char buffer[128];
    return String(_ftoa_formatted(value, buffer, 0, 0, format->AsCharArray()));
    }

  String Convert::ToString(Single value, const char* format) {
    char buffer[128];
    return String(_ftoa_formatted(value, buffer, 0, 0, format));
    }

  String Convert::ToString(Single value, String format) {
    char buffer[128];
    return String(_ftoa_formatted(value, buffer, 0, 0, format.AsCharArray()));
    }

  String Convert::ToString(Single value, String *format) {
    char buffer[128];
    return String(_ftoa_formatted(value, buffer, 0, 0, format->AsCharArray()));
    }

// *****************
// ***** char* *****
// *****************

  char* Convert::ToCharArray(char value) {
    char* ret;
    ret = (char*)malloc(2);
    if (ret == NULL) throw InvalidOpException(NULL, "Could not allocate memory");
    ret[0] = value;
    ret[1] = 0;
    return ret;
    }

  char* Convert::ToCharArray(const char* value) {
    char* ret;
    ret = (char*)malloc(strlen(value) + 1);
    if (ret == NULL) throw InvalidOpException(NULL, "Could not allocate memory");
    strcpy(ret, value);
    return ret;
    }

  char* Convert::ToCharArray(String* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return ToCharArray(value->AsCharArray());
    }

  char* Convert::ToCharArray(UInt8 value) {
    char* ret;
    char buffer[32];
    sprintf(buffer,"%u",value);
    ret = (char*)malloc(strlen(buffer) + 1);
    if (ret == NULL) throw InvalidOpException(NULL, "Could not allocate memory");
    strcpy(ret, buffer);
    return ret;
    }

  char* Convert::ToCharArray(UInt16 value) {
    char* ret;
    char buffer[32];
    sprintf(buffer,"%u",value);
    ret = (char*)malloc(strlen(buffer) + 1);
    if (ret == NULL) throw InvalidOpException(NULL, "Could not allocate memory");
    strcpy(ret, buffer);
    return ret;
    }

  char* Convert::ToCharArray(UInt32 value) {
    char* ret;
    char buffer[32];
    sprintf(buffer,"%u",value);
    ret = (char*)malloc(strlen(buffer) + 1);
    if (ret == NULL) throw InvalidOpException(NULL, "Could not allocate memory");
    strcpy(ret, buffer);
    return ret;
    }

  char* Convert::ToCharArray(UInt64 value) {
    char* ret;
    char buffer[32];
    sprintf(buffer,"%llu",value);
    ret = (char*)malloc(strlen(buffer) + 1);
    if (ret == NULL) throw InvalidOpException(NULL, "Could not allocate memory");
    strcpy(ret, buffer);
    return ret;
    }

  char* Convert::ToCharArray(Int8 value) {
    char* ret;
    char buffer[32];
    sprintf(buffer,"%d",value);
    ret = (char*)malloc(strlen(buffer) + 1);
    if (ret == NULL) throw InvalidOpException(NULL, "Could not allocate memory");
    strcpy(ret, buffer);
    return ret;
    }

  char* Convert::ToCharArray(Int16 value) {
    char* ret;
    char buffer[32];
    sprintf(buffer,"%d",value);
    ret = (char*)malloc(strlen(buffer) + 1);
    if (ret == NULL) throw InvalidOpException(NULL, "Could not allocate memory");
    strcpy(ret, buffer);
    return ret;
    }

  char* Convert::ToCharArray(Int32 value) {
    char* ret;
    char buffer[32];
    sprintf(buffer,"%d",value);
    ret = (char*)malloc(strlen(buffer) + 1);
    if (ret == NULL) throw InvalidOpException(NULL, "Could not allocate memory");
    strcpy(ret, buffer);
    return ret;
    }

  char* Convert::ToCharArray(Int64 value) {
    char* ret;
    char buffer[32];
    sprintf(buffer,"%lld",value);
    ret = (char*)malloc(strlen(buffer) + 1);
    if (ret == NULL) throw InvalidOpException(NULL, "Could not allocate memory");
    strcpy(ret, buffer);
    return ret;
    }

  char* Convert::ToCharArray(Double value) {
    char* ret;
    char buffer[32];
    sprintf(buffer,"%f",value);
    ret = (char*)malloc(strlen(buffer) + 1);
    if (ret == NULL) throw InvalidOpException(NULL, "Could not allocate memory");
    strcpy(ret, buffer);
    return ret;
    }

  char* Convert::ToCharArray(Single value) {
    char* ret;
    char buffer[32];
    sprintf(buffer,"%f",value);
    ret = (char*)malloc(strlen(buffer) + 1);
    if (ret == NULL) throw InvalidOpException(NULL, "Could not allocate memory");
    strcpy(ret, buffer);
    return ret;
    }

  char* Convert::ToCharArray(Fraction value) {
    char* ret;
    char buffer[32];
    if (value.Negative()) {
      if (value.Whole() != 0)
        sprintf(buffer,"-%d %d/%d",value.Whole(), value.Numerator(), value.Denominator());
      else
        sprintf(buffer,"-%d/%d", value.Numerator(), value.Denominator());
      }
    else {
      if (value.Whole() != 0)
        sprintf(buffer,"%d %d/%d",value.Whole(), value.Numerator(), value.Denominator());
      else
        sprintf(buffer,"%d/%d", value.Numerator(), value.Denominator());
      }
    ret = (char*)malloc(strlen(buffer) + 1);
    if (ret == NULL) throw InvalidOpException(NULL, "Could not allocate memory");
    strcat(ret, buffer);
    return ret;
    }

// *****************
// ***** char* *****
// *****************

  char* Convert::ToCharArray(char value,char* dest,UInt32 maxLen) {
    if (maxLen < 2) throw InvalidOpException(NULL, "Buffer too small");
    dest[0] = value;
    dest[1] = 0;
    return dest;
    }

  char* Convert::ToCharArray(const char* value,char* dest,UInt32 maxLen) {
    if (strlen(value)+1 > maxLen) throw InvalidOpException(NULL, "Buffer too small");
    strcpy(dest, value);
    return dest;
    }

  char* Convert::ToCharArray(String* value,char* dest,UInt32 maxLen) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    return ToCharArray(value->AsCharArray(),dest,maxLen);
    }

  char* Convert::ToCharArray(UInt8 value,char* dest,UInt32 maxLen) {
    char buffer[32];
    sprintf(buffer,"%u",value);
    if (strlen(buffer)+1 > maxLen) throw InvalidOpException(NULL, "Buffer too small");
    strcpy(dest, buffer);
    return dest;
    }

  char* Convert::ToCharArray(UInt16 value,char* dest,UInt32 maxLen) {
    char buffer[32];
    sprintf(buffer,"%u",value);
    if (strlen(buffer)+1 > maxLen) throw InvalidOpException(NULL, "Buffer too small");
    strcpy(dest, buffer);
    return dest;
    }

  char* Convert::ToCharArray(UInt32 value,char* dest,UInt32 maxLen) {
    char buffer[32];
    sprintf(buffer,"%u",value);
    if (strlen(buffer)+1 > maxLen) throw InvalidOpException(NULL, "Buffer too small");
    strcpy(dest, buffer);
    return dest;
    }

  char* Convert::ToCharArray(UInt64 value,char* dest,UInt32 maxLen) {
    char buffer[32];
    sprintf(buffer,"%llu",value);
    if (strlen(buffer)+1 > maxLen) throw InvalidOpException(NULL, "Buffer too small");
    strcpy(dest, buffer);
    return dest;
    }

  char* Convert::ToCharArray(Int8 value,char* dest,UInt32 maxLen) {
    char buffer[32];
    sprintf(buffer,"%d",value);
    if (strlen(buffer)+1 > maxLen) throw InvalidOpException(NULL, "Buffer too small");
    strcpy(dest, buffer);
    return dest;
    }

  char* Convert::ToCharArray(Int16 value,char* dest,UInt32 maxLen) {
    char buffer[32];
    sprintf(buffer,"%d",value);
    if (strlen(buffer)+1 > maxLen) throw InvalidOpException(NULL, "Buffer too small");
    strcpy(dest, buffer);
    return dest;
    }

  char* Convert::ToCharArray(Int32 value,char* dest,UInt32 maxLen) {
    char buffer[32];
    sprintf(buffer,"%d",value);
    if (strlen(buffer)+1 > maxLen) throw InvalidOpException(NULL, "Buffer too small");
    strcpy(dest, buffer);
    return dest;
    }

  char* Convert::ToCharArray(Int64 value,char* dest,UInt32 maxLen) {
    char buffer[32];
    sprintf(buffer,"%lld",value);
    if (strlen(buffer)+1 > maxLen) throw InvalidOpException(NULL, "Buffer too small");
    strcpy(dest, buffer);
    return dest;
    }

  char* Convert::ToCharArray(Double value,char* dest,UInt32 maxLen) {
    char buffer[32];
    sprintf(buffer,"%f",value);
    if (strlen(buffer)+1 > maxLen) throw InvalidOpException(NULL, "Buffer too small");
    strcpy(dest, buffer);
    return dest;
    }

  char* Convert::ToCharArray(Single value,char* dest,UInt32 maxLen) {
    char buffer[32];
    sprintf(buffer,"%f",value);
    if (strlen(buffer)+1 > maxLen) throw InvalidOpException(NULL, "Buffer too small");
    strcpy(dest, buffer);
    return dest;
    }

  char* Convert::ToCharArray(Fraction value,char* dest,UInt32 maxLen) {
    char buffer[32];
    if (value.Negative()) {
      if (value.Whole() != 0)
        sprintf(buffer,"-%d %d/%d",value.Whole(), value.Numerator(), value.Denominator());
      else
        sprintf(buffer,"-%d/%d", value.Numerator(), value.Denominator());
      }
    else {
      if (value.Whole() != 0)
        sprintf(buffer,"%d %d/%d",value.Whole(), value.Numerator(), value.Denominator());
      else
        sprintf(buffer,"%d/%d", value.Numerator(), value.Denominator());
      }
    if (strlen(buffer)+1 > maxLen) throw InvalidOpException(NULL, "Buffer too small");
    strcpy(dest, buffer);
    return dest;
    }

// *********************
// ***** ToBoolean *****
// *********************

  Boolean Convert::ToBoolean(char value) {
    if (value == 't' || value == 'T' || value == 'y' || value == 'Y' || value == '1')
      return true;
    if (value == 'f' || value == 'F' || value == 'n' || value == 'N' || value == '0')
      return false;
    throw InvalidOpException("Passed value could not be evaluated as a boolean");
    }

  Boolean Convert::ToBoolean(const char* value) {
    if (value == NULL) throw NullException(NULL, "Value is null");
    if (strcasecmp(value, "yes") == 0) return true;
    if (strcasecmp(value, "no") == 0) return false;
    if (strcasecmp(value, "true") == 0) return true;
    if (strcasecmp(value, "false") == 0) return false;
    if (strcasecmp(value, "1") == 0) return true;
    if (strcasecmp(value, "0") == 0) return false;
    throw InvalidOpException("Passed value could not be evaluated as a boolean");
    }

  Boolean Convert::ToBoolean(String* value) {
    if (value->ToUpper().Trim().Equals("TRUE")) return true;
    if (value->ToUpper().Trim().Equals("YES")) return true;
    if (value->ToUpper().Trim().Equals("1")) return true;
    if (value->ToUpper().Trim().Equals("FALSE")) return false;
    if (value->ToUpper().Trim().Equals("NO")) return false;
    if (value->ToUpper().Trim().Equals("0")) return false;
    throw InvalidOpException("Passed value could not be evaluated as a boolean");
    }

  Boolean Convert::ToBoolean(String value) {
    if (value.ToUpper().Trim().Equals("TRUE")) return true;
    if (value.ToUpper().Trim().Equals("YES")) return true;
    if (value.ToUpper().Trim().Equals("1")) return true;
    if (value.ToUpper().Trim().Equals("FALSE")) return false;
    if (value.ToUpper().Trim().Equals("NO")) return false;
    if (value.ToUpper().Trim().Equals("0")) return false;
    throw InvalidOpException("Passed value could not be evaluated as a boolean");
    }

  Boolean Convert::ToBoolean(UInt8 value) {
    return (value != 0);
    }

  Boolean Convert::ToBoolean(UInt16 value) {
    return (value != 0);
    }

  Boolean Convert::ToBoolean(UInt32 value) {
    return (value != 0);
    }

  Boolean Convert::ToBoolean(UInt64 value) {
    return (value != 0);
    }

  Boolean Convert::ToBoolean(Int8 value) {
    return (value != 0);
    }

  Boolean Convert::ToBoolean(Int16 value) {
    return (value != 0);
    }

  Boolean Convert::ToBoolean(Int32 value) {
    return (value != 0);
    }

  Boolean Convert::ToBoolean(Int64 value) {
    return (value != 0);
    }

  Boolean Convert::ToBoolean(Double value) {
    return (value != 0);
    }

  Boolean Convert::ToBoolean(Single value) {
    return (value != 0);
    }

  Boolean Convert::ToBoolean(Fraction value) {
    if (value.Whole() != 0) return true;
    if (value.Numerator() != 0) return true;
    return false;
    }




  Byte Convert::ToBitSize(UInt32 value) {
    UInt32 bitSize;
    Byte   size;
    size = 1;
    bitSize = 2;
    while (value >= bitSize) {
      size++;
      bitSize <<= 1;
      }
    return size;
    }

  char Convert::ToHexChar(Byte v) {
    if (v > 15) throw InvalidOpException(NULL, "Value exceeds range");
    if (v < 10) return '0' + v;
    return 'a' + (v-10);
    }

  String* Convert::ToHexString(Byte* buffer, UInt32 len) {
    UInt32  i;
    Byte    hi,lo;
    char*   temp;
    String* ret;
    temp = (char*)malloc(len * 2 + 1);
    if (temp == NULL) throw InvalidOpException(NULL, "Could not allocate memory");
    for (i=0; i<len; i++) {
      hi = (buffer[i] >> 4) & 0xf;
      lo = buffer[i] & 0xf;
      temp[i*2] = ToHexChar(hi);
      temp[i*2+1] = ToHexChar(lo);
      }
    temp[len*2] = 0;
    ret = new String((const char*)temp);
    free(temp);
    return ret;
    }

  Byte Convert::FromHexChar(char v) {
    if (v >= '0' && v <= '9') return v - '0';
    if (v >= 'A' && v <= 'F') return v - 'A' + 10;
    if (v >= 'a' && v <= 'f') return v - 'a' + 10;
    throw InvalidOpException(NULL, "Not a valid hex character");
    }

  List<Byte>* Convert::FromHexString(const char* str) {
    Byte       value;
    Byte       digit;
    UInt32     pos;
    List<Byte>* ret;
    bool       nybble;
    ret = new List<Byte>();
    pos = 0;
    nybble = false;
    while (str[pos] != 0) {
      digit = FromHexChar(str[pos]);
      if (nybble) {
        value |= digit;
        ret->Add(value);
        }
      else {
        value = (digit << 4);
        }
      nybble = !nybble;
      pos++;
      }
    return ret;
    }

  List<Byte>* Convert::FromHexString(String* str) {
    if (str == NULL) throw InvalidOpException(NULL, "String object was NULL");
    return FromHexString(str->AsCharArray());
    }

  Int16 Convert::SwitchEndian(Int16 value) {
    Int16 ret;
    UInt16 v;
    v = (UInt16)value;
    ret = ((v >> 8) & 0xff) | ((v & 0xff) << 8);
    return ret;
    }

  Int32 Convert::SwitchEndian(Int32 value) {
    Int32 ret;
    UInt32 v;
    v = (UInt32)value;
    ret = (v & 0xff000000) >> 24;
    ret |= (v & 0x00ff0000) >> 8;
    ret |= (v & 0x0000ff00) << 8;
    ret |= (v & 0x000000ff) << 24;
    return ret;
    }

  UInt16 Convert::SwitchEndian(UInt16 value) {
    Int16 ret;
    ret = ((value >> 8) & 0xff) | ((value & 0xff) << 8);
    return ret;
    }

  UInt32 Convert::SwitchEndian(UInt32 value) {
    UInt32 ret;
    ret = (value & 0xff000000) >> 24;
    ret |= (value & 0x00ff0000) >> 8;
    ret |= (value & 0x0000ff00) << 8;
    ret |= (value & 0x000000ff) << 24;
    return ret;
    }

  UInt64 Convert::BcdToUInt64(Byte* buffer, int bytes) {
    int i;
    Byte   nybble;
    UInt64 ret;
    ret = 0;
    for (i=0; i<bytes; i++) {
      nybble = (buffer[i] >> 4) & 0xf;
      if (nybble > 9) throw RangeException(NULL, "Invalid bcd digit");
      ret = (ret * 10) + nybble;
      nybble = buffer[i] & 0xf;
      if (nybble > 9) throw RangeException(NULL, "Invalid bcd digit");
      ret = (ret * 10) + nybble;
      }
    return ret;
    }

  UInt32 Convert::BcdToUInt32(Byte* buffer, int bytes) {
    return ToUInt32(BcdToUInt64(buffer, bytes));
    }

  UInt16 Convert::BcdToUInt16(Byte* buffer, int bytes) {
    return ToUInt16(BcdToUInt64(buffer, bytes));
    }

  UInt8 Convert::BcdToUInt8(Byte* buffer, int bytes) {
    return ToUInt8(BcdToUInt64(buffer, bytes));
    }

  UInt8 Convert::BcdToUInt8(Byte value) {
    Byte temp[1];
    temp[0] = value & 0xff;
    return BcdToUInt8(temp, 1);
    }

  UInt8 Convert::BcdToUInt8(Word value) {
    Byte temp[2];
    temp[0] = (value >> 8) & 0xff;
    temp[1] = value & 0xff;
    return BcdToUInt8(temp, 2);
    }

  UInt8 Convert::BcdToUInt8(Dword value) {
    Byte temp[4];
    temp[0] = (value >> 24) & 0xff;
    temp[1] = (value >> 16) & 0xff;
    temp[2] = (value >> 8) & 0xff;
    temp[3] = value & 0xff;
    return BcdToUInt8(temp, 4);
    }

  UInt16 Convert::BcdToUInt16(Byte value) {
    Byte temp[1];
    temp[0] = value & 0xff;
    return BcdToUInt16(temp, 1);
    }

  UInt16 Convert::BcdToUInt16(Word value) {
    Byte temp[2];
    temp[0] = (value >> 8) & 0xff;
    temp[1] = value & 0xff;
    return BcdToUInt16(temp, 2);
    }

  UInt16 Convert::BcdToUInt16(Dword value) {
    Byte temp[4];
    temp[0] = (value >> 24) & 0xff;
    temp[1] = (value >> 16) & 0xff;
    temp[2] = (value >> 8) & 0xff;
    temp[3] = value & 0xff;
    return BcdToUInt16(temp, 4);
    }

  UInt32 Convert::BcdToUInt32(Byte value) {
    Byte temp[1];
    temp[0] = value & 0xff;
    return BcdToUInt32(temp, 1);
    }

  UInt32 Convert::BcdToUInt32(Word value) {
    Byte temp[2];
    temp[0] = (value >> 8) & 0xff;
    temp[1] = value & 0xff;
    return BcdToUInt32(temp, 2);
    }

  UInt32 Convert::BcdToUInt32(Dword value) {
    Byte temp[4];
    temp[0] = (value >> 24) & 0xff;
    temp[1] = (value >> 16) & 0xff;
    temp[2] = (value >> 8) & 0xff;
    temp[3] = value & 0xff;
    return BcdToUInt32(temp, 4);
    }

UInt32 Convert::UIntToBcd(Byte digits, UInt64 value, Byte* dest) {
  int i;
  UInt64 v;
  Byte tmp;
  UInt64 digit;
  UInt32 count;
  Boolean      bits;
  count = 0;
  v = 1;
  if ((digits & 1) == 1) digits++;
  for (i=0; i<digits-1; i++) v *= 10;
  value = value % (v * 10);
  tmp = 0;
  bits = false;
  while (v > 0) {
    tmp <<= 4;
    digit = value / v;
    tmp |= digit;
    value -= (digit * v);
    v /= 10;
    bits = (bits) ? false : true;
    if (!bits) {
      dest[count++] = tmp;
      tmp = 0;
      }
    }
  return count;
  }

  Double Convert::MetersToYards(Double d) {
    return d * 1.0936133;
    }

  Double Convert::YardsToMeters(Double d) {
    return d * 0.9144;
    }

  Double Convert::MetersToFeet(Double d) {
    return d * 3.2808399;
    }

  Double Convert::FeetToMeters(Double d) {
    return d * 0.3048;
    }

  Double Convert::MetersToInches(Double d) {
    return d * 39.3700787;
    }

  Double Convert::InchesToMeters(Double d) {
    return d * 0.0254;
    }

  Double Convert::CentimetersToInches(Double d) {
    return d * 0.3937007874;
    }

  Double Convert::InchesToCentimeters(Double d) {
    return d * 2.54;
    }

  Double Convert::GallonsToLiters(Double d) {
    return d * 3.78541178;
    }

  Double Convert::LitersToGallons(Double d) {
    return d * 0.26417205236;
    }

  Double Convert::GallonsToQuarts(Double d) {
    return d * 4;
    }

  Double Convert::QuartsToGallons(Double d) {
    return d * 0.25;
    }

  Double Convert::GallonsToPints(Double d) {
    return d * 8;
    }

  Double Convert::PintsToGallons(Double d) {
    return d * 0.125;
    }

  Double Convert::GallonsToOunces(Double d) {
    return d * 128;
    }

  Double Convert::OuncesToGallons(Double d) {
    return d * 0.0078125;
    }

  Double Convert::QuartsToPints(Double d) {
    return d * 2;
    }

  Double Convert::PintsToQuarts(Double d) {
    return d * 0.5;
    }

  Double Convert::QuartsToOunces(Double d) {
    return d * 32;
    }

  Double Convert::OuncesToQuarts(Double d) {
    return d * 0.03125;
    }

  Double Convert::PintsToOunces(Double d) {
    return d * 16;
    }

  Double Convert::OuncesToPints(Double d) {
    return d * 0.0625;
    }

  Double Convert::MilesToKilometers(Double d) {
    return d * 1.609344;
    }

  Double Convert::MilesToMeters(Double d) {
    return d * 1609.344;
    }

  Double Convert::MilesToYards(Double d) {
    return d * 1760;
    }

  Double Convert::MilesToFeet(Double d) {
    return d * 5280;
    }

  Double Convert::MilesToNauticalMiles(Double d) {
    return d * 0.8689762419;
    }

  Double Convert::NauticalMilesToMiles(Double d) {
    return d * 1.15077945;
    }

  Double Convert::KilometersToMeters(Double d) {
    return d * 1000;
    }

  Double Convert::KilometersToMiles(Double d) {
    return d * 0.62137119224;
    }

  Double Convert::KilometersToYards(Double d) {
    return d * 1093.6133;
    }

  Double Convert::KilometersToFeet(Double d) {
    return d * 3280.8399;
    }

  Double Convert::KilometersToNauticalMiles(Double d) {
    return d * 0.53995680346;
    }

  Double Convert::NauticalMilesToKilometers(Double d) {
    return d * 1.852;
    }

  Double Convert::NauticalMilesToYards(Double d) {
    return d * 2025.37183;
    }

  Double Convert::NauticalMilesToFeet(Double d) {
    return d * 6076.11549;
    }

  Double Convert::NauticalMilesToMeters(Double d) {
    return d * 1852;
    }

  Double Convert::LitersToQuarts(Double d) {
    return d * 1.05668821;
    }

  Double Convert::LitersToPints(Double d) {
    return d * 2.1133763;
    }

  Double Convert::LitersToOunces(Double d) {
    return d * 33.8140565;
    }

  Double Convert::QuartsToLiters(Double d) {
    return d * 0.946352946;
    }

  Double Convert::PintsToLiters(Double d) {
    return d * 0.4731765;
    }

  Double Convert::OuncesToLiters(Double d) {
    return d * 0.0295735;
    }

  Double Convert::GramsToKilograms(Double d) {
    return d * 0.001;
    }

  Double Convert::GramsToOunces(Double d) {
    return d * 0.03527396195;
    }

  Double Convert::GramsToPounds(Double d) {
    return d * 0.00229462262;
    }

  Double Convert::KilogramsToGrams(Double d) {
    return d * 1000;
    }

  Double Convert::KilogramsToOunces(Double d) {
    return d * 35.2739619;
    }

  Double Convert::KilogramsToPounds(Double d) {
    return d * 2.20462262;
    }

  Double Convert::OuncesToPounds(Double d) {
    return d * 0.0625;
    }

  Double Convert::OuncesToGrams(Double d) {
    return d * 28.3495231;
    }

  Double Convert::OuncesToKilograms(Double d) {
    return d * 0.02834952313;
    }

  Double Convert::PoundsToOunces(Double d) {
    return d * 16;
    }

  Double Convert::PoundsToGrams(Double d) {
    return d * 453.59237;
    }

  Double Convert::PoundsToKilograms(Double d) {
    return d * 0.45359237;
    }

  Double Convert::ParsecToMiles(Double d) {
    return d * 1.917350796e+13;
    }

  Double Convert::ParsecToKilometers(Double d) {
    return d * 3.085677e+13;
    }

  Double Convert::ParsecToLightYears(Double d) {
    return d * 3.26164262;
    }

  Double Convert::LightYearsToParsecs(Double d) {
    return d * 0.30659398246;
    }

  Double Convert::LightYearsToMiles(Double d) {
    return d * 5.878482164e+12;
    }

  Double Convert::LightYearsToKilometers(Double d) {
    return d * 9.4605e+12;
    }

  Double Convert::PoundsToStones(Double d) {
    return d * 0.07142857143;
    }

  Double Convert::StonesToPounds(Double d) {
    return d * 14;
    }

  Double Convert::FahrenheitToCelsius(Double d) {
    return (d-32)/1.8;
    }

  Double Convert::CelsiusToFahrenheit(Double d) {
    return (d * 1.8) + 32;
    }

  Double Convert::CelsiusToKelvin(Double d) {
    return d+273.15;
    }

  Double Convert::KelvinToCelsius(Double d) {
    return d-273.15;
    }

  Double Convert::KelvinToFahrenheit(Double d) {
    return (d * 1.8) - 459.7;
    }

  Double Convert::FahrenheitToKelvin(Double d) {
    return ((d - 32) / 1.8) + 273.15;
    }

  Double Convert::CubicInchesToCubicCentimeters(Double d) {
    return d * 16.387064;
    }

  Double Convert::CubicInchesToOunces(Double d) {
    return d * 0.55411310802;
    }

  Double Convert::CubicInchesToPints(Double d) {
    return d * 0.03463203266;
    }

  Double Convert::CubicInchesToQuarts(Double d) {
    return d * 0.01731601732;
   }

  Double Convert::CubicInchesToGallons(Double d) {
    return d * 0.00432900433;
   }

  Double Convert::CubicInchesToLiters(Double d) {
    return d * 0.016387064;
   }

  Double Convert::CubicCentimetersToCubicInches(Double d) {
    return d * 0.06102374409;
    }

  Double Convert::OuncesToCubicInches(Double d) {
    return d * 1.8046857;
    }

  Double Convert::PintsToCubicInches(Double d) {
    return d * 28.8750016;
    }

  Double Convert::QuartsToCubicInches(Double d) {
    return d * 57.75;
    }

  Double Convert::GallonsToCubicInches(Double d) {
    return d * 231;
    }

  Double Convert::LitersToCubicInches(Double d) {
    return d * 61.0237441;
    }

  Double Convert::MetersToMiles(Double d) {
    return d * 0.000621371;
    }

  Double Convert::MetersToLeagues(Double d) {
    return d * 0.000179986;
    }

  Double Convert::LeaguesToMeters(Double d) {
    return d * 5556;
    }

  Double Convert::MetersToFathoms(Double d) {
    return d * 0.546807;
    }

  Double Convert::FathomsToMeters(Double d) {
    return d * 1.8288;
    }

  Double Convert::FurlongsToMeters(Double d) {
    return d * 201.168;
    }

  Double Convert::MetersToFurlongs(Double d) {
    return d * 0.00497096;
    }

  Double Convert::FathomsToFeet(Double d) {
    return d * 6;
    }

  Double Convert::FurlongsToFeet(Double d) {
    return d * 660;
    }

  Double Convert::SquareInchesToSquareFeet(Double d) {
    return d * 0.00694444;
    }

  Double Convert::SquareInchesToSquareCentimeters(Double d) {
    return d * 6.4516;
    }

  Double Convert::SquareInchesToSquareMeters(Double d) {
    return d * 0.00064516;
    }

  Double Convert::SquareInchesToSquareYards(Double d) {
    return d * 0.000771605;
    }

  Double Convert::SquareCentimetersToSquareInches(Double d) {
    return d * 0.155;
    }

  Double Convert::SquareCentimetersToSquareFeet(Double d) {
    return d * 0.00107639;
    }

  Double Convert::SquareCentimetersToSquareYards(Double d) {
    return d * 0.000119599;
    }

  Double Convert::SquareCentimetersToSquareMeters(Double d) {
    return d * 0.0001;
    }

  Double Convert::SquareFeetToSquareInches(Double d) {
    return d * 144;
    }

  Double Convert::SquareFeetToSquareCentimeters(Double d) {
    return d * 929.03;
    }

  Double Convert::SquareFeetToSquareMeters(Double d) {
    return d * 0.092903;
    }

  Double Convert::SquareFeetToSquareYards(Double d) {
    return d * 0.111111;
    }

  Double Convert::SquareYardsToSquareCentimeters(Double d) {
    return d * 8361.27;
    }

  Double Convert::SquareYardsToSquareFeet(Double d) {
    return d * 9;
    }

  Double Convert::SquareYardsToSquareInches(Double d) {
    return d * 1296;
    }

  Double Convert::SquareYardsToSquareMeters(Double d) {
    return d * 0.836127;
    }

  Double Convert::SquareMetersToSquareCentimeters(Double d) {
    return d * 10000;
    }

  Double Convert::SquareMetersToSquareFeet(Double d) {
    return d * 10.7639;
    }

  Double Convert::SquareMetersToSquareInches(Double d) {
    return d * 1550;
    }

  Double Convert::SquareMetersToSquareYards(Double d) {
    return d * 1.19599;
    }

  Double Convert::AcresToSquareFeet(Double d) {
    return d * 43560;
    }

  Double Convert::AcresToSquareMeters(Double d) {
    return d * 4046.86;
    }

  Double Convert::AcresToSquareYards(Double d) {
    return d * 4840;
    }

  Double Convert::AcresToHectares(Double d) {
    return d * 0.404686;
    }

  Double Convert::HectaresToAcres(Double d) {
    return d * 2.47105;
    }

  Double Convert::HectaresToSquareFeet(Double d) {
    return d * 107639;
    }

  Double Convert::HectaresToSquareMeters(Double d) {
    return d * 10000;
    }

  Double Convert::HectaresToSquareYards(Double d) {
    return d * 11959.9;
    }

  Double Convert::SquareMilesToSquareKilometers(Double d) {
    return d * 2.58999;
    }

  Double Convert::SquareMilesToSquareMeters(Double d) {
    return d * 2.59e+6;
    }

  Double Convert::SquareMilesToSquareYards(Double d) {
    return d * 3.098e+6;
    }

  Double Convert::SquareKilometersToSquareMeters(Double d) {
    return d * 1000000;
    }

  Double Convert::SquareKilometersToSquareMiles(Double d) {
    return d * 0.386102;
    }

  Double Convert::SquareKilometersToSquareYards(Double d) {
    return d * 1.196e+6;
    }


  }
