/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

/* **** Supported syntax                                             **** */
/* **** .     - any character                                        **** */
/* **** *     - 0 or more of previous                                **** */
/* **** +     - 1 or more of previous                                **** */
/* **** ?     - 0 or 1 of previous                                   **** */
/* **** {n}   - n occurences of previous                             **** */
/* **** {n,m} - n through m occurences of previous                   **** */
/* **** {,m}  - 0 through m occurences of previous                   **** */
/* **** {n,}  - n or more of previous                                **** */
/* **** ()    - capture what is between parens                       **** */
/* **** []    - character class                                      **** */
/* **** [^]   - excluded character class                             **** */
/* **** /c    - literal version of c, when c is special character    **** */
/* **** /d    - any digit.  Equiv. to [0-9]                          **** */
/* **** /D    - non digit.  Equiv. to [^0-9]                         **** */
/* **** /w    - alphanumeric.    [0-9a-zA-Z_]                        **** */
/* **** /W    - nonalphanumeric. [^0-9a-zA-Z_]                       **** */
/* **** /a    - alphabetic.      [a-zA-Z_]                           **** */
/* **** /A    - nonalphabetic.   [^a-zA-Z_]                          **** */
/* **** /s    - whitespace.                                          **** */
/* **** /S    - non whitespace.                                      **** */
/* **** /p    - punctuation.     [][!@#$%^&*()_+-=`~<>,./?;:'"{}\\]  **** */
/* **** /l    - lowercase letters [a-z]                              **** */
/* **** /u    - uppercase letters [A-Z]                              **** */
/* **** /x    - hexidecimal       [0-9a-fA-F]                        **** */
/* **** /c    - control codes     0x01-0x1f,0x7f                     **** */
/* ****                                                              **** */

#include <stdio.h>
#include "string.h"
#include "SmrFramework.h"

namespace SmrFramework {

