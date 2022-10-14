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

  GroupBox::GroupBox(Control* parent, int x, int y, int w, int h) :
         Panel(parent, x, y, w, h) {
    this->BorderWidth(0);
    this->objectType = (char*)"GroupBox";
    if (parent != NULL) this->BackgroundColor(parent->BackgroundColor());
    }

  GroupBox::~GroupBox() {
    }

  void GroupBox::Redraw() {
    GC            gc;
    XGCValues     values;
    unsigned long mask;
#ifndef USEXFT
    int           fontHeight;
    int           xoffset;
#endif
    int           yoffset;
    if (!visible) return;
    XSetWindowBackground(display, window, backgroundColor);
    XClearWindow(display, window);
#ifdef USEXFT
    XGlyphInfo ginfo;
    XftTextExtents8(display, font->FontObject(), (const FcChar8*)text.AsCharArray(), text.Length(), &ginfo);
    values.line_width = 1;
    values.foreground = foregroundColor;
    values.background = backgroundColor;
    mask = GCLineWidth | GCForeground | GCBackground;
    gc = XCreateGC(display, window, mask, &values);
    yoffset = ginfo.height / 2;
    XDrawRectangle(display, window, gc, 0, yoffset, width - 1, height - yoffset - 1);
    XClearArea(display, window, 3,0,ginfo.width+6,ginfo.height,false);
    XftDrawString8(xftdrawable, &xftcolor, font->FontObject(), ginfo.x + 5,ginfo.y,
                  (const FcChar8*)text.AsCharArray(),text.Length());
#else
    values.line_width = 1;
    values.foreground = foregroundColor;
    values.background = backgroundColor;
    values.font = font->FontObject()->fid;
    mask = GCLineWidth | GCForeground | GCBackground | GCFont;
    gc = XCreateGC(display, window, mask, &values);
    fontHeight = font->Ascent() + font->Descent();
    yoffset = fontHeight / 2;
    XDrawRectangle(display, window, gc, 0, yoffset, width - 1,
      height - yoffset - 1);
    if (text.Length() != 0) {
      yoffset = fontHeight;
      xoffset = fontHeight;
      XDrawImageString(display,window,gc, xoffset,yoffset,text.AsCharArray(),text.Length());
      }
#endif
    XFreeGC(display, gc);
    }

  } 

