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
#include <stdlib.h>
#include <string.h>
#include "SmrAppFramework.h"

namespace SmrFramework {

  PictureBox::PictureBox(Control* parent, int x, int y, int w, int h) :
         Control(parent, x, y, w, h) {
    picture = NULL;
    offsetX = 0;
    offsetY = 0;
    mode = MODE_STANDARD;
    center = false;
    if (parent != NULL) BackgroundColor(parent->BackgroundColor());
    }

  PictureBox::~PictureBox() {
    }

  Boolean PictureBox::Center() {
    return center;
    }

  Boolean PictureBox::Center(Boolean b) {
    center = b;
    XClearWindow(display, window);
    Redraw();
    return center;
    }

  Byte PictureBox::Mode() {
    return mode;
    }

  Byte PictureBox::Mode(Byte m) {
    if (m > 3) throw RangeException(this, "Mode out of range");
    mode = m;
    XClearWindow(display, window);
    Redraw();
    return mode;
    }

  UInt32 PictureBox::OffsetX() {
    return offsetX;
    }

  UInt32 PictureBox::OffsetX(UInt32 x) {
    offsetX = x;
    return offsetX;
    }

  UInt32 PictureBox::OffsetY() {
    return offsetY;
    }

  UInt32 PictureBox::OffsetY(UInt32 y) {
    offsetY = y;
    return offsetY;
    }

  Image* PictureBox::Picture() {
    return picture;
    }

  Image* PictureBox::Picture(Image* i) {
    picture = i;
    XClearWindow(display, window);
    Redraw();
    return picture;
    }

  void PictureBox::Redraw() {
    Graphics *g;
    Image*    transformed;
    Double    ratio;
    UInt32    offX;
    UInt32    offY;
    if (!visible) return;
    if (picture != NULL) {
      g = GetGraphics();

        if (mode == MODE_STANDARD) {
          if (center) {
            offX = (width - picture->Width()) / 2;
            offY = (height - picture->Height()) / 2;
            }
          else {
            offX = offsetX;
            offY = offsetY;
            }
          g->DrawImage(offX,offY, picture);
          }
        if (mode == MODE_FIT) {
          if (width != picture->Width() || height != picture->Height()) {
            ratio = (double)picture->Height() / (double)height;
            if ((picture->Width() / ratio) > width) {
              transformed = picture->Scale(width, 0);
              offX = 0;
              offY = (height - transformed->Height()) / 2;
              }
            else {
              transformed = picture->Scale(0, height);
              offY = 0;
              offX = (width - transformed->Width()) / 2;
              }
            g->DrawImage(offX,offY, transformed);
            delete(transformed);
            }
          else g->DrawImage(offsetX,offsetY, picture);
          }
        if (mode == MODE_STRETCH) {
          if (width != picture->Width() || height != picture->Height()) {
            transformed = picture->Scale(width, height);
            g->DrawImage(0, 0, transformed);
            delete(transformed);
            }
          else g->DrawImage(offsetX,offsetY, picture);
          }
      delete(g);
      }
    }

  } 

