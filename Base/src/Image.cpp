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

  Image::Image(int w, int h) {
    width = w;
    height = h;
    data = (Byte*)malloc(width * height * 3);
    if (data == NULL)
      throw InvalidOpException(this, "Could not allocate memory for image");
    pathname = NULL;
    file = NULL;
    }

  Image::Image(const char* path) {
    if (path== NULL) throw InvalidOpException(this, "Pathname is NULL");
    if (strlen(path) < 1) throw InvalidOpException(this, "Empty pathname");
    objectType = (char*)"Image";
    pathname = (char*)malloc(strlen(path) + 1);
    strcpy(pathname, path);
    file = new FileStream(pathname, Stream::ReadOnly);
    loadImage();
    delete(file);
    file = NULL;
    }

  Image::Image(Stream* stream) {
    if (stream== NULL) throw InvalidOpException(this, "Stream is NULL");
    objectType = (char*)"Image";
    pathname = NULL;
    file = stream;
    loadImage();
    }
  
  Image::~Image() {
    if (pathname != NULL) free(pathname);
    if (data != NULL) free(data);
    if (file != NULL) delete(file);
    }

  void Image::DrawingColor(Byte r, Byte g, Byte b) {
    red = r;
    green = g;
    blue = b;
    }

  void Image::DrawLine(UInt32 x1, UInt32 y1, UInt32 x2, UInt32 y2) {
    UInt32 i;
    UInt32 pos;
    UInt32 dist;
    UInt32 line;
    UInt32 step;
    UInt32 range;
    if (x1 < 0 || x1 >= width) throw RangeException(this, "x out of range");
    if (y1 < 0 || y1 >= height) throw RangeException(this, "y out of range");
    if (x2 < 0 || x2 >= width) throw RangeException(this, "x out of range");
    if (y2 < 0 || y2 >= height) throw RangeException(this, "y out of range");
    dist = width * 3;
    if (x1 == x2) {
      if (y1 > y2) Utils::Swap(&y1, &y2);
      pos = y1 * dist + x1 * 3;
      for (i=y1; i<=y2; i++) {
        data[pos+0] = red;
        data[pos+1] = green;
        data[pos+2] = blue;
        pos += dist;
        }
      return;
      }
    if (y1 == y2) {
      if (x1 > x2) Utils::Swap(&x1, &x2);
      pos = y1 * dist + x1 * 3;
      for (i=x1; i<=x2; i++) {
        data[pos++] = red;
        data[pos++] = green;
        data[pos++] = blue;
        }
      return;
      }
    if (Math::Abs((Int32)x2-(Int32)x1) >= Math::Abs((Int32)y2-(Int32)y1)) {
      line = 0;
      pos = y1 * dist + x1 * 3;
      step = Math::Abs((Int32)y2-(Int32)y1);
      range = Math::Abs((Int32)x2-(Int32)x1);
      while (x1 != x2) {
        data[pos+0] = red;
        data[pos+1] = green;
        data[pos+2] = blue;
        pos += (x1 < x2) ? 3 : -3;
        line += step;
        if (line >= range) {
          line -= range;
          pos += (y1 < y2) ? dist : -dist;
          }
        x1 += (x1 < x2) ? 1 : -1;
        }
      }
    else {
      line = 0;
      pos = y1 * dist + x1 * 3;
      step = Math::Abs((Int32)x2-(Int32)x1);
      range = Math::Abs((Int32)y2-(Int32)y1);
      while (y1 != y2) {
        data[pos+0] = red;
        data[pos+1] = green;
        data[pos+2] = blue;
        pos += (y1 < y2) ? dist : -dist;
        line += step;
        if (line >= range) {
          line -= range;
          pos += (x1 < x2) ? 3 : -3;
          }
        y1 += (y1 < y2) ? 1 : -1;
        }
      }
    }

  void Image::DrawPoint(UInt32 x, UInt32 y) {
    if (x < 0 || x >= width) throw RangeException(this, "x out of range");
    if (y < 0 || y >= height) throw RangeException(this, "y out of range");
    data[y*width*3+x*3+0] = red;
    data[y*width*3+x*3+1] = green;
    data[y*width*3+x*3+2] = blue;
    }

  void Image::DrawRectangle(UInt32 x, UInt32 y, UInt32 w, UInt32 h) {
    w += (w > 0) ? -1 : 1;
    h += (h > 0) ? -1 : 1;
    DrawLine(x, y, x+w, y);
    DrawLine(x+w, y, x+w, y+h);
    DrawLine(x+w, y+h, x, y+h);
    DrawLine(x, y+h, x, y);
    }

  void Image::FillRectangle(UInt32 x, UInt32 y, UInt32 w, UInt32 h) {
    UInt32 i;
    w += (w > 0) ? -1 : 1;
    h += (h > 0) ? -1 : 1;
    for (i=y; i<y+h; i++) DrawLine(x, i, x+w, i);
    }

  void Image::loadImage() {
    int i;
    Byte        buffer[10];
    file->ReadByteArray(buffer, 10);
    file->Rewind();
    if (buffer[0] == 'G' && buffer[1] == 'I' && buffer[2] == 'F') {
      readGIF();
      return;
      }
    if (buffer[1] == 'P' && buffer[2] == 'N' && buffer[3] == 'G') {
      readPNG();
      return;
      }
    if (buffer[0] == 'B' && buffer[1] == 'M') {
      readBMP();
      return;
      }
    if (buffer[0] == 0xff && buffer[1] == 0xd8) {
      readJPG();
      return;
      }
    i = strlen(pathname)-4;
    if (i < 0) {
      throw InvalidOpException(this, "Unknown image file");
      }
    if (pathname[i] == '.' &&
        (pathname[i+1] == 'x' || pathname[i+1] == 'X') &&
        (pathname[i+2] == 'p' || pathname[i+2] == 'P') &&
        (pathname[i+3] == 'm' || pathname[i+3] == 'M')) {
      readXPM();
      return;
      }
    if (pathname[i] == '.' &&
        (pathname[i+1] == 'x' || pathname[i+1] == 'X') &&
        (pathname[i+2] == 'b' || pathname[i+2] == 'B') &&
        (pathname[i+3] == 'm' || pathname[i+3] == 'M')) {
      readXBM();
      return;
      }
    throw InvalidOpException(this, "Unknown image file");
    }

  Image* Image::FlipHorizontal() {
    UInt32 x,y;
    Image* ret;
    Byte*  destData;
    UInt32 bytesPerRow;
    ret = new Image(width, height);
    destData = ret->ImageData();
    bytesPerRow = width * 3;
    for (y=0; y<height; y++)
      for (x=0; x<width; x++) {
        destData[y*bytesPerRow+x*3+0] = data[y*bytesPerRow+(width-x)*3+0];
        destData[y*bytesPerRow+x*3+1] = data[y*bytesPerRow+(width-x)*3+1];
        destData[y*bytesPerRow+x*3+2] = data[y*bytesPerRow+(width-x)*3+2];
        }
    return ret;
    }

  Image* Image::FlipVertical() {
    UInt32 x,y;
    Image* ret;
    Byte*  destData;
    UInt32 bytesPerRow;
    ret = new Image(width, height);
    destData = ret->ImageData();
    bytesPerRow = width * 3;
    for (y=0; y<height; y++)
      for (x=0; x<width; x++) {
        destData[y*bytesPerRow+x*3+0] = data[(height-y)*bytesPerRow+x*3+0];
        destData[y*bytesPerRow+x*3+1] = data[(height-y)*bytesPerRow+x*3+1];
        destData[y*bytesPerRow+x*3+2] = data[(height-y)*bytesPerRow+x*3+2];
        }
    return ret;
    }

  Image* Image::FromPalette(Palette* palette) {
    UInt32 x,y;
    Image* ret;
    Byte*  destData;
    UInt32 bytesPerRow;
    UInt32 index;
    ret = new Image(width, height);
    destData = ret->ImageData();
    bytesPerRow = width * 3;
    for (y=0; y<height; y++)
      for (x=0; x<width; x++) {
        index = palette->Lookup(data[y*bytesPerRow+x*3+0],
                                data[y*bytesPerRow+x*3+1],
                                data[y*bytesPerRow+x*3+2]);
        destData[y*bytesPerRow+x*3+0] = palette->Red(index);
        destData[y*bytesPerRow+x*3+1] = palette->Green(index);
        destData[y*bytesPerRow+x*3+2] = palette->Blue(index);
        }
    return ret;
    }

  Palette* Image::GetPalette() {
    return GetPalette(8);
    }

  Palette* Image::GetPalette(Byte bits) {
    UInt32 x,y;
    Palette* ret;
    ret = new Palette();
    ret->SignificantBits(bits);
    for (y=0; y<height; y++)
      for (x=0; x<width; x++)
        ret->Add(data[y*width*3+x*3+0],
                 data[y*width*3+x*3+1],
                 data[y*width*3+x*3+2]);
    return ret;
    }

  Image* Image::Grayscale() {
    UInt32 x,y;
    Image* ret;
    Byte*  destData;
    Double gs;
    UInt32 bytesPerRow;
    ret = new Image(width, height);
    destData = ret->ImageData();
    bytesPerRow = width * 3;
    for (y=0; y<height; y++)
      for (x=0; x<width; x++) {
        gs = (data[y*bytesPerRow+x*3+0] * 0.21) +
             (data[y*bytesPerRow+x*3+1] * 0.72) +
             (data[y*bytesPerRow+x*3+1] * 0.07);
        destData[y*bytesPerRow+x*3+0] = (int)gs;
        destData[y*bytesPerRow+x*3+1] = (int)gs;
        destData[y*bytesPerRow+x*3+2] = (int)gs;
        }
    return ret;
    }

  UInt32 Image::Height() {
    return height;
    }

  Image* Image::Invert() {
    UInt32 x,y;
    Image* ret;
    Byte*  destData;
    UInt32 bytesPerRow;
    ret = new Image(width, height);
    destData = ret->ImageData();
    bytesPerRow = width * 3;
    for (y=0; y<height; y++)
      for (x=0; x<width; x++) {
        destData[y*bytesPerRow+x*3+0] = data[y*bytesPerRow+x*3+0] ^ 255;
        destData[y*bytesPerRow+x*3+1] = data[y*bytesPerRow+x*3+1] ^ 255;
        destData[y*bytesPerRow+x*3+2] = data[y*bytesPerRow+x*3+2] ^ 255;
        }
    return ret;
    }

  Image* Image::Quantize(Byte numBits) {
    UInt32 x,y;
    Image* ret;
    Byte*  destData;
    Byte   mask;
    UInt32 bytesPerRow;
    if (numBits < 0 || numBits > 8)
      throw RangeException(this, "Value out of range");
    mask = 0xff << (8 - numBits);
    ret = new Image(width, height);
    destData = ret->ImageData();
    bytesPerRow = width * 3;
    for (y=0; y<height; y++)
      for (x=0; x<width; x++) {
        destData[y*bytesPerRow+x*3+0] = data[y*bytesPerRow+x*3+0] & mask;
        destData[y*bytesPerRow+x*3+1] = data[y*bytesPerRow+x*3+1] & mask;
        destData[y*bytesPerRow+x*3+2] = data[y*bytesPerRow+x*3+2] & mask;
        }
    return ret;
    }

  Image* Image::Rotate180() {
    UInt32 x,y;
    Image* ret;
    Byte*  destData;
    UInt32 bytesPerRow;
    ret = new Image(width, height);
    destData = ret->ImageData();
    bytesPerRow = width * 3;
    for (y=0; y<height; y++)
      for (x=0; x<width; x++) {
        destData[y*bytesPerRow+x*3+0] = data[(height-y)*bytesPerRow+(width-x)*3+0];
        destData[y*bytesPerRow+x*3+1] = data[(height-y)*bytesPerRow+(width-x)*3+1];
        destData[y*bytesPerRow+x*3+2] = data[(height-y)*bytesPerRow+(width-x)*3+2];
        }
    return ret;
    }

  Image* Image::RotateClockwise() {
    UInt32 x,y;
    Image* ret;
    Byte*  destData;
    UInt32 srcBytes, dstBytes;
    ret = new Image(height, width);
    destData = ret->ImageData();
    srcBytes = width * 3;
    dstBytes = height * 3;
    for (y=0; y<width; y++)
      for (x=0; x<height; x++) {
        destData[y*dstBytes+x*3+0] = data[(height-x)*srcBytes+y*3+0];
        destData[y*dstBytes+x*3+1] = data[(height-x)*srcBytes+y*3+1];
        destData[y*dstBytes+x*3+2] = data[(height-x)*srcBytes+y*3+2];
        }
    return ret;
    }

  Image* Image::RotateCounterClockwise() {
    UInt32 x,y;
    Image* ret;
    Byte*  destData;
    UInt32 srcBytes, dstBytes;
    ret = new Image(height, width);
    destData = ret->ImageData();
    srcBytes = width * 3;
    dstBytes = height * 3;
    for (y=0; y<width; y++)
      for (x=0; x<height; x++) {
        destData[y*dstBytes+x*3+0] = data[x*srcBytes+(width-y)*3+0];
        destData[y*dstBytes+x*3+1] = data[x*srcBytes+(width-y)*3+1];
        destData[y*dstBytes+x*3+2] = data[x*srcBytes+(width-y)*3+2];
        }
    return ret;
    }

  Image* Image::Scale(Double percent) {
    UInt32 newWidth;
    UInt32 newHeight;
    percent /= 100;
    newWidth = width * percent;
    newHeight = height * percent;
    return Scale(newWidth, newHeight);
    }

  Image* Image::Scale(UInt32 w, UInt32 h) {
    Double    xscale;
    Double    yscale;
    Image*    ret;
    UInt32    i;
    UInt32    x,y;
    UInt32    sx,sy;
    UInt32    rw,rh;
    UInt32    smax, dmax;
    UInt32    sd, dd;
    Byte*     dstData;
    if (w > 0 && h <= 0) {
      xscale = (Double)w / (Double)width;
      yscale = xscale;
      }
    if (w <= 0 && h > 0) {
      yscale = (Double)h / (Double)height;
      xscale = yscale;
      }
    if (w > 0 && h > 0) {
      xscale = (Double)w / (Double)width;
      yscale = (Double)h / (Double)height;
      }
    rw = width * xscale;
    rh = height * yscale;
    smax = width * height * 3;
    dmax = (int)rw * (int)rh * 3;
    ret = new Image(rw, rh);
    dstData = ret->ImageData();
    for (y=0; y<rh; y++)
      for (x=0; x<rw; x++) {
        sx = x / xscale;
        sy = y / yscale;
        for (i=0; i<3; i++) {
          dd = y * rw * 3 + x * 3 + i;
          sd = sy * width * 3 + sx * 3 + i;
          if (dd < dmax && sd < smax) {
            dstData[y*rw*3+x*3+i] = data[sy*width*3+sx*3+i];
            }
          }
        }
    return ret;
    }

  Image* Image::SubImage(UInt32 x, UInt32 y, UInt32 w, UInt32 h) {
    UInt32 ix,iy;
    Image* ret;
    Byte*  dstData;
    if (x < 0 || x >= width) throw InvalidOpException(this, "Cooridnate out of range");
    if (y < 0 || y >= height) throw InvalidOpException(this, "Cooridnate out of range");
    if (x+w > width) throw InvalidOpException(this, "Invalid width");
    if (y+h > height) throw InvalidOpException(this, "Invalid height");
    ret = new Image(w, h);
    dstData = ret->ImageData();
    for (iy=0; iy<h; iy++)
      for (ix=0; ix<w; ix++) {
        dstData[iy*w*3+ix*3+0] = data[(y+iy)*width*3+(x+ix)*3+0];
        dstData[iy*w*3+ix*3+1] = data[(y+iy)*width*3+(x+ix)*3+1];
        dstData[iy*w*3+ix*3+2] = data[(y+iy)*width*3+(x+ix)*3+2];
        }
    return ret;
    }

  Image* Image::Transform(Line src, Line dst) {
    UInt32 x,y;
    UInt32 sx,sy;
    Image* ret;
    Byte*  destData;
    UInt32 bytesPerRow;
    Double distance;
    Double fraction;
    Point  srcPoint;
    ret = new Image(width, height);
    destData = ret->ImageData();
    bytesPerRow = width * 3;
    for (y=0; y<height; y++) {
      for (x=0; x<width; x++) {
        dst.PointToRef(Point(x, y), &distance, &fraction);
        srcPoint = src.RefToPoint(distance, fraction);
        if (srcPoint.X() < 0 || srcPoint.X() >= width ||
            srcPoint.Y() < 0 || srcPoint.Y() >= height) {
          destData[y*bytesPerRow+x*3+0] = 0;
          destData[y*bytesPerRow+x*3+1] = 0;
          destData[y*bytesPerRow+x*3+2] = 0;
          }
        else {
          sx = Convert::ToUInt32(srcPoint.X());
          sy = Convert::ToUInt32(srcPoint.Y());
          destData[y*bytesPerRow+x*3+0] = data[sy*bytesPerRow+sx*3+0];
          destData[y*bytesPerRow+x*3+1] = data[sy*bytesPerRow+sx*3+1];
          destData[y*bytesPerRow+x*3+2] = data[sy*bytesPerRow+sx*3+2];
          }
        }
      }
    return ret;
    }

  UInt32 Image::Width() {
    return width;
    }

  Byte* Image::ImageData() {
    return data;
    }

  }

