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

#define MAX_CHUNK_SIZE 65536

  UInt32 _png_startingY[] = { 0, 0, 4, 0, 2, 0, 1, 0 };
  UInt32 _png_startingX[] = { 0, 4, 0, 2, 0, 1, 0, 0 };
  UInt32 _png_intervalY[] = { 8, 8, 8, 4, 4, 2, 2, 1 };
  UInt32 _png_intervalX[] = { 8, 8, 4, 4, 2, 2, 1, 1 };

  Int32 _paethPredictor(int left,int above,int upperLeft) {
    int pa,pb,pc;
    pa = abs(above - upperLeft);
    pb = abs(left  - upperLeft);
    pc = abs(left  - upperLeft + above - upperLeft);
    if (pa <=pb && pa <= pc) return left;
    if (pb <= pc) return above;
    return upperLeft;
  }

  Dword _GetDWord(Byte* buffer,UInt32 pos,UInt32 endian) {
    Dword ret;
    if (endian == 0) {
      ret = buffer[pos] |
            (buffer[pos+1] << 8) |
            (buffer[pos+2] << 16) |
            (buffer[pos+3] << 24);
      }
    else {
      ret = (buffer[pos] << 24) |
            (buffer[pos+1] << 16) |
            (buffer[pos+2] << 8) |
            buffer[pos+3];
      }
    return ret;
    }

  MemoryStream* _ReadImageData(Stream* file) {
    Dword          chunkSize;
    Dword          chunkType;
    Dword          position;
    Byte*          data;
    UInt32         dataSize;
    Byte buffer[8];
    position = file->Position();
    file->ReadByteArray(buffer, 8);
    chunkSize = _GetDWord(buffer,0,1);
    chunkType = _GetDWord(buffer,4,1);
    chunkType &= 0xdfdfdfdf;
    data = NULL;
    dataSize = 0;
    while (chunkType == 0x49444154) {
      if (data == NULL) data = (Byte*)malloc(chunkSize);
      else data = (Byte*)realloc(data,dataSize+chunkSize);
      file->ReadByteArray(data+dataSize, chunkSize);
      dataSize += chunkSize;
      file->ReadByteArray(buffer, 4);
      position = file->Position();
      file->ReadByteArray(buffer, 8);
      chunkSize = _GetDWord(buffer,0,1);
      chunkType = _GetDWord(buffer,4,1);
      chunkType &= 0xdfdfdfdf;
      }
    file->Position(position);
    return new MemoryStream(data, dataSize);
    }

  /* ******************** */
  /* ***** Read PNG ***** */
  /* ******************** */
  void Image::readPNG() {
    typedef struct {
      Byte red;
      Byte green;
      Byte blue;
      } _ColorRGB;
    int            i;
    Byte*          currentRow;
    Byte*          previousRow;
    Flate*         flate;
    _ColorRGB      palette[256];
//    Byte           paletteTrans[256];
    Byte           buffer[MAX_CHUNK_SIZE];
//    Byte           renderingIntent;
    Dword          chunkSize;
    Dword          chunkType;
//    Dword          densityX;
//    Dword          densityY;
//    Dword          gamma;
//    Word           transRed;
//    Word           transGreen;
//    Word           transBlue;
    Word           backRed;
    Word           backGreen;
    Word           backBlue;
    Int32          haveBackground;
    Byte           interlaceMethod;
    Byte           filterMethod;
//    Int32          windowSize;
//    Word           year;
//    Byte           month;
//    Byte           day;
//    Byte           hour;
//    Byte           minute;
//    Byte           second;
//    Byte           unitSpecifier;
    Byte           bitDepth;
    Byte           colorType;
    Byte           compressionMethod;
    Int32          bitsPerPixel;
//    Byte           sigRedBits;
//    Byte           sigGreenBits;
//    Byte           sigBlueBits;
//    Byte           sigAlphaBits;
    Byte           compMethod;
//    Word           checkBits;
    Byte           presetDict;
//    Byte           compLevel;
    Byte           filterOffset;
    Int32          done;
    Int32          x,y;
    Int32          stx;
    Int32          ivx;
    Int32          pass;
    Byte           filter;
    off_t          chunkStart;
    Int32          bytesPerLine;
    Int32          pixelsPerLine;
    Int32          imageOffset;
    BitStream*     bitstream;
    MemoryStream*  imageData;
  
    width = 0;
    height = 0;
    data = NULL;
    haveBackground = 0;
//    renderingIntent = 0xff;
  
    /* ********************************************** */
    /* ***** Read signature and verify png file ***** */
    /* ********************************************** */
    if (file->ReadByteArray(buffer,8) < 8) {
      throw InvalidOpException(this, "PNG signature not found");
      }
    if (buffer[0] != 137 || buffer[1] != 80 ||
        buffer[2] !=  78 || buffer[3] != 71 ||
        buffer[4] !=  13 || buffer[5] != 10 ||
        buffer[6] !=  26 || buffer[7] != 10) {
      throw InvalidOpException(this, "PNG signature not found");
      }
  
    /* ******************************** */
    /* ***** Now loop over chunks ***** */
    /* ******************************** */
    done = 0;
    while (!done) {
      chunkStart = file->Position();
      if (file->ReadByteArray(buffer, 8) < 8) {
        throw InvalidOpException(this, "Premature end of file");
        }
      chunkSize = _GetDWord(buffer,0,1);
      chunkType = _GetDWord(buffer,4,1);
      chunkType &= 0xdfdfdfdf;
      if (chunkSize >= MAX_CHUNK_SIZE) {
        if (file->ReadByteArray(buffer, MAX_CHUNK_SIZE) < MAX_CHUNK_SIZE) {
          throw InvalidOpException(this, "Premature end of file");
          }
        }
      else {
        if (file->ReadByteArray(buffer, chunkSize+4) < chunkSize+4) {
          throw InvalidOpException(this, "Premature end of file");
          }
        }
      switch (chunkType) {
        case 0x49484452:                                          /* IHDR */
             if (chunkSize != 13) {
               throw InvalidOpException(this, "IHDR had wrong size");
               }
             width = _GetDWord(buffer,0,1);
             height = _GetDWord(buffer,4,1);
             bitDepth = buffer[8];
             colorType = buffer[9];
             compressionMethod = buffer[10];
             filterMethod = buffer[11];
             interlaceMethod = buffer[12];
             if (colorType == 0 && bitDepth != 1 && bitDepth != 2 &&
                 bitDepth != 4 && bitDepth != 8 && bitDepth != 16) {
               throw InvalidOpException(this, "Invalid colortype, depth specified");
               }
             if (colorType == 2 && bitDepth != 8 && bitDepth != 16) {
               throw InvalidOpException(this, "Invalid colortype, depth specified");
               }
             if (colorType == 3 && bitDepth != 1 && bitDepth != 2 &&
                 bitDepth != 4 && bitDepth != 8) {
               throw InvalidOpException(this, "Invalid colortype, depth specified");
               }
             if (colorType == 4 && bitDepth != 8 && bitDepth != 16) {
               throw InvalidOpException(this, "Invalid colortype, depth specified");
               }
             if (colorType == 6 && bitDepth != 8 && bitDepth != 16) {
               throw InvalidOpException(this, "Invalid colortype, depth specified");
               }
             if (compressionMethod != 0) {
               throw InvalidOpException(this, "Invalid compression method");
               }
             if (filterMethod != 0) {
               throw InvalidOpException(this, "Invalid filter method");
               }
             bitsPerPixel = bitDepth;
             if (colorType == 2) bitsPerPixel *= 3;
             if (colorType == 4) bitsPerPixel *= 2;
             if (colorType == 6) bitsPerPixel *= 4;
             filterOffset = bitsPerPixel / 8;
             if (filterOffset < 1) filterOffset = 1;
             data = (Byte*)malloc(width * height * 3);
             bytesPerLine = (width * bitsPerPixel + 7) / 8;
             currentRow = (Byte*)malloc(bytesPerLine + 10000);
             previousRow = (Byte*)malloc(bytesPerLine + 10000);
             for (i=0; i<bytesPerLine; i++)
               previousRow[i] = 0;
             break;
        case 0x53424954:                                          /* sBIT */
             switch (colorType) {
               case 0:
//                    sigRedBits = buffer[0];
//                    sigGreenBits = buffer[0];
//                    sigBlueBits = buffer[0];
//                    sigAlphaBits = 0;
                    break;
               case 2:
               case 3:
//                    sigRedBits = buffer[0];
//                    sigGreenBits = buffer[1];
//                    sigBlueBits = buffer[2];
//                    sigAlphaBits = 0;
                    break;
               case 4:
//                    sigRedBits = buffer[0];
//                    sigGreenBits = buffer[0];
//                    sigBlueBits = buffer[0];
//                    sigAlphaBits = buffer[1];
                    break;
               case 6:
//                    sigRedBits = buffer[0];
//                    sigGreenBits = buffer[1];
//                    sigBlueBits = buffer[2];
//                    sigAlphaBits = buffer[3];
                    break;
               }
             break;
        case 0x49444154:                                          /* IDAT */
             file->Position(chunkStart);
             imageData = _ReadImageData(file);
             if (haveBackground) {
               for (y=0; y<(Int32)height; y++)
                 for (x=0; x<(Int32)width; x++) {
                   data[y*width*3+x*3+0] = backRed >> 8;
                   data[y*width*3+x*3+1] = backGreen >> 8;
                   data[y*width*3+x*3+2] = backBlue >> 8;
                 }
               }
             compMethod = buffer[0] & 0xf;
//             windowSize = 1 << (((buffer[0] >> 4) & 0xf) + 8);
//             checkBits = (buffer[1] & 0x1f);
             presetDict = (buffer[1] & 0x20) ? -1 : 0;
//             compLevel = (buffer[1] >> 6) & 0x3;
             if (compMethod != 8) {
               throw InvalidOpException(this, "Compression method other than 8 specified");
               }
             if (presetDict != 0) {
               throw InvalidOpException(this, "Preset dictionary not supported");
               }
             bitstream = new BitStream(imageData);
             bitstream->DeleteStreamOnDispose(true);
             bitstream->LittleEndian(true);
             bitstream->FillLowToHigh(true);
             flate = new Flate(bitstream, 'R');
             pass = 7;
             if (interlaceMethod != 0) pass = 0;
             x = _png_startingX[pass];
             y = _png_startingY[pass];
             stx = _png_startingX[pass];
             ivx = _png_intervalX[pass];
             pixelsPerLine = (width - stx + ivx - 1) / ivx;
             bytesPerLine = (pixelsPerLine * bitsPerPixel + 7) / 8;
             while (y < (Int32)height) {
               flate->Read(&filter, 1);
               flate->Read(currentRow, bytesPerLine);
               if (filter == 1) {
                 for (x=0; x<bytesPerLine; x++)
                   currentRow[x] +=
                     (x < filterOffset) ? 0 : currentRow[(x-filterOffset)];
                 }
               if (filter == 2) {
                 for (x=0; x<bytesPerLine; x++)
                   currentRow[x] += previousRow[x];
                 }
               if (filter == 3) {
                 for (x=0; x<bytesPerLine; x++)
                     currentRow[x] +=
                       (((x>=filterOffset)?currentRow[(x-filterOffset)] : 0) +
                        previousRow[x]) >> 1;
                 }
               if (filter == 4) {
                 for (x=0; x<bytesPerLine; x++)
                     currentRow[x] +=
                       _paethPredictor(
                          ((x>=filterOffset)?currentRow[(x-filterOffset)] : 0),
                          previousRow[x],
                          ((x>=filterOffset)?previousRow[(x-filterOffset)] : 0));
                 }
               memcpy(previousRow, currentRow, bytesPerLine);
               if (bitDepth == 1) {
                 for (x=pixelsPerLine-1; x>= 0; x--) {
                   switch (x % 8) {
                     case 0: currentRow[x] = (currentRow[x/8] >> 7) & 0x1; break;
                     case 1: currentRow[x] = (currentRow[x/8] >> 6) & 0x1; break;
                     case 2: currentRow[x] = (currentRow[x/8] >> 5) & 0x1; break;
                     case 3: currentRow[x] = (currentRow[x/8] >> 4) & 0x1; break;
                     case 4: currentRow[x] = (currentRow[x/8] >> 3) & 0x1; break;
                     case 5: currentRow[x] = (currentRow[x/8] >> 2) & 0x1; break;
                     case 6: currentRow[x] = (currentRow[x/8] >> 1) & 0x1; break;
                     case 7: currentRow[x] = (currentRow[x/8] >> 0) & 0x1; break;
                     }
                   if (colorType == 0) currentRow[x] = (currentRow[x]) ? 0xff : 0;
                   }
                 }
               if (bitDepth == 2) {
                 for (x=pixelsPerLine-1; x>= 0; x--) {
                   switch (x % 4) {
                     case 0: currentRow[x] = (currentRow[x/4] >> 6) & 0x3; break;
                     case 1: currentRow[x] = (currentRow[x/4] >> 4) & 0x3; break;
                     case 2: currentRow[x] = (currentRow[x/4] >> 2) & 0x3; break;
                     case 3: currentRow[x] = (currentRow[x/4] >> 0) & 0x3; break;
                     }
                   if (colorType == 0) currentRow[x] <<= 6;
                   }
                 }
               if (bitDepth == 4) {
                 for (x=pixelsPerLine-1; x>= 0; x--) {
                   currentRow[x] = (x%2) ? (currentRow[x/2] & 0xf):
                                             (currentRow[x/2] >> 4) & 0xf;
                   if (colorType == 0) currentRow[x] <<= 4;
                   }
                 }
               if (bitDepth == 16) {
                 for (x=0; x<pixelsPerLine; x++) {
                   currentRow[x*filterOffset+0] = currentRow[x*filterOffset+0];
                   currentRow[x*filterOffset+1] = currentRow[x*filterOffset+2];
                   currentRow[x*filterOffset+2] = currentRow[x*filterOffset+4];
                   }
                 }
               imageOffset = y * width * 3 + stx * 3;
               for (x=0; x<pixelsPerLine; x++) {
                 for (i=0; i<3; i++) {
                   switch (colorType) {
                     case 0:
                     case 4:
                          data[imageOffset+x*ivx*3+i] =
                            currentRow[x*filterOffset];
                          break;
                     case 3:
                          if (i == 0) {
                            data[imageOffset+x*ivx*3+0] =
                              palette[currentRow[x*filterOffset]].red;
                            data[imageOffset+x*ivx*3+1] =
                              palette[currentRow[x*filterOffset]].green;
                            data[imageOffset+x*ivx*3+2] =
                              palette[currentRow[x*filterOffset]].blue;
                            }
                          break;
                     case 2:
                     case 6:
                          data[imageOffset+x*ivx*3+i] =
                            currentRow[x*filterOffset+i];
                          break;
                     }
                   }
                 }
               y += _png_intervalY[pass];
               if (y >= (Int32)height) {
                 pass++;
                 if (pass < 7) {
                   stx = _png_startingX[pass];
                   ivx = _png_intervalX[pass];
                   x = _png_startingX[pass];
                   y = _png_startingY[pass];
                   pixelsPerLine = (width - stx + ivx - 1) / ivx;
                   bytesPerLine = (pixelsPerLine * bitsPerPixel + 7) / 8;
                   for (i=0; i<bytesPerLine; i++) {
                     previousRow[i] = 0;
                     }
                   }
                 }
               }
  //done = -1;
             delete(flate);
             delete(bitstream);
             break;
        case 0x49454e44:                                          /* IEND */
             done = -1;
             break;
        case 0x50485953:                                          /* PHYS */
//             densityX = _GetDWord(buffer,0,1);
//             densityY = _GetDWord(buffer,4,1);
//             unitSpecifier = buffer[8];
             break;
        case 0x54494d45:                                          /* TIME */
//             year = (buffer[0] << 8) | buffer[1];
//             month = buffer[2];
//             day = buffer[3];
//             hour = buffer[4];
//             minute = buffer[5];
//             second = buffer[6];
             break;
        case 0x49545854:                                          /* ITXT */
             break;
        case 0x47414d41:                                          /* gAMA */
//             gamma = (buffer[0] << 24) | (buffer[1] << 16) |
//                     (buffer[2] << 8) | buffer[3];
             break;
        case 0x54524e53:                                          /* tRNS */
             switch (colorType) {
               case 0:
//                    transRed = (buffer[0] << 8) | buffer[1];
//                    transGreen = (buffer[0] << 8) | buffer[1];
//                    transBlue = (buffer[0] << 8) | buffer[1];
                    break;
               case 2:
//                    transRed = (buffer[0] << 8) | buffer[1];
//                    transGreen = (buffer[2] << 8) | buffer[3];
//                    transBlue = (buffer[4] << 8) | buffer[5];
                    break;
               case 3:
                    i = chunkSize;
                    x = 0;
                    while (x < 256 && i > 0) {
//                      paletteTrans[x] = buffer[x];
                      x++;
                      i--;
                      }
                    break;
               }
             break;
        case 0x424b4744:                                          /* bKGD */
             switch (colorType) {
               case 0:
               case 4:
                    backRed = (buffer[0] << 8) | buffer[1];
                    backGreen = (buffer[0] << 8) | buffer[1];
                    backBlue = (buffer[0] << 8) | buffer[1];
                    break;
               case 2:
               case 6:
                    backRed = (buffer[0] << 8) | buffer[1];
                    backGreen = (buffer[2] << 8) | buffer[3];
                    backBlue = (buffer[4] << 8) | buffer[5];
                    break;
               case 3:
                    backRed = buffer[0];
                    backGreen = buffer[0];
                    backBlue = buffer[0];
                    break;
               }
             haveBackground = -1;
             break;
        case 0x504c5445:                                          /* PLTE */
             i = 0;
             x = chunkSize;
             while (x > 0) {
               palette[i].red = buffer[i*3+0];
               palette[i].green = buffer[i*3+1];
               palette[i].blue = buffer[i*3+2];
//               paletteTrans[i] = 255;
               i++;
               x -= 3;
               }
             break;
        case 0x53524742:                                         /* sRGB */
//             renderingIntent = buffer[0];
             break;
        default:
             printf("Unknown chunk %8x ignored\n",chunkType);
             break;
        }
      }
    }

  }

