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

  Label::Label(Control* parent, int x, int y, int w, int h, const char* text) :
         Control(parent, x, y, w, h) {
    align = CENTER;
    XSetWindowBorderWidth(display, window, 0);
    this->borderWidth = 0;
    this->objectType = (char*)"Label";
    Control::Text(text);
    if (parent != NULL) BackgroundColor(parent->BackgroundColor());
    Visible(true);
    XFlush(display);
    }

  Label::~Label() {
    }

  Byte Label::Align() {
    return align;
    }

  Byte Label::Align(Byte b) {
    align = b;
    Redraw();
    return align;
    }

  void Label::Redraw() {
#ifdef USEXFT
#else
    GC            gc;
    XGCValues     values;
    XFontStruct*  font;
    unsigned long mask;
#endif
    int           xoffset;
    int           yoffset;
    if (!visible) return;
    XClearWindow(display, window);
    if (enabled) {
#ifdef USEXFT
      XftTextExtents8(display, xftfont, (const FcChar8*)text.AsCharArray(), text.Length(), &ginfo);
      yoffset = height / 2 - ginfo.height / 2;
      if (align == CENTER)
        xoffset = width / 2 - ginfo.width / 2;
      if (align == LEFT)
        xoffset = 5;
      if (align == RIGHT)
        xoffset = (width - ginfo.width) - 5;
      XftDrawString8(xftdrawable, &xftcolor, xftfont, ginfo.x+xoffset,ginfo.y+yoffset,
                    (const FcChar8*)text.AsCharArray(),text.Length());
      
  #else
      if (this->font.Length() == 0) font = XLoadQueryFont(display, "fixed");
        else font = XLoadQueryFont(display, this->font.AsCharArray());
      if (font == NULL) font = XLoadQueryFont(display, "fixed");
      values.line_width = 1;
      values.foreground = foregroundColor;
      values.background = backgroundColor;
      values.font = font->fid;
      mask = GCLineWidth | GCForeground | GCBackground | GCFont;
      gc = XCreateGC(display, window, mask, &values);
      yoffset = (height - (font->ascent + font->descent)) / 2 + font->descent;
      if (align == CENTER)
        xoffset = (width - XTextWidth(font, text.AsCharArray(), text.Length())) / 2;
      if (align == LEFT)
        xoffset = 5;
      if (align == RIGHT)
        xoffset = (width - XTextWidth(font, text.AsCharArray(), text.Length())) - 5;
      XDrawString(display,window,gc, xoffset,height-yoffset,text.AsCharArray(),text.Length());
      XFreeGC(display, gc);
      XFreeFont(display, font);
#endif
      }
    }

  } 

