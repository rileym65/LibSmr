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

UInt32 bytecount;

  BitStream::BitStream(Stream* s) : Object() {
    objectType = (char*)"BitStream";
    stream = s;
    deleteStreamOnDispose = true;
    littleEndian = true;
    fillLowToHigh = true;
    mask = 0;
    written = false;
    currentByte = 0;
    }
  
  BitStream::~BitStream() {
    if (written && mask != 0) stream->Write(currentByte);
    if (deleteStreamOnDispose) delete(stream);
    }
  

  bool BitStream::DeleteStreamOnDispose() {
    return deleteStreamOnDispose;
    }

  bool BitStream::DeleteStreamOnDispose(bool b) {
    deleteStreamOnDispose = b;
    return b;
    }

  bool BitStream::FillLowToHigh() {
    return fillLowToHigh;
    }

  bool BitStream::FillLowToHigh(bool b) {
    fillLowToHigh = b;
    return fillLowToHigh;
    }

  bool BitStream::LittleEndian() {
    return littleEndian;
    }

  bool BitStream::LittleEndian(bool b) {
    littleEndian = b;
    return b;
    }

  void BitStream::Flush() {
    mask = 0;
    if (written) stream->Write(currentByte);
    currentByte = 0;
    }

  Dword BitStream::Read(Byte bits) {
    UInt32 i;
    Dword  ret;
    Dword  bit;
    ret = 0;
    bit = 1;
    for (i=0; i<bits; i++) {
      if (mask == 0) {
        currentByte = stream->ReadByte();
        mask = (littleEndian) ? 1 : 128;
        }
      if (!fillLowToHigh) ret <<= 1;
      if (currentByte & mask) ret |= (fillLowToHigh) ? bit : 1;
      if (littleEndian) mask <<= 1; else mask >>= 1;
      bit <<= 1;
      }
    return ret;
    }

  void BitStream::Write(Byte bits, Dword value) {
    UInt32 i;
    written = true;
    for (i=0; i<bits; i++) {
      if (mask == 0) mask = (littleEndian) ? 1 : 128;
      if (value & 1) currentByte |= mask;
      if (littleEndian) mask <<= 1; else mask >>= 1;
      value >>= 1;
      if (mask == 0) {
        stream->Write(currentByte);
        currentByte = 0;
        }
      }
    }

  }

