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

  Toggle::Toggle(Control* parent, int x, int y, int w, int h) :
         Button(parent, x, y, w, h) {
    this->objectType = (char*)"Toggle";
    }

  Toggle::~Toggle() {
    }

  void Toggle::MouseDownEvent(int x, int y, int button) {
    Control::MouseDownEvent(x, y, button);
    }
    
  void Toggle::MouseUpEvent(int x, int y, int button) {
    XSetWindowAttributes attr;
    unsigned long mask;
    pressed = (pressed) ? 0 : -1;
    if (pressed) {
      if (pressedImage != NULL) {
        mask = CWBackPixmap;
        attr.background_pixmap = pressedPixmap;
        XChangeWindowAttributes(display, window, mask, &attr);
        XClearWindow(display, window);
        }
      }
    else {
      if (backgroundImage != NULL) {
        mask = CWBackPixmap;
        attr.background_pixmap = backgroundPixmap;
        XChangeWindowAttributes(display, window, mask, &attr);
        XClearWindow(display, window);
        }
      }
    Redraw();
    XFlush(display);
    Control::MouseUpEvent(x, y, button);
    if (pressed) {
      if (this->pressHandler != NULL) {
        pressHandler->Call(this, MouseEventArgs(x, y, button, 0));
        }
      }
    else {
      if (this->releaseHandler != NULL) {
        releaseHandler->Call(this, MouseEventArgs(x, y, button, 0));
        }
      }
    }

  } 

