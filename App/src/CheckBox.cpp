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

  CheckBox::CheckBox(Control* parent, int x, int y, int w, int h) :
         Control(parent, x, y, w, h) {
    checkChangedHandler = NULL;
    this->checked = false;
    this->objectType = (char*)"CheckBox";
    XSetWindowBorderWidth(display, window, 0);
    this->borderWidth = 0;
    if (parent != NULL) BackgroundColor(parent->BackgroundColor());
    XFlush(display);
    }

  CheckBox::~CheckBox() {
    if (checkChangedHandler != NULL) delete(checkChangedHandler);
    }

  void CheckBox::changeCheck(Boolean checked) {
    Boolean oldChecked;
    oldChecked = this->checked;
    this->checked = checked;
    Redraw();
    if (this->checked != oldChecked) {
      if (this->checkChangedHandler != NULL) {
        checkChangedHandler->Call(this, MouseEventArgs(0, 0, 0, 0));
        }
      }
    }

  Boolean CheckBox::Checked() {
    return checked;
    }

  Boolean CheckBox::Checked(Boolean checked) {
    changeCheck(checked);
    this->Redraw();
    return this->checked;
    }

  void CheckBox::OnCheckChanged(EventHandler* handler) {
    if (checkChangedHandler != NULL) delete(checkChangedHandler);
    checkChangedHandler = handler;
    }
    
  void CheckBox::MouseUpEvent(int x, int y, int button) {
    changeCheck((checked) ? false : true);
    Control::MouseUpEvent(x, y, button);
    }

  void CheckBox::Redraw() {
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
    XDrawRectangle(display, window, gc, 0, (height-fontHeight) / 2,
                   fontHeight, fontHeight);
    if (checked) {
      XFillRectangle(display, window, gc, 2, (height-fontHeight) / 2 + 2,
                     fontHeight - 3, fontHeight - 3);
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
    XDrawRectangle(display, window, gc, 0, (height-fontHeight) / 2,
                   fontHeight, fontHeight);
    if (checked) {
      XFillRectangle(display, window, gc, 2, (height-fontHeight) / 2 + 2,
                     fontHeight - 3, fontHeight - 3);
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

