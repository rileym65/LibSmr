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

#include "SmrAppFramework.h"

namespace SmrFramework {

  Form::Form(Control* parent, int x, int y, int w, int h) :
         Panel(parent, x, y, w, h) {
    this->objectType = (char*)"Form";
    this->cursor = XCreateFontCursor(this->display, 2);
    XDefineCursor(this->display, this->window, this->cursor);
    }

  Form::~Form() {
    }

  String Form::Text(const char* text) {
    XTextProperty txt;
    Control::Text(text);
    if (text != NULL) {
      XStringListToTextProperty((char**)&text, 1, &txt);
      XSetWMName(display, window, &txt);
      XFlush(display);
      }
    return Control::Text();
    }

  String Form::Text() {
    return Control::Text();
    }

  } 
