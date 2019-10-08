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
#include "string.h"
#include "SmrFramework.h"

namespace SmrFramework {

  /* ******************** */
  /* ***** Read XPM ***** */
  /* ******************** */
  void Image::readXPM() {
    typedef struct {
      unsigned char tag[11];
      unsigned char red;
      unsigned char green;
      unsigned char blue;
      } COLOR;
    int            i,j;
    UInt32         x,y;
    int            colors;
    int            charsPerPixel;
    char           code;
    char           clr[256];
    char           pixel[11];
    unsigned char *pos;
    unsigned int   intColor;
    int            r1;
    int            r2;
    int            r3;
    COLOR         *palette;
    COLOR          clrentry;
    String         line;
    StreamReader*  file;
    Byte*          chars;
//    file = new StreamReader(new MemoryStream(pathname));
    file = new StreamReader(this->file);
    this->file = NULL;
    width = 0;
    height = 0;
    while (width == 0) {
      if (file->EndOfStream()) throw InvalidOpException(this, "File not a valid xpm");
      line = file->ReadLine();
      chars = (Byte*)line.AsCharArray();
      pos = chars;
      while (*pos > 0 && *pos <= ' ') pos++;
      if (*pos == '"') {
        pos++;
        i = 0;
        while (pos[i] != 0 && pos[i] != '"') i++;
        pos[i] = 0;
        i = sscanf((char*)pos," %d %d %d %d", &width, &height, &colors, &charsPerPixel);
        }
      }
    palette = (COLOR*)malloc(sizeof(COLOR) * colors);
    if (palette == NULL) {
      throw MemoryException(this, "Could not allocate memory for colors");
      }
    i = 0;
    while (i < colors) {
      if (file->EndOfStream()) {
        free(palette);
        throw InvalidOpException(this, "Unexpected end of file");
        }
      line = file->ReadLine();
      chars = (Byte*)line.AsCharArray();
      pos = chars;
      while (*pos > 0 && *pos <= ' ') pos++;
      if (*pos == '"') {
        pos++;
        j = 0;
        while (pos[j] != 0 && pos[j] != '"') j++;
        pos[j] = 0;
        strncpy((char*)palette[i].tag,(char*)pos,charsPerPixel);
        palette[i].tag[charsPerPixel] = 0;
        pos += charsPerPixel;
        sscanf((char*)pos," %c %s",&code, clr);
        if (code == 'c' || code == 'C') {
          if (clr[0] == '#') {
            if (strlen(clr) > 8) {
              sscanf(clr+1,"%4x",&intColor);
              palette[i].red = intColor & 0xff;
              sscanf(clr+5,"%4x",&intColor);
              palette[i].green = intColor & 0xff;
              sscanf(clr+9,"%4x",&intColor);
              palette[i].blue = intColor & 0xff;
              }
            else {
              sscanf(clr+1,"%x",&intColor);
              palette[i].red = (intColor >> 16) & 0xff;
              palette[i].green = (intColor >> 8) & 0xff;
              palette[i].blue = intColor & 0xff;
              }
            }
          else {
            palette[i].red = 1;
            palette[i].green = 1;
            palette[i].blue = 1;
            }
          }
        i++;
        }
      }
    j = -1;
    while (j) {
      j = 0;
      for (i=0; i<colors-1; i++)
        if (strcmp((const char*)palette[i].tag,(const char*)palette[i+1].tag) > 0) {
          clrentry = palette[i];
          palette[i] = palette[i+1];
          palette[i+1] = clrentry;
          j = -1;
          }
      }
    data = (unsigned char*)malloc(sizeof(unsigned char)*width*height*3);
    y = 0;
    while (y < height) {
      if (file->EndOfStream()) {
        free(palette);
        free(data);
        data = NULL;
        throw InvalidOpException(this, "Unexpected end of file");
        }
      line = file->ReadLine();
      chars = (Byte*)line.AsCharArray();
      pos = chars;
      while (*pos > 0 && *pos <= ' ') pos++;
      if (*pos == '"') {
        pos++;
        j = 0;
        while (pos[j] != 0 && pos[j] != '"') j++;
        pos[j] = 0;
        x = 0;
        while (x < width) {
          strncpy(pixel, (const char*)pos, charsPerPixel);
          pixel[charsPerPixel] = 0;
          pos += charsPerPixel;
          j = -1;
          r1 = 0;
          r2 = colors - 1;
          while (j == -1) {
            r3 = r1 + ((r2 - r1) >> 1);
            i = strcmp((const char*)pixel, (const char*)palette[r3].tag);
            if (i == 0) j = r3;
            else if (i < 0) {
              r2 = r3;
              }
            else if (i > 0) {
              r1 = r3;
              }
            if (j < 0 && (r2 - r1) < 2) {
              if (strcmp((const char*)pixel, (const char*)palette[r1].tag) == 0) j = r1;
              else if (strcmp((const char*)pixel, (const char*)palette[r2].tag) == 0) j = r2;
              else j = -2;
              }
            }
          if (j >= 0) {
            data[y*width*3+x*3+0] = palette[j].red;
            data[y*width*3+x*3+1] = palette[j].green;
            data[y*width*3+x*3+2] = palette[j].blue;
            }  else printf("Color lookup failure: %s\r\n",pixel);
          x++;
          }
        y++;
        }
      }
  
    delete(file);
    free(palette);
    }

