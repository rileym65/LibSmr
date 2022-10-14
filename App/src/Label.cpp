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
    this->textOffsetX = 0;
    this->textOffsetY = 0;
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
    unsigned long mask;
#endif
    int           xoffset;
    int           yoffset;
    if (!visible) return;
    XClearWindow(display, window);
    if (enabled) {
#ifdef USEXFT
      XGlyphInfo ginfo;
      XftTextExtents8(display, font->FontObject(), (const FcChar8*)text.AsCharArray(), text.Length(), &ginfo);
      yoffset = height / 2 - ginfo.height / 2;
      if (align == CENTER)
        xoffset = width / 2 - ginfo.width / 2;
      if (align == LEFT)
        xoffset = 5;
      if (align == RIGHT)
        xoffset = (width - ginfo.width) - 5;
      XftDrawString8(xftdrawable, &xftcolor, font->FontObject(), ginfo.x+xoffset+textOffsetX,ginfo.y+yoffset+textOffsetY,
                    (const FcChar8*)text.AsCharArray(),text.Length());
      
  #else
      values.line_width = 1;
      values.foreground = foregroundColor;
      values.background = backgroundColor;
      values.font = font->FontObject()->fid;
      mask = GCLineWidth | GCForeground | GCBackground | GCFont;
      gc = XCreateGC(display, window, mask, &values);
      yoffset = (height - (font->Ascent() + font->Descent())) / 2 + font->Descent();
      if (align == CENTER)
        xoffset = (width - XTextWidth(font->FontObject(), text.AsCharArray(), text.Length())) / 2;
      if (align == LEFT)
        xoffset = 5;
      if (align == RIGHT)
        xoffset = (width - XTextWidth(font->FontObject(), text.AsCharArray(), text.Length())) - 5;
      XDrawString(display,window,gc, xoffset+textOffsetX,height-yoffset+textOffsetY,text.AsCharArray(),text.Length());
      XFreeGC(display, gc);
#endif
      }
    }

  Int32 Label::TextOffsetX() {
    return textOffsetX;
    }

  Int32 Label::TextOffsetX(Int32 i) {
    textOffsetX = i;
    Redraw();
    return textOffsetX;
    }

  Int32 Label::TextOffsetY() {
    return textOffsetY;
    }

  Int32 Label::TextOffsetY(Int32 i) {
    textOffsetY = i;
    Redraw();
    return textOffsetY;
    }

  } 