  RegEx::RegEx(const char* expression) {
    Byte i;
    Byte token[1024];
    Byte number[1024];
    Word pos;
    UInt32 min;
    UInt32 max;
    Byte   tokenType;
    Byte   last;
    varCount = 0;
    objectType = (char*)"RegEx";
    tokens = new List<String*>();
    minCount = new List<UInt32>();
    maxCount = new List<UInt32>();
    tokenTypes = new List<Byte>();
    while (*expression != 0) {
      if (*expression == '^') {
        if (tokens->Count() != 0) throw InvalidOpException(this, "^ found after beginning");
        tokenType = TOKEN_START;
        token[0] = *expression++;
        token[1] = 0;
        min = 1;
        max = 1;
        }
      else if (*expression == '$') {
        tokenType = TOKEN_END;
        token[0] = *expression++;
        token[1] = 0;
        min = 1;
        max = 1;
        if (*expression != 0) throw InvalidOpException(this, "Invalid regular expression");
        }
      else if (*expression == '(') {
        token[0] = *expression++;
        token[1] = 0;
        tokenType = TOKEN_VAR_START;
        min = 1;
        max = 1;
        }
      else if (*expression == ')') {
        token[0] = *expression++;
        token[1] = 0;
        tokenType = TOKEN_VAR_END;
        min = 1;
        max = 1;
        }
      else if (*expression == '.') {
        token[0] = *expression++;
        token[1] = 0;
        tokenType = TOKEN_ANY;
        min = 1;
        max = 1;
        }
      else if (*expression == '\\') {
        expression++;
        if (*expression == 0) throw InvalidOpException(this, "Invalid regular expression");
        if (*expression == 'd') {
          strcpy((char*)token,"0123456789");
          tokenType = TOKEN_INCLUDE;
          expression++;
          }
        else if (*expression == 'D') {
          strcpy((char*)token,"0123456789");
          tokenType = TOKEN_EXCLUDE;
          expression++;
          }
        else if (*expression == 'w') {
          strcpy((char*)token,"0123456789");
          strcat((char*)token,"abcdefghijklmnopqrstuvwxyz");
          strcat((char*)token,"ABCDEFGHIJKLMNOPQRSTUVWXYZ_");
          tokenType = TOKEN_INCLUDE;
          expression++;
          }
        else if (*expression == 'W') {
          strcpy((char*)token,"0123456789");
          strcat((char*)token,"abcdefghijklmnopqrstuvwxyz");
          strcat((char*)token,"ABCDEFGHIJKLMNOPQRSTUVWXYZ_");
          tokenType = TOKEN_EXCLUDE;
          expression++;
          }
        else if (*expression == 'a') {
          strcpy((char*)token,"abcdefghijklmnopqrstuvwxyz");
          strcat((char*)token,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
          tokenType = TOKEN_INCLUDE;
          expression++;
          }
        else if (*expression == 'A') {
          strcpy((char*)token,"abcdefghijklmnopqrstuvwxyz");
          strcat((char*)token,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
          tokenType = TOKEN_EXCLUDE;
          expression++;
          }
        else if (*expression == 's') {
          strcpy((char*)token," \t\r\n\v\f");
          tokenType = TOKEN_INCLUDE;
          expression++;
          }
        else if (*expression == 'S') {
          strcpy((char*)token," \t\r\n\v\f");
          tokenType = TOKEN_EXCLUDE;
          expression++;
          }
        else if (*expression == 'p') {
          strcpy((char*)token,"][!@#$%^&*()_+-=`~<>,./?;:'\"{}\\");
          tokenType = TOKEN_INCLUDE;
          expression++;
          }
        else if (*expression == 'u') {
          strcpy((char*)token,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
          tokenType = TOKEN_INCLUDE;
          expression++;
          }
        else if (*expression == 'l') {
          strcpy((char*)token,"abcdefghijklmnopqrstuvwxyz");
          tokenType = TOKEN_INCLUDE;
          expression++;
          }
        else if (*expression == 'x') {
          strcpy((char*)token,"0123456789abcdefABCDEF");
          tokenType = TOKEN_INCLUDE;
          expression++;
          }
        else if (*expression == 'c') {
          for (i=1; i<32; i++) token[i] = i;
          token[32] = 0;
          token[0] = 0x7f;
          tokenType = TOKEN_INCLUDE;
          expression++;
          }
        else {
          token[0] = *expression++;
          token[1] = 0;
          tokenType = TOKEN_LITERAL;
          }
        min = 1;
        max = 1;
        }
      else if (*expression == '[') {
        expression++;
        if (*expression == '^') {
          tokenType = TOKEN_EXCLUDE;
          expression++;
          } else tokenType = TOKEN_INCLUDE;
        min = 1;
        max = 1;
        last = 0;
        pos = 0;
        if (*expression == ']') token[pos++] = *expression++;
        while (*expression != ']') {
          if (*expression == 0)
            throw InvalidOpException(this, "Invalid regular expression");
          if (*expression == '-') {
            if (last == 0) token[pos++] = *expression++;
            else {
              expression++;
              if (*expression == 0)
                throw InvalidOpException(this, "Invalid regular expression");
              if (last > *expression)
                throw InvalidOpException(this, "Invalid regular expression");
              last++;
              while (last <= *expression) {
                token[pos++] = last++;
                }
              last = 0;
              expression++;
              }
            }
          else {
            last = *expression++;
            token[pos++] = last;
            }
          }
        token[pos] = 0;
        if (*expression == ']') expression++;
        }
      else if (*expression == '*')
        throw InvalidOpException(this, "Invalid regular expression");
      else if (*expression == '+')
        throw InvalidOpException(this, "Invalid regular expression");
      else if (*expression == '?')
        throw InvalidOpException(this, "Invalid regular expression");
      else if (*expression == '{')
        throw InvalidOpException(this, "Invalid regular expression");
      else if (*expression == '}')
        throw InvalidOpException(this, "Invalid regular expression");
      else if (*expression == ']')
        throw InvalidOpException(this, "Invalid regular expression");
      else {
        token[0] = *expression++;
        token[1] = 0;
        tokenType = TOKEN_LITERAL;
        min = 1;
        max = 1;
        }
      if (*expression == '*') {
        if (tokenType == TOKEN_START ||
            tokenType == TOKEN_VAR_START)
          throw InvalidOpException(this, "Invalid regular expression");
        min = 0;
        max = 0x0fffffff;
        expression++;
        }
      else if (*expression == '+') {
        if (tokenType == TOKEN_START ||
            tokenType == TOKEN_VAR_START)
          throw InvalidOpException(this, "Invalid regular expression");
        min = 1;
        max = 0x0fffffff;
        expression++;
        }
      else if (*expression == '?') {
        if (tokenType == TOKEN_START ||
            tokenType == TOKEN_VAR_START)
          throw InvalidOpException(this, "Invalid regular expression");
        min = 0;
        max = 1;
        expression++;
        }
      else if (*expression == '{') {
        expression++;
        pos = 0;
        min = 0;
        max = 0;
        if (*expression != ',') {
          while (*expression >= '0' && *expression <= '9')
            number[pos++] = *expression++;
          number[pos] = 0;
          min = Convert::ToUInt32((char*)number);
          max = min;
          if (*expression == ',') {
            expression++;
            max = 0x0fffffff;
            }
          }
        else {
          min = 0;
          expression++;
          }
        if (*expression != '}') {
          pos = 0;
          while (*expression >= '0' && *expression <= '9')
            number[pos++] = *expression++;
          number[pos] = 0;
          max = Convert::ToUInt32((char*)number);
          }
        if (*expression != '}')
          throw InvalidOpException(this, "Invalid regular expression");
        expression++;
        }
// printf("%d  %20s %4d %4d\n",tokenType, token, min, max);
      tokenTypes->Add(tokenType);
      tokens->Add(new String((char*)token));
      minCount->Add(min);
      maxCount->Add(max);
      }
    }
  
  RegEx::~RegEx() {
    if (tokens->Count() > 0) {
      delete(tokens->First());
      while (!tokens->AtEnd()) delete(tokens->Next());
      }
    delete(tokens);
    delete(minCount);
    delete(maxCount);
    delete(tokenTypes);
    }
  
  Boolean RegEx::matchNext(const char* input, UInt32 inPos, UInt32 tokenPos) {
    char  buffer[1024];
    Int32 min;
    Int32 max;
    Int32  i,j;
    UInt32 count;
    UInt32 pos;
    UInt32 len;
//printf("token %d of %d\n",tokenPos, tokens->Count());
    if (tokenPos >= tokens->Count()) return true;
    min = minCount->At(tokenPos);
    max = maxCount->At(tokenPos);
    if (inPos+max >= (UInt32)strlen(input)) max = strlen(input) - inPos;
//printf("min = %d, max = %d\n",min, max);
    switch (tokenTypes->At(tokenPos)) {
      case TOKEN_LITERAL:
           if (max < min) return false;
           if (max == 0 && min == 0) return matchNext(input, inPos, tokenPos+1);
           for (i=max; i>=min; i--) {
             count = 0;
             pos = inPos;
             for (j=0; j<i; j++)
               if (input[pos] == tokens->At(tokenPos)->CharAt(0)) {
                 count++;
                 pos++;
                 }
             if (count == (UInt32)i) {
               if (matchNext(input, inPos+count, tokenPos+1)) return true;
               }
             }
           return false;
           break;
      case TOKEN_INCLUDE:
           if (max < min) return false;
           if (max == 0 && min == 0) return matchNext(input, inPos, tokenPos+1);
           for (i=max; i>=min; i--) {
             count = 0;
             pos = inPos;
             for (j=0; j<i; j++)
               if (tokens->At(tokenPos)->Contains(input[pos])) {
                 count++;
                 pos++;
                 }
             if (count == (UInt32)i) {
               if (matchNext(input, inPos+count, tokenPos+1)) return true;
               }
             }
           return false;
           break;
      case TOKEN_EXCLUDE:
           if (max < min) return false;
           if (max == 0 && min == 0) return matchNext(input, inPos, tokenPos+1);
           for (i=max; i>=min; i--) {
             count = 0;
             pos = inPos;
             for (j=0; j<i; j++)
               if (!tokens->At(tokenPos)->Contains(input[pos])) {
                 count++;
                 pos++;
                 }
             if (count == (UInt32)i) {
               if (matchNext(input, inPos+count, tokenPos+1)) return true;
               }
             }
           return false;
           break;
      case TOKEN_ANY:
           if (max < min) return false;
           if (max == 0 && min == 0) return matchNext(input, inPos, tokenPos+1);
           for (i=max; i>=min; i--) {
             if (matchNext(input, inPos+i, tokenPos+1)) return true;
             }
           return false;
           break;
      case TOKEN_START:
           if (inPos == 0) {
             if (matchNext(input, inPos, tokenPos+1)) return true;
             }
           return false;
           break;
      case TOKEN_END:
           if (inPos >= strlen(input)) return true;
           return false;
           break;
      case TOKEN_VAR_START:
           if (varCount < 32) varCount++;
           varStart[varCount-1] = inPos;
           if (matchNext(input, inPos, tokenPos+1)) return true;
           if (varCount > 0) varCount--;
           return false;
           break;
      case TOKEN_VAR_END:
           varEnd[varCount-1] = inPos;
           len = varEnd[varCount-1] - varStart[varCount-1];
           strncpy((char*)buffer, input+varStart[varCount-1], len);
           buffer[len] = 0;
           vars[varCount-1] = String((char*)buffer);
           if (matchNext(input, inPos, tokenPos+1)) return true;
           return false;
           break;
      }
    return false;
    }

  Boolean RegEx::Match(const char* input) {
    UInt32 i;
    varCount = 0;
//printf("regex match function\n");
    for (i=0; i<strlen(input); i++) {
      if (matchNext(input, i, 0)) return true;
      }
    return false;
    }

  Byte    RegEx::VarCount() {
    return varCount;
    }

  String  RegEx::Var(Byte index) {
    if (index > varCount) throw RangeException(this, "No match at index");
    return vars[index];
    }

  }