  char baseXpm(Byte digit) {
    if (digit == 0) return '.';
    if (digit == 1) return '#';
    if (digit >= 2 && digit <= 27) return 'a' + (digit - 2);
    if (digit >= 28 && digit <= 53) return 'A' + (digit - 28);
    if (digit >= 54 && digit <= 63) return '0' + (digit - 54);
    return ' ';
    }

  /* *********************** */
  /* ***** Save as XPM ***** */
  /* *********************** */
  void Image::SaveAsXPM(const char* pathname) {
    Palette* palette;
    StreamWriter* file;
    char* buffer;
    Byte  bits;
    int   i;
    char  number[4];
    UInt32 color;
    Int32 x,y;
    palette = GetPalette();
    if (palette->Count() > 256) {
      delete(palette);
      throw InvalidOpException(this, "Color count exceeds 256");
      }
    file = new StreamWriter(pathname);
    buffer = (char*)malloc(width * 2 + 3);
    file->WriteLine("/* XPM */");
    sprintf(buffer,"static char *%s[] = {",pathname);
    file->WriteLine(buffer);
    file->WriteLine("/* width height num_colors chars_per_pixel */");
    bits = (palette->Count() <= 63) ? 1 : 2;
    sprintf(buffer,"\"  %4d  %4d      %3d            %d\",",width,height,palette->Count(),bits);
    file->WriteLine(buffer);
    file->WriteLine("/* colors */");
    for (i=0; i<(Int32)palette->Count(); i++) {
      buffer[0] = '\"';
      if (bits == 1) {
        buffer[1] = baseXpm(i);
        buffer[2] = 0;
        }
      else {
        buffer[1] = baseXpm(i / 64);
        buffer[2] = baseXpm(i % 64);
        buffer[3] = 0;
        }
      strcat(buffer," c #");
      sprintf(number,"%02x",palette->Red(i));
      strcat(buffer,number);
      sprintf(number,"%02x",palette->Green(i));
      strcat(buffer,number);
      sprintf(number,"%02x",palette->Blue(i));
      strcat(buffer,number);
      strcat(buffer,"\",");
      file->WriteLine(buffer);
      }
    file->WriteLine("/* pixels */");
    for (y=0; y<(Int32)height; y++) {
      strcpy(buffer,"\"");
      for (x=0; x<(Int32)width; x++) {
        color = palette->Lookup( data[y*width*3+x*3+0],
                                 data[y*width*3+x*3+1],
                                 data[y*width*3+x*3+2]);
        if (bits == 1) {
          number[0] = baseXpm(color);
          number[1] = 0;
          }
        else {
          number[0] = baseXpm(color / 64);
          number[1] = baseXpm(color % 64);
          number[2] = 0;
          }
        strcat(buffer, number);
        }
      strcat(buffer,(y != (Int32)height-1) ? "\"," : "\"");
      file->WriteLine(buffer);
      }
    file->WriteLine("};");
    free(buffer);
    delete(file);
    }
  }

