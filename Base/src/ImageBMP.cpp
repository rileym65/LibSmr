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
  /* ***** Read BMP ***** */
  /* ******************** */
  void Image::readBMP() {
    typedef struct {
      Byte          red;
      Byte          green;
      Byte          blue;
      Byte          alpha;
      } ColorRGBA;
    int            i;
    Int32          x,y;
    unsigned int   mask;
    int            clr;
    int            bpos;
    Byte           buffer[65536];
    unsigned int   offset;
    unsigned int   headerSize;
    int            bits;
    int            compression;
    int            numColors;
    UInt32         rowSize;
    ColorRGBA      colormap[256];
    FileStream*    file;
    width = 0;
    height = 0;
    data = NULL;
    file = new FileStream(pathname);
  
    /* **************************** */
    /* ***** Read file header ***** */
    /* **************************** */
    if (file->ReadByteArray(buffer, 14) < 14) {
      throw InvalidOpException(this, "Could not read bmp header");
      }
    offset = buffer[10] | (buffer[11] << 8) | (buffer[12] << 16) |
             (buffer[13] << 24);
  
    /* *********************************** */
    /* ***** Read bitmap info header ***** */
    /* *********************************** */
    if (file->ReadByteArray(buffer, 4) < 4) {
      delete(file);
      throw InvalidOpException(this, "Could not read bmp info header");
      }
    headerSize = buffer[0] + (buffer[1] << 8) + (buffer[2] << 16) + (buffer[3] << 24) - 4;
    if (file->ReadByteArray(buffer+4,headerSize) < headerSize) {
      delete(file);
      throw InvalidOpException(this, "Could not read bmp header");
      }
    width = buffer[4] | (buffer[5] << 8) | (buffer[6] << 16) | (buffer[7] << 24);
    height = buffer[8] | (buffer[9] << 8) | (buffer[10] << 16) | (buffer[11] << 24);
    bits = buffer[14] | (buffer[15] << 8);
    compression = buffer[16] | (buffer[17] << 8) | (buffer[18] << 16) | (buffer[19] << 24);
    numColors = buffer[32] | (buffer[33] << 8) | (buffer[34] << 16) | (buffer[35] << 24);
    if (numColors == 0) numColors = 1 << bits;
    rowSize = ((bits * width + 31) / 32 * 4);
    if (compression != 0) {
      delete(file);
      throw InvalidOpException(this, "bmp compression mode not supported");
      }
    if (bits != 1 && bits != 2 && bits != 4 && bits != 8 && bits != 24) {
      delete(file);
      throw InvalidOpException(this, "bmp bitsize not supported");
      }
  
    /* ********************************************* */
    /* ***** Read colormap when 8 bits or less ***** */
    /* ********************************************* */
    if (numColors <= 256) {
      for (i=0; i<numColors; i++) {
        if (file->ReadByteArray(buffer, 4) < 4) {
          delete(file);
          throw InvalidOpException(this, "bmp could not read colormap entry");
          }
        colormap[i].red = buffer[2];
        colormap[i].green = buffer[1];
        colormap[i].blue = buffer[0];
        colormap[i].alpha = buffer[3];
        }
      }
  
    /* *************************** */
    /* ***** Read image data ***** */
    /* *************************** */
    file->Position(offset);
    data = (Byte*)malloc(sizeof(Byte)*width*height*3);
//    for (y=0; y<height; y++) {
    for (y=height-1; y>= 0; y--) {
      if (file->ReadByteArray(buffer, rowSize) < rowSize) {
        delete(file);
        free(data);
        data = NULL;
        throw InvalidOpException(this, "Failed to read image data");
        }
      /* ************************* */
      /* ***** 2 color image ***** */
      /* ************************* */
      if (bits == 1) {
        x = 0;
        mask = 128;
        bpos = y * width * 3;
        while (x*8 < (Int32)width) {
          clr = (buffer[x] & mask) ? 1 : 0;
          data[bpos++] = colormap[clr].red;
          data[bpos++] = colormap[clr].green;
          data[bpos++] = colormap[clr].blue;
          mask >>= 1;
          if (mask == 0) {
            x++;
            mask = 128;
            }
          }
        }
      /* ************************* */
      /* ***** 4 color image ***** */
      /* ************************* */
      if (bits == 2) {
        x = 0;
        mask = 0xc0;
        bpos = y * width * 3;
        while (x*4 < (Int32)width) {
          if (mask == 0xc0) clr = (buffer[x] & mask) >> 6;
          if (mask == 0x30) clr = (buffer[x] & mask) >> 4;
          if (mask == 0x0c) clr = (buffer[x] & mask) >> 2;
          if (mask == 0x03) clr = (buffer[x] & mask);
          data[bpos++] = colormap[clr].red;
          data[bpos++] = colormap[clr].green;
          data[bpos++] = colormap[clr].blue;
          mask >>= 2;
          if (mask == 0) {
            x++;
            mask = 0xc0;
            }
          }
        }
      /* ************************** */
      /* ***** 16 color image ***** */
      /* ************************** */
      if (bits == 4) {
        x = 0;
        mask = 0xf0;
        bpos = y * width * 3;
        while (x*2 < (Int32)width) {
          if (mask > 0x0f) clr = (buffer[x] & mask) >> 4;
            else clr = buffer[x] & mask;
          data[bpos++] = colormap[clr].red;
          data[bpos++] = colormap[clr].green;
          data[bpos++] = colormap[clr].blue;
          mask >>= 4;
          if (mask == 0) {
            x++;
            mask = 0xf0;
            }
          }
        }
      /* *************************** */
      /* ***** 256 color image ***** */
      /* *************************** */
      if (bits == 8) {
        for (x=0; x<(Int32)width; x++) {
          data[y*width*3+x*3+0] = colormap[buffer[x]].red;
          data[y*width*3+x*3+1] = colormap[buffer[x]].green;
          data[y*width*3+x*3+2] = colormap[buffer[x]].blue;
          }
        }
      /* ****************************** */
      /* ***** 24 bit color image ***** */
      /* ****************************** */
      if (bits == 24) {
        for (x=0; x<(Int32)width; x++) {
          data[y*width*3+x*3+0] = buffer[x*3 + 2];
          data[y*width*3+x*3+1] = buffer[x*3 + 1];
          data[y*width*3+x*3+2] = buffer[x*3 + 0];
          }
        }
      }
    delete(file);
    }

  void Image::SaveAsBMP(const char* pathname) {
    UInt32 i;
    Byte buffer[54];
    Byte* row;
    UInt32 rowSize;
    File* file;
    Byte  bits;
//    UInt32 pixels;
    UInt32 imageSize;
    Int32  x,y;
    UInt32 imageStart;
    Palette* palette;
    Byte     color;
    palette = GetPalette(8);
    file = File::OpenForWriting(pathname);
//    pixels = width * height;
    if (palette->Count() <= 2) bits = 1;
    else if (palette->Count() <= 4) bits = 2;
    else if (palette->Count() <= 16) bits = 4;
    else if (palette->Count() <= 256) bits = 8;
    else bits = 24;
    rowSize = ((bits * width + 31) / 32) * 4;
    imageSize = rowSize * height;
    imageStart = 54;
    if (bits <= 8) imageStart += palette->Count() * 4;
    /* *************************** */
    /* ***** Fill out header ***** */
    /* *************************** */
    buffer[0] = 'B'; buffer[1] = 'M';
    buffer[2] = ((imageSize + 54) & 0xff);
    buffer[3] = (((imageSize + 54) >> 8) & 0xff);
    buffer[4] = (((imageSize + 54) >> 16) & 0xff);
    buffer[5] = (((imageSize + 54) >> 24) & 0xff);
    buffer[6] = 0; buffer[7] = 0;
    buffer[8] = 0; buffer[9] = 0;
    buffer[10] = imageStart & 0xff;
    buffer[11] = (imageStart >> 8) & 0xff;
    buffer[12] = (imageStart >> 16) & 0xff;
    buffer[13] = (imageStart >> 24) & 0xff;
    /* ******************************** */
    /* ***** Fill out info header ***** */
    /* ******************************** */
    buffer[14] = 40; buffer[15] = 0; buffer[16] = 0; buffer[17] = 0;
    buffer[18] = width & 0xff;
    buffer[19] = (width >> 8) & 0xff;
    buffer[20] = (width >> 16) & 0xff;
    buffer[21] = (width >> 24) & 0xff;
    buffer[22] = height & 0xff;
    buffer[23] = (height >> 8) & 0xff;
    buffer[24] = (height >> 16) & 0xff;
    buffer[25] = (height >> 24) & 0xff;
    buffer[26] = 1; buffer[27] = 0;
    buffer[28] = bits; buffer[29] = 0;
    buffer[30] = 0; buffer[31] = 0; buffer[32] = 0; buffer[33] = 0;
    buffer[34] = 0; buffer[35] = 0; buffer[36] = 0; buffer[37] = 0;
    buffer[38] = 72; buffer[39] = 0; buffer[40] = 0; buffer[41] = 0;
    buffer[42] = 72; buffer[43] = 0; buffer[44] = 0; buffer[45] = 0;
    if (bits <= 8) {
      buffer[46] = palette->Count() & 0xff;
      buffer[47] = (palette->Count() >> 8)  & 0xff;
      buffer[48] = (palette->Count() >> 16)  & 0xff;
      buffer[49] = (palette->Count() >> 24)  & 0xff;
      }
    else {
      buffer[46] = 0;
      buffer[47] = 0;
      buffer[48] = 0;
      buffer[49] = 0;
      }
    buffer[50] = 0; buffer[51] = 0; buffer[52] = 0; buffer[53] = 0;
    file->Write(buffer, 54);

    /* ********************************************** */
    /* ***** Write colormap when 8 bits or less ***** */
    /* ********************************************** */
    if (palette->Count() <= 256) {
      for (i=0; i<palette->Count(); i++) {
        buffer[0] = palette->Blue(i);
        buffer[1] = palette->Green(i);
        buffer[2] = palette->Red(i);
        buffer[3] = 0;
        file->Write(buffer, 4);
        }
      }

    /* ****************************** */
    /* ***** Write 1 bit image ***** */
    /* ****************************** */
    if (bits == 1) {
      row = (Byte*)malloc(rowSize);
      for (y=height-1; y>=0; y--) {
        for (x=0; x<(Int32)width; x++) {
          if ((x % 8) == 0) row[x >> 3] = 0;
          color = palette->Lookup(data[y*width*3+x*3+0],
                                  data[y*width*3+x*3+1],
                                  data[y*width*3+x*3+2]);
          if (color) row[x>>3] |= (1 << (7-(x % 8)));
          }
        file->Write(row, rowSize);
        }
      free(row);
      }

    /* ****************************** */
    /* ***** Write 2 bit image ***** */
    /* ****************************** */
    if (bits == 2) {
      row = (Byte*)malloc(rowSize);
      for (y=height-1; y>=0; y--) {
        for (x=0; x<(Int32)width; x++) {
          color = palette->Lookup(data[y*width*3+x*3+0],
                                  data[y*width*3+x*3+1],
                                  data[y*width*3+x*3+2]);
          switch (x % 4) {
            case 0: row[x >> 2] = (color << 6); break;
            case 1: row[x >> 2] |= (color << 4); break;
            case 2: row[x >> 2] |= (color << 2); break;
            case 3: row[x >> 2] |= color; break;
            }
          }
        file->Write(row, rowSize);
        }
      free(row);
      }

    /* ****************************** */
    /* ***** Write 4 bit image ***** */
    /* ****************************** */
    if (bits == 4) {
      row = (Byte*)malloc(rowSize);
      for (y=height-1; y>=0; y--) {
        for (x=0; x<(Int32)width; x++) {
          color = palette->Lookup(data[y*width*3+x*3+0],
                                  data[y*width*3+x*3+1],
                                  data[y*width*3+x*3+2]);
          if ((x & 1) == 0) row[x >> 1] = (color << 4);
            else row[x >> 1] |= color;
          }
        file->Write(row, rowSize);
        }
      free(row);
      }

    /* ****************************** */
    /* ***** Write 8 bit image ***** */
    /* ****************************** */
    if (bits == 8) {
      row = (Byte*)malloc(rowSize);
      for (y=height-1; y>=0; y--) {
        for (x=0; x<(Int32)width; x++) {
          row[x] = palette->Lookup(data[y*width*3+x*3+0],
                                   data[y*width*3+x*3+1],
                                   data[y*width*3+x*3+2]);
          }
        file->Write(row, rowSize);
        }
      free(row);
      }

    /* ****************************** */
    /* ***** Write 24 bit image ***** */
    /* ****************************** */
    if (bits == 24) {
      row = (Byte*)malloc(rowSize);
      for (y=height-1; y>=0; y--) {
        for (x=0; x<(Int32)width; x++) {
          row[x*3+0] = data[y*width*3+x*3+2];
          row[x*3+1] = data[y*width*3+x*3+1];
          row[x*3+2] = data[y*width*3+x*3+0];
          }
        file->Write(row, rowSize);
        }
      free(row);
      }
    delete(file);
    }

  }

