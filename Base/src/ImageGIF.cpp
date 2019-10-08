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
  /* ***** Read GIF ***** */
  /* ******************** */
  void Image::readGIF() {
    typedef struct {
      unsigned char red;
      unsigned char green;
      unsigned char blue;
      } COLOR;
    Int32         i;
    UInt32        bpos;
    UInt32        x,y;
    int           backcolor;
    int           flagGCT;
    int           flagColorBits;
    int           flag;
    int           imageLeft;
    int           imageTop;
    UInt32        imageWidth;
    UInt32        imageHeight;
    UInt32        imageLctSize;
    int           imageUseLct;
    LZW*          lzw;
    UInt32        currentCodeSize;
    int           interlaced;
    UInt32        blockSize;
    UInt32        gctcolors;
    int           startingBits;
    unsigned char blockType;
    unsigned char buffer[65536];
    COLOR         gct[256];
    COLOR         lct[256];
    COLOR*        activeCt;
    int           code;
    int           mask;
    int           count;
    int           codeRestart;
    int           codeEnd;
    int           blockCount;
    int           pass;
    int           interval;
  
    width = 0;
    height = 0;
    data = NULL;
  
  
    /* **************************** */
    /* ***** Read file header ***** */
    /* **************************** */
    if (file->ReadByteArray(buffer, 13) < 13) {
      throw InvalidOpException(this, "Could not read gif header");
      }
    width = buffer[6] | (buffer[7] << 8);
    height = buffer[8] | (buffer[9] << 8);
    backcolor = buffer[11];
    flagGCT = (buffer[10] & 0x80) ? -1 : 0;
    flagColorBits = (buffer[10] & 0x07) + 1;
  
    /* ************************************************ */
    /* ***** Read global color table if it exists ***** */
    /* ************************************************ */
    if (flagGCT) {
      gctcolors = 1 << flagColorBits;
      if (file->ReadByteArray(buffer, gctcolors * 3) < gctcolors * 3) {
        throw InvalidOpException(this, "Could not read global color table");
        }
      bpos = 0;
      for (i=0; i<(Int32)gctcolors; i++) {
        gct[i].red = buffer[bpos++];
        gct[i].green = buffer[bpos++];
        gct[i].blue = buffer[bpos++];
        }
      }
  
    /* ************************* */
    /* ***** Prepare image ***** */
    /* ************************* */
    data = (Byte*)malloc(sizeof(Byte*)*width*height*3);
    for (y=0; y<height; y++)
      for (x=0; x<width; x++) {
        data[y*width*3 + x*3 + 0] = gct[backcolor].red;
        data[y*width*3 + x*3 + 1] = gct[backcolor].green;
        data[y*width*3 + x*3 + 2] = gct[backcolor].blue;
        }
  
    width = width;
    height = height;
  
    /* **************************************************** */
    /* ***** Now read and parse data sections of file ***** */
    /* **************************************************** */
    flag = -1;
    while (flag) {
      if (file->ReadByteArray(buffer,1) < 1) {
        delete(file);
        file = NULL;
        free(data);
        throw InvalidOpException(this, "Premature end of file");
        }
      blockType = buffer[0];
      switch (blockType) {
        /* **************************** */
        /* ***** Image descriptor ***** */
        /* **************************** */
        case 0x21:
             if (file->ReadByteArray(buffer, 2) < 2) {
               delete(file);
               file = NULL;
               free(data);
               throw InvalidOpException(this, "Premature end of file");
               }
             blockSize = buffer[1];
             if (file->ReadByteArray(buffer, blockSize) < blockSize) {
               delete(file);
               file = NULL;
               free(data);
               throw InvalidOpException(this, "Premature end of file");
               }
             break;
        case 0x2c:
             if (file->ReadByteArray(buffer, 9) < 9) {
               delete(file);
               file = NULL;
               free(data);
               throw InvalidOpException(this, "Premature end of file");
               }
             imageLeft = buffer[0] | (buffer[1] << 8);
             imageTop = buffer[2] | (buffer[3] << 8);
             imageWidth = buffer[4] | (buffer[5] << 8);
             imageHeight = buffer[6] | (buffer[7] << 8);
             imageLctSize = 2 << (buffer[8] & 0x7);
             imageUseLct = (buffer[8] & 0x80) ? -1 : 0;
             interlaced = (buffer[8] & 0x40) ? -1 : 0;
             if (file->ReadByteArray(buffer, 1) < 1) {
               delete(file);
               file = NULL;
               free(data);
               throw InvalidOpException(this, "Premature end of file");
               }
             activeCt = gct;
             if (imageUseLct) {
               if (file->ReadByteArray(buffer, imageLctSize * 3) < imageLctSize * 3) {
                 delete(file);
                 file = NULL;
                 free(data);
                 throw InvalidOpException(this, "Could not read local color table");
                 }
               bpos = 0;
               for (i=0; i<(Int32)imageLctSize; i++) {
                 lct[i].red = buffer[bpos++];
                 lct[i].green = buffer[bpos++];
                 lct[i].blue = buffer[bpos++];
                 }
               activeCt = lct;
               startingBits = imageLctSize >> 8;
               }
               else startingBits = flagColorBits;
             codeRestart = 1 << startingBits;
             codeEnd = (1 << startingBits) + 1;
             lzw = new LZW(startingBits, 12);
             currentCodeSize = Convert::ToBitSize(lzw->DictPosition());
             if (currentCodeSize > 12) currentCodeSize = 12;
             x = 0;
             y = 0;
             mask = 256;
             blockSize = 0;
             bpos = 0;
             blockCount = 0;
             pass = (interlaced) ? 1 : 4;
             interval = (interlaced) ? 8 : 1;
             while (y < imageHeight) {
               code = 0;
               for (i=0; i<(Int32)currentCodeSize; i++) {
                 if (mask > 128) {
                   bpos++;
                   mask = 1;
                   if (bpos >= blockSize) {
                     if (file->ReadByteArray(buffer, 1) < 1) {
                       delete(file);
                       file = NULL;
                       free(data);
                       throw InvalidOpException(this, "Premature end of file");
                       }
                     blockSize = buffer[0];
                     if (blockSize != 0) {
                       if (file->ReadByteArray(buffer, blockSize) < blockSize) {
                         delete(file);
                         file = NULL;
                         free(data);
                         throw InvalidOpException(this, "Premature end of file");
                         }
                       bpos = 0;
                       blockCount++;
                       } else printf("Read zero size block\r\n");
                     }
                   }
                 if (buffer[bpos] & mask) code |= (1 << i);
                 mask <<= 1;
                 }
               if (code == codeRestart || code == codeEnd) {
                 count = 1;
                 lzw->Output()[0] = code;
                 }
               else count = lzw->Decompress(code);
               currentCodeSize = Convert::ToBitSize(lzw->DictPosition());
               if (currentCodeSize > 12) currentCodeSize = 12;
               for (i=count-1; i>=0; i--) {
                 if (lzw->Output()[i] == codeRestart) {
                   lzw->InitDictionary();
                   currentCodeSize = Convert::ToBitSize(lzw->DictPosition());
                   if (currentCodeSize > 12) currentCodeSize = 12;
                   }
                 else if (lzw->Output()[i] == codeEnd) {
                   y = imageHeight;
                   i = -1;
                   }
                 else {
                   data[(imageTop+y)*width*3 +
                            (imageLeft+x)*3 + 0] = activeCt[lzw->Output()[i]].red;
                   data[(imageTop+y)*width*3 +
                            (imageLeft+x)*3 + 1] = activeCt[lzw->Output()[i]].green;
                   data[(imageTop+y)*width*3 +
                            (imageLeft+x)*3 + 2] = activeCt[lzw->Output()[i]].blue;
                   x++;
                   if (x >= imageWidth) {
                     y += interval;
                     if (y >= imageHeight && pass < 4) {
                       pass++;
                       if (pass == 2) { y = 4; interval = 8; }
                       if (pass == 3) { y = 2; interval = 4; }
                       if (pass == 4) { y = 1; interval = 2; }
                       }
                     x = 0;
                     }
                   }
                 }
               }
               delete(lzw);
             break;
        /* *********************** */
        /* ***** End of file ***** */
        /* *********************** */
        case 0x3b:
             flag = 0;
             break;
        case 0x00:
             break;
        default  :
             if (file->ReadByteArray(buffer,1) < 1) {
               delete(file);
               file = NULL;
               free(data);
               throw InvalidOpException(this, "Premature end of file");
               }
             blockSize = buffer[0];
             if (file->ReadByteArray(buffer,blockSize) < blockSize) {
               delete(file);
               file = NULL;
               free(data);
               throw InvalidOpException(this, "Premature end of file");
               }
             break;
        }
      }
    }

  }

