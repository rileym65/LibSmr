/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/
#include <stdio.h>
#include <string.h>
#include "string.h"
#include "SmrFramework.h"

namespace SmrFramework {

  Base64::Base64() {
    objectType = (char*)"Base64";
    strcpy(alphabet,"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
    padding = '=';
    }
  
  Base64::~Base64() {
    }

  int Base64::decodeGroup(const char* inp) {
    int i;
    int i1, i2, i3, i4;
    char* pos;
    for (i=0; i<4; i++) outbytes[i] = 0;
    if (strlen(inp) != 4) return 0;
    pos = strchr(alphabet, inp[0]);
    i1 = (pos != NULL) ? pos-alphabet : -1;
    pos = strchr(alphabet, inp[1]);
    i2 = (pos != NULL) ? pos-alphabet : -1;
    if (inp[2] == padding) i3 = 0;
    else {
      pos = strchr(alphabet, inp[2]);
      i3 = (pos != NULL) ? pos-alphabet : -1;
      }
    if (inp[3] == padding) i4 = 0;
    else {
      pos = strchr(alphabet, inp[3]);
      i4 = (pos != NULL) ? pos-alphabet : -1;
      }
    if (i1 < 0 || i2 < 0 || i3 < 0 || i4 < 0) return 0;
    outbytes[0] = (i1 << 2) | ((i2 >> 4) & 0x3);
    if (inp[2] == padding) return 1;
    outbytes[1] = ((i2 & 0xf) << 4) | ((i3 >> 2) & 0xf);
    if (inp[3] == padding) return 2;
    outbytes[2] = ((i3 & 0x3) << 6) | (i4 & 0x3f);
    return 3;
    }

  String Base64::Decode(const char* msg) {
    String ret;
    Byte* temp;
    char  tempstr[5];
    int   pos;
    int   outpos;
    int   len;
    int   count;
    int   i;
    len = strlen(msg);
    outpos = 0;
    pos = 0;
    while (len-pos >= 4) {
      strncpy(tempstr, msg+pos, 4);
      tempstr[4] = 0;
      count = decodeGroup(tempstr);
      if (outpos == 0) temp = (Byte*)malloc(count+1);
        else temp = (Byte*)realloc(temp,outpos + count + 1);
      for (i=0; i< count; i++) temp[outpos++] = outbytes[i];
      pos += 4;
      }
    temp[outpos] = 0;
    if (outpos == 0) return new String("");
    ret = String((const char*)temp, outpos);
    free(temp);
    return ret;
    }

  String Base64::Decode(String* msg) {
    return Decode(msg->AsCharArray());
    }

  char* Base64::encodeGroup(Byte* inp, int l) {
    int i1, i2, i3;
    int len;
    len = l;
printf("len:%d  ",l);
    outchars[2] = padding;
    outchars[3] = padding;
    outchars[4] = 0;
    i1 = inp[0];
    i2 = (len > 1) ? inp[1] : 0;
    i3 = (len > 2) ? inp[2] : 0;
    outchars[0] = alphabet[(i1 >> 2) & 0x3f];
    outchars[1] = alphabet[((i1 & 0x3) << 4) | ((i2 >> 4) & 0xf)];
    if (len == 1) return outchars;
    outchars[2] = alphabet[((i2 & 0xf) << 2) | ((i3 >> 6) & 0x3)];
    if (len == 2) return outchars;
    outchars[3] = alphabet[i3 & 0x3f];
printf("%s\n",outchars);
    return outchars;
    }

  String Base64::Encode(const Byte* msg,int len) {
    int   i;
    char* output;
    Byte  temp[4];
    String ret;
    int   pos;
    int   l;
    if (len < 1) return new String("");
    output = NULL;
    pos = 0;
    while ((len-pos) > 0) {
      temp[0] = 0;
      temp[1] = 0;
      temp[2] = 0;
      if (len-pos >= 3) l = 3;
        else l = len-pos;
      memcpy((char*)temp, (const char*)(msg+pos), 3);
      temp[3] = 0;
      if (output == NULL) {
        output = (char*)malloc(5);
        output[0] = 0;
        }
        else output = (char*)realloc(output,strlen(output)+5);
      strcat(output, (const char*)encodeGroup(temp, l));
      pos += 3;
      }
    ret = String((const char*)output);
    free(output);
    return ret;
    }

  String Base64::Encode(String* msg) {
    return Encode((Byte*)(msg->AsCharArray()),msg->Length());
    }

  void Base64::SetAlphabet(const char* alpha) {
    if (alpha == NULL) throw InvalidOpException(this, "Alphabet was NULL");
    if (strlen(alpha) != 64) throw InvalidOpException(this, "Alphabet wrong length");
    strcpy(alphabet, alpha);
    }

  void Base64::SetPadding(char p) {
    padding = p;
    }

  }
