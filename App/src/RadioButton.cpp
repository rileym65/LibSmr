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

  RadioButton::RadioButton(Control* parent, int x, int y, int w, int h) :
         CheckBox(parent, x, y, w, h) {
    this->objectType = (char*)"RadioButton";
    }

  RadioButton::~RadioButton() {
    }

  int RadioButton::IsRadioButton() {
    return -1;
    }

  void RadioButton::MouseUpEvent(int x, int y, int button) {
    Panel* p;
    changeCheck(-1);
    Control::MouseUpEvent(x, y, button);
    if (parent != NULL && parent->IsContainer()) {
      p = (Panel*)parent;
      p->ProcessRadioButtons(this);
      }
    }

  void RadioButton::Redraw() {
    GC            gc;
    XGCValues     values;
#ifndef USEXFT
    XFontStruct*  font;
#endif
    unsigned long mask;
    int           xoffset;
    int           yoffset;
    int           fontHeight;
    if (!visible) return;
    XSetWindowBackground(display, window, backgroundColor);
    XClearWindow(display, window);
#ifdef USEXFT
    XGlyphInfo ginfo;
    values.line_width = 1;
    values.foreground = foregroundColor;
    values.background = backgroundColor;
    mask = GCLineWidth | GCForeground | GCBackground;
    gc = XCreateGC(display, window, mask, &values);
    fontHeight = font->Ascent() + font->Descent();
    XDrawArc(display, window, gc, 0, (height-fontHeight) / 2,
                   fontHeight, fontHeight, 0, 360 * 64);
    if (checked) {
      XFillArc(display, window, gc, 1, (height-fontHeight) / 2 + 1,
                     fontHeight - 2, fontHeight - 2, 0, 360 * 64);
      }
    if (text.Length() != 0) {
      XftTextExtents8(display, font->FontObject(), (const FcChar8*)text.AsCharArray(), text.Length(), &ginfo);
      yoffset = (height - fontHeight) / 2 + font->Descent();
      xoffset = fontHeight * 1.5;
      XftDrawString8(xftdrawable, &xftcolor, font->FontObject(), ginfo.x + xoffset,ginfo.y + yoffset,
                    (const FcChar8*)text.AsCharArray(),text.Length());
      }
    XFreeGC(display, gc);
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
    fontHeight = font->ascent + font->descent;
    XDrawArc(display, window, gc, 0, (height-fontHeight) / 2,
                   fontHeight, fontHeight, 0, 360 * 64);
    if (checked) {
      XFillArc(display, window, gc, 1, (height-fontHeight) / 2 + 1,
                     fontHeight - 2, fontHeight - 2, 0, 360 * 64);
      }
    if (text.Length() != 0) {
      yoffset = (height - fontHeight) / 2 + font->descent;
      xoffset = fontHeight * 1.5;
      XDrawImageString(display,window,gc,xoffset,height-yoffset,
                       text.AsCharArray(),text.Length());
      }
    XFreeGC(display, gc);
    XFreeFont(display, font);
#endif
    XFlush(display);
    }

  } 

